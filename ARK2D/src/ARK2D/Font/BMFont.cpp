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
#include "../Graphics/Image.h"
#include "../Core/GameContainer.h"

#include "BMFont.h"

namespace ARK {
	namespace Font {

		BMFont::BMFont():
			Font::Font(),
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
			Font::Font(),
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
			Font::Font(),
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
			Font::Font(),
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
			Font::Font(),
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

		void BMFont::drawString(const std::string str, float x, float y, signed int alignX, signed int alignY, float rotation, float scale) {
			ARK2D::getRenderer()->setFont(this);
			ARK2D::getRenderer()->drawString(str, x, y, alignX, alignY, rotation, scale);
		}

		// remember that u and v are width and height, respectively.
		void BMFont::drawString(const string& Str, int drawx, int drawy) 
		{
			if (m_loaded == false) { return; }
			//ARK2D::getLog()->i("BMFont:: drawing string: ");
			//ARK2D::getLog()->i(Str);

			int CharX, CharY, Width, WidthOriginal, Height, HeightOriginal, OffsetX, OffsetY, XAdvance;
			//m_Image.getSubImage(10, 10, 10, 10).draw(drawx, drawy);

		//	int TotalX = 0;
			for( unsigned int i = 0; i < Str.size(); ++i )
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

				/*Image* img = NULL;
				if (m_letterImages.find(charid) != m_letterImages.end()) {
					img = (Image*) m_letterImages.find(charid)->second;
				} else { 
					ARK2D::getLog()->i("new letter cache");
					img = m_Image->getSubImage(CharX, CharY, Width, Height);
					m_letterImages[charid] = img;
				} 
				img->setAlpha(m_Image->getAlpha());
				img->draw(drawx + OffsetX, drawy + OffsetY);*/

				m_Image->drawSubImageStart();
				m_Image->drawSubImage(CharX, CharY, drawx+OffsetX + (i*m_kerning), drawy+OffsetY, WidthOriginal, HeightOriginal, Width, Height);
				m_Image->drawSubImageEnd();


				/// ******
				/// @todo:
				/// 		- optimise this by making a single vector with triangle strip.
				/// 		- reduce calls to opengl by N relative to string length. ;D
				/// ******

				//TotalX += Width;
				drawx += XAdvance;
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

		void BMFont::setKerning(int k) {
			m_kerning = k;
		}
 
		unsigned int BMFont::getStringWidth(const string& Str) const {
			if (m_loaded == false) { return 0; }

			unsigned int total = 0;
			for (unsigned int i = 0; i < Str.size(); i++ ) {
				//total += m_Charset.Chars[(int) Str[i]].Width;
				//total += m_Charset.Chars[(int) Str[i]].XOffset;
				total += m_Charset.Chars[(int) Str[i]].XAdvance;
			}
			total += (Str.size()-1) * m_kerning;
			return total;
		}
		unsigned int BMFont::getStringHeight(const string& Str) const {
			if (m_loaded == false) { return 0; }

			unsigned int max = 0;
			for (unsigned int i = 0; i < Str.size(); i++ ) {
				if ((m_Charset.Chars[(int) Str[i]].Height + m_Charset.Chars[(int) Str[i]].YOffset) > max) {
					max = (m_Charset.Chars[(int) Str[i]].Height + m_Charset.Chars[(int) Str[i]].YOffset);
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

