 /*
 * Renderer.cpp
 *  Created on: 16-Oct-2009
 *      Author: Ashley Gwinnell
 */

#include "Renderer.h"
#include "Color.h"
#include "../Font/Font.h"
#include "../Font/BMFont.h" 
#include "../Font/FTFont.h"

#include "../Includes.h"
#include "../ARK2D.h"
#include "../Core/GameContainer.h"

#include "../Geometry/Rectangle.h"
#include "../Geometry/Circle.h"
#include "../Geometry/Polygon.h"
#include "../Geometry/Line.h"

#include "Texture.h"
#include "TextureStore.h"
 
namespace ARK {  
	namespace Graphics {

		// renderer statistics
		unsigned int RendererStats::s_lines = 0;
		unsigned int RendererStats::s_tris = 0;
		unsigned int RendererStats::s_textureSwaps = 0;	

		RendererStats::RendererStats() {

		}
		void RendererStats::reset() {
			s_lines = 0;
			s_tris = 0;
			s_textureSwaps = 0;
		}
		RendererStats::~RendererStats() {
 
		}


		// renderer state
		int RendererState::s_renderMode = -1;
		int RendererState::s_textureId = 0;

		void RendererState::start(int renderMode) {
			start(renderMode, 0);
		}
		void RendererState::start(int renderMode, int textureId) 
		{
			#if defined(ARK2D_FLASCC)
				if (renderMode == GEOMETRY) { startGeometry(); }
				else if (renderMode == TEXTURE) { startTexture(textureId); }
			#else 
				if (renderMode == GEOMETRY && (renderMode != s_renderMode || s_renderMode == 0)) { startGeometry(); } 
				else if (renderMode == TEXTURE && (textureId != s_textureId || s_textureId == 0)) { startTexture(textureId); }
			#endif

			s_renderMode = renderMode;
		} 

		void RendererState::startGeometry() {

			s_textureId = 0;
			RendererStats::s_textureSwaps++;

			glBindTexture(GL_TEXTURE_2D, 0);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisable(GL_TEXTURE_2D);

			glEnableClientState(GL_VERTEX_ARRAY);
		}
		void RendererState::startTexture(int textureId) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			
			s_textureId = textureId;
			RendererStats::s_textureSwaps++;
		}


