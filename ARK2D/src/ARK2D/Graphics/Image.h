/*
 * Image.h
 *
 *  Created on: 06-Nov-2009
 *      Author: Ashley
 */

#ifndef IMAGE_H_
#define IMAGE_H_

//#include "../Includes.h"
#include "../Namespaces.h"
#include "../Windres.h"

#include <string>
using namespace std;



#include "../Core/Resource.h"
#include "Color.h"
#include "ImageIO/TargaImage.h"
#include "ImageIO/BMPImage.h"
#include "ImageIO/PNGImage.h"
#include "../SceneGraph/Scene.h"

#include "../Geometry/Cube.h"


namespace ARK {
	namespace Graphics {

		class ARK2D_API Image : public ARK::Core::Resource, public ARK::SceneGraph::SceneNode {
			friend class TextureStore;
			friend class ARK::Core::Resource;
			private:

				void* m_data; // only used by one constructor.
				unsigned int m_dataLength; // used for pkm reading
				unsigned int m_resourceType; // ARK2D_RESOURCE_TYPE_PNG only supported.

				std::string filename;
				float m_Width;
				float m_Height;
				ARK::Geometry::Cube m_bounds;

				// The dimensions of the sprite (usually in a spritesheet). 
				// We need this so we can change the dimensions by a scale value.
				float m_originalWidth; 
				float m_originalHeight; 

				Texture* m_texture;
				unsigned int texture_temp;

				// DX texture bits
				static unsigned int s_dxNextTextureId;
				#if defined(ARK2D_RENDERER_DIRECTX)
					ID3D11Texture2D* m_dxTexture;
					ID3D11SamplerState* m_dxSampler;
					ID3D11ShaderResourceView* m_dxResourceView;
				#else
					void* m_dxTexture;
					void* m_dxSampler;
					void* m_dxResourceView;
				#endif

				//float texture_width;
				//float texture_height;
				//float texture_offset_x;
				//float texture_offset_y;

				float texture_offset_x_tl;
				float texture_offset_y_tl;
				float texture_offset_x_tr;
				float texture_offset_y_tr;
				float texture_offset_x_bl;
				float texture_offset_y_bl;
				float texture_offset_x_br;
				float texture_offset_y_br;
				float texture_source_w;
				float texture_source_h;

				float m_CenterX; // used for rotation
				float m_CenterY;

				Color m_tl_corner_color;
				Color m_tr_corner_color;

				float m_alpha;
				Color* m_color;


				
				

				//#if defined(ARK2D_OPENGL_ES_2_0)
				//	bool m_dirty;
				//	Renderer_InterleavingTextureVertexData m_vertices[4];
				//#else
					bool m_dirty;
					float m_verts[12];
					float m_texCoords[8];
					float m_normals[12];
					unsigned char m_colors[16];
				//#endif

			public:
				GLuint load();
				GLuint load(const Color& mask);
				TargaImage* loadTGA();
				BMPImage* loadBMP();
				PNGImage* loadPNG();

				void addTexture();

			public:
				Image();
				Image(unsigned int resource, unsigned int resourceType);
				Image(void* data, unsigned int resourceType, string file);
				Image(void* data, unsigned int dataLength, unsigned int resourceType, string file);
				Image(const std::string& fname);
				Image(const std::string& fname, const Color& mask);
				unsigned int getWidth() const;
				unsigned int getHeight() const;

				Image* setAlpha(float f);
				float getAlpha() const;

				void setColor(const Color& c); // color overrides alpha... or does it now?
				void setColor(Color* c); // color overrides alpha... or does it now?
				void setColor(float r, float g, float b, float a);
				Color* getColor() const;


				inline Texture* getTexture() { return m_texture; };
				inline float getTextureX() { return texture_offset_x_tl; };
				inline float getTextureY() { return texture_offset_y_tl; };
				float getTextureW() const;
				float getTextureH() const;
				inline float getTextureX_tl() { return texture_offset_x_tl; };
				inline float getTextureY_tl() { return texture_offset_y_tl; };
				inline float getTextureX_tr() { return texture_offset_x_tr; };
				inline float getTextureY_tr() { return texture_offset_y_tr; };
				inline float getTextureX_bl() { return texture_offset_x_bl; };
				inline float getTextureY_bl() { return texture_offset_y_bl; };
				inline float getTextureX_br() { return texture_offset_x_br; };
				inline float getTextureY_br() { return texture_offset_y_br; };
				
				// This is the texture width that never changes!
				inline float getSourceWidth() const { return texture_source_w; }
				inline float getSourceHeight() const { return texture_source_h; }


				void setDirty(bool b);
				void clean();

				Image* setCenterOfRotation(int x, int y);
				inline int getCenterOfRotationX() { return (int) m_CenterX; }
				inline int getCenterOfRotationY() { return (int) m_CenterY; }
				
				virtual SceneNode* rotate(double angle); // return self
				virtual SceneNode* setRotation(double angle);

				virtual SceneNode* scale(float x, float y); // returns self.
				virtual SceneNode* setScale(float x, float y);

				Image* flip(bool flipx, bool flipy); // return self.
				Image* setFlipped(bool flipx, bool flipy);
				
				Image* getSubImage(const SpriteSheetDescription* desc, const char* item);
				Image* getSubImage(const SpriteSheetDescriptionItem& desc);
				Image* getSubImage(int x, int y, int width, int height) const;
				Image* getCopy();
				Image* getScaledCopy(unsigned int x, unsigned int y);
				Image* getScaledCopy(float x, float y);
				Image* getFlippedCopy(bool horizontal_flip, bool vertical_flip);
				void setSize(int x, int y);
				void setWidth(int w);
				void setHeight(int h);

                virtual	void setBounds(float w, float h, float z);
                virtual	ARK::Geometry::Cube* getBounds();
            
				//void bind() const;
				//void unbind() const;
				void draw();
				void draw(int x, int y);
				void draw(float x, float y);
				void draw(float x, float y, float z);
				void draw(float x, float y, unsigned int w, unsigned int h);
				void drawAligned(float x, float y, signed int alignX, signed int alignY, float scale=1.0f);
				void drawPivoted(float x, float y, signed int pivotX, signed int pivotY, float scale);
				void drawCentered(int x, int y);
				void drawCentered(float x, float y);
				void drawCenteredFlipped(float x, float y, bool flipx, bool flipy);
				void drawCenteredScaled(float x, float y, float scalex, float scaley);
				void drawFlipped(float x, float y, bool flipx, bool flipy); // not const as modifies inner variables
				void drawSubImageStart();
				void drawSubImage(int x1, int y1, int x2, int y2, int w1, int h1, int w2, int h2);
				void drawSubImageEnd();
				//void setCornerColor(unsigned int corner, Color c) const;
				//void copyAreaToImage(const Image& image, unsigned int dest_x, unsigned int dest_y,
				//						unsigned int x, unsigned int y, unsigned int width, unsigned int height);

				static string getGlErrorString(int error);
				static void showAnyGlErrorAndExit();
				static void showAnyGlErrorAndExit(const char* fname, int line);

				// http://stackoverflow.com/questions/2008842/creating-and-loading-pngs-in-rgba4444-rgba5551-for-opengl
				// src 	IN 	pointer to source buffer, 
				// cb 	IN 	size of source buffer, in bytes
				static void* util_rgba8888_to_rgba4444( void* src, int cb); 

				private:
					unsigned int createDXTexture(int w, int h, void* data);
				public:

					static Image* s_dummy;
					static Image* getDummy();

					virtual ~Image();

				// SceneGraph
					virtual void render();
		};
	}
}

#endif /* IMAGE_H_ */
