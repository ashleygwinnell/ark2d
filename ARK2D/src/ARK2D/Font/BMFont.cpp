/*
 * BMFont.cpp
 *
 *  Created on: 13-Apr-2010
 *      Author: Ashley
 */

#include <sstream>
#include <iostream>
#include <fstream>
#include "BMFont.h"
#include "../Image.h"
#include "../GameContainer.h"

using namespace std;

BMFont::BMFont(): m_data(NULL) {

}

BMFont::BMFont(unsigned int fntResource, unsigned int imgResource, unsigned int imgResourceType):
	m_data(NULL),
	m_FontFile(""),
	m_ImageFile("")
{
	std::cout << "Loading BMFont... ";
		m_data = (char*) GameContainerPlatform::getARK2DResource(fntResource, ARK2D_RESOURCE_TYPE_FNT);
		m_Image = new Image(imgResource, imgResourceType);
		Parse();
	std::cout << "done." << std::endl;
}

BMFont::BMFont(const string& f, const string& i):
	m_data(NULL),
	m_FontFile(f),
	m_ImageFile(i)
	//m_Image(i)
{
	std::cout << "Loading BMFont... ";
		m_Image = new Image(i);
		Parse();
	std::cout << "done." << std::endl;
}

BMFont::BMFont(const string& f, const string& i, const Color& mask):
	m_data(NULL),
	m_FontFile(f),
	m_ImageFile(i)//,
	//m_Image(i, mask)
{
	OutputWrapper::print(StringUtil::append("Loading BMFont: ", f));

	m_Image = new Image(i, mask);
	Parse();
}

BMFont::~BMFont() {
	delete m_data;
}

Image* BMFont::getImage() const {
	return m_Image;
}

// Yarp!
bool BMFont::Parse() // istream& Stream, Charset& CharsetDesc
{
	try {
		streambuf* fb;
		if (m_data == NULL) {
			fb = new filebuf;
			((filebuf*) fb)->open(m_FontFile.c_str(), ios::in);
		} else {
			fb = new stringbuf;
			((stringbuf*)fb)->str(m_data);
		}
		istream Stream(fb);

		string Line;
		string Read, Key, Value;
		std::size_t i;

		while( !Stream.eof() )
		{
			stringstream LineStream;
			std::getline( Stream, Line );

			bool skippingLine = false;
			//std::cout << Line << std::endl;

			if (Line.length() == 0) { break; }
			//if (Line == -1) {
			//	return false;
			//}
			//if (*ret == -1) { break; }
			LineStream << Line;

			//read the line's type
			LineStream >> Read;
			if( Read == "common" )
			{
				//this holds common data
				while( !LineStream.eof() )
				{
					stringstream Converter;
					LineStream >> Read;
					i = Read.find( '=' );
					Key = Read.substr( 0, i );
					Value = Read.substr( i + 1 );

					//assign the correct value
					Converter << Value;
					if( Key == "lineHeight" )
						Converter >> m_Charset.LineHeight;
					else if( Key == "base" )
						Converter >> m_Charset.Base;
					else if( Key == "scaleW" )
						Converter >> m_Charset.Width;
					else if( Key == "scaleH" )
						Converter >> m_Charset.Height;
					else if( Key == "pages" )
						Converter >> m_Charset.Pages;
				}
			}
			else if( Read == "char" )
			{
				//this is data for a specific char
				unsigned short CharID = 0;

				while( !LineStream.eof() )
				{
					stringstream Converter;
					LineStream >> Read;
					i = Read.find( '=' );
					Key = Read.substr( 0, i );
					Value = Read.substr( i + 1 );

					if (skippingLine) {
						continue;
					}

					//assign the correct value
					Converter << Value;
					if( Key == "id" )
						Converter >> CharID;
						if (CharID > 255) {
							// no non-ascii (only 255 or below) plz
							//std::cout << "bad char: " << CharID << ". skipping..." << std::endl;
							skippingLine = true;
							continue;
						}
					else if( Key == "x" )
						Converter >> m_Charset.Chars[CharID].x;
					else if( Key == "y" )
						Converter >> m_Charset.Chars[CharID].y;
					else if( Key == "width" )
						Converter >> m_Charset.Chars[CharID].Width;
					else if( Key == "height" )
						Converter >> m_Charset.Chars[CharID].Height;
					else if( Key == "xoffset" )
						Converter >> m_Charset.Chars[CharID].XOffset;
					else if( Key == "yoffset" )
						Converter >> m_Charset.Chars[CharID].YOffset;
					else if( Key == "xadvance" )
						Converter >> m_Charset.Chars[CharID].XAdvance;
					else if( Key == "page" )
						Converter >> m_Charset.Chars[CharID].Page;
				}
			}
		}
		if (m_data == NULL) {
			((filebuf*)fb)->close();
		} else {
		//	((stringbuf*)fb)->close();
		}

	} catch(...) {
		ErrorDialog::createAndShow("error loading font");
		return false;
	}
	return true;
}

