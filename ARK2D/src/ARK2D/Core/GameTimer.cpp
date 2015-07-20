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
#include "../Util/Log.h"

#ifdef ARK2D_WINDOWS
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
			m_FrameSecondsElapsed( 0.0f ),
			m_deltaModifier(1.0f) 
		{
			/*#ifdef ARK2D_WINDOWS_PHONE_8
				if (!QueryPerformanceFrequency(&m_wp8_frequency)) {
					ARK2D::getLog()->e("Could not get GameTimer Frequency");
					exit(0);
				}
			#endif*/
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

		float GameTimer::getDeltaModifier() {
			return m_deltaModifier;
		}
		void GameTimer::setDeltaModifier(float s) {
			m_deltaModifier = s;
		}

		unsigned long GameTimer::unixTimestamp() const {
			#if defined(__GNUC__) && !defined(ARK2D_UBUNTU_LINUX)
				return (unsigned long) std::time(NULL);
			#else
				return (unsigned long) time(NULL);
			#endif
		}
 
		long GameTimer::millis() {
			#if defined(ARK2D_IPHONE)
				timeval now; 
				gettimeofday(&now, NULL); 
				return (now.tv_sec + now.tv_usec); 
			#elif defined(ARK2D_ANDROID)
				//return clock();
				struct timespec res;
    			clock_gettime(CLOCK_REALTIME, &res); //  CLOCK_REALTIME / CLOCK_PROCESS_CPUTIME_ID
    			return (res.tv_sec * 1000000000 + res.tv_nsec) / 1000;
			#elif defined(ARK2D_UBUNTU_LINUX)
				//return clock();
 
				//struct timeval now; 
				//gettimeofday(&now, NULL); 
				//return (now.tv_sec + now.tv_usec);
 
				//struct timespec res;
    			//clock_gettime(CLOCK_REALTIME, &res);
    			//return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
    			//return (res.tv_sec * 1000000000 + res.tv_nsec);

    			struct timespec res;
    			clock_gettime(CLOCK_REALTIME, &res); //  CLOCK_REALTIME / CLOCK_PROCESS_CPUTIME_ID
    			return (res.tv_sec * 1000000000 + res.tv_nsec) / 1000;
 
			#elif defined(ARK2D_FLASCC)
				return clock();
			#elif defined(ARK2D_EMSCRIPTEN_JS)
				struct timespec res;
    			clock_gettime(CLOCK_REALTIME, &res); //  CLOCK_REALTIME / CLOCK_PROCESS_CPUTIME_ID
    			return (res.tv_sec * 1000000000 + res.tv_nsec) / 1000;
			#elif defined(ARK2D_WINDOWS)
				return clock();
			#elif defined(ARK2D_WINDOWS_PHONE_8)
				/*if (!QueryPerformanceCounter(&m_wp8_currentTime)) {
					ARK2D::getLog()->e("Could not get GameTimer millis.");
				}
				m_wp8_currentTime.
				return m_wp8_currentTime.QuadPart;*/
				
				return clock();
			#elif defined(ARK2D_MACINTOSH)
				timeval now; 
				gettimeofday(&now, NULL);
				return (now.tv_sec + now.tv_usec);


			#endif 
			return 0;
		}
		float GameTimer::millisf() {

			// TODO: platform-dependent code...
			return 0.0f; // millis() / static_cast<float>(CLOCKS_PER_SEC);
		}
 
		void GameTimer::sleep(int millis) {
			#if defined(ARK2D_WINDOWS)
				Sleep(millis);
			#elif defined(ARK2D_MACINTOSH)
				usleep(millis * 1000); 
			#elif defined(ARK2D_FLASCC)
				usleep(millis * 1000);
			#elif defined(ARK2D_UBUNTU_LINUX)
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

		float GameTimer::getLastFrameTime() { 
			return m_LastTicks / static_cast<float>(CLOCKS_PER_SEC); 
		}
		float GameTimer::getCurrentFrameTime() {  
			return m_CurrentTicks / static_cast<float>(CLOCKS_PER_SEC); 
		}

		/*!
		 * Signifies a new frame in the game.
		 */
		void GameTimer::tick() {

			//#ifdef ARK2D_FLASCC
			//	return;
			//#endif

			// Get the number of ticks passed since program launch.
			

			// Calculate elapsed time in seconds (usually ends up being a fraction of a second).
			#ifdef ARK2D_WINDOWS_PHONE_8
				//m_CurrentTicks = (double) millis(); //clock();
			//	float secondsElapsed = ((double)m_CurrentTicks - (double)m_LastTicks) / static_cast<float>(CLOCKS_PER_SEC);
				m_CurrentTicks = millis(); //clock();
				float secondsElapsed = (m_CurrentTicks - m_LastTicks) / static_cast<float>(CLOCKS_PER_SEC);
				//ARK2D::getLog()->v(StringUtil::appendf("seconds elapsed: ", secondsElapsed));
			#else
				m_CurrentTicks = millis(); //clock();
				float secondsElapsed = (m_CurrentTicks - m_LastTicks) / static_cast<float>(CLOCKS_PER_SEC);
			#endif 

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

			if ( m_FrameTimes.size() > 0 ) {
				std::list<float>::iterator it;
				for ( it = m_FrameTimes.begin(); it != m_FrameTimes.end(); it++ ) {
					m_TimeDelta += *it;
				}
				m_TimeDelta /= static_cast<float>(m_FrameTimes.size());
			}

			//if ( m_FrameTimes.size() > 0 )
				

			if (m_TimeDelta < 0.0f) {
				m_TimeDelta = 0.0f;
			}

		}

		unsigned int GameTimer::getFPS() const {
			return m_FrameRate;
		}


		/*!
		 * Returns the average amount of seconds that have passed
		 */
		float GameTimer::getDelta() const {
			return m_TimeDelta * m_deltaModifier;
		}
		float GameTimer::getDeltaNoModifiers() const {
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
