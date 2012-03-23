/*
 * VerticalMenu.h
 *
 *  Created on: 3 Aug 2010
 *      Author: Ashley
 */

#ifndef VERTICALMENU_H_
#define VERTICALMENU_H_

#include <string>
#include <vector>
#include <stdlib.h>

class GameContainer;
class GameTimer;
class Graphics;

class VerticalMenuItem;

/*!
 * @deprecated
 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
 */
class VerticalMenu {
	public:
		VerticalMenu(GameContainer* container);

		void addItem(VerticalMenuItem* item);

		void setSpacing(unsigned int spacing);
		void update(GameContainer* container, GameTimer* timer);
		void draw(Graphics* g, signed int x, signed int y);
		void drawCentered(Graphics* g, signed int x, signed int y);
		bool isWraparound();
		void setWraparound(bool b);

		virtual void performMoveUp();
		virtual void performMoveDown();
		virtual void performSelect();

		virtual void setSelectedItem(VerticalMenuItem* item);

		virtual void itemFocusedEvent(unsigned int index);
		virtual void itemBlurredEvent(unsigned int index);
		virtual void itemPressedEvent(unsigned int index);

		virtual void preRenderFocusedItem(unsigned int index);
		virtual void preRenderBlurredItem(unsigned int index);
		virtual void renderItem(Graphics* g, VerticalMenuItem* item, signed int x, signed int y);

		unsigned int size();
		unsigned int sizeVisible();

		virtual ~VerticalMenu();

	protected:
		GameContainer* m_container;
		std::vector<VerticalMenuItem*> m_items;
		unsigned int m_spacing;
		signed int m_selected;
		bool m_wraparound;

};



#endif /* VERTICALMENU_H_ */
