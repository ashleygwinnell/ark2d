/*
 * UIComponent.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#include "UIComponent.h"
//#include "../Includes.h"
#include <stddef.h>

namespace ARK { 
	namespace UI {
		UIComponent* UIComponent::s_currentFocus = NULL;


		UIComponent::UIComponent():
			m_focussed(false) 
		{

		}
		void UIComponent::setFocussed(bool b) { 
			setFocussed(b, false);
		}
		void UIComponent::setFocussed(bool b, bool secondPass) { // not sure what this secondpass thing does any more! 
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
		bool UIComponent::isFocussed() { return m_focussed; }
		UIComponent::~UIComponent() {

		}
		

	}
}
