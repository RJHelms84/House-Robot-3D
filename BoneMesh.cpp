#include "Engine.h"

using namespace std;
using namespace Batman;

namespace Batman
{
	//Constructor; inherits from Mesh
    BoneMesh::BoneMesh() : Mesh()
    {
		//initialise vars
        p_speedAdjust = 1.0f;
        p_firstMesh = 0;
        p_currentTrack = 0;
        p_currentTime = 0;
        p_numAnimationSets = 0;
        p_currentAnimationSet = 0;
        p_maxBones = 0;
        p_boneMatrices = 0;
        p_vertexCount = 0;
        p_faceCount = 0;
    }

	//destructor
    BoneMesh::~BoneMesh(void)
    {
		//release animation controller
        if (p_animController)
        {
            p_animController->Release();
            p_animController=0;
        }

		//release root frame
        if (p_frameRoot)
        {
            //create a mesh heirarchy object
            MeshLoaderCallback memoryAllocator;
			//remove all frames
            D3DXFrameDestroy(p_frameRoot, &memoryAllocator);
            p_frameRoot=0;
        }

		//release all bone matrices
        if (p_boneMatrices)
        {
            delete []p_boneMatrices;
            p_boneMatrices=0;
        }
    }

    //filename helper function
    char* duplicateCharString(const char* charString)
    {
        if (!charString)
            return 0;

        size_t len=strlen(charString) + 1;
        char *newString = new char[len];
        memcpy(newString, charString, len*sizeof(char));

        return newString;
    }
	
	//directory helper function
    string getTheCurrentDirectory()
    {
        int bufferSize=GetCurrentDirectory(0,NULL);
        char *buffer=new char[bufferSize];
        GetCurrentDirectory(bufferSize,buffer);
        std::string directory(buffer);
        delete []buffer;
        return directory;
    }

    bool BoneMesh::Load(std::string filename)
    {
        //create mesh hierarchy object
        MeshLoaderCallback *memoryAllocator=new MeshLoaderCallback;

        //find the file
        std::string currentDirectory = getTheCurrentDirectory();
        std::string xfilePath;
        splitPath( filename, &xfilePath, &filename);
        SetCurrentDirectory(xfilePath.c_str());

        //load mesh file
        HRESULT hr = D3DXLoadMeshHierarchyFromX(
            filename.c_str(),		//filename
            D3DXMESH_MANAGED,		//mesh options (managed mesh)
            g_engine->getDevice(),	//D3D Device
            memoryAllocator,		//hierarchy manager interface (callback)
            NULL,					//user data loader
            &p_frameRoot,			//(out) frame root
            &p_animController);		//(out) animation controller 

		//release memory
        delete memoryAllocator;
        memoryAllocator=0;

        //restore working directory
        SetCurrentDirectory(currentDirectory.c_str());
        
		//error checking
        if (hr != D3D_OK)
        {
            debug << "Error loading bone mesh" << endl;
            return false; 
        }

        //save number of animation sets
        if(p_animController)
            p_numAnimationSets = p_animController->GetMaxNumAnimationSets();

        //are there any bones for skinning?
        if (p_frameRoot)
        {
            //create bone structure (and get count)
            createBoneMatrices((D3DXFRAME_NEW*)p_frameRoot, NULL);

            //create the bone matrices array to hold the final transform
            p_boneMatrices  = new D3DXMATRIX[p_maxBones];
            ZeroMemory(p_boneMatrices, sizeof(D3DXMATRIX)*p_maxBones);
        }

        return true;
    }

