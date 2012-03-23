/*
 * ToString.h
 *
 *  Created on: 24-Apr-2010
 *      Author: Ashley
 */

#ifndef ARKTOSTRING_H_
#define ARKTOSTRING_H_

#include <string>
using namespace std;

namespace ARK {
	/*!
	 * \brief Class to extend if you want to implement a toString method.
	 *
	 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
	 */
	class ToString {
		virtual string toString() = 0;
	};
}

#endif /* ARKTOSTRING_H_ */
