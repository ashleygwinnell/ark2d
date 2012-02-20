/*
 * Graphics.cpp
 *  Created on: 16-Oct-2009
 *      Author: Ashley Gwinnell
 */

#include "Graphics.h"
#include "Font/Font.h"
#include "Font/BMFont.h"
#include "Font/FTFont.h"
#include "Color.h"

#include "ARK2D_GL.h"
#include "ARK2D.h"
#include "GameContainer.h"

Graphics::Graphics():
	m_DefaultFont(NULL),
	m_Font(NULL),
	m_DrawColor(255, 0, 255),
	m_MaskColor(),
	m_LineWidth(1)
{
	//m_DefaultFont = new BMFont("data/calibri.fnt", "data/calibri.bmp", Color::magenta);
	//m_Font = m_DefaultFont;
}

void Graphics::setFont(ARK::Font* f) {
	m_Font = f;
}
void Graphics::setDefaultFont(ARK::Font* f) {
	m_DefaultFont = f;
}

ARK::Font* Graphics::getFont() const {
	return m_Font;
}
ARK::Font* Graphics::getDefaultFont() const {
	return m_DefaultFont;
}

void Graphics::translate(int x, int y) const {
	glTranslatef(x,y,0);
}
void Graphics::translate(float x, float y) const {
	glTranslatef(x,y,0);
}
void Graphics::rotate(int angle) const {
	glRotatef(angle, 0, 0, 1);
}
void Graphics::scale(float x, float y) const {
	glScalef(x, y, 0);
}
void Graphics::setScissorTestEnabled(bool b) const {
	if (b) {
		glEnable(GL_SCISSOR_TEST);
	} else {
		glDisable(GL_SCISSOR_TEST);
	}
}
void Graphics::scissor(int x, int y, int w, int h) const {
	//glScissor(x, y - (signed int) ARK2D::getContainer()->getHeight(), w, h * -1);
	glScissor(x, ARK2D::getContainer()->getHeight() - (y + h), w, h);
}
void Graphics::viewport(int x, int y, int w, int h) const {
	glViewport(x, y, w, h);
}
void Graphics::ortho2d(int x, int y, int w, int h) const {
	ortho2d(x, y, w, h, -1, 1);
}
void Graphics::ortho2d(int x, int y, int w, int h, double near, double far) const {
	//glOrtho((double) x, double(x + w), double(y + h), (double) y, near, far);
	//glOrtho(0, w, h, 0, -1, 1);
	ARK2D::getContainer()->disable2D();
	ARK2D::getContainer()->enable2D();
}

