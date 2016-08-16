/*
 * URLRequest.cpp
 *
 *  Created on: 16 May 2012
 *      Author: Ashley
 */

#include "URLRequest.h"

namespace ARK {  
	namespace Util {

		#if !defined(ARK2D_ANDROID) && !defined(ARK2D_IPHONE) && !defined(ARK2D_WINDOWS_PHONE_8) && !defined(ARK2D_XBOXONE)
			CURL* URLRequest::s_curl = NULL;
		#endif
		bool URLRequest::s_curlInitted = false;
		bool URLRequest::s_isThreadedOverride = false;

		URLRequest::URLRequest():
			m_thread(NULL),
			m_url(""),
			m_postArgs(),
			m_cookies(),
			m_response(""), 
			m_error(""),
			m_callback(NULL),
			m_callbackObj(NULL),
			m_callbackExtra(NULL),
			//m_callbackExtras(NULL),
			//m_callbackExtrasSize(0),
			m_threaded(false),
			m_timeout(15) {

			m_threaded = s_isThreadedOverride;

			if (s_curlInitted == false) {
				#if !defined(ARK2D_ANDROID) && !defined(ARK2D_IPHONE) && !defined(ARK2D_FLASCC) && !defined(ARK2D_WINDOWS_PHONE_8) && !defined(ARK2D_XBOXONE)
					ARK2D::getLog()->i("init curl");
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
		}void URLRequest::addCookie(string key, string value) {
			m_cookies[key] = value;
		}

		
		void URLRequest::reset() {
			m_response = "";
			m_error = "";
		}

		
		void URLRequest::start(void* callback) {
			reset();
			
			start(callback, NULL);
		}
		
		void URLRequest::start(void* callback, void* callbackObj) {
			reset();
			
			m_callback = callback; 
			m_callbackObj = callbackObj;
			m_callbackExtra = NULL;
			//m_callbackExtras = NULL;
			//m_callbackExtrasSize = 0;

			m_thread = new Thread(); 
			m_thread->init((void*) &s_startThreaded, this);
			m_thread->start();
		}

		void URLRequest::start(void* callback, void* callbackObj, void* callbackExtra) { //void** callbackExtra, unsigned int callbackExtrasSize) {
			reset();
			
			m_callback = callback; 
			m_callbackObj = callbackObj;
			m_callbackExtra = callbackExtra;
			//m_callbackExtras = callbackExtras;
			//m_callbackExtrasSize = callbackExtrasSize;
 
			m_thread = new Thread(); 
			m_thread->init((void*) &s_startThreaded, this);
			m_thread->start();
		}

        Mutex* s_startThreadedMutex = new ARK::Core::Threading::Mutex();

		void URLRequest::s_startThreaded(void* u) {
			

			#if defined(ARK2D_WINDOWS_PHONE_8)

				s_startThreadedMutex->lock();

				URLRequest* r = reinterpret_cast<URLRequest*>(u);
				ARK2D::getLog()->e(r->m_url);

				wchar_t* wideUrl = Cast::charToWideChar(r->m_url.c_str());
				
				Platform::String^ wideStr = ref new Platform::String(wideUrl);
				
				Uri^ uri = ref new Uri(wideStr);
				HttpClient^ httpClient = ref new HttpClient();

				
				
				IAsyncOperationWithProgress<HttpResponseMessage^, HttpProgress>^ progress = httpClient->GetAsync(uri);
				progress->Completed = ref new AsyncOperationWithProgressCompletedHandler<HttpResponseMessage^, HttpProgress>([r, wideUrl](IAsyncOperationWithProgress<HttpResponseMessage^, HttpProgress>^ asyncOp, AsyncStatus status) {
				
					HttpResponseMessage^ msg = asyncOp->GetResults();
					if (msg->StatusCode == HttpStatusCode::Ok) {
						ARK2D::getLog()->e("OK!");
						//msg->Content->Read
						IHttpContent^ content = msg->Content;
						IAsyncOperationWithProgress<Platform::String^, unsigned long long>^ progress2 = content->ReadAsStringAsync();
						progress2->Completed = ref new AsyncOperationWithProgressCompletedHandler<Platform::String^, unsigned long long>([r, wideUrl](IAsyncOperationWithProgress<Platform::String^, unsigned long long>^ asyncOp2, AsyncStatus status2) {

							ARK2D::getLog()->e("RESULTS!");
							Platform::String^ str = asyncOp2->GetResults();

							std::wstring fooW(str->Begin());
							std::string result(fooW.begin(), fooW.end());

							

							r->doCallbackWithResult(result);
							delete wideUrl;
							
							
						});

					}
				});

				s_startThreadedMutex->unlock();

			#else

				s_startThreadedMutex->lock();

				URLRequest* r = reinterpret_cast<URLRequest*>(u);
				r->setThreaded(true);  
				string result = r->start();

				r->doCallbackWithResult(result);

				s_startThreadedMutex->unlock();
				// Not sure if I can delete the thread here?
				//if (m_thread != NULL) { 
				//	delete m_thread; 
				//	m_thread = NULL;
				//}
				
			#endif

		
		}
		void URLRequest::doCallbackWithResult(string result) {
			ARK2D::getLog()->v("doCallbackWithResult");
			if (m_callback != NULL) {
				if (m_callbackObj == NULL) { 
					void (*pt)(string) = (void(*)(string)) m_callback;
					pt(result);
				} else {
					if (m_callbackExtra == NULL) {
						void (*pt)(void*, string) = (void(*)(void*, string)) m_callback;
						pt(m_callbackObj, result);
					} else {
						void (*pt)(void*, string, void*) = (void(*)(void*, string, void*)) m_callback;
						pt(m_callbackObj, result, m_callbackExtra);
					}
				}
			}  
		}

		bool URLRequest::hasError() {
			return (m_error.length() > 0);
		}
		string URLRequest::getError() {
			return m_error;
		}
   
		string URLRequest::start() { 
			reset();
			
			/*#if defined(ARK2D_WINDOWS)

				BOOL ret = FALSE;
				HINTERNET hSession = NULL;
				HINTERNET hConnect = NULL;
				HINTERNET hRequest = NULL;

				const char* oldurlstr = m_url.c_str();
				WCHAR*    str3 = (WCHAR*) alloca(sizeof(WCHAR) * (m_url.length() + 1));
				MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, oldurlstr, -1, str3, m_url.length() + 1);
				LPCWSTR urlWide = str3;

				hSession = WinHttpOpen (
					L"HTTP/1.1", 
					WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
					WINHTTP_NO_PROXY_NAME, 
					WINHTTP_NO_PROXY_BYPASS, 
					0
				);

				if ( hSession ) {
					hConnect = WinHttpConnect( 
						hSession, 
						urlWide, //CStdStringW( GJAPI_SERVER ), m_url but split before 3rd forward slash, e.g. "gamejolt.com"
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
						urlWide, //CStdStringW( url ),
						NULL,
						WINHTTP_NO_REFERER,
						WINHTTP_DEFAULT_ACCEPT_TYPES,
						WINHTTP_FLAG_REFRESH
					);
				} else {
					ARK2D::getLog()->e("Could not connect to the HTTP session:");
					int err = GetLastError();
					if (err == ERROR_WINHTTP_INCORRECT_HANDLE_TYPE) {
						ARK2D::getLog()->e("ERROR_WINHTTP_INCORRECT_HANDLE_TYPE");
					}
					else if (err == ERROR_WINHTTP_INTERNAL_ERROR) {
						ARK2D::getLog()->e("ERROR_WINHTTP_INTERNAL_ERROR");
					}
					else if (err == ERROR_WINHTTP_INVALID_URL) {
						ARK2D::getLog()->e("ERROR_WINHTTP_INVALID_URL");
					}
					else if (err == ERROR_WINHTTP_OPERATION_CANCELLED) {
						ARK2D::getLog()->e("ERROR_WINHTTP_OPERATION_CANCELLED");
					}
					else if (err == ERROR_WINHTTP_UNRECOGNIZED_SCHEME) {
						ARK2D::getLog()->e("ERROR_WINHTTP_UNRECOGNIZED_SCHEME");
					}
					else if (err == ERROR_WINHTTP_SHUTDOWN) {
						ARK2D::getLog()->e("ERROR_WINHTTP_SHUTDOWN");
					}
					else if (err == ERROR_NOT_ENOUGH_MEMORY) {
						ARK2D::getLog()->e("ERROR_NOT_ENOUGH_MEMORY");
					}
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
					int err = GetLastError();
					if (err == ERROR_WINHTTP_CANNOT_CONNECT) {
						ARK2D::getLog()->e("ERROR_WINHTTP_CANNOT_CONNECT");
					}
					else if (err == ERROR_WINHTTP_CHUNKED_ENCODING_HEADER_SIZE_OVERFLOW) {
						ARK2D::getLog()->e("ERROR_WINHTTP_CHUNKED_ENCODING_HEADER_SIZE_OVERFLOW");
					}
					else if (err == ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED) {
						ARK2D::getLog()->e("ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED");
					}
					else if (err == ERROR_WINHTTP_CONNECTION_ERROR) {
						ARK2D::getLog()->e("ERROR_WINHTTP_CONNECTION_ERROR");
					}
					else if (err == ERROR_WINHTTP_HEADER_COUNT_EXCEEDED) {
						ARK2D::getLog()->e("ERROR_WINHTTP_HEADER_COUNT_EXCEEDED");
					}
					else if (err == ERROR_WINHTTP_HEADER_SIZE_OVERFLOW) {
						ARK2D::getLog()->e("ERROR_WINHTTP_HEADER_SIZE_OVERFLOW");
					}
					else if (err == ERROR_WINHTTP_INCORRECT_HANDLE_STATE) {
						ARK2D::getLog()->e("ERROR_WINHTTP_INCORRECT_HANDLE_STATE");
					}
					else if (err == ERROR_WINHTTP_INCORRECT_HANDLE_TYPE) {
						ARK2D::getLog()->e("ERROR_WINHTTP_INCORRECT_HANDLE_TYPE");
					}
					else if (err == ERROR_WINHTTP_INTERNAL_ERROR) {
						ARK2D::getLog()->e("ERROR_WINHTTP_INTERNAL_ERROR");
					}
					else if (err == ERROR_WINHTTP_INVALID_SERVER_RESPONSE) {
						ARK2D::getLog()->e("ERROR_WINHTTP_INVALID_SERVER_RESPONSE");
					}
					else if (err == ERROR_WINHTTP_INVALID_URL) {
						ARK2D::getLog()->e("ERROR_WINHTTP_INVALID_URL");
					}
					else if (err == ERROR_WINHTTP_LOGIN_FAILURE) {
						ARK2D::getLog()->e("ERROR_WINHTTP_LOGIN_FAILURE");
					}
					else if (err == ERROR_WINHTTP_NAME_NOT_RESOLVED) {
						ARK2D::getLog()->e("ERROR_WINHTTP_NAME_NOT_RESOLVED");
					}
					else if (err == ERROR_WINHTTP_OPERATION_CANCELLED) {
						ARK2D::getLog()->e("ERROR_WINHTTP_OPERATION_CANCELLED");
					}
					else if (err == ERROR_WINHTTP_REDIRECT_FAILED) {
						ARK2D::getLog()->e("ERROR_WINHTTP_REDIRECT_FAILED");
					}
					else if (err == ERROR_WINHTTP_RESEND_REQUEST) {
						ARK2D::getLog()->e("ERROR_WINHTTP_RESEND_REQUEST");
					}
					else if (err == ERROR_WINHTTP_RESPONSE_DRAIN_OVERFLOW) {
						ARK2D::getLog()->e("ERROR_WINHTTP_RESPONSE_DRAIN_OVERFLOW");
					}
					else if (err == ERROR_WINHTTP_SECURE_FAILURE) {
						ARK2D::getLog()->e("ERROR_WINHTTP_SECURE_FAILURE");
					}
					else if (err == ERROR_WINHTTP_TIMEOUT) {
						ARK2D::getLog()->e("ERROR_WINHTTP_TIMEOUT");
					}
					else if (err == ERROR_WINHTTP_UNRECOGNIZED_SCHEME) {
						ARK2D::getLog()->e("ERROR_WINHTTP_UNRECOGNIZED_SCHEME");
					}
					else if (err == ERROR_NOT_ENOUGH_MEMORY) {
						ARK2D::getLog()->e("ERROR_NOT_ENOUGH_MEMORY");
					}
					
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
								m_response += outputBuffer;

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

				return m_response;
 */
			//#elif defined (ARK2D_ANDROID) 
			#if defined (ARK2D_ANDROID)      
				//! todo: HTTP POST args on Android. 
				m_response = "";
				if (m_threaded) { 
					m_response += ARK2D::getContainer()->m_platformSpecific.m_pluggable->urlRequestThreaded(m_url);
				} else {
					m_response += ARK2D::getContainer()->m_platformSpecific.m_pluggable->urlRequest(NULL, m_url);
				}
				return m_response;
            #elif defined(ARK2D_IPHONE) 
                //return "Not implemented";

				NSString* urlNSStr = [NSString stringWithCString:m_url.c_str() encoding:[NSString defaultCStringEncoding]];
				NSString* properlyEscapedURL = [urlNSStr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];

				NSURL* url = [NSURL URLWithString:properlyEscapedURL];
				NSURLRequest* request = [NSURLRequest requestWithURL:url]; 

				NSURLResponse* response = nil;
				NSError* requestError = nil;

				//send it synchronous
				NSData* responseData = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&requestError];
				
				
				if (responseData == nil) {
					// Check for problems
					if (requestError != nil) {
						ARK2D::getLog()->e(m_url);  
						//NSLog(@"%@", requestError);

						NSString* nserrorStr = [requestError localizedDescription];
						m_error = string( [nserrorStr UTF8String] );
					}
				}
				else {
					// Data was received.. continue processing
					NSString* responseString = [[NSString alloc] initWithData:responseData encoding:NSUTF8StringEncoding];
					//NSLog(@"Response from server = %@", responseString);
					m_response = string([responseString UTF8String]);
					ARK2D::getLog()->v(m_response);   
				}
				
				return m_response;

           	#elif defined(ARK2D_FLASCC)

				m_error = "Not implemented!";
                return "Not implemented!";
                 
                /*
                inline_as3(
                	"import com.adobe.flascc.Console;\n"\
                	"Console.s_urlResponse = \"\";"
                );

                const char* str = m_url.c_str();

				inline_as3(
					"import com.adobe.flascc.Console;\n"\
					"Console.urlRequest(CModule.readString(%0, %1));\n"\
					: : "r"(str), "r"(strlen(str))
				);

				int done = 0;
				while(done == 0) {
					inline_as3(
						"import com.adobe.flascc.Console;\n"\
						"%0 = (Console.s_urlResponse == \"\")?0:1;\n" 
						: "=r"(done) : 
					);
				}
				
				char* wordptrs = NULL;
				inline_as3(
			        "var stringptr:int = CModule.mallocString(Console.s_urlResponse);\n"
			        "CModule.write32(%0, stringptr);\n"
			        : : "r"(&wordptrs)
			    ); 
				printf(">>> %s\n", wordptrs);

				m_response = string(wordptrs);

				free(wordptrs);
				return m_response;*/

			#elif defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_XBOXONE)
				m_error = "Synchronous API not possible. Do threaded request!";
				return "Not Implemented!"; 

				


			//#elif defined(ARK2D_WINDOWS) 
			//	m_error = "Synchronous API not possible. Do threaded request!";
			//	ARK2D::getLog()->e(m_error);
			//	return "Not Implemented!"; 
            #else
 
				//String returnString("");
				CURLcode res;
 
				curl_easy_setopt(s_curl, CURLOPT_NOPROGRESS, 1);
				#ifndef ARK2D_WINDOWS	
					curl_easy_setopt(s_curl, CURLOPT_TIMEOUT, (long) m_timeout);
				#endif
				curl_easy_setopt(s_curl, CURLOPT_URL, m_url.c_str());
				curl_easy_setopt(s_curl, CURLOPT_WRITEFUNCTION, &URLRequest::s_curlWriteDataFunction);
				curl_easy_setopt(s_curl, CURLOPT_WRITEDATA, this); 

				if (m_cookies.size() > 0) {
					string cookieStr = "";

					map<string, string>::iterator iter;
					for (iter = m_cookies.begin(); iter != m_cookies.end(); iter++) {
						cookieStr += iter->first;
						cookieStr += "=";
						cookieStr += iter->second;
						iter++;
						if (iter != m_cookies.end()) {
							cookieStr += "; ";
						} else {
							cookieStr += ";";
						}
						iter--;
					}
 
					curl_easy_setopt(s_curl, CURLOPT_COOKIE, cookieStr.c_str());
				}

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
				

				if (http_code != 200) {
					if (http_code == 0) {
						m_error = "No internet connection. Please try again.";
						ARK2D::getLog()->e(m_error);
					} else { 
						m_error = StringUtil::append("error http code: ", (int) http_code);
						ARK2D::getLog()->e(m_error);
					}
				}
				//returnString += (int) http_code;

				ARK2D::getLog()->v(m_response);

				//returnString += ", ";
				//returnString += res;
				//returnString += ", ";
				//returnString += m_response;
				//returnString += ".";
				//return returnString.get();
				return m_response;
			#endif
			return "";
		}
		string URLRequest::getResponse() {
			return m_response;
		}

		size_t URLRequest::writeFunction(void *ptr, size_t size, size_t nmemb) {
			
			#if defined( ARK2D_WINDOWS_PHONE_8 ) || defined(ARK2D_WINDOWS)
				//string newbuff = string(buf);
				m_response.append((char*) ptr, size * nmemb);
				//free(buf);
			#else

			unsigned int bufsize = size*nmemb + 1;
			#if defined( ARK2D_WINDOWS_PHONE_8 ) || defined(ARK2D_WINDOWS)
				char* buf = (char*)alloca(bufsize);
			#else 
				char buf[size*nmemb + 1];
			#endif 



			char* pbuf = &buf[0];
			memset(buf, '\0', bufsize);
			size_t i = 0;
			for (; i < nmemb; i++) {
			#if defined( ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_WINDOWS)
				strncpy_s(pbuf, bufsize, (char*)ptr, size);
			#else
				strncpy(pbuf, (char*)ptr, size);
			#endif
				pbuf += size;
				ptr = ((char*)ptr) + size;
			}


				m_response += string(buf);
			#endif

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
