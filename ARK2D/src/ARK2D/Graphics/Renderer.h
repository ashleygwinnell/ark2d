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

namespace ARK {
	namespace Graphics {

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

			/*
			public:
				static const int RENDER_MODE = 0;
				static const int RENDER_MODE_RECTS = 0;
				static const int RENDER_MODE_LINES = 0;
				static const int RENDER_MODE_POINTS = 0;
				static const int RENDER_MODE_LINES = 0;
				static const int RENDER_MODE_TEXTURE = 0;
				static int RENDER_MODE_TEXTURE_ID = 0;
			*/

			public:
				Renderer();

				void setFont(ARK::Font::Font* f);
				void setDefaultFont(ARK::Font::Font* f);
				ARK::Font::Font* getFont() const ;
				ARK::Font::Font* getDefaultFont() const;

				void drawString(const std::string str, int x, int y) const;
				void drawStringCenteredAt(const std::string str, int x, int y) const;
				void drawStringWordWrap(const std::string str, int x, int y, int maxWidth, int ySpacing) const;

				void setDrawColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
				void setDrawColorf(float r, float g, float b, float a);
				void setDrawColor(const Color& c);
				const Color& getDrawColor() const;

				void setMaskColor(const Color& c);
				const Color& getMaskColor() const;

				void translate(int x, int y) const;
				void translate(float x, float y) const;

				void rotate(int angle) const;

				void scale(float x, float y) const;

				void setScissorTestEnabled(bool b) const;
				void scissor(int x, int y, int w, int h) const;

				void viewport(int x, int y, int w, int h) const;

				void ortho2d(int x, int y, int w, int h) const;
				void ortho2d(int x, int y, int w, int h, double near, double far) const;


				void pushMatrix() const;
				void popMatrix() const;

				void fillArc(int cx, int cy, int width, int height, float startAngle, float endAngle) const;
				void fillArc(int cx, int cy, int width, int height, float startAngle, float endAngle, int segs) const;

				void drawLine(int x1, int y1, int x2, int y2) const;
				void drawRect(int x, int y, int width, int height) const;
				void drawRects(int rects[], int colors[] = NULL) const;
				void fillRect(int x, int y, int width, int height) const;
				void fillRoundedRect(int x, int y, int width, int height, int radius) const;
				void fillRoundedRect(int x, int y, int width, int height, int radius, int segs) const;

				void fillTriangle(int x, int y, int width, int height) const;

				void drawPoint(int x, int y) const;
				void drawPoint(float x, float y) const;

				void drawCircle(int x, int y, int radius, int points) const;
				void fillCircle(int x, int y, int radius, int points) const;

				void drawCircleSpikey(int x, int y, int radius, int points) const;
				void fillCircleSpikey(int x, int y, int radius, int points) const;

				void setLineWidth(unsigned int i);
				unsigned int getLineWidth();

				void setPointSize(float i);
				float getPointSize();

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
		};


	}
}

#endif /* GRAPHICS_H_ */
