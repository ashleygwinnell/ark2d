/*
 * Vibrator.cpp
 *
 *  Created on: 21 Aug 2013
 *      Author: Ashley
 */

#include "Vibrator.h"

// #include "../Includes.h"
#include "../Core/ARK2D.h"
#include "../Core/GameContainer.h"

#if defined(ARK2D_ANDROID)
	#include "../Core/Platform/GameContainerAndroid.h"
#endif

namespace ARK {
	namespace Util { 

		bool Vibrator::hasVibrator() {  
			#if defined(ARK2D_ANDROID) 
				return ARK2D::getContainer()->m_platformSpecific.m_pluggable->vibrator_hasVibrator();
			#endif
			return false;
		}
		void Vibrator::vibrate(float seconds) { 
			#if defined(ARK2D_ANDROID) 
				int millis = int(seconds * 1000);
				return ARK2D::getContainer()->m_platformSpecific.m_pluggable->vibrator_vibrate(millis);
			#endif
		}
		void Vibrator::cancel() { 
			#if defined(ARK2D_ANDROID)
				return ARK2D::getContainer()->m_platformSpecific.m_pluggable->vibrator_cancel();
			#endif
		}

	}
}



