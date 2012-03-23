/*
 * Easing.cpp
 *
 *  Created on: 5 Jul 2011
 *      Author: Ashley
 */

#include "Easing.h"

namespace ARK {
	namespace Tween {

		unsigned int Easing::getByString(string str) {
			if (str == "LINEAR") {
				return LINEAR;
			} else if (str == "QUADRATIC_IN") {
				return QUADRATIC_IN;
			} else if (str == "QUADRATIC_OUT") {
				return QUADRATIC_OUT;
			} else if (str == "QUADRATIC_IN_OUT") {
				return QUADRATIC_IN_OUT;
			} else if (str == "CUBIC_IN") {
				return CUBIC_IN;
			} else if (str == "CUBIC_OUT") {
				return CUBIC_OUT;
			} else if (str == "CUBIC_IN_OUT") {
				return CUBIC_IN_OUT;
			} else if (str == "QUARTIC_IN") {
				return QUARTIC_IN;
			} else if (str == "QUARTIC_OUT") {
				return QUARTIC_OUT;
			} else if (str == "QUARTIC_IN_OUT") {
				return QUARTIC_IN_OUT;
			} else if (str == "SINE_IN") {
				return SINE_IN;
			} else if (str == "SINE_OUT") {
				return SINE_OUT;
			} else if (str == "SINE_IN_OUT") {
				return SINE_IN_OUT;
			} else if (str == "EXPONENTIAL_IN") {
				return EXPONENTIAL_IN;
			} else if (str == "EXPONENTIAL_OUT") {
				return EXPONENTIAL_OUT;
			} else if (str == "EXPONENTIAL_IN_OUT") {
				return EXPONENTIAL_IN_OUT;
			} else if (str == "CIRCLE_IN") {
				return CIRCLE_IN;
			} else if (str == "CIRCLE_OUT") {
				return CIRCLE_OUT;
			} else if (str == "CIRCLE_IN_OUT") {
				return CIRCLE_IN_OUT;
			}
			return LINEAR;
		}

		string Easing::getByInt(unsigned int i) {
			if (i == LINEAR) {
				return "LINEAR";
			} else if (i == QUADRATIC_IN) {
				return "QUADRATIC_IN";
			} else if (i == QUADRATIC_OUT) {
				return "QUADRATIC_OUT";
			} else if (i == QUADRATIC_IN_OUT) {
				return "QUADRATIC_IN_OUT";
			} else if (i == CUBIC_IN) {
				return "CUBIC_IN";
			} else if (i == CUBIC_OUT) {
				return "CUBIC_OUT";
			} else if (i == CUBIC_IN_OUT) {
				return "CUBIC_IN_OUT";
			} else if (i == QUARTIC_IN) {
				return "QUARTIC_IN";
			} else if (i == QUARTIC_OUT) {
				return "QUARTIC_OUT";
			} else if (i == QUARTIC_IN_OUT) {
				return "QUARTIC_IN_OUT";
			} else if (i == SINE_IN) {
				return "SINE_IN";
			} else if (i == SINE_OUT) {
				return "SINE_OUT";
			} else if (i == SINE_IN_OUT) {
				return "SINE_IN_OUT";
			} else if (i == EXPONENTIAL_IN) {
				return "EXPONENTIAL_IN";
			} else if (i == EXPONENTIAL_OUT) {
				return "EXPONENTIAL_OUT";
			} else if (i == EXPONENTIAL_IN_OUT) {
				return "EXPONENTIAL_IN_OUT";
			} else if (i == CIRCLE_IN) {
				return "CIRCLE_IN";
			} else if (i == CIRCLE_OUT) {
				return "CIRCLE_OUT";
			} else if (i == CIRCLE_IN_OUT) {
				return "CIRCLE_IN_OUT";
			}
			return LINEAR;
		}


		double Easing::ease(unsigned int easing, double time, double start, double change, double duration)
		{
			if (time >= duration) {
				return start + change;
			}
			switch(easing) {
				case Easing::LINEAR:
					return change * time / duration + start;

				case Easing::QUADRATIC_IN:
					return change * (time /= duration) * time + start;

				case Easing::QUADRATIC_OUT:
					return -change * (time /= duration) * (time - 2) + start;

				case Easing::QUADRATIC_IN_OUT:
					if ((time /= duration / 2) < 1) {
						return change / 2 * time * time + start;
					}
					return -change / 2 * ((--time) * (time - 2) - 1) + start;

				case Easing::CUBIC_IN:
					return change * (time /= duration) * time * time + start;

				case Easing::CUBIC_OUT:
					return change * ((time = time / duration - 1) * time * time + 1) + start;

				case Easing::CUBIC_IN_OUT:
					if ((time /= duration / 2) < 1) {
						return change / 2 * time * time * time + start;
					}
					return change / 2 * ((time -= 2) * time * time + 2) + start;

				case Easing::QUARTIC_IN:
					return change * (time /= duration) * time * time * time + start;

				case Easing::QUARTIC_OUT:
					return -change * ((time = time / duration - 1) * time * time * time - 1) + start;

				case Easing::QUARTIC_IN_OUT:
					if ((time /= duration / 2) < 1) {
						return change / 2 * time * time * time * time + start;
					}
					return -change / 2 * ((time -= 2) * time * time * time - 2) + start;

				case Easing::SINE_IN:

					return -change * (float) cos(time / duration * (PI / 2)) + change + start;

				case Easing::SINE_OUT:
					return change * (float) sin(time / duration * (PI / 2)) + start;

				case Easing::SINE_IN_OUT:
					return -change / 2 * ((float) cos(PI * time / duration) - 1) + start;

				case Easing::EXPONENTIAL_IN:
					return (time==0) ? start : change * (float)pow(2, 10 * (time/duration - 1)) + start;

				case Easing::EXPONENTIAL_OUT:
					return (time==duration) ? start+change : change * (-(float)pow(2, -10 * time/duration) + 1) + start;

				case Easing::EXPONENTIAL_IN_OUT:
					if (time==0) return start;
					if (time==duration) return start+change;
					if ((time/=duration/2) < 1) return change/2 * (float)pow(2, 10 * (time - 1)) + start;
					return change/2 * (-(float)pow(2, -10 * --time) + 2) + start;

				case Easing::CIRCLE_IN:
					return -change * ((float)sqrt(1 - (time/=duration) * time) - 1) + start;

				case Easing::CIRCLE_OUT:
					return change * (float)sqrt(1 - (time=time/duration-1) * time) + start;

				case Easing::CIRCLE_IN_OUT:
					if ((time/=duration/2) < 1) return -change/2 * ((float)sqrt(1 - time*time) - 1) + start;
					return change/2 * ((float)sqrt(1 - (time-=2)*time) + 1) + start;
				case Easing::NONE:
					return start;
				default:
					return start;
			}
		}
	}
}
