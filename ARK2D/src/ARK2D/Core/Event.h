/*
 * Event.h
 *
 *  Created on: 26-May-2010
 *      Author: Ashley
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "../Includes.h"

// Reducing includes reduces file size.
//  #define ARK2D_API 
// #if defined(ARK2D_WINDOWS)

//  	#ifdef ARK2D_WINDOWS_DLL // abc.dll source code will define this macro before including this header
// 		#define ARK2D_API __declspec( dllexport )				
// 		//#define ARK2D_STATICVAR __declspec( dllimport )				
// 	#else
// 		#define ARK2D_API __declspec( dllimport )
// 		//#define ARK2D_STATICVAR 
// 	#endif 
// #endif

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

#endif /* EVENT_H_ */
