/*
 * Event.h
 *
 *  Created on: 26-May-2010
 *      Author: Ashley
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <iostream>
#include <stdlib.h>

namespace ARK {
	namespace Core {

		class Event {
			public:
				static const unsigned int LEFT_MOUSE_DOWN = 0;

				Event();
				virtual void eventPerformed() = 0;
				virtual ~Event();
		};

		class DefaultEvent : public Event {
			public:
				void eventPerformed() {
					//	std::cout << "DefaultEvent class" << std::endl;
				}
				virtual ~DefaultEvent() { }
		};
	}
}

#endif /* EVENT_H_ */
