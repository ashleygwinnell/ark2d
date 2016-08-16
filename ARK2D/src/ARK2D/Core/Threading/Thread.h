/*
 * Thread.h
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#ifndef ARK_CORE_THREADING_THREAD_H_
#define ARK_CORE_THREADING_THREAD_H_

#include "../../Namespaces.h"
#include "../../Common/DLL.h"

#include "../Event.h"
#include "../Vector.h"

#if (defined (ARK2D_WINDOWS) || defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_XBOXONE))
	#include <windows.h>
#else
	#include <pthread.h>
#endif

namespace ARK {
    namespace Core {
        namespace Threading {

            /*!
             * \brief Yay, multiple threading.
             *
             * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
             */
            class ARK2D_API Thread {
                public:
                    Thread();
                    void init(void* functionPointer);
                    void init(void* functionPointer, void* classPointer);
                    void start();
                    void* doInternal();
                    void pause();
                    void end();
                    void join();
                    void terminate();
                    int getPriority();
                    void setPriority(int i);

                    inline void setAutoDetaching(bool b) { m_autoDetaching = b; } // needed for JNI thing.
                    inline bool isAutoDetaching() { return m_autoDetaching; }

                    virtual ~Thread();

                #if defined (ARK2D_WINDOWS)
                    private:
                        void* m_functionPointer;
                        void* m_classPointer;

                        // win32
                        DWORD m_id;
                        HANDLE m_handle;

                        static void initThread(void* obj);

                    public:
                        HDC m_deviceContext;
                        HGLRC m_renderingContext;

                #elif defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_XBOXONE)

                    void* m_functionPointer;
                    void* m_classPointer;
                    Windows::Foundation::IAsyncAction^ m_handle;

                #else

                    private:
                        void* m_functionPointer;
                        void* m_classPointer;
                        pthread_t m_thread;

                #endif

                public:
                    bool m_autoDetaching;
                    bool m_detached;

                #if defined(ARK2D_ANDROID)
                    unsigned int m_internalId;

                    static bool s_initted;
                    static Vector<Thread* >* s_threads;
                    static unsigned int s_internalId;
                    static void s_threadStartInternal(unsigned int thread_id);
                #endif
            };
        }
	}
}

#endif /* ARKTHREAD_H_ */
