/*
 * Log.cpp
 *
 *  Created on: Oct 27, 2011
 *      Author: ashleygwinnell
 */

#include "Log.h"

#include "ARK2D.h"
#include "String.h"
#include "GameContainer.h"
#include "Graphics/Renderer.h"
#include "Strings.h"
#include "../UI/Slider.h"
#include "../UI/CheckBox.h"
#include "../UI/Label.h"
#include "../UI/ComboBox.h"
#include "../UI/ComboBoxItem.h"
#include "../UI/SplitPane.h"
#include "../UI/TitledPanel.h"

#include <algorithm>
#include <ostream>
#include <fstream>

#ifdef ARK2D_MACINTOSH
    #include <execinfo.h>
#endif

namespace ARK {
	namespace Core {

		Log* Log::s_instance = NULL;

		Log* Log::getInstance() {
			if (s_instance == NULL) {
				s_instance = new Log();
			}
			return s_instance;
		}

		Log::Log():
			m_messages(),
			m_messagesPool(),
			m_maxMessages(1024),

			m_watchedVariables(),

			m_filter(TYPE_ALL),
			m_logToFile(false)
			{
			//this->visible = false;
		}

		void Log::init() {
			for(unsigned int i = 0; i < m_maxMessages; i++) {
				LogMessage* item = new LogMessage();
				m_messagesPool.push_back(item);
			}
		}

		#define  LOGLOGLOG(...)  __android_log_print(ANDROID_LOG_INFO,"ARK2D",__VA_ARGS__)

		#if defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_WINDOWS_VS)
			void WINPHONELOG(const char* ch, int len) {

				#if NTDDI_PHONE8
					LPCSTR lps = ch;
					OutputDebugString(lps);
					OutputDebugString("\r\n");
				#elif NTDDI_WINBLUE
					#if defined(ARK2D_WINDOWS)
						LPCSTR lps = ch;
						OutputDebugString(lps);
						OutputDebugString("\r\n");
					#else
						WCHAR str[1024];
						MultiByteToWideChar(CP_UTF8, 0, ch, len, str, 1024);
						str[len] = L'\0';
						LPCWSTR lps = str;
						OutputDebugString(lps);
						OutputDebugString(L"\r\n");
					#endif
				#endif

			}
		#endif

		void Log::setFilter(unsigned int level) {
			if (level != m_filter) {
				message(StringUtil::append("Changing Log filter to ", level), TYPE_ERROR);
			}
			m_filter = level;
		}
		unsigned int Log::getFilter() {
			return m_filter;
		}

		void Log::message(string s, unsigned int type) {

			/*#if !defined(ARK2D_DEBUG)
				if (type == TYPE_VERBOSE) {
					return;
				}
			#endif*/

			if (type > m_filter) {
				return;
			}

			#if defined(ARK2D_ANDROID)
				if (s.length() < 1000) {
					LOGLOGLOG("%s: %s", getTypeString(type).c_str(), s.c_str());
				} else {
					LOGLOGLOG("%s: %s", getTypeString(type).c_str(), "Long string: ");
					for (unsigned int i = 0; i < s.length(); i += 1024) {
						LOGLOGLOG("%s",  s.substr(i, 1024).c_str());
					}
				}

			#elif defined(ARK2D_FLASCC)
				/*string newStr = StringUtil::append(getTypeString(type), s);
				printf("Log ");
				printf(getTypeString(type).c_str());
				printf(": ");
				printf(s.c_str());
				printf("\n");*/
			#elif defined(ARK2D_WINDOWS_PHONE_8)

				if (s.length() < 1023) {
					WINPHONELOG(s.c_str(), s.length());
				}
				else {
					WINPHONELOG("Long string: ", 13);
					for (unsigned int i = 0; i < s.length(); i += 1023) {
						WINPHONELOG(s.substr(i, 1023).c_str(), s.substr(i, 1023).length());
					}
				}

			#elif defined(ARK2D_WINDOWS_VS)

				std::cout << "Log " << getTypeString(type) << ": " << s << std::endl;
				if (s.length() < 1023) {
					WINPHONELOG(s.c_str(), s.length());
				}
				else {
					WINPHONELOG("Long string: ", 13);
					for (unsigned int i = 0; i < s.length(); i += 1023) {
						WINPHONELOG(s.substr(i, 1023).c_str(), s.substr(i, 1023).length());
					}
				}



			#else
				std::cout << "Log " << getTypeString(type) << ": " << s << std::endl;
			#endif

			if (type == TYPE_THREAD) { return; }

			#ifndef ARK2D_WINDOWS_STORE

				if (m_messagesPool.size() == 0) {
					for(unsigned int i = 0; i < 6; i++) {
						LogMessage* item = new LogMessage();
						m_messagesPool.push_back(item);
					}
				}

				LogMessage* item = m_messagesPool.at(m_messagesPool.size()-1);
				if (item == NULL) {
					item = new LogMessage();
					m_messagesPool.push_back(item);
				}
				item->level = type;
				if (s.length() < 1024) {
					item->message = s;
				} else {
					item->message = s.substr(0, 1024);
				}
				m_messages.push_back(item);
				m_messagesPool.pop_back();
			#endif


			if (isLoggingToFile()) {
				std::ofstream log_file("log.txt", std::ios_base::out | std::ios_base::app );
				log_file << "Log " << getTypeString(type) << ": " << s.c_str() << std::endl;
			}


		}
		void Log::e(const char* s) {
			string str(s);
			message(str, TYPE_ERROR);
		}
		void Log::w(const char* s) {
			string str(s);
			message(str, TYPE_WARNING);
		}
		void Log::i(const char* s) {
			string str(s);
			message(str, TYPE_INFORMATION);
		}
		void Log::v(const char* s) {
			string str(s);
			message(str, TYPE_VERBOSE);
		}
		void Log::g(const char* s) {
			string str(s);
			message(str, TYPE_GAME);
		}
		void Log::t(const char* s) {
			string str(s);
			message(str, TYPE_THREAD);
		}

