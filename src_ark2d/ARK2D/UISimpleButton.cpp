/*
 * UISimpleButton.cpp
 *
 *  Created on: 26-May-2010
 *      Author: Ashley
 */

#include "Color.h"
#include "UISimpleButton.h"
#include "GameContainer.h"
#include "Graphics.h"
#include "Input.h"
#include "Event.h"

UISimpleButton::UISimpleButton(std::string text, signed int x, signed int y, unsigned int width, unsigned int height):
	m_x(x),
	m_y(y),
	m_width(width),
	m_height(height),
	m_text(text),
	m_mousestate(OFF)
{

}

void UISimpleButton::setX(signed int x) {
	m_x = x;
}

void UISimpleButton::setY(signed int y) {
	m_y = y;
}

void UISimpleButton::setWidth(unsigned int width) {
	m_width = width;
}

void UISimpleButton::setHeight(unsigned int height) {
	m_height = height;
}

void UISimpleButton::setText(std::string text) {
	m_text = text;
}

void UISimpleButton::render(GameContainer& container, Graphics& g) {
	updateInputState(container);

	if (m_mousestate == OFF) {
		g.setDrawColor(Color::black_50a);
	} else {
		g.setDrawColor(Color::black);
	}
	g.fillRect(m_x, m_y, m_width, m_height);
	g.drawString(m_text, (m_x + (m_width/2)) - (g.getFont()->getStringWidth(m_text)/2),
						 (m_y + (m_height/2)) - (g.getFont()->getStringHeight(m_text)/2));
	g.setDrawColor(Color::black);
	g.drawRect(m_x, m_y, m_width, m_height);
}

void UISimpleButton::updateInputState(GameContainer& container) {
	Input i = container.getInput();
	if (i.getMouseX() > m_x && i.getMouseX() < int(m_x + m_width)
		&& i.getMouseY() > int(m_y) && i.getMouseY() < int(m_y + m_height)) {
			m_mousestate = OVER;
			//if (i.isMouseButtonDown(Input::MOUSE_BUTTON_LEFT)) {
				//m_eventFunctionPointer();
				m_eventObject->eventPerformed();
			//}
	} else {
		m_mousestate = OFF;
	}
}
