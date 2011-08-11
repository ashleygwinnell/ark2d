//
//  WindowListener.h
//  test
//
//  Created by Ashley Gwinnell on 11/08/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface GameContainerMacWindowListener : NSResponder <NSWindowDelegate> {
    NSWindow* m_window;
}

-(void)init:(NSWindow* )window;

-(BOOL)windowShouldClose:(id)sender;
-(void)windowWillClose:(NSNotification *)notification;

-(void)keyDown:(NSEvent *)theEvent;
-(void)keyUp:(NSEvent *)theEvent;

-(void)mouseDown:(NSEvent *)theEvent;
-(void)mouseUp:(NSEvent *)theEvent;
-(void)mouseMoved:(NSEvent *)theEvent;

@end
