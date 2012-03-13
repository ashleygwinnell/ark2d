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
	class ToString {
		virtual string toString() = 0;
	};
}

#endif /* ARKTOSTRING_H_ */
