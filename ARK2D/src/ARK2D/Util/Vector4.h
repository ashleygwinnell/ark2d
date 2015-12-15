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
#include "../UI/ErrorDialog.h"

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
				Vector4() {

				}
				Vector4(const Vector4<T>& r):
					x(r.x),
					y(r.y),
					z(r.z),
					w(r.w) {

				}
				Vector4(T coords[]) {
					x = coords[0];
					y = coords[1];
					z = coords[2];
					w = coords[3];
				}
				Vector4(T lx, T ly, T lz, T lw) {
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

				Vector4* add(Vector4 v) {
					return add(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				Vector4* add(T lx, T ly, T lz, T lw) {
					x += lx;
					y += ly;
					z += lz;
					w += lw;
					return this;
				}

				void set(Vector4 v) {
					set(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				void set(T lx, T ly, T lz, T lw) {
					x = lx;
					y = ly;
					z = lz;
					w = lw;
				}

				Vector4* subtract(Vector4 v) {
					return subtract(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				Vector4* subtract(T lx, T ly, T lz, T lw) {
					x -= lx;
					y -= ly;
					z -= lz;
					w -= lw;
					return this;
				}

				// Multiply
				Vector4* multiply(Vector4<T>& v) {
					return multiply(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				Vector4* multiply(Vector4<T>* v) {
					return multiply(v->getX(), v->getY(), v->getZ(), v->getW());
				}
				Vector4* multiply(T lx, T ly, T lz, T lw) {
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

				Vector4<T> copy() {
					return Vector4<T>(x, y, z, w);
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

				Vector4<T> operator+=(const Vector4<T>& other) { 
					add(other[0], other[1], other[2], other[3]);  
					return *this; 
				}
				Vector4<T> operator*=(const Vector4<T>& other) { 
					multiply(other[0], other[1], other[2], other[3]);  
					return *this; 
				}
				Vector4<T> operator*=(float other) { 
					multiply(other, other, other, other);  
					return *this; 
				}
				Vector4<T> operator*(float other) { 
					multiply(other, other, other, other);  
					return *this; 
				}
				Vector4<T> operator*(const Matrix44<T>& m) {
                    Vector4<T> ret(*this);
                    ret *= m;
                    return ret;
				}
				Vector4<T> operator*=(const Matrix44<T>& m) {
                    Vector4<T>::multMatrix44(x, y, z, w, m);
				    return *this;
				}

				static void multMatrix44(float& x, float& y, float& z, float& w, const Matrix44<T>& m) {
					T newX = x*m[0][0] + y*m[1][0] + z*m[2][0] + w*m[3][0];
                    T newY = x*m[0][1] + y*m[1][1] + z*m[2][1] + w*m[3][1];
                    T newZ = x*m[0][2] + y*m[1][2] + z*m[2][2] + w*m[3][2];
                    T newW = x*m[0][3] + y*m[1][3] + z*m[2][3] + w*m[3][3];
                    x = newX;
                    y = newY;
                    z = newZ;
                    w = newW;
				}
				
				~Vector4() {

				}

		};


	}
}

#endif /* VECTOR4_H_ */
