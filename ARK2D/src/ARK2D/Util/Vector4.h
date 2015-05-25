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
				T row[4];
				/*T* m_x;
				T* m_y;
				T* m_z;
				T* m_w;*/
 
			public:
				Vector4() {
					init();
				}
				Vector4(T coords[]) {
					init();

					row[0] = coords[0];
					row[1] = coords[1];
					row[2] = coords[2];
					row[3] = coords[3];
				}
				Vector4(T x, T y, T z, T w) {
					init();

					row[0] = x;
					row[1] = y;
					row[2] = z;
					row[3] = w;
				}

				void init() {
					row[0] = 0;
					row[1] = 0;
					row[2] = 0;
					row[3] = 0;
					//m_x = &row[0];
					//m_y = &row[1];
					//m_z = &row[2];
					//m_w = &row[3];
				}

				T getX() { return row[0]; }
				T getY() { return row[1]; }
				T getZ() { return row[2]; }
				T getW() { return row[3]; }

				void setX(T x) { row[0] = x; }
				void setY(T y) { row[1] = y; }
				void setZ(T z) { row[2] = z; }
				void setW(T w) { row[3] = w; }

				Vector4* add(Vector4 v) {
					return add(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				Vector4* add(T x, T y, T z, T w) {
					row[0] += x;
					row[1] += y;
					row[2] += z;
					row[3] += w;
					return this;
				}

				void set(Vector4 v) {
					set(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				void set(T x, T y, T z, T w) {
					row[0] = x;
					row[1] = y;
					row[2] = z;
					row[3] = w;
				}

				Vector4* subtract(Vector4 v) {
					return subtract(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				Vector4* subtract(T x, T y, T z, T w) {
					row[0] -= x;
					row[1] -= y;
					row[2] -= z;
					row[3] -= w;
					return this;
				}

				// Multiply
				Vector4* multiply(Vector4<T>& v) {
					return multiply(v.getX(), v.getY(), v.getZ(), v.getW());
				}
				Vector4* multiply(Vector4<T>* v) {
					return multiply(v->getX(), v->getY(), v->getZ(), v->getW());
				}
				Vector4* multiply(T x, T y, T z, T w) {
					row[0] *= x;
					row[1] *= y;
					row[2] *= z;
					row[3] *= w;
					return this;
				}
				

				void toValue(T v) {
					row[0] = v;
					row[1] = v;
					row[2] = v;
					row[3] = v;
				}
				void toZero() {
					toValue(0);
				}
				void toOne() {
					toValue(1);
				}

				Vector4<T> copy() {
					return Vector4<T>(row[0], row[1], row[2], row[3]);
				}

				T& operator[](unsigned int i) {
					//assert (i<4); 
					//if (i >= 4) { ErrorDialog::createAndShow("Invalid vector4[] index."); exit(0); }
					//return(i == 0) ? m_x : (i == 1) ? m_y : (i == 2) ? m_z : m_w;
					return row[i];
				}

				const T& operator[](unsigned int i) const {
					//assert (i<4); 
					//if (i >= 4) { ErrorDialog::createAndShow("Invalid vector4[] index."); exit(0); }
					//return(i == 0) ? m_x : (i == 1) ? m_y : (i == 2) ? m_z : m_w;
					return row[i];
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
				Vector4<T> operator*=(const Matrix44<T>& m) {
                    Vector4<T>::multMatrix44(row[0], row[1], row[2], row[3], m);
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
