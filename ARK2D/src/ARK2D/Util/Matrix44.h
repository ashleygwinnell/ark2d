/*
 * Matrix44.h
 *
 *  Created on: 14 Apr 2011
 *      Author: Ashley
 */

#ifndef MATRIX44_H_
#define MATRIX44_H_

#include "../Geometry/Vector3.h"
#include "Vector4.h"
#include "MathUtil.h"
#include "../UI/ErrorDialog.h" 

namespace ARK {
	namespace Util {

		// Matrix 33
		template <class T=float>
		class Matrix33 {
			public: 
				Vector3<T> col[3];
				Vector3<T>* x;
				Vector3<T>* y;
				Vector3<T>* z;
			public:
				Matrix33(): col() {
					x = &col[0];
					y = &col[1];
					z = &col[2];
					identity();
				}
				Matrix33(float vals[9]): col() {
					col[0][0] = vals[0];
					col[0][1] = vals[1];
					col[0][2] = vals[2];
					col[1][0] = vals[3];
					col[1][1] = vals[4];
					col[1][2] = vals[5];
					col[2][0] = vals[6];
					col[2][1] = vals[7];
					col[2][2] = vals[8];
					x = &col[0];
					y = &col[1];
					z = &col[2];
				}
				Matrix33(Matrix44<T>& mat44): col() {
					col[0][0] = mat44[0][0];
					col[0][1] = mat44[0][1];
					col[0][2] = mat44[0][2];
					col[1][0] = mat44[1][0];
					col[1][1] = mat44[1][1];
					col[1][2] = mat44[1][2];
					col[2][0] = mat44[2][0];
					col[2][1] = mat44[2][1];
					col[2][2] = mat44[2][2];
					x = &col[0];
					y = &col[1];
					z = &col[2];
				}
				void identity() {
					col[0][0] = 1.0f;
					col[0][1] = 0.0f;
					col[0][2] = 0.0f;
					
					col[1][0] = 0.0f;
					col[1][1] = 1.0f;
					col[1][2] = 0.0f;
					
					col[2][0] = 0.0f;
					col[2][1] = 0.0f;
					col[2][2] = 1.0f;
				}

				float determinant() {
					return MathUtil::determinant((void*) x, 3);
				}
				

				void transpose()
				{
					int n = 3;
					int i, j;
					float temp;

					for (i = 0; i < n; i++) {
						for (j = 0; j < n; j++) {
							temp = col[i][j];
							col[i][j] = col[j][i];
							col[j][i] = temp;
						}
					}
				}

				void inverse() {
					float d = determinant();
					for (int i = 0; i < 3; ++i)
					{
						for (int j = 0; j < 3; ++j)
						{
							col[i][j] /= d;
						}
					}
				}
				T* pointer() { 
					return &col[0][0];
				}
				const T* pointer() const { 
					return &col[0][0];
				}

				Vector4<T>& operator[](unsigned int i) {
					return col[i];
				}
				const Vector4<T>& operator[](unsigned int i) const {
					return col[i];
				}
		};

