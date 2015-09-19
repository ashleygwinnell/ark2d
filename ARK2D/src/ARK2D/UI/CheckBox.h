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
		class ARK2D_API CheckBox : public AbstractUIComponent {
			protected:
				bool m_checked;
				//unsigned int m_state;
				//static const unsigned int STATE_OFF = Button::STATE_OFF;
				//static const unsigned int STATE_OVER = Button::STATE_OVER;
				//static const unsigned int STATE_DOWN = Button::STATE_DOWN;
				void* m_stateChangedEvent;

			public:
				CheckBox();

				bool isChecked();
				void setChecked(bool b);
				void setStateChangedEvent(void* function);
				void doStateChangedEvent();

				virtual void render();
				virtual void renderTick();
				virtual void renderBackground();
				virtual void renderOverlay();

				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);

				virtual ~CheckBox();
		};
	}
}

#endif /* CHECKBOX_H_ */
