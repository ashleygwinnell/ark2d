/*
 * Label.cpp
 *
 *  Created on: 23 Jul 2011
 *      Author: Ashley
 */

#include "Label.h"

namespace ARK {
	namespace UI {
		Label::Label():
			AbstractUIComponent(),
			m_text("") {

		}

		Label::Label(string text):
			AbstractUIComponent(),
			m_text(text) {
			setSize(ARK2D::getGraphics()->getFont()->getStringWidth(text), ARK2D::getGraphics()->getFont()->getLineHeight());
		}
		void Label::setText(string text) {
			setText(text, false);
		}
		void Label::setText(string text, bool updateSize) {
			m_text = text;
			if (updateSize) {
				setSize(ARK2D::getGraphics()->getFont()->getStringWidth(text), ARK2D::getGraphics()->getFont()->getLineHeight());
			}
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
	}
}
