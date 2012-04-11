/*
 * ToString.h
 *
 *  Created on: 24-Apr-2010
 *      Author: Ashley
 */

#ifndef ARK_CORE_TOSTRING_H_
#define ARK_CORE_TOSTRING_H_

#include <string>
using namespace std;

namespace ARK {
	namespace Core {

		/*!
		 * \brief Class to extend if you want to implement a toString method.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ToString {
			virtual string toString() = 0;
		};

	}
}

#endif /* ARK_CORE_TOSTRING_H_ */
