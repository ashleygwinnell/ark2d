/*
 * MathUtil.h
 *
 *  Created on: 25-May-2010
 *      Author: Ashley
 */

#ifndef MATHUTIL_H_
#define MATHUTIL_H_

#include <math.h>
#include <time.h>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "Vector2.h"
#include "Cast.h"

#ifndef PI
	#define PI 3.14159265
#endif

class MathUtil {
	public:
		static const double PIE = 3.14159265;

		static void seedRandom();
		static int randBetween(int lower, int upper);

		static double distance(float x1, float y1, float x2, float y2);
		static double angle(int x1, int y1, int x2, int y2);
		static double anglef(float x1, float y1, float x2, float y2);
		static double toRadians(double angle);
		static double getPercentage(int top, int denominator);
		static const char* getPercentage_CSTR(int top, int denominator);
		static void snap(int& snap, int& x, int& y);



		template <class T>
		static Vector2<T>* findCenter() {
			// get lowest x
			// get highest x
			// get lowest y
			// get highest y
			// find center.
			return new Vector2<T>(0, 0);
		}

		template <class T>
		static void rotatePointAroundPoint(Vector2<T>* pointToRotate, Vector2<T>* centerOfRotation, float angle) {
			T centerx = centerOfRotation->getX();
			T centery = centerOfRotation->getY();

			double newAngle = MathUtil::toRadians((double) angle);

			// normalise by making the center of rotation the origin (0, 0).
			pointToRotate->setX(pointToRotate->getX() - centerx);
			pointToRotate->setY(pointToRotate->getY() - centery);

			//std::cout << (pointToRotate->getX() * cos(angle))  - (pointToRotate->getY() * sin(angle)) << std::endl;
			T newx = (pointToRotate->getX() * cos(newAngle)) - (pointToRotate->getY() * sin(newAngle));
			T newy = (pointToRotate->getX() * sin(newAngle)) + (pointToRotate->getY() * cos(newAngle));

			//std::cout << newx << " : " << newy << std::endl;

			pointToRotate->setX( newx );
			pointToRotate->setY( newy );

			pointToRotate->setX(pointToRotate->getX() + centerx);
			pointToRotate->setY(pointToRotate->getY() + centery);
		}

		//template <class T>
		//static Vector2<T> s_moveAngleFromOriginV;

		template <class T>
		static Vector2<T>* moveAngleFromOrigin(double angleDegrees, float distance) {
			Vector2<T>* v = new Vector2<T>(0, 0);
			moveAngle(v, angleDegrees, distance);
			return v;
		}

		template <class T>
		static void moveAngle(Vector2<T>* vector, double angleDegrees, float distance)  {
			double angleRadians = toRadians(angleDegrees);
			vector->add((int) (distance * double(cos(angleRadians))), (int) (distance * double(sin(angleRadians))));
			//vector->x += distance * double(cos(angleRadians));
			//vector->y +=
		}

		template <class T>
		static Vector2<T> angleToNormalisedVector(double angleDegrees) {
			double angleRadians = toRadians(angleDegrees);
			double x = double(cos(angleRadians));
			double y = double(sin(angleRadians));
			return new Vector2<T>(x, y);
		}
};

#endif /* MATHUTIL_H_ */