    void BoneMesh::createBoneMatrices(D3DXFRAME_NEW *pFrame, LPD3DXMATRIX pParentMatrix)
    {
        //create pointer to the mesh container
        D3DXMESHCONTAINER_NEW* pMesh = (D3DXMESHCONTAINER_NEW*)pFrame->pMeshContainer;

        //does the frame have a mesh?
        if (pMesh)
        {
            p_vertexCount += (int)pMesh->MeshData.pMesh->GetNumVertices();
            p_faceCount += (int)pMesh->MeshData.pMesh->GetNumFaces();

            //save the first mesh in the hierarchy for animation purposes
            if(!p_firstMesh)
                p_firstMesh = pMesh;
            
            //skinning info? then setup the bone matrices
            if(pMesh->pSkinInfo && pMesh->MeshData.pMesh)
            {
                //copy the skin mesh to skin into later
                D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
                if (FAILED(pMesh->MeshData.pMesh->GetDeclaration(Declaration)))
                    return;

                pMesh->MeshData.pMesh->CloneMesh(
                    D3DXMESH_MANAGED, 
                    Declaration, 
                    g_engine->getDevice(), 
                    &pMesh->skinMesh);

                //total bones determines size of bone matrix array
                p_maxBones=max(p_maxBones,(int)pMesh->pSkinInfo->GetNumBones());

                //for each bone calculate its matrix
                for (unsigned int i = 0; i < pMesh->pSkinInfo->GetNumBones(); i++)
                {   
                    //look for the frame's bone
                    D3DXFRAME_NEW* pTempFrame = (D3DXFRAME_NEW*)D3DXFrameFind(
													p_frameRoot, 
													pMesh->pSkinInfo->GetBoneName(i) );

                    //save the name of the bone
                    p_boneNames.push_back((std::string)pMesh->pSkinInfo->GetBoneName(i));

                    //save the combined matrix
                    pMesh->frameCombinedMatrix[i] = &pTempFrame->combinedMatrix;
                }
            }
        }
        
		//These parts of teh code keep calling the function
		//as it navigates the IK hierarchy

        //recursively parse siblings
        if(pFrame->pFrameSibling)
            createBoneMatrices((D3DXFRAME_NEW*)pFrame->pFrameSibling, pParentMatrix);

        //recursively parse children
        if(pFrame->pFrameFirstChild)
        {
            createBoneMatrices((D3DXFRAME_NEW*)pFrame->pFrameFirstChild, 
                &pFrame->combinedMatrix);
        }
    }


