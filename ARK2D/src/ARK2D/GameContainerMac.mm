/*
 * GameContainerMac.cpp
 *
 *  Created on: Aug 9, 2011
 *      Author: ashleygwinnell
 */

#include "GameContainer.h"
#include "GameContainerMac.h"
#include "Util/ARKLog.h"

#ifdef ARK2D_MACINTOSH

	bool isLionPlus() {
		SInt32 MacVersionMajor;
		SInt32 MacVersionMinor;
		if (Gestalt(gestaltSystemVersionMajor, &MacVersionMajor) == noErr 
			&& Gestalt(gestaltSystemVersionMinor, &MacVersionMinor) == noErr
			) {
			if (MacVersionMajor >= 10 
				&& MacVersionMinor >= 7) {
				return true;
			} 
			return false;
		}
		return false;
	}
	
	void* GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
		return new string("");
	}
	

	GameContainer::GameContainer(Game& g, int width, int height, int bpp, bool fullscreen):
		m_timer(),
		m_game(g),
		m_input(),
		m_graphics(),
		m_gamepads(),
		m_originalWidth(width),
		m_originalHeight(height),
		m_width(width),
		m_height(height),
		m_screenWidth(0),
		m_screenHeight(0),
		m_scale(1.0f),
		m_scaleX(1.0f),
		m_scaleY(1.0f),
		m_translateX(0),
		m_translateY(0),
		m_bpp(bpp),
		m_fullscreen(fullscreen),
		m_resizable(false),
		m_scaleToWindow(true),
		m_touchMode(false),
		m_clearColor(Color::black),
		m_resizeBehaviour(RESIZE_BEHAVIOUR_SCALE),
		m_platformSpecific()
		{
			NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
			
			m_platformSpecific.m_container = this;
		
			m_input.setGameContainer(this);

			ARK2D::s_container = this;
			ARK2D::s_game = &m_game;
			ARK2D::s_graphics = &m_graphics;
			ARK2D::s_input = &m_input;
			ARK2D::s_log = new ARKLog();
		
			ProcessSerialNumber psn;
			if (!GetCurrentProcess(&psn)) {
				TransformProcessType(&psn, kProcessTransformToForegroundApplication);
				SetFrontProcess(&psn);
			} 
			
			
			// Get location of current app bundle and make sure there's a resources path.
			m_platformSpecific.m_resourcePath = [[[NSBundle mainBundle] resourcePath] fileSystemRepresentation];
			m_platformSpecific.m_resourcePath += "/data/";
			std::cout << "Resource path: " << m_platformSpecific.m_resourcePath << std::endl;
			
			if (NSApp == nil) {
				[NSApplication sharedApplication];
				[NSApp finishLaunching];
			}
			if ([NSApp delegate] == nil) {
				[NSApp setDelegate:[[GameContainerMacAppDelegate alloc] init]];
			}
			
			NSWindow* window = m_platformSpecific.m_window;
			NSRect rect;
			unsigned int style;
			
			rect.origin.x = 0;
			rect.origin.y = 0;
			rect.size.width = width;
			rect.size.height = height;
			rect.origin.x = (CGDisplayPixelsWide(kCGDirectMainDisplay)/2) - (rect.size.width/2);
			rect.origin.y = (CGDisplayPixelsHigh(kCGDirectMainDisplay)/2)- (rect.size.height/2) + 25;
			
			style = (NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask);
			
			//  Create window
			window = [[NSWindow alloc] initWithContentRect:rect styleMask:style backing:NSBackingStoreBuffered defer:FALSE];
			[window setTitle:[NSString stringWithCString:m_game.getTitle().c_str() encoding:NSUTF8StringEncoding]];
			[window makeKeyAndOrderFront:nil];
			[window orderFrontRegardless];
			[window makeKeyWindow];
		 
			[window setAcceptsMouseMovedEvents:YES];
			[window setReleasedWhenClosed:YES];

			GameContainerMacWindowListener* listener = [GameContainerMacWindowListener alloc];
			[listener init:window];
			[window setDelegate:listener];
			
			//if (isLionPlus()) {
			//	[window setRestorable:NO]; 
			//} 
			
			m_platformSpecific.m_window = window;
			
			NSOpenGLContext* context = m_platformSpecific.createGLContext();
			m_platformSpecific.makeContextCurrent(window, context);
			m_platformSpecific.m_context = context;
			
			//enable gl
			glEnable(GL_TEXTURE_2D);
			glViewport(0, 0, width, height);
			
			glClear( GL_COLOR_BUFFER_BIT );
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			
			// enable 2d
			glMatrixMode(GL_PROJECTION) ;
			glPushMatrix();
			glLoadIdentity();
			
			glOrtho(0, width, height, 0, -1, 1);
			
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			
			[pool release];
			
			m_bRunning = true;

	} 
	
	void GameContainer::setSize(int width, int height) {
		NSWindow* window = m_platformSpecific.m_window;
		NSRect windowFrame = [window frame];
		
		NSRect contentRect;
    	contentRect = [NSWindow contentRectForFrameRect: windowFrame
								styleMask: NSTitledWindowMask];
		int diffy = (windowFrame.size.height - contentRect.size.height);

		// need to shift origin (bottomleft) so window stays in same place
		NSSize desiredContentSize = NSMakeSize (width, height + diffy); 	
		windowFrame.origin.x += (windowFrame.size.width - desiredContentSize.width)/2;
	    windowFrame.origin.y += (windowFrame.size.height - desiredContentSize.height)/2;
	    windowFrame.size = desiredContentSize;
	    contentRect.size = desiredContentSize;
	
		// need to resize opengl NSView
		//NSView* windowView = (NSView*) [window contentView];
		//NSRect windowViewRect = [windowView frame];
		NSRect windowViewRect = [[window contentView] frame];
		windowViewRect.size = desiredContentSize;
		
		
	
	    if (m_resizeBehaviour == RESIZE_BEHAVIOUR_SCALE) {
	   		ARK2D::s_game->resize(this, width, height);
	    } else if (m_resizeBehaviour == RESIZE_BEHAVIOUR_NOSCALE) {
	    	m_width = width;
	    	m_height = height;
	    	ARK2D::s_game->resize(this, width, height);
	    }
	     [[window contentView] setFrame: windowViewRect];
	     [window setFrame: windowFrame
	           display: YES
	           animate: YES];
	           
	           [m_platformSpecific.m_context update];
	}

	void GameContainer::setFullscreen(bool fullscreen) {
		return;
		
		// just some kinks to iron out.
		if (m_fullscreen == fullscreen) { return; }
	
		if (isLionPlus()) { 
    		NSWindow* window = m_platformSpecific.m_window;
    		[window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
    		
    		[window toggleFullScreen:nil]; 
    		if (!m_fullscreen) {
    			[window setStyleMask:NSBorderlessWindowMask];
    		} else {
    			unsigned int style = (NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask);
    			[window setStyleMask:style];
    		}
    		m_fullscreen = !m_fullscreen;
		}
	
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
	
		// seed the random
		MathUtil::seedRandom();
		
		// Set window title
		//setTitle(m_game.getTitle());
		
		// populate the gamepads.
		ARK2D::getLog()->i("Initialising Gamepads... ");
		initGamepads();
		ARK2D::getLog()->i("done.");
		
		// initialise OpenGL -- this is done already
		glClearColor(m_clearColor.getRed()/255.0f, m_clearColor.getGreen()/255.0f, m_clearColor.getBlue()/255.0f, m_clearColor.getAlpha()/255.0f);
		
		// load default font.
		BMFont* fnt = ARK::Resource::get("ark2d/fonts/default.fnt")->asFont()->asBMFont(); // BMFont("ark2d/fonts/default.fnt", "ark2d/fonts/default.png");
		m_graphics.m_DefaultFont = fnt;
		m_graphics.m_Font = fnt;
		
		// initialise 
		enableOpenAL();
		
		// initialise game.
		ARK2D::getLog()->i("Initialising ");
		ARK2D::getLog()->i(m_game.getTitle());
		ARK2D::getLog()->i("...");
		m_game.init(this);

		ARK2D::getLog()->i("Initialised "); 
		ARK2D::getLog()->i(m_game.getTitle());
		ARK2D::getLog()->i("...");
		 
		while(m_bRunning) {
			m_timer.tick();
			m_platformSpecific.doEvents();
			
			processGamepadInput(); 
		   
			ARK2D::getLog()->update();
		   
			int delta = (int) (m_timer.getDelta() * 1000);
			m_game.preUpdate(this, &m_timer);
			m_game.update(this, &m_timer);
			m_game.postUpdate(this, &m_timer);
			m_input.clearKeyPressedRecord();
			for (unsigned int i = 0; i < m_gamepads.size(); i++) {
				m_gamepads.at(i)->clearButtonPressedRecord();
			}
			
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_game.preRender(this, &m_graphics);
			m_game.render(this, &m_graphics);
			m_game.postRender(this, &m_graphics);
			ARK2D::getLog()->render();
			
			swapBuffers();
			
			usleep(delta * 500); // 0.017/2.
		}
		
		disableOpenAL();
		
	}

	void GameContainer::close() const {
		exit(0);
	}
	
	NSOpenGLContext* GameContainerPlatform::createGLContext() 
	{
	    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	    
	    NSOpenGLPixelFormatAttribute attr[] =
	    {
			NSOpenGLPFADoubleBuffer,
			NSOpenGLPFAAccelerated,
			NSOpenGLPFAColorSize, 32,
			NSOpenGLPFADepthSize, 16,
	        NSOpenGLPFAScreenMask,
	        CGDisplayIDToOpenGLDisplayMask(CGMainDisplayID()),
			0 };
		NSOpenGLPixelFormat* format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attr];
		NSOpenGLContext* glContext = [[NSOpenGLContext alloc] initWithFormat: format shareContext: nil];
		//m_context = glContext;
		//[glContext makeCurrentContext];
	    
	    [pool release];
	    
	    return glContext;  
	}
	
	void GameContainerPlatform::makeContextCurrent(NSWindow* window, NSOpenGLContext* context) 
	{
	    if (context) {
			[context setView:[window contentView]];
			[context update];
			[context makeCurrentContext];
		} else {
	        [NSOpenGLContext clearCurrentContext];
	    }
	}

	void GameContainerPlatform::deleteGLContext(NSOpenGLContext* context) 
	{
	    [context clearDrawable];
		[context release];
	}
	
	void GameContainer::swapBuffers() {
		NSOpenGLContext* context = [NSOpenGLContext currentContext];
		if (context != nil) {
			[context flushBuffer];
		}
	}
	
	
	
	
	
	void GameContainerPlatform::handleKeyEvent(NSEvent* event) {
		NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
		
		unsigned short scancode = [event keyCode];
		unsigned int key;
		
		if (scancode < 128) {
			key = 0; //darwin_scancode_table[scancode];
		}
		else {
			//Debug::Log("Unknown scancode");
		}
		
		switch ([event type]) {
			case NSKeyDown:
				// if (![event isARepeat]) {
				//     /* See if we need to rebuild the keyboard layout */
				//     UpdateKeymap(data);
				// }
				
				//CocoaPlatform::platform->SetLocalKey(key, true);
				break;
			case NSKeyUp:
			   // CocoaPlatform::platform->SetLocalKey(key, false);
				break;
				// case NSFlagsChanged:
				//     HandleModifiers(_this, scancode, [event modifierFlags]);
				//     break;
			default: /* just to avoid compiler warnings */
				break;
		}
		
		[pool release];
	}
	
	void GameContainerPlatform::doEvents() {
		NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
		    
		/* TODO Update activity every 30 seconds to prevent screensaver */
		for ( ; ; ) {
			NSEvent *event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES ];
			if ( event == nil ) {
				break;
			}
			
			switch ([event type]) {
				/*case NSKeyDown: 
				case NSKeyUp:
					//			case NSFlagsChanged:
					// XXX handle key event
					handleKeyEvent(event);
					
					//Fall through to pass event to NSApp; er, nevermind... 
					
					// Add to support system-wide keyboard shortcuts like CMD+Space
					if (([event modifierFlags] & NSCommandKeyMask) || [event type] == NSFlagsChanged)
						[NSApp sendEvent: event];
					break;*/
				default:
					[NSApp sendEvent:event];
					break;
			}
		}
		[pool release];
	}
	
	
	void GameContainerPlatform::setTitle(std::string title) {
		[m_window setTitle:[NSString stringWithCString:title.c_str() encoding:NSUTF8StringEncoding]];
	}
	
	string GameContainerPlatform::getResourcePath() const {
		return m_resourcePath;
	}
	
	bool GameContainerPlatform::initOpenAL() {
		
		if (alcGetCurrentContext() != NULL) {
			ErrorDialog::createAndShow("OpenAL is already initialised. Exiting program.");
			exit(0);
		}
	
		// Load OpenAL.
		ALCdevice* dev = NULL;
	
		dev = alcOpenDevice(NULL);
		if(dev == NULL) {
			ErrorDialog::createAndShow("Could not open Audio Device.");
			exit(0);
		} else if (alcGetError(dev) != ALC_NO_ERROR) {
			ErrorDialog::createAndShow("Could not open Audio Device.");
			exit(0);
		}
	
		// TODO: check the context attributes, maybe something is useful:
		// http://www.openal.org/openal_webstf/specs/oal11spec_html/oal11spec6.html
		// 6.2.1. Context Attributes
		// my bet is on ALC_STEREO_SOURCES
		ALCcontext* ctx = NULL;
		ctx = alcCreateContext(dev, NULL);
		if(ctx == NULL) {
			ErrorDialog::createAndShow("Could not create Audio Context.");
			return false;
		} else if (alcGetError(dev) != ALC_NO_ERROR) {
			ErrorDialog::createAndShow("Could not create Audio Context.");
			return false;
		}
	
		ALboolean b = alcMakeContextCurrent(ctx);
		if (b != ALC_TRUE) {
			ErrorDialog::createAndShow("Could not make Audio Context current.");
			exit(0);
		}
	
		if (alcGetError(dev) != ALC_NO_ERROR) {
			ErrorDialog::createAndShow("Problem with Audio Device.");
			exit(0);
		}
	
		//alcProcessContext(ctx);
	
		alListenerfv(AL_POSITION,    Sound::ListenerPos);
		alListenerfv(AL_VELOCITY,    Sound::ListenerVel);
		alListenerfv(AL_ORIENTATION, Sound::ListenerOri);
	
		if (alGetError() != AL_NO_ERROR) {
			ErrorDialog::createAndShow("Could not set OpenAL Listener");
			exit(0);
		}
	
		std::cout << "Initialised OpenAL" << std::endl;
		return true;
	}
	
	bool GameContainerPlatform::deinitOpenAL() {
		// discard context and device
		return true;
	}
	
	int GameContainer::getGlobalMouseX() const {
		NSPoint mouseLoc = [NSEvent mouseLocation];
		return mouseLoc.x;
	}
	int GameContainer::getGlobalMouseY() const {
		NSPoint mouseLoc = [NSEvent mouseLocation];
		return mouseLoc.y;
	}

#endif
