/*
 * ARKException.h
 *
 *  Created on: 27 Sep 2010
 *      Author: Ashley
 */

#ifndef ARKEXCEPTION_H_
#define ARKEXCEPTION_H_

#include "ARK2D.h"

using namespace std;

class ARKException {
	public:
		ARKException();
		ARKException(char* c);
		ARKException(const char* c);
		ARKException(string s);

		void showErrorDialog();

		virtual ~ARKException();

	private:
		string m_string;
};

#endif /* ARKEXCEPTION_H_ */
