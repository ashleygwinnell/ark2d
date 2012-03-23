/*
 * ComboBoxItem.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#include "ComboBoxItem.h"
#include "ComboBox.h"

namespace ARK {
	namespace UI {

		ComboBoxItem::ComboBoxItem(): AbstractUIComponent(), parent(NULL), m_text("") {
			m_width = 20;
		}

		void ComboBoxItem::setText(string t) {
			m_text = t;
		}
		const string& ComboBoxItem::getText() {
			return m_text;
		}

		void ComboBoxItem::setValue(string t) {
			m_value = t;
		}
		const string& ComboBoxItem::getValue() {
			return m_value;
		}

		void ComboBoxItem::keyPressed(unsigned int key) {
			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {
				Input* i = ARK2D::getInput();
				if (GigaRectangle<int>::s_contains(m_x, m_y, (signed int) (m_width), (signed int)(m_height), (signed int) (i->getMouseX()), (signed int) (i->getMouseY()))) {
					parent->m_selected = this;
					if (parent->m_itemChangedEvent != NULL) {
						void (*pt)() = (void(*)()) parent->m_itemChangedEvent;
						pt();
					}
				}
			}
		}

		void ComboBoxItem::render() {
			renderBackground();
			renderOverlay();

			Graphics* g = ARK2D::getGraphics();
			g->drawString(m_text, m_x, m_y);
		}
		void ComboBoxItem::renderBackground() {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::black_50a);
			g->fillRect(m_x, m_y, m_width, m_height);
		}
		void ComboBoxItem::renderOverlay() {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::white);
			g->drawRect(m_x, m_y, m_width, m_height);
		}
	}
}
