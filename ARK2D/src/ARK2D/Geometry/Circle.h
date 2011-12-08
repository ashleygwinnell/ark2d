/*
 * Circle.h
 *
 *  Created on: 13 Nov 2010
 *      Author: Ashley
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "../Util/MathUtil.h"

class Circle {
	public:
		Circle();
		Circle(float x, float y, float r);

		float m_x;
		float m_y;
		float m_radius;

		float getX();
		float getY();
		float getRadius();

		void setX(float f);
		void setY(float f);
		void setRadius(float f);

		static bool s_collides(Circle* c1, Circle* c2);
		static bool s_collides(float x1, float y1, float r1, float x2, float y2, float r2);

		virtual ~Circle();
};

#endif /* CIRCLE_H_ */
