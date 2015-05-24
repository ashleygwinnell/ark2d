/*
 * Event.h
 *
 *  Created on: 26-May-2010
 *      Author: Ashley
 */

#ifndef ARK_CORE_EVENT_H_
#define ARK_CORE_EVENT_H_

#include "../Common/DLL.h"

namespace ARK {
	namespace Core {

		class ARK2D_API Event {
			public:
				static const unsigned int LEFT_MOUSE_DOWN = 0;

				Event();
				virtual void eventPerformed() = 0;
				virtual ~Event();
		};

		class ARK2D_API DefaultEvent : public Event {
			public:
				void eventPerformed() {
					//	std::cout << "DefaultEvent class" << std::endl;
				}
				virtual ~DefaultEvent() { }
		};
	}
}

#endif /* ARK_CORE_EVENT_H_ */
