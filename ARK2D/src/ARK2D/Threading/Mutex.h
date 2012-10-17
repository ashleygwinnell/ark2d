/*
 * Mutex.h
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#ifndef ARKMUTEX_H_
#define ARKMUTEX_H_

//#include <iostream>
//#include <stdlib.h>
#include "../Includes.h"

#if defined(ARK2D_WINDOWS)
	#include <windows.h>
#endif

#include "Thread.h"
#include "../ARK2D.h"

namespace ARK {
	namespace Threading {

		/*!
		 * \brief Semaphore mutex locking.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class Mutex {
			public:
				Mutex();
				void setThread(Thread* t);
				void lock();
				void release();
				virtual ~Mutex();

			private:
				#if defined(ARK2D_WINDOWS)
					HANDLE m_handle;
				#endif
				Thread* m_threadHoldingLock; // null if master thread.
		};
	}
}

#endif /* ARKMUTEX_H_ */
