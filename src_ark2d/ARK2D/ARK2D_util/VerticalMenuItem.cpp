/*
 * VerticalMenuItem.cpp
 *
 *  Created on: 17 Aug 2010
 *      Author: Ashley
 */

#include "VerticalMenuItem.h"
#include "../Event.h"

VerticalMenuItem::VerticalMenuItem():
	m_visible(true),
	m_on_press_event(NULL) {

}
VerticalMenuItem::VerticalMenuItem(const char* text):
	m_text(text),
	m_visible(true),
	m_on_press_event(NULL) {

}
void VerticalMenuItem::setOnPressEvent(Event* e) {
	m_on_press_event = e;
}
Event* VerticalMenuItem::getOnPressEvent() {
	return m_on_press_event;
}
void VerticalMenuItem::setText(const char* text) {
	m_text = std::string(text);
}
void VerticalMenuItem::setVisible(bool b) {
	m_visible = b;
}

bool VerticalMenuItem::isVisible() {
	return m_visible;
}

const char* VerticalMenuItem::getText() {
	return m_text.c_str();
}

VerticalMenuItem::~VerticalMenuItem() {

}
