/*
 * VerticalMenuItem.h
 *
 *  Created on: 17 Aug 2010
 *      Author: Ashley
 */

#ifndef VERTICALMENUITEM_H_
#define VERTICALMENUITEM_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include <string>
using std::string;

/*!
 * @deprecated
 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
 */
class ARK2D_API VerticalMenuItem {
	public:
		VerticalMenuItem();
		VerticalMenuItem(const char* text);

		void setText(const char* text);
		void setVisible(bool b);
		bool isVisible();

		void setOnPressEvent(Event* e);
		Event* getOnPressEvent();

		const char* getText();

		~VerticalMenuItem();

	private:
		std::string m_text;
		bool m_visible;
		Event* m_on_press_event;
};

#endif /* VERTICALMENUITEM_H_ */
