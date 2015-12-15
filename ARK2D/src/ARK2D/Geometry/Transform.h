/*
 * Transform.h
 *
 *  Created on: Oct 8, 2014
 *      Author: ashleygwinnell
 */

#ifndef ARK_MATH_TRANSFORM_H_
#define ARK_MATH_TRANSFORM_H_

#include "Vector3.h"
#include "Quaternion.h"
#include "../Util/Vector4.h"

#include <vector>
using namespace std; 

namespace ARK {
	namespace Util {

		class Transform {
			public:
				Vector3<float> position;
				Vector3<float> scale;
				Quaternion<float> rotation;
				
				Transform();
				// void translate(float x, float y, float z); // use += on position
                // void scale(float x, float y, float z); // use *= on scale
				Matrix44<float> toMatrix();
				virtual ~Transform();
		};

	}
}

#endif
