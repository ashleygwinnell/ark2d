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
				
				virtual void render();

				static void upButtonPressedStatic(void* p);
				void upButtonPressed();

				static void downButtonPressedStatic(void* p);
				void downButtonPressed();


				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);

				virtual void onChildAdded(SceneNode* newChild);
				void calculateSize();

		};
	}
}

#endif /* SCROLLPANEL_H_ */
