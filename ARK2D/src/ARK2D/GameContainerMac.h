/*
 * GameContainerMac.h
 *
 *  Created on: Aug 9, 2011
 *      Author: ashleygwinnell
 */

#ifndef GAMECONTAINERMAC_H_
#define GAMECONTAINERMAC_H_

#include "ARK2D.h"

	#ifdef ARK2D_MACINTOSH

		#include <string>
		using namespace std;

		#import <objc/Object.h>
		#import <Cocoa/Cocoa.h>
		#import <OpenGL/CGLRenderers.h>
		#import <AppKit/AppKit.h>

		class GameContainerPlatform {
			public:
				static void* getARK2DResource(int resourceId, int resourceType);

				NSWindow* m_window;
		};


		@interface GameContainerPlatformListener
		{

		}
		- (void)doFunction;
		@end




	#endif

#endif /* GAMECONTAINERMAC_H_ */
