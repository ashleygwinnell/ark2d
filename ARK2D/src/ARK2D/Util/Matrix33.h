/*
 * Matrix33.h
 *
 *  Created on: 14 Apr 2011
 *      Author: Ashley
 */

#ifndef MATRIX33_H_
#define MATRIX33_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"
#include "../Geometry/Vector3.h"

namespace ARK {
	namespace Util {

		// Matrix 33
		class ARK2D_API Matrix33 {
			public:
				Vector3<float> col[3];
				Vector3<float>* x;
				Vector3<float>* y;
				Vector3<float>* z;
			public:
				Matrix33();
				Matrix33(float vals[9]);
				Matrix33(Matrix44& mat44);

                void set(Matrix44& mat44);
				void identity();

				float determinant();
				void transpose();

				void inverse();
				float* pointer();
				const float* pointer() const;

				Vector3<float>& operator[](unsigned int i);
				const Vector3<float>& operator[](unsigned int i) const;

				virtual ~Matrix33();
		};
	}
}
#endif
