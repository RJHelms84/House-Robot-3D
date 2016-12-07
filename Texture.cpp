#include "Engine.h"
using namespace Batman;

//default constructor - NULLS object pointers
Texture::Texture()
{
	texture = NULL;
	renderDepthStencilSurface = NULL;
	renderSurface = NULL;
}

//Destructor - calls Release
Texture::~Texture()
{
	Release();
}

//using image info struct, get width / height
int Texture::getWidth()
{
	return info.Width;
}

int Texture::getHeight()
{
	return info.Height;
}

//get bounding rectangle
RECT Texture::getBounds()
{
	RECT rect = {0, 0, getWidth()-1, getHeight()-1};
	return rect;
}

//load from file
bool Texture::Load(std::string filename, Color transcolor)
{
	//standard windows return value
	HRESULT result;

	//get width and height from bitmap file
	result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if(result!=D3D_OK)
	{
		texture = NULL;
		return false;
	}

	//create the new texture by loading a bitmap image file
	D3DXCreateTextureFromFileEx(
		g_engine->getDevice(),
		filename.c_str(),
		info.Width,
		info.Height,
		1,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transcolor.ToD3DCOLOR(),
		&info,
		NULL,
		&texture
		);

	//make sure the bitmap texture was loaded correctly
	if(result!=D3D_OK)
	{
		texture=NULL;
		return false;
	}

	return true;
}

//Release memory
void Texture::Release()
{
	if(texture) texture->Release();
	if(renderDepthStencilSurface)
		renderDepthStencilSurface->Release();
	if(renderSurface) renderSurface->Release();
}

//create blank texture with width / height
bool Texture::Create(int width, int height)
{
	//if texture is already in use, first de-allocate memory
	if(texture)
	{
		texture->Release();
		texture=NULL;
	}

	HRESULT r;

	r = D3DXCreateTexture(			
			g_engine->getDevice(),	//D3D device
			width,					//width
			height,					//height
			1,						//mip levels
			D3DUSAGE_DYNAMIC,		//usage (optimised)
			D3DFMT_A8R8G8B8,		//color format
			D3DPOOL_DEFAULT,		//memory pool
			&texture);				//texture location to save

	//error checking
	if(r!=D3D_OK)
	{
		texture=NULL;
		return false;
	}

	//save texture info
	info.Width = width;
	info.Height = height;
	info.Format = D3DFMT_A8R8G8B8;

	return true;
}

//create separate render target
bool Texture::createRenderTarget(int width, int height)
{
	//if texture is already in use, first deallocate memory
	if(texture)
	{
		texture->Release();
		texture=NULL;
	}

	//create the render target surface, depth stencil
	g_engine->getDevice()->CreateTexture(
						width,					//width
						height,					//height
						1,						//mip levels
						D3DUSAGE_RENDERTARGET,  //usage (render target)
						D3DFMT_A8R8G8B8,		//color format
						D3DPOOL_DEFAULT,		//memory pool
						&texture,				//texture location to store
						NULL);					//window handles

	g_engine->getDevice()->CreateDepthStencilSurface(
						width,						//width	
						height,						//height
						D3DFMT_D16,					//format
						D3DMULTISAMPLE_NONE,		//multisample flags
						0,							//multisample quality
						false,						//discard?
						&renderDepthStencilSurface, //where to save
						NULL);						//window handles

	//set as a render surface
	texture->GetSurfaceLevel(0, &renderSurface);

	//save texture info
	info.Width = width;
	info.Height = height;
	info.Format = D3DFMT_A8R8G8B8;

	return true;
}

/**
	Used only when texture is a render target
	and never when the primary device is rendering!
**/
bool Texture::renderStart(bool clear, bool sprite, Color clearColor)
{
	//set render target and depth stencil surface
	//(like the backbuffer, but not the backbuffer)
	g_engine->getDevice()->SetRenderTarget(0, renderSurface);
	g_engine->getDevice()->SetDepthStencilSurface(renderDepthStencilSurface);
	
	//if clear flag set, clear to color set
	if(clear)
	{
		g_engine->getDevice()->Clear(0, NULL, D3DCLEAR_TARGET,
			clearColor.ToD3DCOLOR(), 1.0f, 0);
	}
	//begin scene rendering
	g_engine->getDevice()->BeginScene();

	//if sprite flag set (2D) begin sprite object
	if(sprite)
		g_engine->getSpriteObj()->Begin(D3DXSPRITE_ALPHABLEND);

	//set base for transforms as identity matrix
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	g_engine->getSpriteObj()->SetTransform(&identity);

	return true;
}

/**
	Used only when texture is a render target
	and never when the primary device is rendering!
**/
bool Texture::renderStop(bool sprite)
{
	if(sprite)
	g_engine->getSpriteObj()->End();
	g_engine->getDevice()->EndScene();
	return true;
}

/**
	Saves a texture in memory to a file.
	Supports TGA, JPG, PNG, DDS, BMP (default).
**/
bool Texture::Save(std::string filename, _D3DXIMAGE_FILEFORMAT format)
{
	if(texture)
	{
		D3DXSaveTextureToFile(filename.c_str(), format, texture, NULL);
		return true;
	}
	else
		return false;
}
