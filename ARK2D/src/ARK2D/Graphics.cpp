/*
 * Graphics.cpp
 *  Created on: 16-Oct-2009
 *      Author: Ashley Gwinnell
 */

#include "Graphics.h"
#include "BMFont.h"
#include "Color.h"

#include "ARK2D_GL.h"
#include "ARK2D.h"
#include "GameContainer.h"

Graphics::Graphics():
	m_DrawColor(255, 0, 255),
	m_MaskColor()
{
	//m_DefaultFont = new BMFont("data/calibri.fnt", "data/calibri.bmp", Color::magenta);
	//m_Font = m_DefaultFont;
}

void Graphics::setFont(BMFont* f) {
	m_Font = f;
}
BMFont* Graphics::getFont() const {
	return m_Font;
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
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
}

void Graphics::fillArc(int cx, int cy, int width, int height, float startAngle, float endAngle) const {
	fillArc(cx, cy, width, height, startAngle, endAngle, DEFAULT_SEGMENTS);
}
void Graphics::fillArc(int cx, int cy, int width, int height, float startAngle, float endAngle, int segs) const {

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
	glBegin(GL_LINE_LOOP);
		glVertex2i(x, y);
		glVertex2i(x, y + height);
		glVertex2i(x + width, y + height);
		glVertex2i(x + width, y);
	glEnd();
}
void Graphics::fillRect(int x, int y, int width, int height) const {
	glRecti(x, y, x + width, y + height);
}

void Graphics::fillTriangle(int x, int y, int width, int height) const {
	glBegin(GL_TRIANGLES);
		glVertex2i(x + (width/2), y);
		glVertex2i(x, y + height);
		glVertex2i(x + width, y + height);
	glEnd();
}

void Graphics::drawPoint(int x, int y) const {
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}
void Graphics::drawPoint(float x, float y) const {
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}


void Graphics::drawCircle(int x, int y, int radius, int points) const {
	float each = 360.0f / float(points);
	glBegin(GL_LINE_LOOP);
	for(float i = 0; i <= 360; i += each){
		double angle = 2 * PI * i / 360;
		glVertex2d(x + cos(angle) * radius, y + sin(angle) * radius);
	}
	glEnd();
}

void Graphics::fillCircle(int x, int y, int radius, int points) const {
	/*float each = 360.0f / float(points);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2i(x, y);
	for (float angle = 0; angle < 360; angle += each) {
		glVertex2f(x + sin(angle) * radius, y + cos(angle) * radius);
	}
	glEnd();*/


	float each = 360.0f / float(points);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2i(x, y);
	for(float i = 0; i <= 360; i += each){
		double angle = 2 * PI * i / 360;
		glVertex2d(x + cos(angle) * radius, y + sin(angle) * radius);
	}
	glEnd();
}

void Graphics::drawCircleSpikey(int x, int y, int radius, int points) const {
	float each = 360.0f / float(points);
	glBegin(GL_LINE_LOOP);
	for(float angle = 0; angle < 360; angle += each) {
		glVertex2f(x + sin(angle) * radius, y + cos(angle) * radius);
	}
	glEnd();
}
void Graphics::fillCircleSpikey(int x, int y, int radius, int points) const {
	float each = 360.0f / float(points);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2i(x, y);
	for (float angle = 0; angle < 360; angle += each) {
		glVertex2f(x + sin(angle) * radius, y + cos(angle) * radius);
	}
	glEnd();
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
