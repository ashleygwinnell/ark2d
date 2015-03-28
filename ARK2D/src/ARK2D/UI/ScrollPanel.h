/*
 * ScrollPanel.h
 *
 *  Created on: 21 Jul 2011
 *      Author: Ashley
 */

#ifndef SCROLLPANEL_H_
#define SCROLLPANEL_H_

#include "Button.h"
#include "AbstractUIComponent.h"
#include "Panel.h"

namespace ARK {
	namespace UI {

		/*!
		 * \brief A Panel with a scroll bar.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API ScrollPanel : public Panel {
			public:
				Button m_upButton;
				Button m_downButton;
				Button m_scrollYButton;
				int m_offsetX;
				int m_offsetY;
				int m_calculatedWidth;
				int m_calculatedHeight;

				unsigned int m_layout;
				static const unsigned int LAYOUT_FLOW = 0;

				ScrollPanel();
				void add(AbstractUIComponent* c);
				void calculateSize();

				void renderScrollbars();
				void render();

				static void upButtonPressedStatic(void* p);
				void upButtonPressed();

				static void downButtonPressedStatic(void* p);
				void downButtonPressed();


				void keyPressed(unsigned int key);
				void keyReleased(unsigned int key);
				void mouseMoved(int x, int y, int oldx, int oldy);

		};
	}
}

#endif /* SCROLLPANEL_H_ */
