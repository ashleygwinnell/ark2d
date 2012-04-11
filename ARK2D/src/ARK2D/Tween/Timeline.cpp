/*
 * Timeline.cpp
 *
 *  Created on: 18-Jun-2010
 *      Author: Ashley
 */

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include "../Core/GameTimer.h"
#include "../UI/ErrorDialog.h"

#include "Timeline.h"

/**
 * TIMELINE
 */

namespace ARK {
	namespace Tween {

		Timeline::Timeline():
			staticEvents(),
			tweenedEvents(),
			keyedStaticEvents(),
			keyedTweenedEvents()
		{

		}

		vector<StaticEvent*> Timeline::getStaticEvents() {
			return staticEvents;
		}
		vector<TweenedEvent*> Timeline::getTweenedEvents() {
			return tweenedEvents;
		}
		map<string, StaticEvent*> Timeline::getKeyedStaticEvents() {
			return keyedStaticEvents;
		}
		map<string, TweenedEvent*> Timeline::getKeyedTweenedEvents() {
			return keyedTweenedEvents;
		}
		void Timeline::printKeyedTweenedEvents() {
			cout << "---- Printing Keyed Tweened Events in Timeline ----" << endl;

			map<string, TweenedEvent*>::const_iterator it = keyedTweenedEvents.begin();
			while(it != keyedTweenedEvents.end() ) {
				cout << "key: " << it->first;
				cout << " val:" << it->second;
				cout << endl;
				it++; //make the iterator point to the next pair in the map
			}
		}

		void Timeline::addIndependentEvent(AnimationEvent* e) {
			addIndependentEvent(e, 0);

		}
		void Timeline::addIndependentEvent(AnimationEvent* e, unsigned int delay_millis) {
			if (e->getType() == AnimationEvent::STATIC)
			{
				StaticEvent* se = static_cast<StaticEvent*>(e);
				se->m_starting_delay = float(delay_millis) / 1000.0f;
				staticEvents.push_back(se);
			}
			else if (e->getType() == AnimationEvent::TWEENED)
			{
				TweenedEvent* te = static_cast<TweenedEvent*>(e);
				te->m_starting_delay = float(delay_millis) / 1000.0f;
				tweenedEvents.push_back(te);

			}


		}
		void Timeline::addKeyedIndependentEvent(string key, AnimationEvent* e) {
			addKeyedIndependentEvent(key, e, 0);
		}
		void Timeline::addKeyedIndependentEvent(string key, AnimationEvent* e, unsigned int delay_millis) {
			if (e->getType() == AnimationEvent::STATIC)
			{
				StaticEvent* se = dynamic_cast<StaticEvent*>(e);
				se->m_starting_delay = float(delay_millis / 1000.0f);
				keyedStaticEvents.insert( pair<string, StaticEvent*>(key, se) );
			}
			else if (e->getType() == AnimationEvent::TWEENED)
			{
				TweenedEvent* te = dynamic_cast<TweenedEvent*>(e);
				te->m_starting_delay = float(delay_millis / 1000.0f);
				keyedTweenedEvents.insert( pair<string, TweenedEvent*>(key, te) );
			}
		}


