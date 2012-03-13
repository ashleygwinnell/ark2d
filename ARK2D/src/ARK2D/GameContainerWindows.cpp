/*
 * GameContainerWindows.cpp
 *
 *  Created on: 27 Feb 2011
 *      Author: ashley
 */


#include "GameContainerWindows.h"
#include "GameContainer.h" 
#include "ARK2D.h" 

#if defined(ARK2D_WINDOWS)

	#include "Geometry/GigaRectangle.h"

	#include "ARK2D_windres.h" 
	#include "Graphics/Image/Image.h"
 
	#include "Util/ARKLog.h"

		LRESULT CALLBACK GameContainerPlatform::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch (message)
			{
				case WM_CREATE:
				{
					// "data/icon_32.ico"
					if (m_iconpath != "") {
						HICON hLIcon;
						hLIcon = (HICON) LoadImage(NULL, m_iconpath.c_str(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
						if (hLIcon) {
							SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM) hLIcon);
						} else {
							HINSTANCE dllModule = LoadLibrary("libARK2D.dll");
							if (dllModule == NULL) {
								ErrorDialog::createAndShow("Could not load large icon!");
							} else {
								hLIcon = (HICON) LoadImage(dllModule, MAKEINTRESOURCE(ARK2D_ICON), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
								SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM) hLIcon);
							}
							FreeLibrary(dllModule);
						}

						HICON hSIcon = (HICON) LoadImage(NULL, m_iconpath.c_str(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
						if (hSIcon) {
							SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM) hSIcon);
						} else {
							HINSTANCE dllModule = LoadLibrary("libARK2D.dll");
							if (dllModule == NULL) {
								ErrorDialog::createAndShow("Could not load small icon!");
							} else {
								hSIcon = (HICON) LoadImage(dllModule, MAKEINTRESOURCE(ARK2D_ICON), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
								SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM) hSIcon);
							}
							FreeLibrary(dllModule);
						}
					}
					return 0;
				}
				case WM_SIZE:
				{
					m_container->m_width =  LOWORD(lParam);
					m_container->m_height = HIWORD(lParam);
					if (m_container->m_scaleToWindow) {
						glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
					} else {
						m_container->disable2D();
						m_container->enable2D();
						glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
					}
					m_container->m_game.resize(m_container, LOWORD(lParam), HIWORD(lParam));
					return 0;
				}
				case WM_CLOSE:
				{
					PostQuitMessage( 0 );
					return 0;
				}
				case WM_DESTROY: 
				{
					return 0;
				}
				case WM_KEYDOWN:
				{
					m_container->m_input.pressKey(wParam);
					break;
				}
				case WM_KEYUP:
				{
					m_container->m_input.releaseKey(wParam);
					break;
				}
				case WM_ACTIVATEAPP:
				{
					break;
				}
				case WM_MOUSEMOVE:
				{
					POINTS mouseXY = MAKEPOINTS(lParam);

					ARK2D::getGame()->mouseMoved(mouseXY.x, mouseXY.y, m_container->m_input.mouse_x, m_container->m_input.mouse_y);

					m_container->m_input.mouse_x = mouseXY.x;
					m_container->m_input.mouse_y = mouseXY.y;
					break;
				}
				case WM_LBUTTONDOWN:
				{
					m_container->m_input.pressKey(Input::MOUSE_BUTTON_LEFT);
					break;
				}
				case WM_LBUTTONUP:
				{
					m_container->m_input.releaseKey(Input::MOUSE_BUTTON_LEFT);
					break;
				}
				case WM_RBUTTONDOWN:
				{
					m_container->m_input.pressKey(Input::MOUSE_BUTTON_RIGHT);
					break;
				}
				case WM_RBUTTONUP:
				{
					m_container->m_input.releaseKey(Input::MOUSE_BUTTON_RIGHT);
					break;
				}
				default:
				{
					return DefWindowProc( hWnd, message, wParam, lParam );
				}
			}
			return DefWindowProc( hWnd, message, wParam, lParam );
		}


		LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			GameContainer* container = NULL;
			if (message == WM_CREATE) {
				//Get the pointer we stored during create
				container = (GameContainer*) ((LPCREATESTRUCT)lParam)->lpCreateParams;

				//Associate the window pointer with the hwnd for the other events to access
				SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR) container);
			}
			else
			{
				//If this is not a creation event, then we should have stored a pointer to the window
				container = (GameContainer*)GetWindowLongPtr(hWnd, GWL_USERDATA);

				if(!container)
				{
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
			}
			//Call our window's member WndProc (allows us to access member variables)
			return container->m_platformSpecific.WndProc(hWnd, message, wParam, lParam);
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
			m_showingFPS(false),
			m_platformSpecific()
		{
			m_platformSpecific.m_container = this;

			m_input.setGameContainer(this);

			m_platformSpecific.m_hInstance = GetModuleHandle(NULL);

			ARK2D::s_container = this;
			ARK2D::s_game = &m_game;
			ARK2D::s_graphics = &m_graphics;
			ARK2D::s_input = &m_input;
			ARK2D::s_log = new ARKLog();

			m_platformSpecific.m_windowRect.left = (long) 0; 			// Set Left Value To 0
			m_platformSpecific.m_windowRect.right = (long) width; 		// Set Right Value To Requested Width
			m_platformSpecific.m_windowRect.top = (long) 0;  			// Set Top Value To 0
			m_platformSpecific.m_windowRect.bottom = (long) height;   	// Set Bottom Value To Requested Height


			int fsx = GetSystemMetrics(SM_CXFULLSCREEN);
			int fsy = GetSystemMetrics(SM_CYSCREEN);

			m_platformSpecific.m_screenResolutionRect.left = (long) 0;
			m_platformSpecific.m_screenResolutionRect.right = (long) fsx; 		// Set Right Value To Requested Width
			m_platformSpecific.m_screenResolutionRect.top = (long) 0;  			// Set Top Value To 0
			m_platformSpecific.m_screenResolutionRect.bottom = (long) fsy;   	// Set Bottom Value To Requested Height
		}



		GameContainerDisplayMode* GameContainerPlatform::findDisplayMode(unsigned int w, unsigned int h, unsigned int bpp) {
			for(unsigned int i = 0; i < m_availableDisplayModes.size(); i++) {
				if (m_availableDisplayModes.at(i).width == w
					&& m_availableDisplayModes.at(i).height == h
					&& m_availableDisplayModes.at(i).bpp == bpp
				) {
					return &m_availableDisplayModes.at(i);
				}
			}
			return NULL;
		}

		void GameContainer::setSize(int width, int height) {
 
		}

		void GameContainer::setFullscreen(bool fullscreen) {

			if (fullscreen) {
				// turn off window region without redraw
					//SetWindowRgn(m_hWindow, 0, false);
					DEVMODE newSettings;

					// request current screen settings
					EnumDisplaySettings(0, 0, &newSettings);

					// Find a display mode closest to the width and height
					GameContainerDisplayMode* dm = &m_platformSpecific.m_availableDisplayModes.at(0);
					//GameContainerDisplayMode* dm = m_platformSpecific.m_availableDisplayModes.at(0);


					//  set desired screen size/res
					newSettings.dmPelsWidth  = dm->width; //m_width;
					newSettings.dmPelsHeight = dm->height; //m_height;
					newSettings.dmBitsPerPel = dm->bpp; //m_bpp;

					//specify which aspects of the screen settings we wish to change
					newSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

					// attempt to apply the new settings
					long result = ChangeDisplaySettings(&newSettings, CDS_FULLSCREEN);

					// exit if failure, else set datamember to fullscreen and return true
					if ( result != DISP_CHANGE_SUCCESSFUL )	{
						if (result == DISP_CHANGE_BADFLAGS) {
							ErrorDialog::createAndShow("Could not enter full-screen mode: bad flags.");
						} else if (result == DISP_CHANGE_FAILED) {
							ErrorDialog::createAndShow("Could not enter full-screen mode: change failed.");
						} else if (result == DISP_CHANGE_BADPARAM) {
							ErrorDialog::createAndShow("Could not enter full-screen mode: bad param.");
						} else if (result == DISP_CHANGE_BADMODE) {
							ErrorDialog::createAndShow("Could not enter full-screen mode: bad mode.");
						}
						return;
					} else 	{
						// store the location of the window
						//m_oldLoc = GetLocation();

						// switch off the title bar
						DWORD dwstyle = GetWindowLong(m_platformSpecific.m_hWindow, GWL_STYLE);
						dwstyle &= ~WS_CAPTION;
						SetWindowLong(m_platformSpecific.m_hWindow, GWL_STYLE, dwstyle);

						// move the window to (0,0)
						SetWindowPos(m_platformSpecific.m_hWindow, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
						InvalidateRect(m_platformSpecific.m_hWindow, 0, true);

						MoveWindow(m_platformSpecific.m_hWindow,0 , 0, dm->width, dm->height, TRUE);
						//glViewport(0, 0, m_width, m_height);
						glViewport(0, 0, dm->width, dm->height);

						std::cout << "setting fullscreen: " << m_width << " : " << m_height << std::endl;

						//return;
					}
			} else {
				// this resets the screen to the registry-stored values
				ChangeDisplaySettings(0, CDS_FULLSCREEN);

				// replace the title bar
				DWORD dwstyle = GetWindowLong(m_platformSpecific.m_hWindow, GWL_STYLE);
				dwstyle = dwstyle | WS_CAPTION;
				SetWindowLong(m_platformSpecific.m_hWindow, GWL_STYLE, dwstyle);

				// move the window back to its old position
				//SetWindowPos(m_hWindow, 0, m_oldLoc.x, m_oldLoc.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				//InvalidateRect(m_hWindow, 0, true);

				int s_x = (m_platformSpecific.m_screenResolutionRect.right/2) - (m_platformSpecific.m_windowRect.right/2);
				int s_y = (m_platformSpecific.m_screenResolutionRect.bottom/2) - (m_platformSpecific.m_windowRect.bottom/2) -10;
				//ShowCursor(true);

				if (!SetWindowPos(m_platformSpecific.m_hWindow, HWND_TOP, s_x, s_y, m_platformSpecific.m_windowRect.right - m_platformSpecific.m_windowRect.left, m_platformSpecific.m_windowRect.bottom - m_platformSpecific.m_windowRect.top, SWP_FRAMECHANGED)) {
					ErrorDialog::createAndShow("could not SetWindowPos windowed");
				}

				glViewport(0, 0, m_width, m_height);
				resizeWindowToFitViewport();

				//return;
			}

			this->m_fullscreen = fullscreen;
		}

		void GameContainerPlatform::getWindowFlags(DWORD *windowflags_return, DWORD *exstyle_return, bool undecorated, bool child_window) {
			DWORD exstyle, windowflags;
			if (undecorated) {
				exstyle = WS_EX_APPWINDOW;
				windowflags = WS_POPUP;
			} else if (child_window) {
				exstyle = 0;
				windowflags = WS_CHILDWINDOW;
			} else {
				exstyle = WS_EX_APPWINDOW;
				windowflags = WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
			}
			windowflags = windowflags | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
			*windowflags_return = windowflags;
			*exstyle_return = exstyle;
		}

		void GameContainer::processGamepadInput() {

			JOYINFOEX info;
			MMRESULT result;

			for (unsigned int i = 0; i < m_gamepads.size(); i++) {
				Gamepad* p = m_gamepads.at(i);
				info.dwFlags = JOY_RETURNALL | JOY_RETURNPOVCTS;

				result = joyGetPosEx(p->id, &info);
				if (result == JOYERR_UNPLUGGED) {
					m_gamepads.erase(m_gamepads.begin() + i);
				} else if (result == JOYERR_NOERROR) {

					//handleAxisChange(device, devicePrivate->xAxisIndex, info.dwXpos);

					//if (info.dwXPos < 0) {

					//}


					/*p.rAxisIndex = info.dwRpos;
					p.uAxisIndex = info.dwUpos;
					p.vAxisIndex = info.dwVpos;*/

					unsigned int max = std::numeric_limits<signed short>::max();
					//if (info.dwZpos == max) {
					p->xAxisIndex = info.dwXpos - max;
					p->yAxisIndex = info.dwYpos - max;
					p->zAxisIndex = info.dwZpos - max;


					// update the dpad position for continuous reading.
					unsigned int dpadThisUpdate = Gamepad::DPAD_NONE;

					if (info.dwPOV == 4500) {
						p->dpadPosition = Gamepad::DPAD_UP_RIGHT;
						dpadThisUpdate = Gamepad::DPAD_UP_RIGHT;
						if (!p->isButtonDown(Gamepad::DPAD_UP_RIGHT)) {
							p->pressButton(Gamepad::DPAD_UP_RIGHT);
						}
					} else if (info.dwPOV == 13500) {
						p->dpadPosition = Gamepad::DPAD_DOWN_RIGHT;
						dpadThisUpdate = Gamepad::DPAD_DOWN_RIGHT;
						if (!p->isButtonDown(Gamepad::DPAD_DOWN_RIGHT)) {
							p->pressButton(Gamepad::DPAD_DOWN_RIGHT);
						}
					} else if (info.dwPOV == 22500) {
						p->dpadPosition = Gamepad::DPAD_DOWN_LEFT;
						dpadThisUpdate = Gamepad::DPAD_DOWN_LEFT;
						if (!p->isButtonDown(Gamepad::DPAD_DOWN_LEFT)) {
							p->pressButton(Gamepad::DPAD_DOWN_LEFT);
						}
					} else if (info.dwPOV == 31500) {
						p->dpadPosition = Gamepad::DPAD_UP_LEFT;
						dpadThisUpdate = Gamepad::DPAD_UP_LEFT;
						if (!p->isButtonDown(Gamepad::DPAD_UP_LEFT)) {
							p->pressButton(Gamepad::DPAD_UP_LEFT);
						}
					} else if (info.dwPOV == JOY_POVLEFT) {
						p->dpadPosition = Gamepad::DPAD_LEFT;
						dpadThisUpdate = Gamepad::DPAD_LEFT;
						if (!p->isButtonDown(Gamepad::DPAD_LEFT)) {
							p->pressButton(Gamepad::DPAD_LEFT);
						}
					} else if (info.dwPOV == JOY_POVRIGHT) {
						p->dpadPosition = Gamepad::DPAD_RIGHT;
						dpadThisUpdate = Gamepad::DPAD_RIGHT;
						if (!p->isButtonDown(Gamepad::DPAD_RIGHT)) {
							p->pressButton(Gamepad::DPAD_RIGHT);
						}
					} else if (info.dwPOV == JOY_POVFORWARD) {
						p->dpadPosition = Gamepad::DPAD_UP;
						dpadThisUpdate = Gamepad::DPAD_UP;
						if (!p->isButtonDown(Gamepad::DPAD_UP)) {
							p->pressButton(Gamepad::DPAD_UP);
						}
					} else if (info.dwPOV == JOY_POVBACKWARD) {
						p->dpadPosition = Gamepad::DPAD_DOWN;
						dpadThisUpdate = Gamepad::DPAD_DOWN;
						if (!p->isButtonDown(Gamepad::DPAD_DOWN)) {
							p->pressButton(Gamepad::DPAD_DOWN);
						}
					} else if (info.dwPOV == JOY_POVCENTERED) {
						p->dpadPosition = Gamepad::DPAD_NONE;
						dpadThisUpdate = Gamepad::DPAD_NONE;
						if (!p->isButtonDown(Gamepad::DPAD_NONE)) {
							p->pressButton(Gamepad::DPAD_NONE);
						}
					} else {
						p->dpadPosition = Gamepad::DPAD_NONE;
						dpadThisUpdate = Gamepad::DPAD_NONE;
						if (!p->isButtonDown(Gamepad::DPAD_NONE)) {
							p->pressButton(Gamepad::DPAD_NONE);
						}
					}

					if (!p->isButtonDown(Gamepad::DPAD_NONE)) {// && dpadThisUpdate != Gamepad::DPAD_NONE) {
						p->releaseButton(Gamepad::DPAD_NONE);
					}
					if (p->isButtonDown(Gamepad::DPAD_UP_RIGHT) && dpadThisUpdate != Gamepad::DPAD_UP_RIGHT) {
						p->releaseButton(Gamepad::DPAD_UP_RIGHT);
					}
					if (p->isButtonDown(Gamepad::DPAD_UP) && dpadThisUpdate != Gamepad::DPAD_UP) {
						p->releaseButton(Gamepad::DPAD_UP);
					}
					if (p->isButtonDown(Gamepad::DPAD_RIGHT) && dpadThisUpdate != Gamepad::DPAD_RIGHT) {
						p->releaseButton(Gamepad::DPAD_RIGHT);
					}
					if (p->isButtonDown(Gamepad::DPAD_DOWN_RIGHT) && dpadThisUpdate != Gamepad::DPAD_DOWN_RIGHT) {
						p->releaseButton(Gamepad::DPAD_DOWN_RIGHT);
					}
					if (p->isButtonDown(Gamepad::DPAD_DOWN) && dpadThisUpdate != Gamepad::DPAD_DOWN) {
						p->releaseButton(Gamepad::DPAD_DOWN);
					}
					if (p->isButtonDown(Gamepad::DPAD_DOWN_LEFT) && dpadThisUpdate != Gamepad::DPAD_DOWN_LEFT) {
						p->releaseButton(Gamepad::DPAD_DOWN_LEFT);
					}
					if (p->isButtonDown(Gamepad::DPAD_LEFT) && dpadThisUpdate != Gamepad::DPAD_LEFT) {
						p->releaseButton(Gamepad::DPAD_LEFT);
					}
					if (p->isButtonDown(Gamepad::DPAD_UP_LEFT) && dpadThisUpdate != Gamepad::DPAD_UP_LEFT) {
						p->releaseButton(Gamepad::DPAD_UP_LEFT);
					}


					/*std::cout << " x: " << p.xAxisIndex;
					std::cout << " y: " << p.yAxisIndex;
					std::cout << " z: " << p.zAxisIndex;
					std::cout << std::endl;*/

					//handleButtonChange(device, devicePrivate->lastState.dwButtons, info.dwButtons);
					//handleButtonChange(struct Gamepad_device * device, DWORD lastValue, DWORD value) {

					for (unsigned int buttonIndex = 0; buttonIndex < p->numButtons; buttonIndex++) {
						//if ((p.win32_dwButtons ^ info.dwButtons) & (1 << buttonIndex)) {
							bool b = !!(info.dwButtons & (1 << buttonIndex));

							if (b == true && p->isButtonDown(buttonIndex) == false) {
								p->pressButton(buttonIndex);
							} else if (b == false && p->isButtonDown(buttonIndex) == true) {
								p->releaseButton(buttonIndex);
							}

							//std::cout << "button " << buttonIndex << " is " << p.buttonStates.at(buttonIndex) << std::endl;
						//}
					}



				}
			}
		}

		void GameContainer::setIcon(const std::string& path) {
			m_platformSpecific.m_iconpath = path;
		}

		void GameContainer::initGamepads() {

			//ARK2D::getLog()->i("Initialising Gamepads");

			unsigned int numPadsSupported = joyGetNumDevs();
			unsigned int joystickId = 0;

			if (numPadsSupported == 0) {
				//cout << "Gamepads not supported" << endl;
			} else {
				//cout << numPadsSupported << " gamepads supported." << endl;

				for (unsigned int i = 0; i < numPadsSupported; i++)
				{
					JOYINFOEX info;
					JOYCAPS caps;

					info.dwSize = sizeof(info);
					info.dwFlags = JOY_RETURNALL;

					joystickId = JOYSTICKID1 + i;

					if (joyGetPosEx(joystickId, &info) == JOYERR_NOERROR &&
						joyGetDevCaps(joystickId, &caps, sizeof(JOYCAPS)) == JOYERR_NOERROR) {

						//cout << "Reading gamepad " << joystickId << endl;

						bool duplicate = false;
						for (unsigned int j = 0; j < numPadsSupported; j++) {
#ifdef EXCEPTIONS_AVAILABLE
							try {
								if (m_gamepads.at(j)->id == 0) { }
							} catch (...) {
								continue;
							}
#endif

							//cout << "Duplicate gamepad with id " << i << endl;
							//cout << "id " << m_gamepads[j].id << endl;
							if (m_gamepads[j]->id == joystickId && m_gamepads[j]->initted == true) {
								duplicate = true;
								break;
							}

						}
						if (duplicate) {
							continue;
						}

						//cout << "Adding gamepad with id " << joystickId << endl;

						Gamepad* gamepad = new Gamepad();
						gamepad->id = joystickId;
						gamepad->initted = true;
						gamepad->vendorId = caps.wMid;
						gamepad->productId = caps.wPid;
						gamepad->name = Gamepad::getGamepadDescription(joystickId, info, caps);

						gamepad->numButtons = caps.wNumButtons;
						for (unsigned int j = 0; j < gamepad->numButtons; j++) {
							gamepad->buttonStates.push_back(false);
							//gamepad.buttonDownBuffer[j] = false;
						}

						m_gamepads.push_back(gamepad);

						//cout << "Added gamepad with id " << joystickId << endl;
					}
				}
			}
		}

		void GameContainer::setCursorVisible(bool b) {
			ShowCursor(b);
		}

		void GameContainer::resizeWindowToFitViewport() {
			// This bit resizes the window so that the GL viewport can fit in properly!
			RECT rcClient, rcWind;
			POINT ptDiff;
			GetClientRect(m_platformSpecific.m_hWindow, &rcClient);
			GetWindowRect(m_platformSpecific.m_hWindow, &rcWind);
			ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
			ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
			MoveWindow(m_platformSpecific.m_hWindow ,rcWind.left, rcWind.top, m_width + ptDiff.x, m_height + ptDiff.y, TRUE);

			m_window_rectangle = new GigaRectangle<int>(	rcWind.top,
															rcWind.left,
															(int) m_width,
															(int) m_height); // My Rectangle...
		}
		// Enable OpenGL
		void GameContainerPlatform::enableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC)
		{
			PIXELFORMATDESCRIPTOR pfd;
			int format;

			// get the device context (DC)
			*hDC = GetDC( hWnd );

			// set the pixel format for the DC
			ZeroMemory( &pfd, sizeof( pfd ) );
			pfd.nSize = sizeof( pfd );
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // | PFD_DRAW_TO_WINDOW
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 24;
			pfd.cDepthBits = 16;
			pfd.iLayerType = PFD_MAIN_PLANE;
			format = ChoosePixelFormat( *hDC, &pfd );
			SetPixelFormat( *hDC, format, &pfd );

			// create and enable the render context (RC)
			*hRC = wglCreateContext( *hDC );
			wglMakeCurrent( *hDC, *hRC );

			// initialise that stupid glew crap.
			GLenum err = glewInit();
			if (err != GLEW_OK) {
				//problem: glewInit failed, something is seriously wrong
				char ErrorMessage[100];
				sprintf(ErrorMessage, "Error: %s\n", glewGetErrorString(err));
				return;
			}

			// Setup OGL stuff.
			glEnable(GL_TEXTURE_2D);
			glViewport(0, 0, m_container->m_width, m_container->m_height);
			//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClearColor(m_container->m_clearColor.getRed()/255.0f, m_container->m_clearColor.getGreen()/255.0f, m_container->m_clearColor.getBlue()/255.0f, m_container->m_clearColor.getAlpha()/255.0f);

			//	glViewport(0, 0, 1440, 900);

			glClear( GL_COLOR_BUFFER_BIT );
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			//glEnable(GL_SCISSOR_TEST);
			//glScissor(0, 0, m_container->m_width, m_container->m_height);

			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);



			m_container->m_bRunning = true;

		}


		void* GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
			// Load the DLL and default resources
			HINSTANCE dllModule = LoadLibrary("libARK2D.dll");
			if (dllModule == NULL) {
				std::cout << "bad 1: " << GetLastError() << std::endl;
			}

			HRSRC resourceSrc = FindResource(dllModule, MAKEINTRESOURCE(resourceId), MAKEINTRESOURCE(resourceType));
			if (resourceSrc == NULL) {
				std::cout << "bad 2: " << GetLastError() << std::endl;
			} 
			HGLOBAL resourceData = LoadResource(dllModule, resourceSrc);
			if (resourceData == NULL) {  
				std::cout << "bad 3: " << GetLastError() << std::endl;
			}
			LPVOID resourcePointer = LockResource(resourceData);
			if (resourcePointer == NULL) {
				std::cout << "bad 4: " << GetLastError() << std::endl;
			}

			DWORD resourceSize = SizeofResource(dllModule, resourceSrc);
			char* newPointer = (char*) malloc(resourceSize+1);
			memcpy(newPointer, resourcePointer, resourceSize);

			if (resourceType == ARK2D_RESOURCE_TYPE_TXT || resourceType == ARK2D_RESOURCE_TYPE_FNT) {
				char* newPointerEnd = newPointer+(resourceSize-1);
				newPointerEnd = '\0';
			}

			FreeResource(resourceData); 

			FreeLibrary(dllModule);

			return newPointer;
		}

		void GameContainer::start() {

			//char* fff = (char*) GameContainerPlatform::getARK2DResource(ARK2D_FONT_FNT, TYPE_FNT);
			//std::cout << fff << std::endl;

			//char* fff = (char*) GameContainerPlatform::getARK2DResource(ARK2D_TEST, ARK2D_RESOURCE_TYPE_TXT);
			//string s = string(fff)+ "123";
			//ErrorDialog::createAndShow(s);



			// Display modes!
			// TODO: read the registry... O_O
			GameContainerDisplayMode dm_800x600x32;
			dm_800x600x32.width = 800;
			dm_800x600x32.height = 600;
			dm_800x600x32.bpp = 32;
			m_platformSpecific.m_availableDisplayModes.push_back(dm_800x600x32);


			 // fill out the window class structure
			m_platformSpecific.m_windowClass.cbSize          = sizeof(WNDCLASSEX);
			m_platformSpecific.m_windowClass.style           = CS_OWNDC; //CS_HREDRAW | CS_VREDRAW;
			m_platformSpecific.m_windowClass.lpfnWndProc     = StaticWndProc; //We set our static method as the event handler
			m_platformSpecific.m_windowClass.cbClsExtra      = 0;
			m_platformSpecific.m_windowClass.cbWndExtra      = 0;
			m_platformSpecific.m_windowClass.hInstance       = m_platformSpecific.m_hInstance;
			m_platformSpecific.m_windowClass.hCursor         = LoadCursor(NULL, IDC_ARROW);      // default arrow
			m_platformSpecific.m_windowClass.hbrBackground   = (HBRUSH)GetStockObject( BLACK_BRUSH );	// black background
			m_platformSpecific.m_windowClass.lpszMenuName    = NULL;                             // no menu
			//m_platformSpecific.m_windowClass.lpszClassName   = "GameContainer";
			m_platformSpecific.m_windowClass.lpszClassName   = "GameContainerWindows";
			m_platformSpecific.m_windowClass.hIconSm         = LoadIcon(NULL, IDI_WINLOGO);      // windows logo small icon

			// ICONS?
			//if (true) {
			//	m_windowClass.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_MYICON));
			//} else {
				m_platformSpecific.m_windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);  // default icon
			//}

			if (!RegisterClassEx(&m_platformSpecific.m_windowClass)) {
				MessageBox(NULL, "Could not register window class." , NULL, MB_OK);
				exit(0);
			}

			if (m_fullscreen) {
				memset(&m_platformSpecific.m_dmScreenSettings, 0, sizeof(m_platformSpecific.m_dmScreenSettings));
				m_platformSpecific.m_dmScreenSettings.dmSize = sizeof(m_platformSpecific.m_dmScreenSettings);

				m_platformSpecific.m_dmScreenSettings.dmPelsWidth = m_width;
				m_platformSpecific.m_dmScreenSettings.dmPelsHeight = m_height;
				m_platformSpecific.m_dmScreenSettings.dmBitsPerPel = m_bpp;
				m_platformSpecific.m_dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			}

			//if (m_fullscreen) {
			//	dwExStyle = WS_EX_APPWINDOW;
			//	dwStyle = WS_POPUP | SW_SHOWMAXIMIZED;
			//} else {
			//	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			//	dwStyle = WS_OVERLAPPEDWINDOW;
			//}

			//bool suc = AdjustWindowRectEx(&m_windowRect, dwStyle, false, dwExStyle);
			//if (!suc) {
			//	MessageBox(NULL, "Could not AdjustWindowRectEx()", NULL, MB_OK | MB_ICONEXCLAMATION);
			//	exit(0);
			//}
			//std::cout << m_hInstance << std::endl;
			//AdjustWindowRect(&m_screenResolutionRect, dwStyle, false);


			int s_x = (m_platformSpecific.m_screenResolutionRect.right/2) - (m_platformSpecific.m_windowRect.right/2);
			//int s_y = (m_platformSpecific.m_screenResolutionRect.bottom/2) - (m_platformSpecific.m_windowRect.bottom/2);
			int s_y = (GetSystemMetrics(SM_CYFULLSCREEN)/2) - (m_platformSpecific.m_windowRect.bottom/2);

			s_y -= (GetSystemMetrics(SM_CYBORDER)/2);
			//sy -= GetSystemMetrics(SM_CYSMCAPTION);

			/*RECT clientSize;
			DWORD exstyle, windowflags;
			clientSize.bottom = m_height;
			clientSize.left = 0;
			clientSize.right = m_width;
			clientSize.top = 0;

			getWindowFlags(&windowflags, &exstyle,
				true //undecorated,
				false // child window
			);

			AdjustWindowRectEx(
				&clientSize,    // client-rectangle structure
				windowflags,    // window styles
				false,       // menu-present option
				exstyle   // extended window style
			);
			m_hWindow = CreateWindowEx(
				exstyle,
				"GameContainer",
				m_game.getTitle(),
				windowflags,
				s_x, s_y,
				clientSize.right - clientSize.left, clientSize.bottom - clientSize.top,
				NULL, // parent
				NULL, // menu
				m_hInstance,
				NULL
			);*/

			int windowFlags;
			if (m_resizable) {
				windowFlags = WS_OVERLAPPEDWINDOW;
			} else {
				windowFlags = (WS_BORDER | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
			}

			//m_platformSpecific.m_hWindow = CreateWindow("GameContainer", // lp Class Name
			m_platformSpecific.m_hWindow = CreateWindow("GameContainerWindows", // lp Class Name
										m_game.getTitle().c_str(), // lp Window Name
										windowFlags, // DW Style
										s_x, // x
										s_y, // y
										m_width, //m_windowRect.right - m_windowRect.left, // width
										m_height, // m_windowRect.bottom - m_windowRect.top, // height
										HWND_DESKTOP, // parent window
										NULL, // menu.
										m_platformSpecific.m_hInstance, // instance
										this); // LPVOID?

			//ChangeDisplaySettings(&m_dmScreenSettings, CDS_FULLSCREEN);
			//SetWindowLong(m_hWindow, GWL_STYLE, WS_POPUP | SW_SHOWMAXIMIZED);
			//this->setFullscreen(false);



			if (!m_platformSpecific.m_hWindow) {
				HRESULT r = HRESULT_FROM_WIN32(GetLastError());
				std::cout << HRESULT_CODE(r);
				MessageBox(NULL, "Error at CreateWindowEx()", NULL, MB_OK | MB_ICONEXCLAMATION);
				exit(0);
			}

		//	SetWindowPos(m_hWindow, HWND_TOP, 0, 0, 400,400, 0);

			resizeWindowToFitViewport();


			// Seed the random
			MathUtil::seedRandom();


			// populate the gamepads.
			ARK2D::getLog()->i("Initialising Gamepads... ");
			initGamepads();
			ARK2D::getLog()->i("done.");

			// Enable OpenGL
			ARK2D::getLog()->i("Initialising OpenGL... ");
			m_platformSpecific.enableOpenGL(m_platformSpecific.m_hWindow, &m_platformSpecific.m_hDeviceContext, &m_platformSpecific.m_hRenderingContext);
			this->enable2D();
			ARK2D::getLog()->i("done.");

			// Load default Font - relies on Image so must be done after OpenGL is initted.
			//BMFont* fnt = new BMFont("data/fonts/default.fnt", "data/fonts/default.png");
			//Image* fntImg = new Image((unsigned int) ARK2D_FONT_PNG, ARK2D_RESOURCE_TYPE_PNG);
			BMFont* fnt = new BMFont(ARK2D_FONT_FNT, ARK2D_FONT_PNG, ARK2D_RESOURCE_TYPE_PNG);
			m_graphics.m_DefaultFont = fnt;
			m_graphics.m_Font = fnt;

			// Enable OpenAL
			ARK2D::getLog()->i("Initialising OpenAL... ");
			this->enableOpenAL();
			ARK2D::getLog()->i("done.");



			ARK2D::getLog()->i("Initialising Window... ");
			ShowWindow(m_platformSpecific.m_hWindow, SW_SHOWNORMAL);
			UpdateWindow(m_platformSpecific.m_hWindow);
			//ClipCursor(&m_windowRect);
			ARK2D::getLog()->i("done.");





			ARK2D::getLog()->i("Initialising ");
			ARK2D::getLog()->i(m_game.getTitle());
			ARK2D::getLog()->i("...");
			m_game.init(this);

			ARK2D::getLog()->i("Initialised ");
			ARK2D::getLog()->i(m_game.getTitle());
			ARK2D::getLog()->i("...");


			//LONGLONG cur_time;        // current timer value, moved from inside the loop
			//double delta;         // time elapsed since last frame, moved from inside the loop

			//float t = 0.0f;
			//float dt = 0.017f;
			//float currentTime = 0.0f;
			//float accumulator = 0.0f;

			//unsigned int mySmoothSize = 50;
			//vector<float> mySmoothVector;
			//mySmoothVector.resize(mySmoothSize);
			//unsigned int mySmoothNextInsertPosition = 0;
			//float myDelta = 0.0f;
			//float myCurrentTime = 0.0f;
			//float myLastRenderTime = 0.0f;


			while (this->m_bRunning == true)
			{
				m_timer.tick();

				MSG msg;
				while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // process events one by one.
					if ( msg.message == WM_QUIT ) { this->m_bRunning = false; }
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				processGamepadInput();

				// read appropriate counter
				//if (perf_flag) {
				//   QueryPerformanceCounter((LARGE_INTEGER *) &cur_time);
				//} else {
				//   cur_time = timeGetTime();
				//}
				// scale time value and save
				//delta = (cur_time - last_time) * time_factor;
				//m_fps = (int) (1.f / delta);

				//std::cout << delta << std::endl;
				// save new time reading for next pass through the loop
				//last_time = cur_time;

				//const float newTime = time();
				//float deltaTime = newTime - currentTime;
				//currentTime = newTime;
				//if (deltaTime > 0.25f)
				//	deltaTime = 0.25f;

				//accumulator += deltaTime;

				//while (accumulator>=dt)
				//{
				//	accumulator -= dt;
					//previous = current;
					//integrate(current, t, dt);

				//	t += dt;

				//}

				/*myCurrentTime = this->time();
				myDelta = myCurrentTime - myLastRenderTime;

				if (mySmoothVector.size() > mySmoothSize) {
					mySmoothVector[mySmoothNextInsertPosition] = myDelta;
					mySmoothNextInsertPosition++;
					if (mySmoothNextInsertPosition > mySmoothSize) {
						mySmoothNextInsertPosition = 0;
					}
				} else {
					mySmoothVector.push_back(myDelta);
				}*/

				//float mySmoothTotal = 0.0f;
				//vector<float>::iterator it;
				//for (it = mySmoothVector.begin(); it < mySmoothVector.end(); it++ ) {
				//	myAverageDelta += *it;
				//}
				//for (unsigned int i = 0; i < mySmoothVector.size(); i++) {
				//	mySmoothTotal += mySmoothVector[i];
					//std::cout << i << ": " << mySmoothVector[i] << std::endl;

				//}
				//float myAverageDelta = mySmoothTotal / mySmoothVector.size();
				//(float) myAverageDelta.size();
				//std::cout << myAverageDelta << std::endl;
				//this->m_fps = 1.f / myAverageDelta;


				//std::cout << "D: " << myAverageDelta << std::endl;

				//if (myAverageDelta > dt) {
				//	this->m_game->update(this, dt - myAverageDelta);
				//}
				ARK2D::getLog()->update();

				int delta = (int) (m_timer.getDelta() * 1000);
				m_game.preUupdate(this, &m_timer);
				m_game.update(this, &m_timer);
				m_game.postUpdate(this, &m_timer);
				m_input.clearKeyPressedRecord();
				for (unsigned int i = 0; i < m_gamepads.size(); i++) {
					m_gamepads.at(i)->clearButtonPressedRecord();
				}
				//this->m_game->update(this, myAverageDelta); // fix at 60 fps. bug.
				//this->m_game->update(this, 0.017); // fix at 60 fps. bug.


				//m_lastTime = GetTickCount() / 1000.0f;
				//std::cout << this->getGame()->getTitle().c_str() << std::endl;

				//float elapsedTime = this->getElapsedMilliseconds();
				//this->m_game->update(this, (17 / 1000.f)); // fix at 60 fps. bug.
				//this->m_game->update(this, (float) (1.f / 60.f)); // fix at 60 fps. bug.
				//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				m_game.preRender(this, &m_graphics);
				m_game.render(this, &m_graphics);
				m_game.postRender(this, &m_graphics);
				if (m_showingFPS) { renderFPS(); }

				ARK2D::getLog()->render();
				//myLastRenderTime = this->time();

				swapBuffers();

				Sleep(delta/2);
			}

			// shutdown OpenGL
			this->disable2D();
			m_platformSpecific.disableOpenGL( m_platformSpecific.m_hWindow, m_platformSpecific.m_hDeviceContext, m_platformSpecific.m_hRenderingContext );

			ARK2D::getLog()->i("Deinitialised OpenGL...");

			// destroy the window explicitly
			DestroyWindow( m_platformSpecific.m_hWindow );
			ARK2D::getLog()->i("Deinitialised Window...");
		}



		void GameContainer::close() const {
			PostMessageA(m_platformSpecific.m_hWindow, WM_QUIT, 0, 0);
		}


		void GameContainer::swapBuffers() {
			SwapBuffers(m_platformSpecific.m_hDeviceContext);
		}

		int GameContainer::getGlobalMouseX() const {
			DWORD mousepos = GetMessagePos();
			POINTS mouseXY = MAKEPOINTS(mousepos);
			return mouseXY.x;
		}
		int GameContainer::getGlobalMouseY() const {
			DWORD mousepos = GetMessagePos();
			POINTS mouseXY = MAKEPOINTS(mousepos);
			return mouseXY.y;
		}

		// Disable OpenGL
		void GameContainerPlatform::disableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
		{
			wglMakeCurrent( NULL, NULL );
			wglDeleteContext( hRC );
			ReleaseDC( hWnd, hDC );
		}

		bool GameContainerPlatform::initOpenAL() {
			alutInit(0, NULL);
			alGetError();
			return true;
		}
		bool GameContainerPlatform::deinitOpenAL() {
			return true;
		}

		void GameContainerPlatform::setTitle(std::string title) {
			SetWindowTextA(m_hWindow, title.c_str());
		}

		string GameContainerPlatform::getResourcePath() const {
			return "./";
		}

#endif
