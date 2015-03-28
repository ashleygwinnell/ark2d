/*
 * Vector3.h
 *
 *  Created on: 6th July 2014
 *      Author: Ashley
 */

#ifndef ARK_GEOMETRY_VECTOR3_H_
#define ARK_GEOMETRY_VECTOR3_H_

#include "../ARK2D.h" 
#include "../Util/Log.h"
#include "../Core/String.h"

namespace ARK {
	namespace Geometry { 

		/*!
		 * \brief A 3d point in spaaaaace.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		template <class T=float>
		class Vector3 {
			public:
				T m_x;
				T m_y;
				T m_z;

				Vector3(): m_x(0), m_y(0), m_z(0) {

				}
				Vector3(T coords[]): m_x(coords[0]), m_y(coords[1]), m_z(coords[2]) {

				}
				Vector3(T x, T y, T z): m_x(x), m_y(y), m_z(z) {

				}

				T getX() const {
					return m_x;
				}
				T getY() const {
					return m_y;
				}
				T getZ() const {
					return m_z;
				}

				// Set
				void setX(T x) {
					m_x = x;
				}
				void setY(T y) {
					m_y = y;
				}
				void setZ(T z) {
					m_z = z;
				}
				void set(Vector3<T>* v) {
					set(v->getX(), v->getY(), v->getZ());
				}
				void set(T x, T y, T z) {
					m_x = x;
					m_y = y;
					m_z = z;
				}

				void adjustX(T x) {
					m_x += x;
				}
				void adjustY(T y) {
					m_y += y;
				}
				void adjustZ(T z) {
					m_z += z;
				}
				void adjust(T x, T y, T z) {
					m_x += x;
					m_y += y;
					m_z += z;
				}

				// Add 
				Vector3* add(Vector3<T>* v) {
					return add(v->getX(), v->getY(), v->getZ());
				}
				Vector3* add(T x, T y, T z) {
					m_x += x;
					m_y += y;
					m_z += z;
					return this;
				}
				Vector3* addX(T x) {
					m_x += x;
					return this;
				}
				Vector3* addY(T y) {
					m_y += y;
					return this;
				}
				Vector3* addZ(T z) {
					m_z += z;
					return this;
				}

				// Multiply
				Vector3* multiply(Vector3<T>* v) {
					return multiply(v->getX(), v->getY(), v->getZ());
				}
				Vector3* multiply(T x, T y, T z) {
					m_x *= x;
					m_y *= y;
					return this;
				}
				void multiplyX(T x) {
					m_x *= x;
				}
				void multiplyY(T y) {
					m_y *= y;
				}
				void multiplyZ(T z) {
					m_z *= z;
				}


				// Divide
				Vector3* divide(Vector3<T>* v) {
					return divide(v->getX(), v->getY(), v->getZ());
				}
				Vector3* divide(T x, T y, T z) {
					m_x /= x;
					m_y /= y;
					m_z /= z;
					return this;
				}
				void divideX(T x) {
					m_x /= x;
				}
				void divideY(T y) {
					m_y /= y;
				}
				void divideZ(T z) {
					m_z /= z;
				}

				// Subtract
				Vector3* subtract(Vector3<T>* v) {
					return subtract(v->getX(), v->getY(), v->getZ());
				}
				Vector3* subtract(T x, T y, T z) {
					m_x -= x;
					m_y -= y;
					m_z -= z;
					return this;
				}
				Vector3* subtractX(T x) {
					m_x -= x;
					return this;
				}
				Vector3* subtractY(T y) {
					m_y -= y;
					return this;
				}
				Vector3* subtractZ(T z) {
					m_z -= z;
					return this;
				}

				// Distance
				void distance(Vector3<T>* v, Vector3<T>* result) {
					distance(v->getX(), v->getY(), v->getZ(), result);
				}
				void distance(T x, T y, T z, Vector3<T>* result) { 
					result->set(abs(m_x - x), abs(m_y - y), abs(m_z - z));
				}

				// Friction
				Vector3* friction(T x, T y, T z) {
					frictionX(x);
					frictionY(y);
					frictionZ(z);
					return this;
				}
				Vector3* frictionX(T x) {
					if (m_x < 0) {
						m_x += x;
						if (m_x > 0) { m_x = 0; }
					} else if (m_x > 0) {
						m_x -= x;
						if (m_x < 0) { m_x = 0; }
					}
					return this;
				}
				Vector3* frictionY(T y) {
					if (m_y < 0) {
						m_y += y;
						if (m_y > 0) { m_y = 0; }
					} else if (m_y > 0) {
						m_y -= y;
						if (m_y < 0) { m_y = 0; }
					}
					return this;
				}
				Vector3* frictionZ(T z) {
					if (m_z < 0) {
						m_z += z;
						if (m_z > 0) { m_z = 0; }
					} else if (m_z > 0) {
						m_z -= z; 
						if (m_z < 0) { m_z = 0; }
					}
					return this;
				}

				// Restrict 
				void restrictMax(Vector3<T>* max) {
					restrictMax(max->getX(), max->getY(), max->getZ());
				}
				void restrictMax(T x, T y, T z) {
					if (m_x > x) {
						m_x = x;
					}
					if (m_y > y) {
						m_y = y;
					} 
					if (m_z > z) {
						m_z = z;
					} 

					if (m_x < x * -1) {
						m_x = x * -1;
					}
					if (m_y < y * -1) {
						m_y = y * -1;
					}
					if (m_z < z * -1) {
						m_z = z * -1; 
					}
				}

				T dot(Vector3* v) {
					return dot(v->getX(), v->getY(), v->getZ());
				}
				T dot(T x, T y, T z) {
					return (m_x * x) + (m_y * y) + (m_z * z);
				}

				Vector3<T> cross(Vector3<T>* rhs) {
					return Vector3<T>(m_y * rhs->m_z - m_z * rhs->m_y, m_z * rhs->m_x - m_x * rhs->m_z, m_x * rhs->m_y - m_y * rhs->m_x);
				}

				T lengthSquared() const {
					return (m_x*m_x) + (m_y * m_y) + (m_z * m_z);
				}
				T length() const {
					return (T) sqrt((double) lengthSquared());
				}

				void normalise() {
					T len = length();
					m_x /= len;
					m_y /= len;
					m_z /= len;
				}

				Vector3<T> operator*=(const Vector3<T>& other) { 
					m_x *= other.m_x;
					m_y *= other.m_y;
					m_z *= other.m_z;
					return *this; 
				}
				Vector3<T> operator*=(float other) { 
					m_x *= other;
					m_y *= other;
					m_z *= other;
					return *this; 
				}

				Vector3<T> operator+=(const Vector3<T>& other) { 
					add(other.m_x, other.m_y, other.m_z); 
					return *this; 
				}
				Vector3<T> operator+=(float other) { 
					add(other, other, other); 
					return *this; 
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
					s += "}";
					return s.get();
				}
				
				T& operator[](unsigned int i) {
					return *(&m_x + i);
				}

				const T& operator[](unsigned int i) const {
					return *(&m_x + i);
				}

				~Vector3() {

				}
		};
	}
}
#endif /* ARK_GEOMETRY_VECTOR3_H_ */

