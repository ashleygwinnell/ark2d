/*
 * AbstractUIComponent.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef ABSTRACTUICOMPONENT_H_
#define ABSTRACTUICOMPONENT_H_

#include "UIComponent.h"

class AbstractUIComponent : public UIComponent {
	public:
		int m_x;
		int m_y;
		unsigned int m_width;
		unsigned int m_height;

		AbstractUIComponent(): UIComponent(), m_x(0), m_y(0), m_width(1), m_height(1) { }
		virtual int getX() { return m_x; }
		virtual int getY() { return m_y; }
		virtual unsigned int getWidth() { return m_width; }
		virtual unsigned int getHeight() { return m_height; }
		virtual void setX(int x) { m_x = x; }
		virtual void setY(int y) { m_y = y; }
		virtual void setLocation(int x, int y) { m_x = x; m_y = y; }
		virtual void setSize(unsigned int w, unsigned int h) { m_width = w; m_height = h; }
		virtual void render();
		virtual ~AbstractUIComponent() {

		}
};

#endif /* ABSTRACTUICOMPONENT_H_ */
