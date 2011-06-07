/*
 * UIButton.h
 *
 *  Created on: 26-May-2010
 *      Author: Ashley
 */

#ifndef UIBUTTON_H_
#define UIBUTTON_H_

#include <stdlib.h>
#include <string.h>


class GameContainer;
#include "Graphics.h"
#include "Event.h"

class UIButton {
	public:
		UIButton();
		void setEvent(int event, void (*eventFunctionPointer)());
		void setEvent(int event, Event* e);
		virtual void render(GameContainer& container, Graphics& g) = 0;
		virtual ~UIButton();

	private:

	protected:
		void* m_eventFunctionPointer;
		Event* m_eventObject;
};

#endif /* UIBUTTON_H_ */
