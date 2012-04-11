/*
 * Log.h
 *
 *  Created on: Oct 27, 2011
 *      Author: ashleygwinnell
 */

#ifndef ARKLOG_H_
#define ARKLOG_H_

#include "../ARK2D.h"
//#ifndef ARK2D_ANDROID
//	#include <string>
//	#include <list>
//#endif

struct Graphics;
#include "../Namespaces.h"

using namespace std;

namespace ARK {
	namespace Util {

		class LogMessage {
			public:
				string message;
				unsigned int level;
				LogMessage(): message(""), level(0) {

				}
		};


		/*!
		 * \brief Standard framework logging - contains UI element for log viewing and debugging.
		 *
		 * This class can be extended and overridden to replace the standard log.
		 * Use ARK2D::getLog();
		 *
		 * @see ARK2D
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class Log {
			friend class ARK::Core::GameContainer;

			public:
				static const unsigned int TYPE_GAME = 3;
				static const unsigned int TYPE_INFORMATION = 2;
				static const unsigned int TYPE_WARNING = 1;
				static const unsigned int TYPE_ERROR = 0;

			protected:
				list<LogMessage> m_messages;
				unsigned int m_maxMessages;

				bool m_visible;
				bool m_enabled;

			public:
				Log();
				void message(string s, unsigned int type);
				void e(const char* s);
				void w(const char* s);
				void i(const char* s);
				void g(const char* s);
				void e(string s);
				void w(string s);
				void i(string s);
				void g(string s);
				void e(String s);
				void w(String s);
				void i(String s);
				void g(String s);
				void update();
				void render();
				virtual ~Log();

			protected:
				string getTypeString(unsigned int type);
		};

	}
}

#endif /* ARKLOG_H_ */
