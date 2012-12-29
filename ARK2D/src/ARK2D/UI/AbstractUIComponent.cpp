/*
 * AbstractUIComponent.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#include "AbstractUIComponent.h"

namespace ARK {
	namespace UI {
		AbstractUIComponent::AbstractUIComponent():
			UIComponent(),
			m_parent(NULL),
			m_x(0), m_y(0),
			m_width(1), m_height(1),
			m_padding(0,0,0,0),
			m_margin(0,0,0,0), 
			m_clipping(true),
			m_visible(true),
			m_enabled(true)
			{
 
		}
		float AbstractUIComponent::getX() {
			return m_x;
		}
		float AbstractUIComponent::getY() {
			return m_y;
		}
		float AbstractUIComponent::getCenterX() {
			return m_x + (m_width/2);
		}
		float AbstractUIComponent::getCenterY() {
			return m_y + (m_height/2);
		}
		float AbstractUIComponent::getMaxX() {
			return m_x + m_width;
		}
		float AbstractUIComponent::getMaxY() {
			return m_y + m_height;
		}
		float AbstractUIComponent::getMinX() {
			return m_x; 
		}
		float AbstractUIComponent::getMinY() {
			return m_y;
		}
		unsigned int AbstractUIComponent::getWidth() {
			return m_width;
		} 
		unsigned int AbstractUIComponent::getHeight() {
			return m_height;
		}
		void AbstractUIComponent::setX(float x) {
			m_x = x;
		}
		void AbstractUIComponent::setY(float y) {
			m_y = y;
		}
		void AbstractUIComponent::setLocation(float x, float y) {
			m_x = x;
			m_y = y;
		} 
		void AbstractUIComponent::setLocationByCenter(float x, float y) {
			m_x = x - (m_width/2);
			m_y = y - (m_height/2);
		}
		void AbstractUIComponent::setSize(unsigned int w, unsigned int h) {
			m_width = w;
			m_height = h;
		}

		void AbstractUIComponent::setWidth(unsigned int w) {
			m_width = w;
		}
		void AbstractUIComponent::setHeight(unsigned int h) {
			m_height = h;
		}

		void AbstractUIComponent::setPadding(int l, int t, int r, int b) { m_padding.set(l, t, r, b); }
		void AbstractUIComponent::setPadding(int all) { m_padding.set(all, all, all, all); }
		int AbstractUIComponent::getPaddingLeft() {  return m_padding.getX(); }
		int AbstractUIComponent::getPaddingTop() { return m_padding.getY(); }
		int AbstractUIComponent::getPaddingRight() { return m_padding.getZ(); }
		int AbstractUIComponent::getPaddingBottom() { return m_padding.getW(); }
		int AbstractUIComponent::getPaddingX() { return m_padding.getX() + m_padding.getZ(); }
		int AbstractUIComponent::getPaddingY() { return m_padding.getY() + m_padding.getW(); }

		void AbstractUIComponent::setMargin(int l, int t, int r, int b) { m_margin.set(l, t, r, b); }
		void AbstractUIComponent::setMargin(int all) { m_margin.set(all, all, all, all); }
		int AbstractUIComponent::getMarginLeft() {  return m_margin.getX(); }
		int AbstractUIComponent::getMarginTop() { return m_margin.getY(); }
		int AbstractUIComponent::getMarginRight() { return m_margin.getZ(); }
		int AbstractUIComponent::getMarginBottom() { return m_margin.getW(); }
		int AbstractUIComponent::getMarginX() { return m_margin.getX() + m_margin.getZ(); }
		int AbstractUIComponent::getMarginY() { return m_margin.getY() + m_margin.getW(); }

		void AbstractUIComponent::setClipping(bool b) { m_clipping = b; }

		AbstractUIComponent* AbstractUIComponent::getParent() { return m_parent; }
		void AbstractUIComponent::setParent(AbstractUIComponent* p) { m_parent = p; }
		bool AbstractUIComponent::hasParent() { return (m_parent!=NULL); }
		int AbstractUIComponent::getOnScreenX() {
			if (!hasParent()) { return getX(); }

			int thisx = m_x;
			AbstractUIComponent* thisParent = m_parent;
			while(thisParent != NULL) {
				thisx += thisParent->getX();
				thisParent = thisParent->getParent();
			}
			return thisx;
		}

		void AbstractUIComponent::setVisible(bool b) {
			m_visible = b;
		}
		bool AbstractUIComponent::isVisible() {
			return m_visible;
		}

		void AbstractUIComponent::setEnabled(bool b) {
			m_enabled = b;
		}
		bool AbstractUIComponent::isEnabled() {
			return m_enabled;
		}

		int AbstractUIComponent::getOnScreenY() {
			if (!hasParent()) { return getY(); }

			int thisy = m_y;
			AbstractUIComponent* thisParent = m_parent;
			while(thisParent != NULL) {
				thisy += thisParent->getY();
				thisParent = thisParent->getParent();
			}
			return thisy;
		}

		void AbstractUIComponent::preRender() {
			if (m_clipping) {
				Renderer* g = ARK2D::getRenderer();
				g->setScissorTestEnabled(true);
				g->scissor(getOnScreenX(), getOnScreenY(), m_width, m_height);
			}
		}
		void AbstractUIComponent::postRender() {
			if (m_clipping) {
				GameContainer* c = ARK2D::getContainer();
				Renderer* g = ARK2D::getRenderer();
				g->scissor(0, 0, c->getWidth(), c->getHeight());
				g->setScissorTestEnabled(false);
			}
		}

		void AbstractUIComponent::keyPressed(unsigned int key) { }
		void AbstractUIComponent::keyReleased(unsigned int key) { }
		void AbstractUIComponent::mouseMoved(int x, int y, int oldx, int oldy) { }

		bool AbstractUIComponent::isMouseInBounds() {
			Input* i = ARK2D::getInput();
			return GigaRectangle<int>::s_contains(getOnScreenX(), getOnScreenY(), m_width, m_height, i->getMouseX(), i->getMouseY());
		}

		AbstractUIComponent::~AbstractUIComponent() {

		}
	}
}
