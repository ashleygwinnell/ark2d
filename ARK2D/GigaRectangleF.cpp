/*
 * Rectangle.cpp
 *
 *  Created on: 10-May-2010
 *      Author: Ashley
 */

#include "GigaRectangleF.h"

GigaRectangleF::GigaRectangleF():
	m_x(0),
	m_y(0),
	m_width(1),
	m_height(1)
{

}
GigaRectangleF::GigaRectangleF(float x, float y, int width, int height):
	m_x(x),
	m_y(y),
	m_width(width),
	m_height(height)
{

}
float GigaRectangleF::getX() const {
	return m_x;
}
float GigaRectangleF::getY() const {
	return m_y;
}
int GigaRectangleF::getWidth() const {
	return m_width;
}
int GigaRectangleF::getHeight() const {
	return m_height;
}

void GigaRectangleF::setLocationByCenter(float x, float y) {
	m_x = x - (m_width/2);
	m_y = y - (m_height/2);
}

bool GigaRectangleF::s_collides(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	if ((	x1 + w1) >= x2
			&& x1 <= (x2 + w2)
			&& (y1 + h1) >= y2
			&& y1 <= (y2 + h2)
	)  {
		return true;
	}
	return false;
}

void GigaRectangleF::setX(float x) {
	m_x = x;
}
void GigaRectangleF::setY(float y) {
	m_y = y;
}
void GigaRectangleF::setWidth(int w) {
	m_width = w;
}
void GigaRectangleF::setHeight(int h) {
	m_height = h;
}

GigaRectangleF::~GigaRectangleF() {

}
