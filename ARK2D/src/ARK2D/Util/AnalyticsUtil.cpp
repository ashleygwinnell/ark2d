/*
 * AnalyticsUtil.cpp
 *
 *  Created on: 19 May 2013
 *      Author: Ashley
 */

#include "AnalyticsUtil.h"

//#include "../Includes.h"
#include "../Core/ARK2D.h"
#include "../Core/GameContainer.h"

#if defined(ARK2D_ANDROID)
	#include "../Core/Platform/GameContainerAndroid.h"
#endif

namespace ARK {
	namespace Util { 

		void AnalyticsUtil::ga_init(string trackingId) {
			#ifdef ARK2D_IPHONE
				ARK2D::getLog()->v(StringUtil::append("GA Init: ", trackingId));

				NSString* nsTrackingId = [NSString stringWithCString:trackingId.c_str() encoding:[NSString defaultCStringEncoding]];

				[GAI sharedInstance].trackUncaughtExceptions = YES;

				// Optional: set Google Analytics dispatch interval to e.g. 20 seconds.
				[GAI sharedInstance].dispatchInterval = 30;

				// Optional: set Logger to VERBOSE for debug information.
				#ifdef ARK2D_DEBUG
					[[[GAI sharedInstance] logger] setLogLevel:kGAILogLevelVerbose];
				#endif

				// Initialize tracker.
				id<GAITracker> tracker = [[GAI sharedInstance] trackerWithTrackingId:nsTrackingId];

			#endif
		}
		void AnalyticsUtil::ga_sendScreen(string name) {
			#ifdef ARK2D_IPHONE
				NSString* nsName = [NSString stringWithCString:name.c_str() encoding:[NSString defaultCStringEncoding]]; 
				// Send screen...
				id<GAITracker> tracker = [[GAI sharedInstance] defaultTracker];
				[tracker set:kGAIScreenName value:nsName];
				[tracker send:[[GAIDictionaryBuilder createAppView] build]];
			#endif
		}

		void AnalyticsUtil::ga_sendSocial(string network, string action, string targeturl) {
			#if defined(ARK2D_ANDROID)
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->ga_sendSocial(network, action, targeturl);
			#elif defined(ARK2D_IPHONE)

				NSString* nsNetwork = [NSString stringWithCString:network.c_str() encoding:[NSString defaultCStringEncoding]];
				NSString* nsAction = [NSString stringWithCString:action.c_str() encoding:[NSString defaultCStringEncoding]];
				NSString* nsTargeturl = [NSString stringWithCString:targeturl.c_str() encoding:[NSString defaultCStringEncoding]];

				id<GAITracker> tracker = [[GAI sharedInstance] defaultTracker];
				[tracker send:[[GAIDictionaryBuilder createSocialWithNetwork:nsNetwork          // Social network (required)
					action:nsAction            		// Social action (required)
					target:nsTargeturl] build]];  	// Social target
			#endif
		}  
		void AnalyticsUtil::ga_sendEvent(string category, string action, string label, long opt_value) {
			#if defined(ARK2D_ANDROID)
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->ga_sendEvent(category, action, label, opt_value);
			#elif defined(ARK2D_IPHONE)
				NSString* nsCategory = [NSString stringWithCString:category.c_str() encoding:[NSString defaultCStringEncoding]];
				NSString* nsAction = [NSString stringWithCString:action.c_str() encoding:[NSString defaultCStringEncoding]];
				NSString* nsLabel = [NSString stringWithCString:label.c_str() encoding:[NSString defaultCStringEncoding]];
				NSNumber* nsValue = [NSNumber numberWithLong:opt_value];
			
				id<GAITracker> tracker = [[GAI sharedInstance] defaultTracker];
				[tracker send:[[GAIDictionaryBuilder createEventWithCategory:nsCategory     // Event category (required)
					action:nsAction  			// Event action (required)
					label:nsLabel          		// Event label
					value:nsValue] build]];   // Event value
			#endif
		}
		void AnalyticsUtil::ga_sendTiming(string category, long loadTime, string name, string label) {
			#if defined(ARK2D_ANDROID)
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->ga_sendTiming(loadTime, category, name, label);
			#elif defined(ARK2D_IPHONE)
				NSString* nsCategory = [NSString stringWithCString:category.c_str() encoding:[NSString defaultCStringEncoding]];
				NSString* nsName = [NSString stringWithCString:name.c_str() encoding:[NSString defaultCStringEncoding]];
				NSString* nsLabel = [NSString stringWithCString:label.c_str() encoding:[NSString defaultCStringEncoding]];
				NSNumber* nsLoadTime = [NSNumber numberWithLong:loadTime];

				id<GAITracker> tracker = [[GAI sharedInstance] defaultTracker];
  				[tracker send:[[GAIDictionaryBuilder createTimingWithCategory:nsCategory    // Timing category (required)
					interval:nsLoadTime       // Timing interval (required)
					name:nsName  			// Timing name
					label:nsLabel] build]];  // Timing label
			#endif
		} 

	}
}