		/*!
		 * \brief Matrix manipulation for graphics rendering.
		 *
		 * This is not used internally.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		template <class T=float>
		class Matrix44 {
			public:
				Vector4<T> col[4];
				
				Vector4<T>* m_x;
				Vector4<T>* m_y;
				Vector4<T>* m_z;
				Vector4<T>* m_w;

			public:
				Matrix44(): col() {
					m_x = &col[0];
					m_y = &col[1];
					m_z = &col[2];
					m_w = &col[3];
					
					identity();
				}
				const Vector4<T>& getX() { return *m_x; }
				const Vector4<T>& getY() { return *m_y; }
				const Vector4<T>& getZ() { return *m_z; } 
				const Vector4<T>& getW() { return *m_w; }

				void toValue(T v) {
					m_x->toValue(v);
					m_y->toValue(v);
					m_z->toValue(v);
					m_w->toValue(v);
				}
				void toZero() {
					toValue(0);
				}
				void toOne() {
					toValue(1);
				}
				Matrix44<T> copy() {
					Matrix44<T> cpy;
					cpy.col[0][0] = col[0][0];
					cpy.col[0][1] = col[0][1];
					cpy.col[0][2] = col[0][2];
					cpy.col[0][3] = col[0][3];

					cpy.col[1][0] = col[1][0];
					cpy.col[1][1] = col[1][1]; 
					cpy.col[1][2] = col[1][2];
					cpy.col[1][3] = col[1][3];

					cpy.col[2][0] = col[2][0];
					cpy.col[2][1] = col[2][1];
					cpy.col[2][2] = col[2][2];
					cpy.col[2][3] = col[2][3];

					cpy.col[3][0] = col[3][0];
					cpy.col[3][1] = col[3][1];
					cpy.col[3][2] = col[3][2];
					cpy.col[3][3] = col[3][3];
					return cpy;
				}

				void identity() {
					col[0][0] = 1.0f;
					col[0][1] = 0.0f;
					col[0][2] = 0.0f;
					col[0][3] = 0.0f;
					
					col[1][0] = 0.0f;
					col[1][1] = 1.0f;
					col[1][2] = 0.0f;
					col[1][3] = 0.0f;
					
					col[2][0] = 0.0f;
					col[2][1] = 0.0f;
					col[2][2] = 1.0f;
					col[2][3] = 0.0f;
					
					col[3][0] = 0.0f;
					col[3][1] = 0.0f;
					col[3][2] = 0.0f;
					col[3][3] = 1.0f;

				}

				void ortho2d(float l, float r, float b, float t, float n, float f) {
					//http://www.gruzzlymug.com/projects/viseng/doc/d9/d7b/matrix44_8cpp-source.html
					float width = r - l;
					float height = t - b;
					float depth = f - n;

					col[0][0] = 2.0f / width;
					col[0][1] = 0.0f;
					col[0][2] = 0.0f;
					col[0][3] = 0.0f;

					col[1][0] = 0.0f;
					col[1][1] = 2.0f / height;
					col[1][2] = 0.0f; 
					col[1][3] = 0.0f;
 
					col[2][0] = 0.0f; 
					col[2][1] = 0.0f;  
					col[2][2] = 2.0f / depth;
					col[2][3] = 0.0f;

					col[3][0] = -(r + l) / width; 
					col[3][1] = -(t + b) / height;
					col[3][2] = -(f + n) / depth;
					col[3][3] = 1.0f;
				}
				void frustum(float l, float r, float b, float t, float n, float f) {
					col[0][0] = 2 * n / (r - l);
					col[0][1] = 0;
					col[0][2] = 0;
					col[0][3] = 0;

					col[1][0] = 0;
					col[1][1] = 2 * n / (t - b);
					col[1][2] = 0;
					col[1][3] = 0;

					col[2][0] = (r + l) / (r - l);
					col[2][1] = (t + b) / (t - b);
					col[2][2] = -(f + n) / (f - n);
					col[2][3] = -1;

					col[3][0] = 0;
					col[3][1] = 0;
					col[3][2] = -2 * f * n / (f - n);
					col[3][3] = 0;
				}
				void perspective(float fovDegrees, float aspectRatio, float znear, float zfar)
				{
					float scale = tan(MathUtil::toRadians(fovDegrees * 0.5)) * znear;
				    float r = aspectRatio * scale;
				    float l = -r;
				    float t = scale;
				    float b = -t;
				    frustum(l, r, b, t, znear, zfar);

					/*float ymax = znear * tan(fov * PI_OVER_360);
					float ymin = -ymax;
					float xmax = ymax * aspect;
					float xmin = ymin * aspect;

					//float width = xymax - xmin;
					//float height = xymax - ymin;
					float width = xmax - xmin;
					float height = ymax - ymin;

					float depth = zfar - znear;
					float q = -(zfar + znear) / depth;
					float qn = -2 * (zfar * znear) / depth;

					float w = 2 * znear / width;
					w = w / aspect;
					float h = 2 * znear / height;

					col[0][0] = w;
					col[0][1] = 0;
					col[0][2] = 0;
					col[0][3] = 0;

					col[1][0] = 0;
					col[1][1] = h;
					col[1][2] = 0;
					col[1][3] = 0;

					col[2][0] = 0;
					col[2][1] = 0;
					col[2][2] = q;
					col[2][3] = -1;

					col[3][0] = 0;
					col[3][1] = 0;
					col[3][2] = qn;
					col[3][3] = 0;*/
				}