		void Timeline::update(float delta) {

			for (unsigned int i = 0; i < staticEvents.size(); i++) {
				StaticEvent* event = staticEvents.at(i);
				event->m_starting_delay -= delta;
				if (event->m_starting_delay <= 0) {
					event->invoke(this);
					delete event;
					staticEvents.erase(staticEvents.begin() + i);
				}
			}

			// copy and paste job from above, but with maps!
			map<string, StaticEvent*>::iterator it_static;
			for (it_static = keyedStaticEvents.begin(); it_static != keyedStaticEvents.end(); it_static++ ) {
				StaticEvent* event = it_static->second;
				event->m_starting_delay -= delta;
				if (event->m_starting_delay <= 0) {
					event->invoke(this);
					delete event;
					keyedStaticEvents.erase(it_static);
				}
			}

			for (unsigned int i = 0; i < tweenedEvents.size(); i++) {
				TweenedEvent* event = tweenedEvents.at(i);
				if (event->m_starting_delay >= 0) {
					event->m_starting_delay -= delta;
				} else {
					if (event->m_timer >= event->m_millis) {
						event->complete(this, event->m_end_val);
						delete event;
						tweenedEvents.erase(tweenedEvents.begin() + i);
						continue;
					}

					// Easing contains presets for two-point bezier curves!
					double change = event->m_end_val - event->m_start_val;
					float current_val = (float) Easing::ease(event->m_easing, event->m_timer, event->m_start_val, change, (double) event->m_millis);
					//std::cout << "current val: " << current_val << std::endl;

					event->invoke(this, current_val);
					event->m_timer += (delta * 1000);
				}
			}

			map<string, TweenedEvent*>::iterator it_tweened;
			for (it_tweened = keyedTweenedEvents.begin(); it_tweened != keyedTweenedEvents.end(); it_tweened++ ) {
				TweenedEvent* event = it_tweened->second;
				if (event->m_starting_delay >= 0) {
					event->m_starting_delay -= delta;
				} else {
					if (event->m_timer >= event->m_millis) {
						event->complete(this, event->m_end_val);
						delete event;
						keyedTweenedEvents.erase(it_tweened);
						continue;
					}


					// Easing contains presets for two-point bezier curves!
					double change = event->m_end_val - event->m_start_val;
					float current_val = (float) Easing::ease(event->m_easing, event->m_timer, event->m_start_val, change, (double) event->m_millis);


					event->invoke(this, current_val);
					event->m_timer += (delta * 1000);
				}
			}
		}
		void Timeline::update(GameTimer* timer) {
			update(timer->getDelta());
		}

		unsigned int Timeline::count() {
			return staticEvents.size() + tweenedEvents.size() + keyedStaticEvents.size() + keyedTweenedEvents.size();
		}
		unsigned int Timeline::countRunning() {
			unsigned int count = 0;
			for (unsigned int i = 0; i < tweenedEvents.size(); i++) {
				TweenedEvent* event = tweenedEvents.at(i);
				if (event->m_starting_delay <= 0) {
					count++;
				}
			}

			map<string, TweenedEvent*>::iterator it_tweened;
			for (it_tweened = keyedTweenedEvents.begin(); it_tweened != keyedTweenedEvents.end(); it_tweened++ ) {
				TweenedEvent* event = it_tweened->second;
				if (event->m_starting_delay <= 0) {
					count++;
				}
			}
			//count += staticEvents.size();
			//count += keyedStaticEvents.size();
			return count;
		}

		AnimationEvent* Timeline::getKeyedIndependentEvent(string key) {
			map<string, StaticEvent*>::iterator it_static = keyedStaticEvents.find(key);
			if (it_static != keyedStaticEvents.end()) {
				return it_static->second;
			}

			map<string, TweenedEvent*>::iterator it_tweened = keyedTweenedEvents.find(key);
			if (it_tweened != keyedTweenedEvents.end()) {
				return it_tweened->second;
			}

			return NULL;
		}
		void Timeline::removeKeyedIndependentEvent(string key) {
			map<string, StaticEvent*>::iterator it_static = keyedStaticEvents.find(key);
			if (it_static != keyedStaticEvents.end()) {
				keyedStaticEvents.erase(it_static);
				return;
			}

			map<string, TweenedEvent*>::iterator it_tweened = keyedTweenedEvents.find(key);
			if (it_tweened != keyedTweenedEvents.end()) {
				keyedTweenedEvents.erase(it_tweened);
				return;
			}
		}


		void Timeline::clear() {
			keyedStaticEvents.clear();
			keyedTweenedEvents.clear();
			staticEvents.clear();
			tweenedEvents.clear();
		}


		/**
		 * TWEENED EVENT
		 */

		TweenedEvent::TweenedEvent():
			m_easing(Easing::LINEAR),
			m_start_val(0),
			m_end_val(0),
			m_millis(0),
			m_timer(0) {
			m_type = AnimationEvent::TWEENED;
		}
		TweenedEvent::TweenedEvent(float start_val, float end_val, float millis):
			m_easing(Easing::LINEAR),
			m_start_val(start_val),
			m_end_val(end_val),
			m_millis(millis),
			m_timer(0)
			{
			m_type = AnimationEvent::TWEENED;

		}
		void TweenedEvent::complete(Timeline* t, float end_val) {
			invoke(t, end_val);
		}
		TweenedEvent::~TweenedEvent() { }




		/**
		 * STATIC EVENT
		 */

		StaticEvent::StaticEvent() {
			m_type = AnimationEvent::STATIC;
		}
		StaticEvent::~StaticEvent() { }

	}
}
