#pragma once

#include "Engine.h"

namespace Batman
{
	class Texture
	{
	private:
		//surfaces for when 'rendering to target'
		LPDIRECT3DSURFACE9 renderDepthStencilSurface;
		LPDIRECT3DSURFACE9 renderSurface;
	public:
		//constructor / destructor
		Texture();
		~Texture();
		//create blank texture with width / height
		bool Create(int width, int height);

		//load from file / save to file / release memory
		bool Load(std::string filename, Color transcolor = Color(255,0,255,0));
		bool Save(std::string filename, _D3DXIMAGE_FILEFORMAT format = D3DXIFF_BMP);
		void Release();

		//create render target, begin and end rendering to target
		bool createRenderTarget(int width, int height);
		bool renderStart(bool clear = true, bool sprite = true,
							Color clearColor = Color(255,0,255,0));
		bool renderStop(bool sprite = true);

		//Getters
		LPDIRECT3DTEXTURE9 getTexture() { return texture; }
		int getWidth();
		int getHeight();
		RECT getBounds();

		//Texture pointer and image info struct
		LPDIRECT3DTEXTURE9 texture;
		D3DXIMAGE_INFO info;
	};
};