/*
 * MathUtil.h
 *
 *  Created on: 25-May-2010
 *      Author: Ashley
 */

#ifndef MATHUTIL_H_
#define MATHUTIL_H_

#include "../ARK2D.h"

#include "../Geometry/Vector2.h"
#include "Cast.h"

#ifndef MY_PI
	#define MY_PI 3.14159265
#endif

namespace ARK { 
	namespace Util {

		/*!
		 * \brief Math utility functions (and also random functions).
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class MathUtil {
			public:
                static double PIE;

				static void seedRandom();
				static int randBetween(int lower, int upper);

				static float randBetweenf(float lower, float upper);

				static double distance(float x1, float y1, float x2, float y2);
				static double angle(int x1, int y1, int x2, int y2);
				static double anglef(float x1, float y1, float x2, float y2);
				static double anglef(float x1, float y1, float x2, float y2, bool enforce_abs);
				static double toRadians(double angle);
				static double getPercentage(int top, int denominator);
				static const char* getPercentage_CSTR(int top, int denominator);
				static void snap(int& snap, int& x, int& y);

				static int nextPowerOfTwo(int val);

				static unsigned long unixTimestamp();


				/*
					**  Usage:
					**      bias(b,t)
					**
					**  Arguments:
					**      b       bais, real
					**      t       value, real
					**
					**  Returns:
					**      If (t) varies over the [0,1] interval, then
					**      the result also varies over that interval.
					**      The zero and one endpoints of the interval
					**      are mapped onto themselves. Other values
					**      are shifted upward toward one if (b) is
					**      greater than one, and shifted downward
					**      toward zero if (b) is between zero and one.
					**
					**  GMLscripts.com
					*/
					static float bias(float b, float t)
					{
					    //return power(argument1,ln(argument0)/ln(0.5));
					    return 1.0f;
					}


				template <class T> 
				static Vector2<T>* minY(vector< Vector2<T>* > vecs) {
					Vector2<T>* minObj = NULL;
					T min = std::numeric_limits<T>::max();
					for(unsigned int i = 0; i < vecs.size(); i++) {
						if (vecs.at(i)->getY() < min) {
							min = vecs.at(i)->getY();
							minObj = vecs.at(i);
						}
					}
					return minObj;
				}

				template <class T> 
				static Vector2<T>* maxY(vector< Vector2<T>* > vecs) {
					Vector2<T>* maxObj = NULL;
					T max = std::numeric_limits<T>::min();
					for(unsigned int i = 0; i < vecs.size(); i++) {
						if (vecs.at(i)->getY() > max) {
							max = vecs.at(i)->getY();
							maxObj = vecs.at(i);
						}
					}
					return maxObj;
				}

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
					vector->add(distance * double(cos(angleRadians)), distance * double(sin(angleRadians)));
					//vector->x += distance * double(cos(angleRadians));
					//vector->y +=
				}
				template <class T>
				static void moveAngle(T& x, T& y, double angleDegrees, float distance) {
					double angleRadians = toRadians(angleDegrees);
					x += (distance * double(cos(angleRadians)));
					y += (distance * double(sin(angleRadians)));
				}

				template <class T>
				static Vector2<T>* angleToNormalisedVector(double angleDegrees) {
					double angleRadians = toRadians(angleDegrees);
					double x = double(cos(angleRadians));
					double y = double(sin(angleRadians));
					return new Vector2<T>(x, y);
				}

		};

	}
}

#endif /* MATHUTIL_H_ */
