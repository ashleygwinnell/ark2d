/*
 * GameContainerMac.cpp
 *
 *  Created on: Aug 9, 2011
 *      Author: ashleygwinnell
 */

#include "GameContainer.h"
#include "GameContainerMac.h"

#ifdef ARK2D_MACINTOSH


	@implementation GameContainerPlatformListener
	- (void) doFunction
	{
		std::cout << "WINNING!" << std::endl;
	}
	@end
	
	
	@interface NSApplication(NSAppleMenu)
	- (void)setAppleMenu:(NSMenu *)menu;
	@end

	@interface ARK2DAppDelegate : NSObject
	- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender;
	@end

	@implementation ARK2DAppDelegate : NSObject
	- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
	{
		// SDL_SendQuit();
	   // WindowData* data = CocoaPlatform::instance->windowData;
	   // if (data->created) {
	   //     Cocoa_DestroyWindow(data);
	   // }
		// return NSTerminateCancel;
	    return NSTerminateNow;
	}
	@end
	
	void* GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
		return new string("");
	}

	GameContainer::GameContainer(Game& g, int width, int height, int bpp, bool fullscreen):
		m_timer(),
		m_game(g),
		m_input(),
		m_graphics(),
		m_gamepads(),
		m_width(width),
		m_height(height),
		m_bpp(bpp),
		m_fullscreen(fullscreen),
		m_resizable(false),
		m_scaleToWindow(true),
		m_clearColor(Color::black),
		m_platformSpecific()
		{

			ProcessSerialNumber psn;
			NSAutoreleasePool* pool;

			if (!GetCurrentProcess(&psn)) {
				TransformProcessType(&psn, kProcessTransformToForegroundApplication);
				SetFrontProcess(&psn);
			}
			pool = [[NSAutoreleasePool alloc] init];
			if (NSApp == nil) {
				[NSApplication sharedApplication];
				[NSApp finishLaunching];
			}
			if ([NSApp delegate] == nil) {
				[NSApp setDelegate:[[ARK2DAppDelegate alloc] init]];
			}
			
			//NSAutoreleasePool* pool;
			NSWindow* nswindow;
			NSRect rect;
			unsigned int style;
	
			
			rect.origin.x = 0;
			rect.origin.y = 0;
			rect.size.width = width;
			rect.size.height = height;
			rect.origin.y = CGDisplayPixelsHigh(kCGDirectMainDisplay) - rect.origin.y - rect.size.height;
	
			style = (NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask);
	
			//  Create window
			nswindow = [[NSWindow alloc] initWithContentRect:rect styleMask:style backing:NSBackingStoreBuffered defer:FALSE];
			[nswindow makeKeyAndOrderFront:nil];
	 
			//  Set content view
			NSView* contentView = [[NSView alloc] initWithFrame:rect];
			[nswindow setContentView: contentView];
			[contentView release];
	
			[nswindow setTitle:[NSString stringWithCString:"HelloWorld\0" encoding:NSUTF8StringEncoding]];
			[nswindow makeKeyAndOrderFront:nil];
			
			//todo: gl context
			
			[pool release];
			
			//return nswindow;
		 
	}

	void GameContainer::setFullscreen(bool fullscreen) {

	}

	void GameContainer::processGamepadInput() {

	}

	void GameContainer::setIcon(const std::string& path) {
		//m_platformSpecific.m_iconpath = path;
	}

	void GameContainer::initGamepads() {

	}

	void GameContainer::setCursorVisible(bool b) {

	}

	void GameContainer::resizeWindowToFitViewport() {

	}

	void GameContainer::start() {
		std::cout << "fill me too!" << std::endl;
	}

	void GameContainer::close() const {

	}


	void GameContainer::swapBuffers() {

	}

#endif
