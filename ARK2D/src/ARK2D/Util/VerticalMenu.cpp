/*
 * VerticalMenu.cpp
 *
 *  Created on: 3 Aug 2010
 *      Author: Ashley
 */

#include "../Core/GameContainer.h"
#include "../Core/GameTimer.h"
#include "../Core/Event.h"
#include "../Graphics/Renderer.h"

#include "VerticalMenu.h"
#include "VerticalMenuItem.h"

VerticalMenu::VerticalMenu(GameContainer* container):
	m_container(container),
	m_items(),
	m_spacing(15),
	m_selected(0),
	m_wraparound(true) {

}

void VerticalMenu::addItem(VerticalMenuItem* item) {
	m_items.push_back(item);
}

void VerticalMenu::setSpacing(unsigned int spacing) {
	m_spacing = spacing;
}

bool VerticalMenu::isWraparound()  {
	return m_wraparound;
}
void VerticalMenu::setWraparound(bool b) {
	m_wraparound = b;
}

void VerticalMenu::performMoveUp() {
	if (m_wraparound == true) {
		m_selected--;
		if (m_selected < 0) {
			m_selected = m_items.size() - 1;
		}
		if (!m_items.at(m_selected)->isVisible()) { performMoveUp(); return; }
		itemFocusedEvent(m_selected);
	} else {
		if (m_selected != 0) {
			m_selected--;
			if (!m_items.at(m_selected)->isVisible()) { performMoveUp(); return; }
			itemFocusedEvent(m_selected);
		}
	}
}
void VerticalMenu::performMoveDown() {
	if (m_wraparound == true) {
		m_selected++;
		if ((unsigned int) m_selected > m_items.size() - 1) {
			m_selected = 0;
		}
		if (!m_items.at(m_selected)->isVisible()) { performMoveDown(); return; }
		itemFocusedEvent(m_selected);
	} else {
		if ((unsigned int) m_selected != (m_items.size() - 1)) {
			m_selected++;
			if (!m_items.at(m_selected)->isVisible()) { performMoveDown(); return; }
			itemFocusedEvent(m_selected);
		}
	}
}

void VerticalMenu::performSelect() {
	itemPressedEvent(m_selected);
}


void VerticalMenu::update(GameContainer* container, GameTimer* timer) {

}


void VerticalMenu::draw(Renderer* g, signed int x, signed int y) {
	signed int current_x = x;
	signed int current_y = y;

	unsigned int index = 0;
	std::vector<VerticalMenuItem*>::iterator it;
	for (it = m_items.begin(); it != m_items.end(); it++)
	{
		VerticalMenuItem* item = (*it);
		//if (!item->isVisible()) {
		//	index++;
		//	continue;
		//}
		if (index == (unsigned int) m_selected) {
			preRenderFocusedItem(index);
		} else {
			preRenderBlurredItem(index);
		}

		renderItem(g, item, current_x, current_y);

		if (item->isVisible()) {
			current_y += g->getFont()->getStringHeight(item->getText()) + m_spacing;
		}
		index++;
	}
}

void VerticalMenu::setSelectedItem(VerticalMenuItem* item) {
	unsigned int index = 0;
	std::vector<VerticalMenuItem*>::iterator it;
	for (it = m_items.begin(); it != m_items.end(); it++) {
		VerticalMenuItem* i = (*it);
		if (item == i) {
			m_selected = index;
			return;
		}
		index++;
	}
}

unsigned int VerticalMenu::size() {
	return m_items.size();
}
unsigned int VerticalMenu::sizeVisible() {
	unsigned int size = 0;
	std::vector<VerticalMenuItem*>::iterator it;
	for (it = m_items.begin(); it != m_items.end(); it++) {
		if ((*it)->isVisible()) {
			size++;
		}
	}
	return size;
}

void VerticalMenu::drawCentered(Renderer* g, signed int x, signed int y) {
	// calculate width and height...
	signed int width = 0;
	signed int height = 0;

	std::vector<signed int> y_coords;
	std::vector<VerticalMenuItem*>::iterator it;
	for (it = m_items.begin(); it != m_items.end(); it++) {
		VerticalMenuItem* item = (*it);
		y_coords.push_back(g->getFont()->getStringHeight(item->getText()));
		width += g->getFont()->getStringWidth(item->getText());
		height += g->getFont()->getStringHeight(item->getText());
		height += m_spacing;
	}

	signed int current_x = x;
	signed int current_y = y - (height/2);

	unsigned int index = 0;

	for (it = m_items.begin(); it != m_items.end(); it++) {
		VerticalMenuItem* item = (*it);
		if (index == (unsigned int) m_selected) {
			preRenderFocusedItem(index);
		} else {
			preRenderBlurredItem(index);
		}

		renderItem(g, item, current_x, current_y);
		if (item->isVisible()) {
			current_y += y_coords.at(index) + m_spacing;
		}
		index++;
	}
}

void VerticalMenu::itemFocusedEvent(unsigned int index) {

}
void VerticalMenu::itemBlurredEvent(unsigned int index) {

}
void VerticalMenu::itemPressedEvent(unsigned int index) {
	VerticalMenuItem* i = m_items.at(index);
	if (i != NULL) {
		Event* e = i->getOnPressEvent();
		if (e != NULL) {
			e->eventPerformed();
		}
	}
}
void VerticalMenu::preRenderFocusedItem(unsigned int index) {

}
void VerticalMenu::preRenderBlurredItem(unsigned int index) {

}
void VerticalMenu::renderItem(Renderer* g, VerticalMenuItem* item, signed int x, signed int y) {
	if (!item->isVisible()) { return; }
	g->drawString(item->getText(), x - (g->getFont()->getStringWidth(item->getText()) / 2), y);
}

VerticalMenu::~VerticalMenu() {

}
