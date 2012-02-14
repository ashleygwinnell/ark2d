/*
 * ARKLog.h
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

struct GameContainer;
struct Graphics;
struct ARKString;

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
		static const unsigned int TYPE_GAME = 3;
		static const unsigned int TYPE_INFORMATION = 2;
		static const unsigned int TYPE_WARNING = 1;
		static const unsigned int TYPE_ERROR = 0;

	protected:
		list<ARKLogMessage> m_messages;
		unsigned int m_maxMessages;

		bool m_visible;
		bool m_enabled;

	public:
		ARKLog();
		void message(string s, unsigned int type);
		void e(const char* s);
		void w(const char* s);
		void i(const char* s);
		void g(const char* s);
		void e(string s);
		void w(string s);
		void i(string s);
		void g(string s);
		void e(ARKString s);
		void w(ARKString s);
		void i(ARKString s);
		void g(ARKString s);
		void update();
		void render();
		virtual ~ARKLog();

	protected:
		string getTypeString(unsigned int type);
};

#endif /* ARKLOG_H_ */
