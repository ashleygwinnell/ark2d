/*
 * PathIO.h
 *
 *  Created on: 10 Jul 2011
 *      Author: Ashley
 */

#ifndef PATHIO_H_
#define PATHIO_H_

#include "PathGroup.h"
#include "../UI/ErrorDialog.h"
#include "../Util/StringUtil.h"
#include "../Tween/Easing.h"
#include "../Includes.h"

#include <string>
using namespace std;

namespace ARK {
	namespace Path {

		/*
		 * \brief Load paths using this class.
		 *
		 * You can also get the String value of a PathGroup with this class.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API PathIO {
			public:
				static PathGroup* createFromFile(string file);
				static PathGroup* createFromJSON(string json);
				static string getAsJSON(PathGroup* g);
		};

	}
}

#endif /* PATHIO_H_ */
