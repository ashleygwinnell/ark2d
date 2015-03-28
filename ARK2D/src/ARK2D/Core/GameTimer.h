/*
 * GameTimer.h
 *
 *  Created on: 12-Apr-2010
 *      Author: Ashley
 */

#ifndef GAMETIMER_H_
#define GAMETIMER_H_

#include "../ARK2D.h"
#include "../Includes.h"

namespace ARK {
	namespace Core {

		/*!
		 * \brief Controls the main game loop.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API GameTimer {

			public:
				GameTimer();
				virtual ~GameTimer();
				void tick();
				unsigned int getFPS() const;
				float getDelta() const;
				float getDeltaNoModifiers() const;
				void flush();
				void sleep(int millis);
				void limit(int fps);

				unsigned long unixTimestamp() const;

				long millis();
				float millisf();
				float getLastFrameTime();
				float getCurrentFrameTime();

				float getDeltaModifier();
				void setDeltaModifier(float s);

			private:
				unsigned int m_FrameCount;
				clock_t m_CurrentTicks;
				clock_t m_LastTicks;
				std::list<float> m_FrameTimes;
				float m_TimeDelta;
				unsigned int m_FrameRate;
				float m_FrameSecondsElapsed;

				float m_deltaModifier;

				//#ifdef ARK2D_WINDOWS_PHONE_8
				//	LARGE_INTEGER m_wp8_frequency;
				//	LARGE_INTEGER m_wp8_currentTime;
				//#endif
				
		};
	}
}

#endif /* GAMETIMER_H_ */
