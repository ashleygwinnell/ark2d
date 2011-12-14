/*
 * ARKLog.cpp
 *
 *  Created on: Oct 27, 2011
 *      Author: ashleygwinnell
 */

#include "ARKLog.h"

#include "../ARK2D.h"
#include "../GameContainer.h"
#include "../Graphics.h"
#include "../ARKString.h"

ARKLog::ARKLog():
	m_messages(),
	m_maxMessages(64),
	m_visible(false),
	m_enabled(true)
	{

}

void ARKLog::message(string s, unsigned int type) {
	ARKLogMessage item;
	item.level = type;
	item.message = s;
	m_messages.push_back(item);
	std::cout << "Log " << getTypeString(type) << ": " << s << std::endl;
}
void ARKLog::e(const char* s) {
	string str(s);
	message(str, TYPE_ERROR);
}
void ARKLog::w(const char* s) {
	string str(s);
	message(str, TYPE_WARNING);
}
void ARKLog::i(const char* s) {
	string str(s);
	message(str, TYPE_INFORMATION);
}
void ARKLog::g(const char* s) {
	string str(s);
	message(str, TYPE_GAME);
}
void ARKLog::e(string s) {
	message(s, TYPE_ERROR);
}
void ARKLog::w(string s) {
	message(s, TYPE_WARNING);
}
void ARKLog::i(string s) {
	message(s, TYPE_INFORMATION);
}
void ARKLog::g(string s) {
	message(s, TYPE_GAME);
}
void ARKLog::e(ARKString s) {
	message(s.get(), TYPE_ERROR);
}
void ARKLog::w(ARKString s) {
	message(s.get(), TYPE_WARNING);
}
void ARKLog::i(ARKString s) {
	message(s.get(), TYPE_INFORMATION);
}
void ARKLog::g(ARKString s) {
	message(s.get(), TYPE_GAME);
}
void ARKLog::update() {
	Input* i = ARK2D::getInput();
	if (i->isKeyPressed(Input::KEY_D) && i->isKeyDown(Input::KEY_LSHIFT)) {
		m_visible = !m_visible;
	}
}
void ARKLog::render() {


	if (!m_visible) {
		return;
	}

	GameContainer* container = ARK2D::getContainer();
	Graphics* g = ARK2D::getGraphics();
	BMFont* oldFont = g->getFont();
	const Color& oldColor = g->getDrawColor();

	g->setDrawColor(Color::black_50a);
	g->fillRect(0, 0, container->getWidth(), container->getHeight());

	BMFont* defaultFont = g->getDefaultFont();
	g->setDrawColor(Color::white);
	g->setFont(defaultFont);

	// remove old items
	if (m_messages.size() > m_maxMessages) {
		int removeCount = m_messages.size() - m_maxMessages;
		for(; removeCount > 0; removeCount--) {
			m_messages.pop_front();
		}
	}

	list<ARKLogMessage>::iterator it;
	int actualHeight = 0;
	for(it = m_messages.begin(); it != m_messages.end(); it++) {
		actualHeight += defaultFont->getLineHeight();
	}
	int y = 0;
	if (actualHeight > (signed int) container->getHeight()) { y -= (actualHeight - container->getHeight()); }
	for(it = m_messages.begin(); it != m_messages.end(); it++) {
		ARKLogMessage m = (*it);
		g->drawString(m.message, 0, y);
		y += defaultFont->getLineHeight();
	}

	g->setDrawColor(oldColor);
	g->setFont(oldFont);
}

string ARKLog::getTypeString(unsigned int type) {
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
}

ARKLog::~ARKLog() {

}
