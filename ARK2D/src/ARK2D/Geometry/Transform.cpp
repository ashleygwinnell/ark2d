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
			rotation()
			{

		}

		//void Transform::translate(float x, float y, float z) {
		//	position.adjust(x, y, z);
		//}
		//void Transform::scale(float x, float y, float z) {
		//	scale.multiply(x, y, z);
		//}

		Matrix44<float> Transform::toMatrix() {
			Matrix44<float> m;
			m.translate(position.getX(), position.getY(), position.getZ());
    		m *= rotation.toMatrix();
    		m.scale(scale.getX(), scale.getY(), scale.getZ());
    		return m;
		}

		Transform::~Transform() {

		}

		
	}
}
