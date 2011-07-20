/*
 * Panel.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef PANEL_H_
#define PANEL_H_

#include <vector>
using namespace std;

#include "../ARK2D.h"
#include "AbstractUIComponent.h"

#include "../Graphics.h"
#include "../GameContainer.h"
#include "../Input.h"
#include "../GigaRectangle.h"

class Panel : public AbstractUIComponent {
	public:
		vector<AbstractUIComponent*> m_children;
		bool m_translate;
		Panel(): AbstractUIComponent(), m_children(), m_translate(true) {

		}
		void add(AbstractUIComponent* c) {
			c->setParent(this);
			m_children.push_back(c);
		}

		void setTranslating(bool b) {
			m_translate = b;
		}

		void preRender() {
			AbstractUIComponent::preRender();

			if (m_translate) {
				Graphics* g = ARK2D::getGraphics();
				g->pushMatrix();
				g->translate(m_x, m_y);
			}
		}
		void postRender() {
			AbstractUIComponent::postRender();

			if (m_translate) {
				Graphics* g = ARK2D::getGraphics();
				g->popMatrix();
			}
			renderBorder();
		}
		void render() {
			preRender();
			renderChildren();
			postRender();
		}
		void renderChildren() {
			for(unsigned int i = 0; i < m_children.size(); i++) {
				m_children.at(i)->render();
			}
		}
		void renderBorder() {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::white);
			g->drawRect(m_x, m_y, m_width, m_height);
		}


		virtual void keyPressed(unsigned int key) {
			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT
					|| key == (unsigned int) Input::MOUSE_BUTTON_RIGHT) {
				Input* i = ARK2D::getInput();
				bool s = GigaRectangle<int>::s_contains(getOnScreenX(), getOnScreenY(), m_width, m_height, i->getMouseX(), i->getMouseY());
				if (s) {
					for(unsigned int i = 0; i < m_children.size(); i++) {
						m_children.at(i)->keyPressed(key);
					}
				}
			} else {
				for(unsigned int i = 0; i < m_children.size(); i++) {
					m_children.at(i)->keyPressed(key);
				}
			}
		}
		virtual void keyReleased(unsigned int key) {
			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT
					|| key == (unsigned int) Input::MOUSE_BUTTON_RIGHT) {
				Input* i = ARK2D::getInput();
				bool s = GigaRectangle<int>::s_contains(getOnScreenX(), getOnScreenY(), m_width, m_height, i->getMouseX(), i->getMouseY());
				if (s) {
					for(unsigned int i = 0; i < m_children.size(); i++) {
						m_children.at(i)->keyReleased(key);
					}
				}
			} else {
				for(unsigned int i = 0; i < m_children.size(); i++) {
					m_children.at(i)->keyReleased(key);
				}
			}
		}
		virtual void mouseMoved(int x, int y, int oldx, int oldy) {
			bool s = GigaRectangle<int>::s_contains(getOnScreenX(), getOnScreenY(), m_width, m_height, x, y);
			if (s) {
				for(unsigned int i = 0; i < m_children.size(); i++) {
					m_children.at(i)->mouseMoved(x, y, oldx, oldy);
				}
			}
		}
};

#endif /* PANEL_H_ */
