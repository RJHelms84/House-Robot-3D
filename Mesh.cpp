#include "Engine.h"
using namespace std;
using namespace Batman;

//constructor - initialise vars - render type
Mesh::Mesh() : Entity(RENDER_3D)
{
	mesh = 0;
	materials = 0;
	material_count=0;
	textures=0;
	position = Vector3(0.0f,0.0f,0.0f);
	rotation = Vector3(0.0f,0.0f,0.0f);
	scale = Vector3(1.0f,1.0f,1.0f);
}

//destructor - release memory
Mesh::~Mesh(void)
{
	if(materials!=NULL) delete[] materials;
	if(textures!=NULL) {
		for(DWORD i=0; i<material_count; i++)
		{
			if(textures[i]!=NULL)
				textures[i]->Release();
		}
		delete[] textures;
	}
	if(mesh!=NULL) mesh->Release();
}

//use mesh pointer toi get vertices / faces 
int Mesh::getFaceCount()
{
	return mesh->GetNumFaces();
}

int Mesh::getVertexCount()
{
	return mesh->GetNumVertices();
}

//WRAPPERS FOR PRIMITIVES--------------------------------------------------------------------------
void Mesh::createTorus(float innerRadius, float outerRadius, int sides, int rings)
{
	D3DXCreateTorus(g_engine->getDevice(), innerRadius, outerRadius, sides, rings, &mesh, NULL);
}

void Mesh::createCylinder(float radius1, float radius2, float length, int slices, int stacks)
{
	D3DXCreateCylinder(g_engine->getDevice(), radius1, radius2, length, slices, stacks, &mesh, NULL);
}

void Mesh::createSphere(double radius, int slices, int stacks)
{
	D3DXCreateSphere(g_engine->getDevice(), (float)radius, slices, stacks, &mesh, NULL);
}
    
void Mesh::createCube(double width, double height, double depth)
{
	D3DXCreateBox(g_engine->getDevice(), (float)width, (float)height, (float)depth, &mesh, NULL);
}

void Mesh::createTeapot()
{
	D3DXCreateTeapot(g_engine->getDevice(), &mesh, NULL);
}
//END: WRAPPERS FOR PRIMITIVES--------------------------------------------------------------------------

//file helpers
void Mesh::splitPath(string& inputPath, string* pathOnly,
		string* filenameOnly)
{
	//convert all dividers to forward slashes and find last
	string fullPath(inputPath);
	replace(fullPath.begin(), fullPath.end(), '\\','/');
	string::size_type lastSlashPos = fullPath.find_last_of('/');

	//check for there being no path element in the input
	if(lastSlashPos == string::npos)
	{
		*pathOnly="";
		*filenameOnly=fullPath;
	}
	else
	{
		//if path only, get up to last slash
		if(pathOnly) {
			*pathOnly = fullPath.substr(0, lastSlashPos);
		}
		//otherwise just get the filename
		if(filenameOnly)
		{
			*filenameOnly = fullPath.substr(
				lastSlashPos + 1,
				fullPath.size() - lastSlashPos - 1
				);
		}
	}
}

//try accessing file, return result
bool Mesh::doesFileExist(string &filename)
{
	return(_access(filename.c_str(), 0) !=1);
}

bool Mesh::findFile(string *filename)
{
	if(!filename) return false;

	//since the file was not found, try removing the path
	string pathOnly;
	string filenameOnly;
	splitPath(*filename, &pathOnly, &filenameOnly);

	//is file found in current folder, without the path?
	if(doesFileExist(filenameOnly))
	{
		*filename=filenameOnly;
		return true;
	}

	//not found
	return false;
}

bool Mesh::Load(char* filename, bool computeNormals)
{
	HRESULT result;
	LPD3DXBUFFER matbuffer;
	LPD3DXMATERIAL d3dxMaterials;

	//load mesh from the specified file
	result = D3DXLoadMeshFromX(
		filename,					//filename
		D3DXMESH_SYSTEMMEM,			//mesh options
		g_engine->getDevice(),		//Direct3D Device
		NULL,						//adjacency buffer
		&matbuffer,					//material buffer
		NULL,						//special effects
		&material_count,			//number of materials
		&mesh						//resulting mesh
		);

	if(result!=D3D_OK)
	{
		return false;
	}

	//extract material properties and texture names from material buffer
	d3dxMaterials = (LPD3DXMATERIAL)matbuffer->GetBufferPointer();
	materials = new D3DMATERIAL9[material_count];
	textures = new LPDIRECT3DTEXTURE9[material_count];

	//create the materials and textures
	for(DWORD i=0; i <material_count; i++)
	{
		//grab the material
		materials[i] = d3dxMaterials[i].MatD3D;

		//load textures
		textures[i] = NULL;
		if(d3dxMaterials[i].pTextureFilename!=NULL)
		{
			string filename = d3dxMaterials[i].pTextureFilename;
			if(findFile(&filename))
			{
				//load texture file specified in .x file
				result =D3DXCreateTextureFromFile(g_engine->getDevice(),
					filename.c_str(), &textures[i]);
				if(result!=D3D_OK) return false;

				//verify texture header
				if(textures[i]->GetType()!=D3DRTYPE_TEXTURE)
					return false;					
			}
		}
	}
//done using material buffer
	matbuffer->Release();

	//if compute normals flag selected, do so
	if(computeNormals)
	{
		HRESULT res = D3DXComputeNormals(mesh, NULL);
		if (res!=S_OK)
		{
			debug << "Mesh::Load: Error computing normals\n";
		}
	}
	return true;
}

