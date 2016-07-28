/*
 * Wobble.h
 *
 *  Created on: 03 Dec 2014
 *      Author: Ashley
 */

#ifndef ARK_UTIL_WOBBLE_H_
#define ARK_UTIL_WOBBLE_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"
#include "../Geometry/Vector2.h"

namespace ARK {
	namespace Util {

		class ARK2D_API Wobble {
			public:
				unsigned int movementEasing;
				unsigned int rotationEasing;

				Vector2<float> randomMovement;
				
				//float movementSpeedMin; 
				//float movementSpeedMax;
				
				float randomRotationMin;
				float randomRotationMax;
				float randomRotation;
				
				float rotationSpeedMin;
				float rotationSpeedMax;

				float timer;
				float duration;
				float rottimer;
				float rotduration;

				// x,y wobble
				float wobbleStartX;
				float wobbleStartY;

				float wobbleX;
				float wobbleY;
				float wobbleSpeed;

				float curX;
				float curY;

				// rotation.
				float rotationStartZ;
				float rotationZ;
				float rotationSpeed;

				float curRot;

				bool rotatingClockwise;
			
			public:   
				Wobble();
				void setDuration(float f);
				void setDuration(float translate, float rotate);
				inline float getX() { return curX; }
				inline float getY() { return curY; }
				inline float getRotation() { return curRot; }
				void initMovement(float movementPixels);
				void initMovement(float movementPixelsX, float movementPixelsY);
				void initRotation(float rotationMin, float rotationMax); //, float rotationSpeedMin, float rotationSpeedMax);
				void initRotation(float rotationMin, float rotationMax, float rotationSpeedMin, float rotationSpeedMax);
				void start();
				void update(GameTimer* gt);
				void resetMovement();
				void resetRotation();
				~Wobble();

		};
	}
}

#endif /* ARK_UTIL_WOBBLE_H_ */
