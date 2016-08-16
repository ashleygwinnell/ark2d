/*
 * ComboBoxItem.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#include "ComboBoxItem.h"
#include "ComboBox.h"
#include "../Core/ARK2D.h"
#include "Button.h"

namespace ARK {
	namespace UI {

		ComboBoxItem::ComboBoxItem():
			AbstractUIComponent(),
            parent(NULL),
			m_text("") {
			m_width = 20;
		}
		ComboBoxItem::ComboBoxItem(string text, string value):
			AbstractUIComponent(),
			parent(NULL),
			m_text(text),
			m_value(value) {
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

		bool ComboBoxItem::keyPressed(unsigned int key) {
			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {
				if (m_state == Button::STATE_OVER) {
					m_state = Button::STATE_DOWN;
					return true;
				}
			}
			return false;
		}
		bool ComboBoxItem::keyReleased(unsigned int key) {
			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {
				Input* in = ARK2D::getInput();
                bool mib = isGlobalPositionInBounds(in->getMouseX(), in->getMouseY());
				if (mib && m_state == Button::STATE_DOWN) {
					ARK2D::getLog()->i("clicky");
				//if (GigaRectangle<int>::s_contains(m_x, m_y, (signed int) (m_width), (signed int)(m_height), (signed int) (i->getMouseX()), (signed int) (i->getMouseY()))) {
					parent->m_selected = this;
					if (parent->m_itemChangedEvent != NULL) {
						void (*pt)(ComboBox*) = (void(*)(ComboBox*)) parent->m_itemChangedEvent;
						pt(parent);
                        
					}
					m_state = Button::STATE_OFF;
					return true;
				}
                m_state = Button::STATE_OFF;
			}
            return false;
		}
		bool ComboBoxItem::mouseMoved(int x, int y, int oldx, int oldy) {
            return AbstractUIComponent::mouseMoved(x, y, oldx, oldy);
            
		}

		void ComboBoxItem::render() {
			renderBackground();
			renderOverlay();

			Renderer* g = ARK2D::getRenderer();
			float tx = 0;
			float ty = 0;
			if (m_state == Button::STATE_DOWN) {
				tx += 2;
				ty += 2;
			} 
			g->drawString(m_text, tx, ty);
		}
		void ComboBoxItem::renderBackground() {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::black_50a);
			g->fillRect(0, 0, m_width, m_height);
		}
		void ComboBoxItem::renderOverlay() {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::white);
			if (m_state == Button::STATE_OVER || m_state == Button::STATE_DOWN) {
				g->setDrawColor(Color::white_50a);
			}
			g->drawRect(0, 0, m_width, m_height);
		}
	}
}
