/*
 * FTFont.h
 *
 *  Created on: 24 Jul 2011
 *      Author: Ashley
 */

#ifndef FTFONT_H_
#define FTFONT_H_

#include "../ARK2D.h"

// FreeType Headers
#ifdef ARK2D_WINDOWS
	#include <ft2build.h>
	#include <freetype/freetype.h>
	#include <freetype/ftglyph.h>
	#include <freetype/ftoutln.h>
	#include <freetype/fttrigon.h>
#elif defined (ARK2D_MACINTOSH)
	//#include <X11/freetype2/freetype/freetype.h>
	#include <ft2build.h>
	#include <freetype/freetype.h>
	#include <freetype/ftglyph.h>
	#include <freetype/ftoutln.h>
	#include <freetype/fttrigon.h>
#endif

#include "../ARK2D_GL.h"
#include "../UI/ErrorDialog.h"

#include <stdio.h>
#include <iostream>

#include <string>
#include <vector>
using namespace std;

class FTFontCharData {
	public:
		float x;
		float y;
		int width;
		int height;
		float th;
		float tw;
		int tid;
		int advancex;
		int advancey;
		int offsetx;
		int offsety;
};

class FTFontData {
	private:
		inline int next_p2(int a);
		void make_dlist(FT_Face face, unsigned char ch, GLuint list_base, GLuint* tex_base);
	public:
		float m_height; // holds the height of the font.
		GLuint* textures; // holds the texture ids
		GLuint list_base; // holds the first display list id.
		FTFontCharData* characterDatas; // holds the character datas (this is the pointer to the first of 128?).
		int m_lineHeight;

		// this function will create a font with the height H from the file name.
		void init(const char* fname, unsigned int h);

		// freee the resources for the font.
		void clean();
};



class FTFont {
	private:
		FTFontData* m_data;
	public:
		FTFont(const char* filename, unsigned int height);
		void drawString(const string& str, int x, int y);
		unsigned int getLineHeight() const;
		virtual ~FTFont();
};

#endif /* FTFONT_H_ */
