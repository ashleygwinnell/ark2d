/*
 * CheckBox.cpp
 *
 *  Created on: 24 Jul 2011
 *      Author: Ashley
 */

#include "CheckBox.h"
#include "../Graphics.h"

namespace ARK {
	namespace UI {
		CheckBox::CheckBox():
			AbstractUIComponent(),
			m_checked(false),
			m_state(STATE_OFF)
			{
			setSize(30, 30);
			m_clipping = false;
		}

		bool CheckBox::isChecked() {
			return m_checked;
		}
		void CheckBox::setChecked(bool b) {
			m_checked = b;
		}

		void CheckBox::render() {
			AbstractUIComponent::preRender();

			renderBackground();
			if (m_checked) { renderTick(); }
			renderOverlay();

			AbstractUIComponent::postRender();
		}
		void CheckBox::renderTick() {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::white);
			g->setLineWidth(2);
			g->drawLine(m_x, m_y + (m_height/2), m_x + (m_width/3), m_y + m_height);
			g->drawLine(m_x + (m_width/3), m_y + m_height, m_x + m_width, m_y);
			g->setLineWidth(1);
		}

		void CheckBox::renderBackground() {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::black_50a);
			g->fillRect(m_x, m_y, m_width, m_height);
		}

		void CheckBox::renderOverlay() {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::white);
			if (m_state == STATE_OVER || m_state == STATE_DOWN) {
				g->setDrawColor(Color::white_50a);
			}
			g->drawRect(m_x, m_y, m_width, m_height);
		}

		void CheckBox::keyPressed(unsigned int key) {
			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT && isMouseInBounds()) {
				m_state = STATE_DOWN;
				setFocussed(true);
			}
		}

		void CheckBox::keyReleased(unsigned int key) {
			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT && isMouseInBounds()) {
				if (m_state == STATE_DOWN) {
					m_state = STATE_OVER;
					m_checked = !m_checked;
				}
			} else {
				m_state = STATE_OFF;
				setFocussed(false);
			}
		}

		void CheckBox::mouseMoved(int x, int y, int oldx, int oldy) {
			if (m_state == STATE_DOWN) { return; }
			if (isMouseInBounds()) {
				m_state = STATE_OVER;
			} else {
				m_state = STATE_OFF;
			}
		}


		CheckBox::~CheckBox() {

		}
	}
}
