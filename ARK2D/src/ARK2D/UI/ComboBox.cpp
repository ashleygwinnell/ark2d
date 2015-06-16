/*
 * ComboBox.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#include "ComboBox.h"
#include "ComboBoxItem.h"
#include "Button.h"

namespace ARK {
	namespace UI {
		ComboBox::ComboBox():
			AbstractUIComponent(),
			SceneNode(),
			m_items(),
			m_selected(NULL),
			m_open(false),
			m_state(Button::STATE_OFF),
			m_itemChangedEvent(NULL) {

		}
		void ComboBox::addItem(ComboBoxItem* cbi) {
			if (m_items.size() == 0) {
				m_selected = cbi;
			}
			cbi->parent = this;
			m_items.push_back(cbi);
		}
		void ComboBox::setOpen(bool b) {
			m_open = b;

			if (m_open) {
				int newHeight = m_height;
				for(unsigned int i = 0; i < m_items.size(); i++) {
					ComboBoxItem* item = m_items.at(i);
					item->setSize(m_width, ARK2D::getRenderer()->getFont()->getLineHeight());
					item->m_parent = m_parent;
					newHeight += item->getHeight();
				}
				m_height = newHeight;
			} else {
				m_height = m_originalHeight;
			}
		}
		bool ComboBox::isOpen() {
			return m_open;
		}
		ComboBoxItem* ComboBox::getSelected() {
			return m_selected;
		}
		void ComboBox::setSelectedByValue(string value) {
			for(unsigned int i = 0; i < m_items.size(); i++) {
				if (value == m_items.at(i)->m_value) {
					m_selected = m_items.at(i);
					break;
				}
			}
		}

		void ComboBox::setItemChangedEvent(void* event) {
			m_itemChangedEvent = event;
		}
		void ComboBox::setSize(unsigned int w, unsigned int h) {
			AbstractUIComponent::setSize(w, h);
			m_originalHeight = h;
		}

		void ComboBox::keyPressed(unsigned int key) {
			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {

				if (m_open) {
					for(unsigned int i = 0; i < m_items.size(); i++) {
						m_items.at(i)->keyPressed(key);
					}
				}

				if (isMouseInBounds()) {
					setOpen(!isOpen());
				} else {
					setOpen(false);
				}

			}

		}
		void ComboBox::mouseMoved(int x, int y, int oldx, int oldy) {
			if (isMouseInBounds()) {
				m_state = Button::STATE_OVER;
			} else {
				m_state = Button::STATE_OFF;
			}
			for(unsigned int i = 0; i < m_items.size(); i++) {
				m_items.at(i)->mouseMoved(x,y,oldx,oldy);
			}
		}
		void ComboBox::keyReleased(unsigned int key) {

		}
		void ComboBox::render() {
			renderBackground();
			renderArrow();
			renderOverlay();

			Renderer* g = ARK2D::getRenderer();
			if (m_selected != NULL) {
				g->drawString(m_selected->m_text, m_x, m_y);
			}

			if (m_open) {
				int y = m_y + m_originalHeight;
				for(unsigned int i = 0; i < m_items.size(); i++) {
					m_items.at(i)->setLocation(m_x, y);
					m_items.at(i)->render();
					y += m_items.at(i)->getHeight();
				}
			}
		}
		void ComboBox::renderBackground() {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::black_50a);
			g->fillRect(m_x, m_y, m_width, m_height);
		}
		void ComboBox::renderArrow() {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::white_50a);
			if (m_open) {
				g->fillTriangle(m_x + m_width - 20, m_y+10, 10, 10);
			} else {
				g->fillTriangle(m_x + m_width - 20, m_y+20, 10, -10);
			}

		}
		void ComboBox::renderText(int x, int y) {

		}
		void ComboBox::renderOverlay() {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::white);
			if (m_state == Button::STATE_OVER) {
				g->setDrawColor(Color::white_50a);
			}
			g->drawRect(m_x, m_y, m_width, m_height);
		}
	}
}
