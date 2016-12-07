#include "Engine.h"
using namespace std;
using namespace Batman;

//flexible vertex format - used for creating quads with textures
//for each cube face
const int SKYBOX_FVF = D3DFVF_XYZ | D3DFVF_TEX1;

//a struct for each skybox vertex with texture UV co-ordinate
struct SkyboxVertex
{
	float x,y,z;
	float u,v;
	SkyboxVertex(float _x, float _y, float _z, float _u, float _v)
	{
		x = _x;
		y = _y;
		z = _z;
		u = _u;
		v = _v;
	}
};

//initialise mesh and texture pointers
//as null
Skybox::Skybox(void)
{
	mesh = NULL;

	for(int n=0; n<6; n++)
		textures[n]=NULL;
}

Skybox::~Skybox(void)
{
	//delete the textures
	for(int n=0; n<6;n++)
	{
		if(textures[n])
		{
			delete textures[n];
			textures[n] = NULL;
		}
	}
	//release mesh memory
	if(mesh) mesh->Release();
}

bool Skybox::Create(std::string sharedFilename)
{
	//get the prefix for each cube face
	std::string prefix[] = {"U_","F_","B_","R_","L_","D_"};

	//set texture mapping sampler rates
	for(int i=0; i<4; i++)
	{
		//sets linear filtering for textures
		g_engine->getDevice()->SetSamplerState(
			i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_engine->getDevice()->SetSamplerState(
			i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		g_engine->getDevice()->SetSamplerState(
			i, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	}

	//release textures if already in memory
	for(int n=0; n<6;n++)
	{
		if(textures[n])
		{
			delete textures[n];
			textures[n] = NULL;
		}
	}
	
	//recreate any null texture
	for(int n=0; n<6; n++)
	{
		if(!textures[n])
			textures[n] = new Texture();
	}

	//load skybox textures
	bool res = true;
	for(int n=0; n<6; n++)
	{
		//create temp full filename
		string fn = prefix[n] + sharedFilename;
		//try loading texture
		if(!textures[n]->Load(fn.c_str()))
		{
			debug << "Skybox: Error loading" << fn << endl;
			return false;
		}
	}

	//create mesh
	D3DXCreateMeshFVF(12, 24, D3DXMESH_MANAGED, SKYBOX_FVF, 
		g_engine->getDevice(), &mesh);

	//create vertices
	SkyboxVertex* v = 0;
	mesh->LockVertexBuffer(0,(void**)&v);

	{
		float size = 4000.0f;

		//up face
		v[0] = SkyboxVertex(size, size, size, 0.f, 0.f);
		v[1] = SkyboxVertex(-size, size, size, 1.f, 0.f);
		v[2] = SkyboxVertex(size, size, -size, 0.f, 1.f);
		v[3] = SkyboxVertex(-size, size, -size, 1.f, 1.f);

		//front face
		v[4] = SkyboxVertex(-size, size, size, 0.f, 0.f);
		v[5] = SkyboxVertex( size, size, size, 1.f, 0.f);
		v[6] = SkyboxVertex(-size, -size, size, 0.f, 1.f);
		v[7] = SkyboxVertex( size, -size, size, 1.f, 1.f);

		//back face
		v[8] = SkyboxVertex( size, size, -size, 0.f, 0.f);
		v[9] = SkyboxVertex(-size, size, -size, 1.f, 0.f);
		v[10] = SkyboxVertex( size, -size, -size, 0.f, 1.f);
		v[11] = SkyboxVertex(-size, -size, -size, 1.f, 1.f);

		//right face
		v[12] = SkyboxVertex(-size, size, -size, 0.f, 0.f);
		v[13] = SkyboxVertex(-size, size, size, 1.f, 0.f);
		v[14] = SkyboxVertex(-size, -size, -size, 0.f, 1.f);
		v[15] = SkyboxVertex(-size, -size, size, 1.f, 1.f);

		//left face
		v[16] = SkyboxVertex(size, size, size, 0.f, 0.f);
		v[17] = SkyboxVertex(size, size, -size, 1.f, 0.f);
		v[18] = SkyboxVertex(size, -size, size, 0.f, 1.f);
		v[19] = SkyboxVertex(size, -size, -size, 1.f, 1.f);

		//down face
		v[20] = SkyboxVertex( size, -size, -size, 0.f, 0.f);
		v[21] = SkyboxVertex(-size, -size, -size, 1.f, 0.f);
		v[22] = SkyboxVertex( size, -size, size, 0.f, 1.f);
		v[23] = SkyboxVertex(-size, -size, size, 1.f, 1.f);

	}

	mesh->UnlockVertexBuffer();

	//calculate indices
	//(optimisation technique for rendering)
	//as vertices are for quad, points can
	//be reused
	unsigned short* indices = 0;
	mesh->LockIndexBuffer(0,(void**)&indices);

	int index = 0;
	for(int quad=0; quad<6; quad++)
	{
		//first face
		indices[index++] = quad*4;
		indices[index++] = quad*4 + 1;
		indices[index++] = quad*4 + 2;

		//second face
		indices[index++] = quad*4 + 1;
		indices[index++] = quad*4 + 3;
		indices[index++] = quad*4 + 2;
	}

	mesh->UnlockIndexBuffer();

	//set each quad to its sub mesh
	//(for each draw subset call)
	unsigned long *att = 0;
	mesh->LockAttributeBuffer(0,&att);
	for(int i=0; i<12; i++)
		att[i] = i/2;
	mesh->UnlockAttributeBuffer();

	return true;
}

void Skybox::Render()
{
	//save render states (turn off lighting, Z-buffering, clamp textures)
	g_engine->getDevice()->SetRenderState(D3DRS_LIGHTING, false);
	g_engine->getDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	g_engine->getDevice()->SetRenderState(D3DRS_ZENABLE, false);
	g_engine->getDevice()->SetSamplerState(0,  D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_engine->getDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	
	//render the skybox
	for ( int n = 0; n < 6; n++ )
    {
		g_engine->getDevice()->SetTexture(0, textures[n]->getTexture());
        mesh->DrawSubset(n);
    }
	
	//restore render states
	g_engine->getDevice()->SetRenderState(D3DRS_LIGHTING, true);
	g_engine->getDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	g_engine->getDevice()->SetRenderState(D3DRS_ZENABLE, true);
	g_engine->getDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_engine->getDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}