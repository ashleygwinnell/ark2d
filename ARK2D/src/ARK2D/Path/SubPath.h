/*
 * SubPath.h
 *
 *  Created on: 5 Jul 2011
 *      Author: Ashley
 */

#ifndef SUBPATH_H_
#define SUBPATH_H_

class PathGroup;
class Path;

#include <vector>
#include "../Geometry/Vector2.h"
using namespace std;

namespace ARK {
	namespace Path {

		/*
		 * \brief Used internally to support path functionality. You shouldn't need to use this.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class SubPath {
			friend class PathGroup;
			friend class Path;

			private:
				vector<Vector2<float>*> points;
			public:
				SubPath();
				virtual ~SubPath();
		};
	}
}

#endif /* SUBPATH_H_ */