    void BoneMesh::Animate(float elapsedTime, D3DXMATRIX *matWorld)
    {
        //adjust animation speed
        elapsedTime /= p_speedAdjust;

        //advance the time and set in the controller
        if (p_animController != NULL)
            p_animController->AdvanceTime(elapsedTime, NULL);

        p_currentTime += elapsedTime;

        //update the model matrices in the hierarchy
        updateFrameMatrices(p_frameRoot, matWorld);

        //if there's a skinned mesh, update the vertices
        D3DXMESHCONTAINER_NEW* pMesh = p_firstMesh;
        if(pMesh && pMesh->pSkinInfo)
        {
            unsigned int Bones = pMesh->pSkinInfo->GetNumBones();

            //transform each bone from bone space into character space
            for (unsigned int i = 0; i < Bones; ++i)
                D3DXMatrixMultiply( &p_boneMatrices[i], 
                                    &pMesh->boneOffsets[i], 
                                    pMesh->frameCombinedMatrix[i]);

            //lock the vertex buffers
            void *srcPtr=0, *destPtr=0;
            pMesh->MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&srcPtr);
            pMesh->skinMesh->LockVertexBuffer(0, (void**)&destPtr);

            //update the skinned mesh via software skinning
            pMesh->pSkinInfo->UpdateSkinnedMesh(p_boneMatrices, NULL, srcPtr, destPtr);

            //unlock the vertex buffers
            pMesh->skinMesh->UnlockVertexBuffer();
            pMesh->MeshData.pMesh->UnlockVertexBuffer();
        }
    }

    void BoneMesh::updateFrameMatrices(D3DXFRAME *frameBase, 
										D3DXMATRIX *parentMatrix)
    {
        D3DXFRAME_NEW *currentFrame = (D3DXFRAME_NEW*)frameBase;

        //if parent matrix exists multiply with new frame matrix
        if (parentMatrix != NULL)
            D3DXMatrixMultiply(&currentFrame->combinedMatrix, 
                &currentFrame->TransformationMatrix, 
                parentMatrix);
        else
            currentFrame->combinedMatrix = currentFrame->TransformationMatrix;

        //recursively update siblings
        if (currentFrame->pFrameSibling != NULL)
            updateFrameMatrices(currentFrame->pFrameSibling, parentMatrix);

        //recursively update children
        if (currentFrame->pFrameFirstChild != NULL)
        {
            updateFrameMatrices( currentFrame->pFrameFirstChild, 
                &currentFrame->combinedMatrix);
        }
    }

    void BoneMesh::Render( Batman::Effect* effect ) 
    {
        if (p_frameRoot)
            drawFrame(p_frameRoot, effect);
    }

    std::vector<std::string> BoneMesh::getAllBoneNames() 
    {
        //fill a vector with bone names in hierarchical order
        std::vector<std::string> names;
        buildBoneList(p_frameRoot, names);
        return names;
    }

    void BoneMesh::buildBoneList(LPD3DXFRAME frame, vector<std::string> &names)
    {
        if (frame->Name)
            names.push_back( (std::string) frame->Name );

        if (frame->pFrameSibling != NULL)
            buildBoneList(frame->pFrameSibling, names);

        if (frame->pFrameFirstChild != NULL)
            buildBoneList(frame->pFrameFirstChild, names);
    }

    void BoneMesh::drawFrame(LPD3DXFRAME frame, Batman::Effect* effect ) 
    {
        //draw mesh containers in this frame
        LPD3DXMESHCONTAINER meshContainer = frame->pMeshContainer;
        while (meshContainer)
        {
            //draw this node/limb
            drawMeshContainer(meshContainer, frame, effect);
           //go to the next node/limb in the tree
            meshContainer = meshContainer->pNextMeshContainer;
        }

        //recursively draw siblings
        if (frame->pFrameSibling != NULL)
            drawFrame(frame->pFrameSibling, effect);

        //recursively draw children
        if (frame->pFrameFirstChild != NULL)
            drawFrame(frame->pFrameFirstChild, effect);
    }

    void BoneMesh::drawMeshContainer(LPD3DXMESHCONTAINER meshContainerBase, 
        LPD3DXFRAME frameBase,  Batman::Effect* effect)
    {
        //create pointer to new frame
        D3DXFRAME_NEW *frame = (D3DXFRAME_NEW*)frameBase;

        //create pointer to new mesh container
        D3DXMESHCONTAINER_NEW *meshContainer = 
            (D3DXMESHCONTAINER_NEW*)meshContainerBase;

        //send the world transform to the shader
        effect->setWorldMatrix( frame->combinedMatrix );

        //iterate through the materials, rendering each subset
        for (unsigned int i = 0; i < meshContainer->NumMaterials; i++)
        {
            //set shader's ambient color to the current material
            //(verify that effect file uses this parameter name)
            D3DXVECTOR4 material;
            material.x = meshContainer->materials[i].Diffuse.r;
            material.y = meshContainer->materials[i].Diffuse.g;
            material.z = meshContainer->materials[i].Diffuse.b;
            material.w = meshContainer->materials[i].Diffuse.a;
            effect->setParam("AmbientColor", material);
            
            //send the texture to the shader
            effect->setParam( "Texture", meshContainer->textures[i] );

            //use either the skinned mesh or the normal one
            LPD3DXMESH mesh=NULL;
            if (meshContainer->pSkinInfo)
                mesh = meshContainer->skinMesh;
            else
                mesh = meshContainer->MeshData.pMesh;

            //draw the subset with passed shader
            effect->Begin();
            mesh->DrawSubset(i);
            effect->End();
        }
    }

    void BoneMesh::setAnimationSet(unsigned int index)
    {
        //the timing with which to merge animations
        //(increasing slows down transition time)
        static float TransitionTime = 0.25f;

        if (index==p_currentAnimationSet)
            return;

        if (index>=p_numAnimationSets)
            index=0;

        //remember current animation
        p_currentAnimationSet=index;

        //get the animation set from the controller
        LPD3DXANIMATIONSET set;
        p_animController->GetAnimationSet(p_currentAnimationSet, &set );    

        //alternate tracks for transitions
        DWORD newTrack = ( p_currentTrack == 0 ? 1 : 0 );

        //assign to the correct track
        p_animController->SetTrackAnimationSet( newTrack, set );
        set->Release();    

        //clear any track events currently assigned 
        p_animController->UnkeyAllTrackEvents( p_currentTrack );
        p_animController->UnkeyAllTrackEvents( newTrack );

        //disable the currently playing track 
        p_animController->KeyTrackEnable( p_currentTrack, FALSE, 
            p_currentTime + TransitionTime );

        //change the speed right away so the animation completes 
        p_animController->KeyTrackSpeed( p_currentTrack, 0.0f, p_currentTime, 
            TransitionTime, D3DXTRANSITION_LINEAR );

        //change the weighting (blending) of the current track 
        p_animController->KeyTrackWeight( p_currentTrack, 0.0f, p_currentTime, 
            TransitionTime, D3DXTRANSITION_LINEAR );

        //enable the new track
        p_animController->SetTrackEnable( newTrack, TRUE );

        //set the speed of the new track
        p_animController->KeyTrackSpeed( newTrack, 1.0f, p_currentTime, 
            TransitionTime, D3DXTRANSITION_LINEAR );

        //change the weighting of the current track (0.0 to 1.0)
        p_animController->KeyTrackWeight( newTrack, 1.0f, p_currentTime, 
            TransitionTime, D3DXTRANSITION_LINEAR );

        //save current track
        p_currentTrack = newTrack;
    }

    void BoneMesh::nextAnimation()
    {    
        unsigned int newAnimationSet = p_currentAnimationSet + 1;
        if (newAnimationSet >= p_numAnimationSets)
            newAnimationSet=0;

        setAnimationSet(newAnimationSet);
    }

    std::string BoneMesh::getAnimationSetName(unsigned int index)
    {
        if (index >= p_numAnimationSets)
            return "Error: No set exists";

        //get the animation set
        LPD3DXANIMATIONSET set;
        p_animController->GetAnimationSet(p_currentAnimationSet, &set);

        std::string nameString(set->GetName());

        set->Release();

        return nameString;
    }

    void BoneMesh::adjustAnimationSpeed(float amount)
    {
        p_speedAdjust += amount;
    }

    std::string BoneMesh::getBoneName(int i)
    {
        if (i < p_maxBones)
        {
            return p_boneNames[i];
        }
        return "invalid index";
    }

}

