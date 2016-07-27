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
//#include "../UI/ErrorDialog.h"


namespace ARK {
	namespace Util {

        class Matrix44;

        /*!
		 * \brief Useful for holding four integers e.g. for UI rectangle padding.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		template <class T>
		class Vector4Template {
			public:
				//T row[4];
				/*T* m_x;
				T* m_y;
				T* m_z;
				T* m_w;*/

				T x;
				T y;
				T z;
				T w;


			public:
				Vector4Template() {

				}
				Vector4Template(const Vector4Template<T>& r):
					x(r.x),
					y(r.y),
					z(r.z),
					w(r.w) {

				}
				Vector4Template(T coords[]) {
					x = coords[0];
					y = coords[1];
					z = coords[2];
					w = coords[3];
				}
				Vector4Template(T lx, T ly, T lz, T lw) {
					x = lx;
					y = ly;
					z = lz;
					w = lw;
				}

				T getX() { return x; }
				T getY() { return y; }
				T getZ() { return z; }
				T getW() { return w; }

				void setX(T lx) { x = lx; }
				void setY(T ly) { y = ly; }
				void setZ(T lz) { z = lz; }
				void setW(T lw) { w = lw; }

				Vector4Template* add(Vector4Template v) {
					return add(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				Vector4Template* add(T lx, T ly, T lz, T lw) {
					x += lx;
					y += ly;
					z += lz;
					w += lw;
					return this;
				}

				void set(Vector4Template v) {
					set(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				void set(T lx, T ly, T lz, T lw) {
					x = lx;
					y = ly;
					z = lz;
					w = lw;
				}

				Vector4Template* subtract(Vector4Template v) {
					return subtract(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				Vector4Template* subtract(T lx, T ly, T lz, T lw) {
					x -= lx;
					y -= ly;
					z -= lz;
					w -= lw;
					return this;
				}

				// Multiply
				Vector4Template* multiply(Vector4Template<T>& v) {
					return multiply(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				Vector4Template* multiply(Vector4Template<T>* v) {
					return multiply(v->getX(), v->getY(), v->getZ(), v->getW());
				}
				Vector4Template* multiply(T lx, T ly, T lz, T lw) {
					x *= lx;
					y *= ly;
					z *= lz;
					w *= lw;
					return this;
				}

				void toValue(T v) {
					x = v;
					y = v;
					z = v;
					w = v;
				}
				void toZero() {
					toValue(0);
				}
				void toOne() {
					toValue(1);
				}

				Vector4Template<T> copy() {
					return Vector4Template<T>(x, y, z, w);
				}

				T& operator[](unsigned int i) {
                    return *(&x + i);
                    //T* p = &x;
					//p += i;
					//return *p;
				}

				const T& operator[](unsigned int i) const {
					return *(&x + i);
					//const T* p = &x;
					//p += i;
					//return *p;
				}

				Vector4Template<T> operator+=(const Vector4Template<T>& other) {
					add(other[0], other[1], other[2], other[3]);
					return *this;
				}
				Vector4Template<T> operator*=(const Vector4Template<T>& other) {
					multiply(other[0], other[1], other[2], other[3]);
					return *this;
				}
				Vector4Template<T> operator*=(float other) {
					multiply(other, other, other, other);
					return *this;
				}
				Vector4Template<T> operator*(float other) {
					multiply(other, other, other, other);
					return *this;
				}




				~Vector4Template() {

				}

		};

        class Vector4 : public Vector4Template<float> {
            public:
            	Vector4(float lx, float ly, float lz, float lw):
            		Vector4Template(lx, ly, lz, lw) {

				}

				Vector4(Vector4Template<float>& v):
            		Vector4Template(v.x, v.y, v.z, v.w) {

				}


                Vector4 operator*(const Matrix44& m);
                Vector4 operator*=(const Matrix44& m);

                static void multMatrix44(float& x, float& y, float& z, float& w, const Matrix44& m);
        };


	}
}

#endif /* VECTOR4_H_ */
