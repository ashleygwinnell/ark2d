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
#include "Easing.h"


class Timeline;



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