				void lookAt(float eyeX, float eyeY, float eyeZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ) {
					
					//detail::tvec3<T, P> f(normalize(center - eye));
					//detail::tvec3<T, P> s(normalize(cross(f, up)));
					//detail::tvec3<T, P> u(cross(s, f));
					//eyeZ *= -1.0f;
					//lookAtZ *= -1.0f; 
					
					float fx, fy, fz; // forward
					MathUtil::normalizevec3(&fx, &fy, &fz, lookAtX - eyeX, lookAtY - eyeY, lookAtZ - eyeZ);
					//MathUtil::normalizevec3(&fx, &fy, &fz, eyeX - lookAtX, eyeY - lookAtY, eyeZ - lookAtZ);

					float sx, sy, sz; // side
					MathUtil::crossvec3(&sx, &sy, &sz, fx, fy, fz, upX, upY, upZ);
					MathUtil::normalizevec3(&sx, &sy, &sz, sx, sy, sz);

					float ux, uy, uz; // up
					MathUtil::crossvec3(&ux, &uy, &uz, sx, sy, sz, fx, fy, fz);

					Matrix44<float> lookAtMatrix;
					lookAtMatrix.identity();

					lookAtMatrix[0][0] = sx;
					lookAtMatrix[1][0] = sy;
					lookAtMatrix[2][0] = sz;

					lookAtMatrix[0][1] = ux;
					lookAtMatrix[1][1] = uy;
					lookAtMatrix[2][1] = uz;

					lookAtMatrix[0][2] = -fx;
					lookAtMatrix[1][2] = -fy;
					lookAtMatrix[2][2] = -fz;

					lookAtMatrix[3][0] = MathUtil::dotproductvec3(sx, sy, sz, eyeX, eyeY, eyeZ) * -1.0f;// -dot(s, eye);
					lookAtMatrix[3][1] = MathUtil::dotproductvec3(ux, uy, uz, eyeX, eyeY, eyeZ) * -1.0f; // -dot(u, eye);
					lookAtMatrix[3][2] = MathUtil::dotproductvec3(fx, fy, fz, eyeX, eyeY, eyeZ); // dot(f, eye);
					
					/*lookAtMatrix[0][3] = 0.0f;
					lookAtMatrix[1][3] = 0.0f;
					lookAtMatrix[2][3] = 0.0f;
					lookAtMatrix[3][3] = 1.0f;*/

					*this *= lookAtMatrix;

					//return Result;
				}
				

				void translate(float x, float y, float z) 
				{
					*this *= createTranslateMatrix(x, y, z);
				}

				static Matrix44<T> createTranslateMatrix(float x, float y, float z) {
					// http://en.wikipedia.org/wiki/Translation_(geometry)
					Matrix44<float> translateMatrix;
					translateMatrix.identity();
					translateMatrix[3][0] = x;
					translateMatrix[3][1] = y;
					translateMatrix[3][2] = z;
					return translateMatrix;
				}
				
				void scale(float x, float y, float z) 
				{
					*this *= createScaleMatrix(x, y, z);
				}
 
				static Matrix44<T> createScaleMatrix(float x, float y, float z) {
					// http://en.wikipedia.org/wiki/Scaling_(geometry)
					Matrix44<float> scaleMatrix;
					scaleMatrix.identity();
					scaleMatrix[0][0] = x;
					scaleMatrix[1][1] = y;
					scaleMatrix[2][2] = z;
					scaleMatrix[3][3] = 1;
					return scaleMatrix;
				}

				void rotate(float angle, float x, float y, float z) {
					// http://www.gruzzlymug.com/projects/viseng/doc/d9/d7b/matrix44_8cpp-source.html
					// normalise vector
					float len = (float) sqrt(x*x + y*y + z*z);
					if (len > 0.0f) { 
						len = 1.0f / len;
					} else { 
						len = 0.0f;
					}
					x *= len;
					y *= len;
					z *= len;

					// create rotation matrix
					Matrix44<float> rotMatrix = createRotationMatrix(angle, x, y, z);
					

					// do rotation
					*this *= rotMatrix;
				}

