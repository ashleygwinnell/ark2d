/*
 * GameTimer.cpp
 *
 *  Created on: 12-Apr-2010
 *      Author: Ashley
 */

#include "GameTimer.h"
#include <math.h>

#include "../ARK2D.h"
#include "../Includes.h"

#ifdef _WIN32
	#include <windows.h>
	#include <mmsystem.h>
	#include <winbase.h> 
#endif

namespace ARK {
	namespace Core {

		GameTimer::GameTimer():
			m_FrameCount( 0 ),
			m_CurrentTicks( 0 ),
			m_LastTicks( 0 ),
			m_FrameTimes(),
			m_TimeDelta( 0.0f ),
			m_FrameRate( 0 ),
			m_FrameSecondsElapsed( 0.0f ) {

		}


		GameTimer::~GameTimer() { 

		}

		/*
			#ifdef __APPLE__
				static UInt64 start = 0;
				if (start==0)
				{
					Microseconds((UnsignedWide*)&start);
					return 0.0f;
				}

				UInt64 counter = 0;
				Microseconds((UnsignedWide*)&counter);
				return (counter - start) / 1000000.0f; 
			#endif
		*/

		unsigned long GameTimer::unixTimestamp() const {
			return (unsigned long) std::time(NULL);
		}
 
		long GameTimer::millis() {
			#if defined(ARK2D_IPHONE)
				timeval now; 
				gettimeofday(&now, NULL); 
				return (now.tv_sec + now.tv_usec); 
			#elif defined(ARK2D_ANDROID)
				return clock();
			#elif defined(ARK2D_FLASCC)
				return clock();
			#elif defined(ARK2D_WINDOWS)
				return clock();
			#elif defined(ARK2D_MACINTOSH)
				timeval now;
				gettimeofday(&now, NULL);
				return (now.tv_sec + now.tv_usec);
			#endif 
			return 0;
		}
 
		void GameTimer::sleep(int millis) {
			#if defined(ARK2D_WINDOWS)
				Sleep(millis);
			#elif defined(ARK2D_MACINTOSH)
				usleep(millis * 1000);
			#elif defined(ARK2D_IPHONE)
				//usleep(double(millis)/1000);
			#else
				// ... log not implemented
			#endif
		}

		// call at the end of the rendering to sleep for the correct time. :O
		void GameTimer::limit(int fps) {
			/*float perframesecs = 1000.0f / float(fps);
			if (m_TimeDelta < perframesecs) {
				m_TimeDelta = perframesecs;
				return;
			}*/


			int perframemax = (int) floor(1000.0f/float(fps));
			int diffmillis = millis() - m_CurrentTicks;
			if (diffmillis < perframemax) {
				sleep(perframemax - diffmillis);
			}
		}

		/*!
		 * Signifies a new frame in the game.
		 */
		void GameTimer::tick() {

			#ifdef ARK2D_FLASCC
				return;
			#endif

			// Get the number of ticks passed since program launch.
			m_CurrentTicks = millis(); //clock();

			// Calculate elapsed time in seconds (usually ends up being a fraction of a second).
			float secondsElapsed = (m_CurrentTicks - m_LastTicks) / static_cast<float>(CLOCKS_PER_SEC);

			m_LastTicks = m_CurrentTicks;

			// Weed out any values that are too different than the average.
			if ( fabsf( secondsElapsed - m_TimeDelta ) < 1.0f )
			{
				// Store the frame times in the queue.
				m_FrameTimes.push_back( secondsElapsed );

				if ( m_FrameTimes.size() > 100 )
					m_FrameTimes.pop_front();
			}

			m_FrameCount++;
			m_FrameSecondsElapsed += secondsElapsed;
			if ( m_FrameSecondsElapsed > 1.0f )
			{
				m_FrameRate = m_FrameCount;
				m_FrameCount = 0;
				m_FrameSecondsElapsed = 0.0f;
			}

			m_TimeDelta = 0.0f;
			std::list<float>::iterator it;
			for ( it = m_FrameTimes.begin(); it != m_FrameTimes.end(); it++ )
				m_TimeDelta += *it;

			if ( m_FrameTimes.size() > 0 )
				m_TimeDelta /= static_cast<float>(m_FrameTimes.size());

		}

		unsigned int GameTimer::getFPS() const {
			return m_FrameRate;
		}


		/*!
		 * Returns the average amount of seconds that have passed
		 */
		float GameTimer::getDelta() const {
			return m_TimeDelta;
		}

		void GameTimer::flush() {
			m_LastTicks = millis(); // clock();
			m_CurrentTicks = millis(); //clock();
			m_TimeDelta = 0.017f;

			m_FrameRate = 0;
			m_FrameCount = 0;
			m_FrameSecondsElapsed = 0.0f;

			m_FrameTimes.clear();
		}

	}
}
