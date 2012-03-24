/*
 * ComboBoxItem.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef COMBOBOXITEM_H_
#define COMBOBOXITEM_H_

#include "../ARK2D.h"
#include "AbstractUIComponent.h"

#include "../Graphics.h"
#include "../Graphics/Color.h"
#include "../Geometry/GigaRectangle.h"
#include "../Controls/Input.h"

namespace ARK {
	namespace UI {

		class ComboBox;

		/*!
		 * \brief A drop-down menu item.
		 */
		class ComboBoxItem : public AbstractUIComponent {
			friend class ComboBox;
			private:
				ComboBox* parent;
				string m_text;
				string m_value;
				unsigned int m_state;
			public:

				ComboBoxItem();
				void setText(string t);
				const string& getText();

				void setValue(string t);
				const string& getValue();

				void setItemChangedEvent(void* event);

				void render();
				void renderBackground();
				void renderOverlay();
				void keyPressed(unsigned int key);
				void mouseMoved(int x, int y, int oldx, int oldy);
		};
	}
}

#endif /* COMBOBOXITEM_H_ */
