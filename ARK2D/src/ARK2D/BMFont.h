/*
 * BMFont.h
 *
 *  Created on: 13-Apr-2010
 *      Author: Ashley
 */

#ifndef BMFONT_H_
#define BMFONT_H_

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "ARK2D_GL.h"

#include "Image.h"
#include "Color.h"
#include "OutputWrapper.h"
#include "StringUtil.h"

using std::string;
using std::istream;
using std::stringstream;


// CharDescriptor holds the information for a single character;
struct CharDescriptor
{
	//clean 16 bytes
	unsigned short x, y;
	unsigned short Width, Height;
	unsigned short XOffset, YOffset;
	unsigned short XAdvance;
	unsigned short Page;

	CharDescriptor() : x( 0 ), y( 0 ), Width( 0 ), Height( 0 ), XOffset( 0 ), YOffset( 0 ),
		XAdvance( 0 ), Page( 0 )
	{ }
};
// Charset holds the descriptions that apply to all characters, as well as the descriptors for every character.
struct Charset
{
	unsigned short LineHeight;
	unsigned short Base;
	unsigned short Width, Height;
	unsigned short Pages;
	CharDescriptor Chars[256];
};
struct FontVertex {
	float x, y, tu, tv;
};

class BMFont {
	private:
		string m_FontFile;
		string m_ImageFile;
		Charset m_Charset;
		Image m_Image;

	public:
		BMFont();
		BMFont(const string& f, const string& i);
		BMFont(const string& f, const string& i, const Color& mask);
		~BMFont();
		void drawString(const string& Str, int drawx, int drawy) const;
		void drawStringCentered(const string& Str, int left_x, int right_x, int y) const;
		void drawStringCenteredAt(const string& Str, int x, int y) const;
		unsigned int getStringWidth(const string& Str) const;
		unsigned int getStringHeight(const string& Str) const;
		unsigned int getLineHeight() const;
		Image* getImage() const;
	private:
		bool Parse();


};

#endif /* FONT_H_ */
