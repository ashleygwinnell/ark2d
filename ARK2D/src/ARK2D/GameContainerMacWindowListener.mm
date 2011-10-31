//
//  WindowListener.m
//  test
//
//  Created by Ashley Gwinnell on 11/08/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "GameContainerMacWindowListener.h"
#include "ARK2D.h" 
#include "Input.h"
#include "Game.h"
#include "GameContainer.h"

// see url:
// http://classicteck.com/rbarticles/mackeyboard.php
// there is a nice image and table

static int darwin_scancode_table[] = {
	/*   0 */   Input::KEY_A,
	/*   1 */   Input::KEY_S,
	/*   2 */   Input::KEY_D,
	/*   3 */   Input::KEY_F,
	/*   4 */   Input::KEY_H,
	/*   5 */   Input::KEY_G,
	/*   6 */   Input::KEY_Z,
	/*   7 */   Input::KEY_X,
	/*   8 */   Input::KEY_C,
	/*   9 */   Input::KEY_V,
	/*  10 */   0, // KEY_UNDEFINED, // KEY_NONUSBACKSLASH, /* KEY_NONUSBACKSLASH on ANSI and JIS keyboards (if this key would exist there), KEY_GRAVE on ISO. (The USB keyboard driver actually translates these usage codes to different virtual key codes depending on whether the keyboard is ISO/ANSI/JIS. That's why you have to help it identify the keyboard type when you plug in a PC USB keyboard. It's a historical thing - ADB keyboards are wired this way.) */
	/*  11 */   Input::KEY_B,
	/*  12 */   Input::KEY_Q,
	/*  13 */   Input::KEY_W,
	/*  14 */   Input::KEY_E,
	/*  15 */   Input::KEY_R,
	/*  16 */   Input::KEY_Y,
	/*  17 */   Input::KEY_T,
	/*  18 */   Input::KEY_1,
	/*  19 */   Input::KEY_2,
	/*  20 */   Input::KEY_3,
	/*  21 */   Input::KEY_4,
	/*  22 */   Input::KEY_6,
	/*  23 */   Input::KEY_5,
	/*  24 */   Input::KEY_EQUALS,
	/*  25 */   Input::KEY_9,
	/*  26 */   Input::KEY_7,
	/*  27 */   Input::KEY_HYPHEN, // 0, // Input::KEY_MINUS,
	/*  28 */   Input::KEY_8,
	/*  29 */   Input::KEY_0,
	/*  30 */   0, // Input::KEY_RIGHTBRACKET,
	/*  31 */   Input::KEY_O,
	/*  32 */   Input::KEY_U,
	/*  33 */   0, // Input::KEY_LEFTBRACKET,
	/*  34 */   Input::KEY_I,
	/*  35 */   Input::KEY_P,
	/*  36 */   Input::KEY_ENTER, // Input::KEY_RETURN,
	/*  37 */   Input::KEY_L,
	/*  38 */   Input::KEY_J,
	/*  39 */   Input::KEY_APOSTROPHE, // 0, // Input::KEY_UNDEFINED,
	/*  40 */   Input::KEY_K,
	/*  41 */   Input::KEY_SEMICOLON,
	/*  42 */   Input::KEY_BACK_SLASH,
	/*  43 */   Input::KEY_COMMA,
	/*  44 */   Input::KEY_FORWARD_SLASH,
	/*  45 */   Input::KEY_N,
	/*  46 */   Input::KEY_M,
	/*  47 */   Input::KEY_PERIOD,
	/*  48 */   Input::KEY_TAB,
	/*  49 */   Input::KEY_SPACE,
	/*  50 */   0, // KEY_UNDEFINED, // KEY_GRAVE, /* KEY_GRAVE on ANSI and JIS keyboards, KEY_NONUSBACKSLASH on ISO (see comment about virtual key code 10 above) */
	/*  51 */   Input::KEY_BACKSPACE, // KEY_UNDEFINED, // KEY_BACKSPACE,
	/*  52 */   0, // KEY_UNDEFINED, // KEY_KP_ENTER, /* keyboard enter on portables */
	/*  53 */   Input::KEY_ESCAPE,
	/*  54 */   0, // KEY_UNDEFINED, // KEY_RGUI,
	/*  55 */   0, // KEY_UNDEFINED, // KEY_LGUI,
	/*  56 */   Input::KEY_LSHIFT,
	/*  57 */   Input::KEY_CAPSLOCK,
	/*  58 */   0, // Input::KEY_LALT,
	/*  59 */   Input::KEY_LCONTROL,
	/*  60 */   Input::KEY_RSHIFT,
	/*  61 */   0, // Input::KEY_RALT,
	/*  62 */   Input::KEY_RCONTROL,
	/*  63 */   0, // KEY_UNDEFINED, // KEY_RGUI, /* fn on portables, acts as a hardware-level modifier already, so we don't generate events for it, also XK_Meta_R */
	/*  64 */   0, // KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  65 */   Input::KEY_NUMPAD_DECIMAL,
	/*  66 */   0, // KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  67 */   Input::KEY_NUMPAD_MULTIPLY,
	/*  68 */   0, // KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  69 */   Input::KEY_NUMPAD_ADD,
	/*  70 */   0, // KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  71 */   0, // KEY_UNDEFINED, // KEY_NUMLOCKCLEAR,
	/*  72 */   0, // KEY_UNDEFINED, // KEY_VOLUMEUP,
	/*  73 */   0, // KEY_UNDEFINED, // KEY_VOLUMEDOWN,
	/*  74 */   0, // KEY_UNDEFINED, // KEY_MUTE,
	/*  75 */   Input::KEY_NUMPAD_DIVIDE,
	/*  76 */   0, // KEY_UNDEFINED, // KEY_KP_ENTER, /* keypad enter on external keyboards, fn-return on portables */
	/*  77 */   0, // KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  78 */   Input::KEY_NUMPAD_SUBTRACT,
	/*  79 */   0, // KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  80 */   0, // KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  81 */   0, // KEY_UNDEFINED, // KEY_KP_EQUALS,
	/*  82 */   0, // KEY_UNDEFINED, // KEY_KP_0,
	/*  83 */   0, // KEY_UNDEFINED, // KEY_KP_1,
	/*  84 */   0, // KEY_UNDEFINED, // KEY_KP_2,
	/*  85 */   0, // KEY_UNDEFINED, // KEY_KP_3,
	/*  86 */   0, // KEY_UNDEFINED, // KEY_KP_4,
	/*  87 */   0, // KEY_UNDEFINED, // KEY_KP_5,
	/*  88 */   0, // KEY_UNDEFINED, // KEY_KP_6,
	/*  89 */   0, // KEY_UNDEFINED, // KEY_KP_7,
	/*  90 */   0, // KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/*  91 */   0, // KEY_UNDEFINED, // KEY_KP_8,
	/*  92 */   0, // KEY_UNDEFINED, // KEY_KP_9,
	/*  93 */   0, // KEY_UNDEFINED, // KEY_INTERNATIONAL3, /* Cosmo_USB2ADB.c says "Yen (JIS)" */
	/*  94 */   0, // KEY_UNDEFINED, // KEY_INTERNATIONAL1, /* Cosmo_USB2ADB.c says "Ro (JIS)" */
	/*  95 */   0, // KEY_UNDEFINED, // KEY_KP_COMMA, /* Cosmo_USB2ADB.c says ", JIS only" */
	/*  96 */   Input::KEY_F5,
	/*  97 */   Input::KEY_F6,
	/*  98 */   Input::KEY_F7,
	/*  99 */   Input::KEY_F3,
	/* 100 */   Input::KEY_F8,
	/* 101 */   Input::KEY_F9,
	/* 102 */   0, // KEY_UNDEFINED, // KEY_LANG2, /* Cosmo_USB2ADB.c says "Eisu" */
	/* 103 */   Input::KEY_F11,
	/* 104 */   0, // KEY_UNDEFINED, // KEY_LANG1, /* Cosmo_USB2ADB.c says "Kana" */
	/* 105 */   0, // KEY_UNDEFINED, // KEY_PRINTSCREEN, /* On ADB keyboards, this key is labeled "F13/print screen". Problem: USB has different usage codes for these two functions. On Apple USB keyboards, the key is labeled "F13" and sends the F13 usage code (KEY_F13). I decided to use KEY_PRINTSCREEN here nevertheless since SDL applications are more likely to assume the presence of a print screen key than an F13 key. */
	/* 106 */   0, // KEY_UNDEFINED, // KEY_F16,
	/* 107 */   0, // KEY_UNDEFINED, // KEY_SCROLLLOCK, /* F14/scroll lock, see comment about F13/print screen above */
	/* 108 */   0, // KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/* 109 */   Input::KEY_F10, // KEY_UNDEFINED, // KEY_F10,
	/* 110 */   0, // KEY_UNDEFINED, // KEY_APPLICATION, /* windows contextual menu key, fn-enter on portables */
	/* 111 */   Input::KEY_F12,
	/* 112 */   0, // KEY_UNDEFINED, // KEY_UNKNOWN, /* unknown (unused?) */
	/* 113 */   0, // Input::KEY_PAUSE, /* F15/pause, see comment about F13/print screen above */
	/* 114 */   Input::KEY_INSERT, /* the key is actually labeled "help" on Apple keyboards, and works as such in Mac OS, but it sends the "insert" usage code even on Apple USB keyboards */
	/* 115 */   Input::KEY_HOME,
	/* 116 */   Input::KEY_PAGEUP,
	/* 117 */   Input::KEY_DELETE,
	/* 118 */   Input::KEY_F4,
	/* 119 */   Input::KEY_END,
	/* 120 */   Input::KEY_F2,
	/* 121 */   Input::KEY_PAGEDOWN,
	/* 122 */   Input::KEY_F1,
	/* 123 */   Input::KEY_LEFT,
	/* 124 */   Input::KEY_RIGHT,
	/* 125 */   Input::KEY_DOWN,
	/* 126 */   Input::KEY_UP,
	/* 127 */   0, // KEY_UNDEFINED, // KEY_POWER
};

