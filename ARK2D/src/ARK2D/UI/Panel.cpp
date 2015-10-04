/*
 * Panel.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#include "Panel.h"
#include "../Geometry/Cube.h"
#include "../Graphics/Renderer.h"

namespace ARK {
	namespace UI {
		bool Panel::s_debugCoordinates = false;

		Panel::Panel():
			AbstractUIComponent(), 
			//m_children(), 
			//m_translate(true), 
			m_showBorder(true),
			m_bounds(0,0,0) {
				
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

				preRenderFromPivot();

				if (s_debugCoordinates) { 
					string pos = "(0,0)";
					Renderer* r = ARK2D::getRenderer();
					r->setFont(r->getDefaultFont());
					r->drawString(pos, 0, 0, -1, -1, 0.0f, 0.5f);

					Vector3<float> worldpos = localPositionToGlobalPositionInternal();
					string worldpost = "(";
						worldpost += Cast::toString<float>(worldpos.getX());
						worldpost += ",";
						worldpost += Cast::toString<float>(worldpos.getY());
					worldpost += ")";
					r->drawString(worldpost, 0, 10, -1, -1, 0.0, 0.5f);


					Vector3<float> worldpos2 = localPositionToGlobalPosition();
					string worldpost2 = "(";
						worldpost2 += Cast::toString<float>(worldpos2.getX());
						worldpost2 += ",";
						worldpost2 += Cast::toString<float>(worldpos2.getY());
					worldpost2 += ")";
					r->drawString(worldpost2, 0, 20, -1, -1, 0.0, 0.5f);

					// global to local pos.
					float mx = ARK2D::getInput()->getMouseX();
					float my = ARK2D::getInput()->getMouseY();
					string worldpost3 = "(";
						worldpost3 += Cast::toString<float>(mx);
						worldpost3 += ",";
						worldpost3 += Cast::toString<float>(my);
					worldpost2 += ")";
					r->drawString(worldpost3, 0, 35, -1, -1, 0.0, 0.5f);

					Vector3<float> worldpos4 = globalPositionToLocalPosition(mx, my, 0, false);
					string worldpost4 = "(";
						worldpost4 += Cast::toString<float>(worldpos4.getX());
						worldpost4 += ",";
						worldpost4 += Cast::toString<float>(worldpos4.getY());
					worldpost4 += ")";
					r->drawString(worldpost4, 0, 45, -1, -1, 0.0, 0.5f);

					Vector3<float> worldpos5 = globalPositionToLocalPosition(mx, my, 0, true);
					string worldpost5 = "(";
						worldpost5 += Cast::toString<float>(worldpos5.getX());
						worldpost5 += ",";
						worldpost5 += Cast::toString<float>(worldpos5.getY());
					worldpost5 += ")";
					r->drawString(worldpost5, 0, 55, -1, -1, 0.0, 0.5f);

					ARK::Geometry::Cube* bounds = getBounds();
					r->setDrawColor(Color::white);
					r->fillCircle(bounds->getWidth() * pivot.getX(), bounds->getHeight() * pivot.getY(),2,10);
				}
				
				renderBorder();
				postRenderFromPivot();
				
				//preRender();
				
				
				//postRender();

				renderChildren();

				
			}
		}
		
		void Panel::renderBorder() {
			if (m_showBorder == true) {
				Renderer* g = ARK2D::getRenderer();
				g->setDrawColor(Color::white);
				if (m_state == STATE_OVER) {
					g->setDrawColor(Color::white_50a);
				}
				g->drawRect(0, 0, m_width, m_height);
			}
		}

		void Panel::setBounds(float w, float h, float d) {
			AbstractUIComponent::setBounds(w,h,d);
			m_bounds.set(0,0,0,w,h,d);
		}

		ARK::Geometry::Cube* Panel::getBounds() {
			m_bounds.set(0, 0, 0, m_width, m_height, 0);
			return &m_bounds;
		}

		bool Panel::keyPressed(unsigned int key) {
			if (!m_visible) { return false; }

			for(unsigned int i = 0; i < children.size(); i++) {
				if (children.at(i)->keyPressed(key)) return true;
			}
            return false;
		}
		bool Panel::keyReleased(unsigned int key) {
			if (!m_visible) { return false; }

			for(unsigned int i = 0; i < children.size(); i++) {
                if (children.at(i)->keyReleased(key) ) return true;
			}
	        return false;
		}
		bool Panel::mouseMoved(int x, int y, int oldx, int oldy) {
			if (!m_visible) { return false; }

			m_state = (isGlobalPositionInBounds(x, y)) ? STATE_OVER : STATE_OFF;

			bool consumed = false;
			for(unsigned int i = 0; i < children.size(); i++) {
                if (children.at(i)->mouseMoved(x, y, oldx, oldy)) { consumed = true; }
			}
			if (consumed) { m_state = STATE_OFF; return consumed; }

			
				
			return m_state == STATE_OVER;
		}

		Panel::~Panel() {

		}
	}
}
