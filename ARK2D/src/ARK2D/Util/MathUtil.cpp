/*
 * MathUtil.cpp
 *
 *  Created on: 25-May-2010
 *      Author: Ashley
 */


#include "MathUtil.h"
#include "Log.h"
#include "../ARK2D.h"

#include "../Core/GameContainer.h"

namespace ARK {
	namespace Util {

		double MathUtil::PIE = 3.14159265;

		void MathUtil::seedRandom() {
			ARK2D::getLog()->i("Setting random seed.");
			//time_t t;
			//time(&t);
			long t = ARK2D::getContainer()->getTimer()->millis();
			srand(t);
			ARK2D::getLog()->i(" ...done.");
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

		double MathUtil::distance(float x1, float y1, float x2, float y2) {
			float diffx = x1 - x2;
			float diffy = y1 - y2;
			return sqrt((diffx * diffx) + (diffy * diffy));
		}

		/**
		 * Includes the lower, but excludes the upper.
		 */
		int MathUtil::randBetween(int lower, int upper) {
			if (lower == upper) { return upper; }
			if (lower > upper) { int teacup = lower; lower = upper; upper = teacup; }
			return lower + (rand() % (upper-lower));
		}


		/**
		 * 0 degrees is directly right.
		 * degrees are measured anti clockwise to 180 degrees is directly left and 90 degrees is directly up.
		 */
		double MathUtil::angle(int x1, int y1, int x2, int y2) {
			return anglef(float(x1), float(y1), float(x2), float(y2));
		}
		double MathUtil::anglef(float x1, float y1, float x2, float y2) {
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

			double angle = atan2(yDifference,xDifference) *(180/PI);
			return angle;
		}




		double MathUtil::toRadians(double angle) {
			return angle * (PI/180);
		}

		double MathUtil::getPercentage(int top, int denominator) {
			return (double) ((float) top / (float) denominator) * 100;
		}
		const char* MathUtil::getPercentage_CSTR(int top, int denominator) {
			int pc = (int) round(MathUtil::getPercentage(top, denominator));
			std::string str = Cast::toString<int>(pc).append(&"%"[0]);
			return str.c_str();
		}
		#if defined(ARK2D_ANDROID)
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
			#if defined(ARK2D_ANDROID)
				return (1 << (int) ceil(Log2AndroidTwo((double)val)));
			#else
				return (1 << (int) ceil(log2((double)val)));
			#endif
		}

	}
}