@implementation GameContainerMacWindowListener 

-(void) init:(NSWindow* )window {
   	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
   
	    NSNotificationCenter* center;
	    // NSView* view = [window contentView];
	    center = [NSNotificationCenter defaultCenter];
	    if ([window delegate] != nil) {
	        [center addObserver:self selector:@selector(windowDidExpose:) name:NSWindowDidExposeNotification object:window];
	    } else {
	        [window setDelegate:self];
	    }
	    
	    [window setNextResponder:self];
	      //  [view setNextResponder:self];
	    
	    m_window = window;
	
	[pool release];
}

- (BOOL)windowShouldClose:(id)sender
{
    //Monocle::Game::Quit();
	//Cocoa_DestroyWindow(_data);
    printf("closed windowlsitener\r\n");
    [m_window close];
    exit(0);
    //[NSApp close:sender];
   // [NSApp close];
   // [m_window release];
	return NO;
}
-(void)windowWillClose:(NSNotification *)notification {
    // printf("window delegate window will close now.\r");
    // [m_window close];
    //  [NSApp terminate];
}

-(void)keyDown:(NSEvent *)theEvent {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
  	unsigned short scancode = [theEvent keyCode];
    std::cout << "key pressed: " << scancode << std::endl;
    if (scancode < 128) {
    	unsigned int key = darwin_scancode_table[scancode];
    	ARK2D::getInput()->pressKey(key);
    }
    
    [pool release];
}
-(void)keyUp:(NSEvent *)theEvent {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    unsigned short scancode = [theEvent keyCode];
    std::cout << "key released: " << scancode << std::endl;
    if (scancode < 128) {
    	unsigned int key = darwin_scancode_table[scancode];
    	ARK2D::getInput()->releaseKey(key);
    }
    [pool release];
}

