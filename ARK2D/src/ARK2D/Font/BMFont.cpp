/*
 * BMFont.cpp
 *
 *  Created on: 13-Apr-2010
 *      Author: Ashley
 */

#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
#include "BMFont.h"

#include "../Graphics/Color.h"
#include "../Graphics/Image.h"
#include "../Graphics/Texture.h"
#include "../Core/GameContainer.h"

#include "../vendor/utf8/utf8.h"


namespace ARK {
	namespace Font {

		BMFont::BMFont():
			#ifdef ARK2D_WINDOWS_VS
				ARK::Font::Font(),
			#else 
				Font::Font(),
			#endif 
			m_loaded(false),
			m_data(NULL),
			m_FontFile(""),
			m_ImageFile(""),
			m_Charset(),
			m_Image(NULL),
			m_letterImages(),
			m_kerning(0)
			{

		}

		BMFont::BMFont(unsigned int fntResource, unsigned int imgResource, unsigned int imgResourceType):
			#ifdef ARK2D_WINDOWS_VS
				ARK::Font::Font(),
			#else 
				Font::Font(),
			#endif 
			m_loaded(false),
			m_data(NULL),
			m_FontFile(""),
			m_ImageFile(""),
			m_Charset(),
			m_Image(NULL),
			m_letterImages(),
			m_kerning(0)
		{
			ARK2D::getLog()->i("Loading BMFont from raw data.");
			m_data = (char*) GameContainerPlatform::getARK2DResource(fntResource, ARK2D_RESOURCE_TYPE_FNT);
			m_Image = new Image(imgResource, imgResourceType);
			bool success = Parse();
			if (!success) {
				ARK2D::getLog()->w("Did not load fnt data successfully...");
			}
			ARK2D::getLog()->i("Done");
		}

		BMFont::BMFont(void* data, Image* i):
			#ifdef ARK2D_WINDOWS_VS
				ARK::Font::Font(),
			#else 
				Font::Font(),
			#endif 
			m_loaded(false),
			m_data(NULL),
			m_FontFile(""),
			m_ImageFile(""),
			m_Charset(),
			m_Image(NULL),
			m_letterImages(),
			m_kerning(0)
		{
			ARK2D::getLog()->i("Loading BMFont from raw data.");
			m_data = (char*) data;
			m_Image = i;
			bool success = Parse();
			if (!success) {
				ARK2D::getLog()->w("Did not load fnt data successfully...");
			}
			ARK2D::getLog()->i("Done");
		}

		BMFont::BMFont(const string& f, const string& i):
			#ifdef ARK2D_WINDOWS_VS
				ARK::Font::Font(),
			#else 
				Font::Font(),
			#endif 
			m_loaded(false),
			m_data(NULL),
			m_FontFile(f),
			m_ImageFile(i),
			m_Charset(),
			m_Image(NULL),
			m_letterImages(),
			m_kerning(0)
		{
			ARK2D::getLog()->i("Loading BMFont: ");
			ARK2D::getLog()->i(f);
			
			m_Image = ARK::Core::Resource::get(i)->asImage();
			bool success = Parse();
			if (!success) {
				ARK2D::getLog()->w("Did not load fnt data successfully...");
			}
			ARK2D::getLog()->i("Done");
		}

		BMFont::BMFont(const string& f, const string& i, const Color& mask):
			#ifdef ARK2D_WINDOWS_VS
				ARK::Font::Font(),
			#else 
				Font::Font(),
			#endif 
			m_loaded(false),
			m_data(NULL),
			m_FontFile(f),
			m_ImageFile(i),
			m_Charset(),
			m_Image(NULL), 
			m_letterImages(),
			m_kerning(0)
		{ 
			ARK2D::getLog()->i("Loading BMFont: ");
			ARK2D::getLog()->i(f);

			m_Image = new Image(i, mask);
			bool success = Parse();
			if (!success) {
				ARK2D::getLog()->w("Did not load fnt data successfully...");
			}
		}

		BMFont::~BMFont() {
			ARK2D::getLog()->i("Deleting BMFont");
			delete m_data;

			// @todo memory mangement: loop through m_letterImages and delete them.
		}
 
		Image* BMFont::getImage() const {
			return m_Image;
		}

