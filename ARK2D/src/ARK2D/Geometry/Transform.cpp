/*
 * Transform.cpp
 *
 *  Created on: Oct 8, 2014
 *      Author: ashleygwinnell
 */

#include "Transform.h"
#include "../Util/Matrix44.h"

namespace ARK {
	namespace Util {

		Transform::Transform():
			position(0, 0, 0),
			scale(1.0f, 1.0f, 1.0f),
			rotation(0, 1, 0, 0),
			parent(NULL),
			children()
			{

		}

		void Transform::translate(float x, float y, float z) {
			position.adjust(x, y, z);
		}
		//void Transform::scale(float x, float y, float z) {
		//	scale.multiply(x, y, z);
		//}

		void Transform::toMatrix(Matrix44<float>* out) {
			out->identity();
			out->scale(scale.getX(), scale.getY(), scale.getZ());
			out->rotate(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ()); // angle, x, y, z
			out->translate(position.getX(), position.getY(), position.getZ());
		}

		Transform::~Transform() {

		}

		
	}
}
