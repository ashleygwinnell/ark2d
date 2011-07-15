/*
 * ComboBox.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef COMBOBOX_H_
#define COMBOBOX_H_

#include <vector>
using namespace std;

#include "../ARK.h"
#include "AbstractUIComponent.h"

class ComboBoxItem;

#include <vector>
using namespace std;

class ComboBox : public AbstractUIComponent {
	friend class ComboBoxItem;
	private:
		vector<ComboBoxItem*> m_items;
		ComboBoxItem* m_selected;
		bool m_open;
		void* m_itemChangedEvent;

	public:
		ComboBox();
		void addItem(ComboBoxItem* cbi);
		void setOpen(bool b);
		bool isOpen();
		void setItemChangedEvent(void* event);
		ComboBoxItem* getSelected();
		void setSelectedByValue(string value);
		void keyPressed(unsigned int key);
		void keyReleased(unsigned int key);
		void render();
		void renderBackground();
		void renderArrow();
		void renderText(int x, int y);
		void renderOverlay();

};

#endif /* COMBOBOX_H_ */
