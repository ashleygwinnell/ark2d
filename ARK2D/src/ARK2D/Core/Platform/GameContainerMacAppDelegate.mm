//
//  testAppDelegate.m
//  test
//
//  Created by Ashley Gwinnell on 11/08/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "GameContainerMacAppDelegate.h"

#import "../ARK2D.h"
//#import "../Log.h"

using namespace ARK::Core;

@implementation GameContainerMacAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
    NSLog(@"finished launching \r\n");
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

