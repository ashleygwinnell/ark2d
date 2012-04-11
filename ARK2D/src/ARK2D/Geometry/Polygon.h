/*
 * Polygon.h
 *
 *  Created on: Jan 22, 2012
 *      Author: ashleygwinnell
 *
 *-----------------------------------------
 * Note:
 *
 *
 *-----------------------------------------
 */

#ifndef ARKPOLYGON_H_
#define ARKPOLYGON_H_

#include "Shape.h"
namespace ARK {
	namespace Geometry {

		/*!
		 * \brief Class for Polygon construction. Polygons should be convex for accurate collisions as there is no Triangulation code.
		 *
		 * @warning On Mac there is a conflict in this class name.
		 * 			You will need to use ARK::Geometry::Polygon in all cases if compiling for Mac.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		template <class T>
		class Polygon : public Shape<T> {
			private:
				std::vector<Vector2<T>* > m_points;

			public:
				Polygon(): Shape<T>(), m_points() {

				}
				Polygon(Vector2<T>* coords[]): Shape<T>(), m_points() {
					int l = sizeof(coords) / sizeof(coords[0]);
					for(unsigned int i = 0; i < l; i++) {
						m_points.push_back(coords[i]);
					}
				}

				virtual void addPoint(T x, T y) {
					m_points.push_back(new Vector2<T>(x, y));
				}
				virtual vector<Vector2<T>* >* getPoints() {
					return &m_points;
				}

				virtual T getMinX() {
					T min = std::numeric_limits<T>::max();
					for(unsigned int i = 0; i < m_points.size(); i++) {
						if (m_points.at(i)->getX() < min) {
							min = m_points.at(i)->getX();
						}
					}
					return min;
				}
				virtual T getMaxX() {
					T max = std::numeric_limits<T>::min();
					for(unsigned int i = 0; i < m_points.size(); i++) {
						if (m_points.at(i)->getX() > max) {
							max = m_points.at(i)->getX();
						}
					}
					return max;
				}
				virtual T getCenterX() {
					return (getMinX() + getMaxX())/2;
				}

				virtual T getMinY() {
					T min = std::numeric_limits<T>::max();
					for(unsigned int i = 0; i < m_points.size(); i++) {
						if (m_points.at(i)->getY() < min) {
							min = m_points.at(i)->getY();
						}
					}
					return min;
				}
				virtual T getMaxY() {
					T max = std::numeric_limits<T>::min();
					for(unsigned int i = 0; i < m_points.size(); i++) {
						if (m_points.at(i)->getY() > max) {
							max = m_points.at(i)->getY();
						}
					}
					return max;
				}
				virtual T getCenterY() {
					return (getMinY() + getMaxY())/2;
				}

				virtual T getWidth() {
					return getMaxX() - getMinX();
				}
				virtual T getHeight() {
					return getMaxY() - getMinY();
				}

				virtual void setLocation(T x, T y) {
					T dx = x - getMinX();
					T dy = y - getMinY();

					for(unsigned int i = 0; i < m_points.size(); i++) {
						m_points.at(i)->add(dx, dy);
					}
				}
				virtual void setLocationByCenter(T x, T y) {
					T dx = x - getCenterX();
					T dy = y - getCenterY();

					for(unsigned int i = 0; i < m_points.size(); i++) {
						m_points.at(i)->add(dx, dy);
					}
				}

				virtual bool contains(T x, T y) {
					return false;
				}
				virtual bool collides(Shape<T>* s) {
					if (s == NULL) { ErrorDialog::createAndShow("A Shape was NULL"); }

					Polygon<T>* polygon = NULL;
					polygon = dynamic_cast<Polygon<T>* >(s);
					if (polygon != NULL) {
						return Shape<T>::collision_polygonPolygon(this, polygon);
					}

					Circle<T>* circle = NULL;
					circle = dynamic_cast<Circle<T>* >(s);
					if (circle != NULL) {
						return Shape<T>::collision_polygonCircle(this, circle->getCenterX(), circle->getCenterY(), circle->getRadius());
					}

					return false;
				}
				virtual void resolve(Shape<T>* s) {

				}
				virtual void render() {
					if (m_points.size() < 3) { return; }

					Renderer* g = ARK2D::getRenderer();
					for(unsigned int i = 0; i < m_points.size(); i++) {
						unsigned int nextIndex = i+1;
						if (nextIndex == m_points.size()) { nextIndex = 0; }
						g->drawLine(m_points.at(i)->getX(), m_points.at(i)->getY(), m_points.at(nextIndex)->getX(), m_points.at(nextIndex)->getY());
					}
				}

				virtual ~Polygon() {
					for(unsigned int i = 0; i < m_points.size(); i++) {
						delete m_points.at(i);
					}
				}
		};
	}
}
#endif /* POLYGON_H_ */
