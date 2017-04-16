/*
 * AbstractUIComponent.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#include "AbstractUIComponent.h"
#include "../Core/Geometry/Cube.h"
#include "../Core/GameContainer.h"

namespace ARK {
	namespace UI {
		AbstractUIComponent::AbstractUIComponent():
			UIComponent(),
			SceneNode(),
			m_parent(NULL),
			m_bounds(),
			m_width(1),
			m_height(1),
			m_padding(0,0,0,0),
			m_margin(0,0,0,0),
			m_clipping(false),
			//m_visible(true),
			m_enabled(true),
			m_state(0)
			{
                m_bounds = new ARK::Core::Geometry::Cube();
		}
		float AbstractUIComponent::getX() {
			return transform.position.x;
		}
		float AbstractUIComponent::getY() {
			return transform.position.y;
		}
		float AbstractUIComponent::getMaxX() {
			return transform.position.x + ((1.0f - pivot.x) * m_width);
		}
		float AbstractUIComponent::getMaxY() {
			return transform.position.y + ((1.0f - pivot.y) * m_height);
		}
		float AbstractUIComponent::getMinX() {
			return transform.position.x - (pivot.x * m_width);
		}
		float AbstractUIComponent::getMinY() {
			return transform.position.y - (pivot.y * m_height);
		}
		unsigned int AbstractUIComponent::getWidth() {
			return m_width;
		}
		unsigned int AbstractUIComponent::getHeight() {
			return m_height;
		}
		void AbstractUIComponent::setX(float x) {
			transform.position.setX(x);
		}
		void AbstractUIComponent::setY(float y) {
			transform.position.setY(y);
		}
		void AbstractUIComponent::setLocation(float x, float y) {
			transform.position.set(x, y, 0);
		}
		void AbstractUIComponent::setLocationByCenter(float x, float y) {
			transform.position.set(x, y, 0);
			pivot.set(0.5, 0.5);
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

		void AbstractUIComponent::setVisible(bool b) {
			this->visible = b;
		}
		bool AbstractUIComponent::isVisible() {
			return this->visible;
		}

		void AbstractUIComponent::setEnabled(bool b) {
			m_enabled = b;
		}
		bool AbstractUIComponent::isEnabled() {
			return m_enabled;
		}

		void AbstractUIComponent::preRender() {
			// TODO: clipping does not work beacuse of screen space coordinates and what not.
			if (m_clipping) {
				Renderer* r = ARK2D::getRenderer();
				Vector3<float> worldpos = localPositionToGlobalPosition();
				r->setScissorTestEnabled(true);
				r->scissor(worldpos.x - m_width*pivot.getX()*transform.scale.getX(), worldpos.y - m_height * pivot.getY()*transform.scale.getY(), m_width, m_height);
			}
			SceneNode::preRender();
		}
		void AbstractUIComponent::postRender() {
			SceneNode::postRender();
			if (m_clipping) {
				GameContainer* c = ARK2D::getContainer();
				Renderer* g = ARK2D::getRenderer();
				g->scissor(0, 0, c->getWidth(), c->getHeight());
				g->setScissorTestEnabled(false);
			}
		}

        bool AbstractUIComponent::keyPressed(unsigned int key) { return false; }
        bool AbstractUIComponent::keyReleased(unsigned int key) { return false; }
        bool AbstractUIComponent::mouseMoved(int x, int y, int oldx, int oldy) {

        	if (!m_enabled) { return false; }

			if (m_state != STATE_DOWN) {
				if (isGlobalPositionInBounds(x, y)) {
					m_state = STATE_OVER;
				} else {
					m_state = STATE_OFF;
				}
			}
			return (m_state == STATE_OVER || m_state == STATE_DOWN);

        }

        void AbstractUIComponent::setState(unsigned int i) {
			m_state = i;
		}
		unsigned int AbstractUIComponent::getState() {
			return m_state;
		}

		void AbstractUIComponent::setBounds(float w, float h, float d) {
			m_width = w;
			m_height = h;
		}

        ARK::Core::Geometry::Cube* AbstractUIComponent::getAABBBounds() {
			m_bounds->setWidth(m_width);
			m_bounds->setHeight(m_height);
			return m_bounds;
		}

		/*bool AbstractUIComponent::isMouseInBounds() {
			Input* i = ARK2D::getInput();
			return GigaRectangle<int>::s_contains(getOnScreenX(), getOnScreenY(), m_width, m_height, i->getMouseX(), i->getMouseY());
		}*/

		AbstractUIComponent::~AbstractUIComponent() {

		}
	}
}
