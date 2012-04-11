/*
 * Log.cpp
 *
 *  Created on: Oct 27, 2011
 *      Author: ashleygwinnell
 */

#include "Log.h"

#include "../ARK2D.h"
#include "../Core/GameContainer.h"
#include "../Core/String.h"
#include "../Graphics/Renderer.h"

namespace ARK {
	namespace Util {

		Log::Log():
			m_messages(),
			m_maxMessages(64),
			m_visible(false),
			m_enabled(true)
			{

		}

		void Log::message(string s, unsigned int type) {

			LogMessage item;
			item.level = type;
			item.message = s;
			m_messages.push_back(item);

			#if defined(ARK2D_ANDROID)
				__android_log_print(ANDROID_LOG_INFO, "ARK2D", s.c_str());
			#else
				std::cout << "Log " << getTypeString(type) << ": " << s << std::endl;
			#endif
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
		void Log::g(const char* s) {
			string str(s);
			message(str, TYPE_GAME);
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
		void Log::g(string s) {
			message(s, TYPE_GAME);
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
		void Log::g(String s) {
			message(s.get(), TYPE_GAME);
		}
		void Log::update() {
			Input* i = ARK2D::getInput();
			if (i->isKeyPressed(Input::KEY_D) && i->isKeyDown(Input::KEY_LSHIFT)) {
				m_visible = !m_visible;
			}
		}
		void Log::render() {

			// remove old items
			if (m_messages.size() > m_maxMessages) {
				int removeCount = m_messages.size() - m_maxMessages;
				for(; removeCount > 0; removeCount--) {
					m_messages.pop_front();
				}
			}

			if (!m_visible) {
				return;
			}

			GameContainer* container = ARK2D::getContainer();
			Renderer* g = ARK2D::getRenderer();
			ARK::Font::Font* oldFont = g->getFont();
			const Color& oldColor = g->getDrawColor();

			g->setDrawColor(Color::black_50a);
			g->fillRect(0, 0, container->getWidth(), container->getHeight());

			ARK::Font::Font* defaultFont = g->getDefaultFont();
			g->setDrawColor(Color::white);
			g->setFont(defaultFont);



			list<LogMessage>::iterator it;
			int actualHeight = 0;
			for(it = m_messages.begin(); it != m_messages.end(); it++) {
				actualHeight += defaultFont->getLineHeight();
			}
			int y = 0;
			if (actualHeight > (signed int) container->getHeight()) { y -= (actualHeight - container->getHeight()); }
			for(it = m_messages.begin(); it != m_messages.end(); it++) {
				LogMessage m = (*it);
				g->drawString(m.message, 0, y);
				y += defaultFont->getLineHeight();
			}

			g->setDrawColor(oldColor);
			g->setFont(oldFont);
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
				}
				return "";
			#endif
		}

		Log::~Log() {

		}

	}
}
