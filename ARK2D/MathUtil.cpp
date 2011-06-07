/*
 * MathUtil.cpp
 *
 *  Created on: 25-May-2010
 *      Author: Ashley
 */


#include "MathUtil.h"
#include "OutputWrapper.h"

void MathUtil::seedRandom() {
	OutputWrapper::print("Setting random seed.");
	time_t t;
	time(&t);
	srand(t);
	OutputWrapper::println(" ...done.");
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
