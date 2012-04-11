/*
 * Thread.h
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#ifndef ARKTHREAD_H_
#define ARKTHREAD_H_

#include "../Core/Event.h"

#if defined (ARK2D_WINDOWS)
	#include <windows.h>
#else
	#include <pthread.h>
#endif

namespace ARK {
	namespace Threading {

		/*!
		 * \brief Yay, multiple threading.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class Thread {
			public:
				Thread();
				void init(void* functionPointer);
				void start();
				void pause();
				void end();
				void terminate();
				int getPriority();
				void setPriority(int i);
				virtual ~Thread();

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
	}
}


#endif /* ARKTHREAD_H_ */
