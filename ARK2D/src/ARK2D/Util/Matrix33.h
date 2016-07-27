/*
 * Matrix33.h
 *
 *  Created on: 14 Apr 2011
 *      Author: Ashley
 */

#ifndef MATRIX33_H_
#define MATRIX33_H_

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



		// Matrix 33
		template <class T>
		class Matrix33Template {
			public:
				Vector3<T> col[3];
				Vector3<T>* x;
				Vector3<T>* y;
				Vector3<T>* z;
			public:
				Matrix33Template(): col() {
					x = &col[0];
					y = &col[1];
					z = &col[2];
					identity();
				}
				Matrix33Template(float vals[9]): col() {
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
					/*int n = 3;
					int i, j;
					float temp;

					for (i = 0; i < n; i++) {
						for (j = 0; j < n; j++) {
							temp = col[i][j];
							col[i][j] = col[j][i];
							col[j][i] = temp;
						}
					}*/

					glm::mat3 mat;
					mat[0][0] = col[0][0];
					mat[0][1] = col[0][1];
					mat[0][2] = col[0][2];
					mat[1][0] = col[1][0];
					mat[1][1] = col[1][1];
					mat[1][2] = col[1][2];
					mat[2][0] = col[2][0];
					mat[2][1] = col[2][1];
					mat[2][2] = col[2][2];

					mat = glm::transpose(mat);

					col[0][0] = mat[0][0];
					col[0][1] = mat[0][1];
					col[0][2] = mat[0][2];
					col[1][0] = mat[1][0];
					col[1][1] = mat[1][1];
					col[1][2] = mat[1][2];
					col[2][0] = mat[2][0];
					col[2][1] = mat[2][1];
					col[2][2] = mat[2][2];
				}

				void inverse() {
					/*float d = determinant();
					for (int i = 0; i < 3; ++i)
					{
						for (int j = 0; j < 3; ++j)
						{
							col[i][j] /= d;
						}
					}*/
					glm::mat3 mat;
					mat[0][0] = col[0][0];
					mat[0][1] = col[0][1];
					mat[0][2] = col[0][2];
					mat[1][0] = col[1][0];
					mat[1][1] = col[1][1];
					mat[1][2] = col[1][2];
					mat[2][0] = col[2][0];
					mat[2][1] = col[2][1];
					mat[2][2] = col[2][2];

					mat = glm::inverse(mat);

					col[0][0] = mat[0][0];
					col[0][1] = mat[0][1];
					col[0][2] = mat[0][2];
					col[1][0] = mat[1][0];
					col[1][1] = mat[1][1];
					col[1][2] = mat[1][2];
					col[2][0] = mat[2][0];
					col[2][1] = mat[2][1];
					col[2][2] = mat[2][2];
				}
				T* pointer() {
					return &col[0][0];
				}
				const T* pointer() const {
					return &col[0][0];
				}

				Vector3<T>& operator[](unsigned int i) {
					return col[i];
				}
				const Vector3<T>& operator[](unsigned int i) const {
					return col[i];
				}
		};

		class Matrix33 : public Matrix33Template<float> {
			public:
				Matrix33();
				virtual ~Matrix33();

                Matrix33(Matrix44& mat44);
                void set(Matrix44& mat44);

		};
	}
}
#endif
