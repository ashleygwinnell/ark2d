/*
 * Timeline.h
 *
 *  Created on: 31-May-2010
 *      Author: Ashley
 */

#ifndef PI
#define PI 3.14159265
#endif

#ifndef TIMELINE_H_
#define TIMELINE_H_


#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include "GameTimer.h"


class Timeline;

/**
 * Easing - Original by Robert Penner.
 * http://www.robertpenner.com/easing/easing_demo.html
 * file://C:\xampp\htdocs\C++\ARK2D\Docs\Easing.png
 */
class Easing {
	public:
		static const unsigned int LINEAR = 0;
		static const unsigned int QUADRATIC_IN = 1;
		static const unsigned int QUADRATIC_OUT = 2;
		static const unsigned int QUADRATIC_IN_OUT = 3;
		static const unsigned int CUBIC_IN = 4;
		static const unsigned int CUBIC_OUT = 5;
		static const unsigned int CUBIC_IN_OUT = 6;
		static const unsigned int QUARTIC_IN = 7;
		static const unsigned int QUARTIC_OUT = 8;
		static const unsigned int QUARTIC_IN_OUT = 9;
		static const unsigned int SINE_IN = 10;
		static const unsigned int SINE_OUT = 11;
		static const unsigned int SINE_IN_OUT = 12;
		static const unsigned int EXPONENTIAL_IN = 13;
		static const unsigned int EXPONENTIAL_OUT = 14;
		static const unsigned int EXPONENTIAL_IN_OUT = 15;
		static const unsigned int CIRCLE_IN = 16;
		static const unsigned int CIRCLE_OUT = 17;
		static const unsigned int CIRCLE_IN_OUT = 18;
		static const unsigned int NONE = 19;

		static unsigned int getByString(std::string str) {
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

		static double ease(unsigned int easing, double time, double start, double change, double duration)
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

};

class AnimationEvent {
	public:
		static const unsigned int STATIC = 0;
		static const unsigned int TWEENED = 1;
		float m_starting_delay;

		float getCurrentDelay() {
			return m_starting_delay;
		}

		unsigned int getType() {
			return m_type;
		}

		virtual ~AnimationEvent() {}

	protected:
		unsigned int m_type;
};

class StaticEvent : public AnimationEvent {
	public:

	public:
		StaticEvent();
		virtual void invoke(Timeline* t) = 0;
		virtual ~StaticEvent();
};


class TweenedEvent : public AnimationEvent {
	public:
		unsigned int m_easing;
		float m_start_val;
		float m_end_val;
		float m_millis; // millis is the total time.
		float m_timer; // timer is the current time.
	public:
		TweenedEvent();
		TweenedEvent(float start_val, float end_val, float millis);
		virtual void invoke(Timeline* t, float current_val) = 0;
		virtual void complete(Timeline* t, float end_val);
		virtual ~TweenedEvent();

		void setEasing(unsigned int i) {
			m_easing = i;
		}
};


using namespace std;
class Timeline {
	public:

	private:
		vector<StaticEvent*> staticEvents;
		vector<TweenedEvent*> tweenedEvents;

		map<string, StaticEvent*> keyedStaticEvents;
		map<string, TweenedEvent*> keyedTweenedEvents;

	public:
		Timeline();
		vector<StaticEvent*> getStaticEvents();
		vector<TweenedEvent*> getTweenedEvents();
		map<string, StaticEvent*> getKeyedStaticEvents();
		map<string, TweenedEvent*> getKeyedTweenedEvents();
		void printKeyedTweenedEvents();

		void addIndependentEvent(AnimationEvent* e);
		void addIndependentEvent(AnimationEvent* e, unsigned int delay_millis);
		void addKeyedIndependentEvent(string key, AnimationEvent* e);
		void addKeyedIndependentEvent(string key, AnimationEvent* e, unsigned int delay_millis);
		void removeKeyedIndependentEvent(string key);
		void update(float delta);
		void update(GameTimer* timer);
		void pause();
		void unpause();
		void start();
		void reset();
		void clear();
		unsigned int count();
		unsigned int countRunning();

		AnimationEvent* getKeyedIndependentEvent(string key);

		~Timeline();
	private:

};

#endif /* TIMELINE_H_ */
