/*
 * Matrix33.cpp
 *
 *  Created on: Jul 28, 2016
 *      Author: ashleygwinnell
 */

#include "Matrix33.h"
#include "Matrix44.h"

namespace ARK {
	namespace Util {
		Matrix33::Matrix33():
			Matrix33Template() {

		}
		Matrix33::Matrix33(Matrix44& mat44):
            Matrix33Template() {
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

		Matrix33::~Matrix33() {

		}
	}
}

