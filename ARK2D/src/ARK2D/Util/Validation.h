/*
 * Validation.h
 *
 *  Created on: 13 Feb 2013
 *      Author: Ashley
 */

#ifndef VALIDATION_H_
#define VALIDATION_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include <string>
#include <vector>
using namespace std;

namespace ARK {
	namespace Util {

		class ARK2D_API Validation {
			public:
				static bool isAlphanumeric(string s);
				static bool isAlphanumericSpaces(string s);
				static bool isCharset(const vector<char>& chars, string s);
		};

	}
}


#endif /* VALIDATION_H_ */
