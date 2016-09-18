/*
 * DisplayUtil.cpp
 *
 *  Created on: 11 May 2014
 *      Author: Ashley
 */

#include "DisplayUtil.h"
#include "../Includes.h"
#include "../Core/GameContainer.h"

namespace ARK {
	namespace Util {
		bool DisplayUtil::s_fullscreen320x480 = false;
		signed int DisplayUtil::s_fullscreen320x480_translateX = 0;

		bool DisplayUtil::isFullscreenMode320x480() {
			return s_fullscreen320x480;
		}
		signed int DisplayUtil::getFullscreenModeTranslateX() {
			return s_fullscreen320x480_translateX;
		}

		void DisplayUtil::setFullscreenMode320x480() {
			GameContainer* container = ARK2D::getContainer();

			float localWidth = float(container->getScreenWidth());
			float localHeight = float(container->getScreenHeight());
			if (container->isOrientationInverted()) {
				localWidth = float(container->getScreenHeight());
				localHeight = float(container->getScreenWidth());
			}

			float screenRatio = localWidth / localHeight;
			//ARK2D::getLog()->e(StringUtil::appendf("screenratio: ", screenRatio));
			if (screenRatio == 1.8f) {
				s_fullscreen320x480 = true;
				s_fullscreen320x480_translateX = (864-320)/2;

		 		container->m_originalWidth = 864;
		 		container->m_originalHeight = 480;
		 		container->setSize(864, 480);
		 	} else if (screenRatio > 1.778f && screenRatio < 1.779f) { // Sarika's screen size: 1366x768

				s_fullscreen320x480 = true;
				s_fullscreen320x480_translateX = (856-320)/2;

		 		container->m_originalWidth = 854;
		 		container->m_originalHeight = 480;
		 		container->setSize(856, 480);

		 	} else if (screenRatio > 1.77776f && screenRatio <= 1.77778f) {

				s_fullscreen320x480 = true;
				s_fullscreen320x480_translateX = (854-320)/2;

		 		container->m_originalWidth = 854;
		 		container->m_originalHeight = 480;
		 		container->setSize(854, 480);

		 	} else if (screenRatio == 1.6f) {
		 		s_fullscreen320x480 = true;
				s_fullscreen320x480_translateX = (768-320)/2;

		 		container->m_originalWidth = 768;
		 		container->m_originalHeight = 480;
		 		container->setSize(768, 480);
		 	} else if (screenRatio == 1.5625f) {
		 		s_fullscreen320x480 = true;
				s_fullscreen320x480_translateX = (750-320)/2;

		 		container->m_originalWidth = 750;
		 		container->m_originalHeight = 480;
		 		container->setSize(750, 480);
		 	//}
		 	//else if (screenRatio == 1.52f) {
		 	// 	s_fullscreen320x480 = true;
				// s_fullscreen320x480_translateX = (720-320)/2;

		 	// 	container->m_originalWidth = 720;
		 	// 	container->m_originalHeight = 480;
		 	// 	container->setSize(720, 480);
		 	} else if (screenRatio == 1.5f) {
		 		s_fullscreen320x480 = true;
				s_fullscreen320x480_translateX = (720-320)/2;

		 		container->m_originalWidth = 720;
		 		container->m_originalHeight = 480;
		 		container->setSize(720, 480);
		 	} else if (screenRatio > 1.333332f && screenRatio < 1.333334f) {
		 		s_fullscreen320x480 = true;
				s_fullscreen320x480_translateX = (640-320)/2;

		 		container->m_originalWidth = 640;
		 		container->m_originalHeight = 480;
		 		container->setSize(640, 480);
		 	} else if (screenRatio == 1.25f) {
		 		s_fullscreen320x480 = true;
				s_fullscreen320x480_translateX = (600-320)/2;

		 		container->m_originalWidth = 600;
		 		container->m_originalHeight = 480;
		 		container->setSize(600, 480);
		 	}
		 	else {
		 		resize320x480(container, localWidth, localHeight);
		 		container->resizeBehaviour(localWidth, localHeight);
		 	}
		 	container->setFullscreen(true);
		}


		void DisplayUtil::resize320x480(GameContainer* container, int width, int height)
		{
			float localWidth = (float) container->getDynamicWidth();
			float localHeight = (float) container->getDynamicHeight();
			if (container->isOrientationInverted()) {
				localWidth = (float) container->getDynamicHeight();
				localHeight = (float) container->getDynamicWidth();
			}

			float ratio = localHeight / localWidth;
			if (ratio == 1.5f)
			{
				s_fullscreen320x480 = false;
				s_fullscreen320x480_translateX = 0;

				container->m_originalWidth = 320;
				container->m_originalHeight = 480;
				#if defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE)
					container->setSizeNoCallback(width, height);
				#endif
			}
			else if (ratio == 1.52f) // Acer Iconia Tab 8
			{
				container->m_originalWidth = 320;
				container->m_originalHeight = 486;
				#if defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE)
					container->setSizeNoCallback(width, height);
				#endif
			}
			// iPhone 5, HTC One.
			else if (ratio >= 1.775f)
			{
				container->m_originalWidth = 320;
				container->m_originalHeight = 568;
				#if defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE)
					container->setSizeNoCallback(width, height);
				#endif
			}
			// Android ratios are basically the same?! 15:9?
			else if ((ratio > 1.64f && ratio <= 1.65f) || ratio >= 1.66f)
			{
				container->m_originalWidth = 320;
				container->m_originalHeight = 528;
				#if defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE)
					container->setSizeNoCallback(width, height);
				#endif
			}
			// iPad
			else if ((ratio >= 1.332f && ratio <= 1.334f) || ratio == 1.33125f || ratio == 1.334375f || ratio == 1.33438f)
			{
				container->m_originalWidth = 320;
				container->m_originalHeight = 427;
				#if defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE)
					container->setSizeNoCallback(width, height);
				#endif
			}
			// Nexus9
			else if (ratio >= 1.2708f && ratio <= 1.2709f)
			{
				/*container->m_originalWidth = 320;
				container->m_originalHeight = 406; // realsmall
				#if defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE)
					container->setSizeNoCallback(width, height);
				#endif*/

				// treat as ipad for now.
				container->m_originalWidth = 320;
				container->m_originalHeight = 427;
				#if defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE)
					container->setSizeNoCallback(width, height);
				#endif
			}

			// if it's more portrait than landscape!
			#ifdef ARK2D_WINDOWS
				else if (ratio > 1.0f) {
					s_fullscreen320x480 = true;
					s_fullscreen320x480_translateX = (localWidth-(320*container->getScale()))/2;

					container->m_originalWidth = 320;
					container->m_originalHeight = 480;
					container->setSize(width, height);
				}
			#endif
			//else if (width == 320 && height == 480) {

			//}
		}

