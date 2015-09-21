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
	@public NSEvent* m_latestKeyUpEvent;
	
}


-(void)init:(NSWindow* )window; 

-(BOOL)windowShouldClose:(id)sender;
-(void)windowWillClose:(NSNotification *)notification;

-(void)keyDown:(NSEvent *)theEvent;
-(void)keyUp:(NSEvent *)theEvent;

-(void)flagsChanged:(NSEvent *)theEvent;

- (void)cancelOperation:(id)sender;

-(void)mouseDown:(NSEvent *)theEvent;
-(void)mouseUp:(NSEvent *)theEvent;
-(void)mouseMoved:(NSEvent *)theEvent;

- (void)rightMouseDown:(NSEvent*)theEvent;
- (void)rightMouseUp:(NSEvent*)theEvent;

- (BOOL) acceptsFirstResponder;
- (BOOL) becomeFirstResponder;
- (BOOL) resignFirstResponder;

-(void)aboutMenu:(NSEvent*)theEvent;
-(void)quitMenu:(NSEvent*)theEvent;


@end
