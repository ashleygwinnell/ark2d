/*
 * Mutex.cpp
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */


#include "Mutex.h"
#include "../Controls/ErrorDialog.h"
#include "../../Util/StringUtil.h"
 
namespace ARK {
    namespace Core {
        namespace Threading {

            Mutex::Mutex()
                #if defined(ARK2D_WINDOWS)
                    :
                    m_handle(NULL)
    //			#elif defined(ARK2D_PTHREAD)
    //				m_handle(PTHREAD_MUTEX_INITIALIZER)
                #endif
            {
                #if defined(ARK2D_WINDOWS)
                    m_handle = CreateMutex(NULL, FALSE, NULL);
                    if (m_handle == NULL) {
                        ErrorDialog::createAndShow(StringUtil::append("Mutex error: ", GetLastError()));
                    }
                #elif defined(ARK2D_PTHREAD)
                    //pthread_mutex_t handle = PTHREAD_MUTEX_INITIALIZER;
                    int res = pthread_mutex_init(&m_handle, NULL);
                    if (res != 0) {
                        ARK2D::getLog()->t(StringUtil::append("Mutex could not be created: ", res));
                    }
                #endif
            }

            void Mutex::setThread(Thread* t) {
                m_threadHoldingLock = t;
            }

            void Mutex::lock() {
                #if defined(ARK2D_WINDOWS)
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
                #elif defined(ARK2D_PTHREAD)
                    int res = pthread_mutex_lock( &m_handle );
                    if (res != 0) {
                        ARK2D::getLog()->t(StringUtil::append("Could not lock mutex. error: ", res));
                    }
                #endif
            }
            void Mutex::release() {
                #if defined(ARK2D_WINDOWS)
                    ReleaseMutex(m_handle);
                #elif defined(ARK2D_PTHREAD)
                    int res = pthread_mutex_unlock( &m_handle );
                    if (res != 0) {
                        ARK2D::getLog()->t(StringUtil::append("Could not unlock mutex. error: ", res));
                    }
                #endif
            }
            void Mutex::unlock() {
                release();
            }

            Mutex::~Mutex() {
                #if defined(ARK2D_WINDOWS)
                    CloseHandle(m_handle);
                    //m_handle = 0;
                #elif defined(ARK2D_PTHREAD)
                    pthread_mutex_destroy(&m_handle);
                #endif
            }
        }

	}
}
