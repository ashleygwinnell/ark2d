
/*
 * SocialUtil.h
 *
 *  Created on: 22 May 2013
 *      Author: Ashley
 */

#ifndef SOCIAL_UTIL_H_
#define SOCIAL_UTIL_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include <string>
using namespace std;

namespace ARK {
	namespace Util {

		class ARK2D_API SocialUtil {
			public:
				static void share_googleplus(string text);
				static void share_twitter(string text, string via, string url);
				static void share_facebook(string url, string image, string title, string text);
		};

	}
}

#endif /* SOCIAL_UTIL_H_ */