/**
MeshLoaderCallback class methods are implemented here
(These never really change from the DirectX SDK sample)
**/
HRESULT MeshLoaderCallback::CreateFrame(LPCSTR Name, LPD3DXFRAME *retNewFrame)
{
    *retNewFrame = 0;

    //create a new frame using the derived version of the structure
    D3DXFRAME_NEW *newFrame = new D3DXFRAME_NEW;
    ZeroMemory(newFrame,sizeof(D3DXFRAME_NEW));

    //initialize matrices
    D3DXMatrixIdentity(&newFrame->TransformationMatrix);
    D3DXMatrixIdentity(&newFrame->combinedMatrix);

    //initialize pointers
    newFrame->pMeshContainer = 0;
    newFrame->pFrameSibling = 0;
    newFrame->pFrameFirstChild = 0;

    //use newly created frame
    *retNewFrame = newFrame;
    
    //set the frame name
    if (Name && strlen(Name))
    {
        newFrame->Name = duplicateCharString(Name);    
    }

    return S_OK;
}

HRESULT MeshLoaderCallback::CreateMeshContainer(
    LPCSTR Name,
    CONST D3DXMESHDATA *meshData,
    CONST D3DXMATERIAL *materials,
    CONST D3DXEFFECTINSTANCE *effectInstances,
    DWORD numMaterials,
    CONST DWORD *adjacency,
    LPD3DXSKININFO pSkinInfo,
    LPD3DXMESHCONTAINER* retNewMeshContainer)
{    
    //create a mesh container structure to fill and initilaise to zero values
    D3DXMESHCONTAINER_NEW *newMeshContainer=new D3DXMESHCONTAINER_NEW;
    ZeroMemory(newMeshContainer, sizeof(D3DXMESHCONTAINER_NEW));

    *retNewMeshContainer = 0;

    //save mesh name 
    if (Name && strlen(Name))
    {
        newMeshContainer->Name = duplicateCharString(Name);
    }

    //check the mesh type 
    if (meshData->Type!=D3DXMESHTYPE_MESH)
    {
        DestroyMeshContainer(newMeshContainer);
        return E_FAIL;
    }

    newMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
    
    //create adjacency data, required by ID3DMESH object
    DWORD dwFaces = meshData->pMesh->GetNumFaces();
    newMeshContainer->pAdjacency = new DWORD[dwFaces*3];
    memcpy(newMeshContainer->pAdjacency, adjacency, sizeof(DWORD) * dwFaces*3);
    newMeshContainer->MeshData.pMesh=meshData->pMesh;
    newMeshContainer->MeshData.pMesh->AddRef();

    //create material and texture arrays
    newMeshContainer->NumMaterials = max(numMaterials,1);
    newMeshContainer->materials = new D3DMATERIAL9[newMeshContainer->NumMaterials];
    newMeshContainer->textures  = new LPDIRECT3DTEXTURE9[newMeshContainer->NumMaterials];

    ZeroMemory(newMeshContainer->textures, 
        sizeof(LPDIRECT3DTEXTURE9) * newMeshContainer->NumMaterials);

    if (numMaterials>0)
    {
        // Load all the textures and copy the materials over        
        for(DWORD i = 0; i < numMaterials; ++i)
        {
            newMeshContainer->textures[i] = 0;    
            newMeshContainer->materials[i]=materials[i].MatD3D;

            if(materials[i].pTextureFilename)
            {
                string texturePath(materials[i].pTextureFilename);
                if (findFile(&texturePath))
                {
                    //try to load the texture
                    if(FAILED(D3DXCreateTextureFromFile(g_engine->getDevice(), texturePath.c_str(),
                        &newMeshContainer->textures[i])))
                    {
                        debug << "Could not load texture: " << texturePath << endl;                    
                    }
                }
                else
                {
                    debug << "Could not find texture: " << materials[i].pTextureFilename << endl;                    
                }
            }
        }
    }
    else    
    // make a default material in the case where the mesh did not provide one
    {
        ZeroMemory(&newMeshContainer->materials[0], sizeof( D3DMATERIAL9 ) );
        newMeshContainer->materials[0].Diffuse.r = 0.5f;
        newMeshContainer->materials[0].Diffuse.g = 0.5f;
        newMeshContainer->materials[0].Diffuse.b = 0.5f;
        newMeshContainer->materials[0].Specular = newMeshContainer->materials[0].Diffuse;
        newMeshContainer->textures[0]=0;
    }

    //save skin data 
    if (pSkinInfo)
    {
        // save off the SkinInfo
        newMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();

        //save offset matrices 
        UINT numBones = pSkinInfo->GetNumBones();
        newMeshContainer->boneOffsets = new D3DXMATRIX[numBones];

        // Create the arrays for the bones and the frame matrices
        newMeshContainer->frameCombinedMatrix = new D3DXMATRIX*[numBones];

        // get each of the bone offset matrices so that we don't need to get them later
        for (UINT i = 0; i < numBones; i++)
            newMeshContainer->boneOffsets[i] = 
                *(newMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i));

        debug << "Mesh has skin--bone count: " << numBones << endl;
    }
    else    
    {
        // No skin info so 0 all the pointers
        newMeshContainer->pSkinInfo = 0;
        newMeshContainer->boneOffsets = 0;
        newMeshContainer->skinMesh = 0;
        newMeshContainer->frameCombinedMatrix = 0;
    }

    //does mesh reference an effect file?
    if (effectInstances)
    {
        if (effectInstances->pEffectFilename)
            debug << "Warning: mesh references an effect file" << endl;
    }
    
    // Set the output mesh container pointer to our newly created one
    *retNewMeshContainer = newMeshContainer;    

    return S_OK;
}

