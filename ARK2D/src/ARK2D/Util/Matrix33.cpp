/*
 * Matrix33.cpp
 *
 *  Created on: Jul 28, 2016
 *      Author: ashleygwinnell
 */

#include "Matrix33.h"
#include "Matrix44.h"

#include "Vector4.h"

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

		Matrix33::Matrix33(): col() {
            x = &col[0];
            y = &col[1];
            z = &col[2];
            identity();
        }

        Matrix33::Matrix33(float vals[9]): col() {
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

        Matrix33::Matrix33(Matrix44& mat44): col() {
            set(mat44);
        }

        void Matrix33::set(Matrix44& mat44) {
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

        void Matrix33::identity() {
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

        float Matrix33::determinant() {
            return MathUtil::determinant((void*) x, 3);
        }

        void Matrix33::transpose()
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

        void Matrix33::inverse() {
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
        float* Matrix33::pointer() {
            return &col[0][0];
        }
        const float* Matrix33::pointer() const {
            return &col[0][0];
        }

        Vector3<float>& Matrix33::operator[](unsigned int i) {
            return col[i];
        }
        const Vector3<float>& Matrix33::operator[](unsigned int i) const {
            return col[i];
        }

        Matrix33::~Matrix33() {

        }
	}
}

