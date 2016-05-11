/*
 * FTFont.h
 *
 *  Created on: 24 Jul 2011
 *      Author: Ashley
 */

#ifndef FTFONT_H_
#define FTFONT_H_

#include "../ARK2D.h"

 #if !defined(ARK2D_WINDOWS_PHONE_8) && !defined(ARK2D_WINDOWS) && !defined(ARK2D_UBUNTU_LINUX) && !defined(ARK2D_EMSCRIPTEN_JS) && !defined(ARK2D_ANDROID) && !defined(ARK2D_XBOXONE)

		// FreeType Headers
		#if (defined(ARK2D_IPHONE) || defined(ARK2D_ANDROID) || defined(ARK2D_WINDOWS) || defined(ARK2D_MACINTOSH) || defined(ARK2D_FLASCC) || defined(ARK2D_WINDOWS_PHONE_8))

		 	#include <ft2build.h>

			#include <freetype/freetype.h>
			#include <freetype/ftglyph.h>
			#include <freetype/ftoutln.h>
			#include <freetype/fttrigon.h>
		 	//#include <X11/freetype2/freetype/freetype.h> // mac
		#endif

		#include "../Includes.h"
		#include "../UI/ErrorDialog.h"

		#include "Font.h"

		#include <stdio.h>
		#include <iostream>

		#include <string>
		#include <vector>
		using namespace std;

		namespace ARK {
			namespace Font {

				#ifndef DOXYGEN_SHOULD_SKIP_THIS
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
				#endif

				/**
				 * \brief Freetype font implementation, for displaying TTF fonts.
				 *
				 *
				 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
				 */
				class ARK2D_API FTFont : public ARK::Font::Font {
					private:
						FTFontData* m_data;
					public:
						FTFont(string filename, unsigned int height);
						void drawString(const string& str, float x, float y);
						unsigned int getStringWidth(const string& Str) const;
						unsigned int getStringHeight(const string& Str) const;
						unsigned int getLineHeight() const;
						void scale(float f);
						virtual ~FTFont();
				};
			}
		}

#else

	#include "../Includes.h"
	#include "Font.h"

	namespace ARK {
		namespace Font {
			class ARK2D_API FTFont : public ARK::Font::Font {

			};
		}
	}

#endif

#endif /* FTFONT_H_ */