				static Matrix44<T> createRotationMatrix(float angleDegrees, float x, float y, float z) {
					float angle = MathUtil::toRadians(angleDegrees);
					Matrix44<float> rotMatrix;

					float sinA, cosA;
					float invCosA;
					float xSq, ySq, zSq;

					sinA = (float) sin(angle);
					cosA = (float) cos(angle);
					invCosA = 1.0f - cosA;

					xSq = x * x;
					ySq = y * y;
					zSq = z * z;

					rotMatrix[0][0] = (invCosA * xSq) + (cosA);
					rotMatrix[1][0] = (invCosA * x * y) - (sinA * z );
					rotMatrix[2][0] = (invCosA * x * z) + (sinA * y );
					rotMatrix[3][0] = 0.0f;

					rotMatrix[0][1] = (invCosA * x * y) + (sinA * z);
					rotMatrix[1][1] = (invCosA * ySq) + (cosA);
					rotMatrix[2][1] = (invCosA * y * z) - (sinA * x);
					rotMatrix[3][1] = 0.0f;

					rotMatrix[0][2] = (invCosA * x * z) - (sinA * y);
					rotMatrix[1][2] = (invCosA * y * z) + (sinA * x);
					rotMatrix[2][2] = (invCosA * zSq) + (cosA);
					rotMatrix[3][2] = 0.0f;

					rotMatrix[0][3] = 0.0f;
					rotMatrix[1][3] = 0.0f;
					rotMatrix[2][3] = 0.0f;
					rotMatrix[3][3] = 1.0f;

					return rotMatrix;
				}

				Vector4<T>& operator[](unsigned int i) {
					//assert (i<4);
					//if (i >= 4) { ErrorDialog::createAndShow("Invalid matrix44[] index."); exit(0); }
					return col[i];
				}
				const Vector4<T>& operator[](unsigned int i) const {
					//assert (i<4);
					//if (i >= 4) { ErrorDialog::createAndShow("Invalid matrix44[] index."); exit(0); }
					return col[i];
				}
			
