#pragma once

#include "Engine.h"

//structs for containers and frames
struct D3DXMESHCONTAINER_NEW;
struct D3DXFRAME_NEW;

//callback class for mesh loading
class MeshLoaderCallback;

namespace Batman
{
	class BoneMesh : public Mesh
	{
	private:
		//Direct3D animation objects
		//root frame (e.g. pelvis)
		LPD3DXFRAME p_frameRoot;
		//animation controller
		LPD3DXANIMATIONCONTROLLER p_animController;
		//mesh container pointer
		D3DXMESHCONTAINER_NEW* p_firstMesh;

		//bone data
		//pointer to bone matrices
		D3DXMATRIX *p_boneMatrices;
		//maximum bones
		int p_maxBones;

		//animation variables
		//current animation set
		unsigned int p_currentAnimationSet;
		//number of animation sets
		unsigned int p_numAnimationSets;
		//current track
		unsigned int p_currentTrack;
		//current time
		float p_currentTime;
		//speed adjust
		float p_speedAdjust;

		//new variables
		//vertex count
        int p_vertexCount;
		//face count
        int p_faceCount;
		//vector for bone names
        std::vector<std::string> p_boneNames;

		//update frame matrices
        void updateFrameMatrices(D3DXFRAME *frameBase, 
								D3DXMATRIX *parentMatrix);
		//draw frame
        void drawFrame(LPD3DXFRAME frame, Batman::Effect* effect);
		//draw mesh container
        void drawMeshContainer(LPD3DXMESHCONTAINER meshContainerBase, 
								LPD3DXFRAME frameBase, Batman::Effect* effect);
		//create bone matrices
        void createBoneMatrices(D3DXFRAME_NEW *pFrame, 
								LPD3DXMATRIX pParentMatrix);

		public:
		//constructor / destructor
        BoneMesh();
        virtual ~BoneMesh(void);

		//load file
        bool Load(std::string filename);
        
		//render
        void Render(Batman::Effect* effect);

		//getters and setters - animation set
        void setAnimationSet(unsigned int index);
        unsigned int getCurrentAnimationSet() {return p_currentAnimationSet;}

		//get animation set index
        int getAnimationSetIndex() { 
            return (p_numAnimationSets > 0 ? p_numAnimationSets : 0); 
        }
		
		//get animation set name
        std::string getAnimationSetName(unsigned int index);

		//animate model
        void Animate(float elapsedTime, D3DXMATRIX *matWorld);

		//go to next animation
        void nextAnimation();

		//adjust animation speed
        void adjustAnimationSpeed(float amount);

		//get bone name
        std::string getBoneName(int i);

		//get all bone names
        std::vector<std::string> getAllBoneNames();

		//build list of bones
        void buildBoneList(LPD3DXFRAME frame, std::vector<std::string> &names);

		//getters for root frame and counts
        LPD3DXFRAME getRootFrame() { return p_frameRoot; }
        int getVertexCount() { return p_vertexCount; }
        int getFaceCount() { return p_faceCount; }
        int getBoneCount() { return p_maxBones; }
	};
};

//struct extends MESH CONTAINER
struct D3DXMESHCONTAINER_NEW: public D3DXMESHCONTAINER
{
    IDirect3DTexture9**  textures;				//textures
    D3DMATERIAL9*        materials;				//materials
    ID3DXMesh*           skinMesh;				//skin mesh
    D3DXMATRIX*          boneOffsets;			//bone offsets
    D3DXMATRIX**         frameCombinedMatrix;	//frame combined matrix
};

//struct extends FRAME
struct D3DXFRAME_NEW: public D3DXFRAME
{
    D3DXMATRIX combinedMatrix;			//combined matrix
};

//callback class - extends AllocateHierarchy
class MeshLoaderCallback : public ID3DXAllocateHierarchy
{
public:    
    //creates and initializes a D3DXFRAME object 
    STDMETHOD( CreateFrame )(LPCSTR Name, LPD3DXFRAME *retNewFrame );

    //creates and initializes a D3DXMESHCONTAINER object 
    STDMETHOD( CreateMeshContainer )
       ( LPCSTR Name, 
         const D3DXMESHDATA * meshData, 
         const D3DXMATERIAL * materials, 
         const D3DXEFFECTINSTANCE * effectInstances,
         DWORD numMaterials, 
         const DWORD * adjacency, 
         LPD3DXSKININFO skinInfo, 
         LPD3DXMESHCONTAINER * retNewMeshContainer );

    //releases a D3DXFRAME object
    STDMETHOD( DestroyFrame )(LPD3DXFRAME frameToFree);

    //releases a D3DXMESHCONTAINER object
    STDMETHOD( DestroyMeshContainer )(LPD3DXMESHCONTAINER meshContainerToFree);

    //helper functions
    bool findFile(std::string *filename);
    bool doesFileExist(std::string &filename);
    void splitPath(std::string& inputPath, 
		std::string* pathOnly, std::string* filenameOnly);
};