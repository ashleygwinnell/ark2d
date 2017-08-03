//
//  testAppDelegate.h
//
//  Created by Ashley Gwinnell on 11/08/2011.
//  Copyright 2011 Ashley Gwinnell. All rights reserved.
//

#ifdef ARK2D_MACINTOSH

#include "../../Namespaces.h"
#import <Cocoa/Cocoa.h>
#import "GameContainerAppleShared.h"

using namespace ARK::Core;

		@interface GameContainerMacAppDelegate : NSObject <NSApplicationDelegate, GameCenterManagerDelegate> {
			NSWindow *window;
			GameCenterManager* gameCenterManager;
		}

		@property (assign) IBOutlet NSWindow *window;
		@property (nonatomic, retain) GameCenterManager* gameCenterManager;
		@property (strong, nonatomic) GameICloudManager* icloud;

		@end

#endif
