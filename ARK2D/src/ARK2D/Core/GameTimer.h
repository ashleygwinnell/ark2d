/*
 * GameTimer.h
 *
 *  Created on: 12-Apr-2010
 *      Author: Ashley
 */

#ifndef GAMETIMER_H_
#define GAMETIMER_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#if defined(ARK2D_WINDOWS) || defined(ARK2D_WINDOWS_STORE)
	#include <time.h>
#elif defined(ARK2D_EMSCRIPTEN_JS)
	#include <sys/times.h>
#else
	#include <sys/time.h>
#endif

#include <list>
using std::list;

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

				unsigned long millis();
				float millisf();
				float getLastFrameTime();
				float getCurrentFrameTime();

				float getDeltaModifier();
				void setDeltaModifier(float s);

			private:
				unsigned int m_FrameCount;

				#if defined(ARK2D_WINDOWS_STORE)

					static const uint64_t TicksPerSecond = 10000000;
					LARGE_INTEGER m_qpcFrequency;
					LARGE_INTEGER m_qpcLastTime;
					uint64 m_qpcMaxDelta;

					// Derived timing data uses a canonical tick format.
					uint64 m_elapsedTicks;
					uint64 m_totalTicks;
					uint64 m_leftOverTicks;

					// Get elapsed time since the previous Update call.
					uint64 GetElapsedTicks() const	{ return m_elapsedTicks; }
					double GetElapsedSeconds() const{ return TicksToSeconds(m_elapsedTicks); }

					static float TicksToSeconds(uint64_t ticks){
						return static_cast<float>(ticks) / TicksPerSecond;
					}
					static uint64_t SecondsToTicks(float seconds) {
						return static_cast<uint64_t>(seconds * TicksPerSecond);
					}

					unsigned long m_CurrentTicks;
					unsigned long m_LastTicks;
				#else
					clock_t m_CurrentTicks;
					clock_t m_LastTicks;
				#endif

				std::list<float> m_FrameTimes;
				float m_TimeDelta;
				unsigned int m_FrameRate;
				float m_FrameSecondsElapsed;

				float m_deltaModifier;



		};
	}
}

#endif /* GAMETIMER_H_ */
