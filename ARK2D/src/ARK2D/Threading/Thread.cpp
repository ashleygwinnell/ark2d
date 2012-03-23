/*
 * Thread.cpp
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#include "Thread.h"
#include "../UI/ErrorDialog.h"
#include "../Util/StringUtil.h"
#include "../Util/Log.h"
#include "../ARK2D.h"

namespace ARK {
	namespace Threading {

		#if defined(ARK2D_WINDOWS)

			Thread::Thread() {

			}

			void Thread::init(void* functionPointer) {
				m_functionPointer = functionPointer;
				m_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) functionPointer, NULL, CREATE_SUSPENDED, &m_id);
			}

			void Thread::start() {
				ResumeThread(m_handle);
			}
			void Thread::pause() {
				SuspendThread(m_handle);
			}
			void Thread::setPriority(int i) {
				// THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_BELOW_NORMAL, THREAD_PRIORITY_NORMAL
				// THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST, THREAD_PRIORITY_TIME_CRITICAL,
				// THREAD_PRIORITY_IDLE
				SetThreadPriority(m_handle, 0);
			}
			int Thread::getPriority() {
				return GetThreadPriority(m_handle);
			}
			void Thread::end() {
				ExitThread(m_id);
				CloseHandle(m_handle);
			}
			void Thread::terminate() {
				DWORD exitCode;
				GetExitCodeThread(m_handle, &exitCode);
				TerminateThread(m_handle, exitCode);
			}
			Thread::~Thread() {

			}

		#else


			Thread::Thread():m_functionPointer(NULL), m_thread() {

			}
			void Thread::init(void* functionPointer) {
				m_functionPointer = functionPointer;
			}
			void Thread::start() {
				int rc = pthread_create(&m_thread, NULL, (void* (*)(void*)) m_functionPointer, (void*) NULL);
				if (rc) {
					ErrorDialog::createAndShow(StringUtil::append("Error creating thread: ", rc));
				}
			}
			void Thread::pause() {
				ARK2D::getLog()->w("no thread pausing on non-windows platforms.");
			}
			void Thread::setPriority(int i) {
				ARK2D::getLog()->w("no thread priorities on non-windows platforms.");
			}
			int Thread::getPriority() {
				ARK2D::getLog()->w("no thread priorities on non-windows platforms.");
				return 0;
			}
			void Thread::end() {
				terminate();
			}
			void Thread::terminate() {
				//pthread_exit(NULL);
				#if defined(ARK2D_ANDROID)
					ARK2D::getLog()->w("no thread termination on android platform.");
				#else
					pthread_cancel(m_thread);
				#endif
			}
			Thread::~Thread() { }

		#endif

	}
}
