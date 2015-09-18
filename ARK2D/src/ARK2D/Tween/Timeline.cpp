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
			keyedTweenedEvents(),
			tweenValues()
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
			ARK2D::getLog()->v("---- Printing Keyed Tweened Events in Timeline ----");

			map<string, TweenedEvent*>::const_iterator it = keyedTweenedEvents.begin();
			while(it != keyedTweenedEvents.end() ) {
				//cout << "key: " << it->first;
				//cout << " val:" << it->second;
				//cout << endl;
				ARK2D::getLog()->i(String("key: ").append(it->first).get()); //.append(" val:").append(it->second).get());
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

			// Static events.
			for (unsigned int i = 0; i < staticEvents.size(); ++i) {
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

			// tween events
			for (unsigned int i = 0; i < tweenedEvents.size(); ++i) {
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


			// Values inc. vector3 and double
			vector<TweenValue*>::iterator it_values;
			for (it_values = tweenValues.begin(); it_values != tweenValues.end(); ) {
				TweenValue* obj = *it_values;
				obj->update();
				
				if (obj->timer >= obj->duration) {
					ARK2D::getLog()->e("TweenValue erased");
					delete obj;
					it_values = tweenValues.erase(it_values);
				} else {
					++it_values;
				}
			}

		}
		void Timeline::update() {
			update(ARK2D::getTimer()->getDelta());
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


		void Timeline::reset() {
			clear();
		}
		void Timeline::clear() {
			keyedStaticEvents.clear();
			keyedTweenedEvents.clear();
			staticEvents.clear();
			tweenedEvents.clear();
		}
		Timeline::~Timeline() {
			
		}

		// Tween Values
		TweenValue::TweenValue() {

		}
		bool TweenValue::isRunning() {
			return (timer >= 0.0f && timer <= duration);
		}	
		void TweenValue::update() {
			timer += ARK2D::getTimer()->getDelta();
			if (timer >= duration) {
				timer = duration;
			}
		}
		string TweenValue::toString() { 
			return "TweenValue todo"; 
		} 
		TweenValue::~TweenValue() {

		}

		// Tween Vectors
		void Timeline::tweenVector3(Vector3<float>* obj, float x, float y, float z, float duration, unsigned int easing, float delay) {
			TweenVector3Object* tween = new TweenVector3Object();
			tween->obj = obj;
			tween->fromX = obj->getX();
			tween->fromY = obj->getY();
			tween->fromZ = obj->getZ();
			tween->toX = x;
			tween->toY = y;
			tween->toZ = z;
			tween->timer = delay * -1.0f;
			tween->duration = duration;
			tween->easing = easing;

			tweenValues.push_back(tween);
		}
		TweenVector3Object::TweenVector3Object():TweenValue() {

		}
		void TweenVector3Object::update() {
			TweenValue::update();
			if (timer >= 0.0f) { 
				obj->setX( Easing::easebetween(easing, timer, fromX, toX, duration) );
				obj->setY( Easing::easebetween(easing, timer, fromY, toY, duration) );
				obj->setZ( Easing::easebetween(easing, timer, fromZ, toZ, duration) );
			}
		} 
		string TweenVector3Object::toString() {
			string s = "{";
			s += "\"obj\":"; s += obj->toString(); s += ", ";
			s += "\"fromX\":"; s += Cast::toString<float>(fromX); s += ", ";
			s += "\"fromY\":"; s += Cast::toString<float>(fromY); s += ", ";
			s += "\"fromZ\":"; s += Cast::toString<float>(fromZ); s += ", ";
			s += "\"toX\":"; s += Cast::toString<float>(toX); s += ", ";
			s += "\"toY\":"; s += Cast::toString<float>(toY); s += ", ";
			s += "\"toZ\":"; s += Cast::toString<float>(toZ); s += ", ";
			s += "\"timer\":"; s += Cast::toString<float>(timer); s += ", ";
			s += "\"duration\":"; s += Cast::toString<float>(duration); s += ", ";
			s += "\"easing\":"; s += Cast::toString<unsigned int>(easing);
			s += "}";
			return s;
		}
		TweenVector3Object::~TweenVector3Object() {

		}

		// Tween Vector2s
		void Timeline::tweenVector2(Vector2<float>* obj, float x, float y, float duration, unsigned int easing, float delay) {
			TweenVector2Object* tween = new TweenVector2Object();
			tween->obj = obj;
			tween->fromX = obj->getX();
			tween->fromY = obj->getY();
			tween->toX = x;
			tween->toY = y;
			tween->timer = delay * -1.0f;
			tween->duration = duration;
			tween->easing = easing;
			tweenValues.push_back(tween);
		}
		TweenVector2Object::TweenVector2Object():TweenValue() {

		}
		void TweenVector2Object::update() {
			TweenValue::update();
			if (timer >= 0.0f) { 
				obj->setX( Easing::easebetween(easing, timer, fromX, toX, duration) );
				obj->setY( Easing::easebetween(easing, timer, fromY, toY, duration) );
			}
		} 
		string TweenVector2Object::toString() {
			string s = "{"; 
			s += "\"obj\":"; s += obj->toString(); s += ", ";
			s += "\"fromX\":"; s += Cast::toString<float>(fromX); s += ", ";
			s += "\"fromY\":"; s += Cast::toString<float>(fromY); s += ", ";
			s += "\"toX\":"; s += Cast::toString<float>(toX); s += ", ";
			s += "\"toY\":"; s += Cast::toString<float>(toY); s += ", ";
			s += "\"timer\":"; s += Cast::toString<float>(timer); s += ", ";
			s += "\"duration\":"; s += Cast::toString<float>(duration); s += ", ";
			s += "\"easing\":"; s += Cast::toString<unsigned int>(easing);
			s += "}";
			return s;
		}
		TweenVector2Object::~TweenVector2Object() {

		}
		
		// Tween doubles.
		void Timeline::tweenDouble(double* obj, double d, float duration, unsigned int easing, float delay) {
			TweenDoubleValue* tween = new TweenDoubleValue();
			tween->obj = obj;
			tween->fromValue = *obj;
			tween->toValue = d;
			tween->timer = delay * -1.0f;
			tween->duration = duration;
			tween->easing = easing;

			tweenValues.push_back(tween);
		}
		TweenDoubleValue::TweenDoubleValue():TweenValue(){

		}
		void TweenDoubleValue::update() {
			TweenValue::update();
			if (timer >= 0.0f) { 
				(*obj) = Easing::easebetween(easing, timer, fromValue, toValue, duration);
			}
		}
		string TweenDoubleValue::toString() {
			return "TweenDoubleValue todo";
		}
		TweenDoubleValue::~TweenDoubleValue() {

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

		// Static event with Function Pointer
		FPStaticEvent::FPStaticEvent(void* fp, float delay):
			StaticEvent(),
			m_fp(fp) 
		{
			m_starting_delay = delay;
		}
		void FPStaticEvent::invoke(Timeline* t) {
			void (*pt)() = (void(*)()) m_fp;
			pt();
		}
		FPStaticEvent::~FPStaticEvent() {

		}

	}
}
