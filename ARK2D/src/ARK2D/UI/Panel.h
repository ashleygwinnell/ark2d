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

class Panel : public AbstractUIComponent {
	public:
		vector<AbstractUIComponent*> m_children;
		bool m_clipping;
		bool m_translate;
		Panel(): AbstractUIComponent(), m_children(), m_clipping(true), m_translate(true) {

		}
		void add(AbstractUIComponent* c) {
			m_children.push_back(c);
		}
		void setClipping(bool b) {
			m_clipping = b;
		}
		void setTranslating(bool b) {
			m_translate = b;
		}
		unsigned int getWidth() {
			return m_width;
		}
		unsigned int getHeight() {
			return m_height;
		}
		void preRender() {
			//GameContainer* c = ARK2D::getContainer();
			Graphics* g = ARK2D::getGraphics();

			if (m_clipping) {
				g->setScissorTestEnabled(true);
				g->scissor(m_x, m_y, m_width, m_height);
			}

			if (m_translate) {
				g->pushMatrix();
				g->translate(m_x, m_y);
			}
		}
		void postRender() {
			GameContainer* c = ARK2D::getContainer();
			Graphics* g = ARK2D::getGraphics();

			if (m_clipping) {
				g->scissor(0, 0, c->getWidth(), c->getHeight());
				g->setScissorTestEnabled(false);
			}
			if (m_translate) {
				g->popMatrix();
			}
			renderBorder();
		}
		void render() {

			preRender();
			for(unsigned int i = 0; i < m_children.size(); i++) {
				m_children.at(i)->render();
			}
			postRender();
		}
		void renderBorder() {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::white);
			g->drawRect(m_x, m_y, m_width, m_height);
		}
};

#endif /* PANEL_H_ */
