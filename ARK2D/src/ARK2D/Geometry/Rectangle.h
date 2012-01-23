/*
 * Rectangle.h
 *
 *  Created on: Jan 22, 2012
 *      Author: ashleygwinnell
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "Shape.h"
#include "Vector2.h"

namespace ARK {
	template <class T>
	class Rectangle: public Shape<T> {
		private:
			T m_x;
			T m_y;
			int m_width;
			int m_height;

		public:
			Rectangle():Shape<T>(), m_x(0), m_y(0), m_width(1), m_height(1) {

			}

			virtual T getMinX() {
				return m_x;
			}
			virtual T getMaxX() {
				return m_x + m_width;
			}
			virtual T getCenterX() {
				return m_x + (m_width/2);
			}

			virtual T getMinY() {
				return m_y;
			}
			virtual T getMaxY() {
				return m_y + m_height;
			}
			virtual T getCenterY() {
				return m_y + (m_height/2);
			}

			virtual T getWidth() {
				return m_width;
			}
			virtual T getHeight() {
				return m_height;
			}

			virtual void setLocation(T x, T y) {
				m_x = x;
				m_y = y;
			}
			virtual void setLocationByCenter(T x, T y) {
				m_x = x - (m_width/2);
				m_y = y - (m_height/2);
			}

			virtual bool contains(T x, T y) {
				if (x > m_x && x < getMaxX() && y > m_y && y < getMaxY()) {
					return true;
				}
				return false;
			}
			virtual bool collides(Shape<T>* s) {
				Rectangle<T>* rect = NULL;
				rect = dynamic_cast<Rectangle<T>*>(s);
				if (rect != NULL) {
					return Shape<T>::collision_rectangleRectangle(m_x, m_y, m_width, m_height, rect->getMinX(), rect->getMinY(), rect->getWidth(), rect->getHeight());
				}
				return false;
			}
			virtual void resolve(Shape<T>* s) {

			}



			virtual ~Rectangle() {

			}
	};
}
#endif /* RECTANGLE_H_ */
