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

		Vector2* add(Vector2 v) {
			return add(v.getX(), v.getY());
		}
		Vector2* add(T x, T y) {
			m_x += x;
			m_y += y;
			return this;
		}

		void set(Vector2 v) {
			set(v.getX(), v.getY());
		}

		void set(T x, T y) {
			m_x = x;
			m_y = y;
		}

		Vector2* subtract(Vector2 v) {
			return subtract(v.getX(), v.getY());
		}
		Vector2* subtract(T x, T y) {
			m_x -= x;
			m_y -= y;
			return this;
		}

		Vector2* distance(Vector2 v) {
			return this->distance(v.getX(), v.getY());
		}
		Vector2* distance(T x, T y) {
			Vector2* dist = new Vector2(abs(m_x - x), abs(m_y - y));
			return dist;
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

		~Vector2() {

		}
};

#endif /* VECTOR2_H_ */

