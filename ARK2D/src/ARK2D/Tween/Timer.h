/*
 * Thread.h
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#ifndef ARK_TIMER_H_
#define ARK_TIMER_H_


#include "../Includes.h"
#include "../Namespaces.h"

namespace ARK {
	namespace Tween {

		class Timer {
			public: 
				float m_timer;
				float m_duration;

			public:
				Timer(float duration);
				void limit();
				void limit(int i);
				void setTime(float time);
				float getTime();
				void setDuration(float duration);
				float getDuration();
				bool update();
				void loop();
				void reset();
				string toString();
				virtual ~Timer();

		};
	}
}

#endif /* ARK_TIMER_H_ */
