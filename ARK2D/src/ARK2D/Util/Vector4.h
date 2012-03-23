/*
 * Vector4.h
 *
 *  Created on: 14 Apr 2011
 *      Author: Ashley
 */

#ifndef VECTOR4_H_
#define VECTOR4_H_

#include <math.h>
#include <cmath>
#include <stdlib.h>

namespace ARK {
	namespace Util {

		/*!
		 * \brief Useful for holding four integers e.g. for UI rectangle padding.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		template <class T=float>
		class Vector4 {
			public:
				T m_x;
				T m_y;
				T m_z;
				T m_w;

			public:
				Vector4(): m_x(0), m_y(0), m_z(0), m_w(0) {

				}
				Vector4(T coords[]): m_x(coords[0]), m_y(coords[1]), m_z(coords[2]), m_w(coords[3]) {

				}
				Vector4(T x, T y, T z, T w): m_x(x), m_y(y), m_z(z), m_w(w) {

				}

				T getX() { return m_x; }
				T getY() { return m_y; }
				T getZ() { return m_z; }
				T getW() { return m_w; }

				void setX(T x) { m_x = x; }
				void setY(T y) { m_y = y; }
				void setZ(T z) { m_z = z; }
				void setW(T w) { m_w = w; }

				Vector4* add(Vector4 v) {
					return add(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				Vector4* add(T x, T y, T z, T w) {
					m_x += x;
					m_y += y;
					m_z += z;
					m_w += w;
					return this;
				}

				void set(Vector4 v) {
					set(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				void set(T x, T y, T z, T w) {
					m_x = x;
					m_y = y;
					m_z = z;
					m_w = w;
				}

				Vector4* subtract(Vector4 v) {
					return subtract(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				Vector4* subtract(T x, T y, T z, T w) {
					m_x -= x;
					m_y -= y;
					m_z -= z;
					m_w -= w;
					return this;
				}

				void toValue(T v) {
					m_x = v;
					m_y = v;
					m_z = v;
					m_w = v;
				}
				void toZero() {
					toValue(0);
				}
				void toOne() {
					toValue(1);
				}

				virtual ~Vector4() {

				}

		};
	}
}

#endif /* VECTOR4_H_ */
