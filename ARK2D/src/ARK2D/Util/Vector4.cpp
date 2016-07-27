/*
 * Vector4.cpp
 *
 *  Created on: Dec 8, 2011
 *      Author: ashleygwinnell
 */

#include "Vector4.h"
#include "Matrix44.h"

namespace ARK {
	namespace Util {

		Vector4 Vector4::operator*(const Matrix44& m) {
		    Vector4 ret(*this);
		    ret *= m;
		    return ret;
		}
		Vector4 Vector4::operator*=(const Matrix44& m) {
		    Vector4::multMatrix44(x, y, z, w, m);
		}


		void Vector4::multMatrix44(float& x, float& y, float& z, float& w, const Matrix44& m) {
		    float newX = x*m[0][0] + y*m[1][0] + z*m[2][0] + w*m[3][0];
		    float newY = x*m[0][1] + y*m[1][1] + z*m[2][1] + w*m[3][1];
		    float newZ = x*m[0][2] + y*m[1][2] + z*m[2][2] + w*m[3][2];
		    float newW = x*m[0][3] + y*m[1][3] + z*m[2][3] + w*m[3][3];
		    x = newX;
		    y = newY;
		    z = newZ;
		    w = newW;
		}
	}
}