		void Log::e(string s) {
			message(s, TYPE_ERROR);
		}
		void Log::w(string s) {
			message(s, TYPE_WARNING);
		}
		void Log::i(string s) {
			message(s, TYPE_INFORMATION);
		}
		void Log::v(string s) {
			message(s, TYPE_VERBOSE);
		}
		void Log::g(string s) {
			message(s, TYPE_GAME);
		}
		void Log::t(string s) {
			message(s, TYPE_THREAD);
		}


		void Log::e(String s) {
			message(s.get(), TYPE_ERROR);
		}
		void Log::w(String s) {
			message(s.get(), TYPE_WARNING);
		}
		void Log::i(String s) {
			message(s.get(), TYPE_INFORMATION);
		}
		void Log::v(String s) {
			message(s.get(), TYPE_VERBOSE);
		}
		void Log::g(String s) {
			message(s.get(), TYPE_GAME);
		}
		void Log::t(String s) {
			message(s.get(), TYPE_THREAD);
		}

		void Log::backtrace() {
			#ifdef ARK2D_MACINTOSH
				e("--- BACKTRACE ---");

				void* callstack[128];
                int i, frames = ::backtrace(callstack, 128);
				char** strs = backtrace_symbols(callstack, frames);
				for (i = 0; i < frames; ++i) {
					printf("%s\n", strs[i]);
				}
				free(strs);

				//e("--- BACKTRACE SYMBOLS ---");
				//backtrace_symbols();
			#endif
		}


		void Log::watchVariable(string name, unsigned int type, void* data) {
			// check the memory location/variable is not being watched already!
			for(unsigned int i = 0; i < m_watchedVariables.size(); i++) {
				if (m_watchedVariables[i]->data == data) {
					ARK2D::getLog()->w(StringUtil::append("Variable is already being watched: ", name));
					return;
				}
			}

			WatchedVariable* wv = new WatchedVariable();
			wv->name = name;
			wv->type = type;
			wv->data = data;
			m_watchedVariables.push_back(wv);
		}
		void Log::addWatchedVariable(string name, unsigned int type, void* data) {
			watchVariable(name, type, data);
		}
		void Log::clearWatchedVariables() {
			for(unsigned int i = 0; i < m_watchedVariables.size(); ++i) {
				WatchedVariable* wv = m_watchedVariables[i];
				delete wv;
				wv = NULL;
			}
			m_watchedVariables.clear();
		}

		void Log::update() {
			// remove old items
			if (m_messages.size() > m_maxMessages) {
				int removeCount = m_messages.size() - m_maxMessages;
				for(; removeCount > 0; removeCount--) {
					LogMessage* item = m_messages.front();
					m_messages.pop_front();
					m_messagesPool.push_back(item);
				}
			}
		}

		bool Log::isLoggingToFile() {
			return m_logToFile;
		}
		void Log::setLoggingToFile(bool b) {
			m_logToFile = b;
		}

   		wstring Log::getTypeWString(unsigned int type) {
            return StringUtil::stringToWstring(getTypeString(type));
        }
		string Log::getTypeString(unsigned int type) {
			#if defined(ARK2D_ANDROID)
				switch(type) {
					case ANDROID_LOG_INFO:
						return "INFO";
						break;
				}
				return "";
			#else
				switch(type) {
					case TYPE_ERROR:
						return "ERROR";
						break;
					case TYPE_WARNING:
						return "WARNING";
						break;
					case TYPE_INFORMATION:
						return "INFO";
						break;
					case TYPE_VERBOSE:
						return "VERBOSE";
						break;
					case TYPE_THREAD:
						return "THREAD";
						break;
					case TYPE_GAME:
						return "GAME";
						break;
				}
				return "";
			#endif
		}

		Log::~Log() {

		}

	}
}
