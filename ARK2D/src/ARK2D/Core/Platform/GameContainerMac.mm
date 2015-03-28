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
 
 	// OpenGL Capabilities
 	// https://developer.apple.com/opengl/capabilities/ 

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
			
			void ARK::Core::GameContainerPlatform::setBorderlessWindow(bool b) {
				unsigned int style = 0; 
				if (!b) { 
					style = (NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask);
				} else {
					style = (NSBorderlessWindowMask);
				}
				[m_window setStyleMask:style];
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
				m_scaleLock(false),
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
				m_screenOrientationPrevious(ORIENTATION_DUMMY),
				m_orientationInverted(false),
				m_2in1enabled(false),
				m_clearColor(Color::black),
				m_resizeBehaviour(RESIZE_BEHAVIOUR_SCALE),
				m_showingFPS(false),
				m_willLoadDefaultFont(true),
				m_platformSpecific()
				{
					NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
					
					m_platformSpecific.m_container = this;
				
					m_input.setGameContainer(this);
		
					ARK2D::s_container = this;
					ARK2D::s_game = &m_game;
					ARK2D::s_graphics = &m_graphics;
					ARK2D::s_input = &m_input;
					ARK2D::s_log = ARK::Util::Log::getInstance();

					ARK2D::getRenderer()->preinit();
				
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
						//[NSApp setDelegate:[[GameContainerMacAppDelegate alloc] init]];
						
					}

					NSRect screenRect = [[NSScreen mainScreen] frame];
					m_screenWidth = screenRect.size.width;
					m_screenHeight = screenRect.size.height;
					
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
					
					Image::showAnyGlErrorAndExit();

					// maybe it should be on.. i don't know...
					//glEnable(GL_MULTISAMPLE);

					//enable gl
					ARK2D::getLog()->v("enable texture 2d and do viewport");
					//glEnable(GL_TEXTURE_2D);
					glViewport(0, 0, width, height);

					Image::showAnyGlErrorAndExit();
					
					ARK2D::getLog()->v("clearing and blending");
					glClear( GL_COLOR_BUFFER_BIT );
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

					Image::showAnyGlErrorAndExit();
		 
					// enable 2d!
					enable2D();
					
					[pool release];

					ARK2D::getRenderer()->init();

					// gamepads
					m_platformSpecific.m_hidManager = NULL;
					
					m_bRunning = true;
		
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
				NSView* windowView = (NSView*) [window contentView];
				NSRect windowViewRect = [windowView frame];
				//NSRect windowViewRect = NSRectFromCGRect( [[window contentView] frame] );
				windowViewRect.size = desiredContentSize;
				

			
			    resizeBehaviour(width, height);

			    [[window contentView] setFrame: windowViewRect ];
				//[[window contentView] setFrame: NSRectToCGRect(windowViewRect) ];
			    [window setFrame: windowFrame
			           display: YES
			           animate: YES];
			           
			    [m_platformSpecific.m_context update];
			}


		
			void ARK::Core::GameContainer::setFullscreen(bool fullscreen) {
				
				if (m_fullscreen == fullscreen) { return; }

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

					[[window contentView] setFrame: mainDisplayRect ];
//					[[window contentView] setFrame: NSRectToCGRect( mainDisplayRect) ];
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
					NSView* windowView = (NSView*) [window contentView];
					NSRect windowViewRect = [windowView frame];
					//NSRect windowViewRect = NSRectFromCGRect( [[window contentView] frame] );
					windowViewRect.size = desiredContentSize;


				


				 	[[window contentView] setFrame: windowViewRect ];
					//[[window contentView] setFrame: NSRectToCGRect( windowViewRect) ];


				


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
				
				ARK2D::getTimer()->flush();
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
				for( unsigned int i = 0; i < m_gamepads.size(); ++i) {
					m_gamepads.at(i)->update();
				}
			}
		
			void ARK::Core::GameContainer::setIcon(const std::string& path) {
				//m_platformSpecific.m_iconpath = path;
			}

			static void hatValueToXY(CFIndex value, CFIndex range, int* outX, int* outY) {
				if (value == range) {
					*outX = *outY = 0;
					 
				} else {
					if (value > 0 && value < range / 2) {
						*outX = 1;
						
					} else if (value > range / 2) {
						*outX = -1;
						
					} else {
						*outX = 0;
					}
					
					if (value > range / 4 * 3 || value < range / 4) {
						*outY = -1;
						
					} else if (value > range / 4 && value < range / 4 * 3) {
						*outY = 1;
						
					} else {
						*outY = 0;
					}
				}
			}

			static void onDeviceValueChanged(void* context, IOReturn result, void * sender, IOHIDValueRef value) {
				
				Gamepad* gamepad;
				gamepad = (Gamepad*) context;

				//struct Gamepad_device * deviceRecord;
				//struct Gamepad_devicePrivate * hidDeviceRecord;
				IOHIDElementRef element;
				IOHIDElementCookie cookie;
				
				//static mach_timebase_info_data_t timebaseInfo;
				//if (timebaseInfo.denom == 0) {
				//	mach_timebase_info(&timebaseInfo);
				//}
				
				//deviceRecord = context;
				//hidDeviceRecord = deviceRecord->privateData;
				element = IOHIDValueGetElement(value);
				cookie = IOHIDElementGetCookie(element);
				
				for (unsigned int axisIndex = 0; axisIndex < gamepad->numAxes; axisIndex++) 
				{
					GamepadAxis* axis = gamepad->axes.at(axisIndex);
					if (!axis->isHatSwitchSecondAxis && axis->cookie == cookie) 
					{
						CFIndex integerValue;
						
						if (IOHIDValueGetLength(value) > 4) {
							// Workaround for a strange crash that occurs with PS3 controller; was getting lengths of 39 (!)
							continue;
						}
						integerValue = IOHIDValueGetIntegerValue(value);
						 
						if (axis->isHatSwitch) {
							int x, y;
							
							// Fix for Saitek X52 -- ???
							axis->hasNullState = false;
							if (!axis->hasNullState) {
								if (integerValue < axis->logicalMin) {
									integerValue = axis->logicalMax - axis->logicalMin + 1;
								} else {
									integerValue--;
								}
							}
							
							hatValueToXY(integerValue, axis->logicalMax - axis->logicalMin + 1, &x, &y);
							
							if (x != axis->value) {
								//queueAxisEvent(deviceRecord, IOHIDValueGetTimeStamp(value) * timebaseInfo.numer / timebaseInfo.denom * 0.000000001, axisIndex, x);
								axis->value = x;
							}
							
							if (y != axis->nextHatSwitch->value) {
								//queueAxisEvent(deviceRecord, IOHIDValueGetTimeStamp(value) * timebaseInfo.numer / timebaseInfo.denom * 0.000000001, axisIndex + 1, y);
								axis->nextHatSwitch->value = y;
							}
							
						} else {
							
							
							if (integerValue < axis->logicalMin) {
								axis->logicalMin = integerValue;
							}
							if (integerValue > axis->logicalMax) {
								axis->logicalMax = integerValue;
							}

							float floatValue = (integerValue - axis->logicalMin) / (float) (axis->logicalMax - axis->logicalMin) * 2.0f - 1.0f;
							//queueAxisEvent(deviceRecord, IOHIDValueGetTimeStamp(value) * timebaseInfo.numer / timebaseInfo.denom * 0.000000001, axisIndex, floatValue);
							axis->value = floatValue;
						}
						
						return;
					} 
				}
				
				for (unsigned int buttonIndex = 0; buttonIndex < gamepad->buttons.size(); buttonIndex++) {
					GamepadButton* button = gamepad->buttons.at(buttonIndex);
					if (button->cookie == cookie) {
						bool down = IOHIDValueGetIntegerValue(value);
						/*queueButtonEvent(deviceRecord,
							IOHIDValueGetTimeStamp(value) * timebaseInfo.numer / timebaseInfo.denom * 0.000000001,
							buttonIndex,
							down);*/ 
						//deviceRecord->buttonStates[buttonIndex] = down;
						unsigned int newbutton = gamepad->convertCookieToButton( (int) button->cookie );
						//ARK2D::getLog()->v(StringUtil::append("button press id: ", (int) button->id));
						//ARK2D::getLog()->v(StringUtil::append("button press cookie: ", (int) button->cookie));
						//ARK2D::getLog()->v(StringUtil::append("button press arkid: ", (int) newbutton));

						
						if (down) {
							gamepad->pressButton(newbutton); 
						} else {
							gamepad->releaseButton(newbutton);
						}
						
						return;
					}
				}
			}


			//static void disposeDevice(struct Gamepad_device * deviceRecord) {
			static void disposeDevice(Gamepad* pad) {
				
				IOHIDDeviceRegisterInputValueCallback(pad->deviceRef, NULL, NULL);
				delete pad;
				
				
				// for (unsigned int inputEventIndex = 0; inputEventIndex < inputEventCount; inputEventIndex++) {
				// 	if (inputEventQueue[inputEventIndex].dispatcher == deviceRecord->eventDispatcher) {
				// 		unsigned int inputEventIndex2;
				
				// 		free(inputEventQueue[inputEventIndex].eventData);
				// 		inputEventCount--;
				// 		for (inputEventIndex2 = inputEventIndex; inputEventIndex2 < inputEventCount; inputEventIndex2++) {
				// 			inputEventQueue[inputEventIndex2] = inputEventQueue[inputEventIndex2 + 1];
				// 		}
				// 		inputEventIndex--;
				// 	}
				// }
				
				// for (unsigned int deviceEventIndex = 0; deviceEventIndex < deviceEventCount; deviceEventIndex++) {
				// 	if (deviceEventQueue[deviceEventIndex].dispatcher == deviceRecord->eventDispatcher) {
				// 		unsigned int deviceEventIndex2;
				
				// 		deviceEventCount--;
				// 		for (deviceEventIndex2 = deviceEventIndex; deviceEventIndex2 < deviceEventCount; deviceEventIndex2++) {
				// 			deviceEventQueue[deviceEventIndex2] = deviceEventQueue[deviceEventIndex2 + 1];
				// 		}
				// 		deviceEventIndex--;
				// 	}
				// }
				
				//deviceRecord->eventDispatcher->dispose(deviceRecord->eventDispatcher);
				
				
			}

			static int IOHIDDeviceGetIntProperty(IOHIDDeviceRef device, CFStringRef key)
			{
				CFTypeRef ref;
				int32_t value;
				
				ref = IOHIDDeviceGetProperty(device, key);
				if (ref) {
					if (CFGetTypeID(ref) == CFNumberGetTypeID()) {
						CFNumberGetValue((CFNumberRef) ref, kCFNumberSInt32Type, &value);
						return value;
					}
				}
				return 0;
			}

			static int IOHIDDeviceGetVendorID(IOHIDDeviceRef deviceRef) {
				return IOHIDDeviceGetIntProperty(deviceRef, CFSTR(kIOHIDVendorIDKey));
			}

			static int IOHIDDeviceGetProductID(IOHIDDeviceRef deviceRef) {
				return IOHIDDeviceGetIntProperty(deviceRef, CFSTR(kIOHIDProductIDKey));
			}
			

			static void onDeviceMatched(void * context, IOReturn result, void * sender, IOHIDDeviceRef device) {
				
				GameContainerPlatform* platform = &ARK2D::getContainer()->m_platformSpecific;
				//struct Gamepad_device * deviceRecord; 
				//struct Gamepad_devicePrivate * hidDeviceRecord;
				//struct Gamepad_queuedEvent queuedEvent;
				unsigned int gamepadId = Gamepad::getNextGamepadId();
				ARK2D::getLog()->i(StringUtil::append("Initialising Gamepad: ", gamepadId));

				Gamepad* gamepad = new Gamepad();
				gamepad->id = gamepadId; //platform->m_gamepadId++;
				gamepad->initted = true;
				gamepad->deviceRef = device;
				gamepad->vendorId = IOHIDDeviceGetVendorID(device);
				gamepad->productId = IOHIDDeviceGetProductID(device);

				string thisName = Gamepad::getGamepadDescription(device);
				thisName += "\0"; 
				ARK2D::getLog()->i(StringUtil::append("Gamepad Name: ", thisName));
				gamepad->name = string(thisName);
 
				gamepad->numButtons = 0;
				gamepad->numAxes = 0;

				bool isPS3Controller = StringUtil::str_contains(gamepad->name, "PLAYSTATION(R)3");
				if (isPS3Controller) {
					ARK2D::getLog()->v("PS3 controller, yup!");
				}
				
				{
					
					CFArrayRef elements;
					CFIndex elementIndex;
					IOHIDElementRef element;
					IOHIDElementType type;
					
					elements = IOHIDDeviceCopyMatchingElements(device, NULL, kIOHIDOptionsTypeNone);
					for (elementIndex = 0; elementIndex < CFArrayGetCount(elements); elementIndex++) {
						element = (IOHIDElementRef) CFArrayGetValueAtIndex(elements, elementIndex);
						type = IOHIDElementGetType(element);
						
						// All of the axis elements I've ever detected have been kIOHIDElementTypeInput_Misc. kIOHIDElementTypeInput_Axis is only included for good faith...
						if (type == kIOHIDElementTypeInput_Misc || type == kIOHIDElementTypeInput_Axis) 
						{
							/*hidDeviceRecord->axisElements = realloc(hidDeviceRecord->axisElements, sizeof(struct HIDGamepadAxis) * (deviceRecord->numAxes + 1));
							hidDeviceRecord->axisElements[deviceRecord->numAxes].cookie = IOHIDElementGetCookie(element);
							hidDeviceRecord->axisElements[deviceRecord->numAxes].logicalMin = IOHIDElementGetLogicalMin(element);
							hidDeviceRecord->axisElements[deviceRecord->numAxes].logicalMax = IOHIDElementGetLogicalMax(element);
							hidDeviceRecord->axisElements[deviceRecord->numAxes].hasNullState = !!IOHIDElementHasNullState(element);
							hidDeviceRecord->axisElements[deviceRecord->numAxes].isHatSwitch = IOHIDElementGetUsage(element) == kHIDUsage_GD_Hatswitch;
							hidDeviceRecord->axisElements[deviceRecord->numAxes].isHatSwitchSecondAxis = false;
							deviceRecord->numAxes++;
							
							if (hidDeviceRecord->axisElements[deviceRecord->numAxes - 1].isHatSwitch) {
								hidDeviceRecord->axisElements = realloc(hidDeviceRecord->axisElements, sizeof(struct HIDGamepadAxis) * (deviceRecord->numAxes + 1));
								hidDeviceRecord->axisElements[deviceRecord->numAxes].isHatSwitchSecondAxis = true;
								deviceRecord->numAxes++;
							}*/

							GamepadAxis* axis = new GamepadAxis();
							axis->id = gamepad->numAxes;
							axis->cookie = IOHIDElementGetCookie(element);
							axis->logicalMin = IOHIDElementGetLogicalMin(element);
							axis->logicalMax = IOHIDElementGetLogicalMax(element);
							axis->hasNullState = !!IOHIDElementHasNullState(element);
							axis->isHatSwitch = IOHIDElementGetUsage(element) == kHIDUsage_GD_Hatswitch;
							axis->isHatSwitchSecondAxis = false;
							axis->nextHatSwitch = NULL;
							gamepad->axes.push_back(axis);
							gamepad->numAxes++; 

							if (axis->id == 4 || axis->id == 5) { 
								axis->value = -1.0f;
							}

							if (axis->isHatSwitch) { 
								GamepadAxis* nextaxis = new GamepadAxis();
								//axis->value = -1.0f;
								axis->nextHatSwitch = nextaxis;
								nextaxis->id = gamepad->numAxes; 
								nextaxis->isHatSwitchSecondAxis = true;
								nextaxis->nextHatSwitch = NULL;
								//nextaxis->value = -1.0f;
								gamepad->axes.push_back(nextaxis);
								gamepad->numAxes++;
							}
							
						} else if (type == kIOHIDElementTypeInput_Button) {
							//hidDeviceRecord->buttonElements = realloc(hidDeviceRecord->buttonElements, sizeof(struct HIDGamepadButton) * (deviceRecord->numButtons + 1));
							//hidDeviceRecord->buttonElements[deviceRecord->numButtons].cookie = IOHIDElementGetCookie(element);
							//deviceRecord->numButtons++;

							GamepadButton* button = new GamepadButton();
							button->id = gamepad->numButtons;
							button->cookie = IOHIDElementGetCookie(element);
							button->down = false;
							gamepad->buttons.push_back(button);

							gamepad->numButtons++;
						}
					}
					CFRelease(elements); 
				

				}
				
				//deviceRecord->axisStates = calloc(sizeof(float), deviceRecord->numAxes);
				//deviceRecord->buttonStates = calloc(sizeof(bool), deviceRecord->numButtons);

				//for (unsigned int j = 0; j < gamepad->numButtons; j++) {
				//	gamepad->buttonStates.push_back(false);
				//}
				IOHIDDeviceRegisterInputValueCallback(device, onDeviceValueChanged, gamepad);

				ARK2D::getLog()->v(StringUtil::append("Adding Gamepad: ", gamepad->name));
				ARK2D::getContainer()->getGamepads()->push_back(gamepad);
				
				/*queuedEvent.dispatcher = Gamepad_eventDispatcher();
				queuedEvent.eventType = GAMEPAD_EVENT_DEVICE_ATTACHED;
				queuedEvent.eventData = deviceRecord;
				
				if (deviceEventCount >= deviceEventQueueSize) {
					deviceEventQueueSize = deviceEventQueueSize == 0 ? 1 : deviceEventQueueSize * 2;
					deviceEventQueue = realloc(deviceEventQueue, sizeof(struct Gamepad_queuedEvent) * deviceEventQueueSize);
				}
				deviceEventQueue[deviceEventCount++] = queuedEvent;*/
			}

			
			static void onDeviceRemoved(void * context, IOReturn result, void * sender, IOHIDDeviceRef device) {
				GameContainer* container = ARK2D::getContainer();
				unsigned int numDevices = container->getGamepads()->size();

				ARK2D::getLog()->i("Dispose a gamepad... ");

				for (unsigned int deviceIndex = 0; deviceIndex < numDevices; deviceIndex++) 
				{
					Gamepad* pad = container->getGamepads()->at(deviceIndex);
					if (pad->deviceRef == device) {
						//Gamepad_eventDispatcher()->dispatchEvent(Gamepad_eventDispatcher(), GAMEPAD_EVENT_DEVICE_REMOVED, devices[deviceIndex]);
						
						ARK2D::getLog()->i(StringUtil::append("Dispose gamepad: ", pad->id));
						disposeDevice(pad); 
						container->getGamepads()->erase(container->getGamepads()->begin() + deviceIndex);

						//numDevices--;
						//for (; deviceIndex < numDevices; deviceIndex++) {
						//	devices[deviceIndex] = devices[deviceIndex + 1];
						//}
						return;
					}
				}
			}
 
			
		
			void ARK::Core::GameContainer::initGamepads() {
				if (m_platformSpecific.m_hidManager == NULL) {
					ARK2D::getLog()->v("HID Manager was NULL. Initialising...");

					Gamepad::s_nextGamepadId = 0;

					CFStringRef keys[2];
					int value;
					CFNumberRef values[2];
					CFDictionaryRef dictionaries[3];
					CFArrayRef array;
					 
					m_platformSpecific.m_hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
					IOHIDManagerOpen(m_platformSpecific.m_hidManager, kIOHIDOptionsTypeNone);
					IOHIDManagerScheduleWithRunLoop(m_platformSpecific.m_hidManager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
					
					keys[0] = CFSTR(kIOHIDDeviceUsagePageKey); 
					keys[1] = CFSTR(kIOHIDDeviceUsageKey);
					
					value = kHIDPage_GenericDesktop;
					values[0] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
					value = kHIDUsage_GD_Joystick;
					values[1] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
					dictionaries[0] = CFDictionaryCreate(kCFAllocatorDefault, (const void **) keys, (const void **) values, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
					CFRelease(values[0]);
					CFRelease(values[1]);
					
					value = kHIDPage_GenericDesktop;
					values[0] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
					value = kHIDUsage_GD_GamePad;
					values[1] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
					dictionaries[1] = CFDictionaryCreate(kCFAllocatorDefault, (const void **) keys, (const void **) values, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
					CFRelease(values[0]);
					CFRelease(values[1]);
					
					value = kHIDPage_GenericDesktop;
					values[0] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
					value = kHIDUsage_GD_MultiAxisController;
					values[1] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
					dictionaries[2] = CFDictionaryCreate(kCFAllocatorDefault, (const void **) keys, (const void **) values, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
					CFRelease(values[0]);
					CFRelease(values[1]);
					
					array = CFArrayCreate(kCFAllocatorDefault, (const void **) dictionaries, 3, &kCFTypeArrayCallBacks);
					CFRelease(dictionaries[0]);
					CFRelease(dictionaries[1]);
					CFRelease(dictionaries[2]);
					IOHIDManagerSetDeviceMatchingMultiple(m_platformSpecific.m_hidManager, array);
					CFRelease(array);
					
					IOHIDManagerRegisterDeviceMatchingCallback(m_platformSpecific.m_hidManager, onDeviceMatched, NULL);
					IOHIDManagerRegisterDeviceRemovalCallback(m_platformSpecific.m_hidManager, onDeviceRemoved, NULL);
				}
			}


		
			void ARK::Core::GameContainer::setCursorVisible(bool b) {
				// TODO: cache the value here.
				if (CGCursorIsVisible() && !b) {
					CGDisplayHideCursor(kCGDirectMainDisplay); 
				} else if (!CGCursorIsVisible() && b) {
					CGDisplayShowCursor(kCGDirectMainDisplay);
				}
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
				if (m_willLoadDefaultFont) { 
					ARK::Font::BMFont* fnt = ARK::Core::Resource::get("ark2d/fonts/default.fnt")->asFont()->asBMFont(); // BMFont("ark2d/fonts/default.fnt", "ark2d/fonts/default.png");
					m_graphics.m_DefaultFont = fnt;
					m_graphics.m_Font = fnt;
				} else { 
					m_graphics.m_DefaultFont = NULL;
					m_graphics.m_Font = NULL;
				}
				
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
				   
					int delta = (int) (m_timer.getDeltaNoModifiers() * 1000);
					m_game.preUpdate(this, &m_timer);
					m_game.update(this, &m_timer);
					m_game.postUpdate(this, &m_timer);
					m_input.clearKeyPressedRecord();
					for (unsigned int i = 0; i < m_gamepads.size(); i++) {
						m_gamepads.at(i)->clearButtonPressedRecord();
					}
					
					RendererStats::reset();
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					m_game.preRender(this, &m_graphics);
					m_game.render(this, &m_graphics);
					ARK2D::getLog()->render(this, &m_graphics);
					m_game.postRender(this, &m_graphics);
					//if (m_showingFPS) { renderFPS(); }
					
					
					swapBuffers();
					 
					usleep(delta * 500); // 0.017/2.
				}
				
				disableOpenAL();

				close();
				
			}
		
			void ARK::Core::GameContainer::close() const {
				ARK2D::getLog()->i("GameContainer::close"); 
				ARK2D::getLog()->i("Deleting Game object "); 
				delete &m_game;

				ARK2D::getLog()->i("Closing Window at last "); 
				[m_platformSpecific.m_window close]; 
				exit(0); 
			}
			
			NSOpenGLContext* ARK::Core::GameContainerPlatform::createGLContext() 
			{
			    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
			    
			    NSOpenGLPixelFormatAttribute attr[] =
			    {
					NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
					NSOpenGLPFADoubleBuffer, 
					NSOpenGLPFAAccelerated, 
					//NSOpenGLPFADepthSize, 24, 
					NSOpenGLPFADepthSize, 16,
					NSOpenGLPFAColorSize, 32, 
					//NSOpenGLPFAAlphaSize, 8,
					//NSOpenGLPFAStencilSize, 1, 
					NSOpenGLPFAStencilSize, 8,  
					
					NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)1,
					NSOpenGLPFASamples, (NSOpenGLPixelFormatAttribute)4,

			        NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(CGMainDisplayID()),
					0 
				};
				NSOpenGLPixelFormat* format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attr];

				if (format == nil) {
					// TODO: 
					ARK2D::getLog()->i("Could not create NSOpenGLPixelFormat.");
					ARK2D::getLog()->i("4 x multisampling, 8-bit stencil buffer.");
					// try again without stencil
					// try again without multisampling.
					// ... try again with pre opengl 3.0 -- NSOpenGLProfileVersionLegacy
				}

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

					try {
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
							case NSRightMouseDown:
								std::cout << "right mouse down" << std::endl;
								ARK2D::getInput()->pressKey(Input::MOUSE_BUTTON_RIGHT);
								break;
							case NSRightMouseUp:  
								std::cout << "right mouse up" << std::endl;
								ARK2D::getInput()->releaseKey(Input::MOUSE_BUTTON_RIGHT);
								break;
							default:
								[NSApp sendEvent:event];
								break;
						}
					} catch (exception e) {
						
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