HRESULT MeshLoaderCallback::DestroyFrame(LPD3DXFRAME frameToFree) 
{
    //create pointer to this frame
    D3DXFRAME_NEW *frame = (D3DXFRAME_NEW*)frameToFree;
    if (frame->Name) delete []frame->Name;
    delete frame;
    return S_OK; 
}

HRESULT MeshLoaderCallback::DestroyMeshContainer(LPD3DXMESHCONTAINER meshContainerBase)
{
    //create pointer to mesh container
    D3DXMESHCONTAINER_NEW* meshContainer = (D3DXMESHCONTAINER_NEW*)meshContainerBase;
    if (!meshContainer)
        return S_OK;

    //delete name
    if (meshContainer->Name)
    {
        delete []meshContainer->Name;
        meshContainer->Name=0;
    }

    //delete material array
    if (meshContainer->materials)
    {
        delete []meshContainer->materials;
        meshContainer->materials=0;
    }

    //release the textures before deleting the array
    if(meshContainer->textures)
    {
        for(UINT i = 0; i < meshContainer->NumMaterials; ++i)
        {
            if (meshContainer->textures[i])
                meshContainer->textures[i]->Release();
        }
    }

    //delete texture array
    if (meshContainer->textures)
        delete []meshContainer->textures;

    //delete adjacency data
    if (meshContainer->pAdjacency)
        delete []meshContainer->pAdjacency;
    
    //delete bone parts
    if (meshContainer->boneOffsets)
    {
        delete []meshContainer->boneOffsets;
        meshContainer->boneOffsets=0;
    }
    
    //delete frame matrices
    if (meshContainer->frameCombinedMatrix)
    {
        delete []meshContainer->frameCombinedMatrix;
        meshContainer->frameCombinedMatrix=0;
    }
    
    //release skin mesh
    if (meshContainer->skinMesh)
    {
        meshContainer->skinMesh->Release();
        meshContainer->skinMesh=0;
    }
    
    //release the main mesh
    if (meshContainer->MeshData.pMesh)
    {
        meshContainer->MeshData.pMesh->Release();
        meshContainer->MeshData.pMesh=0;
    }
        
    //release skin information
    if (meshContainer->pSkinInfo)
    {
        meshContainer->pSkinInfo->Release();
        meshContainer->pSkinInfo=0;
    }
    
    //delete the mesh container 
    delete meshContainer;
    meshContainer=0;

    return S_OK;
}

