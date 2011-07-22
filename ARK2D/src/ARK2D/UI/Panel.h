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
		bool m_showBorder;
		Panel();
		void add(AbstractUIComponent* c);
		void setTranslating(bool b);

		virtual void preRender();
		virtual void postRender();
		virtual void render();
		virtual void renderChildren();
		virtual void renderBorder();

		virtual void keyPressed(unsigned int key);
		virtual void keyReleased(unsigned int key);
		virtual void mouseMoved(int x, int y, int oldx, int oldy);
};

#endif /* PANEL_H_ */
