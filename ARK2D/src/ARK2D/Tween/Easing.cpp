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
			} else if (str == "ELASTIC_IN") {
				return ELASTIC_IN;
			} else if (str == "ELASTIC_OUT") {
				return ELASTIC_OUT;
			} else if (str == "ELASTIC_IN_OUT") {
				return ELASTIC_IN_OUT;
			} else if (str == "BOUNCE_IN") {
				return BOUNCE_IN;
			} else if (str == "BOUNCE_OUT") {
				return BOUNCE_OUT;
			} else if (str == "BOUNCE_IN_OUT") {
				return BOUNCE_IN_OUT;
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
			} else if (i == ELASTIC_IN) {
				return "ELASTIC_IN";
			} else if (i == ELASTIC_OUT) {
				return "ELASTIC_OUT";
			} else if (i == ELASTIC_IN_OUT) {
				return "ELASTIC_IN_OUT";
			} else if (i == BOUNCE_IN) {
				return "BOUNCE_IN";
			} else if (i == BOUNCE_OUT) {
				return "BOUNCE_OUT";
			} else if (i == BOUNCE_IN_OUT) {
				return "BOUNCE_IN_OUT";
			}
			return "LINEAR";
		}
		double Easing::easebetween(unsigned int easing, double t, double start, double end, double duration) {
			return ease(easing, t, start, end - start, duration);
		}

		double Easing::ease(unsigned int easing, double t, double start, double change, double duration)
		{
			if (t >= duration) {
				return start + change;
			}
			switch(easing) {
				case Easing::LINEAR:
					return change * t / duration + start;

				case Easing::QUADRATIC_IN:
					return change * (t /= duration) * t + start;

				case Easing::QUADRATIC_OUT:
					return -change * (t /= duration) * (t - 2) + start;

				case Easing::QUADRATIC_IN_OUT:
					if ((t /= duration / 2) < 1) {
						return change / 2 * t * t + start;
					}
					return -change / 2 * ((--t) * (t - 2) - 1) + start;

				case Easing::CUBIC_IN:
					return change * (t /= duration) * t * t + start;

				case Easing::CUBIC_OUT:
					return change * ((t = t / duration - 1) * t * t + 1) + start;

				case Easing::CUBIC_IN_OUT:
					if ((t /= duration / 2) < 1) {
						return change / 2 * t * t * t + start;
					}
					return change / 2 * ((t -= 2) * t * t + 2) + start;

				case Easing::QUARTIC_IN:
					return change * (t /= duration) * t * t * t + start;

				case Easing::QUARTIC_OUT:
					return -change * ((t = t / duration - 1) * t * t * t - 1) + start;

				case Easing::QUARTIC_IN_OUT:
					if ((t /= duration / 2) < 1) {
						return change / 2 * t * t * t * t + start;
					}
					return -change / 2 * ((t -= 2) * t * t * t - 2) + start;

				case Easing::SINE_IN:

					return -change * (float) cos(t / duration * (MY_PI / 2)) + change + start;

				case Easing::SINE_OUT:
					return change * (float) sin(t / duration * (MY_PI / 2)) + start;

				case Easing::SINE_IN_OUT:
					return -change / 2 * ((float) cos(MY_PI * t / duration) - 1) + start;

				case Easing::EXPONENTIAL_IN:
					return (t==0) ? start : change * (float)pow(2, 10 * (t/duration - 1)) + start;

				case Easing::EXPONENTIAL_OUT:
					return (t==duration) ? start+change : change * (-(float)pow(2, -10 * t/duration) + 1) + start;

				case Easing::EXPONENTIAL_IN_OUT:
					if (t==0) return start;
					if (t==duration) return start+change;
					if ((t/=duration/2) < 1) return change/2 * (float)pow(2, 10 * (t - 1)) + start;
					return change/2 * (-(float)pow(2, -10 * --t) + 2) + start;

				case Easing::CIRCLE_IN:
					return -change * ((float)sqrt(1 - (t/=duration) * t) - 1) + start;

				case Easing::CIRCLE_OUT:
					return change * (float)sqrt(1 - (t=t/duration-1) * t) + start;

				case Easing::CIRCLE_IN_OUT:
					if ((t/=duration/2) < 1) return -change/2 * ((float)sqrt(1 - t*t) - 1) + start;
					return change/2 * ((float)sqrt(1 - (t-=2)*t) + 1) + start;
				
				case Easing::ELASTIC_IN: {
					if (t==0) return start;
					if ((t/=duration)==1) return start+change;
					
					double amplitude = 1.0;
					double period = duration * 0.3; //if (!p) p=duration*.3;
					double s = 0;
					if (amplitude < abs(change)) { 
						amplitude = change; 
						s = period / 4; 
					} else { 
						s = period / (2 * MathUtil::PIE) * asin(change/amplitude); 
					}
					return -(amplitude * pow(2, 10 * (t-=1)) * sin( (t*duration-s)*(2* MathUtil::PIE)/period )) + start;
				}
				case Easing::ELASTIC_OUT: {
					if (t==0) return start;
					if ((t/=duration)==1) return start+change;
					
					double amplitude = 1.0;
					double period = duration * 0.3; //if (!p) p=duration*.3;
					double s = 0;
					if (amplitude < abs(change)) { 
						amplitude = change; 
						s = period/4; 
					} else { 
						s = period/(2 * MathUtil::PIE) * asin (change/amplitude); 
					}
					return amplitude * pow(2,-10*t) * sin( (t*duration-s)*(2 * MathUtil::PIE)/period ) + change + start;
				}
				case Easing::ELASTIC_IN_OUT: {
					if (t==0) return start;
					if ((t/=duration/2)==2) return start+change;
					//if (!p) p=d*(.3*1.5); 
					double amplitude = 1.0;
					double period = duration*(0.3*1.5);
					double s = 0;

					if (amplitude < abs(change)) { 
						amplitude=change; 
						s = period/4; 
					} else { 
						s = period/(2 * MathUtil::PIE) * asin (change/amplitude); 
					}

					if (t < 1) {
						return -0.5 * (amplitude * pow(2,10*(t-=1)) * sin( (t*duration-s)*(2*MathUtil::PIE)/period )) + start;
					}
					return amplitude * pow(2,-10*(t-=1)) * sin( (t*duration-s)*(2*MathUtil::PIE)/period )*.5 + change + start;
				}
				case Easing::BOUNCE_IN:
					return change - Easing::ease(Easing::BOUNCE_OUT, duration - t, 0, change, duration) + start;
				case Easing::BOUNCE_OUT:
					if ((t/=duration) < (1/2.75)) {
						return change*(7.5625*t*t) + start;
					} else if (t < (2/2.75)) {
						return change*(7.5625*(t-=(1.5/2.75))*t + .75) + start;
					} else if (t < (2.5/2.75)) {
						return change*(7.5625*(t-=(2.25/2.75))*t + .9375) + start;
					} else {
						return change*(7.5625*(t-=(2.625/2.75))*t + .984375) + start;
					}
					return start;

				case Easing::BOUNCE_IN_OUT:
					if (t < duration/2) { return Easing::ease(Easing::BOUNCE_IN, t*2, 0, change, duration) * 0.5 + start; }
					return Easing::ease(Easing::BOUNCE_OUT, t*2-duration, 0, change, duration) * .5 + change * 0.5 + start;

				case Easing::NONE:
					return start;
				default:
					return start;
			}
		}
	}
}
