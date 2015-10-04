/*
 * Vector2.h
 *
 *  Created on: 12-Dec-2009
 *      Author: Ashley
 */

#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <math.h>
#include <cmath>
#include <stdlib.h>
#include "../ARK2D.h"
#include <string>
#include "../Core/String.h"
//#include "../Util/Log.h"
//#include "../Util/MathUtil.h"

using namespace std;

namespace ARK {
	namespace Geometry {

		/*!
		 * \brief A 2d point with a direction element.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		template <class T=float>
		class Vector2 {
			public:
				T m_x;
				T m_y;

				Vector2(): m_x(0), m_y(0) {

				}
				Vector2(T coords[]): m_x(coords[0]), m_y(coords[1]) {

				}
				Vector2(T x, T y): m_x(x), m_y(y) {

				}

				T getX() const {
					return m_x;
				}
				T getY() const {
					return m_y;
				}

				// copied from MathUtil::randBetween().
				T rand() {
					T lower = m_x;
					T upper = m_y;
					if (lower == upper) { return lower; }
					if (lower > upper) { int teacup = lower; lower = upper; upper = teacup; }
					return lower + (rand() % (upper-lower));
				}

				// Set
				void setX(T x) {
					m_x = x;
				}
				void setY(T y) {
					m_y = y;
				}
				void set(Vector2<T>* v) {
					set(v->getX(), v->getY());
				}
				void set(T x, T y) {
					m_x = x;
					m_y = y;
				}

				void adjustX(T x) {
					m_x += x;
				}
				void adjustY(T y) {
					m_y += y;
				}

				//float angle() {
				//	return MathUtil::anglef(0, 0, m_x, m_y);
				//}

				// Add 
				Vector2* add(Vector2<T>* v) {
					return add(v->getX(), v->getY());
				}
				Vector2* add(T x, T y) {
					m_x += x;
					m_y += y;
					return this;
				}
				Vector2* addX(T x) {
					m_x += x;
					return this;
				}
				Vector2* addY(T y) {
					m_y += y;
					return this;
				}

				// Multiply
				Vector2* multiply(Vector2<T>* v) {
					return multiply(v->getX(), v->getY());
				}
				Vector2* multiply(T x, T y) {
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


				// Divide
				Vector2* divide(Vector2<T>* v) {
					return divide(v->getX(), v->getY());
				}
				Vector2* divide(T x, T y) {
					m_x /= x;
					m_y /= y;
					return this;
				}
				void divideX(T x) {
					m_x /= x;
				}
				void divideY(T y) {
					m_y /= y;
				}

				// Subtract
				Vector2* subtract(Vector2<T>* v) {
					return subtract(v->getX(), v->getY());
				}
				Vector2* subtract(T x, T y) {
					m_x -= x;
					m_y -= y;
					return this;
				}
				Vector2* subtractX(T x) {
					m_x -= x;
					return this;
				}
				Vector2* subtractY(T y) {
					m_y -= y;
					return this;
				}

				// Distance
				Vector2* distance(Vector2<T>* v) {
//					ARK2D::getLog()->w("Vector2* Vector2::distance(Vector2* v) is inefficient, use other method.");
					Vector2<T>* result = new Vector2<T>();
					distance(v, result);
					return result;
				}
				Vector2* distance(T x, T y) {
//					ARK2D::getLog()->w("Vector2* Vector2::distance(T x, T y) is inefficient, use other method.");
					Vector2<T>* result = new Vector2<T>();
					distance(x, y, result);
					return result;
				}
				void distance(Vector2<T>* v, Vector2<T>* result) {
					distance(v->getX(), v->getY(), result);
				}
				void distance(T x, T y, Vector2<T>* result) {
					result->set(abs(m_x - x), abs(m_y - y));
				}

				// Friction
				Vector2* friction(T x, T y) {
					frictionX(x);
					frictionY(y);
					return this;
				}
				Vector2* frictionX(T x) {
					if (m_x < 0) {
						m_x += x;
						if (m_x > 0) { m_x = 0; }
					} else if (m_x > 0) {
						m_x -= x;
						if (m_x < 0) { m_x = 0; }
					}
					return this;
				}
				Vector2* frictionY(T y) {
					if (m_y < 0) {
						m_y += y;
						if (m_y > 0) { m_y = 0; }
					} else if (m_y > 0) {
						m_y -= y;
						if (m_y < 0) { m_y = 0; }
					}
					return this;
				}

				// Restrict 
				void restrictMax(Vector2<T>* max) {
					restrictMax(max->getX(), max->getY());
				}
				void restrictMax(T x, T y) {
					if (m_x > x) {
						m_x = x;
					}
					if (m_y > y) {
						m_y = y;
					} 

					if (m_x < x * -1) {
						m_x = x * -1;
					}
					if (m_y < y * -1) {
						m_y = y * -1;
					}
				}

				T dot(Vector2* v) {
					return dot(v->getX(), v->getY());
				}
				T dot(T x, T y) {
					return (m_x * x) + (m_y * y);
				}

				T cross(Vector2* v) {
					return cross(v->getX(), v->getY());
				}
				T cross(T x, T y) {
					return ((m_x * y) - (m_y * x));
				}

				static T s_cross(T x1, T y1, T x2, T y2) {
					return ((x1 * y2) - (y1 * x2));
				}

				T lengthSquared() const {
					return (m_x*m_x) + (m_y * m_y);
				}
				T length() const {
					return (T) sqrt((double) lengthSquared());
				}

				T diff() {
					return m_y - m_x;
				}

				Vector2<T> operator*(const Vector2<T>& other) { 
					m_x *= other.m_x;
					m_y *= other.m_y;
					return *this; 
				}
				Vector2<T> operator*=(const Vector2<T>& other) { 
					m_x *= other.m_x;
					m_y *= other.m_y;
					return *this; 
				}
				Vector2<T> operator*(float other) { 
					m_x *= other;
					m_y *= other;
					return *this; 
				}
				Vector2<T> operator*=(float other) { 
					m_x *= other;
					m_y *= other;
					return *this; 
				}

				Vector2<T> operator+=(const Vector2<T>& other) { 
					add(other.m_x, other.m_y); 
					return *this; 
				}
				Vector2<T> operator+=(float other) { 
					add(other, other); 
					return *this; 
				}


				string toString() {
					ARK::Core::String s;
					s += "{";
					s += "\"x\":";
					s += m_x;
					s += ", \"y\":";
					s += m_y;
					s += "}";
					return s.get();
				}

				~Vector2() {

				}
		};
	}
}
#endif /* VECTOR2_H_ */

