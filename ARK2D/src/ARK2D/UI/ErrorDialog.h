/*
 * ErrorDialog.h
 *
 *  Created on: 23-Dec-2009
 *      Author: Ashley
 */

#ifndef ERRORDIALOG_H_
#define ERRORDIALOG_H_

#include "../Common/DLL.h"
#include "../Namespaces.h"

#include <string>
using namespace std;

namespace ARK {
	namespace UI {

		/*!
		 * \brief An error dialog. 
		 *
		 * @todo Create a singular dialogs class which does all the dialogs of all the types.
		 * @todo Deprecate this. See previous TODO.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API ErrorDialog {
			public:
				static void createAndShow(string message);
		};
	}
}

#endif /* ERRORDIALOG_H_ */
