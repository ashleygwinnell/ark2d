/*
 * Label.h
 *
 *  Created on: 23 Jul 2011
 *      Author: Ashley
 */

#ifndef LABEL_H_
#define LABEL_H_

#include "AbstractUIComponent.h"

#include <string>
using namespace std;

namespace ARK {
	namespace UI {

		/*!
		 * \brief A generic text label.
		 *
		 * @todo Set font functionality.
		 * @todo Text alignment left top right bottom etc.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Label : public AbstractUIComponent {
			private:
				string m_text;
				signed int m_alignX;
				signed int m_alignY;

			public:
				Label();
				Label(string text);
				Label(string text, signed int alignX, signed int alignY);
				Label(string text, signed int alignX, signed int alignY, float scale);
				void setText(string text);
				void setText(string text, bool updateSize);
				void render();
				void renderBounds();
				virtual ~Label();
		};
	}
}
#endif /* LABEL_H_ */
