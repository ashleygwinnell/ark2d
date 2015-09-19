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
			//m_children(), 
			//m_translate(true), 
			m_showBorder(true),
			m_bounds(NULL) {
				m_bounds = new Cube();
		}

		//void Panel::add(AbstractUIComponent* c) {
		//	c->setParent(this);
		//	m_children.push_back(c);
		//}

		//void Panel::setTranslating(bool b) {
		//	m_translate = b;
		//}
		void Panel::setShowBorder(bool b) {
			m_showBorder = b;
		}

		void Panel::render() {
			if (m_visible) {
				//preRender();
				renderChildren();
				renderBorder();
				//postRender();



				string pos = "(0,0)";
				Renderer* r = ARK2D::getRenderer();
				r->drawString(pos, 0, 0);

				Vector3<float> worldpos = localPositionToGlobalPositionInternal();
				string worldpost = "(";
					worldpost += Cast::toString<float>(worldpos.getX());
					worldpost += ",";
					worldpost += Cast::toString<float>(worldpos.getY());
				worldpost += ")";
				r->drawString(worldpost, 0, 20);


				Vector3<float> worldpos2 = localPositionToGlobalPosition();
				string worldpost2 = "(";
					worldpost2 += Cast::toString<float>(worldpos2.getX());
					worldpost2 += ",";
					worldpost2 += Cast::toString<float>(worldpos2.getY());
				worldpost2 += ")";
				r->drawString(worldpost2, 0, 40);
			}
		}
		
		void Panel::renderBorder() {
			if (m_showBorder == true) {
				Renderer* g = ARK2D::getRenderer();
				g->setDrawColor(Color::white);
				g->drawRect(0, 0, m_width, m_height);
			}
		}


		ARK::Geometry::Cube* Panel::getBounds() {
			m_bounds->set(0,0,0,m_width, m_height, 0);
			return m_bounds;
		}

		bool Panel::keyPressed(unsigned int key) {
			if (!m_visible) { return false; }

			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT || key == (unsigned int) Input::MOUSE_BUTTON_RIGHT) 
			{
				Input* i = ARK2D::getInput();
				Vector3<float> worldpos = localPositionToGlobalPosition();
				bool s = GigaRectangle<int>::s_contains(worldpos.getX(), worldpos.getY(), m_width, m_height, i->getMouseX(), i->getMouseY());
				if (s) {
					for(unsigned int i = 0; i < children.size(); i++) {
                        if (children.at(i)->keyPressed(key)) return true;
					}
				}
			} else {
				for(unsigned int i = 0; i < children.size(); i++) {
                    if (children.at(i)->keyPressed(key)) return true;
				}
			}
            return false;
		}
		bool Panel::keyReleased(unsigned int key) {
			if (!m_visible) { return false; }

			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT || key == (unsigned int) Input::MOUSE_BUTTON_RIGHT) 
			{
				Input* i = ARK2D::getInput();
				Vector3<float> worldpos = localPositionToGlobalPosition();
				bool s = GigaRectangle<int>::s_contains(worldpos.getX(), worldpos.getY(), m_width, m_height, i->getMouseX(), i->getMouseY());
				if (s) {
					for(unsigned int i = 0; i < children.size(); i++) {
                        if (children.at(i)->keyReleased(key) ) return true;
					}
				}
			} else {
				for(unsigned int i = 0; i < children.size(); i++) {
                    if (children.at(i)->keyReleased(key)) return true;
				}
			}
            return false;
		}
		bool Panel::mouseMoved(int x, int y, int oldx, int oldy) {
			if (!m_visible) { return false; }

			Vector3<float> worldpos = localPositionToGlobalPosition();
			bool s = GigaRectangle<int>::s_contains(worldpos.getX(), worldpos.getY(), m_width, m_height, x, y);
			if (s) {
				for(unsigned int i = 0; i < children.size(); i++) {
                    if (children.at(i)->mouseMoved(x, y, oldx, oldy)) return true;
				}
			}
            return false;
		}

		Panel::~Panel() {

		}
	}
}
