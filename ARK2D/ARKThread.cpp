/*
 * ARKThread.cpp
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#include "ARKThread.h"

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
