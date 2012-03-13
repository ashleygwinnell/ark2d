/*
 * ARKThread.cpp
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#include "ARKThread.h"
#include "../UI/ErrorDialog.h"
#include "../Util/StringUtil.h"
#include "../Util/ARKLog.h"
#include "../ARK2D.h"

#if defined(ARK2D_WINDOWS)

	ARKThread::ARKThread() {

	}

	void ARKThread::init(void* functionPointer) {
		m_functionPointer = functionPointer;
		m_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) functionPointer, NULL, CREATE_SUSPENDED, &m_id);
	}

	void ARKThread::start() {
		ResumeThread(m_handle);
	}
	void ARKThread::pause() {
		SuspendThread(m_handle);
	}
	void ARKThread::setPriority(int i) {
		// THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_BELOW_NORMAL, THREAD_PRIORITY_NORMAL
		// THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST, THREAD_PRIORITY_TIME_CRITICAL,
		// THREAD_PRIORITY_IDLE
		SetThreadPriority(m_handle, 0);
	}
	int ARKThread::getPriority() {
		return GetThreadPriority(m_handle);
	}
	void ARKThread::end() {
		ExitThread(m_id);
		CloseHandle(m_handle);
	}
	void ARKThread::terminate() {
		DWORD exitCode;
		GetExitCodeThread(m_handle, &exitCode);
		TerminateThread(m_handle, exitCode);
	}
	ARKThread::~ARKThread() {

	}

#else


	ARKThread::ARKThread():m_functionPointer(NULL), m_thread() {

	}
	void ARKThread::init(void* functionPointer) {
		m_functionPointer = functionPointer;
	}
	void ARKThread::start() {
		int rc = pthread_create(&m_thread, NULL, (void* (*)(void*)) m_functionPointer, (void*) NULL);
		if (rc) {
			ErrorDialog::createAndShow(StringUtil::append("Error creating thread: ", rc));
		}
	}
	void ARKThread::pause() {
		ARK2D::getLog()->w("no thread pausing on non-windows platforms.");
	}
	void ARKThread::setPriority(int i) {
		ARK2D::getLog()->w("no thread priorities on non-windows platforms.");
	}
	int ARKThread::getPriority() {
		ARK2D::getLog()->w("no thread priorities on non-windows platforms.");
		return 0;
	}
	void ARKThread::end() {
		terminate();
	}
	void ARKThread::terminate() {
		//pthread_exit(NULL);
		#if defined(ARK2D_ANDROID)
			ARK2D::getLog()->w("no thread termination on android platform.");
		#else
			pthread_cancel(m_thread);
		#endif
	}
	ARKThread::~ARKThread() { }

#endif
