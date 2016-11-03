/*
 * SocialUtil.cpp
 *
 *  Created on: 22 May 2013
 *      Author: Ashley
 */

#include "SocialUtil.h"

//#include "../Includes.h"
#include "../Core/ARK2D.h"
#include "../Core/GameContainer.h"

#include "../Core/Util/SystemUtil.h"

#if defined(ARK2D_ANDROID)
	#include "../Core/Platform/GameContainerAndroid.h"
#endif

namespace ARK {
	namespace Util {

		void SocialUtil::share_googleplus(string text) {
			#if defined(ARK2D_ANDROID)
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->share_googleplus(text);
			#else
				string url = "https://m.google.com/app/plus/x/toasttime/?v=compose&content=";
				url += StringUtil::urlencode(text);
				SystemUtil::openBrowserToURL(url);
			#endif
		}

		void SocialUtil::share_twitter(string text, string via, string url) {
			string request = string("https://twitter.com/intent/tweet?text=");
			request += StringUtil::urlencode(text);

			if (via.length() > 0) {
				request += string("&via=");
				request += StringUtil::urlencode(via);
			}

			if (url.length() > 0) {
				request += string("&url=");
				request += StringUtil::urlencode(url);
			}

			SystemUtil::openBrowserToURL(request);
		}

		void SocialUtil::share_facebook(string url, string image, string title, string text) {
			string request = "http://www.facebook.com/sharer/sharer.php?s=100";
			request += StringUtil::append("&p[url]=", StringUtil::urlencode(url));
			request += StringUtil::append("&p[images][0]=", StringUtil::urlencode(image));
			request += StringUtil::append("&p[title]=", StringUtil::urlencode(title));
			request += StringUtil::append("&p[summary]=", StringUtil::urlencode(text));
			SystemUtil::openBrowserToURL(request);

		}

	}
}



