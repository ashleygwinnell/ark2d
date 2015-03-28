/*
 * Camera.h
 *
 *  Created on: 02 Sep 2012
 *      Author: Ashley
 */

#ifndef ARK_CORE_CAMERA_H_
#define ARK_CORE_CAMERA_H_

#include "../ARK2D.h"

using namespace std;

namespace ARK {
	namespace Core {
		class ARK2D_API Camera {
			private:
				ARK::Geometry::Rectangle<int>* m_rect;
			public:
				Camera();
				void reposition();
				void setX(int x);
				void setY(int y);
				void setViewRectangle(int x, int y, int w, int h);
				virtual ~Camera();
		};
	}
}

#endif /* ARK_CORE_CAMERA_H_ */
