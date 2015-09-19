/*
 * UIComponent.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef UICOMPONENT_H_
#define UICOMPONENT_H_

#include "../Includes.h"
#include "../Namespaces.h"

namespace ARK {
	namespace UI {

		/*!
		 * \brief The interface for a UI component.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API UIComponent {
			public:
				static UIComponent* s_currentFocus;
			private:
				bool m_focussed;

			public:
				UIComponent():
					m_focussed(false) 
				{

				}
				void setFocussed(bool b) { 
					setFocussed(b, false);
				}
				void setFocussed(bool b, bool secondPass) { // not sure what this secondpass thing does any more! 
					/*if (s_currentFocus != NULL && secondPass == false) { 
						s_currentFocus->setFocussed(false, true); 
					}
					m_focussed = b; 

					if (b) { 
						s_currentFocus = this; 
					}*/
					if (s_currentFocus != NULL && b) {
						s_currentFocus->setFocussed(false); 
					}

					if (b) {
						s_currentFocus = this;
					}
					m_focussed = b; 
					

				}
				bool isFocussed() { return m_focussed; }
				virtual float getX() = 0;
				virtual float getY() = 0;
				virtual void setLocation(float x, float y) = 0;
				virtual void setSize(unsigned int w, unsigned int h) = 0;
				virtual ~UIComponent() {

				}
		};
	}
}


#endif /* UICOMPONENT_H_ */
