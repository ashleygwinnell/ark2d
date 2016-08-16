/*
 * SubPath.h
 *
 *  Created on: 5 Jul 2011
 *      Author: Ashley
 */

#ifndef ARK_PATH_SUBPATH_H_
#define ARK_PATH_SUBPATH_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"
#include "../Core/Geometry/Vector2.h"

#include <string>
#include <vector>
using namespace std;

namespace ARK {
	namespace Path {

		class PathGroup;
		class Path;

		/*
		 * \brief Used internally to support path functionality. You shouldn't need to use this.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API SubPath {
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
