/*
 * ARKThread.h
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#ifndef ARKTHREAD_H_
#define ARKTHREAD_H_

#include "../Event.h"

#if defined (ARK2D_WINDOWS)
	#include <windows.h>
#else
	#include <pthread.h>
#endif

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

	#if defined (ARK2D_WINDOWS)
		private:
			void* m_functionPointer;

			// win32
			DWORD m_id;
			HANDLE m_handle;

		public:
			HDC m_deviceContext;
			HGLRC m_renderingContext;
	#else

		private:
			void* m_functionPointer;
			pthread_t m_thread;

	#endif
};



#endif /* ARKTHREAD_H_ */
