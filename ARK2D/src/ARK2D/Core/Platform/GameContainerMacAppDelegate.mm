//
//  testAppDelegate.m
//  test
//
//  Created by Ashley Gwinnell on 11/08/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "GameContainerMacAppDelegate.h"

using namespace ARK::Core;

@implementation GameContainerMacAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
    printf("finished launching \r\n");
}
 
- (void)applicationDidHide:(NSNotification *)notification {
    printf("closed application\r\n");
}

- (void)applicationWillHide:(NSNotification *)notification {
    printf("will hide now kthx\r\n");
}

- (void) applicationWillTerminate:(NSNotification *)notification {
    printf("will now terminate kthx\r\n");
}

-(void) applicationDidResignActive:(NSNotification *)notification {
    //printf("resigned active");
}


-(void)windowWillEnterFullScreen:(NSNotification* )theEvent { }
-(void)windowDidEnterFullScreen:(NSNotification* )theEvent { }
-(void)windowWillExitFullScreen:(NSNotification* )theEvent { }
-(void)windowDidExitFullScreen:(NSNotification* )theEvent { }


@end
		
