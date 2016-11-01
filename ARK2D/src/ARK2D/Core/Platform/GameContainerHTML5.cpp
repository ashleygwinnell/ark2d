/*
 * GameContainerHTML5.cpp
 *
 *  Created on: Sep 18, 2012
 *      Author: ashleygwinnell
 */



#ifdef ARK2D_EMSCRIPTEN_JS

	#include  <X11/Xlib.h>
	#include  <X11/Xatom.h>
	#include  <X11/Xutil.h>

#include "../../Namespaces.h"
#include "../GameContainer.h"
#include "GameContainerHTML5.h"
#include "../Log.h"
#include "../Camera.h"
#include "../Math/Random.h"

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

#include <SDL.h>


			static Display* x_display = NULL;

			void* ARK::Core::GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
				return new string("");
			}

			EM_BOOL keydown_callback(int eventType, const EmscriptenKeyboardEvent *e, void *userData)
		    {
		      //ARK2D::getLog()->e("You pressed key %lu\n", e->which);
		      //ARK2D::getLog()->e(StringUtil::append("You pressed key ", e->which));
		    	ARK2D::getInput()->pressKey(e->which);
		      return 1;
		    }
		    EM_BOOL keyup_callback(int eventType, const EmscriptenKeyboardEvent *e, void *userData)
		    {
		      //ARK2D::getLog()->e("You pressed key %lu\n", e->which);
		      //ARK2D::getLog()->e(StringUtil::append("You pressed key ", e->which));
		    	ARK2D::getInput()->releaseKey(e->which);
		      return 1;
		    }

		    EM_BOOL mousedown_callback(int eventType, const EmscriptenMouseEvent *e, void *userData)
		    {
				GameContainer* m_container = ARK2D::getContainer();
				Input* i = ARK2D::getInput();
				if (i->mouse_x < 0 || i->mouse_x > m_container->getDynamicWidth() ||
					i->mouse_y < 0 || i->mouse_y > m_container->getDynamicHeight()) { return 1; }

				int ark2d_mouse_button = 0;
				if (e->button == 0) {
					ark2d_mouse_button = Input::MOUSE_BUTTON_LEFT;
				} else if (e->button == 1) {
					ark2d_mouse_button = Input::MOUSE_BUTTON_MIDDLE;
				} else if (e->button == 2) {
					ark2d_mouse_button = Input::MOUSE_BUTTON_RIGHT;
				}
				//ARK2D::getLog()->e(StringUtil::append("Mouse down: ", e->button));
				ARK2D::getInput()->pressKey(ark2d_mouse_button);
				return 1;
		    }

		    EM_BOOL mouseup_callback(int eventType, const EmscriptenMouseEvent *e, void *userData)
		    {
				GameContainer* m_container = ARK2D::getContainer();
				Input* i = ARK2D::getInput();
				if (i->mouse_x < 0 || i->mouse_x > m_container->getDynamicWidth() ||
					i->mouse_y < 0 || i->mouse_y > m_container->getDynamicHeight()) { return 1; }

				int ark2d_mouse_button = 0;
				if (e->button == 0) {
					ark2d_mouse_button = Input::MOUSE_BUTTON_LEFT;
				} else if (e->button == 1) {
					ark2d_mouse_button = Input::MOUSE_BUTTON_MIDDLE;
				} else if (e->button == 2) {
					ark2d_mouse_button = Input::MOUSE_BUTTON_RIGHT;
				}
				//ARK2D::getLog()->e(StringUtil::append("Mouse up: ", e->button));
				ARK2D::getInput()->releaseKey(ark2d_mouse_button);
				return 1;
			}

			EM_BOOL mousemove_callback(int eventType, const EmscriptenMouseEvent* e, void *userData)
			{
				GameContainer* m_container = ARK2D::getContainer();
				float thisx = e->canvasX;
				float thisy = e->canvasY;

				thisx -= m_container->getTranslateX();
				thisy -= m_container->getTranslateY();

				thisx /= m_container->getScale();
				thisy /= m_container->getScale();

				Input* i = ARK2D::getInput();
				//if (thisx < 0 || thisx > m_container->getDynamicWidth()) { i->mouse_x = -1; i->mouse_y = -1; return 0; }
				//if (thisy < 0 || thisy > m_container->getDynamicHeight()) { i->mouse_x = -1; i->mouse_y = -1; return 0; }

				//ARK2D::getLog()->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);
				ARK2D::getGame()->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);

				//string pstr = "mouse moved: ";
				//pstr += Cast::toString<int>(thisx);
				//pstr += ",";
				//pstr += Cast::toString<int>(thisy);
				//ARK2D::getLog()->e(pstr);

				i->mouse_x = (int) thisx;
				i->mouse_y = (int) thisy;

				return 1;
			}

			EM_BOOL fullscreenchange_callback(int eventType, const EmscriptenFullscreenChangeEvent *e, void *userData)
			{
				//printf("%s, isFullscreen: %d, fullscreenEnabled: %d, fs element nodeName: \"%s\", fs element id: \"%s\". New size: %dx%d pixels. Screen size: %dx%d pixels.",
      //Cast::toString<int>(eventType).c_str(), e->isFullscreen, e->fullscreenEnabled, e->nodeName, e->id, e->elementWidth, e->elementHeight, e->screenWidth, e->screenHeight);

				EmscriptenFullscreenChangeEvent state;
				emscripten_get_fullscreen_status(&state);

				ARK2D::getLog()->i("Fullscreen Change Callback");

				GameContainer* container = ARK2D::getContainer();
				int newWidth;
				int newHeight;
				if (state.isFullscreen) {
					newWidth = state.screenWidth;
					newHeight = state.screenHeight;

					ARK2D::getLog()->i("is fullscreen");
				} else {
					newWidth = container->getWidth();
					newHeight = container->getHeight();

					ARK2D::getLog()->i("is not fullscreen");
				}

				container->setSize(newWidth, newHeight, true);
				container->getPlatformSpecific()->setFullscreenInternal(e->isFullscreen);
				//container->resizeBehaviour(newWidth, newHeight);
				//container->resizeWindowToFitViewport();

				return 0;
			}

			void GameContainerPlatform::setFullscreenInternal(bool b) {
				m_container->m_fullscreen = b;
			}

			ARK::Core::GameContainer::GameContainer(Game& g, int width, int height, int bpp, bool fullscreen):
				m_timer(),
				m_game(g),
				m_input(),
				m_graphics(),
				m_gamepads(),
				scene(NULL),
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
				m_bRunning(false),
				m_clearColor(Color::black),
				m_resizeBehaviour(RESIZE_BEHAVIOUR_SCALE),
				m_showingFPS(false),
				m_willLoadDefaultFont(true),
				m_platformSpecific()
			{
				m_platformSpecific.m_container = this;

				m_input.setGameContainer(this);

				ARK2D::s_container = this;
				ARK2D::s_game = &m_game;
				ARK2D::s_graphics = &m_graphics;
				ARK2D::s_input = &m_input;
				ARK2D::s_camera = new ARK::Core::Camera();
				ARK2D::s_camera->setViewport(0, 0, m_width, m_height);
				ARK2D::s_log = ARK::Core::Log::getInstance();
				scene = ARK2D::getScene();

				m_platformSpecific.esContext = new ESContext(); //(ESContext*) malloc(sizeof(ESContext));
				m_platformSpecific.esContext->userData = NULL; //malloc(sizeof(UserData));
				//memset( m_platformSpecific.esContext, 0, sizeof( ESContext) );
				//memset( m_platformSpecific.esContext->userData, 0, sizeof( UserData) );

				ARK2D::getRenderer()->preinit();

			}

			void ARK::Core::GameContainer::setSize(int width, int height, bool docallback) {

				if (width == (signed int) m_width && height == (signed int) m_height) { return; }

				resizeBehaviour(width, height, docallback);

				resizeWindowToFitViewport();
			}


			void ARK::Core::GameContainer::setFullscreen(bool fullscreen) {

				EmscriptenFullscreenChangeEvent state;
				emscripten_get_fullscreen_status(&state);

				if (!state.isFullscreen && fullscreen) {
					ARK2D::getLog()->i("Go fullscreen mode.");
					emscripten_request_fullscreen(0, 1);

					//m_width = m_screenWidth;
					//m_height = m_screenHeight;
					//resizeBehaviour((int) m_screenWidth, (int) m_screenHeight, true);

				} else if (state.isFullscreen && !fullscreen) {
					ARK2D::getLog()->i("Go windowed mode.");
					emscripten_exit_fullscreen();

					//m_width = m_originalWidth;
					//m_height = m_originalHeight;
					//resizeBehaviour((int) m_originalWidth, (int) m_originalHeight, true);
				}
				///resizeWindowToFitViewport();


				/*if (!m_fullscreen && fullscreen) {
					// go fullscreen
					ARK2D::getLog()->i("Go fullscreen mode.");
					//setSize(m_screenWidth, m_screenHeight);

					//m_fullscreen = fullscreen;

				} else if (m_fullscreen && !fullscreen) {
					// go windowed.
					ARK2D::getLog()->i("Go windowed mode.");
					//setSize(m_originalWidth, m_originalHeight);
					emscripten_exit_fullscreen();

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
				EM_ASM_({
					var canvas = document.getElementById("canvas");
					canvas.width = $0;
					canvas.height = $1;

				}, m_width, m_height);
			}

			///
			//  WinCreate()
			//
			//      This function initialized the native X11 display and window for EGL
			//
			EGLBoolean WinCreate(ESContext* esContext, const char *title)
			{
			    Window root;
			    XSetWindowAttributes swa;
			    XSetWindowAttributes  xattr;
			    Atom wm_state;
			    XWMHints hints;
			    XEvent xev;
			    EGLConfig ecfg;
			    EGLint num_config;
			    Window win;

			    /*
			     * X11 native display initialization
			     */

			    x_display = XOpenDisplay(NULL);
			    if ( x_display == NULL )
			    {
			        return EGL_FALSE;
			    }

			    root = DefaultRootWindow(x_display);

			    swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;
			    win = XCreateWindow(
			               x_display, root,
			               0, 0, esContext->width, esContext->height, 0,
			               CopyFromParent, InputOutput,
			               CopyFromParent, CWEventMask,
			               &swa );

			    xattr.override_redirect = FALSE;
			    XChangeWindowAttributes ( x_display, win, CWOverrideRedirect, &xattr );

			    hints.input = TRUE;
			    hints.flags = InputHint;
			    XSetWMHints(x_display, win, &hints);

			    // make the window visible on the screen
			    XMapWindow (x_display, win);
			    XStoreName (x_display, win, title);

			    // get identifiers for the provided atom name strings
			    wm_state = XInternAtom (x_display, "_NET_WM_STATE", FALSE);

			    memset ( &xev, 0, sizeof(xev) );
			    xev.type                 = ClientMessage;
			    xev.xclient.window       = win;
			    xev.xclient.message_type = wm_state;
			    xev.xclient.format       = 32;
			    xev.xclient.data.l[0]    = 1;
			    xev.xclient.data.l[1]    = FALSE;
			    XSendEvent (
			       x_display,
			       DefaultRootWindow ( x_display ),
			       FALSE,
			       SubstructureNotifyMask,
			       &xev );

			    esContext->hWnd = (EGLNativeWindowType) win;
			    return EGL_TRUE;
			}

			EGLBoolean CreateEGLContext ( EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
                              EGLContext* eglContext, EGLSurface* eglSurface,
                              EGLint attribList[])
			{
			   EGLint numConfigs;
			   EGLint majorVersion;
			   EGLint minorVersion;
			   EGLDisplay display;
			   EGLContext context;
			   EGLSurface surface;
			   EGLConfig config;
			   EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

			   // Get Display
			   display = eglGetDisplay((EGLNativeDisplayType)x_display);
			   if ( display == EGL_NO_DISPLAY )
			   {
			      ARK2D::getLog()->e("EGL_NO_DISPLAY");
			      return EGL_FALSE;
			   }

			   // Initialize EGL
			   if ( !eglInitialize(display, &majorVersion, &minorVersion) )
			   {
			      ARK2D::getLog()->e("eglInitialize");
			      return EGL_FALSE;
			   }

			   // Get configs
			   if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
			   {
			      ARK2D::getLog()->e("eglGetConfigs");
			      return EGL_FALSE;
			   }

			   // Choose config
			   if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) )
			   {
			      ARK2D::getLog()->e("eglChooseConfig");
			      return EGL_FALSE;
			   }

			   // Create a surface
			   surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
			   if ( surface == EGL_NO_SURFACE )
			   {
			      ARK2D::getLog()->e("EGL_NO_SURFACE");
			      return EGL_FALSE;
			   }

			   // Create a GL context
			   context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
			   if ( context == EGL_NO_CONTEXT )
			   {
			      ARK2D::getLog()->e("eglCreateContext");
			      return EGL_FALSE;
			   }

			   // Make the context current
			   if ( !eglMakeCurrent(display, surface, surface, context) )
			   {
			      ARK2D::getLog()->e("eglMakeCurrent");
			      return EGL_FALSE;
			   }

			   *eglDisplay = display;
			   *eglSurface = surface;
			   *eglContext = context;
			   return EGL_TRUE;
			}

			///
			//  userInterrupt()
			//
			//      Reads from X11 event loop and interrupt program if there is a keypress, or
			//      window close action.
			//
			/*GLboolean userInterrupt(ESContext* esc)
			{
			    XEvent xev;
			    KeySym key;
			    GLboolean userinterrupt = GL_FALSE;
			    char text;

			    // Pump all messages from X server. Keypresses are directed to keyfunc (if defined)
			    while ( XPending ( x_display ) )
			    {
			        XNextEvent( x_display, &xev );
			        if ( xev.type == KeyPress )
			        {
			            ARK2D::getLog()->e("a key was pressed");
			            if (XLookupString(&xev.xkey,&text,1,&key,0)==1)
			            {
			                if (esc->keyFunc != NULL)
			                    esc->keyFunc(esc, text, 0, 0);
			            }
			        }
			        if ( xev.type == DestroyNotify )
			            userinterrupt = GL_TRUE;
			    }
			    return userinterrupt;
			}*/


			/// esCreateWindow flag - RGB color buffer
			#define ES_WINDOW_RGB           0
			/// esCreateWindow flag - ALPHA color buffer
			#define ES_WINDOW_ALPHA         1
			/// esCreateWindow flag - depth buffer
			#define ES_WINDOW_DEPTH         2
			/// esCreateWindow flag - stencil buffer
			#define ES_WINDOW_STENCIL       4
			/// esCreateWindow flat - multi-sample buffer
			#define ES_WINDOW_MULTISAMPLE   8

			void ARK::Core::GameContainer::start() {

				// seed the random
				Random::init();

				m_screenWidth = EM_ASM_INT_V({
					return screen.width;
				});

				m_screenHeight = EM_ASM_INT_V({
					return screen.height;
				});

				emscripten_set_keydown_callback(0, 0, 1, keydown_callback);
				emscripten_set_keyup_callback(0, 0, 1, keyup_callback);
      			emscripten_set_mousedown_callback( "#document" ,  0, 1, mousedown_callback);
      			emscripten_set_mouseup_callback( "#document" ,  0, 1, mouseup_callback);
      			emscripten_set_mousemove_callback( "#document" ,  0, 1, mousemove_callback);
      			emscripten_set_fullscreenchange_callback(0, 0, 1, fullscreenchange_callback);

				SDL_Init(SDL_INIT_VIDEO);

				/*SDL_CreateWindow(
			        "",                  // window title
			        SDL_WINDOWPOS_UNDEFINED,           // initial x position
			        SDL_WINDOWPOS_UNDEFINED,           // initial y position
			        m_originalWidth,                               // width, in pixels
			        m_originalHeight,                               // height, in pixels
			        SDL_WINDOW_OPENGL                  // flags - see below
			    );
*/

				unsigned int flags = ES_WINDOW_RGB | ES_WINDOW_ALPHA | ES_WINDOW_STENCIL | ES_WINDOW_MULTISAMPLE;
				// init window
				EGLint attribList[] =
				   {
				       EGL_RED_SIZE,       5,
				       EGL_GREEN_SIZE,     6,
				       EGL_BLUE_SIZE,      5,
				       EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
				       EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
				       EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
				       EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 2 : 0,
				       EGL_NONE
				   };

				m_platformSpecific.esContext->width = m_originalWidth;
   				m_platformSpecific.esContext->height = m_originalHeight;

   				bool wincreated = WinCreate(m_platformSpecific.esContext, m_strTitle.c_str());
   				if (!wincreated) {
   					ARK2D::getLog()->e("Could not create Window. ");
   					return;
   				}

   				if ( !CreateEGLContext ( m_platformSpecific.esContext->hWnd,
                            &m_platformSpecific.esContext->eglDisplay,
                            &m_platformSpecific.esContext->eglContext,
                            &m_platformSpecific.esContext->eglSurface,
                            attribList) )
				   {
				      ARK2D::getLog()->e("Could not create Window GL Context (v2). ");
				      return;
				   }



				//glfwSetMousePosCallback(OnMouseMove);
  				//glfwSetMouseButtonCallback(OnMouseClick);

				showAnyGlErrorAndExitMacro();
				ARK2D::getLog()->i("Enabling OpenGL.");
				ARK2D::getRenderer()->init();
				showAnyGlErrorAndExitMacro();

				ARK2D::getLog()->i("Enabling Texturing?");
				/*glEnable(GL_TEXTURE_2D);
				glViewport(0, 0, m_originalWidth, m_originalHeight);
				glClearColor(m_clearColor.getRed()/255.0f, m_clearColor.getGreen()/255.0f, m_clearColor.getBlue()/255.0f, m_clearColor.getAlpha()/255.0f);
*/
				glClear( GL_COLOR_BUFFER_BIT );
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				showAnyGlErrorAndExitMacro();

				ARK2D::getLog()->i("Enabling OpenGL 2d.");
				enable2D();

				m_bRunning = true;

				// Set window title
				//setTitle(m_game.getTitle());

				// populate the gamepads.
				ARK2D::getLog()->i("Initialising Gamepads... ");
				initGamepads();
				ARK2D::getLog()->i("done.");



				// load default font.
				if (m_willLoadDefaultFont) {
					ARK::Core::Font::BMFont* fnt = ARK::Core::Resource::get("ark2d/fonts/default.fnt")->asFont()->asBMFont(); // BMFont("ark2d/fonts/default.fnt", "ark2d/fonts/default.png");
					fnt->scale(0.5f);
					m_graphics.m_DefaultFont = fnt;
					m_graphics.m_Font = fnt;

					std::cout << "font pointer: " << (int) (fnt) <<  " and null is: " << (int) (NULL) << "." << std::endl;
				} else {
					m_graphics.m_DefaultFont = NULL;
					m_graphics.m_Font = NULL;
				}
				//ARK2D::s_graphics->setDefaultFont(fnt);
				//ARK2D::s_graphics->setFont(fnt);



				// initialise
				enableOpenAL();

				ARK2D::getLog()->i("Initialising Log");
				ARK2D::s_log->init();

				ARK2D::getLog()->i("Initialising Localisations");
				initLocalisation();

				// initialise game.
				ARK2D::getLog()->i("Initialising ");
				ARK2D::getLog()->i(m_game.getTitle());
				ARK2D::getLog()->i("...");
				m_game.init(this);

				ARK2D::getLog()->i("Initialised ");
				ARK2D::getLog()->i(m_game.getTitle());
				ARK2D::getLog()->i("...");

				ARK2D::getTimer()->flush();

				if (m_fullscreen) {
					setFullscreen(true);
				}

				//AS3_GoAsync();
				//disableOpenAL();
				//close();

				//while(userInterrupt(m_platformSpecific.esContext) == GL_FALSE) {
				//	m_platformSpecific.doTick();
				//}
				emscripten_set_main_loop(&GameContainerPlatform::staticTick, 0, 1);

			}
			void GameContainerPlatform::staticTick() {
				ARK2D::getContainer()->getPlatformSpecific()->doTick();
			}

			void GameContainerPlatform::doTick() {
				//ARK2D::getLog()->i("Main Loop Tick");
				GameContainer* container = ARK2D::getContainer();



				/*{
					XEvent xev;
					while (XPending(x_display) > 0) {
						XNextEvent(x_display, &xev);

						XEvent event = xev;
						if (event.type == Expose) {
							//XGetWindowAttributes(m_platformSpecific.dpy, m_platformSpecific.win, &m_platformSpecific.gwa);
						} else if (event.type == ConfigureNotify) {
							//if ((event.xconfigure.width != m_width) || (event.xconfigure.height != m_height)) {
							//	resizeBehaviour((int) event.xconfigure.width, (int) event.xconfigure.height);
								//resizeWindowToFitViewport();

                        	//}
						} else if (event.type == FocusIn) {

							ARK2D::getLog()->v("focus in");
							//if (m_fullscreen) {
							//	m_platformSpecific.setAlwaysOnTop(true);
							//	XRaiseWindow(m_platformSpecific.dpy, m_platformSpecific.win);
							//}

						} else if (event.type == FocusOut) {
							ARK2D::getLog()->v("focus out");
							//if (m_fullscreen) {
							//	m_platformSpecific.setAlwaysOnTop(false);
							//	XLowerWindow(m_platformSpecific.dpy, m_platformSpecific.win);
							//}


						} else if (event.type == MotionNotify) {

							float thisx = event.xmotion.x;
							float thisy = event.xmotion.y;

						    thisx -= m_container->getTranslateX();
						    thisy -= m_container->getTranslateY();

						    thisx /= m_container->getScale();
						    thisy /= m_container->getScale();

							Input* i = ARK2D::getInput();
							ARK2D::getLog()->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);
							ARK2D::getGame()->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);

							//string pstr = "mouse moved: ";
							//pstr += Cast::toString<int>(thisx);
							//pstr += ",";
							//pstr += Cast::toString<int>(thisy);
							//ARK2D::getLog()->e(pstr);

							i->mouse_x = (int) thisx;
							i->mouse_y = (int) thisy;

							//m_platformSpecific.m_globalMouseX = event.xmotion.x_root;
							//m_platformSpecific.m_globalMouseY = event.xmotion.y_root;

						} else if (event.type == ButtonPress) {

							// http://stackoverflow.com/questions/16185286/how-to-detect-mouse-click-events-in-all-applications-in-x11
							if (event.xbutton.button == 1) {
								ARK2D::getInput()->pressKey(Input::MOUSE_BUTTON_LEFT);
							} else if (event.xbutton.button == 2) {
								ARK2D::getInput()->pressKey(Input::MOUSE_BUTTON_MIDDLE);
							} else if (event.xbutton.button == 3) {
								ARK2D::getInput()->pressKey(Input::MOUSE_BUTTON_RIGHT);
							} else if (event.xbutton.button == 4) {
								// scroll up
							} else if (event.xbutton.button == 5) {
								// scroll down
							}

						} else if (event.type == ButtonRelease) {
							if (event.xbutton.button == 1) {
								ARK2D::getInput()->releaseKey(Input::MOUSE_BUTTON_LEFT);
							} else if (event.xbutton.button == 2) {
								ARK2D::getInput()->releaseKey(Input::MOUSE_BUTTON_MIDDLE);
							} else if (event.xbutton.button == 3) {
								ARK2D::getInput()->releaseKey(Input::MOUSE_BUTTON_RIGHT);
							} else if (event.xbutton.button == 4) {
								// scroll up
							} else if (event.xbutton.button == 5) {
								// scroll down
							}
						} else if (event.type == KeyPress) {
							//std::map<int, int>::iterator it = m_platformSpecific.keycodemap_reverse->find(event.xkey.keycode);
							//if (it != m_platformSpecific.keycodemap_reverse->end()) {
							//	ARK2D::getInput()->pressKey(it->second);
							//}
						} else if (event.type == KeyRelease) {
							//std::map<int, int>::iterator it = m_platformSpecific.keycodemap_reverse->find(event.xkey.keycode);
							//if (it != m_platformSpecific.keycodemap_reverse->end()) {
							//	ARK2D::getInput()->releaseKey(it->second);
							//}
						}
					}
				}*/

				{
					//SDL_PumpEvents();



					/*SDL_PumpEvents();
					if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
					    ARK2D::getLog()->e("Mouse Button 1 (left) is pressed.");
					}*/

/*
					SDL_Event event;
					while(SDL_PollEvent(&event)){  // Loop until there are no events left on the queue
					  switch(event.type){  // Process the appropiate event type
					    case SDL_KEYDOWN:  // Handle a KEYDOWN event
					      ARK2D::getLog()->e("Oh! Key down\n");
					      break;
					    case SDL_KEYUP:  // Handle a KEYDOWN event
					      ARK2D::getLog()->e("Oh! Key up\n");
					      break;
					    case SDL_MOUSEBUTTONDOWN:
					      ARK2D::getLog()->e("Oh! Mouse down\n");
					       if (event.button.button == SDL_BUTTON_LEFT) {
					       	ARK2D::getInput()->pressKey(Input::MOUSE_BUTTON_LEFT);
					       }
					      break;
					    case SDL_MOUSEBUTTONUP:
					    	ARK2D::getLog()->e("Oh! Mouse up\n");
					    	if (event.button.button == SDL_BUTTON_LEFT) {
					       		ARK2D::getInput()->releaseKey(Input::MOUSE_BUTTON_LEFT);
					       	}
					    	break;
					    case SDL_MOUSEMOTION:
					    	ARK2D::getLog()->e("Oh! Mouse motion.\n");
					    	break;
					    default: // Report an unhandled event
					      ARK2D::getLog()->e("I don't know what this event is!\n");
					  }
					}*/

				}

				GameTimer* m_timer = container->getTimer();
				Game* m_game = ARK2D::getGame();
				Input* m_input = ARK2D::getInput();
				Renderer* m_graphics = ARK2D::getRenderer();
				//vector<Gamepad*> m_gamepads = container->getGamepads();

				//ARK2D::getLog()->i("Timer Tick");
				m_timer->tick();
				container->m_platformSpecific.doEvents();

				//container->processGamepadInput();

				//ARK2D::getLog()->update();

			   	//ARK2D::getLog()->update();
			   	container->scene->update();

			   //	ARK2D::getLog()->i("Game Pre-update");
			   	//ARK2D::getLog()->i("timerdelta:");
			   	//string deltastr = Cast::toString<int>(int(m_timer->getDelta()*1000));
			   	//ARK2D::getLog()->i(deltastr);
				//int delta = (int) (m_timer->getDelta() * 1000);
				//ARK2D::getLog()->i("Game Pre-update 2");
				//m_game->preUpdate(container, m_timer);			//ARK2D::getLog()->i("Game Pre-update 3");
				//m_game->update(container, m_timer);				//ARK2D::getLog()->i("Game Pre-update 4");
				//m_game->postUpdate(container, m_timer);

				//ARK2D::getLog()->i("Clear Input");
				m_input->clearKeyPressedRecord();
				// for (unsigned int i = 0; i < m_gamepads.size(); i++) {
				// 	m_gamepads.at(i)->clearButtonPressedRecord();
				// }

				//ARK2D::getLog()->i("Game Pre-render");
				RendererStats::reset();
				glClearColor(container->m_clearColor.getRedf(), container->m_clearColor.getGreenf(), container->m_clearColor.getBluef(), container->m_clearColor.getAlpha());
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				container->preRender();
				container->scene->render();
				container->postRender();
				//if (container->isShowingFPS()) { container->renderFPS(); }


				container->swapBuffers();
				//ARK2D::getLog()->i("Game Post-render");

				//usleep(delta * 500); // 0.017/2.
			}

			void ARK::Core::GameContainer::close() const {
				ARK2D::getLog()->i("GameContainer::close");
				//ARK2D::getLog()->i("Deleting Game object ");
				//delete &m_game;

				//ARK2D::getLog()->i("Closing Window at last ");
				//[m_platformSpecific.m_window close];
				//exit(0);
			}


			void ARK::Core::GameContainer::swapBuffers() {
				eglSwapBuffers(m_platformSpecific.esContext->eglDisplay, m_platformSpecific.esContext->eglSurface);
			}







			void ARK::Core::GameContainerPlatform::doEvents() {
				//userInterrupt(this->esContext);
			}


			void ARK::Core::GameContainerPlatform::setTitle(std::string title) {
				//[m_window setTitle:[NSString stringWithCString:title.c_str() encoding:NSUTF8StringEncoding]];
			}

			string ARK::Core::GameContainerPlatform::getResourcePath() const {
				return "./data/";
			}

			bool ARK::Core::GameContainerPlatform::initOpenAL() {

				int major, minor;
				alcGetIntegerv(NULL, ALC_MAJOR_VERSION, 1, &major);
				alcGetIntegerv(NULL, ALC_MAJOR_VERSION, 1, &minor);

				assert(major == 1);

				printf("ALC version: %i.%i\n", major, minor);
				printf("Default device: %s\n", alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER));

				ALCdevice* device = alcOpenDevice(NULL);
				ALCcontext* context = alcCreateContext(device, NULL);
				alcMakeContextCurrent(context);

				assert(alGetString(AL_VERSION));

				printf("OpenAL version: %s\n", alGetString(AL_VERSION));
				printf("OpenAL vendor: %s\n", alGetString(AL_VENDOR));
				printf("OpenAL renderer: %s\n", alGetString(AL_RENDERER));

				ALfloat listenerPos[] = {0.0, 0.0, 0.0};
				ALfloat listenerVel[] = {0.0, 0.0, 0.0};
				ALfloat listenerOri[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

				alListenerfv(AL_POSITION, listenerPos);
				alListenerfv(AL_VELOCITY, listenerVel);
				alListenerfv(AL_ORIENTATION, listenerOri);

				return false;
			}

			bool ARK::Core::GameContainerPlatform::deinitOpenAL() {
				// discard context and device
				return true;
			}

			int ARK::Core::GameContainer::getGlobalMouseX() const {
				//NSPoint mouseLoc = [NSEvent mouseLocation];
				//return mouseLoc.x;
				return 0;
			}
			int ARK::Core::GameContainer::getGlobalMouseY() const {
				//NSPoint mouseLoc = [NSEvent mouseLocation];
				//return mouseLoc.y;
				return 0;
			}



#endif
