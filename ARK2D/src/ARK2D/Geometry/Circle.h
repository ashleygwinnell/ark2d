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
	namespace Geometry {

		/*!
		 * \brief It's a Circle!
		 * It's computationally expensive to render circles because it's one more point per smoothness.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		template <class T>
		class Circle : public Shape<T> {
			private:
				T m_x;
				T m_y;
				T m_radius;
				static unsigned int RENDER_SEGMENTS;
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

				virtual void setX(T x) {
					m_x = x + m_radius;
				}
				virtual void setY(T y) {
					m_y = y + m_radius;
				}

				virtual void setLocation(T x, T y) {
					m_x = x + m_radius;
					m_y = y + m_radius;
				}
				virtual void setLocationByCenter(T x, T y) {
					m_x = x;
					m_y = y;
				}

				virtual void setXByCenter(T x) {
					m_x = x;
				}
				virtual void setYByCenter(T y) {
					m_y = y;
				}

				virtual void adjustX(T x) {
					m_x += x; 
				}
				virtual void adjustY(T y) {
					m_y += y;
				}

				virtual void setLocationByAlignment(T x, T y, signed int alignX, signed int alignY) {
					ARK2D::getLog()->w("Circle::setLocationByAlignment not implemented");
				}

				virtual bool contains(T x, T y) {
					float distance = (float) MathUtil::distance(m_x, m_y, x, y);
					if (distance <= m_radius) {
						return true;
					}
					return false;
				}
				virtual bool collides(Shape<T>* s) {
					if (s == NULL) { ErrorDialog::createAndShow("A Shape was NULL"); }

					Circle<T>* circle = NULL;
					circle = dynamic_cast<Circle<T>* >(s);
					if (circle != NULL) {
						return Shape<T>::collision_circleCircle(m_x, m_y, m_radius, circle->getCenterX(), circle->getCenterY(), circle->getRadius());
					}

					Rectangle<T>* rect = NULL;
					rect = dynamic_cast<Rectangle<T>* >(s);
					if (rect != NULL) {
						return Shape<T>::collision_circleRectangle(m_x, m_y, m_radius, rect->getMinX(), rect->getMinY(), rect->getWidth(), rect->getHeight());
					}

					Line<T>* line = NULL;
					line = dynamic_cast<Line<T>* >(s);
					if (line != NULL) {
						return Shape<T>::collision_circleLine(m_x, m_y, m_radius, line->getStart()->getX(), line->getStart()->getY(), line->getEnd()->getX(), line->getEnd()->getY());
					}

					Polygon<T>* polygon = NULL;
					polygon = dynamic_cast<Polygon<T>* >(s);
					if (polygon != NULL) {
						return Shape<T>::collision_polygonCircle(polygon, m_x, m_y, m_radius);
					}

					return false;
				}
				virtual void resolve(Shape<T>* s) {

				}

				virtual void render() {
					Renderer* g = ARK2D::getRenderer();
					g->drawCircle((int)m_x, (int)m_y, (int)m_radius, RENDER_SEGMENTS);
				}

				virtual ~Circle() {

				}
		};

		
	}
}

template <class T>
unsigned int ARK::Geometry::Circle<T>::RENDER_SEGMENTS = 20;

#endif /* CIRCLE_H_ */