		void RendererState::internalBindTexture(int textureId) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureId);
			
			s_textureId = textureId;
		}

		signed int Renderer::s_interpolation = Renderer::INTERPOLATION_NEAREST;
		signed int Renderer::getInterpolation() {
			return s_interpolation;
		} 
		void Renderer::setInterpolation(signed int i) {
			s_interpolation = i;
		}


		// renderer
		Renderer::Renderer():
			m_DefaultFont(NULL),
			m_Font(NULL), 
			m_DrawColor(255, 255, 255, 255),
			m_MaskColor(),
			m_LineWidth(1),
			m_pointSize(1),
			m_blendMode(BLEND_NORMAL), 
			m_ScissorBoxColors()
		{
			//m_DefaultFont = new BMFont("data/calibri.fnt", "data/calibri.bmp", Color::magenta);
			//m_Font = m_DefaultFont;
		}



		void Renderer::setFont(ARK::Font::Font* f) {
			m_Font = f;
		} 
		void Renderer::setDefaultFont(ARK::Font::Font* f) {
			m_DefaultFont = f;
		}

		ARK::Font::Font* Renderer::getFont() const {
			return m_Font;
		}
		ARK::Font::Font* Renderer::getDefaultFont() const {
			return m_DefaultFont;
		} 

		void Renderer::translate(int x, int y) const {
			glTranslatef(x,y,0);
		} 
		void Renderer::translate(float x, float y) const {
			glTranslatef(x,y,0);
		}
		void Renderer::rotate(int angle) const { 
			glRotatef(angle, 0, 0, 1); 
		}
		void Renderer::rotate(float angle) const { 
			glRotatef(angle, 0, 0, 1);
		}
		void Renderer::scale(float x, float y) const {
			glScalef(x, y, 0.0f);
		}
		void Renderer::setScissorTestEnabled(bool b) const {
			if (b) {
				glEnable(GL_SCISSOR_TEST);
			} else {
				glDisable(GL_SCISSOR_TEST);
			}
		}
		void Renderer::scissor(int x, int y, int w, int h) const {
			//glScissor(x, y - (signed int) ARK2D::getContainer()->getHeight(), w, h * -1);
			//glScissor(x, ARK2D::getContainer()->getHeight() - (y + h), w, h);
			glScissor(x, ARK2D::getContainer()->getDynamicHeight() - (y + h), w, h);
		}
		void Renderer::viewport(int x, int y, int w, int h) const {
			glViewport(x, y, w, h);
		}
		void Renderer::ortho2d(int x, int y, int w, int h) const {
			ortho2d(x, y, w, h, -1, 1);
		}
		void Renderer::ortho2d(int x, int y, int w, int h, double near, double far) const {
			//glOrtho((double) x, double(x + w), double(y + h), (double) y, near, far);
			//glOrtho(0, w, h, 0, -1, 1);
			ARK2D::getContainer()->disable2D();
			ARK2D::getContainer()->enable2D();
		}

		void Renderer::drawString(const std::string str, int x, int y) const {
			if (m_Font == NULL) { ARK2D::getLog()->e("Renderer::m_Font is NULL..."); return; }
			ARK2D::getLog()->i("draw string"); 
			//m_Font->asBMFont()->getImage()->draw(x, y);
			m_Font->drawString(str, x, y);
		}
		void Renderer::drawString(const std::string str, float x, float y, signed int alignX, signed int alignY, float rotation, float sc) 
		{
			float strWidth = float(m_Font->getStringWidth(str)) * sc;
			float strHeight = float(m_Font->getLineHeight()) * sc;

			//if (rotation != 0 && alignY == ALIGN_CENTER) // there's gotta be a better way of calculating height of rotated rectangle.
			//{ 
				/*float cx = x + (strWidth/2);
				float cy = y + (strHeight/2);
				Vector2<float> tl(x, y);
				Vector2<float> bl(x, y + strHeight);
				Vector2<float> br(x + strWidth, y + strHeight);
				Vector2<float> tr(x + strWidth, y);
				Vector2<float> c(cx, cy);

				MathUtil::rotatePointAroundPoint<float>(&tl, &c, rotation);
				MathUtil::rotatePointAroundPoint<float>(&bl, &c, rotation);
				MathUtil::rotatePointAroundPoint<float>(&br, &c, rotation);
				MathUtil::rotatePointAroundPoint<float>(&tr, &c, rotation);
				
				vector<Vector2<float>* > coords();
				coords.push_back(&tl);  
				coords.push_back(&bl); 
				coords.push_back(&br);  
				coords.push_back(&tr);

				Vector2<float> mincoord = *(MathUtil::minY<float>(coords));
				Vector2<float> maxcoord = *(MathUtil::maxY<float>(coords));

				strHeight = maxcoord.getY() - mincoord.getY();*/
				//strHeight = strHeight * (abs(rotation)*0.1f);
			//}

			// Alignment
			/*if (alignX == ALIGN_CENTER) {
				x -= strWidth/2;
			} else if (alignX == ALIGN_RIGHT || alignX == ALIGN_END) {
				x -= strWidth; 
			}

			if (alignY == ALIGN_CENTER) {
				y -= strHeight/2;
			} else if (alignY == ALIGN_END || alignY == ALIGN_BOTTOM) {
				y -= strHeight;
			}*/ 

			float inv_scale = 1.0f / sc;

			float offsetX = 0.0f;
			if (alignX == ALIGN_CENTER) { 
				offsetX = (strWidth/2.0f) * -1.0f;	
			} else if (alignX == ALIGN_LEFT || alignX == ALIGN_START) {
				
			} else if (alignX == ALIGN_RIGHT || alignX == ALIGN_END) {
				offsetX = strWidth * -1.0f;
			} 

			float offsetY = 0.0f;
			if (alignY == ALIGN_CENTER) { 
				offsetY = (strHeight/2.0f) * -1.0f;	
			} else if (alignY == ALIGN_START || alignY == ALIGN_TOP) {
				
			} else if (alignY == ALIGN_END || alignY == ALIGN_BOTTOM) {
				offsetY = strHeight * -1.0f;
			} 


			glPushMatrix();
			glTranslatef(x, y, 0);

			if (rotation != 0.0f)
				rotate(rotation);
			
			glPushMatrix();

			if (sc != 1.0f) 
				scale(sc, sc);



			m_Font->drawString(str, 0 + (offsetX*inv_scale), 0 + (offsetY*inv_scale));

			if (sc != 1.0f) 
				scale(1.0f/sc, 1.0f/sc);
			
			glPopMatrix();

			if (rotation != 0.0f)
				rotate(rotation * -1);

			glTranslatef(x * -1, y * -1, 0);
			

			glPopMatrix();

		}
		void Renderer::drawStringCenteredAt(const std::string str, int x, int y) const {
			m_Font->drawStringCenteredAt(str, x, y);
		}
		void Renderer::drawStringWordWrap(const std::string str, int x, int y, int maxWidth, int ySpacing) const {
			int accumulatedWidth = 0;
			unsigned int lastLineUpToWord = 0;

			std::vector<std::string> lines;
			std::vector<std::string> words = StringUtil::split(str, " ");

			for (unsigned int i = 0; i < words.size(); i++) {
				accumulatedWidth += m_Font->getStringWidth(words.at(i).append(" "));
				if (i == words.size()-1 && lastLineUpToWord != words.size()) {
					accumulatedWidth = maxWidth + 1;
					i += 1;
				}
				if (accumulatedWidth > maxWidth) {
					std::string line = "";
					for (unsigned int j = lastLineUpToWord; j < i; j++) {
						line += words.at(j) + " ";
					}
					lines.push_back(line);
					lastLineUpToWord = i;
					accumulatedWidth = 0;
				}
			}

			for (unsigned int i = 0; i < lines.size(); i++) {
				drawString(lines.at(i), x, y);
				y += m_Font->getStringHeight(lines.at(i)) + ySpacing;
			}
		}

		void Renderer::drawLine(int x1, int y1, int x2, int y2) const {
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))

			#else
				RendererState::start(RendererState::GEOMETRY);
				glBegin(GL_LINES);
					glVertex2i(x1, y1);
					glVertex2i(x2, y2);
				glEnd();

				RendererStats::s_lines++;
			#endif
		}
		// image, scale... line coordinates. image line subcoordinates.
		void Renderer::texturedLineOverlay(
			Image* img, 
			float scale, 
			float x1, float y1, 
			float x2, float y2,
			float startX, float startY, 
			float endX, float endY) 
		{
			
			Vector2<float> tempVector(0, 0);
			Vector2<float> tempVector2(0, 0);

			float lineAngle = MathUtil::anglef(x1, y1, x2, y2);

			// top left. 
			tempVector.set( (startX * -1) * scale, (startY * -1) * scale);
			MathUtil::rotatePointAroundPoint(&tempVector, &tempVector2, lineAngle);
			float tl_x = x1 + tempVector.getX();
			float tl_y = y1 + tempVector.getY();

			// bottom left. 
			tempVector.set( (startX * -1) * scale, (img->getHeight() - startY) * scale);
			MathUtil::rotatePointAroundPoint(&tempVector, &tempVector2, lineAngle);
			float bl_x = x1 + tempVector.getX();
			float bl_y = y1 + tempVector.getY();

			// top right. 
			tempVector.set( (img->getWidth() - endX) * scale, (endY * -1) * scale);
			MathUtil::rotatePointAroundPoint(&tempVector, &tempVector2, lineAngle);
			float tr_x = x2 + tempVector.getX();
			float tr_y = y2 + tempVector.getY();

			// bottom right. 
			tempVector.set( (img->getWidth() - endX) * scale, (img->getHeight() - endY) * scale);
			MathUtil::rotatePointAroundPoint(&tempVector, &tempVector2, lineAngle);
			float br_x = x2 + tempVector.getX();
			float br_y = y2 + tempVector.getY();

			glColor4f(1.0f, 1.0f, 1.0f, img->getAlpha());
			texturedQuad(img->getTexture()->getId(), 
				tl_x, tl_y, 
				bl_x, bl_y, 
				br_x, br_y, 
				tr_x, tr_y, 
				img->getTextureX(), img->getTextureY(), 
				img->getTextureX(), img->getTextureY() + img->getTextureH(),
				img->getTextureX() + img->getTextureW(), img->getTextureY() + img->getTextureH(),
				img->getTextureX() + img->getTextureW(), img->getTextureY()
			);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		}

		void Renderer::fillArc(float cx, float cy, int width, int height, float startAngle, float endAngle) const {
			fillArc(cx, cy, width, height, startAngle, endAngle, DEFAULT_SEGMENTS);
		}
		void Renderer::fillArc(float cx, float cy, int width, int height, float startAngle, float endAngle, int segs) const {
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
			#else

				int halfwidth = width/2;
				int halfheight = height/2;

				glBegin(GL_TRIANGLE_FAN);

					glVertex2f(cx, cy);

					int step = 360 / segs;

					for (int i = (int) startAngle; i < (endAngle + step); i += step) {
						float a = i;
						if (a > endAngle) {
							a = endAngle;
						}
						float x = cx + cos(MathUtil::toRadians(a)) * halfwidth;
						float y = cy + (sin(MathUtil::toRadians(a)) * halfheight * -1); // we want to stick to the angles scheme we have going.

						glVertex2f(x, y);
					}

				glEnd();

				RendererStats::s_tris += segs;

			#endif
		}

		void Renderer::fillRoundedRect(float x, float y, int width, int height, int radius) const {
			fillRoundedRect(x, y, width, height, radius, DEFAULT_SEGMENTS);
		}
		void Renderer::fillRoundedRect(float x, float y, int width, int height, int radius, int segs) const {
			if (radius <= 0) {
				fillRect(x, y, width, height);
				return;
			}

			fillRect(x + radius, y, width - (radius*2), height);
			fillRect(x, y + radius, radius, height - (radius*2));
			fillRect(x+width-radius, y + radius, radius, height - (radius*2));

			fillArc(x + radius, y + radius, 2*radius, 2*radius, 90, 180, segs);
			fillArc(x + width - radius, y + radius, 2*radius, 2*radius, 0, 90, segs);
			fillArc(x + radius, y + height - radius, 2*radius, 2*radius, 180, 270, segs);
			fillArc(x + width - radius, y + height - radius, 2*radius, 2*radius, 270, 360, segs);
		}

		
		/*
		 * Immediate Mode rendering! :(
		 * ...what ever happened to vertex arrays?
		 * ...or maybe even OpenGL 3.0: hello deprecation model!
		 */
		void Renderer::drawRect(ARK::Geometry::Rectangle<int>* rect) const { 
		 	drawRect(rect->getMinX(), rect->getMinY(), rect->getWidth(), rect->getHeight());
		}
		void Renderer::drawRect(ARK::Geometry::Rectangle<float>* rect) const { 
		 	drawRect(rect->getMinX(), rect->getMinY(), (int) rect->getWidth(), (int) rect->getHeight());
		}
		void Renderer::drawRect(float x, float y, int width, int height) const {
			//#if defined(ARK2D_ANDROID)


				#if defined(ARK2D_FLASCC)
					RendererState::start(RendererState::GEOMETRY);

					float hlw = (m_LineWidth/2.0f);
					float fw = (float) width;
					float fh = (float) height;
					float rawVertices[] = {
						// left side
						0.0f - hlw,		0.0f,	// tl
						0.0f + hlw,		0.0f,	// tr
						0.0f - hlw,		fh,	  	// bl
						0.0f - hlw,		fh,	  	// bl
						0.0f + hlw,		0.0f,	// tr
						0.0f + hlw,		fh,		// br

						// right side side
						fw - hlw,		0.0f,	// tl
						fw + hlw,		0.0f,	// tr
						fw - hlw,		fh,	  	// bl
						fw - hlw,		fh,	  	// bl
						fw + hlw,		0.0f,	// tr
						fw + hlw,		fh,		// br

						// top side
						0.0f,		0.0f - hlw,	// tl
						0.0f+fw,	0.0f - hlw,	// tr
						0.0f,		0.0f + hlw,	// bl
						0.0f,		0.0f + hlw,	// bl
						0.0f+fw,	0.0f - hlw,	// tr
						0.0f+fw,	0.0f + hlw,	// br

						// bottom side
						0.0f,		fh - hlw,	// tl
						0.0f+fw,	fh - hlw,	// tr
						0.0f,		fh + hlw,	// bl
						0.0f,		fh + hlw,	// bl
						0.0f+fw,	fh - hlw,	// tr
						0.0f+fw,	fh + hlw	// br
					};

					glPushMatrix();
					glTranslatef(x, y, 0);
				 
					glVertexPointer(2, GL_FLOAT, 0, rawVertices);
					glDrawArrays(GL_TRIANGLES, 0, 24); //6); // 6 is number of points in verts.

					glTranslatef(x * -1, y * -1, 0);
					glPopMatrix(); 

					RendererStats::s_tris += 8;

				#else 
					float verts[] = {
						0, 0,//tl
						width, 0,// tr
						width, height,// br 
						0, height//lr
					};
					
					RendererState::start(RendererState::GEOMETRY);
					glPushMatrix();
					glTranslatef(x, y, 0);

					glVertexPointer(2, GL_FLOAT, 0, verts);
					glDrawArrays(GL_LINE_LOOP, 0, 4);

					glTranslatef(x * -1, y * -1, 0);
					glPopMatrix();

					RendererStats::s_lines += 4;
				#endif
				//glDisableClientState(GL_VERTEX_ARRAY); 
				//glEnable(GL_TEXTURE_2D);
			/*#else
				glBegin(GL_LINE_LOOP);
					glVertex2i(x, y);
					glVertex2i(x, y + height);
					glVertex2i(x + width, y + height);
					glVertex2i(x + width, y);
				glEnd();
			#endif*/
		}
		//! @todo: drawRects is broken.
		// rects must be [[x1,y1,x2,y2,x3,y3,x4,y4], etc].
		// colors must be [[r,g,b,a, r,g,b,a, r,g,b,a, r,g,b,a], etc], or NULL
		void Renderer::drawRects(float rects[], int colors[]) const {
			return;

			int total = (sizeof(&rects) / sizeof(int))/8;
			
			//glDisable(GL_TEXTURE_2D);
			//glEnableClientState(GL_VERTEX_ARRAY);
			RendererState::start(RendererState::GEOMETRY);
			
			if (colors != NULL) {
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(4, GL_UNSIGNED_INT, 0, &colors);
			}

			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
				glVertexPointer(2, GL_FIXED, 0, rects);
			#else
				glVertexPointer(2, GL_INT, 0, rects);
			#endif

			for(int i = 0; i < total; i++) {
				glDrawArrays(GL_LINE_LOOP, i*4, 4);
			}

			//glDisableClientState(GL_VERTEX_ARRAY);
			//glEnable(GL_TEXTURE_2D);
		}
		void Renderer::fillGradientRect(float x, float y, float width, float height, Color* top, Color* bottom) const {
			RendererState::start(RendererState::GEOMETRY);

			glPushMatrix();
			glTranslatef(x, y, 0);
			glEnableClientState(GL_COLOR_ARRAY);

			#if defined(ARK2D_FLASCC) 

				char rawColors[] = {
					(char) top->getRed(), (char) top->getGreen(), (char) top->getBlue(), (char) top->getAlpha(), // tl 
					(char) top->getRed(), (char) top->getGreen(), (char) top->getBlue(), (char) top->getAlpha(),
					(char) bottom->getRed(), (char) bottom->getGreen(), (char) bottom->getBlue(), (char) bottom->getAlpha(),
					(char) bottom->getRed(), (char) bottom->getGreen(), (char) bottom->getBlue(), (char) bottom->getAlpha(),
					(char) top->getRed(), (char) top->getGreen(), (char) top->getBlue(), (char) top->getAlpha(),
					(char) bottom->getRed(), (char) bottom->getGreen(), (char) bottom->getBlue(), (char) bottom->getAlpha()
				};

				float rawVertices[] = {
					0.0f,			0.0f,		// tl
					(float) width,	0.0f,		// tr
					0.0f,			(float)height,	  	// bl
					0.0f,			(float)height,	  	// bl
					(float) width,	0.0f,		// tr
					(float) width,	(float)height		// br
				};

				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glDrawArrays(GL_TRIANGLES, 0, 6);

			#else

				float rawVertices[] = {
					0.0f,			0.0f,		  // tl
					(float) width,	0.0f,			// tr
					0.0f,			(float)height,	  // bl
					(float) width,	(float)height	// br
				};

				float rawColors[] = {
					top->getRedf(), top->getGreenf(), top->getBluef(), top->getAlphaf(),	// tl
					top->getRedf(), top->getGreenf(), top->getBluef(), top->getAlphaf(),	// tr
					bottom->getRedf(), bottom->getGreenf(), bottom->getBluef(), bottom->getAlphaf(),	// bl
					bottom->getRedf(), bottom->getGreenf(), bottom->getBluef(), bottom->getAlphaf()		// br
				};

				glColorPointer(4, GL_FLOAT, 0, rawColors);
				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			#endif

			glDisableClientState(GL_COLOR_ARRAY);
			glTranslatef(x * -1, y * -1, 0);
			glPopMatrix();	

			RendererStats::s_tris += 2;

		}
		void Renderer::fillRect(float x, float y, int width, int height) const {
			//#if defined(ARK2D_ANDROID)
				//glDisable(GL_TEXTURE_2D);
				//glEnableClientState(GL_VERTEX_ARRAY);


			#if defined(ARK2D_FLASCC)
				RendererState::start(RendererState::GEOMETRY);

				/*char rawColors[] = {
					255,255,255,255,
					255,255,255,255,
					255,255,255,255,
					255,255,255,255
				};*/

				float rawVertices[] = {
					0.0f,			0.0f,		// tl
					(float) width,	0.0f,		// tr
					0.0f,			(float)height,	  	// bl
					0.0f,			(float)height,	  	// bl
					(float) width,	0.0f,		// tr
					(float) width,	(float)height		// br
				};
				/*float rawVertices[] = {
					x,					y,		// tl
					x + (float) width,	y,		// tr
					x,					y + (float)height,	  	// bl
					x,					y + (float)height,	  	// bl
					x + (float) width,	y,		// tr
					x + (float) width,	y + (float)height		// br
				};*/
				glPushMatrix();
				glTranslatef(x, y, 0);
				//glEnableClientState(GL_COLOR_ARRAY);
			 
				//glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				//glDisableClientState(GL_COLOR_ARRAY);
				glTranslatef(x * -1, y * -1, 0);
				glPopMatrix();

				RendererStats::s_tris += 2;
			#else
				RendererState::start(RendererState::GEOMETRY);

				float rawVertices[] = {
					0.0f,			0.0f,		  // tl
					(float) width,	0.0f,			// tr
					0.0f,			(float)height,	  // bl
					(float) width,	(float)height	// br
				};
				glPushMatrix();
				glTranslatef(x, y, 0);

				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				glTranslatef(x * -1, y * -1, 0);
				glPopMatrix();

				RendererStats::s_tris += 2;
			#endif

				//glDisableClientState(GL_VERTEX_ARRAY);
				//glEnable(GL_TEXTURE_2D);
			/*#else
				glRecti(x, y, x + width, y + height);
			#endif*/
		}

		void Renderer::texturedRect(unsigned int texId, float x, float y, int width, int height) const {
			RendererState::start(RendererState::TEXTURE, texId);

			float rawVertices[] = {
				0.0f,			0.0f,		  // tl
				(float) width,	0.0f,			// tr
				0.0f,			(float)height,	  // bl
				(float) width,	(float)height	// br
			};
			float rawTextureCoords[] = {
				0.0f,	0.0f,	// tl
				1.0f,	0.0f,	// tr
				0.0f,	1.0f,	// bl
				1.0f,	1.0f	// br
			};
			glPushMatrix();
			glTranslatef(x, y, 0);

			glVertexPointer(2, GL_FLOAT, 0, rawVertices);
			glTexCoordPointer(2, GL_FLOAT, 0, rawTextureCoords);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glTranslatef(x * -1, y * -1, 0);
			glPopMatrix();

			RendererStats::s_tris += 2;
		}

		// order: anti-clockwise. 
		void Renderer::fillQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) const 
		{
			RendererState::start(RendererState::GEOMETRY);

				float rawVertices[] = {
					x1,		y1,		// tl
					x2,		y2,		// tr
					x3,		y3,	  	// bl
					x3,		y3,	  	// bl
					x4,		y4,		// tr
					x1, 	y1		// br
				};
				//glPushMatrix();
				//glTranslatef(x1, y1, 0);

				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				RendererStats::s_tris += 2;

				//glTranslatef(x1 * -1, y1 * -1, 0);
				//glPopMatrix();
		}

		void Renderer::texturedQuad(unsigned int texId, 
			float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, 
			float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float tx4, float ty4) 
		{
			RendererState::start(RendererState::TEXTURE, texId);

			float rawVertices[] = {
				x1,		y1,		// tl
				x2,		y2,		// tr
				x3,		y3,	  	// bl
				x3,		y3,	  	// bl
				x4,		y4,		// tr
				x1, 	y1		// br
			};
			
			float rawTextureCoords[] = {
				tx1,	ty1,	// tl
				tx2,	ty2,	// tr
				tx3,	ty3,	// bl
				tx3,	ty3,	// bl
				tx4,	ty4,	// tr
				tx1, 	ty1		// br
			};
			
			glVertexPointer(2, GL_FLOAT, 0, rawVertices);
			glTexCoordPointer(2, GL_FLOAT, 0, rawTextureCoords);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			RendererStats::s_tris += 2;

		}

		void Renderer::fillTriangle(int x, int y, int width, int height) const {
			RendererState::start(RendererState::GEOMETRY);

			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
			#else
				glBegin(GL_TRIANGLES);
					glVertex2i(x + (width/2), y);
					glVertex2i(x, y + height);
					glVertex2i(x + width, y + height);
				glEnd();

				RendererStats::s_tris += 1;
			#endif 
		}
		
		void Renderer::fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3) const {
			RendererState::start(RendererState::GEOMETRY);
			
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
			#else
				glBegin(GL_TRIANGLES);
					glVertex2f(x1, y1);
					glVertex2f(x2, y2);
					glVertex2f(x3, y3);
				glEnd();

				RendererStats::s_tris += 1;
			#endif 
		}

		void Renderer::drawPoint(int x, int y) const {
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
			#else
				glEnable(GL_POINT_SMOOTH);
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
				glDisable(GL_POINT_SMOOTH);
			#endif
		}
		void Renderer::drawPoint(float x, float y) const {
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
			#else
				glEnable(GL_POINT_SMOOTH);
				glBegin(GL_POINTS);
				glVertex2f(x, y);
				glEnd();
				glDisable(GL_POINT_SMOOTH);
			#endif
		}

		void Renderer::drawCircle(ARK::Geometry::Circle<int>* circle) const {
			drawCircle(circle->getCenterX(), circle->getCenterY(), (int) circle->getRadius(), DEFAULT_SEGMENTS);
		}
		void Renderer::drawCircle(ARK::Geometry::Circle<float>* circle) const {
			drawCircle(circle->getCenterX(), circle->getCenterY(), (int) circle->getRadius(), DEFAULT_SEGMENTS);
		}
		void Renderer::drawCircle(float x, float y, int radius) const {
			drawCircle(x, y, radius, DEFAULT_SEGMENTS);
		}
		void Renderer::drawCircle(float x, float y, int radius, int points) const {
			//#if defined(ARK2D_ANDROID)
				//ARK2D::getLog()->i("drawCircle - preparation");

				float each = 360.0f / float(points);
				float verts[(points+1)*2];
				int j = 0;
				for(float i = 0; i <= 360; i += each) { 
					double angle = 2 * MY_PI * i / 360;
					verts[j] = float(0 + cos(angle) * radius);
					verts[j+1] = float(0 + sin(angle) * radius);
					j+=2;
				}

				//ARK2D::getLog()->i("drawCircle - render");
				//glDisable(GL_TEXTURE_2D);
				//glEnableClientState(GL_VERTEX_ARRAY);
				RendererState::start(RendererState::GEOMETRY);
				glPushMatrix();
				glTranslatef(x, y, 0);

				glVertexPointer(2, GL_FLOAT, 0, verts);
				glDrawArrays(GL_LINE_LOOP, 0, points+1);

				glTranslatef(x * -1, y * -1, 0);
				glPopMatrix();

				RendererStats::s_lines += points;

				//glDisableClientState(GL_VERTEX_ARRAY);
				//glEnable(GL_TEXTURE_2D);
			

			/*#else
				float each = 360.0f / float(points);
				glBegin(GL_LINE_LOOP);
				for(float i = 0; i <= 360; i += each){
					double angle = 2 * MY_PI * i / 360;
					glVertex2d(x + cos(angle) * radius, y + sin(angle) * radius);
				}
				glEnd();
			#endif*/
		}

		void Renderer::fillCircle(float x, float y, int radius, int points) const {

			//ARK2D::getLog()->i("fillCircle - preparation");

			float each = 360.0f / float(points);
			float verts[(points+3)*2];
			verts[0] = 0;
			verts[1] = 0; 
			int j = 2;
			for(float i = 0; i <= 360; i += each) {
				double angle = 2 * MY_PI * i / 360;
				verts[j] = float(0 + cos(angle) * radius);
				verts[j+1] = float(0 + sin(angle) * radius);
				j+=2;
			}
			verts[j] = 0;
			verts[j+1] = 0;
			//glDisable(GL_TEXTURE_2D);
			//glEnableClientState(GL_VERTEX_ARRAY);

			//ARK2D::getLog()->i("fillCircle - render");

			RendererState::start(RendererState::GEOMETRY);
			glPushMatrix();
			glTranslatef(x, y, 0);

			glVertexPointer(2, GL_FLOAT, 0, verts);
			glDrawArrays(GL_TRIANGLE_FAN, 0, points+2);

			glTranslatef(x * -1, y * -1, 0);
			glPopMatrix();

			RendererStats::s_tris += points;

			//glDisableClientState(GL_VERTEX_ARRAY);
			//glEnable(GL_TEXTURE_2D);

			/*#if defined(ARK2D_ANDROID)

			#else
				float each = 360.0f / float(points);
				glBegin(GL_TRIANGLE_FAN);
				glVertex2i(x, y);
				for(float i = 0; i <= 360; i += each){
					double angle = 2 * MY_PI * i / 360;
					glVertex2d(x + cos(angle) * radius, y + sin(angle) * radius);
				}
				glEnd();
			#endif*/
		}

		void Renderer::fillEllipse(float x, float y, float rx, float ry) const 
		{
			int points = 30; 

			float each = 360.0f / float(points);
			float verts[(points+3)*2];
			verts[0] = 0;
			verts[1] = 0; 
			int j = 2;
			for(float i = 0; i <= 360; i += each) {
				double angle = 2 * MY_PI * i / 360;
				verts[j] = float(0 + cos(angle) * rx);
				verts[j+1] = float(0 + sin(angle) * ry);
				j+=2;
			}
			verts[j] = 0;
			verts[j+1] = 0;
			
			RendererState::start(RendererState::GEOMETRY);
			glPushMatrix();
			glTranslatef(x, y, 0);

			glVertexPointer(2, GL_FLOAT, 0, verts);
			glDrawArrays(GL_TRIANGLE_FAN, 0, points+2);

			glTranslatef(x * -1, y * -1, 0);
			glPopMatrix();

			RendererStats::s_tris += points;
		}

		void Renderer::drawCircleSpikey(float x, float y, int radius, int points) const {
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))

			#else
				float each = 360.0f / float(points);
				glBegin(GL_LINE_LOOP);
				for(float angle = 0; angle < 360; angle += each) {
					glVertex2f(x + sin(angle) * radius, y + cos(angle) * radius);
				}
				glEnd();

				RendererStats::s_lines += points;
			#endif
		}
		void Renderer::fillCircleSpikey(float x, float y, int radius, int points) const {
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
			#else
				float each = 360.0f / float(points);
				glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x, y);
				for (float angle = 0; angle < 360; angle += each) {
					glVertex2f(x + sin(angle) * radius, y + cos(angle) * radius);
				}
				glEnd();

				RendererStats::s_tris += points;
			#endif
		}

		void Renderer::setDrawColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
			m_DrawColor.setRed(r);
			m_DrawColor.setGreen(g);
			m_DrawColor.setBlue(b);
			m_DrawColor.setAlpha(a); 

			glColor4f(m_DrawColor.getRed()/255.f, m_DrawColor.getGreen()/255.f, m_DrawColor.getBlue()/255.f, m_DrawColor.getAlpha()/255.f);
		}
		void Renderer::setDrawColorf(float r, float g, float b, float a) {
			m_DrawColor.setRed((float) r);
			m_DrawColor.setGreen((float) g); 
			m_DrawColor.setBlue((float) b);
			m_DrawColor.setAlpha((float) a);
			glColor4f(r, g, b, a);
		}
		void Renderer::setDrawColor(const Color& c) {
			m_DrawColor = c;
			c.bind();
			//glColor4b(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
			//glColor4f(c.getRed()/255.f, c.getGreen()/255.f, c.getBlue()/255.f, c.getAlpha()/255.f);
		}
		void Renderer::setDrawColor(Color* c) {
			setDrawColor(c->getRed(), c->getGreen(), c->getBlue(), c->getAlpha());
		}
		const Color& Renderer::getDrawColor() const {
			return m_DrawColor;
		}

		void Renderer::setMaskColor(const Color& c) {
			m_MaskColor = c;
		}
		const Color& Renderer::getMaskColor() const {
			return m_MaskColor;
		}

		void Renderer::setPointSize(float f) {
			glPointSize(f);
		}
		float Renderer::getPointSize() {
			return m_pointSize;
		} 
		void Renderer::setLineWidth(unsigned int i) {
			glLineWidth(i);
			m_LineWidth = i;
		}
		unsigned int Renderer::getLineWidth() {
			return m_LineWidth;
		}

		void Renderer::setBlendMode(unsigned int blendMode) {
			if (m_blendMode == blendMode) { return; }

			if (blendMode == BLEND_NONE) {
				glDisable(GL_BLEND);
			} else {
				if (m_blendMode == BLEND_NONE) { // it was disabled, enable it.
					glEnable(GL_BLEND);	
				}
				
				if (blendMode == BLEND_NORMAL) {
					glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				} else if (blendMode == BLEND_ADDITIVE) {
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				} else if (blendMode == BLEND_TEST) {
					//glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
					glBlendFunc(GL_DST_COLOR, GL_ZERO);
					//glBlendFunc(GL_ZERO, GL_SRC_COLOR);
					//glDepthFunc( GL_LEQUAL);
					//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
				}
			}
			m_blendMode = blendMode;
		}
		unsigned int Renderer::getBlendMode() {
			return m_blendMode;
		}

		void Renderer::setScissorBoxColors(const Color& top, const Color& left, const Color& bottom, const Color& right) {
			m_ScissorBoxColors[0] = top;
			m_ScissorBoxColors[1] = left;
			m_ScissorBoxColors[2] = bottom;
			m_ScissorBoxColors[3] = right;
		}
		void Renderer::drawScissorBoxes() {
			GameContainer* container = ARK2D::getContainer();
			// draw scissor boxes because glScissor does not work on some HTC phones.
			//Color cc = container->getClearColor();
			//setDrawColor(cc.getRed(), cc.getGreen(), cc.getBlue(), cc.getAlpha());

			// left edge
			setDrawColor(m_ScissorBoxColors[1].getRed(), m_ScissorBoxColors[1].getGreen(), m_ScissorBoxColors[1].getBlue(), m_ScissorBoxColors[1].getAlpha());
			fillRect(0,0, (int) container->getTranslateX(), container->getDynamicHeight());// * container->getScaleY());

			// right edge
			setDrawColor(m_ScissorBoxColors[3].getRed(), m_ScissorBoxColors[3].getGreen(), m_ScissorBoxColors[3].getBlue(), m_ScissorBoxColors[3].getAlpha());
			fillRect(container->getTranslateX() + container->getWidth() * container->getScaleX(), 0,
						(int) container->getTranslateX(), container->getDynamicHeight());// * container->getScaleY());
 
			// top edge
			setDrawColor(m_ScissorBoxColors[0].getRed(), m_ScissorBoxColors[0].getGreen(), m_ScissorBoxColors[0].getBlue(), m_ScissorBoxColors[0].getAlpha());
			fillRect(0,0, container->getDynamicWidth(), (int) container->getTranslateY());

			// bottom edge
			setDrawColor(m_ScissorBoxColors[2].getRed(), m_ScissorBoxColors[2].getGreen(), m_ScissorBoxColors[2].getBlue(), m_ScissorBoxColors[2].getAlpha());
			fillRect(0, container->getTranslateY() + container->getHeight() * container->getScaleY(),
						container->getDynamicWidth(), (int) container->getTranslateY());
						//container->getWidth() * container->getScaleX(), container->getTranslateY());

			setDrawColor(Color::white);
		}

		void Renderer::pushMatrix() const {
			glPushMatrix();
		}
		void Renderer::popMatrix() const {
			glPopMatrix();
		}

		void Renderer::loadMatrix(Matrix44<float>* m) const {
			glLoadMatrixf(&m->m_x.m_x);
		}
		void Renderer::loadMatrix(const Matrix44<float>& m) const {
			glLoadMatrixf(&m.m_x.m_x);
		}
		void Renderer::resetMatrix() const {
			glLoadIdentity();
		}
	}
}
