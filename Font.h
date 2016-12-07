#pragma once;
#include "Engine.h"

//the namespace that Gotham deserves
namespace Batman
{
	class Font
	{
	private:
		//Direct3D Font
		LPD3DXFONT fontObj;

	public:
		//Constructor; gets font name and point size
		Font(std::string name, int size);
		//Destructor
		~Font();
		//Print: displays font at x,y position
		//with defined string text and colour
		void Print(int x, int y,
			std::string text,
			D3DCOLOR color=0xffffffff);
		//get width of text
		int getTextWidth(std::string text);
		//get height of text
		int getTextHeight(std::string text);
	};
};