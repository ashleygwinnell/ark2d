/*
 * ARKMutex.h
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#ifndef ARKMUTEX_H_
#define ARKMUTEX_H_

#include <iostream>
#include <stdlib.h>

#include <windows.h>

#include "ARKThread.h"

class ARKMutex {
	public:
		ARKMutex();
		void setThread(ARKThread* t);
		void lock();
		void release();
		virtual ~ARKMutex();

	private:
		HANDLE m_handle;
		ARKThread* m_threadHoldingLock; // null if master thread.
};

#endif /* ARKMUTEX_H_ */
