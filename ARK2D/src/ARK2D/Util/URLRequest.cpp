/*
 * URLRequest.cpp
 *
 *  Created on: 16 May 2012
 *      Author: Ashley
 */

#include "URLRequest.h"

namespace ARK {
	namespace Util {
		bool URLRequest::s_curlInitted = false;
		CURL* URLRequest::s_curl = NULL;
	}
}

namespace ARK {
	namespace Util {
		
		URLRequest::URLRequest():
			m_thread(NULL),
			m_url(""),
			m_postArgs(),
			m_response(""),
			m_callback(NULL),
			m_callbackObj(NULL) {

			if (s_curlInitted == false) {
				s_curl = curl_easy_init();
				if (!s_curl) {
					ErrorDialog::createAndShow("could not init libcurl");
					exit(0);
				}
			}
		}

		void URLRequest::setUrl(string url) {
			m_url = url;
		}
		void URLRequest::addPostArg(string key, string value) {
			m_postArgs[key] = value;
		}


		
		void URLRequest::start(void* callback) {
			start(callback, NULL);
		}
		
		void URLRequest::start(void* callback, void* callbackObj) {
			m_callback = callback; 
			m_callbackObj = callbackObj;

			m_thread = new Thread(); 
			m_thread->init((void*) &s_startThreaded, this);
			m_thread->start();
		}

		void URLRequest::s_startThreaded(void* u) {
			URLRequest* r = reinterpret_cast<URLRequest*>(u);
			string result = r->start();

			if (r->m_callback != NULL) {
				if (r->m_callbackObj == NULL) {
					void (*pt)(string) = (void(*)(string)) r->m_callback;
					pt(result);
				} else {
					void (*pt)(void*, string) = (void(*)(void*, string)) r->m_callback;
					pt(r->m_callbackObj, result);
				}
			}
		}

		string URLRequest::start() {

			String returnString("");
			CURLcode res;

			curl_easy_setopt(s_curl, CURLOPT_NOPROGRESS, 1);
			curl_easy_setopt(s_curl, CURLOPT_URL, m_url.c_str());
			curl_easy_setopt(s_curl, CURLOPT_WRITEFUNCTION, &URLRequest::s_curlWriteDataFunction);
			curl_easy_setopt(s_curl, CURLOPT_WRITEDATA, this);

			if (m_postArgs.size() > 0) {
				string postArgs = "";

				map<string, string>::iterator iter;
				for (iter = m_postArgs.begin(); iter != m_postArgs.end(); iter++) {
					postArgs += iter->first;
					postArgs += "=";
					postArgs += iter->second;
					iter++;
					if (iter != m_postArgs.end()) {
						postArgs += "&";
					}
					iter--;
				}
				//ErrorDialog::createAndShow(postArgs);
				curl_easy_setopt(s_curl, CURLOPT_POST, 1);
				curl_easy_setopt(s_curl, CURLOPT_POSTFIELDS, postArgs.c_str());
			}

			res = curl_easy_perform(s_curl);

			long http_code = 0;
			curl_easy_getinfo (s_curl, CURLINFO_RESPONSE_CODE, &http_code);

			//returnString += (int) http_code;



			//returnString += ", ";
			//returnString += res;
			//returnString += ", ";
			returnString += m_response;
			//returnString += ".";
			return returnString.get();
		}
		string URLRequest::getResponse() {
			return m_response;
		}

		size_t URLRequest::writeFunction(void *ptr, size_t size, size_t nmemb) {
			char buf[size*nmemb+1];
			char * pbuf = &buf[0];
			memset(buf, '\0', size*nmemb+1);
			size_t i = 0;
			for(;  i < nmemb ; i++){
				strncpy(pbuf, (char*) ptr, size);
				pbuf += size;
				ptr = ((char*) ptr) + size;
			}
			m_response = buf;
			return size * nmemb;
		}

		URLRequest::~URLRequest() {
			// TODO: cleanup
			//curl_easy_cleanup(s_curl); // always cleanup
		}

		size_t URLRequest::s_curlWriteDataFunction(void *ptr, size_t size, size_t nmemb, void* cls) {
			return static_cast<URLRequest*>(cls)->writeFunction(ptr, size, nmemb);
		}


	}
}
