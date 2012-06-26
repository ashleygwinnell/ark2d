/*
 * Mutex.cpp
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */


#include "Mutex.h"
#include "../UI/ErrorDialog.h"
#include "../Util/StringUtil.h"
 
namespace ARK {

	namespace Threading {

#if defined (ARK2D_WINDOWS)

		Mutex::Mutex() {
			m_handle = CreateMutex(NULL, FALSE, NULL);
			if (m_handle == NULL) {
				ErrorDialog::createAndShow(StringUtil::append("Mutex error: ", GetLastError()));
			}
		}

		void Mutex::setThread(Thread* t) {
			m_threadHoldingLock = t;
		}

		void Mutex::lock() {
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
		void Mutex::release() {
			ReleaseMutex(m_handle);
			//std::cout << "released lock" << std::endl;
		}

		Mutex::~Mutex() {
			CloseHandle(m_handle);
			//m_handle = 0;
		}

#else

		Mutex::Mutex() {

		}

		void Mutex::setThread(Thread* t) {
			m_threadHoldingLock = t;
		}

		void Mutex::lock() { }
		void Mutex::release() {	}

		Mutex::~Mutex() {	}


#endif

	}
}
