/*
 * Thread.cpp
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#include "Thread.h"
#include "../Controls/ErrorDialog.h"
#include "../ARK2D.h"
#include "../GameContainer.h"
#include "../Vector.h"
#include "../Log.h"

#include "../Util/StringUtil.h"
#include "../Util/Callbacks.h"

#ifdef ARK2D_EMSCRIPTEN_JS
    #include <emscripten/emscripten.h>
    #include <emscripten/html5.h>
#else
    #define EMSCRIPTEN_KEEPALIVE
#endif

#ifdef ARK2D_WINDOWS_STORE
    #include "../Vendor/8741.ThreadEmulation/ThreadEmulation.h"
#endif


std::map<string, const char*> emscripten_functionPointerKeys;

void Thread::init(string key, void* functionPointer) {
    ARK2D::getLog()->t("Initialising thread with string and function pointer...");

    m_functionPointerKey = key;
    m_functionPointer = functionPointer;
    emscripten_functionPointerKeys[key] = (const char*) functionPointer;
}
extern "C" void EMSCRIPTEN_KEEPALIVE emscripten_run_thread(char* key) {
    ARK2D::getLog()->w("emscripten_run_thread c function");
    printf("%s",key);
    //printf("%s",(const char*) &key);
    //ARK2D::getLog()->w(key);
    string s(key); //(const char*) &key);
    ARK2D::getLog()->w(s);

    void (*pt)() = (void(*)()) emscripten_functionPointerKeys[s];;
    pt();
}

namespace ARK {
    namespace Core {
        namespace Threading {


            #if (defined(ARK2D_WINDOWS_PHONE_8))

                using namespace Windows::Foundation;
                using namespace concurrency;

                Thread::Thread():
                    m_functionPointerKey(""),
                    m_functionPointer(NULL),
                    m_classPointer(NULL),
                    m_autoDetaching(false),
                    m_detached(false)
                    {

                }

                void Thread::init(void* functionPointer, void* classPointer) {
                    m_functionPointer = functionPointer;
                    m_classPointer = classPointer;
                }
                void Thread::init(void* functionPointer) {
                    m_functionPointer = functionPointer;
                    //m_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) functionPointer, NULL, CREATE_SUSPENDED, &m_id);
                }

                void Thread::start() {

                    // Call the *Async method that starts the operation.
                    // IAsyncOperation<DeviceInformationCollection^>^ deviceOp = DeviceInformation::FindAllAsync();

                    // // Recommended:
                    // auto deviceEnumTask = create_task(deviceOp);

                    // // Call the task's .then member function, and provide
                    // // the lambda to be invoked when the async operation completes.
                    // deviceEnumTask.then( [this] (DeviceInformationCollection^ devices )
                    // {
                    //     for(int i = 0; i < devices->Size; i++)
                    //     {
                    //         DeviceInformation^ di = devices->GetAt(i);
                    //         // Do something with di...
                    //     }
                    // }); // end lambda

                    // auto workItemDelegate = [this](IAsyncAction^ workItem)
                    // {

                    // };

                    // Creates an IAsyncAction object and uses an implicit cancellation token.
					GameContainer* container = ARK2D::getContainer();
                    auto op1 = create_async([this, container]
                    {
                        // Define work here.
                        ARK2D::getLog()->t("doing thread now...");
                        if (m_functionPointer != NULL) {
                            if (m_classPointer == NULL) {
                                void (*pt)() = (void(*)()) m_functionPointer;
                                pt();
                            } else {
                                void (*pt)(void*) = (void(*)(void*)) m_functionPointer;
                                pt(m_classPointer);
                            }
                        }
                    });

                    // auto completionDelegate = [this](IAsyncAction^ action, Windows::Foundation::AsyncStatus status)
                    // {
                    //     switch (action->Status)
                    //     {
                    //         case Windows::Foundation::AsyncStatus::Started:
                    //             ARK2D::getLog()->t("Thread Started");
                    //             break;
                    //         case Windows::Foundation::AsyncStatus::Completed:
                    //             ARK2D::getLog()->t("Thread Completed");
                    //             break;
                    //         case Windows::Foundation::AsyncStatus::Canceled:
                    //             ARK2D::getLog()->t("Thread Cancelled");
                    //             break;
                    //         case Windows::Foundation::AsyncStatus::Error:
                    //             ARK2D::getLog()->t("Thread Error. ;O; ");
                    //             break;
                    //     }
                    // };

                   // #ifndef ARK2D_XBOXONE

                       // auto workItemHandler = ref new Windows::System::Threading::WorkItemHandler(workItemDelegate);
                       // auto completionHandler = ref new Windows::Foundation::AsyncActionCompletedHandler(completionDelegate, Platform::CallbackContext::Same);


                        //m_handle = Windows::System::Threading::ThreadPool::RunAsync(workItemHandler, Windows::System::Threading::WorkItemPriority::Normal);
                   // #endif
                }
                void Thread::pause() {
                    //SuspendThread(m_handle);
                }
                void Thread::setPriority(int i) {
                    // THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_BELOW_NORMAL, THREAD_PRIORITY_NORMAL
                    // THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST, THREAD_PRIORITY_TIME_CRITICAL,
                    // THREAD_PRIORITY_IDLE
                    //SetThreadPriority(m_handle, 0);
                }
                int Thread::getPriority() {
                    //return GetThreadPriority(m_handle);
                    return 0;
                }
                void Thread::join() {
                    // TODO:
                }
                void Thread::end() {
                    // ExitThread(m_id);
                    // CloseHandle(m_handle);
                }
                void Thread::terminate() {
                    m_handle->Cancel();
                    // DWORD exitCode;
                    // GetExitCodeThread(m_handle, &exitCode);
                    // TerminateThread(m_handle, exitCode);
                }
                Thread::~Thread() {

                }
            #elif (defined(ARK2D_WINDOWS_STORE))
                 Thread::Thread():
                    m_functionPointerKey(""),
                    m_functionPointer(NULL),
                    m_classPointer(NULL),
                    m_autoDetaching(false),
                    m_detached(false)
                    {

                }

                void Thread::init(void* functionPointer, void* classPointer) {
                    m_functionPointer = functionPointer;
                    m_classPointer = classPointer;
					m_handle = ThreadEmulation::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) initThread, reinterpret_cast<void*>(this), CREATE_SUSPENDED, nullptr);
                }
                void Thread::init(void* functionPointer) {
                    m_functionPointer = functionPointer;

					m_handle = ThreadEmulation::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)initThread, reinterpret_cast<void*>(this), CREATE_SUSPENDED, nullptr);
                }
                void Thread::initThread(void* obj) {
                    //Thread* t = (Thread*) obj;
                    Thread* thr = reinterpret_cast<Thread*>(obj);
                    if (thr->m_functionPointer != NULL) {
                        if (thr->m_classPointer == NULL) {

                            void (*pt)() = (void(*)()) thr->m_functionPointer;
                            //typedef void fnct();
                            //fnct* pt = (fnct*) m_event;
                            pt();
                        } else {
                            void (*pt)(void*) = (void(*)(void*)) thr->m_functionPointer;
                            pt(thr->m_classPointer);
                        }
                    }
                }

                void Thread::start() {
					ThreadEmulation::ResumeThread(m_handle);
                }
                void Thread::pause() {

                }
                void Thread::setPriority(int i) {
					ThreadEmulation::SetThreadPriority(m_handle, 0);
                }
                int Thread::getPriority() {
					return 0;
                }
                void Thread::join() {

                }
                void Thread::end() {

                }
                void Thread::terminate() {

                }
                Thread::~Thread() {

                }
            #elif (defined(ARK2D_WINDOWS))

                Thread::Thread():
                    m_functionPointerKey(""),
                    m_functionPointer(NULL),
                    m_classPointer(NULL),
                    m_autoDetaching(false),
                    m_detached(false)
                    {

                }

                void Thread::init(void* functionPointer, void* classPointer) {
                    m_functionPointer = functionPointer;
                    m_classPointer = classPointer;
                    m_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) initThread, (void*) this, CREATE_SUSPENDED, &m_id);
                }
                void Thread::init(void* functionPointer) {
                    m_functionPointer = functionPointer;
                    m_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) functionPointer, NULL, CREATE_SUSPENDED, &m_id);
                }
                void Thread::initThread(void* obj) {
                    //Thread* t = (Thread*) obj;
                    Thread* thr = reinterpret_cast<Thread*>(obj);
                    if (thr->m_functionPointer != NULL) {
                            if (thr->m_classPointer == NULL) {

                                void (*pt)() = (void(*)()) thr->m_functionPointer;
                                //typedef void fnct();
                                //fnct* pt = (fnct*) m_event;
                                pt();
                            } else {
                                void (*pt)(void*) = (void(*)(void*)) thr->m_functionPointer;
                                pt(thr->m_classPointer);
                            }
                        }
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
                void Thread::join() {
                    // TODO:
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

            /*	void thread_exit_handler(int sig)
                {
                    printf("this signal is %d \n", sig);
                    pthread_exit(0);
                }*/

                #if defined(ARK2D_ANDROID)
                    bool Thread::s_initted = false;
                    Vector<Thread* >* Thread::s_threads = NULL;
                    unsigned int Thread::s_internalId = 0;

                    void Thread::s_threadStartInternal(unsigned int thread_id) {
                        VectorIterator<Thread* >* it = s_threads->iterator();
                        while (it->hasNext()) {
                            Thread* tt = it->next();
                            if (tt->m_internalId == thread_id) {
                                tt->doInternal();
                                break;
                            }
                        }
                    }
                #endif

                Thread::Thread():
                    m_functionPointerKey(""),
                    m_functionPointer(NULL),
                    m_classPointer(NULL),
                    m_thread(),
                    m_autoDetaching(false),
                    m_detached(false)
                    #if defined(ARK2D_ANDROID)
                        ,
                        m_internalId(0)
                    #endif
                    {

                    #if defined(ARK2D_ANDROID)
                        if (s_initted == false) {
                            /*struct sigaction actions;
                            memset(&actions, 0, sizeof(actions));
                            sigemptyset(&actions.sa_mask);
                            actions.sa_flags = 0;
                            actions.sa_handler = thread_exit_handler;
                            sigaction(SIGUSR1, &actions, NULL);*/

                            s_initted = true;
                            s_threads = new Vector<Thread*>();

                            // pirate version callbacks
                            ARK::Core::Utils::Callback callback;
                            callback.setId(ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_THREAD_START);
                            callback.setFunctionPointer((void*) &s_threadStartInternal);
                            Callbacks::add(callback);
                        }

                        m_internalId = s_internalId;
                        s_internalId++;

                        s_threads->add(this);
                    #endif

                }
                void Thread::init(void* functionPointer) {
                    ARK2D::getLog()->t("Initialising thread with function pointer...");
                    m_functionPointer = functionPointer;
                }
                void Thread::init(void* functionPointer, void* classPointer) {
                    ARK2D::getLog()->t("Initialising thread with class pointer and function pointer...");
                    m_functionPointer = functionPointer;
                    m_classPointer = classPointer;
                }

                void* myLauncher(void* obj)	{
                    Thread* myLauncher = reinterpret_cast<Thread*>(obj);
                    return myLauncher->doInternal();
                }

                void Thread::start() {

                    //#if defined(ARK2D_FLASCC)
                    //	ARK2D::getLog()->w("Threads not supported properly before Flash Player 11.5. Running thread now.");
                    //	doInternal();
                    //#else

                    #if defined(ARK2D_ANDROID)
                        ARK2D::getLog()->t("Starting thread...");
                        ARK2D::getContainer()->getPlatformSpecific()->getPluggable()->thread_start(m_internalId);
                        ARK2D::getLog()->t("Thread started! ");
                    #elif defined(ARK2D_EMSCRIPTEN_JS)

                        ARK2D::getLog()->t(StringUtil::append("Starting thread (Emscripten)... key:", m_functionPointerKey));
                        const char* cptr = m_functionPointerKey.c_str();
                       //uint32_t cptrint = cptr;
                        EM_ASM_({
                            console.log( 'JS code', $0, Pointer_stringify($0) );


                            //var data = new Float32Array([1, 2, 3, 4, 5]);

                            //setTimeout(function(){
                            // Module.ccall('emscripten_run_thread', // name of C function
                            //     null, // void return type
                            //     ['string'] // argument types,
                            //     [ Pointer_stringify($0) ]//, // arguments
                            //     //{ async:true } // optional options
                            // );

                            var runthread = Module.cwrap('emscripten_run_thread', null,  ['string'] );
                            runthread(Pointer_stringify($0));
                            console.log('Done!');
                            //},1);

                        }, cptr);

                    #else

                        ARK2D::getLog()->t("Starting thread...");
                        int rc = 0;
                        rc = pthread_create(&m_thread, NULL, (void* (*)(void*)) myLauncher, (void*) this);
                        if (rc) {
                            ErrorDialog::createAndShow(StringUtil::append("Error creating thread: ", rc));
                            return;
                        }
                        ARK2D::getLog()->t("Thread started! ");

                    #endif

                    //#endif
                }
                void* Thread::doInternal() {
                    #if defined(ARK2D_ANDROID)
                        JNIEnv* env = 0;

                        jint getEnvSuccess = ARK2D::getContainer()->m_platformSpecific.m_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
                        if (getEnvSuccess == JNI_OK) {
                            ARK2D::getLog()->t("thread: thread already attached.");
                        } else if (getEnvSuccess == JNI_EDETACHED) {
                            ARK2D::getLog()->t("thread: thread not attached. attaching...");
                            jint attachSuccess = ARK2D::getContainer()->m_platformSpecific.m_jvm->AttachCurrentThread(&env, NULL);
                            if (attachSuccess == 0) {
                                ARK2D::getLog()->t("thread: attached.");
                            } else {
                                ARK2D::getLog()->e("thread: could not attach current thread... maybe it's already attached! ");
                            }
                        }

                    #elif defined(ARK2D_IPHONE)
                        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
                    #endif

                    ARK2D::getLog()->t("doing thread now...");
                    if (m_functionPointer != NULL) {
                        if (m_classPointer == NULL) {
                            void (*pt)() = (void(*)()) m_functionPointer;
                            pt();
                        } else {
                            void (*pt)(void*) = (void(*)(void*)) m_functionPointer;
                            pt(m_classPointer);
                        }
                    }

                    #ifdef ARK2D_IPHONE
                        [pool drain];
                    #endif

                    if (m_autoDetaching && !m_detached) {
                        terminate();
                    }
                    return NULL;
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
                void Thread::join() {
                    #if defined(ARK2D_ANDROID)

                    #else
                        pthread_join(m_thread, NULL);
                    #endif
                }
                void Thread::end() {
                    terminate();
                }
                void Thread::terminate() {
                    //pthread_exit(NULL);
                    #if defined(ARK2D_ANDROID)
                        m_detached = true;

                        s_threads->remove(this);

                        ARK2D::getLog()->t("terminating thread...");
                        //ARK2D::getContainer()->m_platformSpecific.m_jvm->DetachCurrentThread();

                        //ARK2D::getLog()->w("no thread termination on android platform.");

                        /*int status = 0;
                        if ( (status = pthread_kill(m_thread, SIGUSR1)) != 0) {
                            ARK2D::getLog()->i("error terminating thread :( ");
                            printf("Error cancelling thread %d, error = %d (%s)", m_thread, status, strerror(status));
                        } */
                    #elif defined(ARK2D_FLASCC)
                        m_detached = true;

                        ARK2D::getLog()->t("terminating thread (flascc)");
                        pthread_exit(m_thread);
                        //pthread_join(m_thread);
                    #elif defined(ARK2D_EMSCRIPTEN_JS)
                        ARK2D::getLog()->t("terminating thread (emscripten) (there is no thread.)");
                    #else
                        try {
                            m_detached = true;

                            ARK2D::getLog()->t("terminating thread");
                            pthread_cancel(m_thread);
                        } catch (...) {
                            ARK2D::getLog()->t("could not terminate the thread. ");
                        }
                    #endif
                }
                Thread::~Thread() {
                    ARK2D::getLog()->t("thread destructor");
                    if (m_autoDetaching && !m_detached) {
                        terminate();
                    }
                }

            #endif
        }
	}
}
