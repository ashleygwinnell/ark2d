/*
 * Cube.h
 *
 *  Created on: July 4, 2014
 *      Author: ashleygwinnell
 */

#ifndef ARK_GEOMETRY_CUBE_H_
#define ARK_GEOMETRY_CUBE_H_

#include "../Namespaces.h"
#include "Shape.h"
#include "../Common/DLL.h"
//#include "../Graphics/Renderer.h"

namespace ARK {
	namespace Geometry {

		/*!
		 * \brief Cube bounds class. Use this for cuboid collision/resolution.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */

		template <class T>
		class CubeTemplate : public Shape<T> {
			protected:
				T m_x;
				T m_y;
				T m_z;
				T m_width;
				T m_height;
				T m_depth;

			public:
				CubeTemplate():
					Shape<T>(),
					m_x(0),
					m_y(0),
					m_z(0),
					m_width(1),
					m_height(1),
					m_depth(1) {

                }
                CubeTemplate(T w, T h, T d):
                    Shape<T>(),
                    m_x(0),
                    m_y(0),
                    m_z(0),
                    m_width(w),
                    m_height(h),
                    m_depth(d) {

                }
				CubeTemplate(T x, T y, T z, T width, T height, T depth):
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
					m_width = width;
					m_height = height;
					m_depth = depth;
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
					return m_depth;
				}

				virtual void setX(T x) {
					m_x = x;
				}
				virtual void setY(T y) {
					m_y = y;
				}
				virtual void setZ(T z) {
					m_z = z;
				}
				virtual void setLocation(T x, T y) {
					m_x = x;
					m_y = y;
				}
				virtual void setLocation(T x, T y, T z) {
					m_x = x;
					m_y = y;
					m_z = z;
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
                //virtual void setLocationByAlignment(T x, T y, signed int alignX, signed int alignY);
                //virtual void setLocationByAlignment(T x, T y, T z, signed int alignX, signed int alignY, signed alignZ);
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
					m_depth = (int) h;
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
				virtual bool collides(T x, T y, T z) {
					return (x >= m_x && x <= getMaxX() && y >= m_y && y <= getMaxY() && z >= m_z && z <= getMaxZ());
				}

				virtual bool collides(Shape<T>* s) {
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

				virtual ~CubeTemplate() {

				}
		};



		class Cube : public CubeTemplate<float> {
			public:
				Cube():
					CubeTemplate() {

				}
				Cube(float w, float h, float d):
					CubeTemplate(w, h, d) {

				}
				Cube(float x, float y, float z, float w, float h, float d):
					CubeTemplate(x, y, z, w, h, d) {

				}
				virtual void setLocationByAlignment(float x, float y, signed int alignX, signed int alignY);
				virtual void setLocationByAlignment(float x, float y, float z, signed int alignX, signed int alignY, signed alignZ);

				virtual ARK2D_API bool collides(Shape<float>* s);
				virtual ~Cube() {

				}
		};
	}
}
#endif /* ARK_GEOMETRY_CUBE_H_ */
