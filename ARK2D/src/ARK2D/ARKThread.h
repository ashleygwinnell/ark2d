/*
 * ARKThread.h
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#ifndef ARKTHREAD_H_
#define ARKTHREAD_H_

#include "Event.h"

#include <windows.h>

class ARKThread {
	public:
		ARKThread();
		void init(void* functionPointer);
		void start();
		void pause();
		void end();
		void terminate();
		int getPriority();
		void setPriority(int i);
		virtual ~ARKThread();

	private:
		void* m_functionPointer;

		// win32
		DWORD m_id;
		HANDLE m_handle;

	public:
		HDC m_deviceContext;
		HGLRC m_renderingContext;
};



#endif /* ARKTHREAD_H_ */
