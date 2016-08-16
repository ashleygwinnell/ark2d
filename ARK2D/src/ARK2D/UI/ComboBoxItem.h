/*
 * ComboBoxItem.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef ARK_UI_COMBOBOXITEM_H_
#define ARK_UI_COMBOBOXITEM_H_

#include "../Core/ARK2D.h"
#include "AbstractUIComponent.h"

#include "../Core/Graphics/Renderer.h"
#include "../Core/Graphics/Color.h"
#include "../Core/Geometry/GigaRectangle.h"
#include "../Core/Controls/Input.h"

namespace ARK {
	namespace UI {

		class ComboBox;

		/*!
		 * \brief A drop-down menu item.
		 */
		class ARK2D_API ComboBoxItem : public AbstractUIComponent {
			friend class ComboBox;
			private:
				ComboBox* parent;
				string m_text;
				string m_value;
				//unsigned int m_state;
			public:

				ComboBoxItem();
				ComboBoxItem(string text, string value);
				void setText(string t);
				const string& getText();

				void setValue(string t);
				const string& getValue();

				void setItemChangedEvent(void* event);

				void render();
				void renderBackground();
				void renderOverlay();
				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				bool mouseMoved(int x, int y, int oldx, int oldy);
		};
	}
}

#endif /* COMBOBOXITEM_H_ */
