/*
 * Circle.cpp
 *
 *  Created on: 13 Nov 2010
 *      Author: Ashley
 */

#include "Circle.h"

Circle::Circle(): m_x(0), m_y(0), m_radius(0) {

}
Circle::Circle(float x, float y, float r): m_x(x), m_y(y), m_radius(r) {

}

float Circle::getX() {
	return m_x;
}
float Circle::getY() {
	return m_y;
}
float Circle::getRadius() {
	return m_radius;
}

void Circle::setX(float f) {
	m_x = f;
}
void Circle::setY(float f) {
	m_y = f;
}
void Circle::setRadius(float f) {
	m_radius = f;
}

Circle::~Circle() { }




bool Circle::s_collides(Circle* c1, Circle* c2) {
	return s_collides(c1->getX(), c1->getY(), c1->getRadius(),
						c2->getX(), c2->getY(), c2->getRadius() );
}

bool Circle::s_collides(float x1, float y1, float r1, float x2, float y2, float r2) {
	float distance = (float) MathUtil::distance(x1, y1, x2, y2);
	if (distance <= r1 + r2) {
		return true;
	}
	return false;
}




