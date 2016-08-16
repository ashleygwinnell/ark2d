/*
 * Exception.cpp
 *
 *  Created on: 27 Sep 2010
 *      Author: Ashley
 */

#include "Exception.h"
#include "Controls/ErrorDialog.h"

namespace ARK {
	namespace Core {

		Exception::Exception():
			m_string("") {

		}

		Exception::Exception(char* c):
			m_string(c) {

		}

		Exception::Exception(const char* c):
			m_string(c) {

		}

		Exception::Exception(string s):
			m_string(s) {

		}

		void Exception::showErrorDialog() {
			ErrorDialog::createAndShow(m_string);
		}

		Exception::~Exception() {

		}
	}
}
