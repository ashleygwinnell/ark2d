/*
 * URLRequest.cpp
 *
 *  Created on: 16 May 2012
 *      Author: Ashley
 */

#include "URLRequest.h"

namespace ARK {  
	namespace Util {

		#if !defined(ARK2D_ANDROID) && !defined(ARK2D_IPHONE)
			CURL* URLRequest::s_curl = NULL;
		#endif
		bool URLRequest::s_curlInitted = false;

		URLRequest::URLRequest():
			m_thread(NULL),
			m_url(""),
			m_postArgs(),
			m_response(""),
			m_callback(NULL),
			m_callbackObj(NULL) {


			if (s_curlInitted == false) {
				#if !defined(ARK2D_ANDROID) && !defined(ARK2D_IPHONE)
					s_curl = curl_easy_init();
					if (!s_curl) {
						ErrorDialog::createAndShow("could not init libcurl");
						exit(0);
					}
				#endif
				s_curlInitted = true;
			}
		}

		void URLRequest::setUrl(string url) {
			m_url = url;
		}
		void URLRequest::addPostArg(string key, string value) {
			m_postArgs[key] = value;
		}

		
		void URLRequest::reset() {
			m_response = "";
		}

		
		void URLRequest::start(void* callback) {
			reset();
			
			start(callback, NULL);
		}
		
		void URLRequest::start(void* callback, void* callbackObj) {
			reset();
			
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
			reset();
			
			//#if defined(ARK2D_WINDOWS)

				/*BOOL ret = FALSE;
				HINTERNET hSession = NULL;
				HINTERNET hConnect = NULL;
				HINTERNET hRequest = NULL;

				hSession = WinHttpOpen (
					L"ARK2D URLRequest/1.0", 
					WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
					WINHTTP_NO_PROXY_NAME, 
					WINHTTP_NO_PROXY_BYPASS, 
					0
				);

				if ( hSession ) {
					hConnect = WinHttpConnect( 
						hSession, 
						m_url, //CStdStringW( GJAPI_SERVER ), m_url but split before 3rd forward slash, e.g. "gamejolt.com"
						INTERNET_DEFAULT_PORT, 
						0
					);
				} else {
					ARK2D::getLog()->e("Could not open HTTP session.");
					return "";
				}

				if ( hConnect ) {
					hRequest = WinHttpOpenRequest ( 
						hConnect,
						L"GET",
						m_url, //CStdStringW( url ),
						NULL,
						WINHTTP_NO_REFERER,
						WINHTTP_DEFAULT_ACCEPT_TYPES,
						WINHTTP_FLAG_REFRESH
					);
				} else {
					ARK2D::getLog()->e("Could not connect to the HTTP session.");
					return "";
				}

				if ( hRequest ) {
					ret = WinHttpSendRequest ( 
						hRequest,
						WINHTTP_NO_ADDITIONAL_HEADERS,
						0,
						WINHTTP_NO_REQUEST_DATA,
						0,
						0,
						0
					);
				} else {
					ARK2D::getLog()->e("Could not set up the HTTP request.");
					return "";
				}

				if ( ret ) {
					ret = WinHttpReceiveResponse( hRequest, NULL );
				} else {
					ARK2D::getLog()->e("Could not send the HTTP request.");
					return "";
				}

				DWORD bufferSize = 0;
				DWORD outputDownloaded = 0;
				LPSTR outputBuffer = 0;

				// Keep checking for data until there is nothing left.
				if ( ret )
				{
					do 
					{
						// Check for available data.
						bufferSize = 0;
						WinHttpQueryDataAvailable( hRequest, &bufferSize );

						// Allocate space for the buffer.
						outputBuffer = new char[bufferSize + 1];
						if ( outputBuffer )
						{
							// Read the data.
							ZeroMemory( outputBuffer, bufferSize + 1 );

							if ( WinHttpReadData( hRequest, (LPVOID)outputBuffer, bufferSize, &outputDownloaded ) )
								output += outputBuffer;

							// Free the memory allocated to the buffer.
							delete [] outputBuffer;
						}

					} while( bufferSize > 0 );
				}
				else {
					ARK2D::getLog()->e("Did not get a response from the server.");
					return "";
				}

				if ( hRequest ) WinHttpCloseHandle( hRequest );
			    if ( hConnect ) WinHttpCloseHandle( hConnect );
			    if ( hSession ) WinHttpCloseHandle( hSession );

				return true;*/

			//#elif defined (ARK2D_ANDROID) 
			#if defined (ARK2D_ANDROID) 
				//! todo: HTTP POST args on Android.
				m_response = ARK2D::getContainer()->m_platformSpecific.m_pluggable->urlRequest(m_url);
				return m_response;
            #elif defined(ARK2D_IPHONE) 
                return "Not implemented";
            #else

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
				
				ARK2D::getLog()->v(StringUtil::append("curl_easy_perform returned: ", res));

				long http_code = 0;
				curl_easy_getinfo (s_curl, CURLINFO_RESPONSE_CODE, &http_code);

				ARK2D::getLog()->v(StringUtil::append("curl http response code: ", (int) http_code));
				
				//returnString += (int) http_code;



				//returnString += ", ";
				//returnString += res;
				//returnString += ", ";
				returnString += m_response;
				//returnString += ".";
				return returnString.get();
			#endif
			return "";
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
			m_response += buf;
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
