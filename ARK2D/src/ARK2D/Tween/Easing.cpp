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

					return -change * (float) cos(time / duration * (MY_PI / 2)) + change + start;

				case Easing::SINE_OUT:
					return change * (float) sin(time / duration * (MY_PI / 2)) + start;

				case Easing::SINE_IN_OUT:
					return -change / 2 * ((float) cos(MY_PI * time / duration) - 1) + start;

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
				
				case Easing::ELASTIC_IN: {
					if (time==0) return start;  
					if ((time/=duration)==1) return start+change;  
					
					double amplitude = 1.0;
					double period = duration * 0.3; //if (!p) p=duration*.3;
					double s = 0;
					if (amplitude < abs(change)) { 
						amplitude = change; 
						s = period / 4; 
					} else { 
						s = period / (2 * MathUtil::PIE) * asin(change/amplitude); 
					}
					return -(amplitude * pow(2, 10 * (time-=1)) * sin( (time*duration-s)*(2* MathUtil::PIE)/period )) + start;
				}
				case Easing::ELASTIC_OUT: {
					if (time==0) return start;  
					if ((time/=duration)==1) return start+change;  
					
					double amplitude = 1.0;
					double period = duration * 0.3; //if (!p) p=duration*.3;
					double s = 0;
					if (amplitude < abs(change)) { 
						amplitude = change; 
						s = period/4; 
					} else { 
						s = period/(2 * MathUtil::PIE) * asin (change/amplitude); 
					}
					return amplitude * pow(2,-10*time) * sin( (time*duration-s)*(2 * MathUtil::PIE)/period ) + change + start;
				}
				case Easing::ELASTIC_IN_OUT: {
					if (time==0) return start;  
					if ((time/=duration/2)==2) return start+change;  
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

					if (time < 1) { 
						return -0.5 * (amplitude * pow(2,10*(time-=1)) * sin( (time*duration-s)*(2*MathUtil::PIE)/period )) + start;
					}
					return amplitude * pow(2,-10*(time-=1)) * sin( (time*duration-s)*(2*MathUtil::PIE)/period )*.5 + change + start;
				}
				case Easing::BOUNCE_IN:
					return change - Easing::ease(Easing::BOUNCE_OUT, duration - time, 0, change, duration) + start;
				case Easing::BOUNCE_OUT:
					if ((time/=duration) < (1/2.75)) {
						return change*(7.5625*time*time) + start;
					} else if (time < (2/2.75)) {
						return change*(7.5625*(time-=(1.5/2.75))*time + .75) + start;
					} else if (time < (2.5/2.75)) {
						return change*(7.5625*(time-=(2.25/2.75))*time + .9375) + start;
					} else {
						return change*(7.5625*(time-=(2.625/2.75))*time + .984375) + start;
					}
					return start;

				case Easing::BOUNCE_IN_OUT:
					if (time < duration/2) { return Easing::ease(Easing::BOUNCE_IN, time*2, 0, change, duration) * 0.5 + start; }
					return Easing::ease(Easing::BOUNCE_OUT, time*2-duration, 0, change, duration) * .5 + change * 0.5 + start;

				case Easing::NONE:
					return start;
				default:
					return start;
			}
		}
	}
}
