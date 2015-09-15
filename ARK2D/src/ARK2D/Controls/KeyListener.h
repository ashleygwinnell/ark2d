/*
 * KeyListener.h
 *
 *  Created on: 12-Dec-2009
 *      Author: Ashley
 */

#ifndef ARK_CONTROLS_KEYLISTENER_H_
#define ARK_CONTROLS_KEYLISTENER_H_

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
				virtual bool keyPressed(unsigned int key) = 0; // return true if event consumed. otherwise return false.
				virtual bool keyReleased(unsigned int key) = 0;
		};

	}
}

#endif /* KEYLISTENER_H_ */
