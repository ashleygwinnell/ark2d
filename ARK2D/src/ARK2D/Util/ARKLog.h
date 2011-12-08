/*
 * ARKLog.h
 *
 *  Created on: Oct 27, 2011
 *      Author: ashleygwinnell
 */

#ifndef ARKLOG_H_
#define ARKLOG_H_

#include "../../ARK.h"
#include <string>

using namespace std;

class ARKLogMessage {
	public:
		string message;
		unsigned int level;
		ARKLogMessage(): message(""), level(0) {

		}
};

class ARKLog {
	friend class GameContainer;
	public:
		static const unsigned int TYPE_INFORMATION = 0;
		static const unsigned int TYPE_WARNING = 1;
		static const unsigned int TYPE_ERROR = 2;
	private:
		static list<ARKLogMessage> s_messages;
		static unsigned int s_maxMessages;

		static bool s_visible;
		static bool s_enabled;

	public:
		static void message(string s, unsigned int type);
		static void e(const char* s);
		static void w(const char* s);
		static void i(const char* s);
		static void e(string s);
		static void w(string s);
		static void i(string s);
		static void e(ARKString s);
		static void w(ARKString s);
		static void i(ARKString s);
		static void update();
		static void render();

	private:
		static string getTypeString(unsigned int type);
};

#endif /* ARKLOG_H_ */
