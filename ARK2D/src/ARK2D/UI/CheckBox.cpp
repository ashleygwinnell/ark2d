/*
 * CheckBox.cpp
 *
 *  Created on: 24 Jul 2011
 *      Author: Ashley
 */

#include "CheckBox.h"
#include "../Graphics/Renderer.h"

namespace ARK {
	namespace UI { 
		CheckBox::CheckBox():
			AbstractUIComponent(),
          	m_checked(false),
			m_stateChangedEvent(NULL)
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
		void CheckBox::setStateChangedEvent(void* function) {
			m_stateChangedEvent = function;
		}
		void CheckBox::doStateChangedEvent() {
			if (m_stateChangedEvent != NULL) {
				void (*pt)() = (void(*)()) m_stateChangedEvent;
				pt();
			}
		}

		void CheckBox::render() {
			renderBackground();
			if (m_checked) { renderTick(); }
			renderOverlay();
		}
		void CheckBox::renderTick() {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::white);
			g->setLineWidth(2);
			g->drawLine(0, 0 + (m_height/2), 0 + (m_width/3), 0 + m_height);
			g->drawLine(0 + (m_width/3), 0 + m_height, 0 + m_width, 0);
			g->setLineWidth(1);
		}

		void CheckBox::renderBackground() {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::black_50a);
			g->fillRect(0, 0, m_width, m_height);
		}

		void CheckBox::renderOverlay() {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::white);
			if (m_state == STATE_OVER || m_state == STATE_DOWN) {
				g->setDrawColor(Color::white_50a);
			}
			g->drawRect(0, 0, m_width, m_height);
			g->setDrawColor(Color::white);  
		}

		bool CheckBox::keyPressed(unsigned int key) {
            Input* in = ARK2D::getInput();
			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT && isGlobalPositionInBounds(in->getMouseX(), in->getMouseY())) {
				m_state = STATE_DOWN;
				setFocussed(true);
                return true;
			}
            return false;
		}

		bool CheckBox::keyReleased(unsigned int key) {
            Input* in = ARK2D::getInput();
            if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT && isGlobalPositionInBounds(in->getMouseX(), in->getMouseY())) {
				if (m_state == STATE_DOWN) {
					m_state = STATE_OVER;
					m_checked = !m_checked;
					doStateChangedEvent();
                    return true;
				}
			} else {
				m_state = STATE_OFF;
				setFocussed(false);
			}
            return false;
		}

		bool CheckBox::mouseMoved(int x, int y, int oldx, int oldy) {
			AbstractUIComponent::mouseMoved(x, y, oldx, oldy);
		}


		CheckBox::~CheckBox() {

		}
	}
}
