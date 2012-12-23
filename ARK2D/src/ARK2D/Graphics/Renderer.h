/*
 * Renderer.h
 *
 *  Created on: 15-Oct-2009
 *      Author: user
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <string>
#include "../Font/BMFont.h"
#include "Color.h"
#include "../Util/StringUtil.h"
#include "../Util/Matrix44.h"
#include "../Font/Font.h"

#include "../Includes.h"
#include "../Namespaces.h"



namespace ARK { 
	namespace Graphics {

		class RendererStats 
		{
			public: 
				static unsigned int s_lines;
				static unsigned int s_tris;
				static unsigned int s_textureSwaps;	
				static void reset();

				RendererStats();
				virtual ~RendererStats();
		};

		class RendererState {
			public:
				static const int NONE = 0;
				static const int GEOMETRY = 1;
				static const int TEXTURE = 2;

				static int s_renderMode;
				static int s_textureId;

				static void start(int renderMode);
				static void start(int renderMode, int textureId);

				static void startGeometry();
				static void startTexture(int textureId);

                static void internalBindTexture(int textureId);
				
		};

		/*!
		 * \brief Contains rendering methods.
		 *
		 * @warning Primitive shapes are slow on most mobile devices so try to use textures where you can.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class Renderer
		{
			friend class ARK::Core::GameContainer;
			friend class ARK::Core::GameContainerPlatform;

			private:
				static const int DEFAULT_SEGMENTS = 50;

			/*public:
				static const int RENDER_MODE = 0;
				static const int RENDER_MODE_RECTS = 0;
				static const int RENDER_MODE_LINES = 0;
				static const int RENDER_MODE_POINTS = 0;
				static const int RENDER_MODE_LINES = 0;
				static const int RENDER_MODE_TEXTURE = 0;
				static int RENDER_MODE_TEXTURE_ID = 0;
			*/

				static signed int s_interpolation;

			public:

				static const signed int ALIGN_START = -1;
				static const signed int ALIGN_LEFT = -1;
				static const signed int ALIGN_TOP = -1;

				static const signed int ALIGN_CENTER = 0;

				static const signed int ALIGN_END = 1;
				static const signed int ALIGN_RIGHT = 1;
				static const signed int ALIGN_BOTTOM = 1;

				static const signed int INTERPOLATION_NEAREST = 0;
				static const signed int INTERPOLATION_LINEAR = 1;
				static const signed int INTERPOLATION_BICUBIC = 2;
			

				Renderer();

				void setFont(ARK::Font::Font* f);
				void setDefaultFont(ARK::Font::Font* f);
				ARK::Font::Font* getFont() const ;
				ARK::Font::Font* getDefaultFont() const;

				void drawString(const std::string str, int x, int y) const;
				void drawString(const std::string str, float x, float y, signed int alignX, signed int alignY, float rotation=0.0f, float scale=1.0f);
				void drawStringCenteredAt(const std::string str, int x, int y) const;
				void drawStringWordWrap(const std::string str, int x, int y, int maxWidth, int ySpacing) const;

				static signed int getInterpolation();
				static void setInterpolation(signed int i);
				static unsigned int getInterpolationGL() {
					switch(s_interpolation) {
						case INTERPOLATION_NEAREST:
							return GL_NEAREST;
						case INTERPOLATION_LINEAR:
							return GL_LINEAR;
					}
					return GL_NEAREST;
				}

				void setDrawColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
				void setDrawColorf(float r, float g, float b, float a);
				void setDrawColor(const Color& c);
				void setDrawColor(Color* c);
				const Color& getDrawColor() const;

				void setMaskColor(const Color& c);
				const Color& getMaskColor() const;

				void translate(int x, int y) const;
				void translate(float x, float y) const;

				void rotate(int angle) const;
				void rotate(float angle) const;

				void scale(float x, float y) const;

				void setScissorTestEnabled(bool b) const;
				void scissor(int x, int y, int w, int h) const;

				void viewport(int x, int y, int w, int h) const;

				void ortho2d(int x, int y, int w, int h) const;
				void ortho2d(int x, int y, int w, int h, double near, double far) const;


				void pushMatrix() const;
				void popMatrix() const;

				void fillArc(float cx, float cy, int width, int height, float startAngle, float endAngle) const;
				void fillArc(float cx, float cy, int width, int height, float startAngle, float endAngle, int segs) const;

				void drawLine(int x1, int y1, int x2, int y2) const;
				void texturedLineOverlay(Image* img, float scale, float x1, float y1, float x2, float y2, float startX, float startY, float endX, float endY);

				void drawRect(ARK::Geometry::Rectangle<int>* rect) const;
				void drawRect(ARK::Geometry::Rectangle<float>* rect) const;
				void drawRect(float x, float y, int width, int height) const;
				void drawRects(float rects[], int colors[] = NULL) const;
				
				void fillRect(float x, float y, int width, int height) const;
				void fillRoundedRect(float x, float y, int width, int height, int radius) const;
				void fillRoundedRect(float x, float y, int width, int height, int radius, int segs) const;

				void fillGradientRect(float x, float y, float width, float height, Color* top, Color* bottom) const;

				void texturedRect(unsigned int texId, float x, float y, int width, int height) const;

				void fillQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) const;
				void texturedQuad(unsigned int texId, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float tx4, float ty4);



				void fillTriangle(int x, int y, int width, int height) const;
				void fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3) const;

				void drawPoint(int x, int y) const;
				void drawPoint(float x, float y) const;

				void drawCircle(ARK::Geometry::Circle<int>* circle) const;
				void drawCircle(ARK::Geometry::Circle<float>* circle) const;
				void drawCircle(float x, float y, int radius) const;
				void drawCircle(float x, float y, int radius, int points) const;
				void fillCircle(float x, float y, int radius, int points) const;

				void fillEllipse(float x, float y, float rx, float ry) const;

				void drawCircleSpikey(float x, float y, int radius, int points) const;
				void fillCircleSpikey(float x, float y, int radius, int points) const;

				void setLineWidth(unsigned int i);
				unsigned int getLineWidth();

				void setPointSize(float i);
				float getPointSize();

				void drawScissorBoxes();
				void setScissorBoxColors(const Color& top, const Color& left, const Color& bottom, const Color& right);

				static const unsigned int BLEND_NONE = 0;
				static const unsigned int BLEND_NORMAL = 1;
				static const unsigned int BLEND_ADDITIVE = 2;
				static const unsigned int BLEND_TEST = 3;
				void setBlendMode(unsigned int blendMode);
				unsigned int getBlendMode();

				void loadMatrix(Matrix44<float>* m) const;
				void loadMatrix(const Matrix44<float>& m) const;
				void resetMatrix() const;

			private:
				ARK::Font::Font* m_DefaultFont;
				ARK::Font::Font* m_Font;
				Color m_DrawColor;
				Color m_MaskColor;
				unsigned int m_LineWidth;
				float m_pointSize;
				unsigned int m_blendMode;

				Color m_ScissorBoxColors[4]; // top, left, bottom, right;
		};


	}
}

#endif /* GRAPHICS_H_ */