-(void)flagsChanged:(NSEvent *)theEvent {
	int flags = [theEvent modifierFlags];

	// opt = (flags & NSAlternateKeyMask) ? YES : NO;
	// ctrl = (flags & NSControlKeyMask) ? YES : NO;
	// command = (flags & NSCommandKeyMask) ? YES : NO;
	
	if (flags & NSShiftKeyMask) {
		ARK2D::getInput()->pressKey(darwin_scancode_table[56]); // lshift
	} else {
		ARK2D::getInput()->releaseKey(darwin_scancode_table[56]); // rshift
	}
	
	// shift = ( flags & NSShiftKeyMask ) ? YES : NO;
}

-(void)mouseDown:(NSEvent *)theEvent {
  	std::cout << "left mouse down" << std::endl;
    ARK2D::getInput()->pressKey(Input::MOUSE_BUTTON_LEFT);
}
-(void)mouseUp:(NSEvent *)theEvent {
    std::cout << "left mouse up" << std::endl;
    ARK2D::getInput()->releaseKey(Input::MOUSE_BUTTON_LEFT);
}
-(void)mouseMoved:(NSEvent *)theEvent {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	NSPoint v = [theEvent locationInWindow];
	//NSPoint v = [NSView convertPoint:[NSEvent mouseLocation] fromView:nil];
	
	int ch = ARK2D::getContainer()->getHeight();
	if (v.y > ch/2) { 
		int diff = v.y - (ch/2);
		v.y -= 2 * diff;
	} else {
		int diff = (ch/2) - v.y;
		v.y += 2 * diff;
	}
	
	Input* i = ARK2D::getInput();
	ARK2D::getGame()->mouseMoved((int) v.x, (int) v.y, i->mouse_x, i->mouse_y);

	i->mouse_x = (int) v.x;
	i->mouse_y = (int) v.y;
	
	[pool release];
}
-(void)mouseDragged:(NSEvent* )theEvent {
	[self mouseMoved:theEvent];
}




@end