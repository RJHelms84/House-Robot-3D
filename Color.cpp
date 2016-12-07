#include "Engine.h"
using namespace Batman;

//destructor
Color::~Color() { }

//default constructor (white)
Color::Color()
{
	r=g=b=a=1.0f;
}

//copy constructor
Color::Color(const Color& color)
{
	*this = color;
}

Color::Color(int R, int G, int B, int A)
{
	Set(R,G,B,A);
}

//constructor with initial values passed
Color::Color(float R, float G, float B, float A)
{
	Set(R,G,B,A);
}

//copy constructor with operator overload
Color& Color::operator=(const Color& color)
{
	Set(color.r,color.g,color.b,color.a);
	return *this;
}

/**
Convert 0-255 color values to 0.0-1.0 range
**/
void Color::Set(int R, int G, int B, int A)
{
	r = (float)R/256.f;
	g = (float)G/256.f;
	b = (float)B/256.f;
	a = (float)A/256.f;
}

//set RGBA values
void Color::Set(float R, float G, float B, float A)
{
	r=R; g=G; b=B; a=A;
}

/**
Convert from our 0.0f-1.0f color values to 0-255 based D3DCOLOR
**/
D3DCOLOR Color::ToD3DCOLOR()
{
	D3DCOLOR color = D3DCOLOR_COLORVALUE(r,g,b,a);
	return color;
}

/**
Convert to a D3DXVECTOR4 for use by shader code
**/
D3DXVECTOR4 Color::ToD3DXVECTOR4()
{
	D3DXVECTOR4 v4(r,g,b,a);
	return v4;
}