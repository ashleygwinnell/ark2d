
/*
 * AnalyticsUtil.h
 *
 *  Created on: 19 May 2013
 *      Author: Ashley
 */

#ifndef ANALYTICS_UTIL_H_
#define ANALYTICS_UTIL_H_

#include "../Common/DLL.h"

#include <string>
using std::string;

namespace ARK {
	namespace Util {

		class ARK2D_API AnalyticsUtil {
			public:
				static void ga_init(string trackingId);
				static void ga_sendScreen(string name);
				static void ga_sendSocial(string network, string action, string targeturl);
				static void ga_sendEvent(string category, string action, string label, long opt_value);
				static void ga_sendTiming(string category, long loadTime, string name, string label);
		};

	}
}

#endif /* ANALYTICS_UTIL_H_ */