void BMFont::drawStringCentered(const string& Str, int left_x, int right_x, int y) const {
	int c_x = (right_x / 2) + (left_x/2);
	drawString(Str, c_x - (getStringWidth(Str)/2), y);
}

void BMFont::drawStringCenteredAt(const string& Str, int x, int y) const {
	drawString(Str, x - (getStringWidth(Str)/2), y - (getStringHeight(Str)/2));
}

// remember that u and v are width and height, respectively.
void BMFont::drawString(const string& Str, int drawx, int drawy) const {

	short CharX, CharY, Width, Height, OffsetX, OffsetY;
	//m_Image.getSubImage(10, 10, 10, 10).draw(drawx, drawy);

//	int TotalX = 0;
	for( unsigned int i = 0; i < Str.size(); ++i )
	{

		CharX = m_Charset.Chars[(int) Str[i]].x;
		CharY = m_Charset.Chars[(int) Str[i]].y;
		Width = m_Charset.Chars[(int) Str[i]].Width;
		Height = m_Charset.Chars[(int) Str[i]].Height;
		OffsetX = m_Charset.Chars[(int) Str[i]].XOffset;
		OffsetY = m_Charset.Chars[(int) Str[i]].YOffset;

		Image* img = m_Image->getSubImage(CharX, CharY, Width, Height);
		img->setAlpha(m_Image->getAlpha());
		img->draw(drawx + OffsetX, drawy + OffsetY);
		delete img;
		//TotalX += Width;
		drawx += m_Charset.Chars[(int) Str[i]].XAdvance;
		//drawy += m_Charset.Chars[Str[i]].YAdvance;

		//upper left
	/*	Verts[i*4].tu = (float) CharX / (float) m_Charset.Width;
		Verts[i*4].tv = (float) CharY / (float) m_Charset.Height;
		Verts[i*4].x = (float) CurX + OffsetX;
		Verts[i*4].y = (float) OffsetY;

		//upper right
		Verts[i*4+1].tu = (float) (CharX+Width) / (float) m_Charset.Width;
		Verts[i*4+1].tv = (float) CharY / (float) m_Charset.Height;
		Verts[i*4+1].x = (float) Width + CurX + OffsetX;
		Verts[i*4+1].y = (float) OffsetY;

		//lower right
		Verts[i*4+2].tu = (float) (CharX+Width) / (float) m_Charset.Width;
		Verts[i*4+2].tv = (float) (CharY+Height) / (float) m_Charset.Height;
		Verts[i*4+2].x = (float) Width + CurX + OffsetX;
		Verts[i*4+2].y = (float) Height + OffsetY;

		//lower left
		Verts[i*4+3].tu = (float) CharX / (float) m_Charset.Width;
		Verts[i*4+3].tv = (float) (CharY+Height) / (float) m_Charset.Height;
		Verts[i*4+3].x = (float) CurX + OffsetX;
		Verts[i*4+3].y = (float) Height + OffsetY;

		CurX += m_Charset.Chars[Str[i]].XAdvance;*/
	}
	//std::cout << TotalX << std::endl;
}

unsigned int BMFont::getStringWidth(const string& Str) const {
	unsigned int total = 0;
	for (unsigned int i = 0; i < Str.size(); i++ ) {
		//total += m_Charset.Chars[(int) Str[i]].Width;
		//total += m_Charset.Chars[(int) Str[i]].XOffset;
		total += m_Charset.Chars[(int) Str[i]].XAdvance;
	}
	return total;
}
unsigned int BMFont::getStringHeight(const string& Str) const {
	unsigned int max = 0;
	for (unsigned int i = 0; i < Str.size(); i++ ) {
		if ((m_Charset.Chars[(int) Str[i]].Height + m_Charset.Chars[(int) Str[i]].YOffset) > max) {
			max = (m_Charset.Chars[(int) Str[i]].Height + m_Charset.Chars[(int) Str[i]].YOffset);
		}
	}
	return max;
}
unsigned int BMFont::getLineHeight() const {
	return m_Charset.LineHeight;
}


