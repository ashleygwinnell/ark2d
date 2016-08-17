/*
 * MathUtil.cpp
 *
 *  Created on: 25-May-2010
 *      Author: Ashley
 */


#include "MathUtil.h"
#include "Random.h"

#include "../ARK2D.h"
#include "../GameContainer.h"

namespace ARK {
	namespace Core {
        namespace Math {

            double MathUtil::PIE = 3.14159265;

            unsigned long MathUtil::unixTimestamp() {
                return ARK2D::getContainer()->getTimer()->unixTimestamp();
            }

            void MathUtil::snap(int& snap, int& x, int& y) {
                int snapd2 = (snap/2);
                int xr = (x % snap);
                int yr = (y % snap);
                if (xr < snapd2) {
                    x -= xr;
                } else if (xr >= snapd2) {
                    x -= xr;
                    x += snap;
                }
                if (yr < snapd2) {
                    y -= yr;
                } else if (yr >= snapd2) {
                    y -= yr;
                    y += snap;
                }
            }
            void MathUtil::snap(float& snap, float angle) {
                signed int divisions = round(snap / angle);
                snap = divisions * angle;
            }

            double MathUtil::distance(float x1, float y1, float x2, float y2) {
                float diffx = x1 - x2;
                float diffy = y1 - y2;
                return sqrt((diffx * diffx) + (diffy * diffy));
            }
            double MathUtil::distanceSquared(float x1, float y1, float x2, float y2) {
                float diffx = x1 - x2;
                float diffy = y1 - y2;
                return (diffx * diffx) + (diffy * diffy);
            }
            double MathUtil::distance3d(float x1, float y1, float z1, float x2, float y2, float z2) {
                float xd = x2 - x1;
                float yd = y2 - y1;
                float zd = z2 - z1;
                return sqrt((xd * xd) + (yd * yd) + (zd * zd));
            }
            double MathUtil::distanceSquared3d(float x1, float y1, float z1, float x2, float y2, float z2) {
                float xd = x2 - x1;
                float yd = y2 - y1;
                float zd = z2 - z1;
                return (xd * xd) + (yd * yd) + (zd * zd);
            }

            /**
             * Includes the lower, but excludes the upper.
             */
            void MathUtil::seedRandom() {
                Random::get()->seed();
            }
            int MathUtil::randBetween(int lower, int upper) {
                return Random::get()->randBetween(lower, upper);
            }

            float MathUtil::randBetweenf(float lower, float upper) {
                return Random::get()->randBetweenf(lower, upper);
            }



            /**
             * 0 degrees is directly right.
             * degrees are measured anti clockwise to 180 degrees is directly left and 90 degrees is directly up.
             */
            double MathUtil::angle(int x1, int y1, int x2, int y2) {
                return anglef(float(x1), float(y1), float(x2), float(y2));
            }
            double MathUtil::anglef(float x1, float y1, float x2, float y2) {
                return anglef(x1, y1, x2, y2, false);
            }

            double MathUtil::anglef(float x1, float y1, float x2, float y2, bool enforce_abs) {
                if (x1 == x2 && y1 == y2) {
                    return 0;
                } else if (x1 == x2) {
                    if (y1 > y2) {
                        return 270;
                    } else {
                        return 90;
                    }
                } else if (y1 == y2) {
                    if (x1 > x2) {
                        return 180;
                    } else {
                        return 0;
                    }
                }

                double xDifference = x2 - x1;
                double yDifference = y2 - y1;

                double angle = atan2(yDifference,xDifference) *(180/MY_PI);

                if (enforce_abs && angle < 0) {
                    angle = fmod(angle, 360);
                    angle += 360;
                }

                return angle;
            }
            double MathUtil::forcePositiveAngle(double angle) {
                if (angle < 0) {
                    angle = fmod(angle, 360);
                    angle += 360;
                }
                return angle;
            }

            float MathUtil::averageAngle(float a, float b)
            {
                a = fmod(a,360); // a % 360;
                b = fmod(b,360); // b % 360;

                int sum = a + b;
                if (sum > 360 && sum < 540) {
                    sum = sum % 180;
                }
                return sum / 2;
            }

            float MathUtil::rotateAngleToTarget(float angleStart, float angleTarget, float degrees) {
                return rotateAngleToTarget(angleStart, angleTarget, degrees, false);
            }
            float MathUtil::rotateAngleToTarget(float angleStart, float angleTarget, float degrees, bool restrictOvershoot) {
                //angleTarget = atan2(newVelocityVector.getY(),newVelocityVector.getX()) * (180/pi);


                float diff = angleTarget - angleStart;
                if (diff > 180) diff -= 360;
                if (diff < -180) diff += 360;

                if (diff > -0.05f && diff < 0.05f) { return angleTarget; }


                float multiplier = 0.0f;
                if (diff < 0) {
                    multiplier = -1.0f;
                } else if (diff > 0) {
                    multiplier = 1.0f;
                }

                float returnAngle = angleStart + (degrees*multiplier);

                if (restrictOvershoot) {
                    // make sure we don't go past it.
                    if (multiplier == 1.0f && returnAngle > angleTarget) {
                        returnAngle = angleTarget;
                    } else if (multiplier == -1.0f && returnAngle < angleTarget) {
                        returnAngle = angleTarget;
                    }
                }


                return returnAngle;
            }
            float MathUtil::rotateAngleToTarget2(float angleStart, float angleTarget, float degrees) {

                float antiClockwiseDistance = abs(180 - angleStart) + abs(-180 - angleTarget);
                float clockwiseDistance = angleTarget - angleStart;

                //ARK2D::getLog()->i(StringUtil::appendf("clockwise distance: ", clockwiseDistance));
                //ARK2D::getLog()->i(StringUtil::appendf("anticlockwise distance: ", antiClockwiseDistance));

                if (clockwiseDistance < antiClockwiseDistance) {
                    angleStart += degrees;
                    if (angleStart > angleTarget) {
                        angleStart = angleTarget;
                    }
                } else {
                    angleStart -= degrees;
                    if (angleStart < angleTarget) {
                        angleStart = angleTarget;
                    }
                }
                return angleStart;
            }


            double MathUtil::toRadians(double degrees) {
                return degrees * (MY_PI/180);
            }
            double MathUtil::toDegrees(double radians) {
                return radians * (180.0/MY_PI);
            }

            double MathUtil::getPercentage(int top, int denominator) {
                return (double) ((float) top / (float) denominator) * 100;
            }
            const char* MathUtil::getPercentage_CSTR(int top, int denominator) {
                int pc = (int) round(MathUtil::getPercentage(top, denominator));
                std::string str = Cast::toString<int>(pc).append(&"%"[0]);
                return str.c_str();
            }
            #if (defined(ARK2D_ANDROID) || defined(ARK2D_FLASCC) || defined(ARK2D_WINDOWS_VS))
                unsigned int util_androidlog2( unsigned int x )
                {
                  unsigned int ans = 0 ;
                  while( x>>=1 ) ans++;
                  return ans ;
                }
                // http://weseetips.com/2008/09/10/how-to-calculate-log2-of-any-number/
                double Log2AndroidTwo( double n )
                {
                    // log(n)/log(2) is log2.
                    return log( n ) / log( 2.0 );
                }
            #endif

            int MathUtil::nextPowerOfTwo(int val) {
                #if (defined(ARK2D_ANDROID) || defined(ARK2D_FLASCC) || defined(ARK2D_WINDOWS_VS))
                    return (1 << (int) ceil(Log2AndroidTwo((double)val)));
                #else
                    return (1 << (int) ceil(log2((double)val)));
                #endif
            }
        }
	}
}