		void BMFont::scale(float f) {
			//ARK2D::getLog()->e("BMFont scale not implemented.");
			//exit(0);

	/*		m_Charset.LineHeight = (unsigned int) (float(m_Charset.LineHeight) * f);
			m_Charset.Base = (unsigned int) (float(m_Charset.Base) * f);
			
			for(unsigned int i = 0; i <= 255; ++i) { //m_Charset.Chars.length
				m_Charset.Chars[i].Width = (unsigned int) (float(m_Charset.Chars[i].Width) * f);
				m_Charset.Chars[i].Height = (unsigned int) (float(m_Charset.Chars[i].Height) * f);
				m_Charset.Chars[i].XOffset = (unsigned int) (float(m_Charset.Chars[i].XOffset) * f);
				m_Charset.Chars[i].YOffset = (unsigned int) (float(m_Charset.Chars[i].YOffset) * f);
				m_Charset.Chars[i].XAdvance = (unsigned int) (float(m_Charset.Chars[i].XAdvance) * f);
			}*/
			//CharDescriptor Chars[256];


			m_Charset.LineHeight = m_Charset.LineHeight * f;
			m_Charset.Base = m_Charset.Base * f;
			
			for(unsigned int i = 0; i <= 255; ++i) { //m_Charset.Chars.length
				m_Charset.Chars[i].Width = m_Charset.Chars[i].Width * f;
				m_Charset.Chars[i].Height = m_Charset.Chars[i].Height * f;
				m_Charset.Chars[i].XOffset = m_Charset.Chars[i].XOffset * f;
				m_Charset.Chars[i].YOffset = m_Charset.Chars[i].YOffset * f;
				m_Charset.Chars[i].XAdvance = m_Charset.Chars[i].XAdvance * f;
			}
		}

		// Yarp!
		bool BMFont::Parse() // istream& Stream, Charset& CharsetDesc
		{
		#ifdef EXCEPTIONS_AVAILABLE
			try {
		#endif
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

					//#ifdef ARK2D_FLASCC
						//std::cout << Line << std::endl;
					//#endif

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
							Converter <<  Value;
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
						unsigned int CharID = 0;

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
									ARK2D::getLog()->w(StringUtil::append("Warning. Font includes character above ASCII values: ", CharID));
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
 
				for(unsigned int i = 0; i <= 255; ++i) { //m_Charset.Chars.length
					m_Charset.Chars[i].WidthOriginal = m_Charset.Chars[i].Width;
					m_Charset.Chars[i].HeightOriginal = m_Charset.Chars[i].Height;
				}

				m_loaded = true;
		#ifdef EXCEPTIONS_AVAILABLE
			} catch(...) {
				ErrorDialog::createAndShow("error loading font");
				return false;
			}
		#endif 
			ARK2D::getLog()->i("Loaded fnt data.");
			return true;
		}


		

		void BMFont::drawString(const string str, float x, float y, signed int alignX, signed int alignY, float rotation, float scale) {
			Renderer* r = ARK2D::getRenderer();
			r->setFont(this); 
			r->drawString(str, x, y, alignX, alignY, rotation, scale);
		}

