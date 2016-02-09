/*
 * Image.cpp
 *
 *  Created on: 06-Nov-2009
 *      Author: user
 */

#include "../ARK2D.h"

#include "Image.h"
#include "ImageIO/TargaImage.h"
#include "ImageIO/BMPImage.h"
#include "ImageIO/PNGImage.h"
#include "../Util/StringUtil.h" 
#include "Color.h"
#include "SpriteSheetDescriptionItem.h"

//#include "../Includes.h"
#include "../Core/GameContainer.h"
#include "../Core/Resource.h"
#include "Texture.h"
#include "TextureStore.h" 
#include "Shader.h"

#include "../vendor/FileInterface.h"
#include "../Common/OpenGL.h"

#define bswap16(x) ((x)>>8 | ((x)&255)<<8)

namespace ARK {
	namespace Graphics {

		unsigned int Image::s_dxNextTextureId = 0;
 
		GLuint Image::load(const Color& mask) {
 
			unsigned int thisDataType = 0;
			if (m_data != NULL) {
				thisDataType = m_resourceType;
			} else {
				string ext = StringUtil::getExtension(this->filename);
				if (ext == "tga") {
					thisDataType = ARK2D_RESOURCE_TYPE_TGA;
				} else if (ext == "bmp") {
					thisDataType = ARK2D_RESOURCE_TYPE_BMP;
				} else if (ext == "png") {
					thisDataType = ARK2D_RESOURCE_TYPE_PNG;
				} else if (ext == "pkm_png") {
					thisDataType = ARK2D_RESOURCE_TYPE_PKM;
				}
			}


			if (thisDataType == ARK2D_RESOURCE_TYPE_TGA) {
				//std::cout << "Loading TGA!" << std::endl;
				TargaImage* tga = loadTGA();
				ARK2D::getLog()->v("loaded a bit.");

				if (tga == NULL) {
					ARK2D::getLog()->e("Something has gone terribly wrong loading that TGA file. :(");
					return 0;
				}

				m_Width = tga->getWidth();
				m_Height = tga->getHeight();
				m_originalWidth = m_Width;
				m_originalHeight = m_Height;

				ARK2D::getLog()->v(StringUtil::append("width: ", m_Width));
				ARK2D::getLog()->v(StringUtil::append("height: ", m_Height));

				ARK2D::getLog()->v("loaded a bit more.");

				unsigned int Object(0);

				#if defined(ARK2D_RENDERER_OPENGL)

					// Generate one texture (we're creating only one).
					glGenTextures(1, &Object);
					showAnyGlErrorAndExitMacro();

					// Bind
					RendererState::internalBindTexture(Object);

					// You can use these values to specify mipmaps if you want to, such as 'GL_LINEAR_MIPMAP_LINEAR'.
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Renderer::getInterpolationGL());
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Renderer::getInterpolationGL());

					// Create the actual texture object.
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->getWidth(), this->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, tga->getImageData());

					// Don't forget to unbind the texture now...
					RendererStats::s_textureAllocatedBytes += int(m_Width * m_Height * 3 * 8);
					RendererState::internalBindTexture(0);
					showAnyGlErrorAndExitMacro();

				#elif defined(ARK2D_RENDERER_DIRECTX)

					ARK2D::getLog()->v("loaded a bit more again... somehow?");

					// different size to image. need moar memory.
					const unsigned char* pixelData = tga->getImageData();
					unsigned char* newdata = (unsigned char*)malloc(m_Width * m_Height * 4);
					int offset = 0;
					int oldOffset = 0;
					for (unsigned int i = 0; i < m_Width; i++) {
						for (unsigned int j = 0; j < m_Height; j++) {

							

							if (i >= m_Height || j >= m_Width) {
								newdata[offset] = (unsigned char)0;
								newdata[offset + 1] = (unsigned char)0;
								newdata[offset + 2] = (unsigned char)0;
								newdata[offset + 3] = (unsigned char)0;
							}
							else if ((unsigned int)((unsigned char)pixelData[oldOffset]) == mask.getRed() &&
								(unsigned int)((unsigned char)pixelData[oldOffset + 1]) == mask.getGreen() &&
								(unsigned int)((unsigned char)pixelData[oldOffset + 2]) == mask.getBlue()) {
								//std::cout << "replacing " << j << ":" << i << std::endl;
								newdata[offset] = (unsigned char)0;
								newdata[offset + 1] = (unsigned char)0;
								newdata[offset + 2] = (unsigned char)0;
								newdata[offset + 3] = (unsigned char)0;
								oldOffset += 3;
							}
							else {
								memcpy((newdata + offset), &pixelData[oldOffset], 3);
								newdata[offset + 3] = 255;
								oldOffset += 3;
							}
							offset += 4;
						}
					}

					offset = 0;
					for (unsigned int i = 0; i < m_Width; i++) {
						for (unsigned int j = 0; j < m_Height; j++) {

							if (i == 0 && j == 0) {
								// print first color...
								string ppp = "{";
								ppp += string("r: "); ppp += Cast::toString<unsigned int>((unsigned int)newdata[offset]);   ppp += string(", ");
								ppp += string("g: "); ppp += Cast::toString<unsigned int>((unsigned int)newdata[offset + 1]); ppp += string(", ");
								ppp += string("b: "); ppp += Cast::toString<unsigned int>((unsigned int)newdata[offset + 2]); ppp += string(", ");
								ppp += string("a: "); ppp += Cast::toString<unsigned int>((unsigned int)newdata[offset + 2]); ppp += string(", ");
								ppp += string("}");
								ARK2D::getLog()->v(StringUtil::append("tl color: ", ppp));
							}
							else if (i == 10 && j == 10) {
								string ppp = "{";
								ppp += string("r: "); ppp += Cast::toString<unsigned int>((unsigned int)newdata[offset]);   ppp += string(", ");
								ppp += string("g: "); ppp += Cast::toString<unsigned int>((unsigned int)newdata[offset + 1]); ppp += string(", ");
								ppp += string("b: "); ppp += Cast::toString<unsigned int>((unsigned int)newdata[offset + 2]); ppp += string(", ");
								ppp += string("a: "); ppp += Cast::toString<unsigned int>((unsigned int)newdata[offset + 2]); ppp += string(", ");
								ppp += string("}");
								ARK2D::getLog()->v(StringUtil::append("10,10 color: ", ppp));
							} 
							else if (i == 10 && j == 244) {
								string ppp = "{";
								ppp += string("r: "); ppp += Cast::toString<unsigned int>((unsigned int)newdata[offset]);   ppp += string(", ");
								ppp += string("g: "); ppp += Cast::toString<unsigned int>((unsigned int)newdata[offset + 1]); ppp += string(", ");
								ppp += string("b: "); ppp += Cast::toString<unsigned int>((unsigned int)newdata[offset + 2]); ppp += string(", ");
								ppp += string("a: "); ppp += Cast::toString<unsigned int>((unsigned int)newdata[offset + 2]); ppp += string(", ");
								ppp += string("}");
								ARK2D::getLog()->v(StringUtil::append("10,10 color: ", ppp));
							}

							newdata[offset] = MathUtil::randBetween(0, 255);
							newdata[offset+1] = MathUtil::randBetween(0, 255);
							newdata[offset+2] = MathUtil::randBetween(0, 255);
							newdata[offset+3] = 255;

							offset += 4;
						}
					}

					Object = createDXTexture(m_Width, m_Height, newdata); 
					RendererStats::s_textureAllocatedBytes += (m_Width * m_Height * 8 * 4);

					free(newdata);
					
				#endif

				// Free TGA memory.
				ARK2D::getLog()->v("Freeing up TGA memory");
				delete(tga);

				RendererStats::s_glCalls += 4;

