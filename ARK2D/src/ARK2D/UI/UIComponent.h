/*
 * UIComponent.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef UICOMPONENT_H_
#define UICOMPONENT_H_

//#include "../Includes.h"
//#include "../Namespaces.h"
#include "../Common/DLL.h"

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
				UIComponent();
				void setFocussed(bool b);
				void setFocussed(bool b, bool secondPass);
				bool isFocussed();
				virtual float getX() = 0;
				virtual float getY() = 0;
				virtual void setLocation(float x, float y) = 0;
				virtual void setSize(unsigned int w, unsigned int h) = 0;
				virtual ~UIComponent();
		};
	}
}


#endif /* UICOMPONENT_H_ */
