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

#ifndef ARK_CORE_GEOMETRY_POLYGON_H_
#define ARK_CORE_GEOMETRY_POLYGON_H_

#include "Shape.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Line.h"

namespace ARK {
	namespace Core {
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
					std::vector<Vector2<T> > m_points;

				public:
					Polygon(): Shape<T>(), m_points() {
						m_points.reserve(3);
					}
                    Polygon(ARK::Core::Geometry::Rectangle* rect):
						Shape<T>(),
						m_points()
						{
						m_points.reserve(4);
						addPoint(rect->getMinX(), rect->getMinY());
						addPoint(rect->getMaxX(), rect->getMinY());
						addPoint(rect->getMaxX(), rect->getMaxY());
						addPoint(rect->getMinX(), rect->getMaxY());


					}
					Polygon(Vector2<T>* coords[]): Shape<T>(), m_points() {
						int l = sizeof(coords) / sizeof(coords[0]);
						m_points.reserve(l);
						for(unsigned int i = 0; i < l; i++) {
							addPoint(coords[i]->getX(), coords[i]->getY());
							//m_points.push_back(coords[i]);
						}
					}
					Polygon(unsigned int numPoints): Shape<T>() {
						for(unsigned int i = 0; i < numPoints; i++) {
							addPoint(0, 0);
						}
					}

					virtual void addPoint(T x, T y) {
						m_points.push_back(Vector2<T>(x, y));
					}
					virtual bool addPointIfNotExists(T x, T y) {
						for(unsigned int i = 0; i < m_points.size(); ++i) {
							if (m_points[i].getX() == x && m_points[i].getY() == y) {
								return false;
							}
						}
						addPoint(x, y);
						return true;
					}
					virtual vector<Vector2<T> >* getPoints() {
						return &m_points;
					}
					virtual Vector2<T>* getPoint(unsigned int i) {
						return &m_points.at(i);
					}
					virtual void setPoint(unsigned int i, T x, T y) {
						m_points.at(i).set(x, y);
					}

					virtual void add(T x, T y) {
						for(unsigned int i = 0; i < m_points.size(); i++) {
							m_points.at(i).add(x, y);
						}
					}
					virtual void rotate(float degrees) {
						T cx = getCenterX();
						T cy = getCenterY();
						Vector2<T> center(cx, cy);

						for(unsigned int i = 0; i < m_points.size(); i++) {
							Vector2<T>* p = &m_points.at(i);
							MathUtil::rotatePointAroundPoint<T>(p, &center, degrees);
						}
					}

					virtual T getMinX() {
						T min = std::numeric_limits<T>::max();
						for(unsigned int i = 0; i < m_points.size(); i++) {
							if (m_points.at(i).getX() < min) {
								min = m_points.at(i).getX();
							}
						}
						return min;
					}
					virtual T getMaxX() {
						T max = std::numeric_limits<T>::min();
						for(unsigned int i = 0; i < m_points.size(); i++) {
							if (m_points.at(i).getX() > max) {
								max = m_points.at(i).getX();
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
							if (m_points.at(i).getY() < min) {
								min = m_points.at(i).getY();
							}
						}
						return min;
					}
					virtual T getMaxY() {
						T max = std::numeric_limits<T>::min();
						for(unsigned int i = 0; i < m_points.size(); i++) {
							if (m_points.at(i).getY() > max) {
								max = m_points.at(i).getY();
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
							m_points.at(i).add(dx, dy);
						}
					}
					virtual void setLocationByCenter(T x, T y) {
						T dx = x - getCenterX();
						T dy = y - getCenterY();

						for(unsigned int i = 0; i < m_points.size(); i++) {
							m_points.at(i).add(dx, dy);
						}
					}
					virtual void setLocationByAlignment(T x, T y, signed int alignX, signed int alignY) {
						//ARK2D::getLog()->w("Polygon::setLocationByAlignment not implemented");
					}
					virtual void setXByCenter(T x) {
						// TODO: improve this.
						setLocationByCenter(x, getCenterY());
					}
					virtual void setYByCenter(T y) {
						// TODO: improve this.
						setLocationByCenter(getCenterX(), y);
					}

					virtual bool contains(T x, T y) {
						return false;
					}
					virtual bool collides(Shape<T>* s) {
	                    if (s == NULL) {
                            //ARK2D::getLog()->e("A Shape was NULL");
                            return false;
                        }

						Polygon<T>* polygon = NULL;
						polygon = dynamic_cast<Polygon<T>* >(s);
						if (polygon != NULL) {
							return Shape<T>::collision_polygonPolygon(this, polygon);
						}

						RectangleTemplate<T>* rect = NULL;
						rect = dynamic_cast<RectangleTemplate<T>* >(s);
						if (rect != NULL) {
							return Shape<T>::collision_polygonRectangle(this, rect);
						}

						Line<T>* line = NULL;
						line = dynamic_cast<Line<T>* >(s);
						if (line != NULL) {
							return Shape<T>::collision_polygonLine(this, line->getStart()->getX(), line->getStart()->getY(), line->getEnd()->getX(), line->getEnd()->getY());
						}

						Circle<T>* circle = NULL;
						circle = dynamic_cast<Circle<T>* >(s);
						if (circle != NULL) {
							return Shape<T>::collision_polygonCircle(this, circle->getCenterX(), circle->getCenterY(), circle->getRadius());
						}

						return false;
					}
					bool isConvex() { //
						for (unsigned int i = 0; i < m_points.size(); i++) {
							if (!MathUtil::isVertexConvex(&m_points, i)) {
								return false;
							}
						}
						return true;
					}
					virtual void resolve(Shape<T>* s) {

					}
					virtual void render() {
						if (m_points.size() < 3) { return; }

						Renderer* g = ARK2D::getRenderer();
						for(unsigned int i = 0; i < m_points.size(); i++) {
							unsigned int nextIndex = i+1;
							if (nextIndex == m_points.size()) { nextIndex = 0; }
							g->drawLine(m_points.at(i).getX(), m_points.at(i).getY(), m_points.at(nextIndex).getX(), m_points.at(nextIndex).getY());
						}
					}

					virtual ~Polygon() {
						//for(unsigned int i = 0; i < m_points.size(); i++) {
						//	delete m_points.at(i);
						//}
						m_points.clear();
					}
			};
            
		}
	}
}
#endif /* POLYGON_H_ */
