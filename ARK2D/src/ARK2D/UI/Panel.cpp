/*
 * Panel.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#include "Panel.h"

namespace ARK {
	namespace UI {
		Panel::Panel():
			AbstractUIComponent(), 
			SceneNode(), m_children(), m_translate(true), m_showBorder(true) {

		}

		void Panel::add(AbstractUIComponent* c) {
			c->setParent(this);
			m_children.push_back(c);
		}

		void Panel::setTranslating(bool b) {
			m_translate = b;
		}
		void Panel::setShowBorder(bool b) {
			m_showBorder = b;
		}

		void Panel::preRender() {
			AbstractUIComponent::preRender();

			if (m_translate) {
				Renderer* g = ARK2D::getRenderer();
				g->pushMatrix();
				g->translate(m_x, m_y);
			}
		}
		void Panel::postRender() {
			AbstractUIComponent::postRender();
			if (m_translate) {
				Renderer* g = ARK2D::getRenderer();
				g->popMatrix();
			}
			renderBorder();

		}
		void Panel::render() {
			if (m_visible) {
				preRender();
				renderChildren();
				postRender();
			}
		}
		void Panel::renderChildren() {
			for(unsigned int i = 0; i < m_children.size(); i++) {
				m_children.at(i)->render();
			}
		}
		void Panel::renderBorder() {
			if (m_showBorder == true) {
				Renderer* g = ARK2D::getRenderer();
				g->setDrawColor(Color::white);
				g->drawRect(m_x, m_y, m_width, m_height);
			}
		}
		bool Panel::keyPressed(unsigned int key) {
			if (!m_visible) { return false; }

			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT
					|| key == (unsigned int) Input::MOUSE_BUTTON_RIGHT) {
				Input* i = ARK2D::getInput();
				bool s = GigaRectangle<int>::s_contains(getOnScreenX(), getOnScreenY(), m_width, m_height, i->getMouseX(), i->getMouseY());
				if (s) {
					for(unsigned int i = 0; i < m_children.size(); i++) {
                        if (m_children.at(i)->keyPressed(key)) return true;
					}
				}
			} else {
				for(unsigned int i = 0; i < m_children.size(); i++) {
                    if (m_children.at(i)->keyPressed(key)) return true;
				}
			}
            return false;
		}
		bool Panel::keyReleased(unsigned int key) {
			if (!m_visible) { return false; }

			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT
					|| key == (unsigned int) Input::MOUSE_BUTTON_RIGHT) {
				Input* i = ARK2D::getInput();
				bool s = GigaRectangle<int>::s_contains(getOnScreenX(), getOnScreenY(), m_width, m_height, i->getMouseX(), i->getMouseY());
				if (s) {
					for(unsigned int i = 0; i < m_children.size(); i++) {
                        if (m_children.at(i)->keyReleased(key) ) return true;
					}
				}
			} else {
				for(unsigned int i = 0; i < m_children.size(); i++) {
                    if (m_children.at(i)->keyReleased(key)) return true;
				}
			}
            return false;
		}
		bool Panel::mouseMoved(int x, int y, int oldx, int oldy) {
			if (!m_visible) { return false; }

			bool s = GigaRectangle<int>::s_contains(getOnScreenX(), getOnScreenY(), m_width, m_height, x, y);
			if (s) {
				for(unsigned int i = 0; i < m_children.size(); i++) {
                    if (m_children.at(i)->mouseMoved(x, y, oldx, oldy)) return true;
				}
			}
            return false;
		}

		Panel::~Panel() {

		}
	}
}
