/*
 * FTFont.cpp
 *
 *  Created on: 24 Jul 2011
 *      Author: Ashley
 */

#include "FTFont.h"
#include "../Graphics/Renderer.h"

namespace ARK {
	namespace Font {

		int FTFontData::next_p2(int a) {
			int rval=2;
			while(rval<a) rval<<=1;
			return rval;
		}

		void FTFontData::make_dlist(FT_Face face, unsigned char ch, GLuint list_base, GLuint* tex_base) {

			// get freetype to render out character in to a bitmap.
			// this requires a couple of freetype commands.

			// load the glyph for our character.
			if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT)) {
				ErrorDialog::createAndShow("FT_Load_Glyph failed");
				exit(0);
			}

			// move the face's glyph into a glyph object.
			FT_Glyph glyph;
			if (FT_Get_Glyph(face->glyph, &glyph)) {
				ErrorDialog::createAndShow("FT_Get_Glyph failed");
				exit(0);
			}

			// convert the glyph in to a bitmap.
			FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
			FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;

			// this reference makes referencing the bitmap easier.
			FT_Bitmap& bitmap = bitmap_glyph->bitmap;

			// get the width of the bitmap data and pad it the texture.
			int width = next_p2(bitmap.width);
			int height = next_p2(bitmap.rows);

			// allocate memory for texture data.
			GLubyte* expanded_data = new GLubyte[2 * width * height];

			// fill the data for the expanded bitmap.
			for(int j = 0; j < height; j++) {
				for(int i = 0; i < width; i++) {
					expanded_data[2 * (i+j*width)] = 255;
					expanded_data[2 * (i+j*width)+1] =
						(i >= bitmap.width || j >= bitmap.rows)
							?0
							:bitmap.buffer[i + bitmap.width*j];
				} 
			}

