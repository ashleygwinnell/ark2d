/*
 * Log.h
 *
 *  Created on: Oct 27, 2011
 *      Author: ashleygwinnell
 */

#ifndef ARK_CORE_LOG_H_
#define ARK_CORE_LOG_H_

#include "../Namespaces.h"

#include "ARK2D.h"
#include "String.h"
#include "../UI/Button.h"
#include "../UI/TitledPanel.h"
#include "../UI/ScrollPanel.h"
#include "../UI/Label.h"

#include <list>
using namespace std;

#undef TYPE_BOOL

namespace ARK {
	namespace Core {

		class ARK2D_API LogMessage {
			public:
				string message;
				unsigned int level;
				LogMessage(): message(""), level(0) {

				}
		};

		class ARK2D_API WatchedVariable {
			public:
				string name;
				unsigned int type;
				void* data;

			public:
				static const unsigned int TYPE_UINT = 0;
				static const unsigned int TYPE_SINT = 1;
				static const unsigned int TYPE_FLOAT = 2;
				static const unsigned int TYPE_STR = 3;
				static const unsigned int TYPE_BOOL = 4;
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
		class ARK2D_API Log {
			friend class ARK::Core::GameContainer;
			friend class ARK2D;

			public:
				static Log* s_instance;
				static Log* getInstance();

			public:
				static const unsigned int TYPE_GAME = 4;
				static const unsigned int TYPE_VERBOSE = 3;
				static const unsigned int TYPE_INFORMATION = 2;
				static const unsigned int TYPE_WARNING = 1;
				static const unsigned int TYPE_ERROR = 0;
				static const unsigned int TYPE_THREAD = 5;
				static const unsigned int TYPE_ALL = 99999;

			protected:
				// logging
				list<LogMessage*> m_messages;
				vector<LogMessage*> m_messagesPool;
				unsigned int m_maxMessages;

				// watched variables
				vector<WatchedVariable*> m_watchedVariables;

				unsigned int m_filter;
				bool m_enabled;

				bool m_logToFile;

			public:
				Log();
				void init(); // call this just before game.init
				void message(string s, unsigned int type);

				void e(const char* s);
				void w(const char* s);
				void i(const char* s);
				void v(const char* s);
				void g(const char* s);
				void t(const char* s);

				void e(string s);
				void w(string s);
				void i(string s);
				void v(string s);
				void g(string s);
				void t(string s);

				void e(String s);
				void w(String s);
				void i(String s);
				void v(String s);
				void g(String s);
				void t(String s);

				list<LogMessage*>* getMessages() { return &m_messages; }
				unsigned int getMessageLimit() { return m_maxMessages; }

				vector<WatchedVariable*>* getWatchedVariables() { return &m_watchedVariables; }

				void backtrace();

				void watchVariable(string name, unsigned int type, void* data); // could templateise this?
				void addWatchedVariable(string name, unsigned int type, void* data); // could templateise this?
				void clearWatchedVariables();

				void setFilter(unsigned int level);
				unsigned int getFilter();

				virtual void update();

				bool isLoggingToFile();
				void setLoggingToFile(bool b);

				virtual ~Log();

			protected:
				string getTypeString(unsigned int type);
				wstring getTypeWString(unsigned int type);
		};

	}
}

#endif /* ARK_CORE_LOG_H_ */
