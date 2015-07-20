/*
 * URLRequest.h
 *
 *  Created on: 16 May 2012
 *      Author: Ashley
 */

#ifndef ARK_UTIL_URLREQUEST_H_
#define ARK_UTIL_URLREQUEST_H_

#include <map>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <string>

#include "../Includes.h"
#include "../Namespaces.h"

#include "../UI/ErrorDialog.h"
#include "../Threading/Thread.h"
#include "../Threading/Mutex.h"
#include "../Core/String.h"

 #include "../Core/GameContainer.h"

#ifdef ARK2D_WINDOWS_PHONE_8
	using namespace Windows::Web::Http;
#endif

using namespace std; 

namespace ARK {
	namespace Util {

		/*!
		 * \brief GET & POST to a URL. Choice of synchronous or asynchronous with callback.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API URLRequest {

			private:
				ARK::Threading::Thread* m_thread;
				string m_url;
				map<string, string> m_postArgs;
				map<string, string> m_cookies;
				string m_response;
				string m_error;
				void* m_callback;
				void* m_callbackObj;
				void* m_callbackExtra;
				//void** m_callbackExtras;
				//unsigned int m_callbackExtraSize;
				bool m_threaded;

				int m_timeout;
			
			public: 
				URLRequest();
				void setUrl(string url);
				void addPostArg(string key, string value);
				void addCookie(string key, string value);
				void start(void* callback);
				void start(void* callback, void* callbackObj);
				void start(void* callback, void* callbackObj, void* callbackExtra);
				//void start(void* callback, void* callbackObj, void** callbackExtras, unsigned int callbackExtrasSize);
				string start();
				string getResponse();

				bool hasError();
				string getError();

				void doCallbackWithResult(string result);

				inline void setTimeout(int seconds) { m_timeout = seconds; }
				inline int getTimeout() { return m_timeout; }

				inline void setThreaded(bool b) { m_threaded = b; }
				inline bool isThreaded() { return m_threaded; }

				virtual ~URLRequest();

			private:
				size_t writeFunction(void *ptr, size_t size, size_t nmemb);
				void reset();
			private:
				static bool s_curlInitted;
				static bool s_isThreadedOverride;
				#if !defined(ARK2D_ANDROID) && !defined(ARK2D_IPHONE)  && !defined(ARK2D_WINDOWS_PHONE_8) && !defined(ARK2D_XBOXONE)
					static CURL* s_curl;
				#endif
				static size_t s_curlWriteDataFunction(void *ptr, size_t size, size_t nmemb, void* cls);
				static void s_startThreaded(void* obj);

			public:
				inline static bool isThreadedStatic() { return s_isThreadedOverride; }
				inline static void setThreadedStatic(bool b) { s_isThreadedOverride = b; }
		};
	}
}

#endif /* ARK_UTIL_URLREQUEST_H_ */
