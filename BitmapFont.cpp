#pragma once

#include "Engine.h"

namespace Batman
{
	BitmapFont::BitmapFont() : Sprite()
	{
		//set character widths default
		memset(&p_widths, 0 , sizeof(p_widths));
	}

	void BitmapFont::Print(int x, int y, std::string text, Color color)
	{
		//x,y and color
		float fx = (float)x;
		float fy = (float)y;
		setColor(color);

		//draw each character of string
		for(unsigned int n=0; n<text.length(); n++)
		{
			//set frame and pos to render sprite char
			int frame = (int)text[n];
			setCurrentFrame(frame);
			setX(fx);
			setY(fy);
			Render();

			//use proportional width if available & update pos
			if(p_widths[frame] == 0) p_widths[frame] = (int)size.x;
			fx += (float)(p_widths[frame] * scale.x);
		}
	}

	bool BitmapFont::loadWidthData(std::string filename)
	{
		unsigned char buffer[512];

		//open font width data file
		std::ifstream infile;
		infile.open(filename.c_str(), std::ios::binary);
		if(!infile) return false;

		//read 512 bytes
		infile.read((char*)(&buffer),512);
		if(infile.bad()) return false;
		infile.close();

		//convert raw data to width data
		for(int n=0; n<256;n++)
			p_widths[n] = (int)buffer[n*2];

		return true;
	}
}