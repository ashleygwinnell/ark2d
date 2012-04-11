//
//  WindowListener.h
//
//  Created by Ashley Gwinnell on 11/08/2011.
//  Copyright 2011 Ashley Gwinnell. All rights reserved.
//
#include "../../Namespaces.h"

#import <Cocoa/Cocoa.h>

using namespace ARK::Core;

@interface GameContainerMacWindowListener : NSResponder <NSWindowDelegate> {
	NSWindow* m_window;
}

-(void)init:(NSWindow* )window;

-(BOOL)windowShouldClose:(id)sender;
-(void)windowWillClose:(NSNotification *)notification;

-(void)keyDown:(NSEvent *)theEvent;
-(void)keyUp:(NSEvent *)theEvent;

-(void)flagsChanged:(NSEvent *)theEvent;

-(void)mouseDown:(NSEvent *)theEvent;
-(void)mouseUp:(NSEvent *)theEvent;
-(void)mouseMoved:(NSEvent *)theEvent;

@end
