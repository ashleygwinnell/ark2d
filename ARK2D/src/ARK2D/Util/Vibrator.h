
/*
 * Vibrator.h
 *
 *  Created on: 21 Aug 2013
 *      Author: Ashley
 */

#ifndef ARK_UTIL_VIBRATOR_H_
#define ARK_UTIL_VIBRATOR_H_

#include "../Includes.h"

namespace ARK {
	namespace Util {
		 
		class ARK2D_API Vibrator {
			public:
				static bool hasVibrator();
				static void vibrate(float seconds);
				static void cancel();
		};
		 
	}
}

#endif /* ARK_UTIL_VIBRATOR_H_ */