				// and finally return
				return Object;
			} else if (thisDataType == ARK2D_RESOURCE_TYPE_BMP) {
				//std::cout << "Loading BMP!" << std::endl;
				BMPImage* bmp = loadBMP();
				m_Width = bmp->Width;
				m_Height = bmp->Height;
				m_originalWidth = m_Width;
				m_originalHeight = m_Height;


				// generate a new Raster from the BMP information. this one has an alpha channel!
				unsigned int OldRasterSize = (bmp->BytesPerRow * bmp->Height);
				unsigned int NewRasterSize = ((bmp->BytesPerRow * bmp->Height) / 3) * 4;

				//std::cout << OldRasterSize << " : " << NewRasterSize << std::endl;
				//std::cout << bmp->BPP << std::endl;

				//char NewRaster[NewRasterSize];	// 32 bits raster, yes!
				char* NewRaster = (char*) malloc(NewRasterSize);

				int NewRasterIndex = 0;
				for (unsigned int i = 0; i < OldRasterSize; i += 3) {
					unsigned int b = ((unsigned int) (unsigned char) bmp->Raster[i]); // blue because BMPs stores pixels in BGR not RGB
					unsigned int g = ((unsigned int) (unsigned char) bmp->Raster[i + 1]);
					unsigned int r = ((unsigned int) (unsigned char) bmp->Raster[i + 2]);

					NewRaster[NewRasterIndex] = (unsigned char) r;
					NewRaster[NewRasterIndex + 1] = (unsigned char) g;
					NewRaster[NewRasterIndex + 2] = (unsigned char) b;
					NewRaster[NewRasterIndex + 3] = (unsigned char) 255;
					// apply mask
					if (r == mask.getRed() && g == mask.getGreen() && b == mask.getBlue()) {
						NewRaster[NewRasterIndex + 3] = (unsigned char) 0;
					} 
					NewRasterIndex += 4;
				}

				//std::cout << "Width: " << m_Width << ". Height: " << m_Width << std::endl;

				unsigned Object(0);
				
				#if defined(ARK2D_RENDERER_OPENGL)

					// Generate one texture (we're creating only one).
					glGenTextures(1, &Object);
					showAnyGlErrorAndExitMacro();

					// Bind
					RendererState::internalBindTexture(Object);

					// You can use these values to specify mipmaps if you want to, such as 'GL_LINEAR_MIPMAP_LINEAR'.
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Renderer::getInterpolationGL());
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Renderer::getInterpolationGL());

					// Create the actual texture object.
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int) m_Width, (int) m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NewRaster);

					// Don't forget to unbind the texture now.
					RendererStats::s_textureAllocatedBytes += int(m_Width * m_Height * 4 * 8);
					RendererState::internalBindTexture(0);
					showAnyGlErrorAndExitMacro();

				#elif defined(ARK2D_RENDERER_DIRECTX)
					
					//gl2dx::OpenGL::getInstance()->_device->SetSamplerState(IDirect3DDevice9, )
					Object = createDXTexture(m_Width, m_Height, NewRaster); 

				#endif

				// Free BMP memory. 
				free(NewRaster);
				delete(bmp);

				RendererStats::s_glCalls += 4;

				// and return!
				return Object;
			} 
			else if (thisDataType == ARK2D_RESOURCE_TYPE_PKM) 
			{
				
				#if ( defined(ARK2D_ANDROID)  )
				
					/*
						http://forums.arm.com/index.php?/topic/15835-pkm-header-format/
						4 byte magic number: "PKM "
						2 byte version "10"
						2 byte data type: 0 (ETC1_RGB_NO_MIPMAPS)
						16 bit big endian extended width
						16 bit big endian extended height
						16 bit big endian original width
						16 bit big endian original height
						data, 64bit big endian words.
					*/ 
					char* buffer = NULL;
					long lSize = 0;
					if (m_data == NULL) {
						FILE* fp = fopen(this->filename.c_str(), "r");

						fseek (fp, 0, SEEK_END);
						lSize = ftell (fp);
						rewind (fp);

						buffer = (char*) malloc(sizeof(char) * lSize);
						fread (buffer, 1, lSize, fp);
						
						fclose(fp);
					} else {
						buffer = (char*) m_data;
						lSize = (long) m_dataLength;
					}

					FILE_INTERFACE* fp = fi_fopen("", "r", buffer, lSize);
					
					char stuff[8];
					fi_fread((void*) &stuff, sizeof(char), 8, fp);

					uint16_t width = 0;
					uint16_t height = 0; 
					uint16_t extended_width = 0;
					uint16_t extended_height = 0; 	

					fi_fread((void*) &width, sizeof(uint16_t), 1, fp);
					fi_fread((void*) &height, sizeof(uint16_t), 1, fp);

					fi_fread((void*) &extended_width, sizeof(uint16_t), 1, fp);
					fi_fread((void*) &extended_height, sizeof(uint16_t), 1, fp);

					m_Width = bswap16(width);
					m_Height = bswap16(height);
					m_originalWidth = m_Width;
					m_originalHeight = m_Height;

					ARK2D::getLog()->v(StringUtil::append("w: ", m_Width));
					ARK2D::getLog()->v(StringUtil::append("h: ", m_Height));

					size_t result;
					char* pcData = (char*) malloc(lSize - 16);
					result = fi_fread(pcData, sizeof(char), lSize - 16, fp);
					if (result != lSize - 16) {
						ARK2D::getLog()->e("PKM: Reading error");
						exit(1);
					}

					//Now generate the OpenGL texture object
					ARK2D::getLog()->v("generating texture");
					unsigned Object(0);
					
					glGenTextures(1, &Object);
					showAnyGlErrorAndExitMacro();

					ARK2D::getLog()->v("binding texture");
					RendererState::internalBindTexture(Object);
					showAnyGlErrorAndExitMacro();

					ARK2D::getLog()->v("setting texture data");
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Renderer::getInterpolationGL());
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Renderer::getInterpolationGL());
					
					GLuint uiSize = 8 * ((int(m_Width) + 3) >> 2) * ((int(m_Height) + 3) >> 2);
					glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, m_Width, m_Height, 0, uiSize, pcData);
					//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempTextureWidth, tempTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, newdata);
					
					showAnyGlErrorAndExitMacro();  

					RendererStats::s_textureAllocatedBytes += uiSize;

					ARK2D::getLog()->v("unbinding texture");
					RendererState::internalBindTexture(0);
					//texture = Object;

					showAnyGlErrorAndExitMacro();

					free(pcData); 

					if (m_data == NULL) {
						free(buffer);
					}

					RendererStats::s_glCalls += 4;

					return Object;

				#else
					ARK2D::getLog()->e("PKM only supported on OPENGL ES platforms. ");
				#endif
				
				return 0;
					

				
			}
			else if (thisDataType == ARK2D_RESOURCE_TYPE_PNG) 
			{
				//std::cout << "Loading PNG!" << std::endl;
				PNGImage* png = loadPNG();

				unsigned char* pixelData = (unsigned char*) png->getImageData();
				m_Width = png->getWidth();
				m_Height = png->getHeight();
				m_originalWidth = m_Width;
				m_originalHeight = m_Height;

				//std::cout << "w: " << m_Width << " h: " << m_Height << std::endl;

				

				// apply mask
				unsigned int offset = 0;
				/*for (unsigned int i = 0; i < m_Height; i++) {
					for (unsigned int j = 0; j < m_Width; j++) {
						unsigned int r = ((unsigned int) (unsigned char) pixelData[offset]);
						unsigned int g = ((unsigned int) (unsigned char) pixelData[offset+1]);
						unsigned int b = ((unsigned int) (unsigned char) pixelData[offset+2]);
						unsigned int a = ((unsigned int) (unsigned char) pixelData[offset+3]);

						//unsigned int r = (unsigned int) pixelData[offset];
						//unsigned int g = (unsigned int) pixelData[offset+1];
						//unsigned int b = (unsigned int) pixelData[offset+2];
						//unsigned int a = (unsigned int) pixelData[offset+3];


						//if (i == 0 && j == 0) { // top left pixel
							//std::cout << "tl pixel data- " << "r: " << r << " g: " << g << " b: " << b << " a: " << a << std::endl;
							//std::cout << "mask data- " << "r: " << mask.getRed() << " g: " << mask.getGreen() << " b: " << mask.getBlue() << " a: " << mask.getAlpha() << std::endl;
						//}

						if (r == mask.getRed() && g == mask.getGreen() && b == mask.getBlue() && a == mask.getAlpha()) {
							//std::cout << "replacing " << j << ":" << i << std::endl;
							pixelData[offset] = (unsigned char) 0;
							pixelData[offset+1] = (unsigned char) 0;
							pixelData[offset+2] = (unsigned char) 0;
							pixelData[offset+3] = (unsigned char) 0;
						}

						//if (i == 0 && j == 0) {
						//	std::cout << "after" << std::endl;
						//	std::cout << "tl pixel data- " << "r: " << r << " g: " << g << " b: " << b << " a: " << a << std::endl;
						//	std::cout << "mask data- " << "r: " << mask.getRed() << " g: " << mask.getGreen() << " b: " << mask.getBlue() << " a: " << mask.getAlpha() << std::endl;
						//}

						offset += 4;
					}
				}*/

				//! auto-power-of-twoing
				unsigned int tempTextureWidth = MathUtil::nextPowerOfTwo((unsigned int) m_Width);
				unsigned int tempTextureHeight = MathUtil::nextPowerOfTwo((unsigned int) m_Height);

				ARK2D::getLog()->v(StringUtil::append("new width:", tempTextureWidth));
				ARK2D::getLog()->v(StringUtil::append("new height:", tempTextureHeight));

				// 
				// check it can fit! and is compatible with hardware!
				// 
				// http://www.opengl.org/archives/resources/faq/technical/texture.htm
				// 21.130 What's the maximum size texture map my device will render hardware accelerated?

                ARK2D::getLog()->v("check texture is compatible with hardware...");
				#if !defined(ARK2D_IPHONE) && !defined(ARK2D_ANDROID) && !defined(ARK2D_FLASCC) && !defined(ARK2D_EMSCRIPTEN_JS) && !defined(ARK2D_WINDOWS_PHONE_8) && !defined(ARK2D_RENDERER_DIRECTX)
                
                    GLint loltexcompat_w = tempTextureWidth;
                    GLint loltexcompat_h = tempTextureHeight;
                    GLint compatibleTexSize;
                    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &compatibleTexSize);
                    glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_RGBA, loltexcompat_w, loltexcompat_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

                    GLint textureIsCompatible;
                    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureIsCompatible);
                    if (textureIsCompatible == 0) {
                        String errs("Texture of size: ");
                        errs += (unsigned int) tempTextureWidth;
                        errs += " could not be created. :( Max texture size is: ";
                        errs += (unsigned int) compatibleTexSize;
                        ErrorDialog::createAndShow(errs.get()); 
                        exit(0);
                    }
                
                #endif



				float newTextureWidth =  float(m_Width) / float(tempTextureWidth);
				float newTextureHeight =  float(m_Height) / float(tempTextureHeight);
				//this->texture_width = newTextureWidth;
				//this->texture_height = newTextureHeight;
				texture_offset_x_tl = 0.0f;
				texture_offset_y_tl = 0.0f;
				texture_offset_x_tr = newTextureWidth;
				texture_offset_y_tr = 0.0f;
				texture_offset_x_bl = 0.0f;
				texture_offset_y_bl = newTextureHeight;
				texture_offset_x_br = newTextureWidth;
				texture_offset_y_br = newTextureHeight;


				if (png->isRGBA()) {
					ARK2D::getLog()->v("rgba png");

					unsigned char* newdata = NULL;
					bool usesNewMemory = false;
					if (tempTextureWidth == m_Width && tempTextureHeight == m_Height) {
                		// same size. don't need to allocate new memory, only change existing memory.
                		newdata = pixelData;
                		offset = 0;
						int oldOffset = 0;
						for (unsigned int i = 0; i < tempTextureHeight; i++) {
							for (unsigned int j = 0; j < tempTextureWidth; j++) {
								if ((unsigned int) ((unsigned char) pixelData[oldOffset]) == mask.getRed() &&
										(unsigned int) ((unsigned char) pixelData[oldOffset+1]) == mask.getGreen() &&
										(unsigned int) ((unsigned char) pixelData[oldOffset+2]) == mask.getBlue() &&
										(unsigned int) ((unsigned char) pixelData[oldOffset+3]) == mask.getAlpha()) {
									//std::cout << "replacing " << j << ":" << i << std::endl;
									pixelData[offset] = (unsigned char) 0;
									pixelData[offset+1] = (unsigned char) 0;
									pixelData[offset+2] = (unsigned char) 0; 
									pixelData[offset+3] = (unsigned char) 0;
									oldOffset += 4;
								} else {
									oldOffset += 4;
								}
								offset += 4;
							}
						}

					} else {
						// different size to image. need moar memory.
						usesNewMemory = true;
						newdata = (unsigned char*) malloc(tempTextureWidth * tempTextureHeight * 4);
						offset = 0;
						int oldOffset = 0;
						for (unsigned int i = 0; i < tempTextureHeight; i++) {
							for (unsigned int j = 0; j < tempTextureWidth; j++) {
								if (i >= m_Height || j >= m_Width) {
									newdata[offset] = (unsigned char) 0;
									newdata[offset+1] = (unsigned char) 0;
									newdata[offset+2] = (unsigned char) 0;
									newdata[offset+3] = (unsigned char) 0;
								} else if ((unsigned int) ((unsigned char) pixelData[oldOffset]) == mask.getRed() &&
										(unsigned int) ((unsigned char) pixelData[oldOffset+1]) == mask.getGreen() &&
										(unsigned int) ((unsigned char) pixelData[oldOffset+2]) == mask.getBlue() &&
										(unsigned int) ((unsigned char) pixelData[oldOffset+3]) == mask.getAlpha()) {
									//std::cout << "replacing " << j << ":" << i << std::endl;
									newdata[offset] = (unsigned char) 0;
									newdata[offset+1] = (unsigned char) 0;
									newdata[offset+2] = (unsigned char) 0;
									newdata[offset+3] = (unsigned char) 0;
									oldOffset += 4;
								} else {
									memcpy((newdata + offset), &pixelData[oldOffset], 4);
									oldOffset += 4;
								}
								offset += 4;
							}
						}
					}

					
					unsigned Object(0);

					#if defined(ARK2D_RENDERER_OPENGL)

						//Now generate the OpenGL texture object
						ARK2D::getLog()->v("generating texture");
						//if (texture == 0) { 
							glGenTextures(1, &Object);
							showAnyGlErrorAndExitMacro(); 
						//} else {
						//	Object = texture;
						//}
						//glGenTextures(1, &Object);
						//showAnyGlErrorAndExit();
						
						//glBindTexture(GL_TEXTURE_2D, Object);
						ARK2D::getLog()->v("binding texture");
						RendererState::internalBindTexture(Object);
						showAnyGlErrorAndExitMacro();

						ARK2D::getLog()->v("setting texture data");
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Renderer::getInterpolationGL());
						glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Renderer::getInterpolationGL());

						ARK2D::getLog()->v("glTexImage2d");
						//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempTextureWidth, tempTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, newdata);
						//glBindTexture(GL_TEXTURE_2D, 0);
						showAnyGlErrorAndExitMacro(); 

						RendererStats::s_textureAllocatedBytes += (tempTextureWidth * tempTextureHeight * 8 * 4);

						ARK2D::getLog()->v("unbinding texture");
						RendererState::internalBindTexture(0);
						//texture = Object;

						showAnyGlErrorAndExitMacro();
					
					#elif defined(ARK2D_RENDERER_DIRECTX)
						Object = createDXTexture(tempTextureWidth, tempTextureHeight, newdata); 
						RendererStats::s_textureAllocatedBytes += (tempTextureWidth * tempTextureHeight * 8 * 4);
					#endif

					

					delete png;

					if (usesNewMemory) { 
						free(newdata);
					}

					return Object;
				} else if (png->isRGB()) {

					//ErrorDialog::createAndShow(StringUtil::append("Add an alpha channel to png image: ", filename));
					//exit(0);

					ARK2D::getLog()->v("rgb png");

					// power of two textures!
					unsigned char* newdata = (unsigned char*) malloc(tempTextureWidth * tempTextureHeight * 4);
					offset = 0;
					int oldOffset = 0;
					for (unsigned int i = 0; i < tempTextureHeight; i++) {
						for (unsigned int j = 0; j < tempTextureWidth; j++) {
							if (i >= m_Height || j >= m_Width) {
								newdata[offset] = (unsigned char) 0;
								newdata[offset+1] = (unsigned char) 0;
								newdata[offset+2] = (unsigned char) 0;
								newdata[offset+3] = (unsigned char) 0;
							} else if ((unsigned int) ((unsigned char) pixelData[oldOffset]) == mask.getRed() &&
									(unsigned int) ((unsigned char) pixelData[oldOffset+1]) == mask.getGreen() &&
									(unsigned int) ((unsigned char) pixelData[oldOffset+2]) == mask.getBlue()) {
								//std::cout << "replacing " << j << ":" << i << std::endl;
								newdata[offset] = (unsigned char) 0;
								newdata[offset+1] = (unsigned char) 0;
								newdata[offset+2] = (unsigned char) 0;
								newdata[offset+3] = (unsigned char) 0;
								oldOffset += 4;
							} else {
								memcpy((newdata + offset), &pixelData[oldOffset], 4);
								/*
								memcpy((newdata + offset), &pixelData[oldOffset], 1);
								memcpy((newdata + offset + 1), &pixelData[oldOffset+1], 1);
								memcpy((newdata + offset + 2), &pixelData[oldOffset+2], 1);
								memcpy((newdata + offset + 3), &pixelData[oldOffset+3], 1);
								*/
								//newdata[offset] = (unsigned char) pixelData[oldOffset];
								//newdata[offset+1] = (unsigned char) pixelData[oldOffset+1];
								//newdata[offset+2] = (unsigned char) pixelData[oldOffset+2];
								//newdata[offset+3] = (unsigned char) pixelData[oldOffset+3];
								oldOffset += 4;
							}
							offset += 4; 
						}
					}

					unsigned Object(0);

					#if defined(ARK2D_RENDERER_OPENGL)

	  					//Now generate the OpenGL texture object
						ARK2D::getLog()->v("generating texture");
						glGenTextures(1, &Object);
						showAnyGlErrorAndExitMacro();
						
						ARK2D::getLog()->v("binding texture");
						RendererState::internalBindTexture(Object);
						showAnyGlErrorAndExitMacro();

						ARK2D::getLog()->v("setting texture data");
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Renderer::getInterpolationGL());
						glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Renderer::getInterpolationGL());

						//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempTextureWidth, tempTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, newdata);
						//glBindTexture(GL_TEXTURE_2D, 0);

						RendererStats::s_textureAllocatedBytes += (tempTextureWidth * tempTextureHeight * 8 * 4);

						ARK2D::getLog()->v("unbinding texture");
						RendererState::internalBindTexture(0);
						//texture = Object;
 
						showAnyGlErrorAndExitMacro(); 

					#elif defined(ARK2D_RENDERER_DIRECTX)
						Object = createDXTexture(tempTextureWidth, tempTextureHeight, newdata); 
						RendererStats::s_textureAllocatedBytes += (tempTextureWidth * tempTextureHeight * 8 * 4);
					#endif

					

					delete png;
					free(newdata);
					return Object;
				}

				return 0; 
			} 
			return 0;
		}
		void Image::showAnyGlErrorAndExit() {
			showAnyGlErrorAndExit( __FILE__, __LINE__ );
		}
		void Image::showAnyGlErrorAndExit(const char* fname, int line) {
			//#ifdef ARK2D_DEBUG
			if (ARK2D::getLog()->getFilter() == ARK::Util::Log::TYPE_ALL)
			{
				#if defined(ARK2D_RENDERER_DIRECTX)
					/*string s = "dx: ";
					s += fname;
					s += " - ";
					s += Cast::toString<int>(line);
					s += ".";
					ARK2D::getLog()->v(s);*/
					return;
				#elif defined(ARK2D_RENDERER_OPENGL)
					RendererStats::s_glCalls++; 
					int e = glGetError();
					if (e != GL_NO_ERROR) { 
						ARK2D::getLog()->e("Definitely a GL error. :( ");
						ARK2D::getLog()->e("Renderer state:" );
						ARK2D::getLog()->e(ARK2D::getRenderer()->toString());

						RendererStats::s_glCalls++;
						string s = getGlErrorString(e);
						s += " : ";
						s += fname;
						s += Cast::toString<int>(line);
						s += ".";
						ErrorDialog::createAndShow(s); 
						exit(0);
					} 
				#endif
			}
			//#endif
		}
		


		string Image::getGlErrorString(int error) {
			#if defined(ARK2D_RENDERER_OPENGL)
				switch(error) {
					case GL_INVALID_ENUM:
						return "GL_INVALID_ENUM"; 
						break;
					case GL_INVALID_VALUE:
						return "GL_INVALID_VALUE";
						break;
					#if !defined(ARK2D_ANDROID)
						case GL_INVALID_FRAMEBUFFER_OPERATION:
							return "GL_INVALID_FRAMEBUFFER_OPERATION";
							break;
					#endif
					case GL_INVALID_OPERATION:
						return "GL_INVALID_OPERATION";
						break;
					case GL_OUT_OF_MEMORY:
						return "GL_OUT_OF_MEMORY";
						break;
	 
					/*case GL_STACK_OVERFLOW:
						return "GL_STACK_OVERFLOW";
						break;
					case GL_STACK_UNDERFLOW:
						return "GL_STACK_UNDERFLOW";
						break;
					case GL_TABLE_TOO_LARGE:
						return "GL_TABLE_TOO_LARGE";
						break;*/
				}
				return "GL_UNKNOWN_ERROR"; 
			#else
				return "DIRECTX_ERROR_NOT_IMPLEMENTED";
			#endif
		}

		unsigned int Image::createDXTexture(int w, int h, void* data) 
		{
			#if defined(ARK2D_RENDERER_DIRECTX)
				D3D11_TEXTURE2D_DESC desc;
				desc.Width = w; // this->getWidth();
				desc.Height = h; // this->getHeight();
				desc.MipLevels = desc.ArraySize = 1;
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				desc.SampleDesc.Count = 1; 
				desc.SampleDesc.Quality = 0;
				desc.Usage = D3D11_USAGE_IMMUTABLE;// D3D11_USAGE_DYNAMIC;
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
				desc.MiscFlags = 0;

				D3D11_SUBRESOURCE_DATA texdata;
				texdata.pSysMem = data;
				texdata.SysMemPitch = w * 4; // 0;
				texdata.SysMemSlicePitch = w * h * 4 * sizeof(unsigned char); // 0;

				ID3D11Device* pd3dDevice = ARK2D::getContainer()->getPlatformSpecific()->m_device;
				HRESULT rs = pd3dDevice->CreateTexture2D(&desc, &texdata, &m_dxTexture);
				if (FAILED(rs)) { ARK2D::getLog()->e("Could not create texture 2d."); exit(0); }

				D3D11_SAMPLER_DESC sdesc;
				sdesc.Filter = (Renderer::getInterpolation() == Renderer::INTERPOLATION_LINEAR) ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
				sdesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
				sdesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
				sdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
				sdesc.MipLODBias = 0.0f;
				sdesc.MaxAnisotropy = 1;
				sdesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
				sdesc.BorderColor[0] = 0;
				sdesc.BorderColor[1] = 0;
				sdesc.BorderColor[2] = 0;
				sdesc.BorderColor[3] = 0;
				sdesc.MinLOD = 0;
				sdesc.MaxLOD = D3D11_FLOAT32_MAX;
 
				//(Renderer::getInterpolation() == Renderer::INTERPOLATION_LINEAR) ? D3D11_FILTER_TYPE_LINEAR : D3D11_FILTER_TYPE_POINT;

				rs = pd3dDevice->CreateSamplerState(&sdesc, &m_dxSampler);
				if (FAILED(rs)) { ARK2D::getLog()->e("Could not create texture 2d."); exit(0); }

				//D3D11_SHADER_RESOURCE_VIEW_DESC vdesc;
				//vdesc.Texture2D
				pd3dDevice->CreateShaderResourceView(m_dxTexture, NULL, &m_dxResourceView);
			#endif
			unsigned int retval = s_dxNextTextureId;
			s_dxNextTextureId++;
			return retval;
		}

		GLuint Image::load() {
			return load(ARK2D::getRenderer()->getMaskColor());
		}
 
		BMPImage* Image::loadBMP() {
			BMPImage* bmp = new BMPImage();
			int suc = bmp->LoadBMP(this->filename.c_str());
			std::string errmsg = "loadBMP: ";
			if (suc == 1) {
				errmsg = "Error opening bitmap file: " + this->filename + ".";
			} else if (suc == 2) {
				errmsg = this->filename + " is not a valid bitmap image.";
			}

			if (errmsg != "") {
				ErrorDialog::createAndShow(errmsg);
				return 0;
			}
			return bmp;
		}

		TargaImage* Image::loadTGA() {
			TargaImage* targa = new TargaImage();
			int suc = targa->load(this->filename);
			std::string errmsg = "";
			if (suc == -1) {
				errmsg = "loadTGA: unknown error with tga image: ";
			} else if (suc == 1) {
				errmsg = "loadTGA: could not open image for reading: ";
			} else if (suc == 2) {
				errmsg = "loadTGA: unsupported image tga type: ";
			} else if (suc == 3) {
				errmsg = "loadTGA: unsupported color depth, seem_bytesPerPixel: ";
			} 

			if (errmsg != "") {
				ErrorDialog::createAndShow(errmsg);
				return 0; 
			}
			return targa;
		}

		PNGImage* Image::loadPNG() {
			PNGImage* png = new PNGImage(this->filename);
			int suc = 0;
			if (m_data == NULL) {
				suc = png->load();
			} else {
				suc = png->load(m_data);
			}
			if (suc != 0) {
				//std::cout << suc << std::endl;
				string errmsg = StringUtil::append("Error loading png image: ", suc);
				ErrorDialog::createAndShow(errmsg);
				exit(0);
			}
			return png;
		}

		Image* Image::s_dummy = NULL;
		Image* Image::getDummy() {
			if (s_dummy == NULL) {
				s_dummy = new Image();
				s_dummy->addTexture();
			}
			return s_dummy;
		}

		Image::Image():
			ARK::Core::Resource(),
			ARK::SceneGraph::SceneNode("image",SceneNode::TYPE_IMAGE),
			m_data(NULL),
			m_dataLength(0),
			m_resourceType(0),
			filename(""),
			m_bounds(0,0,0),
			m_texture(NULL),
			texture_temp(0),
			m_dxTexture(NULL),
			//texture_width(1),
			//texture_height(1),
			//texture_offset_x(0),
			//texture_offset_y(0),
			texture_offset_x_tl(0.0f),
			texture_offset_y_tl(0.0f),
			texture_offset_x_tr(1.0f),
			texture_offset_y_tr(0.0f),
			texture_offset_x_bl(0.0f),
			texture_offset_y_bl(1.0f),
			texture_offset_x_br(1.0f),
			texture_offset_y_br(1.0f),
			texture_source_w(256.0f),
			texture_source_h(256.0f),
			m_CenterX(0),
			m_CenterY(0),
			m_tl_corner_color(),
			m_tr_corner_color(),
			m_alpha(1.0f),
			m_color(NULL),
			m_dirty(false)
		{
			clean();
		}

		Image::Image(unsigned int resource, unsigned int resourceType):
			ARK::Core::Resource(),
			ARK::SceneGraph::SceneNode("image",SceneNode::TYPE_IMAGE),
			m_data(NULL),
			m_dataLength(0),
			m_resourceType(resourceType),
			filename(""),
			m_bounds(0,0,0),
			m_texture(NULL),
			texture_temp(0),
			m_dxTexture(NULL),
			//texture_width(1),
			//texture_height(1),
			//texture_offset_x(0),
			//texture_offset_y(0),
			texture_offset_x_tl(0.0f),
			texture_offset_y_tl(0.0f),
			texture_offset_x_tr(1.0f),
			texture_offset_y_tr(0.0f),
			texture_offset_x_bl(0.0f),
			texture_offset_y_bl(1.0f),
			texture_offset_x_br(1.0f),
			texture_offset_y_br(1.0f),
			texture_source_w(256.0f),
			texture_source_h(256.0f),
			m_CenterX(0),
			m_CenterY(0),
			m_tl_corner_color(),
			m_tr_corner_color(),
			m_alpha(1.0f),
			m_color(NULL),
			m_dirty(false)
		{
			ARK2D::getLog()->v("Loading Image from resource. "); //std::cout << "Loading Image from resource. " << std::endl;
			m_data = GameContainerPlatform::getARK2DResource(resource, resourceType);
			this->texture_temp = this->load();
			//std::cout << "texture id " << (this->texture_temp) << std::endl;
			ARK2D::getLog()->v(StringUtil::append("Texture id: ", this->texture_temp));
			addTexture();
			clean(); 
		}

		Image::Image(void* data, unsigned int resourceType, string file):
			ARK::Core::Resource(),
			ARK::SceneGraph::SceneNode(file,SceneNode::TYPE_IMAGE),
			m_data(data),
			m_dataLength(0),
			m_resourceType(resourceType),
			filename(file),
			m_bounds(0,0,0),
			m_texture(NULL),
			texture_temp(0),
			m_dxTexture(NULL),
			m_dxSampler(NULL),
			m_dxResourceView(NULL),
			//texture_width(1),
			//texture_height(1),
			//texture_offset_x(0),
			//texture_offset_y(0),
			texture_offset_x_tl(0.0f),
			texture_offset_y_tl(0.0f),
			texture_offset_x_tr(1.0f),
			texture_offset_y_tr(0.0f),
			texture_offset_x_bl(0.0f),
			texture_offset_y_bl(1.0f),
			texture_offset_x_br(1.0f),
			texture_offset_y_br(1.0f),
			texture_source_w(256.0f),
			texture_source_h(256.0f),
			m_CenterX(0),
			m_CenterY(0),
			m_tl_corner_color(),
			m_tr_corner_color(),
			m_alpha(1.0f),
			m_color(NULL),
			m_dirty(false)
		{
			ARK2D::getLog()->v("Loading Image from data. ");
			this->texture_temp = this->load();
			addTexture();
			clean();
		}

		Image::Image(void* data, unsigned int dataLength, unsigned int resourceType, string file):
			ARK::Core::Resource(),
			ARK::SceneGraph::SceneNode("file",SceneNode::TYPE_IMAGE),
			m_data(data),
			m_dataLength(dataLength),
			m_resourceType(resourceType),
			filename(file),
			m_bounds(0,0,0),
			m_texture(NULL),
			texture_temp(0),
			m_dxTexture(NULL),
			m_dxSampler(NULL),
			m_dxResourceView(NULL),
			//texture_width(1),
			//texture_height(1),
			//texture_offset_x(0),
			//texture_offset_y(0),
			texture_offset_x_tl(0.0f),
			texture_offset_y_tl(0.0f),
			texture_offset_x_tr(1.0f),
			texture_offset_y_tr(0.0f),
			texture_offset_x_bl(0.0f),
			texture_offset_y_bl(1.0f),
			texture_offset_x_br(1.0f),
			texture_offset_y_br(1.0f),
			texture_source_w(256.0f),
			texture_source_h(256.0f),
			m_CenterX(0),
			m_CenterY(0),
			m_tl_corner_color(),
			m_tr_corner_color(),
			m_alpha(1.0f),
			m_color(NULL),
			m_dirty(false)
		{
			ARK2D::getLog()->v("Loading Image from data. ");
			this->texture_temp = this->load();
			addTexture();
			clean();
		}

		Image::Image(const std::string& fname):
			ARK::Core::Resource(),
			ARK::SceneGraph::SceneNode(fname,SceneNode::TYPE_IMAGE),
			m_data(NULL),
			m_resourceType(0),
			filename(fname),
			m_bounds(0,0,0),
			m_texture(NULL),
			texture_temp(0),
			m_dxTexture(NULL),
			m_dxSampler(NULL),
			m_dxResourceView(NULL),
			//texture_width(1),
			//texture_height(1),
			//texture_offset_x(0),
			//texture_offset_y(0),
			texture_offset_x_tl(0.0f),
			texture_offset_y_tl(0.0f),
			texture_offset_x_tr(1.0f),
			texture_offset_y_tr(0.0f),
			texture_offset_x_bl(0.0f),
			texture_offset_y_bl(1.0f),
			texture_offset_x_br(1.0f),
			texture_offset_y_br(1.0f),
			texture_source_w(256.0f),
			texture_source_h(256.0f),
			m_CenterX(0),
			m_CenterY(0),
			m_tl_corner_color(),
			m_tr_corner_color(),
			m_alpha(1.0f),
			m_color(NULL),
			m_dirty(false)
		{
			this->texture_temp = this->load(); // this sets the width and height too! :)
			//std::cout << "Loaded Image: " << this->filename << " tex id: " << (this->texture_temp) << std::endl; //  BMP must be 24bit unrestricted bmp!
			ARK2D::getLog()->v(String("Loaded Image: ").append(this->filename).append(" tex id: ").append((unsigned int) this->texture_temp).get());
			addTexture();
			clean();
		}

		Image::Image(const std::string& fname, const Color& mask) :
			ARK::Core::Resource(),
   			ARK::SceneGraph::SceneNode(fname,SceneNode::TYPE_IMAGE),
			m_data(NULL), 
			m_resourceType(0), 
			filename(fname),
			m_bounds(0,0,0),
			m_texture(NULL),
			texture_temp(0),
			m_dxTexture(NULL),
			m_dxSampler(NULL),
			m_dxResourceView(NULL),
			//texture_width(1),
			//texture_height(1),
			//texture_offset_x(0),
			//texture_offset_y(0),
			texture_offset_x_tl(0.0f),
			texture_offset_y_tl(0.0f),
			texture_offset_x_tr(1.0f),
			texture_offset_y_tr(0.0f),
			texture_offset_x_bl(0.0f),
			texture_offset_y_bl(1.0f),
			texture_offset_x_br(1.0f),
			texture_offset_y_br(1.0f),
			texture_source_w(256.0f),
			texture_source_h(256.0f),
			m_CenterX(0),
			m_CenterY(0), 
			m_tl_corner_color(),
			m_tr_corner_color(),
			m_alpha(1.0f),
			m_color(NULL),
			m_dirty(false)
		{
			this->texture_temp = this->load(mask); // this sets the width and height too! :)
			//std::cout << "Loaded Image: " << this->filename << " tex id: " << (this->texture_temp) << std::endl; //  BMP must be 24bit unrestricted bmp!
			ARK2D::getLog()->v(String("Loaded Image: ").append(this->filename).append(" tex id: ").append((unsigned int) this->texture_temp).get());
			addTexture();
			clean();
		}

		void Image::addTexture()  
		{
			// add to the texture store. for android reloading purposes, yo. 
			TextureStore* ts = TextureStore::getInstance();
			if (!ts->hasTexture(filename)) { 
				ARK2D::getLog()->v("New Texture Object");
				Texture* t = new Texture(texture_temp, this);
				t->m_interpolation = Renderer::getInterpolation();
				#ifdef ARK2D_RENDERER_DIRECTX
					t->m_dxTexture = m_dxTexture;
					t->m_dxSampler = m_dxSampler;
					t->m_dxResourceView = m_dxResourceView;

				#endif
				ts->addTexture(filename, t);
				m_texture = t;
			} else {
				ARK2D::getLog()->v("Recyling Texture Object");
				m_texture = ts->getTexture(filename);
			}
		}

		unsigned int Image::getWidth() const { 
			return (unsigned int) m_Width;
		}
		unsigned int Image::getHeight() const {
			return (unsigned int) m_Height;
		}
		
		SceneNode* Image::setRotation(double angle) {
			// clamp angles between 0 and 360
			//  because quaternions are silly. 
			angle = MathUtil::absangle<double>(angle);

            SceneNode::transform.rotation = Quaternion<float>::angleAxis(angle, 0,0,1);
			m_dirty = true;
			return this;
		}
		SceneNode* Image::rotate(double angle) {
			double newAngle = angle + SceneNode::transform.rotation.angle();
			return setRotation(newAngle);
		}
		Image* Image::setCenterOfRotation(int x, int y) {
			m_CenterX = x;
			m_CenterY = y;
			return this;
		}
		void Image::setWidth(int w) {
			m_Width = w;
			m_dirty = true;
		}
		void Image::setHeight(int h) {
			m_Height = h;
			m_dirty = true;
		}
		Image* Image::getSubImage(const SpriteSheetDescription* desc, const char* name) {
			return getSubImage(desc->getItemByName(name));
		}
		Image* Image::getSubImage(const SpriteSheetDescriptionItem& desc) {
			if (desc.isRotated()) { 
				Image* sub = getSubImage(desc.getX(), desc.getY(), desc.getWidth(), desc.getHeight());
				
				float t1 = sub->texture_offset_x_tl;
				float t2 = sub->texture_offset_y_tl;
				float t3 = sub->texture_offset_x_tr;
				float t4 = sub->texture_offset_y_tr;
				float t5 = sub->texture_offset_x_bl;
				float t6 = sub->texture_offset_y_bl;
				float t7 = sub->texture_offset_x_br;
				float t8 = sub->texture_offset_y_br;

				sub->texture_offset_x_tl = t3;
				sub->texture_offset_y_tl = t4;
				sub->texture_offset_x_tr = t7;
				sub->texture_offset_y_tr = t8;
				sub->texture_offset_x_bl = t1;
				sub->texture_offset_y_bl = t2;
				sub->texture_offset_x_br = t5;
				sub->texture_offset_y_br = t6;
				sub->texture_source_w = texture_source_w;
				sub->texture_source_h = texture_source_h;

				sub->setWidth(desc.getHeight());
				sub->setHeight(desc.getWidth());
				sub->m_originalWidth = sub->m_Width;
				sub->m_originalHeight = sub->m_Height;
				sub->setDirty(true);
				sub->clean();
				return sub;
			} 

			return getSubImage(desc.getX(), desc.getY(), desc.getWidth(), desc.getHeight());
		}
		float Image::getTextureW() const {
			if (texture_offset_y_tl == texture_offset_y_tr) {
				//if (texture_offset_x_tl > texture_offset_x_tr) {
				//	return texture_offset_x_tl - texture_offset_x_tr;
				//}
				return texture_offset_x_tr - texture_offset_x_tl;
			} else if (texture_offset_x_tl == texture_offset_x_tr) {
				//if (texture_offset_y_tl > texture_offset_y_tr) {
				//	return texture_offset_y_tl -texture_offset_y_tr;
				//}
				return texture_offset_y_tr - texture_offset_y_tl;
			}
			ARK2D::getLog()->w("getTextureW warning");
			return 1.0f;
		};
		float Image::getTextureH() const {
			if (texture_offset_x_tl == texture_offset_x_bl) {
				//if (texture_offset_y_tl > texture_offset_y_bl) {
				//	return texture_offset_y_tl - texture_offset_y_bl;
				//}
				return texture_offset_y_bl - texture_offset_y_tl;
			} else if (texture_offset_y_tl == texture_offset_y_bl) {
				//if (texture_offset_x_bl > texture_offset_x_tl) {
				//	return texture_offset_x_bl - texture_offset_x_tl;
				//}
				return texture_offset_x_tl - texture_offset_x_bl;
			}
			ARK2D::getLog()->w("getTextureH warning");
			return 1.0f;
		};
		Image* Image::getSubImage(int x, int y, int width, int height) const {
			//float newTextureOffsetX = ((x / (float) m_Width) * this->texture_width) + this->texture_offset_x;
			//float newTextureOffsetY = ((y / (float) m_Height) * this->texture_height) + this->texture_offset_y;
			//float newTextureWidth = ((width / (float) m_Width) * this->texture_width);
			//float newTextureHeight = ((height / (float) m_Height) * this->texture_height);
			float newTextureOffsetX = ((x / (float) m_Width) * this->getTextureW()) + this->texture_offset_x_tl;
			float newTextureOffsetY = ((y / (float) m_Height) * this->getTextureH()) + this->texture_offset_y_tl;
			float newTextureWidth = ((width / (float) m_Width) * this->getTextureW());
			float newTextureHeight = ((height / (float) m_Height) * this->getTextureH());


			Image* sub = new Image();
			sub->m_texture = m_texture;
			sub->texture_temp = texture_temp;
			//sub->texture_width = newTextureWidth;
			//sub->texture_height = newTextureHeight;
			sub->texture_offset_x_tl = newTextureOffsetX;
			sub->texture_offset_y_tl = newTextureOffsetY;
			sub->texture_offset_x_tr = newTextureOffsetX+newTextureWidth;
			sub->texture_offset_y_tr = newTextureOffsetY;
			sub->texture_offset_x_bl = newTextureOffsetX;
			sub->texture_offset_y_bl = newTextureOffsetY+newTextureHeight;
			sub->texture_offset_x_br = newTextureOffsetX+newTextureWidth;
			sub->texture_offset_y_br = newTextureOffsetY+newTextureHeight;
			sub->texture_source_w = texture_source_w;
			sub->texture_source_h = texture_source_h;
			
			sub->setWidth(width);
			sub->setHeight(height);
			sub->m_originalWidth = width;
			sub->m_originalHeight = height;
			sub->clean();
			return sub;

			/*Image sub;
			sub.texture = texture;
			sub.texture_width = newTextureWidth;
			sub.texture_height = newTextureHeight;
			sub.texture_offset_x = newTextureOffsetX;
			sub.texture_offset_y = newTextureOffsetY;
			sub.setWidth(width);
			sub.setHeight(height);
			sub.m_tl_corner_color = Color(255,255,255,255);
			sub.m_tr_corner_color = Color(255,255,255,255);
			return sub;*/
		}

		SceneNode* Image::scale(float x, float y) {
            SceneNode::transform.scale.multiply(x, y, 1);
			m_Width = (float(m_Width) * x);
			m_Height = (float(m_Height) * y);
			clean();
			return this;
		}
		SceneNode* Image::setScale(float x, float y) {
			SceneNode::transform.scale.set(x, y);
			m_Width = m_originalWidth * x;
			m_Height = m_originalHeight * y;
			clean();
			return this;
		}
 
		Image* Image::getCopy() {
			Image* sub = new Image();
			sub->m_texture = m_texture;
			sub->texture_temp = texture_temp;
			//sub->texture_width = texture_width;
			//sub->texture_height = texture_height;
			//sub->texture_offset_x = texture_offset_x;
			//sub->texture_offset_y = texture_offset_y;
			sub->texture_offset_x_tl = texture_offset_x_tl;
			sub->texture_offset_y_tl = texture_offset_y_tl;
			sub->texture_offset_x_tr = texture_offset_x_tr;
			sub->texture_offset_y_tr = texture_offset_y_tr;
			sub->texture_offset_x_bl = texture_offset_x_bl;
			sub->texture_offset_y_bl = texture_offset_y_bl;
			sub->texture_offset_x_br = texture_offset_x_br;
			sub->texture_offset_y_br = texture_offset_y_br;
			sub->texture_source_w = texture_source_w;
			sub->texture_source_h = texture_source_h;

			sub->setWidth(int(m_Width));
			sub->setHeight(int(m_Height));
			sub->m_originalWidth = sub->m_Width;
			sub->m_originalHeight = sub->m_Height;
			sub->clean(); 
			return sub;
		}

		Image* Image::getScaledCopy(unsigned int x, unsigned int y) {

			Image* sub = new Image();
			sub->m_texture = m_texture;
			sub->texture_temp = texture_temp;
			//sub->texture_width = texture_width;
			//sub->texture_height = texture_height;
			//sub->texture_offset_x = texture_offset_x;
			//sub->texture_offset_y = texture_offset_y;
			sub->texture_offset_x_tl = texture_offset_x_tl;
			sub->texture_offset_y_tl = texture_offset_y_tl;
			sub->texture_offset_x_tr = texture_offset_x_tr;
			sub->texture_offset_y_tr = texture_offset_y_tr;
			sub->texture_offset_x_bl = texture_offset_x_bl;
			sub->texture_offset_y_bl = texture_offset_y_bl;
			sub->texture_offset_x_br = texture_offset_x_br;
			sub->texture_offset_y_br = texture_offset_y_br;
			sub->texture_source_w = texture_source_w;
			sub->texture_source_h = texture_source_h;
			sub->setWidth((int) m_Width * x);
			sub->setHeight((int) m_Height * y);
			sub->m_originalWidth = sub->m_Width;
			sub->m_originalHeight = sub->m_Height;
			sub->clean();
			return sub;
		}
		Image* Image::getScaledCopy(float x, float y) {

			Image* sub = new Image();
			sub->m_texture = m_texture;
			sub->texture_temp = texture_temp;
			//sub->texture_width = texture_width;
			//sub->texture_height = texture_height;
			//sub->texture_offset_x = texture_offset_x;
			//sub->texture_offset_y = texture_offset_y;
			sub->texture_offset_x_tl = texture_offset_x_tl;
			sub->texture_offset_y_tl = texture_offset_y_tl;
			sub->texture_offset_x_tr = texture_offset_x_tr;
			sub->texture_offset_y_tr = texture_offset_y_tr;
			sub->texture_offset_x_bl = texture_offset_x_bl;
			sub->texture_offset_y_bl = texture_offset_y_bl;
			sub->texture_offset_x_br = texture_offset_x_br;
			sub->texture_offset_y_br = texture_offset_y_br;
			sub->texture_source_w = texture_source_w;
			sub->texture_source_h = texture_source_h;
			sub->setWidth((int)(m_Width * x));
			sub->setHeight((int)(m_Height * y));
			sub->m_originalWidth = sub->m_Width;
			sub->m_originalHeight = sub->m_Height;
			sub->clean();
			return sub;
		}

		Image* Image::flip(bool flipx, bool flipy) 
		{
			if (flipx) { 
				//texture_offset_x = texture_offset_x + texture_width;
				//texture_width = -texture_width;
				float tl = texture_offset_x_tl;
				float bl = texture_offset_x_bl;
				texture_offset_x_tl = texture_offset_x_tr;
				texture_offset_x_bl = texture_offset_x_br;
				texture_offset_x_tr = tl;
				texture_offset_x_br = bl;
			}
			if (flipy) {
				//texture_offset_y = texture_offset_y + texture_height;
				//texture_height = -texture_height;
				float tl = texture_offset_y_tl;
				float tr = texture_offset_y_tr;
				texture_offset_y_tl = texture_offset_y_bl;
				texture_offset_y_tr = texture_offset_y_br;
				texture_offset_y_bl = tl;
				texture_offset_y_br = tr;
			}
			clean();
			return this;
		}

		Image* Image::setFlipped(bool flipx, bool flipy) {
			//if ((flipx && texture_width > 0.0f) || (!flipx && texture_width < 0.0f)) {
			//	flip(true,false);
			//}
			
			//if ((flipy && texture_height > 0.0f) || (!flipy && texture_height < 0.0f)) {
			//	flip(false, true);
			//}
			if ((flipx && getTextureW() > 0.0f) || (!flipx && getTextureW() < 0.0f)) {
				flip(true, false);
			}
			
			if ((flipy && getTextureH() > 0.0f) || (!flipy && getTextureH() < 0.0f)) {
				flip(false, true);
			}
			return this;
		}
		
		
		Image* Image::getFlippedCopy(bool horizontal_flip, bool vertical_flip) {
			Image* sub = new Image();
			sub->m_texture = m_texture;
			sub->texture_temp = texture_temp;
			//sub->texture_width = texture_width;
			//sub->texture_height = texture_height;
			//sub->texture_offset_x = texture_offset_x;
			//sub->texture_offset_y = texture_offset_y;
			sub->texture_offset_x_tl = texture_offset_x_tl;
			sub->texture_offset_y_tl = texture_offset_y_tl;
			sub->texture_offset_x_tr = texture_offset_x_tr;
			sub->texture_offset_y_tr = texture_offset_y_tr;
			sub->texture_offset_x_bl = texture_offset_x_bl;
			sub->texture_offset_y_bl = texture_offset_y_bl;
			sub->texture_offset_x_br = texture_offset_x_br;
			sub->texture_offset_y_br = texture_offset_y_br;
			sub->texture_source_w = texture_source_w;
			sub->texture_source_h = texture_source_h;
			sub->m_CenterX = m_CenterX;
			sub->m_CenterY = m_CenterY;
			sub->setWidth((int) m_Width);
			sub->setHeight((int) m_Height);
			sub->m_originalWidth = m_Width;
			sub->m_originalHeight = m_Height;
			
			
			sub->flip(horizontal_flip, vertical_flip);

			/*if (horizontal_flip) {
				sub->texture_offset_x = texture_offset_x + texture_width;
				sub->texture_width = -texture_width;
			}
			if (vertical_flip) {
				sub->texture_offset_y = texture_offset_y + texture_height;
				sub->texture_height = -texture_height;
			}*/
			sub->clean();
			return sub;
		}

		Image* Image::setAlpha(float f) {
			m_alpha = f;
			m_dirty = true;
			return this;
		}
		float Image::getAlpha() const {
			return m_alpha;
		}

		void Image::setColor(const Color& c) {
			if (m_color == NULL) { 
				m_color = new Color(c);
			} else { 
				m_color->set(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
			}
			m_dirty = true;
		}
		void Image::setColor(Color* c) {
			m_color = c;
			m_dirty = true;
		}
		void Image::setColor(float r, float g, float b, float a) {
			if (m_color != NULL) {
				m_color->set(r, g, b, a);
				m_dirty = true;
			}
		}
		Color* Image::getColor() const {
			return m_color;
		}

		void Image::setDirty(bool b) {
			m_dirty = true;
		}
		void Image::clean() { 
			texture_temp = 0;

			/*#if defined(ARK2D_OPENGL_ES_2_0)

				unsigned char r = 255;
				unsigned char g = 255;
				unsigned char b = 255;
				unsigned char a = (unsigned char) (m_alpha * 255);
				if (m_color != NULL) {
					r = m_color->getRedc();
					g = m_color->getGreenc();
					b = m_color->getBluec();
					a = m_color->getAlphac();
				}

				// tl
				m_vertices[0].vertex[0] 	= 0;
				m_vertices[0].vertex[1]		= 0;
				m_vertices[0].texcoord[0]	= this->texture_offset_x;
				m_vertices[0].texcoord[1]	= this->texture_offset_y;
				m_vertices[0].color[0]		= r;
				m_vertices[0].color[1]		= g;
				m_vertices[0].color[2]		= b;
				m_vertices[0].color[3]		= a;

				// tr
				m_vertices[1].vertex[0]		= 0 + this->m_Width;
				m_vertices[1].vertex[1]		= 0;
				m_vertices[1].texcoord[0]	= this->texture_offset_x + this->texture_width;
				m_vertices[1].texcoord[1]	= this->texture_offset_y;
				m_vertices[1].color[0]		= r;
				m_vertices[1].color[1]		= g;
				m_vertices[1].color[2]		= b;
				m_vertices[1].color[3]		= a;

				// bl
				m_vertices[2].vertex[0]		= 0;
				m_vertices[2].vertex[1]		= 0 + this->m_Height;
				m_vertices[2].texcoord[0]	= this->texture_offset_x;
				m_vertices[2].texcoord[1]	= this->texture_offset_y + this->texture_height;
				m_vertices[2].color[0]		= r;
				m_vertices[2].color[1]		= g;
				m_vertices[2].color[2]		= b;
				m_vertices[2].color[3]		= a;

				// br
				m_vertices[3].vertex[0] 	= 0 + this->m_Width;
				m_vertices[3].vertex[1]		= 0 + this->m_Height;
				m_vertices[3].texcoord[0]	= this->texture_offset_x + this->texture_width;
				m_vertices[3].texcoord[1]	= this->texture_offset_y + this->texture_height;
				m_vertices[3].color[0]		= r;
				m_vertices[3].color[1]		= g;
				m_vertices[3].color[2]		= b;
				m_vertices[3].color[3]		= a;

			#else */

				m_verts[0]  = 0; 					// tl
				m_verts[1]  = 0;
				m_verts[2]  = 0;
				m_verts[3]  = 0 + this->m_Width;		// tr
				m_verts[4]  = 0;
				m_verts[5]  = 0;
				m_verts[6]  = 0;						// bl
				m_verts[7]  = 0 + this->m_Height;
				m_verts[8]  = 0;
				m_verts[9]  = 0 + this->m_Width;		// br
				m_verts[10] = 0 + this->m_Height;
				m_verts[11] = 0;

				//m_texCoords[0] = this->texture_offset_x;
				//m_texCoords[1] = this->texture_offset_y;

				//m_texCoords[2] = this->texture_offset_x + this->texture_width;
				//m_texCoords[3] = this->texture_offset_y;

				//m_texCoords[4] = this->texture_offset_x;
				//m_texCoords[5] = this->texture_offset_y + this->texture_height;

				//m_texCoords[6] = this->texture_offset_x + this->texture_width;
				//m_texCoords[7] = this->texture_offset_y + this->texture_height;
			
				m_texCoords[0] = this->texture_offset_x_tl;
				m_texCoords[1] = this->texture_offset_y_tl;
			
				m_texCoords[2] = this->texture_offset_x_tr;
				m_texCoords[3] = this->texture_offset_y_tr;
			
				m_texCoords[4] = this->texture_offset_x_bl;
				m_texCoords[5] = this->texture_offset_y_bl;
			
				m_texCoords[6] = this->texture_offset_x_br;
				m_texCoords[7] = this->texture_offset_y_br;

				// always point image normals towards camera. 
				m_normals[0]  = 0; 		// tl 
				m_normals[1]  = 0;
				m_normals[2]  = 1;
				m_normals[3]  = 0;		// tr
				m_normals[4]  = 0;
				m_normals[5]  = 1;
				m_normals[6]  = 0;		// bl
				m_normals[7]  = 0;
				m_normals[8]  = 1;
				m_normals[9]  = 0;		// br
				m_normals[10] = 0;
				m_normals[11] = 1;

				Renderer* r = ARK2D::getRenderer();
				unsigned char red = r->getDrawColor().getRedc();
				unsigned char g   = r->getDrawColor().getGreenc();
				unsigned char b   = r->getDrawColor().getBluec();
				unsigned char a   = (unsigned char) (m_alpha * r->getDrawColor().getAlphac());
				if (m_color != NULL) {
					red = (unsigned char) (m_color->getRedc() * r->getDrawColor().getRedf());
					g   = (unsigned char) (m_color->getGreenc() * r->getDrawColor().getGreenf());
					b   = (unsigned char) (m_color->getBluec() * r->getDrawColor().getBluef());
					a   = (unsigned char) (m_color->getAlphac() * (r->getDrawColor().getAlphaf() * m_alpha));
				}

				m_colors[0] = red;
				m_colors[1] = g;
				m_colors[2] = b;
				m_colors[3] = a;
	 
				m_colors[4] = red;
				m_colors[5] = g;
				m_colors[6] = b;
				m_colors[7] = a;

				m_colors[8] = red;
				m_colors[9] = g;
				m_colors[10] = b;
				m_colors[11] = a;

				m_colors[12] = red;
				m_colors[13] = g;
				m_colors[14] = b;
				m_colors[15] = a;

				m_bounds.set(0, 0, 0, m_Width, m_Height, 0);

			//#endif

			m_dirty = false;
		}


		void Image::drawFlipped(float x, float y, bool flipx, bool flipy) {

			/*float previous_texture_offset_x = texture_offset_x;
			float previous_texture_width = texture_width;
			if (flipx) {
				texture_offset_x = texture_offset_x + texture_width;
				texture_width = texture_width * -1;
			}

			float previous_texture_offset_y = texture_offset_y;
			float previous_texture_height = texture_height;
			if (flipy) {
				texture_offset_y = texture_offset_y + texture_height;
				texture_height = texture_height * -1;
			}*/
			flip(flipx, flipy);

			clean();

			draw(x, y);
			
			flip(flipx, flipy);

			/*if (flipx) {
				texture_offset_x = previous_texture_offset_x;
				texture_width = previous_texture_width;
			}

			if (flipy) {
				texture_offset_y = previous_texture_offset_y;
				texture_height = previous_texture_height;
			}*/

			clean();
		}

		void Image::draw() {
			this->draw(0, 0);
		}
		void Image::draw(int x, int y) {
			this->draw((float)x, (float) y);
		}
		/**
		 * Draws at x,y with the image size w,h.
		 */
		void Image::draw(float x, float y, unsigned int w, unsigned int h) {
			m_dirty = true;

			float w2 = m_Width;
			float h2 = m_Height;

			m_Width = w;
			m_Height = h;

			draw(x, y);

			m_Width = w2;
			m_Height = h2;
		}
		void Image::draw(float x, float y) {
			draw(x, y, 0);
		}
		void Image::draw(float x, float y, float z) {

			//ARK2D::getLog()->v("temp: 0");
			//showAnyGlErrorAndExitMacro();

			Renderer* r = ARK2D::getRenderer();

			//#if defined(ARK2D_MACINTOSH) || defined(ARK2D_WINDOWS)
				//clean();
			//#else 
				if (m_dirty) {
					clean();
				}
			//#endif 
 
			if (Renderer::isBatching()) {
				float batch_rawVertices[] = {
					x,				y,				z, // tl
					x + m_Width,	y,				z, // tr
					x,				y + m_Height,	z, // bl
					x,				y + m_Height,	z, // bl
					x + m_Width,	y,				z, // tr 
					x + m_Width,	y + m_Height,	z // br
				};

				unsigned char red = r->getDrawColor().getRedc();
				unsigned char g = r->getDrawColor().getGreenc();
				unsigned char b = r->getDrawColor().getBluec();
				unsigned char a = (unsigned char) (m_alpha * 255.0f);
				if (m_color != NULL) {
					red = (unsigned char) (m_color->getRedc() * r->getDrawColor().getRedf());
					g   = (unsigned char) (m_color->getGreenc() * r->getDrawColor().getGreenf());
					b   = (unsigned char) (m_color->getBluec() * r->getDrawColor().getBluef());
					a   = (unsigned char) (m_color->getAlphac() * (m_alpha * r->getDrawColor().getAlphaf()) );
				}

				if (SceneNode::transform.rotation.angle() != 0) {
					MathUtil::rotate3dQuadAroundPoint<float>(batch_rawVertices, x + m_CenterX, y + m_CenterY, SceneNode::transform.rotation.angle());
				}
				
				Renderer::getBatch()->addTexturedQuad(
					m_texture->getId(), 
					batch_rawVertices[0], batch_rawVertices[1], batch_rawVertices[2],
					batch_rawVertices[3], batch_rawVertices[4], batch_rawVertices[5],
					batch_rawVertices[6], batch_rawVertices[7], batch_rawVertices[8],
					batch_rawVertices[15], batch_rawVertices[16], batch_rawVertices[17],
                                                      0,0,1,
                                                      0,0,1,
                                                      0,0,1,
                                                      0,0,1,
					//texture_offset_x, texture_offset_y,
					//texture_offset_x + texture_width, texture_offset_y,
					//texture_offset_x, texture_offset_y + texture_height,
					//texture_offset_x + texture_width, texture_offset_y + texture_height,
					texture_offset_x_tl, texture_offset_y_tl,
					texture_offset_x_tr, texture_offset_y_tr,
					texture_offset_x_bl, texture_offset_y_bl,
					texture_offset_x_br, texture_offset_y_br,
					red, g, b, a,
					red, g, b, a,
					red, g, b, a,
					red, g, b, a
				);
				return;
			}

			//ARK2D::getLog()->v("temp: 1");

			if (m_texture == NULL) { ARK2D::getLog()->w("Tried to draw image but texture was null."); return; }
			showAnyGlErrorAndExitMacro();

			#ifndef NO_FIXED_FUNCTION_PIPELINE
				glColor4f(r->getDrawColor().getRedf(), r->getDrawColor().getGreenf(), r->getDrawColor().getBluef(), m_alpha * r->getDrawColor().getAlphaf());
				RendererStats::s_glCalls++;
				if (m_color != NULL) {
					glColor4f(
						m_color->getRedf() * r->getDrawColor().getRedf(), 
						m_color->getGreenf() * r->getDrawColor().getGreenf(), 
						m_color->getBluef() * r->getDrawColor().getBluef(),
						m_color->getAlphaf() * r->getDrawColor().getAlphaf()
					);
					RendererStats::s_glCalls++;
				}
			#endif
			showAnyGlErrorAndExitMacro();


			//ARK2D::getLog()->v("temp: 2");

			// rotation
			if (SceneNode::transform.rotation.angle() != 0.0f) {
				float angle = SceneNode::transform.rotation.angle();
				Vector3<float> axis = SceneNode::transform.rotation.axis();
				r->translate(x + m_CenterX, y + m_CenterY, z);
				r->rotate(angle, axis.x, axis.y, axis.z);
				r->translate((x + m_CenterX) * -1, (y + m_CenterY) * -1, -z);
			}

			//ARK2D::getLog()->v("temp: 3");

			r->pushMatrix();
			r->translate(x, y, z);


			#if defined(ARK2D_FLASCC) 

				RendererState::start(RendererState::TEXTURE, m_texture->m_id);

				float rawVertices[] = {
					0.0f,			0.0f,		 	m_verts[2], 	// tl
					m_verts[9],		0.0f,			m_verts[5],	// tr
					0.0f,			m_verts[10],	m_verts[8],  // bl
					0.0f,			m_verts[10],	m_verts[11],  // bl
					m_verts[9],		0.0f,			m_verts[14],	// tr 
					m_verts[9],		m_verts[10],  	m_verts[17]	// br
				};

				/*float texCoords[] = {
					this->texture_offset_x,							this->texture_offset_y,							// tl
					this->texture_offset_x + this->texture_width,	this->texture_offset_y,							// tr
					this->texture_offset_x,							this->texture_offset_y + this->texture_height,	// bl
					this->texture_offset_x,							this->texture_offset_y + this->texture_height,	// bl
					this->texture_offset_x + this->texture_width,	this->texture_offset_y,							// tr
					this->texture_offset_x + this->texture_width,	this->texture_offset_y + this->texture_height	// br
				};*/

				float texCoords[] = {
					this->texture_offset_x_tl,	this->texture_offset_y_tl,
					this->texture_offset_x_tr,	this->texture_offset_y_tr,
					this->texture_offset_x_bl,	this->texture_offset_y_bl,
					this->texture_offset_x_bl,	this->texture_offset_y_bl,
					this->texture_offset_x_tr,	this->texture_offset_y_tr,
					this->texture_offset_x_br,	this->texture_offset_y_br
				};

				

				unsigned char red = 255;
				unsigned char g = 255;
				unsigned char b = 255;
				unsigned char a = (unsigned char) (m_alpha * 255.0f);
				if (m_color != NULL) {
					red = (unsigned char) (m_color->getRedc() * r->getDrawColor().getRedf());
					g   = (unsigned char) (m_color->getGreenc() * r->getDrawColor().getGreenf());
					b   = (unsigned char) (m_color->getBluec() * r->getDrawColor().getBluef());
					a   = (unsigned char) (m_color->getAlphac() * (m_alpha * r->getDrawColor().getAlphaf()));
				}

				unsigned char rawColors[] = { 
					red, g, b, a,
					red, g, b, a, 
					red, g, b, a,
					red, g, b, a,
					red, g, b, a,
					red, g, b, a
				};

				glVertexPointer(3, GL_FLOAT, 0, rawVertices);
				glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors); 
				glDrawArrays(GL_TRIANGLES, 0, 6);

				RendererStats::s_glCalls += 4;

			#else 
				
				#ifdef NO_FIXED_FUNCTION_PIPELINE

            		#if defined(ARK2D_OPENGL_3_2) || defined(ARK2D_OPENGL_ES_2_0)
 
						clean();
                        Shader* shader = RendererState::start(RendererState::TEXTURE, m_texture->m_id);
                        shader->setData(&m_verts[0], &m_normals[0], &m_texCoords[0], &m_colors[0], 4);
                        shader->drawTriangleStrip();

					#elif defined(ARK2D_RENDERER_DIRECTX)

						ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
						ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;
						ID3D11RenderTargetView* renderTargetView = ARK2D::getContainer()->m_platformSpecific.m_renderTargetView;
						ID3D11DepthStencilView* depthStencilView = ARK2D::getContainer()->m_platformSpecific.m_depthStencilView;

						float red = 1.0f;
						float g = 1.0f;
						float b = 1.0f;
						float a = m_alpha;
						if (m_color != NULL) {
							red = m_color->getRedf() * r->getDrawColor().getRedf();
							g   = m_color->getGreenf() * r->getDrawColor().getGreenf();
							b   = m_color->getBluef() * r->getDrawColor().getBluef();
							a   = m_color->getAlphaf() * m_alpha * r->getDrawColor().getAlphaf();
						}
 
						Renderer::__internalsDXUpdateMatrices();

						//ID3D11RenderTargetView *const targets[1] = { renderTargetView };
						//deviceContext->OMSetRenderTargets(1, targets, depthStencilView); 

						

						Renderer_DX_InterleavingTextureVertexData rawVertices[] = {
							{	
								DirectX::XMFLOAT4(0, 0, 0.0f, 0.0f), 
								DirectX::XMFLOAT2(this->texture_offset_x_tl, this->texture_offset_y_tl),
								DirectX::XMFLOAT4(red, g, b, a) 
							},
							{ 
								DirectX::XMFLOAT4(0 + m_verts[6], 0, 0.0f, 0.0f),
								DirectX::XMFLOAT2(this->texture_offset_x_tr, this->texture_offset_y_tr),
								DirectX::XMFLOAT4(red, g, b, a)
							},
							{ 
								DirectX::XMFLOAT4(0, m_verts[7], 0.0f, 0.0f),
								DirectX::XMFLOAT2(this->texture_offset_x_bl, this->texture_offset_y_bl),
								DirectX::XMFLOAT4(red, g, b, a)
							},
							{ 
								DirectX::XMFLOAT4(m_verts[6], m_verts[7], 0.0f, 0.0f),
								DirectX::XMFLOAT2(this->texture_offset_x_br, this->texture_offset_y_br),
								DirectX::XMFLOAT4(red, g, b, a)
							}
						};

						unsigned int rawIndices[] = { 
							0, 1, 2, 2, 1, 3
						};

						Renderer::s_vboQuadVerts->setData((void*)&rawVertices[0], (unsigned int) sizeof(Renderer_DX_InterleavingTextureVertexData) * 4);
						Renderer::s_vboIndices->setData((void*)&rawIndices[0], sizeof(unsigned int) * 6);

						unsigned int stride = sizeof(Renderer_DX_InterleavingTextureVertexData);
						unsigned int offset = 0;
						deviceContext->IASetVertexBuffers(0, 1, &Renderer::s_vboQuadVerts->m_buffer, &stride, &offset);

						deviceContext->IASetIndexBuffer(Renderer::s_vboIndices->m_buffer, DXGI_FORMAT_R32_UINT, 0);

						deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

						//ID3D11Buffer *const constantBuffers[1] = { Renderer::s_d3d_matrixBuffer };
						//deviceContext->VSSetConstantBuffers(0, 1, constantBuffers);

						deviceContext->DrawIndexed(6, 0, 0);
						//deviceContext->Draw(6, 0);

					#endif

				#else

					RendererState::start(RendererState::TEXTURE, m_texture->m_id);

					glVertexPointer(3, GL_FLOAT, 0, m_verts);
					glTexCoordPointer(2, GL_FLOAT, 0, m_texCoords);
					glColorPointer(4, GL_UNSIGNED_BYTE, 0, m_colors);

					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

					RendererStats::s_glCalls += 4; 

				#endif

			#endif

			r->popMatrix();

			if (SceneNode::transform.rotation.angle() != 0.0f) {
				float angle = SceneNode::transform.rotation.angle();
				Vector3<float> axis = SceneNode::transform.rotation.axis();
				r->translate(x + m_CenterX, y + m_CenterY, z);
				r->rotate(angle*-1, axis.x, axis.y, axis.z);
				r->translate((x + m_CenterX) * -1, (y + m_CenterY) * -1, -z);
			}

			RendererStats::s_tris += 2;

			//this->unbind();
			#ifndef NO_FIXED_FUNCTION_PIPELINE
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				RendererStats::s_glCalls++;
			#endif

		}

		void Image::drawCentered(int x, int y) {
			this->drawCentered((float)x, (float) y);
		}
 
		void Image::drawCentered(float x, float y) {
			this->draw(x - (this->m_Width/2), y - (this->m_Height/2));
		}
		void Image::drawCenteredFlipped(float x, float y, bool flipx, bool flipy) {
			this->drawFlipped(x - (this->m_Width/2), y - (this->m_Height/2), flipx, flipy);
		}

		void Image::drawAligned(float x, float y, signed int alignX, signed int alignY, float scale)
		{
			unsigned int oldWidth = (unsigned int) m_Width;
			unsigned int oldHeight = (unsigned int) m_Height;

			m_Width = float(m_Width) * scale; 
			m_Height = float(m_Height) * scale;

			if (alignX == Renderer::ALIGN_CENTER) {
				x -= m_Width/2;
			} else if (alignX == Renderer::ALIGN_RIGHT || alignX == Renderer::ALIGN_END) {
				x -= m_Width; 
			}

			if (alignY == Renderer::ALIGN_CENTER) {
				y -= m_Height/2;
			} else if (alignY == Renderer::ALIGN_END || alignY == Renderer::ALIGN_BOTTOM) {
				y -= m_Height;
			} 

			m_dirty = true;

			draw(x, y);

			m_Width = oldWidth;
			m_Height = oldHeight;

			clean();

			/*Renderer* g = ARK2D::getRenderer();
			g->pushMatrix();
			g->translate(x, y);
			g->scale(scale);
			draw(0, 0);
			g->popMatrix();*/
		}

		void Image::setBounds(float w, float h, float d) 
		{
			m_Width = w;
			m_Height = h;
			m_dirty = true;
			clean();
		}
		ARK::Geometry::Cube* Image::getBounds() 
		{
			if (m_dirty) { clean(); }
			return &m_bounds;
		}
		
		void Image::drawPivoted(float x, float y, signed int pivotX, signed int pivotY, float scale)
		{
			unsigned int oldWidth = (unsigned int) m_Width;
			unsigned int oldHeight = (unsigned int) m_Height;
			
			m_Width = float(m_Width) * scale;
			m_Height = float(m_Height) * scale;
			
			x -= pivotX;
			y -= pivotY;
			
			m_dirty = true;
			 
			draw(x, y);
			
			m_Width = oldWidth;
			m_Height = oldHeight;
			
			clean();
		}

		void Image::drawCenteredScaled(float x, float y, float scalex, float scaley) {
			float oldw = m_Width;
			float oldh = m_Height;
			m_Width *= scalex;
			m_Height *= scaley; 

			float oldCenterX = m_CenterX;
			float oldCenterY = m_CenterY;

			float oldCenterXpercentage = m_CenterX / oldw;
			float oldCenterYpercentage = m_CenterY / oldh;
			m_CenterX = m_Width * oldCenterXpercentage;
			m_CenterY = m_Height * oldCenterYpercentage;

			m_dirty = true;

			drawCentered(x, y);

			m_Width = oldw;
			m_Height = oldh; 

			m_CenterX = oldCenterX;
			m_CenterY = oldCenterY;

			clean();
		}

		//void Image::bind() const {
			//if (texture == s_current_texture_id) { return; }

			//glEnable(GL_TEXTURE_2D);
			//glBindTexture( GL_TEXTURE_2D, this->texture );
			//s_current_texture_id = texture;
		//}
		//void Image::unbind() const {
			//if (texture == s_current_texture_id) { return; }
			//if (s_current_texture_id == 0) { return; }
			//glBindTexture(GL_TEXTURE_2D, 0);
			//s_current_texture_id = 0;
			//glDisable(GL_TEXTURE_2D);
		//}

		/* void Image::copyAreaToImage(const Image& image, unsigned int dest_x, unsigned int dest_y, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
			bind();
			glCopyTexSubImage2D(image.texture, 0, x, y, dest_x, dest_y, width, height );
			unbind();
		} */
		/*
		void Image::setCornerColor(unsigned int corner, Color c) const {
			switch (corner) {
				case 0: 
					m_tl_corner_color = Color(c); break;
				case 1: 
					m_tr_corner_color = Color(c); break;
			}
		}*/


		void Image::drawSubImageStart() {

			if (Renderer::isBatching()) { return; }

			#ifdef NO_FIXED_FUNCTION_PIPELINE
				RendererState::start(RendererState::TEXTURE, this->m_texture->m_id);

				#if defined(ARK2D_OPENGL_3_2)

					/*Renderer::s_vaoQuad->bind();

					glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexPositionIn);
                    glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexNormalIn);
					glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexTexCoordIn);
					glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexColorIn);

					RendererStats::s_glCalls += 3;

					Renderer::s_vboQuadVerts->setWidth(3);
                    Renderer::s_vboQuadNormals->setWidth(3);
					Renderer::s_vboQuadTexCoords->setWidth(2);
					Renderer::s_vboQuadColors->setWidth(4);

					Renderer::s_vboQuadVerts->setHeight(6);
                    Renderer::s_vboQuadNormals->setHeight(6);
					Renderer::s_vboQuadTexCoords->setHeight(6);
					Renderer::s_vboQuadColors->setHeight(6);*/
					
				#endif

				
			#else 
				
				RendererState::start(RendererState::TEXTURE, this->m_texture->m_id);
				//bind();
				//glColor4f(1.0f, 1.0f, 1.0f, m_alpha);
				//RendererStats::s_glCalls++;
				//if (m_color != NULL) { glEnableClientState(GL_COLOR_ARRAY); }
				//glEnableClientState(GL_VERTEX_ARRAY);
				//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			#endif
			
		}
		void Image::drawSubImageEnd() {

			if (Renderer::isBatching()) { return; }
			
			#ifdef NO_FIXED_FUNCTION_PIPELINE
				//Renderer::s_shaderBasicTexture->unbind();  
			#else
				//glDisableClientState(GL_VERTEX_ARRAY);
				//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				//if (m_color != NULL) { glDisableClientState(GL_COLOR_ARRAY); }
				//unbind();
			#endif
		} 

		/** 
		 * the 1s: the texture sub-image
		 * the 2s: the location and width/height to draw at.
		 */
		void Image::drawSubImage(int x1, int y1, int x2, int y2, int w1, int h1, int w2, int h2) {

 
			Renderer* g = ARK2D::getRenderer();

			float old_texture_offset_x_tl = texture_offset_x_tl;
			float old_texture_offset_y_tl = texture_offset_y_tl;
			float old_texture_offset_x_tr = texture_offset_x_tr;
			float old_texture_offset_y_tr = texture_offset_y_tr;
			float old_texture_offset_x_bl = texture_offset_x_bl;
			float old_texture_offset_y_bl = texture_offset_y_bl;
			float old_texture_offset_x_br = texture_offset_x_br;
			float old_texture_offset_y_br = texture_offset_y_br;
			
			//float oldTextureOffsetX = texture_offset_x;
			//float oldTextureOffsetY = texture_offset_y;
			//float oldTextureWidth = texture_width;
			//float oldTextureHeight = texture_height;
			unsigned int oldWidth = (unsigned int) m_Width;
			unsigned int oldHeight = (unsigned int) m_Height;

			//float newTextureOffsetX = ((x1 / (float) m_Width) * this->texture_width) + this->texture_offset_x;
			//float newTextureOffsetY = ((y1 / (float) m_Height) * this->texture_height) + this->texture_offset_y;
			//float newTextureWidth = ((w1 / (float) m_Width) * this->texture_width);
			//float newTextureHeight = ((h1 / (float) m_Height) * this->texture_height);
			float newTextureOffsetX = ((x1 / (float) m_Width) * this->getTextureW()) + this->texture_offset_x_tl;
			float newTextureOffsetY = ((y1 / (float) m_Height) * this->getTextureH()) + this->texture_offset_y_tl;
			float newTextureWidth = ((w1 / (float) m_Width) * this->getTextureW());
			float newTextureHeight = ((h1 / (float) m_Height) * this->getTextureH());

 
			//texture_width = newTextureWidth;
			//texture_height = newTextureHeight;
			//texture_offset_x = newTextureOffsetX;
			//texture_offset_y = newTextureOffsetY;
			texture_offset_x_tl = newTextureOffsetX;
			texture_offset_y_tl = newTextureOffsetY;
			texture_offset_x_tr = newTextureOffsetX+newTextureWidth;
			texture_offset_y_tr = newTextureOffsetY;
			texture_offset_x_bl = newTextureOffsetX;
			texture_offset_y_bl = newTextureOffsetY+newTextureHeight;
			texture_offset_x_br = newTextureOffsetX+newTextureWidth;
			texture_offset_y_br = newTextureOffsetY+newTextureHeight;
			m_Width = w2;
			m_Height = h2;
			clean();

			Renderer* r = ARK2D::getRenderer();
			unsigned char color_r = r->getDrawColor().getRedc();
			unsigned char color_g = r->getDrawColor().getGreenc();
			unsigned char color_b = r->getDrawColor().getBluec();
			unsigned char color_a = (unsigned char) (r->getDrawColor().getAlphac() * m_alpha);

			if (m_color != NULL) {
				color_r = (unsigned char) (m_color->getRedc() * r->getDrawColor().getRedf());
				color_g = (unsigned char) (m_color->getGreenc() * r->getDrawColor().getGreenf());
				color_b = (unsigned char) (m_color->getBluec() * r->getDrawColor().getBluef());
				color_a = (unsigned char) (m_color->getAlphac() * (m_alpha * r->getDrawColor().getAlphaf()));
			} 
 
			if (Renderer::isBatching()) { 
				unsigned char red = color_r;
				unsigned char g = color_g;
				unsigned char b = color_b;
				unsigned char a = color_a;
				
				Renderer::getBatch()->addTexturedQuad( 
					m_texture->getId(), 
					x2,				y2,             0,
					x2 + m_Width,	y2,             0,
					x2,				y2 + m_Height,  0,
					x2 + m_Width,	y2 + m_Height,  0,
                      0,0,1,
                      0,0,1,
                      0,0,1,
                      0,0,1,
					//texture_offset_x, texture_offset_y,
					//texture_offset_x + texture_width, texture_offset_y,
					//texture_offset_x, texture_offset_y + texture_height,
					//texture_offset_x + texture_width, texture_offset_y + texture_height,
					texture_offset_x_tl, texture_offset_y_tl,
					texture_offset_x_tr, texture_offset_y_tr,
					texture_offset_x_bl, texture_offset_y_bl,
					texture_offset_x_br, texture_offset_y_br,
					red, g, b, a,
					red, g, b, a,
					red, g, b, a,
					red, g, b, a
				);

				//texture_width = oldTextureWidth;
				//texture_height = oldTextureHeight;
				//texture_offset_x = oldTextureOffsetX;
				//texture_offset_y = oldTextureOffsetY;
				texture_offset_x_tl = old_texture_offset_x_tl;
				texture_offset_y_tl = old_texture_offset_y_tl;
				texture_offset_x_tr = old_texture_offset_x_tr;
				texture_offset_y_tr = old_texture_offset_y_tr;
				texture_offset_x_bl = old_texture_offset_x_bl;
				texture_offset_y_bl = old_texture_offset_y_bl;
				texture_offset_x_br = old_texture_offset_x_br;
				texture_offset_y_br = old_texture_offset_y_br;

				m_Width = oldWidth;
				m_Height = oldHeight;
				clean(); 
									
				return;
			}

			g->pushMatrix();
			g->translate(x2, y2);
        
			#if defined(ARK2D_FLASCC) 
				
				float rawVertices[] = {
					0.0f,			0.0f,           0, // tl
					m_verts[9],		0.0f,           0, // tr
					0.0f,			m_verts[10],	0, // bl
					0.0f,			m_verts[10],	0, // bl
					m_verts[9],		0.0f,           0, // tr
					m_verts[9],		m_verts[10]		0  // br
				};

                float rawNormals[] = {
                    0,0,1,
                    0,0,1
                    0,0,1,
                    0,0,1,
                    0,0,1,
                    0,0,1
                };
            
				float texCoords[] = {
					this->texture_offset_x_tl,	this->texture_offset_y_tl,
					this->texture_offset_x_tr,	this->texture_offset_y_tr,
					this->texture_offset_x_bl,	this->texture_offset_y_bl,
					this->texture_offset_x_bl,	this->texture_offset_y_bl,
					this->texture_offset_x_tr,	this->texture_offset_y_tr,
					this->texture_offset_x_br,	this->texture_offset_y_br
				};
				
				unsigned char red = color_r;
				unsigned char gre = color_g;
				unsigned char b = color_b;
				unsigned char a = color_a;

				unsigned char rawColors[] = { 
					red, gre, b, a,
					red, gre, b, a,
					red, gre, b, a,
					red, gre, b, a,
					red, gre, b, a,
					red, gre, b, a
				};
            
                Shader* shader = RendererState::start(RendererState::TEXTURE, this->m_texture->m_id);
                shader->setData(rawVertices, rawNormals, texCoords, rawColors, 6);
                shader->drawTriangles()

			#elif defined( NO_FIXED_FUNCTION_PIPELINE )

				#if defined(ARK2D_OPENGL_3_2) ||  defined(ARK2D_OPENGL_ES_2_0)

                    Shader* shader = RendererState::start(RendererState::TEXTURE, this->m_texture->m_id);
                    shader->setData(m_verts, m_normals, m_texCoords, m_colors, 4);
                    shader->drawTriangleStrip();


				#elif defined(ARK2D_RENDERER_DIRECTX)

					ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
					ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;
					//ID3D11RenderTargetView* renderTargetView = ARK2D::getContainer()->m_platformSpecific.m_renderTargetView;
					//ID3D11DepthStencilView* depthStencilView = ARK2D::getContainer()->m_platformSpecific.m_depthStencilView;

					MatrixStack* modelMatrix = Renderer::getMatrix(MatrixStack::TYPE_MODEL);
					MatrixStack* viewMatrix = Renderer::getMatrix(MatrixStack::TYPE_VIEW);
					MatrixStack* projectionMatrix = Renderer::getMatrix(MatrixStack::TYPE_PROJECTION);

					float red = r->getDrawColor().getRedf();
					float gr = r->getDrawColor().getGreenf();
					float b = r->getDrawColor().getBluef();
					float a = m_alpha * r->getDrawColor().getAlphaf();
					if (m_color != NULL) {
						red = m_color->getRedf() * r->getDrawColor().getRedf();
						gr  = m_color->getGreenf() * r->getDrawColor().getGreenf();
						b   = m_color->getBluef() * r->getDrawColor().getBluef();
						a   = m_color->getAlphaf() * m_alpha * r->getDrawColor().getAlphaf();
					}

					Renderer::__internalsDXUpdateMatrices();

					//XMStoreFloat4x4(&Renderer::s_d3d_matrixBufferData->model, XMMatrixTranspose(modelMatrix->d3dpointer()));
					//XMStoreFloat4x4(&Renderer::s_d3d_matrixBufferData->view, XMMatrixTranspose(viewMatrix->d3dpointer()));
					//XMStoreFloat4x4(&Renderer::s_d3d_matrixBufferData->projection, XMMatrixTranspose(projectionMatrix->d3dpointer()));

					//ID3D11RenderTargetView *const targets[1] = { renderTargetView };
					//deviceContext->OMSetRenderTargets(1, targets, depthStencilView);

					//deviceContext->UpdateSubresource(Renderer::s_d3d_matrixBuffer, 0, NULL, Renderer::s_d3d_matrixBufferData, 0, 0);

					Renderer_DX_InterleavingTextureVertexData rawVertices[] = {
							{
								DirectX::XMFLOAT4(0, 0, 0.0f, 0.0f),
								DirectX::XMFLOAT2(this->texture_offset_x_tl, this->texture_offset_y_tl),
								DirectX::XMFLOAT4(red, gr, b, a)
							},
							{
								DirectX::XMFLOAT4(0 + m_verts[9], 0, 0.0f, 0.0f),
								DirectX::XMFLOAT2(this->texture_offset_x_tr, this->texture_offset_y_tr),
								DirectX::XMFLOAT4(red, gr, b, a)
							},
							{
								DirectX::XMFLOAT4(0, m_verts[10], 0.0f, 0.0f),
								DirectX::XMFLOAT2(this->texture_offset_x_bl, this->texture_offset_y_bl),
								DirectX::XMFLOAT4(red, gr, b, a)
							},
							{
								DirectX::XMFLOAT4(m_verts[9], m_verts[10], 0.0f, 0.0f),
								DirectX::XMFLOAT2(this->texture_offset_x_br, this->texture_offset_y_br),
								DirectX::XMFLOAT4(red, gr, b, a)
							}
					};

					unsigned int rawIndices[] = {
						0, 1, 2, 2, 1, 3
					};

					Renderer::s_vboQuadVerts->setData((void*)&rawVertices[0], (unsigned int) sizeof(Renderer_DX_InterleavingTextureVertexData) * 4);
					Renderer::s_vboIndices->setData((void*)&rawIndices[0], sizeof(unsigned int) * 6);

					unsigned int stride = sizeof(Renderer_DX_InterleavingTextureVertexData);
					unsigned int offset = 0;
					deviceContext->IASetVertexBuffers(0, 1, &Renderer::s_vboQuadVerts->m_buffer, &stride, &offset);

					deviceContext->IASetIndexBuffer(Renderer::s_vboIndices->m_buffer, DXGI_FORMAT_R32_UINT, 0);

					deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

					deviceContext->DrawIndexed(6, 0, 0); 

				#endif

				RendererStats::s_glCalls += 7;
				
				/*glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ModelMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODEL)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_VIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, &m_verts);  
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexTexCoordIn, 2, GL_FLOAT, GL_FALSE, 0, &m_texCoords);
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexColorIn, 4, GL_FLOAT, GL_FALSE, 0, &m_colors); 
				
				glUniform1i(Renderer::s_shaderBasicTexture_TextureId, 0);  

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  */

			#else 
				
				glVertexPointer(3, GL_FLOAT, 0, m_verts);
				glTexCoordPointer(2, GL_FLOAT, 0, m_texCoords);
				/*if (m_color != NULL) {*/ glColorPointer(4, GL_UNSIGNED_BYTE, 0, m_colors); //}

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				RendererStats::s_glCalls += 3;

			#endif

			g->popMatrix();

			//texture_width = oldTextureWidth;
			//texture_height = oldTextureHeight;
			//texture_offset_x = oldTextureOffsetX;
			//texture_offset_y = oldTextureOffsetY;
			texture_offset_x_tl = old_texture_offset_x_tl;
			texture_offset_y_tl = old_texture_offset_y_tl;
			texture_offset_x_tr = old_texture_offset_x_tr;
			texture_offset_y_tr = old_texture_offset_y_tr;
			texture_offset_x_bl = old_texture_offset_x_bl;
			texture_offset_y_bl = old_texture_offset_y_bl;
			texture_offset_x_br = old_texture_offset_x_br;
			texture_offset_y_br = old_texture_offset_y_br;

			m_Width = oldWidth;
			m_Height = oldHeight;
			clean(); 

			RendererStats::s_tris += 2;

		}



		void Image::setSize(int w, int h) {
			m_Width = w;
			m_Height = h;
			m_dirty = true;
		}

		Image::~Image() {
			//glDeleteTextures( 1, &texture );
		}

		void Image::render() {
			if (!visible) { return; }
			/*double tempRotation = SceneNode::rotation;
            SceneNode::rotation = 0.0f;
            draw(
				pivot.getX() * -1.0f * m_Width,// * SceneNode::scale.getX(),
				pivot.getY() * -1.0f * m_Height,// * SceneNode::scale.getY(),
				m_Width,// * SceneNode::scale.getX(),
				m_Height// * SceneNode::scale.getY()
			);
			SceneNode::rotation = tempRotation;*/
			//draw(0, 0);

			if (m_dirty) {
				clean();
			}

			preRenderFromPivot();
            
            Renderer* r = ARK2D::getRenderer();


			float batch_rawVertices[] = {
				0,				0,				0, // tl
				0 + m_Width,	0,				0, // tr
				0,				0 + m_Height,	0, // bl
				0,				0 + m_Height,	0, // bl
				0 + m_Width,	0,				0, // tr
				0 + m_Width,	0 + m_Height,	0 // br
			};

			unsigned char red = r->getDrawColor().getRedc();
			unsigned char g = r->getDrawColor().getGreenc();
			unsigned char b = r->getDrawColor().getBluec();
			unsigned char a = (unsigned char) (m_alpha * 255.0f);
			if (m_color != NULL) {
				red = (unsigned char) (m_color->getRedc() * r->getDrawColor().getRedf());
				g   = (unsigned char) (m_color->getGreenc() * r->getDrawColor().getGreenf());
				b   = (unsigned char) (m_color->getBluec() * r->getDrawColor().getBluef());
				a   = (unsigned char) (m_color->getAlphac() * (m_alpha * r->getDrawColor().getAlphaf()) );
			}

			Renderer::getBatch()->addTexturedQuad(
				m_texture->getId(), 
				batch_rawVertices[0], batch_rawVertices[1], batch_rawVertices[2],
				batch_rawVertices[3], batch_rawVertices[4], batch_rawVertices[5],
				batch_rawVertices[6], batch_rawVertices[7], batch_rawVertices[8],
				batch_rawVertices[15], batch_rawVertices[16], batch_rawVertices[17],
                  0,0,1,
                  0,0,1,
                  0,0,1,
                  0,0,1,
				texture_offset_x_tl, texture_offset_y_tl,
				texture_offset_x_tr, texture_offset_y_tr,
				texture_offset_x_bl, texture_offset_y_bl,
				texture_offset_x_br, texture_offset_y_br,
				red, g, b, a,
				red, g, b, a,
				red, g, b, a,
				red, g, b, a
			);
			

			postRenderFromPivot();

			SceneNode::renderChildren();


		}

		// http://stackoverflow.com/questions/2008842/creating-and-loading-pngs-in-rgba4444-rgba5551-for-opengl
		void* Image::util_rgba8888_to_rgba4444(void* src, int cb)
		{
		    int i;
		    // compute the actual number of pixel elements in the buffer.
		    int cpel = cb/4;
		    unsigned long* psrc = (unsigned long*)src;
		    unsigned short* pdst = (unsigned short*)src;

		    // convert every pixel
		    for(i=0;i<cpel; i++)
		    {
		        // read a source pixel
		        unsigned pel = psrc[i];
		        // unpack the source data as 8 bit values
		        unsigned r = (pel << 8)  & 0xf000;
		        unsigned g = (pel >> 4) & 0x0f00;
		        unsigned b = (pel >> 16) & 0x00f0;
		        unsigned a = (pel >> 28) & 0x000f;

		        // and store
		        pdst[i] = r | g | b | a;
		    }
		    return pdst;
		}

	}
}
