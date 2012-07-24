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



using namespace std; 

namespace ARK {
	namespace Util {

		/*!
		 * \brief GET & POST to a URL. Choice of synchronous or asynchronous with callback.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class URLRequest {

			private:
				ARK::Threading::Thread* m_thread;
				string m_url;
				map<string, string> m_postArgs;
				string m_response;
				void* m_callback;
				void* m_callbackObj;
			
			public: 
				URLRequest();
				void setUrl(string url);
				void addPostArg(string key, string value);
				void start(void* callback);
				void start(void* callback, void* callbackObj);
				string start();
				string getResponse();
				virtual ~URLRequest();

			private:
				size_t writeFunction(void *ptr, size_t size, size_t nmemb);
				void reset();
			private:
				static bool s_curlInitted;
				#ifndef ARK2D_ANDROID
					static CURL* s_curl;
				#endif
				static size_t s_curlWriteDataFunction(void *ptr, size_t size, size_t nmemb, void* cls);
				static void s_startThreaded(void* obj);
		};
	}
}

#endif /* ARK_UTIL_URLREQUEST_H_ */