		float DisplayUtil::sc() { return float(ARK2D::getContainer()->getHeight()) / 480.0f; }
		float DisplayUtil::sc2() { float scl = float(ARK2D::getContainer()->getHeight()) / 480.0f; if (scl < 1.0f) { return 1.0f; } return scl; }
		float DisplayUtil::sc3() { float scl = float(ARK2D::getContainer()->getHeight()) / 480.0f; if (scl > 1.0f) { return 1.0f; } return scl; }
		float DisplayUtil::sc4() { float scl = float(ARK2D::getContainer()->getHeight()) / 480.0f; if (scl > 1.1f) { return 1.1f; } return scl; } // cap at RES_TALL


		unsigned int DisplayUtil::res()
		{
			// ratios
			// 1.5f -- iphone / original.
			// 1.48 -- nexus 7
			// 1.644444444 - galaxy nexus
			// 1.33125 - iPad
			// 1.270833333 - nexus 9

			GameContainer* container = ARK2D::getContainer();
			float ratio = float(container->getHeight()) / float(container->getWidth());
			//ARK2D::getLog()->g(Cast::toString<float>(ratio));
			if (ratio == 1.5f) {
				return RES_ORIGINAL;
			} else if (ratio == 1.52f) {
				return RES_TALLISH;
			} else if (ratio > 1.64f && ratio <= 1.65f) {
				return RES_TALL;
			} else if (ratio >= 1.775f) { // iPhone 5, HTC One.
				return RES_TALLER;
			} else if (ratio >= 1.66f) {
				return RES_TALL;
			} else if ((ratio >= 1.332f && ratio <= 1.334f) || ratio == 1.33125f || ratio == 1.334375f) {
				return RES_SHORT;
			} else if (ratio >= 1.2708f && ratio <= 1.2709f) {
				//return RES_SHORTER;
				return RES_SHORT;
			}
			return RES_ORIGINAL;
		}

		float DisplayUtil::pc() {
			return float(ARK2D::getContainer()->getHeight()) / 480.0f;
		}

		float DisplayUtil::pcY(float y) {
			GameContainer* container = ARK2D::getContainer();
			if (res() == RES_TALLISH) {
				float sc = float(container->getHeight()) / 480.0f;
				return (float) (y * sc);
			} else if (res() == RES_TALL) {
				float sc = float(container->getHeight()) / 480.0f;
				return (float) (y * sc);
			} else if (res() == RES_TALLER) {
				float sc = float(container->getHeight()) / 480.0f;
				return (float) (y * sc);
			} else if (res() == RES_SHORT) {
				float sc = float(container->getHeight()) / 480.0f;
				return (float) (y * sc);
			} else if (res() == RES_SHORTER) {
				float sc = float(container->getHeight()) / 480.0f;
				return (float) (y * sc);
			}
			return y;
		}



		float DisplayUtil::pcY2(float y) {
			GameContainer* container = ARK2D::getContainer();
			if (res() == RES_TALL || res() == RES_TALLER) {
				float sc = float(container->getHeight()) / 480.0f;
				return (float) (y * sc);
			}
			return y;
		}


		float DisplayUtil::pcY3(float y) {
			GameContainer* container = ARK2D::getContainer();
			if (res() == RES_TALL || res() == RES_TALLER) {
				float sc = 1.0f / (float(container->getHeight()) / 480.0f);
				return (float) (y * sc);
			}
			return y;
		}


		float DisplayUtil::pcY4(float y) {
			GameContainer* container = ARK2D::getContainer();
			if (res() == RES_TALLISH) {
				float sc = float(container->getHeight()) / 480.0f;
				return (float) (y * sc);
			}else if (res() == RES_TALL) {
				float sc = float(container->getHeight()) / 480.0f;
				return (float) (y * sc);
			} else if (res() == RES_TALLER) {
				float sc = 528.0f / 480.0f;
				return (float) (y * sc);
			} else if (res() == RES_SHORT) {
				float sc = float(container->getHeight()) / 480.0f;
				return (float) (y * sc);
			} else if (res() == RES_SHORTER) {
				float sc = float(container->getHeight()) / 480.0f;
				return (float) (y * sc);
			}
			return y;
		}

	}
}