//Set transform matrices
void Mesh::Update(float deltaTime)
{
	Transform();
}

//Render methods
void Mesh::Render()
{
	Render(p_effect,"Texture");
}

//Render method 
void Mesh::Render(Effect *effect, std::string fxTextureParam)
{
	//set matrices and begin effect rendering
	p_effect = effect;
	p_effect->setWorldMatrix(matWorld);
	p_effect->Begin();

	//if material count 0, render only one subset
	if(material_count==0)
	{
		mesh->DrawSubset(0);
	}
	else
	{
		//otherwsie we loop through
		//draw each mesh subset
		for(DWORD i=0; i < material_count; i++)
		{
			//set the texture used by this face
			if(textures[i])
			{
				p_effect->setParam(fxTextureParam, (textures[i]));
			}

			//draw the mesh subset
			mesh->DrawSubset(i);
		}
	}
	p_effect->End();
}

void Mesh::Transform()
{
	//set rotation matrix
	double x = D3DXToRadian(rotation.x);
	double y = D3DXToRadian(rotation.y);
	double z = D3DXToRadian(rotation.z);
	D3DXMatrixRotationYawPitchRoll(&matRotate, (float)x, (float)y, (float)z);
    
	//set scaling matrix
	D3DXMatrixScaling(&matScale, (float)scale.x, (float)scale.y, (float)scale.z);
    
	//set translation matrix
	D3DXMatrixTranslation(&matTranslate, (float)position.x, (float)position.y, (float)position.z);
    
	//transform the mesh
	matWorld = matRotate * matScale * matTranslate;
}

//set position with values
void Mesh::setPosition(double x, double y, double z)
{
	position = Vector3((float)x,(float)y,(float)z);
}

//set rotation with values
void Mesh::Rotate(Vector3 rot)
{
	Rotate(rot.x, rot.y, rot.z);
}

//rotate given values
void Mesh::Rotate(double x, double y, double z)
{
	rotation.x+=(float)x;
	rotation.y+=(float)y;
	rotation.z+=(float)z;
}

//set rotate values
void Mesh::setRotation(double x, double y, double z)
{
	rotation = Vector3((float)x,(float)y,(float)z);
}

//set scale 
void Mesh::setScale(double x,double y,double z) 
{
	scale = Vector3((float)x,(float)y,(float)z); 
}

void Mesh::setScale(double value)
{
	scale.x = scale.y = scale.z = value;
}

BSPHERE Mesh::getBoundingSphere()
{
	//create vertex buffer for mesh
	BYTE* pVertices=NULL;
	mesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);

	//centre and radius of bounding sphere
	D3DXVECTOR3 center;
	float radius;

	//built in method to compute bounding sphere
	D3DXComputeBoundingSphere((D3DXVECTOR3*)pVertices, mesh->GetNumVertices(),
								D3DXGetFVFVertexSize(mesh->GetFVF()), &center, &radius );

	//unlock vertex buffer
	mesh->UnlockVertexBuffer();

	//sphere struct 
	BSPHERE sphere;
	sphere.center = Vector3(center);
	sphere.radius = radius;	//this could be adjusted for scale

	return sphere;
}

BBOX Mesh::getBoundingBox()
{
	//rather than creating fixed array size and setting to read-only on the vertex buffer
	//memory can be more easily allocated once this method is completed

	//create a buffer to store vertices of mesh
	BYTE* pVertices = 0;
	mesh->LockVertexBuffer(0, (LPVOID*)&pVertices);

	//min/max AABB x,y,z points
	D3DXVECTOR3 minBounds,maxBounds;

	//built in method to get min/max values
	D3DXComputeBoundingBox((D3DXVECTOR3*)pVertices, mesh->GetNumVertices(),
							D3DXGetFVFVertexSize(mesh->GetFVF()), &minBounds, &maxBounds );

	//unlock vertex buffer
	mesh->UnlockVertexBuffer();

	//new AABB struct
	BBOX box;
	Vector3 minimum = minBounds;
	Vector3 maximum = maxBounds;

	//compute min/max x,y,z adjusted for scale
	box.min.x = minBounds.x * scale.x;
	box.min.y = minBounds.y * scale.y;
	box.min.z = minBounds.z * scale.z;
	box.max.x = maxBounds.x * scale.x;
	box.max.y = maxBounds.y * scale.y;
	box.max.z = maxBounds.z * scale.z;

	return box;
}