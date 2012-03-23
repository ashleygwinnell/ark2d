/*
 * CheckBox.h
 *
 *  Created on: 24 Jul 2011
 *      Author: Ashley
 */

#ifndef CHECKBOX_H_
#define CHECKBOX_H_


#include "AbstractUIComponent.h"
#include "Button.h"

namespace ARK {
	namespace UI {
		/*!
		 * \brief A checkable box.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class CheckBox : public AbstractUIComponent {
			private:
				bool m_checked;
				unsigned int m_state;
				static const unsigned int STATE_OFF = Button::STATE_OFF;
				static const unsigned int STATE_OVER = Button::STATE_OVER;
				static const unsigned int STATE_DOWN = Button::STATE_DOWN;

			public:
				CheckBox();

				bool isChecked();
				void setChecked(bool b);

				virtual void render();
				virtual void renderTick();
				virtual void renderBackground();
				virtual void renderOverlay();

				virtual void keyPressed(unsigned int key);
				virtual void keyReleased(unsigned int key);
				virtual void mouseMoved(int x, int y, int oldx, int oldy);

				virtual ~CheckBox();
		};
	}
}

#endif /* CHECKBOX_H_ */
