/*
 * Button.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef BUTTON_H_
#define BUTTON_H_

//#include "../Includes.h"
#include "../Namespaces.h"
#include "AbstractUIComponent.h"
#include "../Core/String.h"

namespace ARK {
	namespace UI {
		/*!
		 * \brief A clickable button.
		 *
		 * @todo Improve rendering on mobile devices not using primitive geometry.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Button : public AbstractUIComponent {
			protected:
				ARK::Core::String m_text;
				Image* m_image;
				void* m_event;
				void* m_eventObj;

			public:
				Button();
				Button(string text);

				void setEvent(void* e);
				void setEventObj(void* o);
				void* getEvent();
				void* getEventObj();
				void setImage(Image* i);

				virtual bool isPointerOver();

				virtual void doEvent();

				void setText(string s);
				const String& getText();

				virtual void render();
				virtual void renderBackground();
				virtual void renderText(int x, int y);
				virtual void renderImage(int x, int y);
				virtual void renderOverlay();

				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);
		};
	}
}

#endif /* BUTTON_H_ */
