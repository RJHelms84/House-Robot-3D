#pragma once

#include "Engine.h"

namespace Batman
{
	class Color
	{
	public:
		//red, freen, blue, alpha
		float r, g, b, a;
		//destructor
		virtual ~Color();
		//constructor, overloads and copy constructors
		Color();
		Color(const Color& color);
		Color(int R, int G, int B, int A);
		Color(float R, float G, float B, float A);
		Color& Color::operator=(const Color& c);

		//set values for color
		void Set(int R, int G, int B, int A);
		void Set(float R, float G, float B, float A);

		//D3D compatibility
		D3DCOLOR ToD3DCOLOR();
		//shader compatibility
		D3DXVECTOR4 ToD3DXVECTOR4();
	};
};