			// do some texture shizzle.
			//glBindTexture(GL_TEXTURE_2D, tex_base[ch]);
			RendererState::start(RendererState::TEXTURE, tex_base[ch]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			// create the texture with the data.
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
					GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

			// we don't need this any more.
			delete[] expanded_data;


			// Now We Need To Account For The Fact That Many Of
			// Our Textures Are Filled With Empty Padding Space.
			// We Figure What Portion Of The Texture Is Used By
			// The Actual Character And Store That Information In
			// The x And y Variables, Then When We Draw The
			// Quad, We Will Only Reference The Parts Of The Texture
			// That Contains The Character Itself.
			float x = (float) bitmap.width / (float) width;
			float y = (float) bitmap.rows / (float) height;


			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))

			#else
				// create the display list...
				glNewList(list_base+ch, GL_COMPILE);
				//glBindTexture(GL_TEXTURE_2D, tex_base[ch]);
				RendererState::start(RendererState::TEXTURE, tex_base[ch]);

				glPushMatrix();

					// First We Need To Move Over A Little So That
					// The Character Has The Right Amount Of Space
					// Between It And The One Before It.
					glTranslatef(bitmap_glyph->left, 0, 0);

					// Now We Move Down A Little In The Case That The
					// Bitmap Extends Past The Bottom Of The Line
					// This Is Only True For Characters Like 'g' Or 'y'.
					glTranslatef(0, bitmap_glyph->top - bitmap.rows, 0);
					//glTranslatef(0, bitmap_glyph->top, 0);


					// Here We Draw The Texturemapped Quads.
					// The Bitmap That We Got From FreeType Was Not
					// Oriented Quite Like We Would Like It To Be,
					// But We Link The Texture To The Quad
					// In Such A Way That The Result Will Be Properly Aligned.
					glBegin(GL_QUADS);
						glTexCoord2d(0,0); glVertex2f(0,0);
						glTexCoord2d(0,y); glVertex2f(0,bitmap.rows);
						glTexCoord2d(x,y); glVertex2f(bitmap.width,bitmap.rows);
						glTexCoord2d(x,0); glVertex2f(bitmap.width,0);
					glEnd();

				glPopMatrix();
				glTranslatef(face->glyph->advance.x >> 6 ,0,0);

				// Finish The Display List
				glEndList();

			#endif

			// scorecharacter data.
			FTFontCharData* charData = &characterDatas[ch];// + (sizeof(FTFontCharData) * ch_ui);
			charData->offsetx = bitmap_glyph->left;
			charData->offsety = bitmap_glyph->top * -1;
			charData->width = bitmap.width;
			charData->height = bitmap.rows;
			charData->advancex = face->glyph->advance.x >> 6; // * 64
			charData->advancey = face->glyph->advance.y >> 6; // * 64
			charData->x = x;
			charData->y = y;
			charData->tw = x;
			charData->th = y;
			charData->tid = tex_base[ch];
			// Increment The Raster Position As If We Were A Bitmap Font.
			// (Only Needed If You Want To Calculate Text Length)
			//glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);



			FT_Done_Glyph(glyph);

		}


		void FTFontData::init(const char* fname, unsigned int h) {

			// allocate memory to store texture ids
			textures = new GLuint[128];
			characterDatas = new FTFontCharData[128];

			m_height = h;

			// create & initialise the freetype font library.
			FT_Library library;
			if (FT_Init_FreeType(&library)) {
				ErrorDialog::createAndShow("FT_Init_FreeType failed");
				exit(0);
			}

			// The object in which freetype holds information on a given font is called a "Face".
			FT_Face face;
			m_lineHeight = face->height;

			// Load in the font information from the file.
			// Code is most likely to die here as FT_New_Face
			// will fail if the font does not exist or is (somehow) broken.
			if (FT_New_Face(library, fname, 0, &face)) {
				ErrorDialog::createAndShow("FT_New_Face failed (there is probably a problem with your font file)");
				exit(0);
			}

			// FreeType measures font size in terms of 1/64ths of pixels.
			// so to make a font H pixels high, we need to make it h*64 or h << 6.
			FT_Set_Char_Size(face, h << 6, h << 6, 96, 96);

			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
			
			#else

				// here we ask opengl to allocate resources for all the
				// textures and display lists which we are about to create.
				list_base = glGenLists(128);
				glGenTextures(128, textures);

				// this is where we actually create each of the fonts display lists.
				for(unsigned char i = 0; i < 128; i++) {
					make_dlist(face, i, list_base, textures);
				}

			#endif

			// we fon#t need the face information now the the display lists have been created.
			// free the resources.
			FT_Done_Face(face);
			FT_Done_FreeType(library);
		}

		void FTFontData::clean() {
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
			#else
				glDeleteLists(list_base, 128);
			#endif
			glDeleteTextures(128, textures);
			delete[] textures;
		}


		FTFont::FTFont(string filename, unsigned int height):
			Font::Font(),
			m_data(NULL) {
			m_data = new FTFontData();
			m_data->init(filename.c_str(), height);
		}

		void FTFont::drawString(const string& str, int x, int y) {
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
			#else
				//GLuint font = m_data->list_base;
				//glEnable(GL_TEXTURE_2D);
				
				//glListBase(font);
				for( unsigned int i = 0; i < str.size(); ++i ) {
					unsigned char in = (unsigned char) str[i];
					FTFontCharData& d = m_data->characterDatas[in];
					glPushMatrix();
					glTranslatef(
						x + d.offsetx,
						y + d.offsety + m_data->m_height,
						0
					);
					// glCallLists(1, GL_UNSIGNED_BYTE, (GLvoid*) &in);
					//glBindTexture(GL_TEXTURE_2D, d.tid);
					RendererState::start(RendererState::TEXTURE, d.tid);
					glBegin(GL_QUADS);
						glTexCoord2d(0,0);       glVertex2f(0,0);
						glTexCoord2d(0,d.th);    glVertex2f(0,d.height);
						glTexCoord2d(d.tw,d.th); glVertex2f(d.width,d.height);
						glTexCoord2d(d.tw,0);    glVertex2f(d.width,0);
					glEnd();


					glPopMatrix();
					x += m_data->characterDatas[in].advancex;

				}
				//glBindTexture(GL_TEXTURE_2D, 0);
			#endif
		}
		unsigned int FTFont::getStringWidth(const string& Str) const {
			unsigned int total = 0;
			for (unsigned int i = 0; i < Str.size(); i++ ) {
				total += m_data->characterDatas[(int) Str[i]].advancex;
			}
			return total;
		}
		unsigned int FTFont::getStringHeight(const string& Str) const {
			unsigned int total = 0;
			for (unsigned int i = 0; i < Str.size(); i++ ) {
				total += m_data->characterDatas[(int) Str[i]].advancey;
			}
			return total;
		}
		unsigned int FTFont::getLineHeight() const {
			return (unsigned int) m_data->m_height;
		}

		FTFont::~FTFont() {
			m_data->clean();
			delete m_data;
		}
	}
}
