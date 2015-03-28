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
				
				~Vector4() {

				}

		};
	}
}

#endif /* VECTOR4_H_ */
