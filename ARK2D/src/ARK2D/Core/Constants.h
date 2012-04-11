/*
 * Constants.h
 *
 *  Created on: 13 Nov 2010
 *      Author: Ashley
 */

#ifndef ARK_CORE_CONSTANTS_H_
#define ARK_CORE_CONSTANTS_H_

namespace ARK {
	namespace Core {

		/*!
		 * \brief Constants for the core internal game framework.
		 *
		 * @todo Find somewhere else that is more suitable for this.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class Constants {
			public:
				static const unsigned int DIRECTION_UP = 0;
				static const unsigned int DIRECTION_DOWN = 1;
				static const unsigned int DIRECTION_LEFT = 2;
				static const unsigned int DIRECTION_RIGHT = 3;
		};

	}
}

#endif /* ARK_CORE_CONSTANTS_H_ */
