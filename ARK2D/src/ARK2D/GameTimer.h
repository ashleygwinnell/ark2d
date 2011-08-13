/*
 * GameTimer.h
 *
 *  Created on: 12-Apr-2010
 *      Author: Ashley
 */

#ifndef GAMETIMER_H_
#define GAMETIMER_H_

#include <time.h>
#include <sys/time.h>
#include <list>

#ifdef _WIN32
	#ifndef ARK2D_WIN32
		#define ARK2D_WIN32
		#include <windows.h>
		#include <mmsystem.h>
		#include <winbase.h>
	#endif
#endif

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
