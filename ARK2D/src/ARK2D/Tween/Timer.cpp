/*
 * Thread.cpp
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#include "../ARK2D.h"
#include "../Core/GameContainer.h"
#include "Timer.h"

namespace ARK {
	namespace Tween {

		Timer::Timer(float duration):
			m_timer(0.0f),
			m_duration(duration) {

		}
		void Timer::limit() {
			limit(1);
		}
		void Timer::limit(int i) {
			if (m_timer > (m_duration * i)) {
				m_timer = (m_duration * i);
			}
		}
		void Timer::setTime(float t) {
			m_timer = t;
		}
		float Timer::getTime() {
			return m_timer;
		}
		void Timer::setDuration(float duration) {
			m_duration = duration;
		}
		float Timer::getDuration() {
			return m_duration;
		}
		float Timer::percentage() {
			return m_timer / m_duration;
		}
		bool Timer::update() {
			return update(1.0f);
		}
		bool Timer::update(float modifier) {
			GameTimer* t = ARK2D::getContainer()->getTimer();
			m_timer += (t->getDelta() * modifier);
			if (m_timer >= m_duration) {
				return true;
			}
			return false;
		}
		void Timer::loop() {
			m_timer -= m_duration;
		}
		void Timer::reset() {
			m_timer = 0.0f;
		}
		string Timer::toString() {
			string s = "{";
			s += "\"timer\":";
			s += Cast::toString<float>(m_timer);
			s += ",";
			s += "\"duration\":";
			s += Cast::toString<float>(m_duration);
			s += "}";
			return s;
		}
		Timer::~Timer() {

		}

	}
}
