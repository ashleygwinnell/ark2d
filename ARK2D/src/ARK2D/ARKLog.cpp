/*
 * ARKLog.cpp
 *
 *  Created on: Oct 27, 2011
 *      Author: ashleygwinnell
 */

#include "ARKLog.h"

list<ARKLogMessage> ARKLog::s_messages;
unsigned int ARKLog::s_maxMessages = 64;

bool ARKLog::s_enabled = true;
bool ARKLog::s_visible = false;

void ARKLog::message(string s, unsigned int type) {
	ARKLogMessage item;
	item.level = type;
	item.message = s;
	s_messages.push_back(item);
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
void ARKLog::e(string s) {
	message(s, TYPE_ERROR);
}
void ARKLog::w(string s) {
	message(s, TYPE_WARNING);
}
void ARKLog::i(string s) {
	message(s, TYPE_INFORMATION);
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
void ARKLog::update() {
	Input* i = ARK2D::getInput();
	if (i->isKeyPressed(Input::KEY_D)) {
		s_visible = !s_visible;
	}
}
void ARKLog::render() {


	if (!s_visible) {
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
	if (s_messages.size() > s_maxMessages) {
		int removeCount = s_messages.size() - s_maxMessages;
		for(; removeCount > 0; removeCount--) {
			s_messages.pop_front();
		}
	}

	list<ARKLogMessage>::iterator it;
	int actualHeight = 0;
	for(it = s_messages.begin(); it != s_messages.end(); it++) {
		actualHeight += defaultFont->getLineHeight();
	}
	int y = 0;
	if (actualHeight > (signed int) container->getHeight()) { y -= (actualHeight - container->getHeight()); }
	for(it = s_messages.begin(); it != s_messages.end(); it++) {
		ARKLogMessage m = (*it);
		g->drawString(m.message, 0, y);
		y += defaultFont->getLineHeight();
	}

	g->setDrawColor(oldColor);
	g->setFont(oldFont);
}
