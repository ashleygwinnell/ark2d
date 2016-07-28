/*
 * Wobble.cpp
 *
 *  Created on: 04 Dec 2014
 *      Author: Ashley
 */

#include "Wobble.h"
#include "../Tween/Easing.h"
#include "../Core/GameTimer.h"
#include "../Util/MathUtil.h"

namespace ARK {
	namespace Util { 

		Wobble::Wobble() {
			movementEasing = Easing::QUADRATIC_IN_OUT;
			rotationEasing = Easing::QUADRATIC_IN_OUT;
			timer = 0.0f;
			duration = 2.0f;

			rottimer = 0.0f; 
			rotduration = 2.0f;

			rotatingClockwise = true;
		}
		void Wobble::setDuration(float f) {
			duration = f;
			rotduration = f;
		}
		void Wobble::setDuration(float translate, float rotate) {
			duration = translate;
			rotduration = rotate;
		}
		void Wobble::initMovement(float movementPixels) {
			this->randomMovement.set(movementPixels, movementPixels);
		}
		void Wobble::initMovement(float movementPixelsX, float movementPixelsY) {
			this->randomMovement.set(movementPixelsX, movementPixelsY);
		}
		void Wobble::initRotation(float rotationMin, float rotationMax) {
			randomRotation = rotationMin;
			this->randomRotationMin = rotationMin;
			this->randomRotationMax = rotationMax;
		}
		void Wobble::initRotation(float rotationMin, float rotationMax, float rotationSpeedMin, float rotationSpeedMax) {
			randomRotation = rotationMin;
			this->randomRotationMin = rotationMin;
			this->randomRotationMax = rotationMax;
			//this->rotationSpeedMin = rotationSpeedMin;
			//this->rotationSpeedMax = rotationSpeedMax;
		}
		void Wobble::start() { 
			timer = MathUtil::randBetweenf(0.0f, duration);
			rottimer = MathUtil::randBetweenf(0.0f, rotduration);

			curX = 0.0f;
			curY = 0.0f;
			wobbleX = 0.0f;
			wobbleY = 0.0f;
			wobbleStartX = 0.0f;
			wobbleStartY = 0.0f;
			resetMovement();

			curRot = 0.0f;
			rotationZ = 0.0f;
			rotationStartZ = 0.0f;
			resetRotation();
		}
		void Wobble::update(GameTimer* gt) {
			
			// Go from wobbleStartX to wobbleX.
			curX = Easing::ease(movementEasing, timer, wobbleStartX, wobbleX, duration);
			curY = Easing::ease(movementEasing, timer, wobbleStartY, wobbleY, duration);

			timer += gt->getDelta();
			if (timer > duration) {
				resetMovement();
				timer = 0.0f;
			}

			// Go from rotationStartZ to rotationZ;
			curRot = Easing::easebetween(rotationEasing, rottimer, rotationStartZ, rotationZ, rotduration);

			rottimer += gt->getDelta();
			if (rottimer > rotduration) {
				resetRotation();
				rottimer = 0.0f;
			}
			
		}
		void Wobble::resetMovement() 
		{
			wobbleStartX = wobbleStartX + wobbleX;
			wobbleStartY = wobbleStartY + wobbleY;

			float xMinimum = randomMovement.m_x * -1;
			float yMinimum = randomMovement.m_y * -1;

			float xMaximum = randomMovement.m_x;
			float yMaximum = randomMovement.m_y;

			xMinimum = (randomMovement.m_x + wobbleStartX) * -1;
			yMinimum = (randomMovement.m_y + wobbleStartY) * -1;

			xMaximum = randomMovement.m_x + (wobbleStartX * -1);
			yMaximum = randomMovement.m_y + (wobbleStartY * -1);

			wobbleX = MathUtil::randBetweenf(xMinimum, xMaximum);
			wobbleY = MathUtil::randBetweenf(yMinimum, yMaximum);
		}
		void Wobble::resetRotation() {
			rotationStartZ = curRot; //rotationStartZ + rotationZ;

			/*float rotMinimum = randomRotation * -1.0f;
			float rotMaximum = randomRotation;

			rotMinimum = (randomRotation + rotationStartZ) * -1;
			rotMaximum = randomRotation + (rotationStartZ * -1);*/
			float rotMinimum = this->randomRotationMin;
			float rotMaximum = this->randomRotationMax;

			if (rotationStartZ < 0) {
				rotationZ = MathUtil::randBetweenf(rotMaximum*0.5f, rotMaximum);	
			} else {
				rotationZ = MathUtil::randBetweenf(rotMinimum, rotMinimum*0.5f);	
			}
			
			
			//rotationSpeed = MathUtil::randBetweenf(rotationSpeedMin, rotationSpeedMax);
		}
		Wobble::~Wobble() {

		}
	}
}