void Graphics::drawString(const std::string str, int x, int y) const {
	m_Font->drawString(str, x, y);
}
void Graphics::drawStringCenteredAt(const std::string str, int x, int y) const {
	m_Font->drawStringCenteredAt(str, x, y);
}
void Graphics::drawStringWordWrap(const std::string str, int x, int y, int maxWidth, int ySpacing) const {
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

void Graphics::drawLine(int x1, int y1, int x2, int y2) const {
	#if defined(ARK2D_ANDROID)

	#else
		glBegin(GL_LINES);
			glVertex2i(x1, y1);
			glVertex2i(x2, y2);
		glEnd();
	#endif
}

void Graphics::fillArc(int cx, int cy, int width, int height, float startAngle, float endAngle) const {
	fillArc(cx, cy, width, height, startAngle, endAngle, DEFAULT_SEGMENTS);
}
void Graphics::fillArc(int cx, int cy, int width, int height, float startAngle, float endAngle, int segs) const {
	#if defined(ARK2D_ANDROID)
	#else

		int halfwidth = width/2;
		int halfheight = height/2;

		glBegin(GL_TRIANGLE_FAN);

			glVertex2i(cx, cy);

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

	#endif
}

void Graphics::fillRoundedRect(int x, int y, int width, int height, int radius) const {
	fillRoundedRect(x, y, width, height, radius, DEFAULT_SEGMENTS);
}
void Graphics::fillRoundedRect(int x, int y, int width, int height, int radius, int segs) const {
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
void Graphics::drawRect(int x, int y, int width, int height) const {
	//#if defined(ARK2D_ANDROID)
		float verts[] = {
			0, 0,//tl
			width, 0,// tr
			width, height,// br
			0, height//lr
		};
		glDisable(GL_TEXTURE_2D);
		glEnableClientState(GL_VERTEX_ARRAY);
		glPushMatrix();
		glTranslatef(x, y, 0);

		glVertexPointer(2, GL_FLOAT, 0, verts);
		glDrawArrays(GL_LINE_LOOP, 0, 4);

		glTranslatef(x * -1, y * -1, 0);
		glPopMatrix();
		glDisableClientState(GL_VERTEX_ARRAY);
		glEnable(GL_TEXTURE_2D);
	/*#else
		glBegin(GL_LINE_LOOP);
			glVertex2i(x, y);
			glVertex2i(x, y + height);
			glVertex2i(x + width, y + height);
			glVertex2i(x + width, y);
		glEnd();
	#endif*/
}
void Graphics::fillRect(int x, int y, int width, int height) const {
	//#if defined(ARK2D_ANDROID)
		glDisable(GL_TEXTURE_2D);
		glEnableClientState(GL_VERTEX_ARRAY);

		float rawVertices[] = {
			0,		0,		  // tl
			width,	0,			// tr
			0,		height,	  // bl
			width,	height	// br
		};
		glPushMatrix();
		glTranslatef(x, y, 0);

		glVertexPointer(2, GL_FLOAT, 0, rawVertices);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glTranslatef(x * -1, y * -1, 0);
		glPopMatrix();

		glDisableClientState(GL_VERTEX_ARRAY);
		glEnable(GL_TEXTURE_2D);
	/*#else
		glRecti(x, y, x + width, y + height);
	#endif*/
}

void Graphics::fillTriangle(int x, int y, int width, int height) const {
	#if defined(ARK2D_ANDROID)
	#else
		glBegin(GL_TRIANGLES);
			glVertex2i(x + (width/2), y);
			glVertex2i(x, y + height);
			glVertex2i(x + width, y + height);
		glEnd();
	#endif
}

void Graphics::drawPoint(int x, int y) const {
	#if defined(ARK2D_ANDROID)
	#else
		glBegin(GL_POINTS);
		glVertex2i(x, y);
		glEnd();
	#endif
}
void Graphics::drawPoint(float x, float y) const {
	#if defined(ARK2D_ANDROID)
	#else
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();
	#endif
}


void Graphics::drawCircle(int x, int y, int radius, int points) const {
	//#if defined(ARK2D_ANDROID)
		float each = 360.0f / float(points);
		float verts[points*2];
		int j = 0;
		for(float i = 0; i <= 360; i += each) {
			double angle = 2 * PI * i / 360;
			verts[j] = float(0 + cos(angle) * radius);
			verts[j+1] = float(0 + sin(angle) * radius);
			j+=2;
		}
		glDisable(GL_TEXTURE_2D);
		glEnableClientState(GL_VERTEX_ARRAY);
		glPushMatrix();
		glTranslatef(x, y, 0);

		glVertexPointer(2, GL_FLOAT, 0, verts);
		glDrawArrays(GL_LINE_LOOP, 0, points);

		glTranslatef(x * -1, y * -1, 0);
		glPopMatrix();
		glDisableClientState(GL_VERTEX_ARRAY);
		glEnable(GL_TEXTURE_2D);
	/*#else
		float each = 360.0f / float(points);
		glBegin(GL_LINE_LOOP);
		for(float i = 0; i <= 360; i += each){
			double angle = 2 * PI * i / 360;
			glVertex2d(x + cos(angle) * radius, y + sin(angle) * radius);
		}
		glEnd();
	#endif*/
}

void Graphics::fillCircle(int x, int y, int radius, int points) const {

	float each = 360.0f / float(points);
	float verts[(points+2)*2];
	verts[0] = 0;
	verts[1] = 0;
	int j = 2;
	for(float i = 0; i <= 360; i += each) {
		double angle = 2 * PI * i / 360;
		verts[j] = float(0 + cos(angle) * radius);
		verts[j+1] = float(0 + sin(angle) * radius);
		j+=2;
	}
	verts[j] = 0;
	verts[j+1] = 0;
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glPushMatrix();
	glTranslatef(x, y, 0);

	glVertexPointer(2, GL_FLOAT, 0, verts);
	glDrawArrays(GL_TRIANGLE_FAN, 0, points+2);

	glTranslatef(x * -1, y * -1, 0);
	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_2D);

	/*#if defined(ARK2D_ANDROID)

	#else
		float each = 360.0f / float(points);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2i(x, y);
		for(float i = 0; i <= 360; i += each){
			double angle = 2 * PI * i / 360;
			glVertex2d(x + cos(angle) * radius, y + sin(angle) * radius);
		}
		glEnd();
	#endif*/
}

void Graphics::drawCircleSpikey(int x, int y, int radius, int points) const {
	#if defined(ARK2D_ANDROID)

	#else
		float each = 360.0f / float(points);
		glBegin(GL_LINE_LOOP);
		for(float angle = 0; angle < 360; angle += each) {
			glVertex2f(x + sin(angle) * radius, y + cos(angle) * radius);
		}
		glEnd();
	#endif
}
void Graphics::fillCircleSpikey(int x, int y, int radius, int points) const {
	#if defined(ARK2D_ANDROID)
	#else
		float each = 360.0f / float(points);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2i(x, y);
		for (float angle = 0; angle < 360; angle += each) {
			glVertex2f(x + sin(angle) * radius, y + cos(angle) * radius);
		}
		glEnd();
	#endif
}

void Graphics::setDrawColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
	m_DrawColor.setRed(r);
	m_DrawColor.setGreen(g);
	m_DrawColor.setBlue(b);
	m_DrawColor.setAlpha(a);

	glColor4f(m_DrawColor.getRed()/255.f, m_DrawColor.getGreen()/255.f, m_DrawColor.getBlue()/255.f, m_DrawColor.getAlpha()/255.f);
}
void Graphics::setDrawColorf(float r, float g, float b, float a) {
	m_DrawColor.setRed(int(r * 255));
	m_DrawColor.setGreen(int(g * 255));
	m_DrawColor.setBlue(int(b * 255));
	m_DrawColor.setAlpha(int(a * 255));
	glColor4f(r, g, b, a);
}
void Graphics::setDrawColor(const Color& c) {
	m_DrawColor = c;
	c.bind();
	//glColor4b(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
	//glColor4f(c.getRed()/255.f, c.getGreen()/255.f, c.getBlue()/255.f, c.getAlpha()/255.f);
}
const Color& Graphics::getDrawColor() const {
	return m_DrawColor;
}

void Graphics::setMaskColor(const Color& c) {
	m_MaskColor = c;
}
const Color& Graphics::getMaskColor() const {
	return m_MaskColor;
}

void Graphics::setLineWidth(unsigned int i) {
	glLineWidth(i);
	m_LineWidth = i;
}
unsigned int Graphics::getLineWidth() {
	return m_LineWidth;
}

void Graphics::pushMatrix() const {
	glPushMatrix();
}
void Graphics::popMatrix() const {
	glPopMatrix();
}

void Graphics::loadMatrix(Matrix44<float>* m) const {
	glLoadMatrixf(&m->m_x.m_x);
}
void Graphics::loadMatrix(const Matrix44<float>& m) const {
	glLoadMatrixf(&m.m_x.m_x);
}
void Graphics::resetMatrix() const {
	glLoadIdentity();
}
