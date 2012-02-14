/*
 * GameTimer.h
 *
 *  Created on: 12-Apr-2010
 *      Author: Ashley
 */

#ifndef GAMETIMER_H_
#define GAMETIMER_H_

#include "ARK2D.h"

class GameTimer {

	public:
		GameTimer();
		~GameTimer();
		void tick();
		unsigned int getFPS() const;
		float getDelta() const;
		void flush();

	private:
		unsigned int m_FrameCount;
		clock_t m_CurrentTicks;
		clock_t m_LastTicks;
		std::list<float> m_FrameTimes;
		float m_TimeDelta;
		unsigned int m_FrameRate;
		float m_FrameSecondsElapsed;

		long millis();
};

#endif /* GAMETIMER_H_ */
