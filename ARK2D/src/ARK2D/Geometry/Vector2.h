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
#include "../Util/Log.h"
#include "../ARK2D.h"

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

				T getX() {
					return m_x;
				}
				T getY() {
					return m_y;
				}

				void setX(T x) {
					m_x = x;
				}
				void setY(T y) {
					m_y = y;
				}

				Vector2* add(Vector2<T>* v) {
					return add(v->getX(), v->getY());
				}
				Vector2* add(T x, T y) {
					m_x += x;
					m_y += y;
					return this;
				}

				void set(Vector2<T>* v) {
					set(v->getX(), v->getY());
				}

				void set(T x, T y) {
					m_x = x;
					m_y = y;
				}

				Vector2* subtract(Vector2<T>* v) {
					return subtract(v->getX(), v->getY());
				}
				Vector2* subtract(T x, T y) {
					m_x -= x;
					m_y -= y;
					return this;
				}

				Vector2* distance(Vector2<T>* v) {
					ARK2D::getLog()->w("Vector2* Vector2::distance(Vector2* v) is inefficient, use other method.");
					Vector2<T>* result = new Vector2<T>();
					distance(v, result);
					return result;
				}
				Vector2* distance(T x, T y) {
					ARK2D::getLog()->w("Vector2* Vector2::distance(T x, T y) is inefficient, use other method.");
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

				T dot(Vector2* v) {
					return dot(v->getX(), v->getY());
				}
				T dot(T x, T y) {
					return (m_x * x) + (m_y * y);
				}

				T lengthSquared() {
					return (m_x*m_x) + (m_y * m_y);
				}
				T length() {
					return sqrt(lengthSquared());
				}

				~Vector2() {

				}
		};
	}
}
#endif /* VECTOR2_H_ */

