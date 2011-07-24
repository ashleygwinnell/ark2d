/*
 * Label.cpp
 *
 *  Created on: 23 Jul 2011
 *      Author: Ashley
 */

#include "Label.h"

Label::Label():
	AbstractUIComponent(),
	m_text("") {

}

Label::Label(string text):
	AbstractUIComponent(),
	m_text(text) {

}
void Label::setText(string text) {
	m_text = text;
}

void Label::render() {
	Graphics* g = ARK2D::getGraphics();
	g->drawString(m_text, m_x, m_y);
}
void Label::renderBounds() {
	Graphics* g = ARK2D::getGraphics();
	g->drawRect(m_x, m_y, m_width, m_height);
}

Label::~Label() {

}
