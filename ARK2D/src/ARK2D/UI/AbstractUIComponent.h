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
#include "../Input.h"
#include "../GigaRectangle.h"

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
		bool m_visible;

		AbstractUIComponent();
		virtual int getX();
		virtual int getY();
		virtual unsigned int getWidth();
		virtual unsigned int getHeight();
		virtual void setX(int x);
		virtual void setY(int y);
		virtual void setLocation(int x, int y);
		virtual void setSize(unsigned int w, unsigned int h);

		void setWidth(unsigned int w);
		void setHeight(unsigned int h);

		void setPadding(int l, int t, int r, int b);
		void setPadding(int all);
		int getPaddingLeft();
		int getPaddingTop();
		int getPaddingRight();
		int getPaddingBottom();
		int getPaddingX();
		int getPaddingY();

		void setMargin(int l, int t, int r, int b);
		void setMargin(int all);
		int getMarginLeft();
		int getMarginTop();
		int getMarginRight();
		int getMarginBottom();
		int getMarginX();
		int getMarginY();

		void setClipping(bool b);

		AbstractUIComponent* getParent();
		void setParent(AbstractUIComponent* p);
		bool hasParent();

		int getOnScreenX();
		int getOnScreenY();

		void setVisible(bool b);
		bool isVisible();

		virtual void render() = 0;
		virtual void preRender();
		virtual void postRender();

		virtual void keyPressed(unsigned int key);
		virtual void keyReleased(unsigned int key);
		virtual void mouseMoved(int x, int y, int oldx, int oldy);

		bool isMouseInBounds();

		virtual ~AbstractUIComponent();
};

#endif /* ABSTRACTUICOMPONENT_H_ */
