/*
 * GameContainerMac.cpp
 *
 *  Created on: Aug 9, 2011 
 *      Author: ashleygwinnell 
 */
 
#include "../GameContainer.h"
#include "GameContainerMac.h"  
#include "../../Util/Log.h"   

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
			
			void* ARK::Core::GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
				return new string("");
			}
			
		
			ARK::Core::GameContainer::GameContainer(Game& g, int width, int height, int bpp, bool fullscreen):
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
				m_showingFPS(false),
				m_platformSpecific()
				{
					NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
					
					m_platformSpecific.m_container = this;
				
					m_input.setGameContainer(this);
		
					ARK2D::s_container = this;
					ARK2D::s_game = &m_game;
					ARK2D::s_graphics = &m_graphics;
					ARK2D::s_input = &m_input;
					ARK2D::s_log = new ARK::Util::Log();
				
					ProcessSerialNumber psn;
					if (!GetCurrentProcess(&psn)) {
						TransformProcessType(&psn, kProcessTransformToForegroundApplication);
						SetFrontProcess(&psn);
					} 
					
					
					// Get location of current app bundle and make sure there's a resources path.
					m_platformSpecific.m_resourcePath = [[[NSBundle mainBundle] resourcePath] fileSystemRepresentation];
					m_platformSpecific.m_resourcePath += "/data/";
					ARK2D::getLog()->i(StringUtil::append("Resource Path: ", m_platformSpecific.m_resourcePath));
					
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

					m_platformSpecific.m_listener = listener;
					
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
		
					// enable 2d!
					enable2D();
					
					[pool release];
					
					m_bRunning = true;
		
			} 
			
			void ARK::Core::GameContainer::resizeBehaviour(int width, int height) {
				if (m_resizeBehaviour == RESIZE_BEHAVIOUR_SCALE) 
			    {
			    	m_scaleX = (float) width / (float) m_originalWidth;
					m_scaleY = (float) height / (float) m_originalHeight;
					if (m_scaleX > m_scaleY) {
						m_scale = m_scaleY;
						m_scaleX = m_scaleY;
						m_scaleY = 1.0f;
					} else { // y > x
						m_scale = m_scaleX;
						m_scaleY = m_scaleX;
						m_scaleX = 1.0f;
					}
					m_width = width;
					m_height = height;

			   		
			    } else if (m_resizeBehaviour == RESIZE_BEHAVIOUR_NOSCALE) {
			    	m_width = width; 
			    	m_height = height;
			    	//ARK2D::s_game->resize(this, width, height);
			    	//ARK2D::getRenderer()->setScissorTestEnabled(false);
			    	//ARK2D::getRenderer()->scissor(0,0,width,height);
			    }
			    ARK2D::s_game->resize(this, width, height);
			}

			void ARK::Core::GameContainer::setSize(int width, int height) {

				if (width == (signed int) m_width && height == (signed int) m_height) { return; }

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
				

			
			    resizeBehaviour(width, height);

			    [[window contentView] setFrame: windowViewRect];
			    [window setFrame: windowFrame
			           display: YES
			           animate: YES];
			           
			    [m_platformSpecific.m_context update];
			}
		
			void ARK::Core::GameContainer::setFullscreen(bool fullscreen) {
				
				//if (m_fullscreen == fullscreen) { return; }
				
				NSRect testDisplayRect = [[NSScreen mainScreen] frame];
				if (fullscreen && 
					testDisplayRect.size.width == (signed int) m_width && 
					testDisplayRect.size.height == (signed int) m_height) 
					{ 
					return; 
				}

				if (fullscreen) {
					NSRect mainDisplayRect = [[NSScreen mainScreen] frame];
					
					NSWindow* window = m_platformSpecific.m_window;			
					[window setStyleMask:NSBorderlessWindowMask];

					 [[window contentView] setFrame: mainDisplayRect];
			   		 [window setFrame: mainDisplayRect
			           display: YES
			           animate: NO];

			   		[window setLevel:NSMainMenuWindowLevel+1];

			   		[window setOpaque:YES];
					[window setHidesOnDeactivate:YES];

					[window makeKeyAndOrderFront:nil];
			           
			        [m_platformSpecific.m_context update];
					m_fullscreen = fullscreen;

					resizeBehaviour((int) mainDisplayRect.size.width, (int) mainDisplayRect.size.height);

				} else {
					
					NSWindow* window = m_platformSpecific.m_window;	
					NSRect windowFrame = [window frame];

					//setSize();
					resizeBehaviour((int) m_originalWidth, (int) m_originalHeight);
					

		    		

					int width = (signed int) getWidth();
					int height = (signed int) getHeight();

					NSRect contentRect;

			    	contentRect = [NSWindow contentRectForFrameRect: windowFrame
											styleMask: NSTitledWindowMask];

					//int diffy = (windowFrame.size.height - contentRect.size.height);
			
					// need to shift origin (bottomleft) so window stays in same place
					NSSize desiredContentSize = NSMakeSize (width, height); 	
					windowFrame.origin.x += (windowFrame.size.width - desiredContentSize.width)/2;
				    windowFrame.origin.y += ((windowFrame.size.height - desiredContentSize.height)/2)+ 25;
				    windowFrame.size = desiredContentSize;
				    contentRect.size = desiredContentSize;
				
					// need to resize opengl NSView
					//NSView* windowView = (NSView*) [window contentView];
					//NSRect windowViewRect = [windowView frame];
					NSRect windowViewRect = [[window contentView] frame];
					windowViewRect.size = desiredContentSize;


				


				 	[[window contentView] setFrame: windowViewRect];


				


				     [window setFrame: windowFrame
				           display: YES
				           animate: NO];

				     unsigned int style = (NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask);
			    	[window setStyleMask:style];

			    	[window setTitle:[NSString stringWithCString:m_game.getTitle().c_str() encoding:NSUTF8StringEncoding]];

					/*NSRect mainDisplayRect;
					mainDisplayRect.origin.x = 0;
					mainDisplayRect.origin.y = 0;
					mainDisplayRect.size.width = width;
					mainDisplayRect.size.height = height;
					mainDisplayRect.origin.x = (CGDisplayPixelsWide(kCGDirectMainDisplay)/2) - (mainDisplayRect.size.width/2);
					mainDisplayRect.origin.y = (CGDisplayPixelsHigh(kCGDirectMainDisplay)/2)- (mainDisplayRect.size.height/2);
					
					

					NSSize desiredContentSize = NSMakeSize (width, height); 
					mainDisplayRect.size = desiredContentSize;

					NSRect contentRect;
					contentRect.origin.x = 0;
					contentRect.origin.y = 0;
					contentRect.size = desiredContentSize;
					//contentRect.size.width = 100;
					//contentRect.size.height = 100;
					[[window contentView] setFrame: contentRect];
			   		 // [m_platformSpecific.m_context setFrame: contentRect];

					[window setFrame: mainDisplayRect
			           display: YES
			           animate: NO];*/

			   		  [window setLevel:NSMainMenuWindowLevel-1];

			   		 [window setOpaque:NO];
					[window setHidesOnDeactivate:YES];

					[window makeKeyAndOrderFront:nil];
			           
			        [m_platformSpecific.m_context update];

					m_fullscreen = fullscreen;
					

					
					
					
				}


			    // if (m_resizeBehaviour == RESIZE_BEHAVIOUR_SCALE) {
			   	// 	ARK2D::s_game->resize(this, width, height);
			    // } else if (m_resizeBehaviour == RESIZE_BEHAVIOUR_NOSCALE) {
			    // 	m_width = width;
			    // 	m_height = height;
			    // 	ARK2D::s_game->resize(this, width, height);
			    // }
				

				return;
				
				// just some kinks to iron out.
				/*if (m_fullscreen == fullscreen) { return; }
			
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
				}*/
			
			}
		
			void ARK::Core::GameContainer::processGamepadInput() {
		
			}
		
			void ARK::Core::GameContainer::setIcon(const std::string& path) {
				//m_platformSpecific.m_iconpath = path;
			}
		
			void ARK::Core::GameContainer::initGamepads() {
		
			}
		
			void ARK::Core::GameContainer::setCursorVisible(bool b) {
		
			}
		
			void ARK::Core::GameContainer::resizeWindowToFitViewport() {
		
			}
		
			void ARK::Core::GameContainer::start() {
			
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
				ARK::Font::BMFont* fnt = ARK::Core::Resource::get("ark2d/fonts/default.fnt")->asFont()->asBMFont(); // BMFont("ark2d/fonts/default.fnt", "ark2d/fonts/default.png");
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

				if (m_fullscreen) {
					setFullscreen(true);
				}
				 
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
					if (m_showingFPS) { renderFPS(); }
					ARK2D::getLog()->render();
					
					swapBuffers();
					 
					usleep(delta * 500); // 0.017/2.
				}
				
				disableOpenAL();
				
			}
		
			void ARK::Core::GameContainer::close() const {
				exit(0);
			}
			
			NSOpenGLContext* ARK::Core::GameContainerPlatform::createGLContext() 
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
			
			void ARK::Core::GameContainerPlatform::makeContextCurrent(NSWindow* window, NSOpenGLContext* context) 
			{
			    if (context) {
					[context setView:[window contentView]];
					[context update];
					[context makeCurrentContext];
				} else {
			        [NSOpenGLContext clearCurrentContext];
			    }
			}
		
			void ARK::Core::GameContainerPlatform::deleteGLContext(NSOpenGLContext* context) 
			{
			    [context clearDrawable];
				[context release];
			}
			
			void ARK::Core::GameContainer::swapBuffers() {
				NSOpenGLContext* context = [NSOpenGLContext currentContext];
				if (context != nil) {
					[context flushBuffer];
				}
			}
			
			
			
			
			
			void ARK::Core::GameContainerPlatform::handleKeyEvent(NSEvent* event) {
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
			
			void ARK::Core::GameContainerPlatform::doEvents() {
				NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
				    
				//! @todo Update activity every 30 seconds to prevent screensaver
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
			
			
			void ARK::Core::GameContainerPlatform::setTitle(std::string title) {
				[m_window setTitle:[NSString stringWithCString:title.c_str() encoding:NSUTF8StringEncoding]];
			}
			
			string ARK::Core::GameContainerPlatform::getResourcePath() const {
				return m_resourcePath;
			}
			
			bool ARK::Core::GameContainerPlatform::initOpenAL() {
				
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
			
				//! @todo: check the context attributes, maybe something is useful:
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
			
				alListenerfv(AL_POSITION,    ARK::Audio::Sound::ListenerPos);
				alListenerfv(AL_VELOCITY,    ARK::Audio::Sound::ListenerVel);
				alListenerfv(AL_ORIENTATION, ARK::Audio::Sound::ListenerOri);
			
				if (alGetError() != AL_NO_ERROR) {
					ErrorDialog::createAndShow("Could not set OpenAL Listener");
					exit(0);
				}
			
				std::cout << "Initialised OpenAL" << std::endl;
				return true;
			}
			
			bool ARK::Core::GameContainerPlatform::deinitOpenAL() {
				// discard context and device
				return true;
			}
			 
			int ARK::Core::GameContainer::getGlobalMouseX() const {
				NSPoint mouseLoc = [NSEvent mouseLocation];
				return mouseLoc.x;
			}
			int ARK::Core::GameContainer::getGlobalMouseY() const {
				NSPoint mouseLoc = [NSEvent mouseLocation];
				return mouseLoc.y;
			}
			
	

#endif
