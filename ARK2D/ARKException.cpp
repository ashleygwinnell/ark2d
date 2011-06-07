/*
 * ARKException.cpp
 *
 *  Created on: 27 Sep 2010
 *      Author: Ashley
 */

#include "ARKException.h"
#include "ErrorDialog.h"

ARKException::ARKException():
	m_string("") {

}

ARKException::ARKException(char* c):
	m_string(c) {

}

ARKException::ARKException(const char* c):
	m_string(c) {

}

ARKException::ARKException(string s):
	m_string(s) {

}

void ARKException::showErrorDialog() {
	ErrorDialog::createAndShow(m_string);
}

ARKException::~ARKException() {

}
