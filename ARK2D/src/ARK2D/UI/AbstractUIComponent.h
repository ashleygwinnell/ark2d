/*
 * AbstractUIComponent.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef ABSTRACTUICOMPONENT_H_
#define ABSTRACTUICOMPONENT_H_

#include "UIComponent.h"
#include "../Graphics.h"
#include "../GameContainer.h"
#include "../Vector4.h"

class AbstractUIComponent : public UIComponent {
	public:
		AbstractUIComponent* m_parent;
		int m_x;
		int m_y;
		unsigned int m_width;
		unsigned int m_height;
		Vector4<int> m_padding; // left, top, right, bottom.
		Vector4<int> m_margin; // left, top, right, bottom.
		bool m_clipping;

		AbstractUIComponent(): UIComponent(), m_parent(NULL), m_x(0), m_y(0), m_width(1), m_height(1), m_padding(0,0,0,0), m_margin(0,0,0,0), m_clipping(true) { }
		virtual int getX() { return m_x; }
		virtual int getY() { return m_y; }
		virtual unsigned int getWidth() { return m_width; }
		virtual unsigned int getHeight() { return m_height; }
		virtual void setX(int x) { m_x = x; }
		virtual void setY(int y) { m_y = y; }
		virtual void setLocation(int x, int y) { m_x = x; m_y = y; }
		virtual void setSize(unsigned int w, unsigned int h) { m_width = w; m_height = h; }

		void setPadding(int l, int t, int r, int b) { m_padding.set(l, t, r, b); }
		void setPadding(int all) { m_padding.set(all, all, all, all); }
		int getPaddingLeft() {  return m_padding.getX(); }
		int getPaddingTop() { return m_padding.getY(); }
		int getPaddingRight() { return m_padding.getZ(); }
		int getPaddingBottom() { return m_padding.getW(); }
		int getPaddingX() { return m_padding.getX() + m_padding.getZ(); }
		int getPaddingY() { return m_padding.getY() + m_padding.getW(); }

		void setMargin(int l, int t, int r, int b) { m_margin.set(l, t, r, b); }
		void setMargin(int all) { m_margin.set(all, all, all, all); }
		int getMarginLeft() {  return m_margin.getX(); }
		int getMarginTop() { return m_margin.getY(); }
		int getMarginRight() { return m_margin.getZ(); }
		int getMarginBottom() { return m_margin.getW(); }
		int getMarginX() { return m_margin.getX() + m_margin.getZ(); }
		int getMarginY() { return m_margin.getY() + m_margin.getW(); }

		void setClipping(bool b) { m_clipping = b; }

		AbstractUIComponent* getParent() { return m_parent; }
		void setParent(AbstractUIComponent* p) { m_parent = p; }
		bool hasParent() { return (m_parent!=NULL); }
		int getOnScreenX() {
			if (!hasParent()) { return getX(); }

			int thisx = m_x;
			AbstractUIComponent* thisParent = m_parent;
			while(thisParent != NULL) {
				thisx += thisParent->getX();
				thisParent = thisParent->getParent();
			}
			return thisx;
		}
		int getOnScreenY() {
			if (!hasParent()) { return getY(); }

			int thisy = m_y;
			AbstractUIComponent* thisParent = m_parent;
			while(thisParent != NULL) {
				thisy += thisParent->getY();
				thisParent = thisParent->getParent();
			}
			return thisy;
		}

		virtual void render();
		virtual void preRender() {
			if (m_clipping) {
				Graphics* g = ARK2D::getGraphics();
				g->setScissorTestEnabled(true);
				g->scissor(getOnScreenX(), getOnScreenY(), m_width, m_height);
			}
		}
		virtual void postRender() {
			if (m_clipping) {
				GameContainer* c = ARK2D::getContainer();
				Graphics* g = ARK2D::getGraphics();
				g->scissor(0, 0, c->getWidth(), c->getHeight());
				g->setScissorTestEnabled(false);
			}
		}

		virtual void keyPressed(unsigned int key) { }
		virtual void keyReleased(unsigned int key) { }
		virtual void mouseMoved(int x, int y, int oldx, int oldy) { }

		virtual ~AbstractUIComponent() { }
};

#endif /* ABSTRACTUICOMPONENT_H_ */
