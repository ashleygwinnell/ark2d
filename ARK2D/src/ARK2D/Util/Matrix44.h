/*
 * Matrix44.h
 *
 *  Created on: 14 Apr 2011
 *      Author: Ashley
 */

#ifndef MATRIX44_H_
#define MATRIX44_H_

#include "Vector4.h"
#include "../Namespaces.h"
#include "../Geometry/Vector3.h"
#include "MathUtil.h"
//#include "../UI/ErrorDialog.h"

 #include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtx/transform.hpp"
#include "../vendor/glm/gtc/quaternion.hpp"
#include "../vendor/glm/gtx/quaternion.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/gtc/type_ptr.hpp"

namespace ARK {
	namespace Util {


		/*!
		 * \brief Matrix manipulation for graphics rendering.
		 *
		 * This is not used internally.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		template <class T>
		class Matrix44Template {
			public:
				Vector4Template<T> col[4];

				Vector4Template<T>* m_x;
				Vector4Template<T>* m_y;
				Vector4Template<T>* m_z;
				Vector4Template<T>* m_w;

			public:
				Matrix44Template(): col() {
					m_x = &col[0];
					m_y = &col[1];
					m_z = &col[2];
					m_w = &col[3];

					identity();
				}
				Matrix44Template(float vals[16]): col() {
					m_x = &col[0];
					m_y = &col[1];
					m_z = &col[2];
					m_w = &col[3];

					col[0][0] = vals[0];
					col[0][1] = vals[1];
					col[0][2] = vals[2];
					col[0][3] = vals[3];
					col[1][0] = vals[4];
					col[1][1] = vals[5];
					col[1][2] = vals[6];
					col[1][3] = vals[7];
					col[2][0] = vals[8];
					col[2][1] = vals[9];
					col[2][2] = vals[10];
					col[2][3] = vals[11];
					col[3][0] = vals[12];
					col[3][1] = vals[13];
					col[3][2] = vals[14];
					col[3][3] = vals[15];
				}
				const Vector4Template<T>& getX() { return *m_x; }
				const Vector4Template<T>& getY() { return *m_y; }
				const Vector4Template<T>& getZ() { return *m_z; }
				const Vector4Template<T>& getW() { return *m_w; }

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
					glm::mat4 m = glm::ortho(l, r, b, t, n, f);
					set(m);
					return;

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
					glm::mat4 m = glm::perspective(fovDegrees, aspectRatio, znear, zfar);
					set(m);
					return;
					/*float scale = tan(MathUtil::toRadians(fovDegrees * 0.5)) * znear;
				    float r = aspectRatio * scale;
				    float l = -r;
				    float t = scale;
				    float b = -t;
				    frustum(l, r, b, t, znear, zfar);*/

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

					glm::mat4 m = glm::lookAt(
						glm::vec3(eyeX, eyeY, eyeZ),
						glm::vec3(lookAtX, lookAtY, lookAtZ),
						glm::vec3(upX, upY, upZ)
					);
					set(m);
					return;

					/*
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
					//lookAtMatrix.identity(); // this is done in constructor

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

					//lookAtMatrix[0][3] = 0.0f;
					//lookAtMatrix[1][3] = 0.0f;
					//lookAtMatrix[2][3] = 0.0f;
					//lookAtMatrix[3][3] = 1.0f;

					*this *= lookAtMatrix;

					//return Result;*/
				}









                void inverse() {
                    glm::mat4 mat;
                    mat[0][0] = col[0][0];
                    mat[0][1] = col[0][1];
                    mat[0][2] = col[0][2];
                    mat[0][3] = col[0][3];
                    mat[1][0] = col[1][0];
                    mat[1][1] = col[1][1];
                    mat[1][2] = col[1][2];
                    mat[1][3] = col[1][3];
                    mat[2][0] = col[2][0];
                    mat[2][1] = col[2][1];
                    mat[2][2] = col[2][2];
                    mat[2][3] = col[2][3];
                    mat[3][0] = col[3][0];
                    mat[3][1] = col[3][1];
                    mat[3][2] = col[3][2];
                    mat[3][3] = col[3][3];

                    mat = glm::inverse(mat);
                    set(mat);
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


				virtual void set(const glm::mat4& m) {
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



				virtual ~Matrix44Template() {

				}
		};


		class Matrix44 : public Matrix44Template<float> {
			public:
				Matrix44();
				Matrix44(float vals[16]);
				virtual ~Matrix44();

                void scale(float x, float y, float z);
				void translate(float x, float y, float z);
				void rotate(float angle, float x, float y, float z);

                static Matrix44 createTranslateMatrix(float x, float y, float z);
                static Matrix44 createScaleMatrix(float x, float y, float z);
				static Matrix44 createRotationMatrix(float angleDegrees, float x, float y, float z);

				virtual void set(const glm::mat4& m);
				void set(const Matrix44& m);
				Matrix44 copy();

				Matrix44 operator*(const Matrix44& m) const;
				Matrix44& operator*=(const Matrix44& m);
				Vector4 operator*(const Vector4& other) const;

				Vector4Template<float>& operator[](unsigned int i);
				const Vector4Template<float>& operator[](unsigned int i) const;



		};
	}
}

#endif /* MATRIX44_H_ */
