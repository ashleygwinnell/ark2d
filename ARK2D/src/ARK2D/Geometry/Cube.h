/*
 * Cube.h
 *
 *  Created on: July 4, 2014
 *      Author: ashleygwinnell
 */

#ifndef ARK_GEOMETRY_CUBE_H_
#define ARK_GEOMETRY_CUBE_H_

#include "Shape.h"
#include "Line.h"
#include "Vector2.h"
  
namespace ARK { 
	namespace Geometry {

		/*!
		 * \brief Cube bounds class. Use this for cuboid collision/resolution.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
        
		template <class T>
		class Cube : public Shape<T> {
			private:
				T m_x;
				T m_y;
				T m_z;
				int m_width;
				int m_height;
				int m_depth;

			public:
				Cube():
					Shape<T>(), 
					m_x(0), 
					m_y(0), 
					m_z(0),
					m_width(1), 
					m_height(1),
					m_depth(1) {

				}
				Cube(T x, T y, T z, int width, int height, int depth):
					Shape<T>(), 
					m_x(x), 
					m_y(y), 
					m_z(z), 
					m_width(width), 
					m_height(height),
					m_depth(depth) {

				}
				virtual void set(T x, T y, T z, T width, T height, T depth) {
					m_x = x;
					m_y = y; 
					m_z = z; 
					m_width = (int) width;
					m_height = (int) height;
					m_depth = (int) depth;
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

				virtual T getMinZ() {
					return m_z;
				}
				virtual T getMaxZ() {
					return m_z + m_depth;
				}
				virtual T getCenterZ() {
					return m_z + (m_depth/2);
				}

				virtual T getWidth() {
					return m_width;
				}
				virtual T getHeight() {
					return m_height;
				}
				virtual T getDepth() {
					return m_z;
				}

				virtual void setX(T x) {
					m_x = x;
				}
				virtual void setY(T y) {
					m_y = y;
				}
				virtual void setZ(T y) {
					m_y = y;
				}
				virtual void setLocation(T x, T y) {
					m_x = x;
					m_y = y;
				}
				virtual void setLocation(T x, T y, T z) {
					m_x = x;
					m_y = y;
					m_z = y;
				}
				virtual void setLocationByCenter(T x, T y) {
					m_x = x - (m_width/2);
					m_y = y - (m_height/2);
				}
				virtual void setLocationByCenter(T x, T y, T z) {
					m_x = x - (m_width/2);
					m_y = y - (m_height/2);
					m_z = z - (m_depth/2);
				}
				virtual void setLocationByAlignment(T x, T y, signed int alignX, signed int alignY) {
					
					if (alignX == Renderer::ALIGN_CENTER) { 
						x -= m_width / 2.0f;
					} else if (alignX == Renderer::ALIGN_RIGHT || alignX == Renderer::ALIGN_END) {
						x -= m_width;
					} 

					if (alignY == Renderer::ALIGN_CENTER) { 
						y -= m_height / 2.0f;
					} else if (alignY == Renderer::ALIGN_END || alignY == Renderer::ALIGN_BOTTOM) {
						y -= m_height;
					} 
					
					m_x = x;
					m_y = y;
				}
				virtual void setLocationByAlignment(T x, T y, T z, signed int alignX, signed int alignY, signed alignZ) {
					
					if (alignX == Renderer::ALIGN_CENTER) { 
						x -= m_width / 2.0f;
					} else if (alignX == Renderer::ALIGN_RIGHT || alignX == Renderer::ALIGN_END) {
						x -= m_width;
					} 

					if (alignY == Renderer::ALIGN_CENTER) { 
						y -= m_height / 2.0f;
					} else if (alignY == Renderer::ALIGN_END || alignY == Renderer::ALIGN_BOTTOM) {
						y -= m_height;
					} 

					if (alignZ == Renderer::ALIGN_CENTER) { 
						z -= m_depth / 2.0f;
					} else if (alignY == Renderer::ALIGN_END || alignY == Renderer::ALIGN_BOTTOM) {
						z -= m_depth;
					} 
					
					m_x = x;
					m_y = y;
					m_z = z;
				}
				virtual void setXByCenter(T x) {
					m_x = x - (m_width/2);
				}
				virtual void setYByCenter(T y) {
					m_y = y - (m_height/2);
				}
				virtual void setZByCenter(T z) {
					m_z = z - (m_depth/2);
				}

				virtual void adjustX(T x) {
					m_x += x; 
				}
				virtual void adjustY(T y) {
					m_y += y;
				}
				virtual void adjustZ(T z) {
					m_z += z;
				}

				virtual void setWidth(T w) {
					m_width = (int) w;
				}
				virtual void setHeight(T h) {
					m_height = (int) h;
				}
				virtual void setDepth(T h) {
					m_height = (int) h;
				}
				virtual void setSize(T w, T h) {
					m_width = (int) w;
					m_height = (int) h;
				}
				virtual void setSize(T w, T h, T d) {
					m_width = (int) w;
					m_height = (int) h;
					m_depth = (int) d;
				}

				virtual bool contains(T x, T y) {
					return (x > m_x && x < getMaxX() && y > m_y && y < getMaxY());
				}
				virtual bool contains(T x, T y, T z) {
					return (x > m_x && x < getMaxX() && y > m_y && y < getMaxY() && z > m_z && z < getMaxZ());
				}
				
				virtual bool collides(Shape<T>* s) {
					if (s == NULL) { ErrorDialog::createAndShow("A Shape was NULL"); }

					// cube
					Cube<T>* cube = NULL;
					cube = dynamic_cast<Cube<T>*>(s);
					if (cube != NULL) {
						return Shape<T>::collision_cubeCube(m_x, m_y, m_z, m_width, m_height, m_depth, cube->getMinX(), cube->getMinY(), cube->getMinZ(), cube->getWidth(), cube->getHeight(), cube->getDepth());
					}

					// rectangle
					Rectangle<T>* rect = NULL;
					rect = dynamic_cast<Rectangle<T>*>(s);
					if (rect != NULL) {
						return Shape<T>::collision_rectangleRectangle(m_x, m_y, m_width, m_height, rect->getMinX(), rect->getMinY(), rect->getWidth(), rect->getHeight());
					}
 
                    // circle 
					Circle<T>* circle = NULL;
					circle = dynamic_cast<Circle<T>* >(s);
					if (circle != NULL) {
						return Shape<T>::collision_circleRectangle(circle->getCenterX(), circle->getCenterY(), circle->getRadius(), m_x, m_y, m_width, m_height);
					}

					// line
					Line<T>* line = NULL;
					line = dynamic_cast<Line<T>* >(s);
					if (line != NULL) {
						return Shape<T>::collision_rectangleLine(m_x, m_y, m_width, m_height, line->getStart()->getX(), line->getStart()->getY(), line->getEnd()->getX(), line->getEnd()->getY());
					}

					// Polygon
					Polygon<T>* poly = NULL;
					poly = dynamic_cast<Polygon<T>* >(s);
					if (poly != NULL) {
						return Shape<T>::collision_polygonRectangle(poly, this);
					}

					return false;
				}
				virtual void resolve(Shape<T>* s) {

				}
				virtual void render() {
					//ARK2D::getRenderer()->drawRect(this);
				}

				string toString() {
					ARK::Core::String s;
					s += "{";
					s += "\"x\":";
					s += m_x;
					s += ", \"y\":";
					s += m_y;
					s += ", \"z\":";
					s += m_z;
					s += ", \"w\":";
					s += m_width;
					s += ", \"h\":";
					s += m_height;
					s += ", \"d\":";
					s += m_depth;
					s += "}";
					return s.get();
				}

				virtual ~Cube() {

				}
		};
	}
}
#endif /* ARK_GEOMETRY_CUBE_H_ */
