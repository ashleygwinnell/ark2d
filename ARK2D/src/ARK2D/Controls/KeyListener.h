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
				virtual void keyPressed(int key, char c) = 0;
				virtual void keyReleased(int key, char c) = 0;
		};

	}
}

#endif /* KEYLISTENER_H_ */