void MeshLoaderCallback::splitPath(string& inputPath, 
									string* pathOnly, string* filenameOnly)
{
    string fullPath( inputPath );
    replace( fullPath.begin(), fullPath.end(), '\\', '/');
    string::size_type lastSlashPos = fullPath.find_last_of('/');

    // check for there being no path element in the input
    if (lastSlashPos == string::npos)
    {
        *pathOnly="";
        *filenameOnly = fullPath;
    }
    else {
        if (pathOnly) {
            *pathOnly = fullPath.substr(0, lastSlashPos);
        }
        if (filenameOnly)
        {
            *filenameOnly = fullPath.substr( 
                lastSlashPos + 1,
                fullPath.size() - lastSlashPos - 1 );
        }
    } 
}

bool MeshLoaderCallback::doesFileExist(string &filename)
{
    return (_access(filename.c_str(), 0) != -1);
}

bool MeshLoaderCallback::findFile(string *filename)
{
    if (!filename) return false;

    //since the file was not found, try removing the path
    string pathOnly;
    string filenameOnly;
    splitPath(*filename,&pathOnly,&filenameOnly);

    //is file found in current folder, without the path?
    if (doesFileExist(filenameOnly))
    {
        *filename=filenameOnly;
        return true;
    }

    //not found
    return false;
}    