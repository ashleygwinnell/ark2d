/*
 * KeyListener.h
 *
 *  Created on: 12-Dec-2009
 *      Author: Ashley
 */

#ifndef KEYLISTENER_H_
#define KEYLISTENER_H_

namespace ARK {
	namespace Controls {

		/*!
		 * \brief Do not use.
		 *
		 * @deprecated Never implemented in to framework.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class KeyListener {
			public:
				void keyPressed(int key, char c);
				void keyReleased(int key, char c);
		};

	}
}

#endif /* KEYLISTENER_H_ */
