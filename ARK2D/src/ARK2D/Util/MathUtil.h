/*
 * MathUtil.h
 *
 *  Created on: 25-May-2010
 *      Author: Ashley
 */

#ifndef MATHUTIL_H_
#define MATHUTIL_H_

//#include "../ARK2D.h"
#include "../Common/DLL.h"

#include "../Geometry/Vector2.h"
#include "../Geometry/Vector3.h"
#include "Cast.h"

#ifndef MY_PI
	#define MY_PI 3.14159265
#endif

#ifndef PI_OVER_360
	#define PI_OVER_360 0.008726646259972
#endif

#ifndef EPSILON
	#define EPSILON 0.000001f
#endif

namespace ARK { 
	namespace Util {

		/*!
		 * \brief Math utility functions (and also random functions).
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API MathUtil {
			public:
                static double PIE;

				static void seedRandom();
				static int randBetween(int lower, int upper);

				static float randBetweenf(float lower, float upper); 

				static double distance(float x1, float y1, float x2, float y2);
				static double distanceSquared(float x1, float y1, float x2, float y2);
				static double distance3d(float x1, float y1, float z1, float x2, float y2, float z2);
				static double distanceSquared3d(float x1, float y1, float z1, float x2, float y2, float z2);
				static double angle(int x1, int y1, int x2, int y2);
				static double anglef(float x1, float y1, float x2, float y2);
				static double anglef(float x1, float y1, float x2, float y2, bool enforce_abs);
				static double forcePositiveAngle(double angle);
				static double toRadians(double angle);
				static double getPercentage(int top, int denominator);
				static const char* getPercentage_CSTR(int top, int denominator);
				static void snap(int& snap, int& x, int& y);
				static void snap(float& snap, float angle);

				static int nextPowerOfTwo(int val);


				static unsigned long unixTimestamp();

				static float averageAngle(float a, float b);
				static float rotateAngleToTarget(float angleStart, float angleTarget, float degrees);
				static float rotateAngleToTarget(float angleStart, float angleTarget, float degrees, bool restrictOvershoot);
				static float rotateAngleToTarget2(float angleStart, float angleTarget, float degrees);
			
				

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
				static bool isOdd(T number) {
					return number % 2 == 1;
				}

				template <class T>
				static bool isEven(T number) {
					return number % 2 == 0;
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
				static void rotateVectorAroundAxis(Vector3<T>* vec, Vector3<T>* axis, float degreeAngle)
				{
					// Check the preconditions.
					if (vec == NULL || axis == NULL) { return; }
					if (abs(axis->length() - 1) >= EPSILON) { return; }

					rotateVectorAroundAxis(&vec->m_x, &vec->m_y, &vec->m_z, axis->m_x, axis->m_y, axis->m_z, degreeAngle);
				}
				static void rotateVectorAroundAxis(float* vecx, float* vecy, float* vecz, float axisx, float axisy, float axisz, float degreeAngle) {
					
					// Main algorithm
					double radianAngle = degreeAngle * MY_PI/180.0;
					double cosAngle = cos(radianAngle);
					double sinAngle = sin(radianAngle);
					
					float crossX = axisy * *vecz - axisz * *vecy;
					float crossY = axisz * *vecx - axisx * *vecz;
					float crossZ = axisx * *vecy - axisy * *vecx;

					*vecx = *vecx * cosAngle;
					*vecy = *vecy * cosAngle;
					*vecz = *vecz * cosAngle;
					
					*vecx = *vecx + (sinAngle * crossX); 
					*vecy = *vecy + (sinAngle * crossY);
					*vecz = *vecz + (sinAngle * crossZ);

					float dotp = ((axisx * *vecx) + (axisy * *vecy) + (axisz * *vecz));
					float factor = dotp * (1 - cosAngle);
					*vecx = *vecx + (factor * axisx);
					*vecy = *vecy + (factor * axisy);
					*vecz = *vecz + (factor * axisz);
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

				template <class T>
				static void rotatePointAroundPoint(T* pointToRotateX, T* pointToRotateY, T centerOfRotationX, T centerOfRotationY, float angle) {
					double newAngle = MathUtil::toRadians((double) angle);

					(*pointToRotateX) -= centerOfRotationX;
					(*pointToRotateY) -= centerOfRotationY;
					
					T newx = ((*pointToRotateX) * cos(newAngle)) - ((*pointToRotateY) * sin(newAngle));
					T newy = ((*pointToRotateX) * sin(newAngle)) + ((*pointToRotateY) * cos(newAngle));

					(*pointToRotateX) = newx + centerOfRotationX;
					(*pointToRotateY) = newy + centerOfRotationY;
				}

				template <class T>
				static void rotateQuadAroundPoint(T* points, T centerOfRotationX, T centerOfRotationY, float angle) {
					double newAngle = MathUtil::toRadians((double) angle);

					*points   -= centerOfRotationX;
					*(points+1) -= centerOfRotationY;

					*(points+2) -= centerOfRotationX;
					*(points+3) -= centerOfRotationY;
					
					*(points+4) -= centerOfRotationX;
					*(points+5) -= centerOfRotationY;
					
					*(points+6) -= centerOfRotationX;
					*(points+7) -= centerOfRotationY;
					
					*(points+8) -= centerOfRotationX;
					*(points+9) -= centerOfRotationY;
					
					*(points+10) -= centerOfRotationX;
					*(points+11) -= centerOfRotationY;
					
					T tlX = centerOfRotationX + ((*points) * cos(newAngle)) - (*(points+1) * sin(newAngle));
					T tlY = centerOfRotationY + ((*points) * sin(newAngle)) + (*(points+1) * cos(newAngle));

					T trX = centerOfRotationX + (*(points+2) * cos(newAngle)) - (*(points+3) * sin(newAngle));
					T trY = centerOfRotationY + (*(points+2) * sin(newAngle)) + (*(points+3) * cos(newAngle));

					T blX = centerOfRotationX + (*(points+4) * cos(newAngle)) - (*(points+5) * sin(newAngle));
					T blY = centerOfRotationY + (*(points+4) * sin(newAngle)) + (*(points+5) * cos(newAngle));
					
					T brX = centerOfRotationX + (*(points+10) * cos(newAngle)) - (*(points+11) * sin(newAngle));
					T brY = centerOfRotationY + (*(points+10) * sin(newAngle)) + (*(points+11) * cos(newAngle));

					*(points)   = tlX;
					*(points+1) = tlY;

					*(points+2) = trX;
					*(points+3) = trY;

					*(points+4) = blX;
					*(points+5) = blY;
					
					*(points+6) = blX;
					*(points+7) = blY;
					
					*(points+8) = trX;
					*(points+9) = trY;
					
					*(points+10) = brX;
					*(points+11) = brY;
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
					x = T(x + (distance * double(cos(angleRadians))));
					y = T(y + (distance * double(sin(angleRadians))));
				}
				template <class T> 
				static void moveAngleEllipse(T& x, T& y, double angleDegrees, float distanceX, float distanceY) {
					double angleRadians = toRadians(angleDegrees);
					x = T(x + (distanceX * double(cos(angleRadians))));
					y = T(y + (distanceY * double(sin(angleRadians))));
				}

				template <class T>
				static Vector2<T>* angleToNormalisedVector(double angleDegrees) {
					double angleRadians = toRadians(angleDegrees);
					double x = double(cos(angleRadians));
					double y = double(sin(angleRadians));
					return new Vector2<T>(x, y);
				}

				template <class T>
				static void reflectVectorOverCircle(T cx, T cy, T cr, T& vx, T& vy) {

				}

				static bool isVertexConcave(vector<Vector2<float> >* vertices, int vertex)
				{
					Vector2<float>* current = &vertices->at(vertex);
					Vector2<float>* next = &vertices->at((vertex + 1) % vertices->size());
					Vector2<float>* previous = &vertices->at(vertex == 0 ? vertices->size() - 1 : vertex - 1);

					Vector2<float> left(current->getX() - previous->getX(), current->getY() - previous->getY());
					Vector2<float> right(next->getX() - current->getX(), next->getY() - current->getY());

					float cross = (left.getX() * right.getY()) - (left.getY() * right.getX());

					return cross < 0;
				}

				static bool isVertexInsidePolygon(vector<Vector2<float> >* polygon, Vector2<float>* position, bool toleranceOnOutside = true)
				{
					Vector2<float>* point = position;

					const float epsilon = 0.5f;

					bool inside = false;

					// Must have 3 or more edges
					if (polygon->size() < 3) return false;

					Vector2<float>* oldPoint = &polygon->at(polygon->size() - 1);
					float oldSqDist = (float)MathUtil::distanceSquared(oldPoint->getX(), oldPoint->getY(), point->getX(), point->getY());

					for (int i = 0; i < polygon->size(); i++)
					{
						Vector2<float>* newPoint = &polygon->at(i);
						float newSqDist = (float)MathUtil::distanceSquared(newPoint->getX(), newPoint->getY(), point->getX(), point->getY());  //Vector2.DistanceSquared(newPoint, point);

						if (oldSqDist + newSqDist + 2.0f * sqrt(oldSqDist * newSqDist) - float(MathUtil::distanceSquared(newPoint->getX(), newPoint->getY(), oldPoint->getX(), oldPoint->getY())) < epsilon) //  Vector2.DistanceSquared(newPoint, oldPoint) < epsilon)
							return toleranceOnOutside;

						Vector2<float>* left;
						Vector2<float>* right;
						if (newPoint->getX() > oldPoint->getX())
						{
							left = oldPoint;
							right = newPoint;
						}
						else
						{
							left = newPoint;
							right = oldPoint;
						}

						if (left->getX() < point->getX() && point->getX() <= right->getX() && (point->getY() - left->getY()) * (right->getX() - left->getX()) < (right->getY() - left->getY()) * (point->getX() - left->getX()))
							inside = !inside;

						oldPoint = newPoint;
						oldSqDist = newSqDist;
					}

					return inside;
				}

				static bool isInLineOfSight(vector<Vector2<float> >* polygon, Vector2<float>* start, Vector2<float>* end)
				{
					// Not in LOS if any of the ends is outside the polygon
					if (!isVertexInsidePolygon(polygon, start) || !isVertexInsidePolygon(polygon, end)) return false;

					// In LOS if it's the same start and end location
					float epsilon = 0.5f;
					float dist = MathUtil::distance(start->getX(), start->getY(), end->getX(), end->getY());
					if (dist < epsilon) return true;

					// Not in LOS if any edge is intersected by the start-end line segment
					for (int i = 0; i < polygon->size(); i++) {
						if (LineSegmentsCross(start, end, &polygon->at(i), &polygon->at((i + 1) % polygon->size()))) {
							return false;
						}
					}

					// Finally the middle point in the segment determines if in LOS or not
					Vector2<float> middle((start->getX() + end->getX()) * 0.5f, (start->getY() + end->getY()) * 0.5f);
					return isVertexInsidePolygon(polygon, &middle);
				}

				static bool LineSegmentsCross(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy)
				{
					float denominator = ((bx - ax) * (dy - cy)) - ((by - ay) * (dx - cx));

					if (denominator == 0) {
						return false;
					}

					float numerator1 = ((ay - cy) * (dx - cx)) - ((ax - cx) * (dy - cy));

					float numerator2 = ((ay - cy) * (bx - ax)) - ((ax - cx) * (by - ay));

					if (numerator1 == 0 || numerator2 == 0) {
						return false;
					}

					float r = numerator1 / denominator;
					float s = numerator2 / denominator;

					return (r > 0 && r < 1) && (s > 0 && s < 1);
				}
				static bool LineSegmentsCross(Vector2<float>* a, Vector2<float>* b, Vector2<float>* c, Vector2<float>* d)
				{
					return LineSegmentsCross(a->getX(), a->getY(), b->getX(), b->getY(), c->getX(), c->getY(), d->getX(), d->getY());
				}

				static void normalizevec3(float* resultX, float* resultY, float* resultZ, float x1, float y1, float z1) {
					float sqr = x1 * x1 + y1 * y1 + z1 * z1;
					float l = sqrt(sqr);
					*resultX = x1 / l;
					*resultY = y1 / l;
					*resultZ = z1 / l;
					//return x * inversesqrt(sqr); // multiplication is faster?
				}
				static void crossvec3(float* resultX, float* resultY, float* resultZ, float x1, float y1, float z1, float x2, float y2, float z2) {
					*resultX = y1 * z2 - y2 * z1;
					*resultY = z1 * x2 - z2 * x1;
					*resultZ = x1 * y2 - x2 * y1;
				}

				static float dotproductvec3(float x1, float y1, float z1, float x2, float y2, float z2) {
					return (x1 * x2) + (y1 * y2) + (z1 * z2);
				}

				// http://www.ccodechamp.com/c-program-to-find-inverse-of-matrix/
				// http://www.cs.rochester.edu/u/brown/Crypto/assts/projects/adj.html
				static float determinant(void* ap, float k) // a = mat[25][25], k = sz
				{
					float s = 1, det = 0, b[25][25];
					int i, j, m, n, c;

					float (*a)[25] = (float(*)[25]) ap;

					if (k==1) {
						return (a[0][0]);
					} else {
						det = 0;
						for (c = 0; c < k; c++)
						{
							m = 0;
							n = 0;
							for (i = 0; i < k; i++)
							{
								for (j = 0; j < k; j++)
								{
					            	b[i][j] = 0;
									if (i != 0 && j != c)
									{
										b[m][n] = a[i][j];
										if (n < (k-2)) { 
											n++;
										} else {
											n = 0;
											m++;
										}
									}
								}
							}
							det = det + s * (a[0][c] * determinant(b, k-1));
							s = -1 * s;
						}
					}
					return det;
				}

				static float linearInterpolation(float* values, unsigned int length, float k) {

					unsigned int m = length - 1;
					float f = m * k;
					unsigned int i = floor(f);

					if (k < 0) {
						return linear(values[0], values[1], f);
					}

					if (k > 1) {
						return linear(values[m], values[m - 1], m - f);
					}

					return linear(values[i], values[i + 1 > m ? m : i + 1], f - i);
				}


				static float bezierInterpolation(float* values, unsigned int length, float k) {

					float result = 0;
					unsigned int n = length - 1;

					for (unsigned int i = 0; i <= n; ++i) {
                        float fi = float(i);
						result += pow(1 - k, n - fi) * pow(k, fi) * values[i] * bernstein(n, i);
					}

					return result;
				}

				static float catmullRomInterpolation(float* values, unsigned int length, float k)
				{
					unsigned int m = length - 1;
					float f = m * k;
					signed int i = floor(f);

					if (values[0] == values[m]) {
						if (k < 0) {
							i = floor(f = m * (1 + k));
						}
						return catmullRom(values[(i - 1 + m) % m], values[i], values[(i + 1) % m], values[(i + 2) % m], f - i);
					}
					
					if (k < 0) {
						return values[0] - (catmullRom(values[0], values[0], values[1], values[1], -f) - values[0]);
					}

					if (k > 1) {
						return values[m] - (catmullRom(values[m], values[m], values[m - 1], values[m - 1], f - m) - values[m]);
					}

					return catmullRom(values[i ? i - 1 : 0], values[i], values[m < i + 1 ? m : i + 1], values[m < i + 2 ? m : i + 2], f - i);
				}

				static float catmullRom(float p0, float p1, float p2, float p3, float t) 
				{
					float v0 = (p2 - p0) * 0.5;
					float v1 = (p3 - p1) * 0.5;
					float t2 = t * t;
					float t3 = t * t2;
					return (2 * p1 - 2 * p2 + v0 + v1) * t3 + (-3 * p1 + 3 * p2 - 2 * v0 - v1) * t2 + v0 * t + p1;
				}

				static float factorial(signed int value) {
					if (value == 0) {
						return 1;
					}

					signed int res = value;
					while(--value) {
						res *= value;
					}
					return res;
				}
				static float bernstein(signed int n, signed int i) {
        			return factorial(n) / factorial(i) / factorial(n - i);
    			}
    			static float linear(float p0, float p1, float t) {
        			return (p1 - p0) * t + p0;	
    			}
				

		};

	}
}

#endif /* MATHUTIL_H_ */
