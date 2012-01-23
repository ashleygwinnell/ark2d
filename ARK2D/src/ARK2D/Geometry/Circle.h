/*
 * Circle.h
 *
 *  Created on: 22 Jan 2012
 *      Author: ashleygwinnell
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "../Util/MathUtil.h"
#include "Shape.h"

namespace ARK {
	template <class T>
	class Circle : public Shape<T> {
		private:
			T m_x;
			T m_y;
			T m_radius;
		public:
			Circle(): Shape<T>(), m_x(0), m_y(0), m_radius(0) {

			}
			Circle(T x, T y, T r): Shape<T>(), m_x(x), m_y(y), m_radius(r) {

			}

			virtual T getMinX() {
				return m_x - m_radius;
			}
			virtual T getMaxX() {
				return m_x + m_radius;
			}
			virtual T getCenterX() {
				return m_x;
			}

			virtual T getMinY() {
				return m_y - m_radius;
			}
			virtual T getMaxY() {
				return m_y + m_radius;
			}
			virtual T getCenterY() {
				return m_y;
			}

			virtual T getWidth() {
				return 2 * m_radius;
			}
			virtual T getHeight() {
				return 2 * m_radius;
			}

			virtual T getRadius() {
				return m_radius;
			}
			virtual void setRadius(T radius) {
				m_radius = radius;
			}

			virtual void setLocation(T x, T y) {
				m_x = x + m_radius;
				m_y = y + m_radius;
			}
			virtual void setLocationByCenter(T x, T y) {
				m_x = x;
				m_y = y;
			}

			virtual bool contains(T x, T y) {
				float distance = (float) MathUtil::distance(m_x, m_y, x, y);
				if (distance <= m_radius) {
					return true;
				}
				return false;
			}
			virtual bool collides(Shape<T>* s) {
				Circle<T>* circle = NULL;
				circle = dynamic_cast<Circle<T>* >(s);
				if (circle != NULL) {
					return Shape<T>::collision_circleCircle(m_x, m_y, m_radius, circle->getCenterX(), circle->getCenterY(), circle->getRadius());
				}

				return false;
			}
			virtual void resolve(Shape<T>* s) {

			}

			virtual ~Circle() {

			}
	};
}
#endif /* CIRCLE_H_ */
