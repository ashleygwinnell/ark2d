/*
 * Exception.h
 *
 *  Created on: 27 Sep 2010
 *      Author: Ashley
 */

#ifndef ARK_CORE_EXCEPTION_H_
#define ARK_CORE_EXCEPTION_H_

#include "ARK2D.h"
 
/*!
 * @deprecated
 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
 */
namespace ARK {
	namespace Core {
		class ARK2D_API Exception {
			public:
				Exception();
				Exception(char* c);
				Exception(const char* c);
				Exception(string s);

				void showErrorDialog();

				virtual ~Exception();

			private:
				string m_string;
		};
	}
}

#endif /* ARK_CORE_EXCEPTION_H_ */
