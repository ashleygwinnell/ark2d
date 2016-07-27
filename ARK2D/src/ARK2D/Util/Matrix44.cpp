/*
 * Matrix44.cpp
 *
 *  Created on: Dec 8, 2011
 *      Author: ashleygwinnell
 */

#include "Matrix44.h"

namespace ARK {
	namespace Util {

		Matrix44::Matrix44():
			Matrix44Template() {

		}
		Matrix44::Matrix44(float vals[16]):
            Matrix44Template(vals) {
		}

		Matrix44::~Matrix44() {

		}

		void Matrix44::scale(float x, float y, float z)
		{
			*this *= createScaleMatrix(x, y, z);
		}
		void Matrix44::translate(float x, float y, float z)
		{
		    *this *= createTranslateMatrix(x, y, z);
		}
		void Matrix44::rotate(float angle, float x, float y, float z) {
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
			Matrix44 rotMatrix = createRotationMatrix(angle, x, y, z);


			// do rotation
			*this *= rotMatrix;
		}

		Matrix44 Matrix44::createScaleMatrix(float x, float y, float z) {
			// http://en.wikipedia.org/wiki/Scaling_(geometry)
			Matrix44 scaleMatrix;
			//scaleMatrix.identity(); // this is done in constructor
			scaleMatrix[0][0] = x;
			scaleMatrix[1][1] = y;
			scaleMatrix[2][2] = z;
			scaleMatrix[3][3] = 1;
			return scaleMatrix;
		}

		Matrix44 Matrix44::createTranslateMatrix(float x, float y, float z) {
			// http://en.wikipedia.org/wiki/Translation_(geometry)
			Matrix44 translateMatrix;
			//translateMatrix.identity(); // this is done in constructor
			translateMatrix[3][0] = x;
			translateMatrix[3][1] = y;
			translateMatrix[3][2] = z;
			return translateMatrix;
		}


		Matrix44 Matrix44::createRotationMatrix(float angleDegrees, float x, float y, float z) {
			float angle = MathUtil::toRadians(angleDegrees);
			Matrix44 rotMatrix;

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


		Matrix44& Matrix44::operator*=(const Matrix44& m) {
			*this = *this * m;
			return *this;
		}

		Vector4Template<float>& Matrix44::operator[](unsigned int i) {
			//assert (i<4);
			//if (i >= 4) { ErrorDialog::createAndShow("Invalid matrix44[] index."); exit(0); }
			return col[i];
		}
		const Vector4Template<float>& Matrix44::operator[](unsigned int i) const {
			//assert (i<4);
			//if (i >= 4) { ErrorDialog::createAndShow("Invalid matrix44[] index."); exit(0); }
			return col[i];
		}


		Vector4 Matrix44::operator*(const Vector4& other) const {
		    Vector4 ret(other);
		    ret *= *this;
		    return ret;
		}

        Matrix44 Matrix44::operator*(const Matrix44& m) const {

			glm::mat4 lhs;
			lhs[0][0] = col[0][0];
			lhs[0][1] = col[0][1];
			lhs[0][2] = col[0][2];
			lhs[0][3] = col[0][3];
			lhs[1][0] = col[1][0];
			lhs[1][1] = col[1][1];
			lhs[1][2] = col[1][2];
			lhs[1][3] = col[1][3];
			lhs[2][0] = col[2][0];
			lhs[2][1] = col[2][1];
			lhs[2][2] = col[2][2];
			lhs[2][3] = col[2][3];
			lhs[3][0] = col[3][0];
			lhs[3][1] = col[3][1];
			lhs[3][2] = col[3][2];
			lhs[3][3] = col[3][3];
			glm::mat4 rhs;
			rhs[0][0] = m[0][0];
			rhs[0][1] = m[0][1];
			rhs[0][2] = m[0][2];
			rhs[0][3] = m[0][3];
			rhs[1][0] = m[1][0];
			rhs[1][1] = m[1][1];
			rhs[1][2] = m[1][2];
			rhs[1][3] = m[1][3];
			rhs[2][0] = m[2][0];
			rhs[2][1] = m[2][1];
			rhs[2][2] = m[2][2];
			rhs[2][3] = m[2][3];
			rhs[3][0] = m[3][0];
			rhs[3][1] = m[3][1];
			rhs[3][2] = m[3][2];
			rhs[3][3] = m[3][3];
			glm::mat4 rs = lhs * rhs;
			Matrix44 ret;
			ret.set(rs);
			return ret;

		}

			void Matrix44::set(const Matrix44& m) {
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

		Matrix44 Matrix44::copy() {
					Matrix44 cpy;
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

				void Matrix44::set(const glm::mat4& m) {
					Matrix44Template<float>::set(m);
				}

	}
}
