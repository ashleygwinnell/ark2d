/*
 * ErrorDialog.h
 *
 *  Created on: 23-Dec-2009
 *      Author: Ashley
 */

#ifndef ERRORDIALOG_H_
#define ERRORDIALOG_H_

#include <stdlib.h>
#include <string>

class ErrorDialog {
	public:
		static void createAndShow(std::string message);
};

#endif /* ERRORDIALOG_H_ */
