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

#include "../ARK2D.h"
#include "AbstractUIComponent.h"
#include <vector>
using namespace std;

namespace ARK {
	namespace UI {
		class ComboBoxItem;

		/*!
		 * \brief A drop-down menu.
		 *
		 * Subscribe to change events for functionality.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API ComboBox : public AbstractUIComponent {
			friend class ComboBoxItem;
			private:
				vector<ComboBoxItem*> m_items;
				ComboBoxItem* m_selected;
				bool m_open;
				unsigned int m_state;
				void* m_itemChangedEvent;
				int m_originalHeight;

			public:
				ComboBox();
				void addItem(ComboBoxItem* cbi);
				void setOpen(bool b);
				bool isOpen();
				void setItemChangedEvent(void* event);
				ComboBoxItem* getSelected();
				void setSelectedByValue(string value);
				void setSize(unsigned int w, unsigned int h);
				void keyPressed(unsigned int key);
				void keyReleased(unsigned int key);
				void mouseMoved(int x, int y, int oldx, int oldy);
				void render();
				void renderBackground();
				void renderArrow();
				void renderText(int x, int y);
				void renderOverlay();

		};
	}
}

#endif /* COMBOBOX_H_ */