        void BMFont::drawString(const string& Str, int drawx, int drawy) {
            drawString(Str, drawx, drawy, 0);
        }
        void BMFont::drawString(const string& Str, int drawx, int drawy, int drawz)
		{
			if (m_loaded == false) { return; } 
			if (Str.length() == 0) { return; }
			//return; 

			int CharX, CharY, Width, WidthOriginal, Height, HeightOriginal, OffsetX, OffsetY, XAdvance;

			#if (defined(ARK2D_OPENGL_3_2) || defined(ARK2D_OPENGL_ES_2_0) || defined(ARK2D_RENDERER_DIRECTX))

				const int numVerts = Str.length() * 6 * 3;
				const int numTVerts = Str.length() * 6 * 2;
				const int numCVerts = Str.length() * 6 * 4;
				
				

				#ifdef ARK2D_WINDOWS_VS
					float* rawVertices = (float*)alloca(numVerts * sizeof(float));
                    float* rawNormals = (float*)alloca(numVerts * sizeof(float));
					float* rawTextureCoords = (float*)alloca(numTVerts * sizeof(float));
					unsigned char* rawColors = (unsigned char*)alloca(numCVerts * sizeof(unsigned char));

					//Assert(rawVertices);
					//Assert(rawTextureCoords);
					//Assert(rawColors);
				#else
					float rawVertices[numVerts];
                    float rawNormals[numVerts];
					float rawTextureCoords[numTVerts];
					unsigned char rawColors[numCVerts];
				#endif


				Renderer* r = ARK2D::getRenderer();
				unsigned char color_r = r->getDrawColor().getRedc();
				unsigned char color_g = r->getDrawColor().getGreenc();
				unsigned char color_b = r->getDrawColor().getBluec();
				unsigned char color_a = (unsigned char) ((m_Image->getAlpha() * r->getDrawColor().getAlphaf()) * 255);

				if (m_Image->getColor() != NULL) { 
					color_r = (unsigned char) (m_Image->getColor()->getRedc() * r->getDrawColor().getRedf());
					color_g = (unsigned char) (m_Image->getColor()->getGreenc() * r->getDrawColor().getGreenf());
					color_b = (unsigned char) (m_Image->getColor()->getBluec() * r->getDrawColor().getBluef());
					color_a = (unsigned char) ((m_Image->getColor()->getAlphaf() * m_Image->getAlpha() * r->getDrawColor().getAlphaf()) * 255);
				}

				/*if (Renderer::isBatching()) {
					for( unsigned int i = 0; i < Str.length(); ++i )
					{
						int charid = (int) Str[i];
						CharX = m_Charset.Chars[charid].x;
						CharY = m_Charset.Chars[charid].y;
						Width = m_Charset.Chars[charid].Width;
						WidthOriginal = m_Charset.Chars[charid].WidthOriginal;
						Height = m_Charset.Chars[charid].Height;
						HeightOriginal = m_Charset.Chars[charid].HeightOriginal;
						OffsetX = m_Charset.Chars[charid].XOffset;
						OffsetY = m_Charset.Chars[charid].YOffset;
						XAdvance = m_Charset.Chars[charid].XAdvance;

						float drawx2 = drawx+OffsetX + (i*m_kerning);
						float drawy2 = drawy+OffsetY;

						float charXPC = ((CharX / float(m_Image->getWidth())) * m_Image->getTextureW()) + m_Image->getTextureX();
						float charYPC = ((CharY / float(m_Image->getHeight())) * m_Image->getTextureH()) + m_Image->getTextureY();
						float widthPC = ((WidthOriginal / float(m_Image->getWidth())) * m_Image->getTextureW());
						float heightPC = ((HeightOriginal / float(m_Image->getHeight())) * m_Image->getTextureH());

						r->getBatch()->addTexturedQuad(
							m_Image->getTexture()->getId(),
							drawx2, drawy2,
							drawx2 + Width, drawy2,
							drawx2, drawy2 + Height,
							drawx2 + Width, drawy2 + Height,

							charXPC, charYPC, 
							charXPC + widthPC, charYPC, 
							charXPC, charYPC + heightPC,
							charXPC + widthPC, charYPC + heightPC,

							color_r, color_g, color_b, color_a,
							color_r, color_g, color_b, color_a,
							color_r, color_g, color_b, color_a,
							color_r, color_g, color_b, color_a
						);
						
					}
					return;
				}*/

				//signed int i = 0;
				//std::wstring::const_iterator pos = Str.begin();
				//while (pos != Str.end())
//				char* u8Str = (char*) Str.c_str();
//				unsigned int len2 = utf8string::u8_strlen(u8Str);

//				unsigned int len = Str.length();


//				ARK2D::getLog()->e(Str);
//                ARK2D::getLog()->e(Cast::toString<unsigned int>(len));
//				ARK2D::getLog()->e(Cast::toString<unsigned int>(len2));
            
                char* str = (char*)Str.c_str();    // utf-8 string
                char* str_i = str;                  // string iterator
                char* end = str+strlen(str)+1;      // end iterator

                signed int len = StringUtil::utf8strlen(str);
				//signed int i = 0;
              //  unsigned int charid;
				//for(signed int i = 0; i < len; ++i)
				for(signed int i = 0; i < len; ++i) { 
					//charid = utf8string::u8_nextchar(u8Str, &i);
                    uint32_t charid = utf8::next(str_i, end);

					//if (i == len) {
					if (charid == 0) {
						continue;
					}


					//int charid = (int) *pos;
					//unsigned int charid = utf8string::u8_nextchar(u8Str, &i);
					//unsigned int charid = Str[i];

					//ARK2D::getLog()->e(Cast::toWideString<int>(charid));
					CharX = m_Charset.Chars[charid].x;
					CharY = m_Charset.Chars[charid].y;
					Width = m_Charset.Chars[charid].Width;
					WidthOriginal = m_Charset.Chars[charid].WidthOriginal;
					Height = m_Charset.Chars[charid].Height;
					HeightOriginal = m_Charset.Chars[charid].HeightOriginal;
					OffsetX = m_Charset.Chars[charid].XOffset;
					OffsetY = m_Charset.Chars[charid].YOffset;
					XAdvance = m_Charset.Chars[charid].XAdvance;
  
					float charXPC = ((CharX / float(m_Image->getWidth())) * m_Image->getTextureW()) + m_Image->getTextureX();
					float charYPC = ((CharY / float(m_Image->getHeight())) * m_Image->getTextureH()) + m_Image->getTextureY();
					float widthPC = ((WidthOriginal / float(m_Image->getWidth())) * m_Image->getTextureW());
					float heightPC = ((HeightOriginal / float(m_Image->getHeight())) * m_Image->getTextureH());

					int vert = (i * 18);
					int vertt = (i * 12);
					int vertc = (i * 24);
                    
                    // tl
					rawVertices[vert] = drawx+OffsetX + (i*m_kerning);
					rawVertices[vert+1] = drawy+OffsetY;
                    rawVertices[vert+2] = drawz;

                    rawNormals[vert]   = 0;
                    rawNormals[vert+1] = 0;
                    rawNormals[vert+2] = 1;
					
					rawTextureCoords[vertt] = charXPC;
					rawTextureCoords[vertt+1] = charYPC;

					rawColors[vertc] = color_r;
					rawColors[vertc+1] = color_g;
					rawColors[vertc+2] = color_b;
					rawColors[vertc+3] = color_a;


					// tr
					rawVertices[vert+3] = drawx+OffsetX + (i*m_kerning) + Width;
					rawVertices[vert+4] = drawy+OffsetY;
                    rawVertices[vert+5] = drawz;

					rawNormals[vert+3] = 0;
                    rawNormals[vert+4] = 0;
                    rawNormals[vert+5] = 1;

					rawTextureCoords[vertt+2] = charXPC + widthPC;
					rawTextureCoords[vertt+3] = charYPC;

					rawColors[vertc+4] = color_r;
					rawColors[vertc+5] = color_g;
					rawColors[vertc+6] = color_b;
					rawColors[vertc+7] = color_a;


					// bl
					rawVertices[vert+6] = drawx+OffsetX + (i*m_kerning);
					rawVertices[vert+7] = drawy+OffsetY + Height;
                    rawVertices[vert+8] = drawz;

                    rawNormals[vert+6] = 0;
                    rawNormals[vert+7] = 0;
                    rawNormals[vert+8] = 1;

					rawTextureCoords[vertt+4] = charXPC;
					rawTextureCoords[vertt+5] = charYPC + heightPC;

					rawColors[vertc+8] = color_r;
					rawColors[vertc+9] = color_g;
					rawColors[vertc+10] = color_b;
					rawColors[vertc+11] = color_a;


					// bl
					rawVertices[vert+9] = drawx+OffsetX + (i*m_kerning);
					rawVertices[vert+10] = drawy+OffsetY + Height;
                    rawVertices[vert+11] = drawz;

                    rawNormals[vert+9] = 0;
                    rawNormals[vert+10] = 0;
                    rawNormals[vert+11] = 1;

					rawTextureCoords[vertt+6] = charXPC;
					rawTextureCoords[vertt+7] = charYPC + heightPC;

					rawColors[vertc+12] = color_r;
					rawColors[vertc+13] = color_g;
					rawColors[vertc+14] = color_b;
					rawColors[vertc+15] = color_a;


					// tr
					rawVertices[vert+12] = drawx+OffsetX + (i*m_kerning) + Width;
					rawVertices[vert+13] = drawy+OffsetY;
                    rawVertices[vert+14] = drawz;

                    rawNormals[vert+12] = 0;
                    rawNormals[vert+13] = 0;
                    rawNormals[vert+14] = 1;

					rawTextureCoords[vertt+8] = charXPC + widthPC;
					rawTextureCoords[vertt+9] = charYPC;

					rawColors[vertc+16] = color_r;
					rawColors[vertc+17] = color_g;
					rawColors[vertc+18] = color_b;
					rawColors[vertc+19] = color_a;


					// br
					rawVertices[vert+15] = drawx+OffsetX + (i*m_kerning) + Width;
					rawVertices[vert+16] = drawy+OffsetY + Height;
                    rawVertices[vert+17] = drawz;
					
					rawNormals[vert+15] = 0;
                    rawNormals[vert+16] = 0;
                    rawNormals[vert+17] = 1;

					rawTextureCoords[vertt+10] = charXPC + widthPC;
					rawTextureCoords[vertt+11] = charYPC + heightPC;

					rawColors[vertc+20] = color_r;
					rawColors[vertc+21] = color_g;
					rawColors[vertc+22] = color_b;
					rawColors[vertc+23] = color_a;
                    
                  	// multiply position coordinates (???)
					drawx += XAdvance;
					//++pos;
					//++i;
					//i++;
					
                } //while ( str_i < end );

				//for( signed int i = 0; i < (signed int) Str.length(); ++i )
				//{
				//}
				r->texturedTriangles(m_Image->getTexture()->getId(), rawVertices, rawNormals, rawTextureCoords, rawColors, Str.length()*2, false);
			
				//#ifdef ARK2D_WINDOWS_VS
				//	free(rawVertices);
				//	free(rawTextureCoords);
				//	free(rawColors);
				//#endif

			#else 

				m_Image->drawSubImageStart();
				for( unsigned int i = 0; i < Str.length(); ++i )
				{
					int charid = (int) Str.at(i);
					CharX = m_Charset.Chars[charid].x;
					CharY = m_Charset.Chars[charid].y;
					Width = m_Charset.Chars[charid].Width;
					WidthOriginal = m_Charset.Chars[charid].WidthOriginal;
					Height = m_Charset.Chars[charid].Height;
					HeightOriginal = m_Charset.Chars[charid].HeightOriginal;
					OffsetX = m_Charset.Chars[charid].XOffset;
					OffsetY = m_Charset.Chars[charid].YOffset;
					XAdvance = m_Charset.Chars[charid].XAdvance;

					m_Image->drawSubImage(CharX, CharY, drawx+OffsetX + (i*m_kerning), drawy+OffsetY, WidthOriginal, HeightOriginal, Width, Height);
					drawx += XAdvance;
				}
				m_Image->drawSubImageEnd();
			#endif
		
		}

