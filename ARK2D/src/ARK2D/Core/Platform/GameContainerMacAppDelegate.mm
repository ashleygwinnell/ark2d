//
//  testAppDelegate.m
//  test
//
//  Created by Ashley Gwinnell on 11/08/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//



#ifdef ARK2D_MACINTOSH

#import "GameContainerMacAppDelegate.h"
#import "../GameContainer.h"

	#import "../ARK2D.h"
	//#import "../Log.h"

	using namespace ARK::Core;

	@implementation GameContainerMacAppDelegate

	@synthesize window;
	@synthesize gameCenterManager;
	@synthesize icloud = _icloud;

	- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
	{
	    // Insert code here to initialize your application
	    NSLog(@"finished launching \r\n");

	    self.icloud = [[GameICloudManager alloc] init];
	    //ARK2D::getContainer()->m_platformSpecific.m_appDelegate = self;
	}

	- (void)applicationDidHide:(NSNotification *)notification {
	  NSLog(@"closed application\r\n");
	}

	- (void)applicationWillHide:(NSNotification *)notification {
	    NSLog(@"will hide now\r\n");
	}

	- (void) applicationWillTerminate:(NSNotification *)notification {
	    NSLog(@"will now terminate\r\n");
	}

	-(void) applicationDidResignActive:(NSNotification *)notification {
	    //printf("resigned active");
	    NSLog(@"DidResignActive\r\n");
	}


	-(void)windowWillEnterFullScreen:(NSNotification* )theEvent { }
	-(void)windowDidEnterFullScreen:(NSNotification* )theEvent { }
	-(void)windowWillExitFullScreen:(NSNotification* )theEvent { }
	-(void)windowDidExitFullScreen:(NSNotification* )theEvent { }


	@end

#endif

