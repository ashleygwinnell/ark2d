/*
 * DisplayUtil.h
 *
 *  Created on: 11 May 2014
 *      Author: Ashley
 */

#ifndef ARK_UTIL_DISPLAYUTIL_H_
#define ARK_UTIL_DISPLAYUTIL_H_

#include "../Common/DLL.h"
#include "../Namespaces.h"

namespace ARK {
	namespace Util {

		class ARK2D_API DisplayUtil {
			public:
				static bool s_fullscreen320x480;
				static signed int s_fullscreen320x480_translateX;
				static void resize320x480(GameContainer* container, int width, int height);
				static void setFullscreenMode320x480();
				static bool isFullscreenMode320x480();
				static signed int getFullscreenModeTranslateX();


				// All of this stuff is copied
				// directly from Toast Time source.
				// It's very not likely relevant to you.
				// but shows how different display sizes were handled
				// in that game.

				template <class T>
				static T screenY(T y) {
					return adjustY<T>(y);
				}
				template <class T>
				static T adjustYBG(T y) {
					if (res() == RES_SHORT) {
						return y - 53;
					} else if (res() == RES_SHORTER) {
						return y - 74;
					}
					return adjustY<T>(y);
				}
				template <class T>
				static T adjustYBGIG(T y) {
					if (res() == RES_SHORT) {
						return (y - 53); // 377 is ground
					} else if (res() == RES_SHORTER) {
						return (y - 74);
					}
					return adjustY<T>(y);
				}


				static float pcY(float y);
				static float pcY2(float y);
				static float pcY3(float y);
				static float pcY4(float y);

				static float pc();

				template <class T>
				static T adjustY(T y) {
					if (res() == RES_TALLISH) {
						return y + 3;
					} else if (res() == RES_TALL) {
						return y + 24;
					} else if (res() == RES_TALLER) {
						return y + 44;
					} else if (res() == RES_SHORT) {
						return y - 26.5;
					} else if (res() == RES_SHORTER) {
						return y - 37;
					}
					return y;
				}
				template <class T>
				static T adjustY2(T y) {
					if (res() == RES_TALLISH) {
						return y + 6;
					} else if (res() == RES_TALL) {
						return y + 48;
					} else if (res() == RES_TALLER) {
						return y + 88;
					} else if (res() == RES_SHORT) {
						return y - 53;
					} else if (res() == RES_SHORTER) {
						return y - 74;
					}
					return y;
				}

				template <class T>
				static T adjustY3(T y) {
					if (res() == RES_SHORT) {
						return y - 26.5;
					} else if (res() == RES_SHORTER) {
						return y - 37;
					}
					return y;
				}

				template <class T>
				static T adjustY4(T y) {
					if (res() == RES_SHORT) {
						return y - 53;
					} else if (res() == RES_SHORT) {
						return y - 74;
					}
					return y;
				}

				template <class T>
				static T adjustY5(T y) {
					if (res() == RES_TALL || res() == RES_TALLER) {
						return y + 24;
					} else if (res() == RES_SHORT) {
						return y - 26.5;
					} else if (res() == RES_SHORTER) {
						return y - 37;
					}
					return y;
				}


				static float sc();
				static float sc2();
				static float sc3();
				static float sc4();

				static const unsigned int RES_ORIGINAL = 0; // iphone 3gs.
				static const unsigned int RES_TALLISH = 1; // iconia tab 8.
				static const unsigned int RES_TALL = 2; // galaxy nexus, xperia z1
				static const unsigned int RES_SHORT = 3; // ipad 1.33333
				static const unsigned int RES_SHORTER = 5; // nexus9: 1536 x 1952 1.270833333
				static const unsigned int RES_TALLER = 4; // iphone 5
				static unsigned int res();
		};
	}
}

#endif