		void BMFont::setKerning(int k) {
			m_kerning = k;
		}
		float BMFont::getCharXAdvance(unsigned char ch) {
			return m_Charset.Chars[ch].XAdvance;
		}
		float BMFont::getCharXOffset(unsigned char ch) {
			return m_Charset.Chars[ch].XOffset;
		}
 
 		unsigned int BMFont::getStringWidth(const string& Str) const {
 			if (m_loaded == false) { return 0; }

            char* str_i = (char*) Str.c_str();
            char* end = str_i+strlen(str_i)+1;
 			signed int len = StringUtil::utf8strlen(str_i);
            
            unsigned int total = 0;
			for (signed int i = 0; i < len; i++ )
            {
				uint32_t charid = utf8::next(str_i, end);
				if (charid == 0) { continue; }

				total += m_Charset.Chars[charid].XAdvance;
			}

			total += (len-1) * m_kerning;
			return total;
 		}
		unsigned int BMFont::getStringHeight(const string& Str) const {
			if (m_loaded == false) { return 0; }
            
            char* str_i = (char*) Str.c_str();
            char* end = str_i+strlen(str_i)+1;
            unsigned int len = StringUtil::utf8strlen(str_i);

			unsigned int max = 0;
			for (unsigned int i = 0; i < len; i++ )
            {
                uint32_t charid = utf8::next(str_i, end);
                if (charid == 0) { continue; }
                
				if ((m_Charset.Chars[charid].Height + m_Charset.Chars[charid].YOffset) > max) {
					max = (m_Charset.Chars[charid].Height + m_Charset.Chars[charid].YOffset);
				}
			}
			return max;
		}
		
		unsigned int BMFont::getLineHeight() const {
			if (m_loaded == false) { return 0; }

			return m_Charset.LineHeight;
		}
	}
}