				Matrix44<T>& operator*=(const Matrix44<T>& m) {
					Matrix44<T> t;
					/*for (unsigned int r = 0; r < 4; r++)
					{
						for (unsigned int c = 0; c < 4; c++)
						{
							t[c][r] = (col[0][r] * m[c][0]) + (col[1][r] * m[c][1]) + (col[2][r] * m[c][2]) + (col[3][r] * m[c][3]);
						}
					}*/

					// r = 0, c = 0;

					// assume right column is 0,0,0,1.
					// e.g. http://www.willmcgugan.com/gameobjects/docs/gameobjects.matrix44-pysrc.html#Matrix44.__imul__
					
					t[0][0] = (col[0][0] * m[0][0]) + (col[1][0] * m[0][1]) + (col[2][0] * m[0][2]) + (col[3][0] * m[0][3]);
					t[1][0] = (col[0][0] * m[1][0]) + (col[1][0] * m[1][1]) + (col[2][0] * m[1][2]) + (col[3][0] * m[1][3]);
					t[2][0] = (col[0][0] * m[2][0]) + (col[1][0] * m[2][1]) + (col[2][0] * m[2][2]) + (col[3][0] * m[2][3]);
					t[3][0] = (col[0][0] * m[3][0]) + (col[1][0] * m[3][1]) + (col[2][0] * m[3][2]) + (col[3][0] * m[3][3]);
					//t[3][0] = 0;

					t[0][1] = (col[0][1] * m[0][0]) + (col[1][1] * m[0][1]) + (col[2][1] * m[0][2]) + (col[3][1] * m[0][3]);
					t[1][1] = (col[0][1] * m[1][0]) + (col[1][1] * m[1][1]) + (col[2][1] * m[1][2]) + (col[3][1] * m[1][3]);
					t[2][1] = (col[0][1] * m[2][0]) + (col[1][1] * m[2][1]) + (col[2][1] * m[2][2]) + (col[3][1] * m[2][3]);
					t[3][1] = (col[0][1] * m[3][0]) + (col[1][1] * m[3][1]) + (col[2][1] * m[3][2]) + (col[3][1] * m[3][3]);
					//t[3][1] = 0;

					t[0][2] = (col[0][2] * m[0][0]) + (col[1][2] * m[0][1]) + (col[2][2] * m[0][2]) + (col[3][2] * m[0][3]);
					t[1][2] = (col[0][2] * m[1][0]) + (col[1][2] * m[1][1]) + (col[2][2] * m[1][2]) + (col[3][2] * m[1][3]);
					t[2][2] = (col[0][2] * m[2][0]) + (col[1][2] * m[2][1]) + (col[2][2] * m[2][2]) + (col[3][2] * m[2][3]);
					t[3][2] = (col[0][2] * m[3][0]) + (col[1][2] * m[3][1]) + (col[2][2] * m[3][2]) + (col[3][2] * m[3][3]);
					//t[3][2] = 0;

					//t[0][3] = (col[0][3] * m[0][0]) + (col[1][3] * m[0][1]) + (col[2][3] * m[0][2]) + (col[3][3] * m[0][3]);
					//t[1][3] = (col[0][3] * m[1][0]) + (col[1][3] * m[1][1]) + (col[2][3] * m[1][2]) + (col[3][3] * m[1][3]);
					//t[2][3] = (col[0][3] * m[2][0]) + (col[1][3] * m[2][1]) + (col[2][3] * m[2][2]) + (col[3][3] * m[2][3]);
					//t[3][3] = (col[0][3] * m[3][0]) + (col[1][3] * m[3][1]) + (col[2][3] * m[3][2]) + (col[3][3] * m[3][3]);
					t[0][3] = 0;
					t[1][3] = 0;
					t[2][3] = 0; 
					t[3][3] = 1;
					//t[3][3] = 1;
					
					//t[c][r] = (col[0][r] * m[c][0]) + (col[1][r] * m[c][1]) + (col[2][r] * m[c][2]) + (col[3][r] * m[c][3]);

					*this = t;
					return *this;
				}
				string toString() {
					string s = "{";
					s += Cast::toString<float>(col[0][0]); s += string(",");
					s += Cast::toString<float>(col[0][1]); s += string(",");
					s += Cast::toString<float>(col[0][2]); s += string(",");
					s += Cast::toString<float>(col[0][3]); s += string(",");

					s += Cast::toString<float>(col[1][0]); s += string(",");
					s += Cast::toString<float>(col[1][1]); s += string(",");
					s += Cast::toString<float>(col[1][2]); s += string(",");
					s += Cast::toString<float>(col[1][3]); s += string(",");

					s += Cast::toString<float>(col[2][0]); s += string(",");
					s += Cast::toString<float>(col[2][1]); s += string(",");
					s += Cast::toString<float>(col[2][2]); s += string(",");
					s += Cast::toString<float>(col[2][3]); s += string(",");

					s += Cast::toString<float>(col[3][0]); s += string(",");
					s += Cast::toString<float>(col[3][1]); s += string(",");
					s += Cast::toString<float>(col[3][2]); s += string(",");
					s += Cast::toString<float>(col[3][3]);
					s += string("}");
					return s;
				}

				void set(const Matrix44<T>& m) {
					col[0][0] = m[0][0];
					col[0][1] = m[0][1];
					col[0][2] = m[0][2];
					col[0][3] = m[0][3];

					col[1][0] = m[1][0];
					col[1][1] = m[1][1];
					col[1][2] = m[1][2];
					col[1][3] = m[1][3];
 
					col[2][0] = m[2][0];
					col[2][1] = m[2][1];
					col[2][2] = m[2][2];
					col[2][3] = m[2][3];

					col[3][0] = m[3][0];
					col[3][1] = m[3][1];
					col[3][2] = m[3][2];
					col[3][3] = m[3][3];
				}

				T* pointer() { 
					return &col[0][0];
				}
				const T* pointer() const { 
					return &col[0][0];
				}
				

				virtual ~Matrix44() {

				}
		};
	}
}

#endif /* MATRIX44_H_ */
