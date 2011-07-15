/*
 * ComboBox.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#include "ComboBox.h"
#include "ComboBoxItem.h"

ComboBox::ComboBox(): AbstractUIComponent(), m_items(), m_selected(NULL), m_open(false), m_itemChangedEvent(NULL) {

}
void ComboBox::addItem(ComboBoxItem* cbi) {
	if (m_items.size() == 0) {
		m_selected = cbi;
	}
	cbi->parent = this;
	m_items.push_back(cbi);
}
void ComboBox::setOpen(bool b) {
	m_open = b;
}
bool ComboBox::isOpen() {
	return m_open;
}
ComboBoxItem* ComboBox::getSelected() {
	return m_selected;
}
void ComboBox::setSelectedByValue(string value) {
	for(unsigned int i = 0; i < m_items.size(); i++) {
		if (value == m_items.at(i)->m_value) {
			m_selected = m_items.at(i);
			break;
		}
	}
}

void ComboBox::setItemChangedEvent(void* event) {
	m_itemChangedEvent = event;
}

void ComboBox::keyPressed(unsigned int key) {
	if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {
		Input* i = ARK2D::getInput();

		if (m_open) {
			for(unsigned int i = 0; i < m_items.size(); i++) {
				m_items.at(i)->keyPressed(key);
			}
		}

		if (GigaRectangle<int>::s_contains(m_x, m_y, (signed int) (m_width), (signed int)(m_height), (signed int) (i->getMouseX()), (signed int) (i->getMouseY()))) {
			setOpen(!isOpen());
		} else {
			setOpen(false);
		}



	}

}
void ComboBox::keyReleased(unsigned int key) {

}
void ComboBox::render() {
	renderBackground();
	renderArrow();
	renderOverlay();

	Graphics* g = ARK2D::getGraphics();
	if (m_selected != NULL) {
		g->drawString(m_selected->m_text, m_x, m_y);
	}

	if (m_open) {
		int y = m_y + m_height;
		for(unsigned int i = 0; i < m_items.size(); i++) {
			m_items.at(i)->setLocation(m_x, y);
			m_items.at(i)->render();
			y += m_items.at(i)->getHeight();
		}
	}
}
void ComboBox::renderBackground() {
	Graphics* g = ARK2D::getGraphics();
	g->setDrawColor(Color::black_50a);
	g->fillRect(m_x, m_y, m_width, m_height);
}
void ComboBox::renderArrow() {
	Graphics* g = ARK2D::getGraphics();
	g->setDrawColor(Color::white_50a);
	if (m_open) {
		g->fillTriangle(m_x + m_width - 20, m_y+10, 10, 10);
	} else {
		g->fillTriangle(m_x + m_width - 20, m_y+20, 10, -10);
	}

}
void ComboBox::renderText(int x, int y) {

}
void ComboBox::renderOverlay() {
	Graphics* g = ARK2D::getGraphics();
	g->setDrawColor(Color::white);
	//if (m_state == STATE_OVER || m_state == STATE_DOWN) {
	//	g->setDrawColor(Color::white_50a);
	//}
	g->drawRect(m_x, m_y, m_width, m_height);
}
