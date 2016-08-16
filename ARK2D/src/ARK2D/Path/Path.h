/*
 * Path.h
 *
 *  Created on: 5 Jul 2011
 *      Author: Ashley
 */

#ifndef ARK_PATH_PATH_H_
#define ARK_PATH_PATH_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"
#include "../Core/Geometry/Vector2.h"

#include <vector>
using namespace std;

namespace ARK {
	namespace Path {

		class PathGroup;
		class SubPath;

		/*!
		 * \brief Used internally by PathGroup.
		 *
		 * A path conceptually consists of two major points and many minor points in between.
		 * In practice, it needs to consist of many smaller paths (group of poiints) with an increasing number of points.
		 *
		 * @see http://en.wikipedia.org/wiki/B%C3%A9zier_curve#Quadratic_curves
		 * @see http://en.wikipedia.org/wiki/File:Bezier_3_big.gif
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Path {
			friend class ARK::Path::PathGroup;

			private:
				vector<SubPath*> subpaths;
				float timer;
				float duration;
				unsigned int easing;

			public:
				Path();

				void setDuration(float d);
				float getDuration();

				void setEasing(unsigned int e);
				unsigned int getEasing();

				void addPoint(float x, float y);
				void addPoint(float x, float y, int index);
				void removePoint(int index);
				Vector2<float>* getPoint(unsigned int i);
				vector<Vector2<float>*> getPoints();
				unsigned int getSize();

				virtual ~Path();
		};
	}
}
#endif /* PATH_H_ */
