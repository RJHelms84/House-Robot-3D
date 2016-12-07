#include "Engine.h"
using namespace std;

namespace Batman
{
	//Constructor
	Font::Font(string name, int pointsize)
	{
		//initialises font object to null
		fontObj=NULL;
		D3DXFONT_DESC fontDesc =
		{
			pointsize,	//height
			0,			//width
			0,			//weight
			0,			//miplevels
			false,		//italic
			DEFAULT_CHARSET,	//charset
			OUT_TT_PRECIS,		//output precision
			CLIP_DEFAULT_PRECIS,	//quality
			DEFAULT_PITCH,		//pitch and family
			""
		};
		//copies font name from constructor to
		//FaceName (font face) of font object
		strcpy(fontDesc.FaceName, name.c_str());

		/*this creates the font object with the set properties
		for the selected Direct3D Device,
		g_engine means global engine and we'll 
		be doing that later once the full engine is complete
		*/
		D3DXCreateFontIndirect(g_engine->getDevice(),
			&fontDesc, &fontObj);
	}

	//Destructor
	Font::~Font()
	{
		//Frees up memory used by font object
		fontObj->Release();
	}

	void Font::Print(int x, int y, string text, D3DCOLOR color)
	{
		//figure out the text boundary
		/*First a rectangle object is created; blank.
		Then the DrawText method of the font object
		is called and the text properies are added.
		DT_CALCRECT means that the required rectangle
		to hold the text will be stored in rect (&rect)
		*/
		RECT rect = {x, y, 0, 0};
		fontObj->DrawText(NULL, text.c_str(), text.length(),
			&rect, DT_CALCRECT, color);

		//print the text
		fontObj->DrawText(g_engine->getSpriteObj(), text.c_str(),
			text.length(), &rect, DT_LEFT, color);
	}

	int Font::getTextWidth(std::string text)
	{
		//figures out text boundary
		RECT rect = {0,0,0,0};
		fontObj->DrawText(NULL, text.c_str(), text.length(), &rect,
			DT_CALCRECT, 0xffffffff);
		//returns right boundary value
		return rect.right;
	}

	int Font::getTextHeight(std::string text)
	{
		//figures out text boundary
		RECT rect = {0,0,0,0};
		fontObj->DrawText(NULL, text.c_str(), text.length(), &rect,
			DT_CALCRECT, 0xffffffff);
		//returns bottom boundary value
		return rect.bottom;
	}
};