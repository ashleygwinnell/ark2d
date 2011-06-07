
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "UIButton.h"
#include "Event.h"

UIButton::UIButton() {

}


void UIButton::setEvent(int event, void (*eventFunctionPointer)()) {
	//m_eventFunctionPointer = eventFunctionPointer;
}

void UIButton::setEvent(int event, Event* e) {
	m_eventObject = e;
}

UIButton::~UIButton() {

}
