/*
 * ARKMutex.cpp
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */


#include "ARKMutex.h"
#include "ErrorDialog.h"
#include "StringUtil.h"

#if defined (ARK2D_WINDOWS)
	ARKMutex::ARKMutex() {
		m_handle = CreateMutex(NULL, FALSE, NULL);
		if (m_handle == NULL) {
			ErrorDialog::createAndShow(StringUtil::append("Mutex error: ", GetLastError()));
		}
	}

	void ARKMutex::setThread(ARKThread* t) {
		m_threadHoldingLock = t;
	}

	void ARKMutex::lock() {
		//DWORD waitResult;
		//waitResult =

		//std::cout << "pre lock" << std::endl;
		WaitForSingleObject(m_handle, INFINITE);
		//std::cout << "post lock" << std::endl;
		//wglMakeCurrent(ARK2D::getContainer(), m_hRenderingContext);
		/*switch (waitResult) {
			case WAIT_OBJECT_0:
				break;

			case WAIT_ABANDONED:
				return;
		}*/
	}
	void ARKMutex::release() {
		ReleaseMutex(m_handle);
		//std::cout << "released lock" << std::endl;
	}

	ARKMutex::~ARKMutex() {
		CloseHandle(m_handle);
		//m_handle = 0;
	}

#else

	ARKMutex::ARKMutex() {

	}

	void ARKMutex::setThread(ARKThread* t) {
		m_threadHoldingLock = t;
	}

	void ARKMutex::lock() { }
	void ARKMutex::release() {	}

	ARKMutex::~ARKMutex() {	}

#endif
