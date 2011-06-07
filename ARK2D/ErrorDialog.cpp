/*
 * ErrorDialog.cpp
 *
 *  Created on: 23-Dec-2009
 *      Author: Ashley
 */

#include <stdlib.h>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#include "ErrorDialog.h"

void ErrorDialog::createAndShow(std::string message) {
	#ifdef _WIN32
		MessageBox(NULL, message.c_str(), NULL, MB_OK | MB_ICONEXCLAMATION);
	#endif
}
