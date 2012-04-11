/*
 * UIComponent.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef UICOMPONENT_H_
#define UICOMPONENT_H_

#include "../Namespaces.h"

namespace ARK {
	namespace UI {

		/*!
		 * \brief The interface for a UI component.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class UIComponent {
			public:
				static UIComponent* s_currentFocus;
			private:
				bool m_focussed;

			public:
				UIComponent(): m_focussed(false) {}
				void setFocussed(bool b) { m_focussed = b; if (b) { s_currentFocus = this; } }
				bool isFocussed() { return m_focussed; }
				virtual int getX() = 0;
				virtual int getY() = 0;
				virtual void setLocation(int x, int y) = 0;
				virtual void setSize(unsigned int w, unsigned int h) = 0;
				virtual ~UIComponent() {

				}
		};
	}
}


#endif /* UICOMPONENT_H_ */
