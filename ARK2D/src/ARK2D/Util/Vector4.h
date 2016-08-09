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
//#include "../UI/ErrorDialog.h"


namespace ARK {
	namespace Util {

        class Matrix44;

        /*!
		 * \brief Useful for holding four integers e.g. for UI rectangle padding.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class Vector4 {
			public:
				float x;
				float y;
				float z;
				float w;

			public:
                Vector4();
				Vector4(const Vector4& r);
				Vector4(float coords[]);
				Vector4(float lx, float ly, float lz, float lw);

				float getX();
				float getY();
				float getZ();
				float getW();

				void setX(float lx);
				void setY(float ly);
				void setZ(float lz);
				void setW(float lw);

				Vector4* add(Vector4 v);
				Vector4* add(float lx, float ly, float lz, float lw);

				void set(Vector4 v);
				void set(float lx, float ly, float lz, float lw);

				Vector4* subtract(Vector4 v);
				Vector4* subtract(float lx, float ly, float lz, float lw);

				// Multiply
				Vector4* multiply(Vector4& v);
				Vector4* multiply(Vector4* v);
				Vector4* multiply(float lx, float ly, float lz, float lw);

				void toValue(float v);
				void toZero();
				void toOne();

				Vector4 copy();

				float& operator[](unsigned int i);

				const float& operator[](unsigned int i) const;

				Vector4 operator+=(const Vector4& other);
				Vector4 operator*=(const Vector4& other);
				Vector4 operator*=(float other);
				Vector4 operator*(float other);

				Vector4 operator*(const Matrix44& m);
                Vector4 operator*=(const Matrix44& m);

                static void multMatrix44(float& x, float& y, float& z, float& w, const Matrix44& m);


				~Vector4();

		};




	}
}

#endif /* VECTOR4_H_ */
