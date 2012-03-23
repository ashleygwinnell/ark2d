//
//  testAppDelegate.h
//
//  Created by Ashley Gwinnell on 11/08/2011.
//  Copyright 2011 Ashley Gwinnell. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface GameContainerMacAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end
