/*
 * GameContainerWindows.cpp
 *
 *  Created on: 27 Feb 2011
 *      Author: ashley
 */ 
  
#include "GameContainerWindows.h"
#include "../GameContainer.h" 
#include "../../ARK2D.h"

#include "../../Namespaces.h"
#include "../../Includes.h"

#include "../../Geometry/Shape.h"
#include "../../Geometry/Circle.h"
#include "../../Geometry/Line.h"
#include "../../Geometry/Rectangle.h"


#if defined(ARK2D_WINDOWS)

	#include "../../Geometry/GigaRectangle.h"

	#include "../../Windres.h"
	#include "../../Graphics/Image.h" 
	#include "../../Util/Log.h"
 	#include "../../Util/Callbacks.h"
	
	#include <VersionHelpers.h>

 			

	namespace ARK {
		namespace Core {

			// This function is used to return an index given an ID
			int GameContainerPlatform::__GetContactIndex(int dwID){
				for (int i = 0; i < MAXPOINTS; i++){
					if (idLookup[i] == -1){
						idLookup[i] = dwID;
						return i;
					}
					else{
						if (idLookup[i] == dwID){
							return i;
						}
					}
				}
				// Out of contacts
				return -1;
			}

			bool GameContainerPlatform::mySetDisplayAutoRotationPreferences(MY_ORIENTATION_PREFERENCE pref) {
				//#if (WINVER <= 0x0601)
					MySetDisplayAutoRotationPreferences = (pSDARP) GetProcAddress(GetModuleHandle(TEXT("user32.dll")), "SetDisplayAutoRotationPreferences");
					if(MySetDisplayAutoRotationPreferences)
					{
						return MySetDisplayAutoRotationPreferences(pref);
					}
				//#else // #if (WINVER > 0x0601)
				//	if (IsWindows8OrGreater()) {
				//		return SetDisplayAutoRotationPreferences(pref);
				//	}
				//	return false;
				//#endif
					return false;
			}
			bool GameContainerPlatform::myGetDisplayAutoRotationPreferences(MY_ORIENTATION_PREFERENCE* pref) {
				MyGetDisplayAutoRotationPreferences = (pSDARP2)GetProcAddress(GetModuleHandle(TEXT("user32.dll")), "GetDisplayAutoRotationPreferences");
				if (MyGetDisplayAutoRotationPreferences)
				{
					return MyGetDisplayAutoRotationPreferences(pref);
				}
				return false;
				//MyGetDisplayAutoRotationPreferences
			}
			bool GameContainerPlatform::myGetAutoRotationState(AR_STATE* state) {
				MyGetAutoRotationState = (pSDARP3)GetProcAddress(GetModuleHandle(TEXT("user32.dll")), "GetAutoRotationState");
				if (MyGetAutoRotationState)
				{
					return MyGetAutoRotationState(state);
				}
				return false;
				//MyGetDisplayAutoRotationPreferences
			}
			bool GameContainerPlatform::myRegisterTouchWindow(HWND wnd, ULONG flags) {
				MyRegisterTouchWindow = (pSDARP4)GetProcAddress(GetModuleHandle(TEXT("user32.dll")), "RegisterTouchWindow");
				if (MyRegisterTouchWindow) {
					return MyRegisterTouchWindow(wnd, flags);
				}
				return false;
			}
			// https://gist.github.com/paulhoux/1341083
			bool GameContainerPlatform::myGetTouchInputInfo(HTOUCHINPUT hTouchInput, UINT cInputs, PTOUCHINPUT pInputs, int cbSize) {
				MyGetTouchInputInfo = (pGetTouchInputInfo) GetProcAddress(GetModuleHandle(TEXT("user32.dll")), "GetTouchInputInfo");
				if (MyGetTouchInputInfo) {
					return MyGetTouchInputInfo(hTouchInput, cInputs, pInputs, cbSize);
				}
				return false;
			}
			bool GameContainerPlatform::myCloseTouchInputHandle(HTOUCHINPUT hTouchInput) {
				MyCloseTouchInputHandle = (pCloseTouchInputHandle) GetProcAddress(GetModuleHandle(TEXT("user32.dll")), "CloseTouchInputHandle");
				if (MyCloseTouchInputHandle) {
					return MyCloseTouchInputHandle(hTouchInput);
				}
				return false;
			}


			LRESULT CALLBACK GameContainerPlatform::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
			{
				switch (message)
				{
					case WM_CREATE:
					{
						ARK2D::getLog()->v("WM_CREATE");
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
						//m_container->resizeBehaviour((int) LOWORD(lParam), (int) HIWORD(lParam));
						ARK2D::getLog()->v("WM_SIZE");
						if (m_container->m_bRunning) { 
							m_container->m_width =  LOWORD(lParam);
							m_container->m_height = HIWORD(lParam);
							
							#if defined(ARK2D_RENDERER_OPENGL)
								if (m_container->m_scaleToWindow) {
									glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
								} else {
									m_container->disable2D();
									m_container->enable2D();
									glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
								}
							#elif defined(ARK2D_RENDERER_DIRECTX)
								ARK2D::getLog()->v("DX WM_SIZE event.");
								//UINT width = LOWORD(lParam);
                    			//UINT height = HIWORD(lParam);
								//m_pRenderTarget->Resize(D2D1::SizeU(width, height));
							#endif
					
							m_container->m_game.resize(m_container, LOWORD(lParam), HIWORD(lParam));
						}
						return 0;
					}
					case WM_CLOSE:
					{
						ARK2D::getLog()->v("WM_CLOSE");
						PostQuitMessage( 0 );
						return 0;
					}
					case WM_DESTROY:
					{
						ARK2D::getLog()->v("WM_DESTROY");
						return 0;
					}
					// Orientation changed. e.g. device rotated or right-alt & arrow pressed.
					case WM_DISPLAYCHANGE:
					{
						ARK2D::getLog()->v("WM_DISPLAYCHANGE event.");

						{
							DEVMODE devmode;
							ZeroMemory(&devmode, sizeof(DEVMODE));
							devmode.dmSize = sizeof(DEVMODE);
							devmode.dmFields = DM_DISPLAYORIENTATION;
							EnumDisplaySettingsEx(NULL, ENUM_CURRENT_SETTINGS, &devmode, EDS_RAWMODE); 

							if (devmode.dmDisplayOrientation == DMDO_90 || devmode.dmDisplayOrientation == DMDO_270) {
								// Portrait

							} else if (devmode.dmDisplayOrientation == DMDO_DEFAULT || devmode.dmDisplayOrientation == DMDO_180) {
								// Landscape
							}
						}

						m_container->m_screenOrientationPrevious = m_container->getScreenOrientation();

						refreshScreenResolutionRect();
						unsigned int currentScreenOrientation = m_container->getScreenOrientation();

						if (m_container->m_screenOrientationPrevious != currentScreenOrientation) {
							Callbacks::invoke(Callbacks::CALLBACK_WINDOWS_ORIENTATION_CHANGED);
							
							ARK2D::getGame()->orientationChanged(currentScreenOrientation);
						}

						/*int newWidth = LOWORD(lParam);
						int newHeight = HIWORD(lParam);
						m_container->m_width =  newWidth;
						m_container->m_height = newHeight;

						#if defined(ARK2D_RENDERER_OPENGL)
							glViewport(0, 0, newWidth, newHeight);
						#endif
						m_container->m_game.resize(m_container, newWidth, newHeight);*/
							if (m_container->isFullscreen()) {
								m_container->setSize(m_container->getScreenWidth(), m_container->getScreenHeight(), true);
							}

						return 0;
					}
					// Desktop/tablet mode changed.
					case WM_SETTINGCHANGE:
					{
						ARK2D::getLog()->v("WM_SETTINGCHANGE");
						TCHAR* settingChangeType = (TCHAR*) lParam;
						if (settingChangeType != NULL && strcmp(settingChangeType, "ConvertibleSlateMode") == 0) { // _tcscmp
							ARK2D::getLog()->v("ConvertibleSlateMode");
							int val = GetSystemMetrics(SM_CONVERTIBLESLATEMODE);	
							if (val == 0) {
								switchToTabletMode(true);
							} else {
								switchToDesktopMode(true);
							}
							m_container->m_2in1enabled = true;
							return 0;
						} else if (settingChangeType != NULL && strcmp(settingChangeType, "Desktop") == 0) { // _tcscmp
							ARK2D::getLog()->v("Desktop message change");
							// wParam LOWORD: 24
							// wParam HIWORD: 0

							// Window was docked... like Windows 8 docked.
							// An SPI_SETWORKAREA comes after this one.
							return 0;
						} else {
							if (settingChangeType != NULL) { 
								ARK2D::getLog()->v(settingChangeType);
							} else {
								unsigned int type = LOWORD(wParam);
								ARK2D::getLog()->v(StringUtil::append("WPARAM: ", type));
								if (type == SPI_SETWORKAREA) {
									if (m_container->isFullscreen()) {
										refreshScreenResolutionRect();
										m_container->m_width =  m_container->m_screenWidth;
										m_container->m_height = m_container->m_screenHeight;
										//setFullscreen_Fake(true);
										//m_container->m_game.resize(m_container, m_container->m_width, m_container->m_height);

										// Not really the best thing to do. Perhaps refactor resize() and orientationchange...?
										ARK2D::getGame()->orientationChanged(m_container->getScreenOrientation());

										return 0;
									}

								}
							
							}
						}
						
						
						return DefWindowProc(hWnd, message, wParam, lParam);
					}
					case WM_NCCALCSIZE:
					{
						ARK2D::getLog()->v("WM_SETTINGCHANGE");	
						return DefWindowProc(hWnd, message, wParam, lParam);
					}
					case WM_TOUCH: 
					{    
					  	int wmId, wmEvent, i, x, y;
						UINT cInputs;
						PTOUCHINPUT pInputs;
						POINT ptInput;   

						//int MAXPOINTS = 10;
						bool bHandled = false;
						int index = 0;

						cInputs = LOWORD(wParam);
					  	pInputs = new TOUCHINPUT[cInputs];
						if (pInputs){
					    	if (myGetTouchInputInfo((HTOUCHINPUT)lParam, cInputs, pInputs, sizeof(TOUCHINPUT))){
					      		for (int i=0; i < static_cast<INT>(cInputs); i++) {
					        		TOUCHINPUT ti = pInputs[i];
					        		index = __GetContactIndex(ti.dwID);
									//if (index != 0) {
									//	continue;
									//}
					        		if (ti.dwID != 0 && index < MAXPOINTS){                            
					   					// Do something with your touch input handle
					          			ptInput.x = TOUCH_COORD_TO_PIXEL(ti.x);
					         			ptInput.y = TOUCH_COORD_TO_PIXEL(ti.y);
					          			ScreenToClient(hWnd, &ptInput);

					          			float thisx = ptInput.x;
										float thisy = ptInput.y;

										if (m_container->isOrientationInverted()) {
											MathUtil::rotatePointAroundPoint<float>(&thisx, &thisy, 0, 0, 90);
											thisx -= (m_container->getDynamicHeight()*-1.0f);
										}

									    thisx -= m_container->getTranslateX();
									    thisy -= m_container->getTranslateY();

									    thisx /= m_container->getScale();
									    thisy /= m_container->getScale();

										Input* in = ARK2D::getInput(); 
										//ARK2D::getLog()->e(StringUtil::append("index: ", index));
										//ARK2D::getLog()->e(StringUtil::append("ti.dwID: ", ti.dwID));
										
					          			if (ti.dwFlags & TOUCHEVENTF_DOWN) {
											int fingerID = in->getTouchByInternalData(ti.dwID);
											if (fingerID == -1) {
								          		ARK::Controls::Pointer p;
												p.x = thisx;
												p.y = thisy;
												p.data = ti.dwID;
												fingerID = in->addTouch(p);
												//ARK2D::getLog()->v("add touch point");

												if (in->countTouches() == 1) {
													in->mouse_x = (int) thisx;
													in->mouse_y = (int) thisy;
													m_container->m_input.pressKey(Input::MOUSE_BUTTON_LEFT);
												}
											}
										} 
										if (ti.dwFlags & TOUCHEVENTF_UP) {
							            	// 2-in-one handling: if in desktop mode, switch to touch mode. 
							            	// Your game must handle the case where player taps to confirm state change 
							            	// but actually wants desktop mode.
							            	//switchToTabletMode(true);
							            	m_container->setTouchMode(true);


							            	int fingerID = in->getTouchByInternalData(ti.dwID);
											if (fingerID != -1) {
												// mimic mouse
												if (fingerID == 0) {
													in->mouse_x = (int) thisx;
													in->mouse_y = (int) thisy;
													m_container->m_input.releaseKey(Input::MOUSE_BUTTON_LEFT);
												}
												in->removeTouch(fingerID);
								               // ARK2D::getLog()->v("remove touch point");
											} 
										}
										if (ti.dwFlags & TOUCHEVENTF_MOVE) {
											// found a touch. Is it already on our list?
											int fingerID = in->getTouchByInternalData(ti.dwID);
											if (fingerID != -1) {

												//ARK2D::getLog()->v("touch moved");
												// mimic mouse
												if (fingerID == 0) {
													in->mouse_x = (int) thisx;
													in->mouse_y = (int) thisy;
													//ARK2D::getLog()->e(StringUtil::append("touch moved thisx: ", thisx));
													//ARK2D::getLog()->e(StringUtil::append("touch moved thisy: ", thisy));

													ARK2D::getLog()->mouseMoved((int)thisx, (int)thisy, in->mouse_x, in->mouse_y);
													ARK2D::getGame()->mouseMoved((int)thisx, (int)thisy, in->mouse_x, in->mouse_y);
												}
												in->m_touchPointers[fingerID].x = thisx;
												in->m_touchPointers[fingerID].y = thisy;
												in->m_touchPointers[fingerID].data = ti.dwID;
											}
								            
								            
										}
										


										
					        		}
					        		//break;
					      		}
					      		bHandled = true;
					    	}
					    	// If you handled the message and don't want anything else done with it, you can close it
					    	myCloseTouchInputHandle((HTOUCHINPUT)lParam);
					    	delete [] pInputs;
						} else {
							// Handle the error here 
						}
						if (bHandled) {
							return 0;
						}
						return DefWindowProc(hWnd, message, wParam, lParam);

					}
					case WM_KEYDOWN:
					case WM_SYSKEYDOWN:
					{
						if (wParam == VK_MENU) {
							if (lParam & (1 << 24)) {
								m_container->m_input.pressKey(Input::KEY_ALT); // RIGHT ALT
							} else {
								m_container->m_input.pressKey(Input::KEY_ALT); // LEFT ALT
							}
						} else if (wParam == VK_CONTROL) {
							if (lParam & (1 << 24)){
								m_container->m_input.pressKey(Input::KEY_RCONTROL); // RIGHT CTRL
							} else {
								m_container->m_input.pressKey(Input::KEY_LCONTROL); // LEFT CTRL
							}
						} else if (wParam == VK_SHIFT) {
							m_container->m_input.pressKey(Input::KEY_LSHIFT);
						} else {
							//ARK2D::getLog()->w(StringUtil::append("pressed key: ", wParam));
							m_container->m_input.pressKey(wParam);
						} 
						break;
					} 
					case WM_KEYUP:
					case WM_SYSKEYUP:
					{
						if (wParam == VK_MENU) {
							if (lParam & (1 << 24)) {
								m_container->m_input.releaseKey(Input::KEY_ALT); // RIGHT ALT
							} else {
								m_container->m_input.releaseKey(Input::KEY_ALT); // LEFT ALT
							}
						} else if (wParam == VK_CONTROL) {
							if (lParam & (1 << 24)){
								m_container->m_input.releaseKey(Input::KEY_RCONTROL); // RIGHT CTRL
							} else {
								m_container->m_input.releaseKey(Input::KEY_LCONTROL); // LEFT CTRL
							}
						} else if (wParam == VK_SHIFT) {
							m_container->m_input.releaseKey(Input::KEY_LSHIFT);
						} else {
							m_container->m_input.releaseKey(wParam);
						}
						break;
					}
					case WM_ACTIVATEAPP:
					{
						ARK2D::getLog()->v("WM_ACTIVATEAPP");
						break;
					}

					case WM_MOUSEMOVE:
					{
						
						/*
						ARK2D::getGame()->mouseMoved(mouseXY.x, mouseXY.y, m_container->m_input.mouse_x, m_container->m_input.mouse_y);

						m_container->m_input.mouse_x = mouseXY.x;
						m_container->m_input.mouse_y = mouseXY.y;
						break;*/

						POINTS mouseXY = MAKEPOINTS(lParam);

						float thisx = mouseXY.x;
					    float thisy = mouseXY.y;

					    if (m_container->isOrientationInverted()) {
							MathUtil::rotatePointAroundPoint<float>(&thisx, &thisy, 0, 0, 90);
							thisx -= (m_container->getDynamicHeight()*-1.0f);
						}

					    thisx -= m_container->getTranslateX();
					    thisy -= m_container->getTranslateY();

					    thisx /= m_container->getScale();
					    thisy /= m_container->getScale();

						Input* i = ARK2D::getInput();
						ARK2D::getLog()->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);
						ARK2D::getGame()->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);

						i->mouse_x = (int) thisx;
						i->mouse_y = (int) thisy;

						break;
					}
					case WM_LBUTTONDOWN:
					{
						// Don't process if it's a touch event.
						if (GetMessageExtraInfo() == 0) {
							m_container->m_input.pressKey(Input::MOUSE_BUTTON_LEFT);
						}
						break; 
					}
					case WM_LBUTTONUP:
					{
						// Don't process if it's a touch event.
						if (GetMessageExtraInfo() == 0) {
							m_container->m_input.releaseKey(Input::MOUSE_BUTTON_LEFT);
							
							// 2-in-one handling:if in tablet mode, switch to desktop mode.
							//switchToDesktopMode(true); 
							m_container->setTouchMode(false);
						}
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

			void GameContainerPlatform::switchToTabletMode(bool doCallback) {
				if (m_twoInOneState == TWOINONESTATE_UNKNOWN || m_twoInOneState == TWOINONESTATE_DESKTOP) {
					ARK2D::getLog()->v("Slate mode activated");
					m_twoInOneState = TWOINONESTATE_TABLET;
					m_container->setTouchMode(true);
					if (doCallback) {
						Callbacks::invoke(Callbacks::CALLBACK_WINDOWS_TABLETMODE_STARTED);
					}
				}
			}
			void GameContainerPlatform::switchToDesktopMode(bool doCallback) {
				if (m_twoInOneState == TWOINONESTATE_UNKNOWN || m_twoInOneState == TWOINONESTATE_TABLET) {
					ARK2D::getLog()->v("Desktop mode activated");
					m_twoInOneState = TWOINONESTATE_DESKTOP;
					m_container->setTouchMode(false);
					if (doCallback) {
						Callbacks::invoke(Callbacks::CALLBACK_WINDOWS_DESKTOPMODE_STARTED);
					}
				}
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

				m_platformSpecific.m_hInstance = GetModuleHandle(NULL);

				ARK2D::s_container = this;
				ARK2D::s_game = &m_game;
				ARK2D::s_graphics = &m_graphics;
				ARK2D::s_input = &m_input;
				ARK2D::s_log = ARK::Util::Log::getInstance();

				m_platformSpecific.m_windowRect.left = (long) 0; 			// Set Left Value To 0
				m_platformSpecific.m_windowRect.right = (long) width; 		// Set Right Value To Requested Width
				m_platformSpecific.m_windowRect.top = (long) 0;  			// Set Top Value To 0
				m_platformSpecific.m_windowRect.bottom = (long) height;   	// Set Bottom Value To Requested Height

				m_platformSpecific.refreshScreenResolutionRect();

				m_platformSpecific.m_twoInOneState = GameContainerPlatform::TWOINONESTATE_UNKNOWN;
				m_platformSpecific.m_hotplugGamepadsTimer = 0.0f;
				

				ARK2D::getRenderer()->preinit();
			}

			void GameContainerPlatform::refreshScreenResolutionRect() {

				int fsx = GetSystemMetrics(SM_CXFULLSCREEN);
				int fsy = GetSystemMetrics(SM_CYSCREEN);

				m_screenResolutionRect.left = (long) 0;
				m_screenResolutionRect.right = (long) fsx; 		// Set Right Value To Requested Width
				m_screenResolutionRect.top = (long) 0;  			// Set Top Value To 0
				m_screenResolutionRect.bottom = (long) fsy;   	// Set Bottom Value To Requested Height

				m_container->m_screenWidth = m_screenResolutionRect.right;
				m_container->m_screenHeight = m_screenResolutionRect.bottom;

				RECT tempRect; 
				SystemParametersInfo(SPI_GETWORKAREA, 0, &tempRect, 0);
				//ARK2D::getLog()->v(StringUtil::append("x: ", tempRect.left));
				//ARK2D::getLog()->v(StringUtil::append("y: ", tempRect.top));
				//ARK2D::getLog()->v(StringUtil::append("w: ", tempRect.right));
				//ARK2D::getLog()->v(StringUtil::append("h: ", tempRect.bottom));
				m_screenStartX = tempRect.left;
				m_screenStartY = tempRect.top;

				// TODO: 
				// screenStartX and screenStartY are wrong on desktops with side/top/right start bars...
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

			void GameContainer::setSize(int width, int height, bool docallback) {
				
				if (width == (signed int) m_width && height == (signed int) m_height) { return; }

				resizeBehaviour(width, height, docallback);

				resizeWindowToFitViewport();
			}

			void GameContainer::setFullscreen(bool fullscreen) {

				m_platformSpecific.setFullscreen_Fake(fullscreen);
			}

			WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };

			void GameContainerPlatform::setFullscreen_Fake(bool fullscreen) 
			{
				DWORD dwStyle = GetWindowLong(m_hWindow, GWL_STYLE);
				if (fullscreen) {  
					MONITORINFO mi = { sizeof(mi) };
					if (GetWindowPlacement(m_hWindow, &g_wpPrev) && GetMonitorInfo(MonitorFromWindow(m_hWindow, MONITOR_DEFAULTTOPRIMARY), &mi) ) {
					
						dwStyle &= ~WS_CAPTION;
						dwStyle &= ~WS_OVERLAPPEDWINDOW;
 
						SetWindowLongPtr(m_hWindow, GWL_STYLE, dwStyle);
						SetWindowPos(m_hWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED); //  
						//SetWindowPos(m_hWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED); //  
						//SetWindowPos(m_hWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED); //  

						m_container->resizeBehaviour((int) m_container->m_screenWidth, m_container->m_screenHeight);
						m_container->resizeWindowToFitViewport(); 

					}
				} else {
					int s_x = (m_screenResolutionRect.right/2) - (m_windowRect.right/2);
					int s_y = (GetSystemMetrics(SM_CYFULLSCREEN)/2) - (m_windowRect.bottom/2) - (GetSystemMetrics(SM_CYBORDER)/2);
					int s_w = m_windowRect.right - m_windowRect.left;
					int s_h = m_windowRect.bottom - m_windowRect.top;

					dwStyle |= WS_BORDER;
					dwStyle |= WS_OVERLAPPED;
					dwStyle |= WS_CAPTION; 
					dwStyle |= WS_SYSMENU;
					dwStyle |= WS_MINIMIZEBOX; 
					dwStyle |= WS_CLIPCHILDREN;
					dwStyle |= WS_CLIPSIBLINGS;

					SetWindowLongPtr(m_hWindow, GWL_STYLE, dwStyle);
					SetWindowPos(m_hWindow, NULL, s_x, s_y, s_w, s_h, SWP_FRAMECHANGED);//SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

					m_container->resizeBehaviour((int) m_container->m_originalWidth, (int) m_container->m_originalHeight); 
					m_container->resizeWindowToFitViewport();
				}
				m_container->m_fullscreen = fullscreen;
			}
			void GameContainerPlatform::setFullscreen_ScreenMode(bool fullscreen) 
			{
				if (fullscreen) {
					// turn off window region without redraw
						//SetWindowRgn(m_hWindow, 0, false);
						DEVMODE newSettings;

						// request current screen settings
						EnumDisplaySettings(0, 0, &newSettings);

						// Find a display mode closest to the width and height
						GameContainerDisplayMode* dm = &m_availableDisplayModes.at(0);
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
							DWORD dwstyle = GetWindowLong(m_hWindow, GWL_STYLE);
							dwstyle &= ~WS_CAPTION;
							SetWindowLong(m_hWindow, GWL_STYLE, dwstyle);

							// move the window to (0,0)
							SetWindowPos(m_hWindow, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
							InvalidateRect(m_hWindow, 0, true);

							MoveWindow(m_hWindow,0 , 0, dm->width, dm->height, TRUE);
							//glViewport(0, 0, m_width, m_height);
							//glViewport(0, 0, dm->width, dm->height);

							m_container->resizeBehaviour((int) dm->width, (int) dm->height);


							std::cout << "setting fullscreen: " << m_container->m_width << " : " << m_container->m_height << std::endl;

							//return;
						}
				} else {
					// this resets the screen to the registry-stored values
					ChangeDisplaySettings(0, CDS_FULLSCREEN);

					// replace the title bar
					DWORD dwstyle = GetWindowLong(m_hWindow, GWL_STYLE);
					dwstyle = dwstyle | WS_CAPTION;
					SetWindowLong(m_hWindow, GWL_STYLE, dwstyle);

					// move the window back to its old position
					//SetWindowPos(m_hWindow, 0, m_oldLoc.x, m_oldLoc.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
					//InvalidateRect(m_hWindow, 0, true);

					int s_x = (m_screenResolutionRect.right/2) - (m_windowRect.right/2);
					int s_y = (m_screenResolutionRect.bottom/2) - (m_windowRect.bottom/2) -10;
					//ShowCursor(true);

					if (!SetWindowPos(m_hWindow, HWND_TOP, s_x, s_y, m_windowRect.right - m_windowRect.left, m_windowRect.bottom - m_windowRect.top, SWP_FRAMECHANGED)) {
						ErrorDialog::createAndShow("could not SetWindowPos windowed");
					}

					//glViewport(0, 0, m_width, m_height);
					m_container->resizeBehaviour((int) m_container->m_width, (int) m_container->m_height); 
					
					m_container->resizeWindowToFitViewport();

					//return;
				}

				m_container->m_fullscreen = fullscreen;
			}
			
			bool GameContainerPlatform::isRetina() {
				return false;
			}

			int GameContainerPlatform::getWindowFlags2() {
				int windowFlags;
				if (m_container->m_resizable) {
					windowFlags = WS_OVERLAPPEDWINDOW;
				} else {
					windowFlags = (WS_BORDER | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
				}
				return windowFlags;
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

			static void __handleAxisChange(Gamepad* gamepad, int axisIndex, unsigned int value) {
				
				if (axisIndex < 0 || axisIndex >= (int) gamepad->numAxes) {// || axisIndex >= (int) gamepad->axes.size()) {
					ARK2D::getLog()->i(StringUtil::append("Bit weird... ", axisIndex));
					return;
				}


				
				int newAxisIndex = Gamepad::convertSystemAxisToARKAxis(gamepad, axisIndex);
				ARK2D::getLog()->e(StringUtil::append("axis change: ", axisIndex));
				ARK2D::getLog()->e(StringUtil::append("axis change 2: ", newAxisIndex));

				// Windows multimedia api maps xbox 360 triggers on to A SINGLE AXIS LIKE A FUCKING IDIOT.
				// It's fixed in Xinput apis but that rquires a Visual Studio compiler set up. :( 
				float trigger1RawValue = 0.0f; 
				if (gamepad->m_sharedTriggerAxis) { 
					if (axisIndex == Gamepad::TRIGGER_2) {
						for(unsigned int i = 0; i < gamepad->axes.size(); ++i) {
							if (gamepad->axes.at(i)->id == newAxisIndex) { 
								trigger1RawValue = gamepad->axes.at(i)->value;
							} 
						}
					}
				}
			
				GamepadAxis* axis = NULL;
				for (signed int i = 0; i < gamepad->numAxes /* && i < gamepad->axes.size()*/; ++i) {
					axis = gamepad->axes.at(i);
					if (axis->id == (unsigned int)newAxisIndex) { break; }
				}

				if (axis != NULL) { 

					float triggerValue = (value - axis->rangeMin) / (float) (axis->rangeMax - axis->rangeMin) * 2.0f - 1.0f;

					// not a shared axis. axis values go from -1.0f to +1.0f sp sort this out.
					if (!gamepad->m_sharedTriggerAxis) { 
						if (axisIndex == Gamepad::TRIGGER_1 || axisIndex == Gamepad::TRIGGER_2) { 
							triggerValue = (triggerValue + 1.0f) / 2.0f;
						}
					} else { 
						if (newAxisIndex == Gamepad::TRIGGER_1 || newAxisIndex == Gamepad::TRIGGER_2) {

							
							GamepadAxis* leftAxis = NULL;
							for (signed int i = 0; i < gamepad->numAxes ; ++i) {
								GamepadAxis* thisaxis = gamepad->axes.at(i);
								if (thisaxis->id == Gamepad::TRIGGER_1) { 
									leftAxis = thisaxis;
									break;
								}
							}


							ARK2D::getLog()->e(StringUtil::appendf("axis val o: ", triggerValue));
							float epsilon = 0.0001f;
							if (triggerValue >= epsilon) {
								axis = leftAxis;
								newAxisIndex = Gamepad::TRIGGER_1;
								ARK2D::getLog()->e(StringUtil::append("axis change 3 (left trigger): ", newAxisIndex));
							} else if (triggerValue < epsilon*-1.0f){
								triggerValue *= -1.0f;
							} else {
								triggerValue = 0.0f;

								// we have to set the left axis to zero in this circumstance too. 
								gamepad->moveAxis(Gamepad::TRIGGER_1, 0.0f);
							}
						} 
					}

					gamepad->moveAxis(axis->id, triggerValue);

					return; 
					
				} 

				ARK2D::getLog()->i(StringUtil::append("Could not handle axis change for index: ", axisIndex));
				//axisEvent.axisID = axisIndex;
				//axisEvent.value = 
			}

			void GameContainer::processGamepadInput() {

				JOYINFOEX info;
				MMRESULT result;

				for( unsigned int i = 0; i < m_gamepads.size(); ++i) {
					m_gamepads.at(i)->update();
				}

				for (unsigned int i = 0; i < m_gamepads.size(); i++) {
					Gamepad* p = m_gamepads.at(i);
					info.dwFlags = JOY_RETURNALL | JOY_RETURNPOVCTS;

					result = joyGetPosEx(p->id, &info);
					if (result == JOYERR_UNPLUGGED) {
						m_gamepads.erase(m_gamepads.begin() + i);

						// Give event to game
						Game* g = ARK2D::getGame();
						GamepadListener* gl = NULL;
						gl = dynamic_cast<GamepadListener*>(g);
						if (gl != NULL) {
							gl->gamepadDisconnected(p);
						}
						//i = 0;

					} else if (result == JOYERR_NOERROR) {

						//handleAxisChange(device, devicePrivate->xAxisIndex, info.dwXpos);

						//if (info.dwXPos < 0) {

						//}


						/*p.rAxisIndex = info.dwRpos;
						p.uAxisIndex = info.dwUpos;
						p.vAxisIndex = info.dwVpos;*/

						//unsigned int max = std::numeric_limits<signed short>::max();
						//if (info.dwZpos == max) {
						//p->xAxisIndex = info.dwXpos - max;  
						//p->yAxisIndex = info.dwYpos - max; 
						//p->zAxisIndex = info.dwZpos - max;

						if (info.dwXpos != p->lastState.dwXpos) {
							__handleAxisChange(p, Gamepad::ANALOG_STICK_1_X, info.dwXpos);
						}
						if (info.dwYpos != p->lastState.dwYpos) {
							__handleAxisChange(p, Gamepad::ANALOG_STICK_1_Y, info.dwYpos); 
						}
						

						if (info.dwZpos != p->lastState.dwZpos) {     
							//	ARK2D::getLog()->i(StringUtil::append("ltrigger changed: ", info.dwZpos));
							//	ARK2D::getLog()->i(StringUtil::append("r pos: ", info.dwRpos));
							//	ARK2D::getLog()->i(StringUtil::append("u pos: ", info.dwUpos));  
							//	ARK2D::getLog()->i(StringUtil::append("v pos: ", info.dwVpos));
							__handleAxisChange(p, Gamepad::ANALOG_STICK_2_X, info.dwZpos);
						}

						if (info.dwRpos != p->lastState.dwRpos) {
							__handleAxisChange(p, Gamepad::ANALOG_STICK_2_Y, info.dwRpos);
						}

						if (info.dwUpos != p->lastState.dwUpos) {
							__handleAxisChange(p, Gamepad::TRIGGER_1, info.dwUpos); 
						}
 
						if (info.dwVpos != p->lastState.dwVpos) {
							__handleAxisChange(p, Gamepad::TRIGGER_2, info.dwVpos);
						} 
 
						p->lastState = info;
						

						//(value - devicePrivate->axisRanges[axisIndex][0]) / (float) (devicePrivate->axisRanges[axisIndex][1] - devicePrivate->axisRanges[axisIndex][0]) * 2.0f - 1.0f;

						//if (info.dwXpos != devicePrivate->lastState.dwXpos) {
							//handleAxisChange(device, devicePrivate->xAxisIndex, info.dwXpos);
						//}

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
							//if (!p->isButtonDown(Gamepad::DPAD_NONE)) {
							//	p->pressButton(Gamepad::DPAD_NONE);
							//}
						} else {
							p->dpadPosition = Gamepad::DPAD_NONE;
							dpadThisUpdate = Gamepad::DPAD_NONE;
							//if (!p->isButtonDown(Gamepad::DPAD_NONE)) {
							//	p->pressButton(Gamepad::DPAD_NONE);
							//}
						}

						//if (!p->isButtonDown(Gamepad::DPAD_NONE)) {// && dpadThisUpdate != Gamepad::DPAD_NONE) {
						//	p->releaseButton(Gamepad::DPAD_NONE);
						//}
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

								GamepadButton* but = p->buttons.at(buttonIndex);
								unsigned int newId = Gamepad::convertSystemButtonToARKButton(p, but->id); 

								if (p->isPS4Controller() && newId == but->id && (but->id == 6 || but->id == 7)) { continue; }
								 
								if (p->m_triggersSendBumperEvents && 
										(newId == Gamepad::BUTTON_LBUMPER || newId == Gamepad::BUTTON_RBUMPER || 
										newId == Gamepad::TRIGGER_1 || newId == Gamepad::TRIGGER_2
										)) { 

									unsigned int triggerIndex = (newId == Gamepad::BUTTON_LBUMPER || newId == Gamepad::TRIGGER_1) ? Gamepad::TRIGGER_1 : Gamepad::TRIGGER_2;

									bool b = !!(info.dwButtons & (1 << but->id));

									//if (b == true && (!p->isButtonDown(newId) && p->getAxisValue(triggerIndex) < 0.5f ) ) {
									//	p->pressButton(newId);
									//} else if (b == false && (p->isButtonDown(newId) && p->getAxisValue(triggerIndex) > 0.5f) ) {
									//	p->releaseButton(newId);
									//}
 
									if (b && !p->isButtonDown(newId)) {
										//if (p->getAxisValue(triggerIndex) > 0.5f) {
											p->pressButton(newId);
										//}
									} else if (!b && p->isButtonDown(newId)) {
										if (p->getAxisValue(triggerIndex) < 0.5f) {
											p->releaseButton(newId);
										}
									}
 

								} else { 

									bool b = !!(info.dwButtons & (1 << but->id));

									if (b == true && p->isButtonDown(newId) == false) {

										ARK2D::getLog()->i(StringUtil::append("old button id: ", but->id));
										ARK2D::getLog()->i(StringUtil::append("new button id: ", newId)); 

										p->pressButton(newId);
									} else if (b == false && p->isButtonDown(newId) == true) {
										p->releaseButton(newId);
									}
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

				Gamepad::initMapping();

				//ARK2D::getLog()->i("Initialising Gamepads");

				unsigned int numPadsSupported = joyGetNumDevs();
				unsigned int joystickId = 0;
				unsigned int actualJoyId = 0;

				if (numPadsSupported == 0) {
					//cout << "Gamepads not supported" << endl;
				} else {
					//cout << numPadsSupported << " gamepads supported." << endl;

					for (unsigned int i = 0; i < numPadsSupported && i < 10; i++)
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
							for (unsigned int j = 0; j < numPadsSupported && j < 10; j++) 
							{
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

							string gamepadName = Gamepad::getGamepadDescription(joystickId, info, caps); 
							if (StringUtil::str_contains(gamepadName, "Razer DeathStalker Ultimate")) {
								continue; 
							}
							
							//cout << "Adding gamepad with id " << joystickId << endl; 
							ARK2D::getLog()->i(StringUtil::append("Adding gamepad with id: ", joystickId));

							Gamepad* gamepad = new Gamepad();
							//gamepad->id = actualJoyId;
							gamepad->id = joystickId;
							gamepad->lastState = info;
							gamepad->initted = true;
							gamepad->vendorId = caps.wMid;
							gamepad->productId = caps.wPid;
							gamepad->name = gamepadName;

							GamepadMapping* mapping = gamepad->getMapping();
							if (mapping != NULL) { 
								gamepad->m_sharedTriggerAxis = mapping->shared_triggers_axis;
							} 

						//	ARK2D::getLog()->i(StringUtil::append("Gamepad description: ", gamepad->name));

							gamepad->numButtons = caps.wNumButtons;
							for (unsigned int j = 0; j < gamepad->numButtons; j++) {
								GamepadButton* button = new GamepadButton();
								button->id = j;
								button->down = false;  
								gamepad->buttons.push_back(button);
								//gamepad.buttonDownBuffer[j] = false;
							}

							GamepadButton* dpad_button = new GamepadButton();
							dpad_button->id = Gamepad::DPAD_UP;
							dpad_button->down = false;  
							gamepad->buttons.push_back(dpad_button);

							dpad_button = new GamepadButton();
							dpad_button->id = Gamepad::DPAD_DOWN;
							dpad_button->down = false;  
							gamepad->buttons.push_back(dpad_button);

							dpad_button = new GamepadButton();
							dpad_button->id = Gamepad::DPAD_LEFT;
							dpad_button->down = false;  
							gamepad->buttons.push_back(dpad_button);

							dpad_button = new GamepadButton();
							dpad_button->id = Gamepad::DPAD_RIGHT;
							dpad_button->down = false;  
							gamepad->buttons.push_back(dpad_button);


							//if ( gamepad->isXbox360Controller()) {

								gamepad->numAxes = 0; // caps.wNumAxes + ((caps.wCaps & JOYCAPS_HASPOV) ? 2 : 0);

								GamepadAxis* xAxis = new GamepadAxis();
								xAxis->id = 0;
								xAxis->axisId = Gamepad::ANALOG_STICK_1_X;
								xAxis->rangeMin = caps.wXmin;
								xAxis->rangeMax = caps.wXmax;
								gamepad->axes.push_back(xAxis);

								GamepadAxis* yAxis = new GamepadAxis(); 
								yAxis->id = 1;
								yAxis->axisId = Gamepad::ANALOG_STICK_1_Y;
								yAxis->rangeMin = caps.wYmin;
								yAxis->rangeMax = caps.wYmax; 
								gamepad->axes.push_back(yAxis);
								 
								GamepadAxis* rAxis = new GamepadAxis();
								rAxis->id = 3;
								rAxis->axisId = Gamepad::ANALOG_STICK_2_X;
								rAxis->rangeMin = caps.wRmin;
								rAxis->rangeMax = caps.wRmax;
								gamepad->axes.push_back(rAxis);
								
								GamepadAxis* uAxis = new GamepadAxis();
								uAxis->id = 4; 
								uAxis->axisId = Gamepad::ANALOG_STICK_2_Y;
								uAxis->rangeMin = caps.wUmin;
								uAxis->rangeMax = caps.wUmax;
								gamepad->axes.push_back(uAxis);

								 // ltrigger
								GamepadAxis* zAxis = new GamepadAxis();
								zAxis->id = 2;
								zAxis->axisId = Gamepad::TRIGGER_1;
								zAxis->rangeMin = caps.wZmin;
								zAxis->rangeMax = caps.wZmax;
								gamepad->axes.push_back(zAxis);
						
								GamepadAxis* vAxis = new GamepadAxis(); // rtrigger
								vAxis->id = 5;
								vAxis->axisId = Gamepad::TRIGGER_2;
								vAxis->rangeMin = caps.wVmin;
								vAxis->rangeMax = caps.wVmax;  
								gamepad->axes.push_back(vAxis);
							
								gamepad->povXAxisIndex = -1; //(caps.wCaps & JOYCAPS_HASPOV) ? axisIndex++ : -1;
								gamepad->povYAxisIndex = -1; // (caps.wCaps & JOYCAPS_HASPOV) ? axisIndex++ : -1;

								gamepad->numAxes = gamepad->axes.size(); //caps.wNumAxes + ((caps.wCaps & JOYCAPS_HASPOV) ? 2 : 0);  


							/*} else { 					

								gamepad->numAxes = 0; // caps.wNumAxes + ((caps.wCaps & JOYCAPS_HASPOV) ? 2 : 0);

								GamepadAxis* xAxis = new GamepadAxis();
								xAxis->id = 0;
								xAxis->rangeMin = caps.wXmin;
								xAxis->rangeMax = caps.wXmax;
								gamepad->axes.push_back(xAxis);

								GamepadAxis* yAxis = new GamepadAxis();
								yAxis->id = 1;
								yAxis->rangeMin = caps.wYmin;
								yAxis->rangeMax = caps.wYmax; 
								gamepad->axes.push_back(yAxis);
								 
								int axisIndex = 2; 

								if (caps.wCaps & JOYCAPS_HASZ) { // ltrigger
									GamepadAxis* zAxis = new GamepadAxis();
									zAxis->id = axisIndex;
									zAxis->rangeMin = caps.wZmin;
									zAxis->rangeMax = caps.wZmax;
									gamepad->axes.push_back(zAxis);
									axisIndex++;
								}

								if (caps.wCaps & JOYCAPS_HASR) { // right stick x
									GamepadAxis* rAxis = new GamepadAxis();
									rAxis->id = axisIndex;
									rAxis->rangeMin = caps.wRmin;
									rAxis->rangeMax = caps.wRmax;
									gamepad->axes.push_back(rAxis);
									axisIndex++;
								}

								if (caps.wCaps & JOYCAPS_HASU) { // right stick y
									GamepadAxis* uAxis = new GamepadAxis();
									uAxis->id = axisIndex;
									uAxis->rangeMin = caps.wUmin;
									uAxis->rangeMax = caps.wUmax;
									gamepad->axes.push_back(uAxis);
									axisIndex++;
								} 

								if (caps.wCaps & JOYCAPS_HASV) { // right trigger
									GamepadAxis* vAxis = new GamepadAxis();
									vAxis->id = axisIndex;
									vAxis->rangeMin = caps.wVmin;
									vAxis->rangeMax = caps.wVmax; 
									gamepad->axes.push_back(vAxis);
									axisIndex++;
								}
								
								gamepad->povXAxisIndex = (caps.wCaps & JOYCAPS_HASPOV) ? axisIndex++ : -1;
								gamepad->povYAxisIndex = (caps.wCaps & JOYCAPS_HASPOV) ? axisIndex++ : -1;

								gamepad->numAxes = caps.wNumAxes + ((caps.wCaps & JOYCAPS_HASPOV) ? 2 : 0);  
							}*/

							//int extraAxis = ((caps.wCaps & JOYCAPS_HASPOV) ? 2 : 0);  
							//gamepad->numAxes = axisIndex + extraAxis;

							/*deviceRecordPrivate->axisRanges = malloc(sizeof(unsigned int) * 2 * axisIndex);
							deviceRecordPrivate->axisRanges[0][0] = caps.wXmin;
							deviceRecordPrivate->axisRanges[0][1] = caps.wXmax;
							deviceRecordPrivate->axisRanges[1][0] = caps.wYmin;
							deviceRecordPrivate->axisRanges[1][1] = caps.wYmax;
							if (gamepad->zAxisIndex != -1) {
								deviceRecordPrivate->axisRanges[gamepad->zAxisIndex][0] = caps.wZmin;
								deviceRecordPrivate->axisRanges[gamepad->zAxisIndex][1] = caps.wZmax;
							}
							if (gamepad->rAxisIndex != -1) {
								deviceRecordPrivate->axisRanges[gamepad->rAxisIndex][0] = caps.wRmin;
								deviceRecordPrivate->axisRanges[gamepad->rAxisIndex][1] = caps.wRmax;
							}
							if (gamepad->uAxisIndex != -1) {
								deviceRecordPrivate->axisRanges[gamepad->uAxisIndex][0] = caps.wUmin;
								deviceRecordPrivate->axisRanges[gamepad->uAxisIndex][1] = caps.wUmax;
							}
							if (gamepad->vAxisIndex != -1) {
								deviceRecordPrivate->axisRanges[gamepad->vAxisIndex][0] = caps.wVmin;
								deviceRecordPrivate->axisRanges[gamepad->vAxisIndex][1] = caps.wVmax;
							}
							
							gamepad->povXAxisIndex = (caps.wCaps & JOYCAPS_HASPOV) ? axisIndex++ : -1;
							gamepad->povYAxisIndex = (caps.wCaps & JOYCAPS_HASPOV) ? axisIndex++ : -1;*/

							ARK2D::getLog()->i(gamepad->toString());
							m_gamepads.push_back(gamepad);

							// Give event to game
							Game* g = ARK2D::getGame();
							GamepadListener* gl = NULL;
							gl = dynamic_cast<GamepadListener*>(g);
							if (gl != NULL) {
								gl->gamepadConnected(gamepad);
							}

							actualJoyId++;

							//cout << "Added gamepad with id " << joystickId << endl;
						}
					}
				}
			}

			void GameContainer::setCursorVisible(bool b) {
				if (b) {
					int newCounter = ShowCursor(TRUE);
					ARK2D::getLog()->v(StringUtil::append("Cursor set to visible. Returned: ", newCounter));
					// TODO loop until it's visible or use some other method of making sure it's always visible.
				} else {
					int newCounter = ShowCursor(FALSE);	
					ARK2D::getLog()->v(StringUtil::append("Cursor set to visible. Returned: ", newCounter));
					// TODO loop until it's invisible or use some other method of making sure it's always invisible.
				}
				
			}

			void GameContainer::resizeWindowToFitViewport() {

				// This bit resizes the window so that the GL viewport can fit in properly!
				RECT rcClient, rcWind;
				POINT ptDiff;
				
				/*rcClient.left=0;
				rcClient.right=m_width;
				rcClient.top=0;
				rcClient.bottom=m_height;*/
				GetClientRect(m_platformSpecific.m_hWindow, &rcClient); // size inside the window.
				GetWindowRect(m_platformSpecific.m_hWindow, &rcWind); // size including the window. 

				ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
				ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;

				// we don't want to do this if we're fullscreen at screen width/height already. 
				// it has a slight (annoying) flicker.
				//bool repaint = TRUE;
				//if (m_fullscreen && m_width == m_screenWidth && m_height == m_screenHeight) {
				//	repaint = FALSE;
				//}
				if (isFullscreen()) {
					rcWind.left += m_platformSpecific.m_screenStartX;
					rcWind.top += m_platformSpecific.m_screenStartY;
				}

				MoveWindow(
					m_platformSpecific.m_hWindow,
					rcWind.left, 
					rcWind.top, 
					m_width + ptDiff.x, 
					m_height + ptDiff.y, 
					TRUE
				);
				 
	
				//(GetSystemMetrics(SM_CYBORDER)/2);
				/*m_window_rectangle = new ARK::Geometry::Rectangle<int>(	(int) rcWind.top,
																(int) rcWind.left,
																(int) m_width,
																(int) m_height); // My Rectangle...*/
			}

			HRESULT GameContainerPlatform::initDXDeviceIndependentResources() 
			{
				#ifdef ARK2D_RENDERER_DIRECTX
					ARK2D::getLog()->v("DX: initDXDeviceIndependentResources");
					// Create a Direct2D factory.
			    	HRESULT hr = S_OK;
					//hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

			    	// Initialize Direct2D resources.
					D2D1_FACTORY_OPTIONS options;
					ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

					#if defined(ARK2D_DEBUG)
						// If the project is in a debug build, enable Direct2D debugging via SDK Layers.
						options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
					#endif

					// Initialize the Direct2D Factory.
					hr = D2D1CreateFactory(
						D2D1_FACTORY_TYPE_SINGLE_THREADED,
						__uuidof(ID2D1Factory1),
						&options,
						(void**) &m_d2dFactory
					);
					if (FAILED(hr)){
						ErrorDialog::createAndShow("D2D1CreateFactory failed");
						exit(0);
						return hr;
					}

					// Initialize the DirectWrite Factory.
					hr = DWriteCreateFactory(
						DWRITE_FACTORY_TYPE_SHARED,
						__uuidof(IDWriteFactory1),
						reinterpret_cast<IUnknown**>( &m_dwriteFactory )
					);
					if (FAILED(hr)){
						ErrorDialog::createAndShow("DWriteCreateFactory failed");
						exit(0);
						return hr;
					}
					
					// Initialize the Windows Imaging Component (WIC) Factory.
					/*hr = CoCreateInstance(
						CLSID_WICImagingFactory2,
						nullptr,
						CLSCTX_INPROC_SERVER,
						IID_PPV_ARGS(&m_wicFactory)
					);
					if (FAILED(hr)){
						ARK2D::getLog()->e("CoCreateInstance failed");
						return hr;
					}*/

			    	return hr;
			    #else
			    	return NULL;
			    #endif
			}
			bool GameContainerPlatform::initDXDeviceResources() {
				
				#ifdef ARK2D_RENDERER_DIRECTX
					
					ARK2D::getLog()->v("DX: initDXDeviceResources");
					RECT rc;
					GetClientRect(m_hWindow, &rc);

					D2D1_SIZE_U size = D2D1::SizeU( rc.right - rc.left, rc.bottom - rc.top);

					// Create a Direct2D render target.
					/*hr = m_pDirect2dFactory->CreateHwndRenderTarget(
						D2D1::RenderTargetProperties(),
						D2D1::HwndRenderTargetProperties(m_hWindow, size),
						&m_pRenderTarget
					);*/

					/*if (SUCCEEDED(hr))
					{
						// Create a gray brush.
						hr = m_pRenderTarget->CreateSolidColorBrush(
							D2D1::ColorF(D2D1::ColorF::LightSlateGray),
							&m_pLightSlateGrayBrush
						);
					} 
					if (SUCCEEDED(hr))
					{
						// Create a blue brush.
						hr = m_pRenderTarget->CreateSolidColorBrush(
							D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
							&m_pCornflowerBlueBrush
						);
					}*/

					//IDXGIFactory* factory;
					IDXGIAdapter* adapter;
					IDXGIOutput* adapterOutput;

					HRESULT result;
					
					//unsigned int numModes, i, numerator, denominator, stringLength;
					/*DXGI_MODE_DESC* displayModeList;
					DXGI_ADAPTER_DESC adapterDesc;
					int error;*/
					DXGI_SWAP_CHAIN_DESC swapChainDesc;
					D3D_FEATURE_LEVEL featureLevel;
					ID3D11Texture2D* backBufferPtr;
					D3D11_TEXTURE2D_DESC depthBufferDesc;
					D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
					D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
					D3D11_RASTERIZER_DESC rasterDesc;
					D3D11_VIEWPORT viewport;
					float fieldOfView, screenAspect;
					D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

					// Store the vsync setting.
					m_vsync_enabled = false;

					// Create a DirectX graphics interface factory.
					/*result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
					if(FAILED(result))
					{
						ARK2D::getLog()->e("CreateDXGIFactory failed");
						return false;
					}

					// Use the factory to create an adapter for the primary graphics interface (video card).
					result = factory->EnumAdapters(0, &adapter);
					if(FAILED(result))
					{
						ARK2D::getLog()->e("EnumAdapters failed");
						return false;
					}

					// Enumerate the primary adapter output (monitor).
					result = adapter->EnumOutputs(0, &adapterOutput);
					if(FAILED(result))
					{
						ARK2D::getLog()->e("EnumOutputs failed");
						return false;
					}

					// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
					result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
					if(FAILED(result))
					{
						ARK2D::getLog()->e("GetDisplayModeList 1 failed");
						return false;
					}

					// Create a list to hold all the possible display modes for this monitor/video card combination.
					displayModeList = new DXGI_MODE_DESC[numModes];
					if(!displayModeList)
					{
						ARK2D::getLog()->e("DXGI_MODE_DESC failed");
						return false;
					}

					// Now fill the display mode list structures.
					result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
					if(FAILED(result))
					{
						ARK2D::getLog()->e("GetDisplayModeList 2 failed");
						return false;
					}

					// Now go through all the display modes and find the one that matches the screen width and height.
					// When a match is found store the numerator and denominator of the refresh rate for that monitor.
					bool found = false;
					for(i=0; i<numModes; i++)
					{
						if(displayModeList[i].Width == (unsigned int)m_container->getScreenWidth())
						{
							if(displayModeList[i].Height == (unsigned int)m_container->getScreenHeight())
							{
								numerator = displayModeList[i].RefreshRate.Numerator;
								denominator = displayModeList[i].RefreshRate.Denominator;
								found = true;
							}
						}
					}
					if (!found) {
						string res = Cast::toString<unsigned int>(m_container->getScreenWidth());
						res += " x ";
						res += Cast::toString<unsigned int>(m_container->getScreenHeight());
						ARK2D::getLog()->e(StringUtil::append("Could not find Display Mode for resolution: ", res));
						return false;
					}

					// Get the adapter (video card) description.
					result = adapter->GetDesc(&adapterDesc);
					if(FAILED(result))
					{
						ARK2D::getLog()->e("GetDesc failed");
						return false;
					}

					// Store the dedicated video card memory in megabytes.
					m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

					// Convert the name of the video card to a character array and store it.
					error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
					if(error != 0)
					{
						ARK2D::getLog()->e("wcstombs_s failed");
						return false;
					}

					ARK2D::getLog()->i("Video Card Info: ");
					ARK2D::getLog()->i(getDXVideoCardInfo());

					// Release the display mode list.
					delete [] displayModeList;
					displayModeList = 0;

					// Release the adapter output.
					adapterOutput->Release();
					adapterOutput = 0;

					// Release the adapter.
					adapter->Release();
					adapter = 0;

					// Release the factory.
					factory->Release();
					factory = 0;*/

					// Initialize the swap chain description.
					ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

					// Set to a single back buffer.
					swapChainDesc.BufferCount = 1;

					// Set the width and height of the back buffer.
					swapChainDesc.BufferDesc.Width = m_container->getWidth();//screenWidth;
					swapChainDesc.BufferDesc.Height = m_container->getHeight();//screenHeight;

					// Set regular 32-bit surface for the back buffer.
					swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

					// Set the refresh rate of the back buffer.
					//if(m_vsync_enabled)
					//{
					//	swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
					//	swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
					//}
					//else
					//{
						swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
						swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
					//}

					// Set the usage of the back buffer.
					swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

					// Set the handle for the window to render to.
					swapChainDesc.OutputWindow = m_hWindow; //hwnd;

					// Turn multisampling off.
					swapChainDesc.SampleDesc.Count = 1;
					swapChainDesc.SampleDesc.Quality = 0;

					// Set to full screen or windowed mode.
					if(m_container->isFullscreen())
					{
						swapChainDesc.Windowed = FALSE;
					}
					else
					{
						swapChainDesc.Windowed = TRUE;
						swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
					}

					// Set the scan line ordering and scaling to unspecified.
					swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
					swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

					// Discard the back buffer contents after presenting.
					swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

					// Don't set the advanced flags.
					swapChainDesc.Flags = 0;

					// Set the feature level to DirectX 11.
					//featureLevel = D3D_FEATURE_LEVEL_9_1; //D3D_FEATURE_LEVEL_11_0;
					D3D_FEATURE_LEVEL featureLevels[] =
					{
						D3D_FEATURE_LEVEL_11_1,
						D3D_FEATURE_LEVEL_11_0,
						D3D_FEATURE_LEVEL_10_1,
						D3D_FEATURE_LEVEL_10_0,
						D3D_FEATURE_LEVEL_9_3,
						D3D_FEATURE_LEVEL_9_2,
						D3D_FEATURE_LEVEL_9_1
					};

					UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

					#ifdef ARK2D_DEBUG
						creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
					#endif

					// Create the swap chain, Direct3D device, and Direct3D device context.

					// WINDOWS STORE ONLY 
					/*result = D3D11CreateDevice(
						nullptr,
						D3D_DRIVER_TYPE_HARDWARE,
						0,
						creationFlags,
						featureLevels,
						ARRAYSIZE(featureLevels),
						D3D11_SDK_VERSION,
						&m_device,
						&featureLevel,
						&m_deviceContext
					);
					if (FAILED(result)) {
						ErrorDialog::createAndShow(StringUtil::append("DirectX 11 D3D11CreateDevice failed: ", DX_GetError(result)));
						exit(0);
					} 

					result = CreateSwapChainForCoreWindow(
						m_device
					);*/

					result = D3D11CreateDeviceAndSwapChain(
						nullptr, 
						D3D_DRIVER_TYPE_HARDWARE,  //D3D_DRIVER_TYPE_HARDWARE
						0,  
						creationFlags, //D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_DEBUGGABLE, // 0, 
						featureLevels, 
						ARRAYSIZE(featureLevels), 
						D3D11_SDK_VERSION, //D3D11_SDK_VERSION, 
						&swapChainDesc, 
						&m_swapChain, 
						&m_device, 
						&featureLevel, 
						&m_deviceContext
					);
					
					if (FAILED(result)) {
						//
						ARK2D::getLog()->w(StringUtil::append("DirectX 11 D3D11CreateDeviceAndSwapChain failed: ", DX_GetError(result)));
						ARK2D::getLog()->w("Creating WARP device.");
						
						result = D3D11CreateDeviceAndSwapChain(
							nullptr,
							D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
							0,
							creationFlags,
							featureLevels,
							ARRAYSIZE(featureLevels),
							D3D11_SDK_VERSION,
							&swapChainDesc,
							&m_swapChain, 
							&m_device,
							&featureLevel,
							&m_deviceContext
						);
						if (FAILED(result)) {
							ErrorDialog::createAndShow(StringUtil::append("DirectX 11 D3D11CreateDeviceAndSwapChain (WARP) failed: ", DX_GetError(result)));
							exit(0);
						}
						
					} 

					// Feature level...
					ARK2D::getLog()->v(StringUtil::append("DirectX feature level: ", getD3DFeatureLevelString(featureLevel)));

					// D2D stuffs.
					IDXGIDevice* pDXGIDevice;
					result = m_device->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
					if (FAILED(result)) {
						ErrorDialog::createAndShow(StringUtil::append("DirectX 11 IDXGIDevice QueryInterface failed: ", DX_GetError(result)));
						exit(0);
					}
					 
					result = m_d2dFactory->CreateDevice(pDXGIDevice, &m_d2dDevice);
					if (FAILED(result)) {
						ErrorDialog::createAndShow(StringUtil::append("DirectX 11 Could not create D2D device: ", DX_GetError(result)));
						exit(0);
					}

					result = m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext);
					if (FAILED(result)) {
						ErrorDialog::createAndShow(StringUtil::append("DirectX 11 Could not create D2D device context: ", DX_GetError(result)));
						exit(0);
					}

					result = m_swapChain->SetFullscreenState(false, NULL);
					if (FAILED(result)) {
						ErrorDialog::createAndShow(StringUtil::append("DirectX 11 SwapChain::SetFullscreenState failed: ", DX_GetError(result)));
						exit(0);
					}
					
					// Get the pointer to the back buffer.
					result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
					if(FAILED(result))
					{
						ARK2D::getLog()->e("GetBuffer failed");
						return false;
					}

					// Create the render target view with the back buffer pointer.
					result = m_device->CreateRenderTargetView(backBufferPtr, nullptr, &m_renderTargetView);
					if(FAILED(result))
					{
						ARK2D::getLog()->e("CreateRenderTargetView failed");
						return false;
					}

					ARK2D::getLog()->i("ere 5");

					// Release pointer to the back buffer as we no longer need it.
					backBufferPtr->Release();
					backBufferPtr = 0;

					ARK2D::getLog()->i("ere 6");

					// Initialize the description of the depth buffer.
					ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

					ARK2D::getLog()->i("ere 7");

					// Set up the description of the depth buffer.
					depthBufferDesc.Width = m_container->getWidth();//screenWidth;
					depthBufferDesc.Height = m_container->getHeight();//screenHeight;
					depthBufferDesc.MipLevels = 1;
					depthBufferDesc.ArraySize = 1;
					depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
					depthBufferDesc.SampleDesc.Count = 1;
					depthBufferDesc.SampleDesc.Quality = 0;
					depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
					depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
					depthBufferDesc.CPUAccessFlags = 0;
					depthBufferDesc.MiscFlags = 0;

					// Create the texture for the depth buffer using the filled out description.
					result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
					if(FAILED(result))
					{
						ARK2D::getLog()->e("CreateTexture2D failed");
						return false;
					}

					ARK2D::getLog()->i("ere 8");

					// Initialize the description of the stencil state.
					ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

					// Set up the description of the stencil state.
					depthStencilDesc.DepthEnable = true;
					depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
					depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

					depthStencilDesc.StencilEnable = true;
					depthStencilDesc.StencilReadMask = 0xFF;
					depthStencilDesc.StencilWriteMask = 0xFF;

					// Stencil operations if pixel is front-facing.
					depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
					depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
					depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
					depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

					// Stencil operations if pixel is back-facing.
					depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
					depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
					depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
					depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

					ARK2D::getLog()->i("ere 9");

					// Create the depth stencil state.
					result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
					if(FAILED(result))
					{
						ARK2D::getLog()->e("CreateDepthStencilState failed");
						return false;
					}

					ARK2D::getLog()->i("ere 10");

					// Set the depth stencil state.
					m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

					// Initailze the depth stencil view.
					ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

					// Set up the depth stencil view description.
					depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
					depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
					depthStencilViewDesc.Texture2D.MipSlice = 0;

					// Create the depth stencil view.
					result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
					if(FAILED(result))
					{
						ARK2D::getLog()->e("CreateDepthStencilView failed");
						return false;
					}

					ARK2D::getLog()->i("ere 11");

					// Bind the render target view and depth stencil buffer to the output render pipeline.
					m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

					ARK2D::getLog()->i("ere 12");

					// Setup the raster description which will determine how and what polygons will be drawn.
					rasterDesc.AntialiasedLineEnable = false;
					rasterDesc.CullMode = D3D11_CULL_BACK;
					rasterDesc.DepthBias = 0;
					rasterDesc.DepthBiasClamp = 0.0f;
					rasterDesc.DepthClipEnable = true;
					rasterDesc.FillMode = D3D11_FILL_SOLID;
					rasterDesc.FrontCounterClockwise = false;
					rasterDesc.MultisampleEnable = false;
					rasterDesc.ScissorEnable = false;
					rasterDesc.SlopeScaledDepthBias = 0.0f;

					// Create the rasterizer state from the description we just filled out.
					result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
					if(FAILED(result))
					{
						ARK2D::getLog()->e("CreateRasterizerState failed");
						return false;
					}

					ARK2D::getLog()->i("ere 13");

					// Now set the rasterizer state.
					m_deviceContext->RSSetState(m_rasterState);

					// Setup the viewport for rendering.
					viewport.Width = m_container->getWidth();//(float)m_container->m_screenWidth;
					viewport.Height = m_container->getHeight();//(float)m_container->m_screenHeight;
					viewport.MinDepth = 0.0f;
					viewport.MaxDepth = 1.0f;
					viewport.TopLeftX = 0.0f;
					viewport.TopLeftY = 0.0f;

					ARK2D::getLog()->i("ere 14");

					// Create the viewport.
					m_deviceContext->RSSetViewports(1, &viewport);

					ARK2D::getLog()->i("ere 15");

					// 2d backbuffer something
					D2D1_BITMAP_PROPERTIES1 bitmapProperties =
						D2D1::BitmapProperties1(
						D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
						D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
						96,
						96
					);

					/*ComPtr<IDXGISurface2> dxgiBackBuffer;
					m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))
							);

						DX::ThrowIfFailed(
							m_d2dContext->CreateBitmapFromDxgiSurface(
							dxgiBackBuffer.Get(),
							&bitmapProperties,
							&m_d2dTargetBitmap
							)
						);

					m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());
 
					// Grayscale text anti-aliasing is recommended for all Windows Store apps.
					m_d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
*/
					ARK2D::getLog()->i("ere 15");

					// Setup the projection matrix.
					float screenNear = -1.0f;
					float screenDepth = 2.0;
					fieldOfView = (float)DirectX::XM_PI / 4.0f; //(float)D3DX_PI / 4.0f;
					screenAspect = (float)m_container->getWidth() / (float)m_container->getHeight();

					ARK2D::getLog()->i("ere 16");

					/*// Create the projection matrix for 3D rendering.
					m_projectionMatrix = DirectX::XMMatrixPerspectiveLH((float)m_container->getWidth(), (float)m_container->getHeight(), screenNear, screenNear + screenDepth); //DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenNear + screenDepth); //XMMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth); // D3DXMatrixPerspectiveFovLH

					ARK2D::getLog()->i("ere 17");

					// Initialize the world matrix to the identity matrix.
					m_worldMatrix = DirectX::XMMatrixIdentity(); // D3DXMatrixIdentity(&m_worldMatrix);

					ARK2D::getLog()->i("ere 18");

					// Create an orthographic projection matrix for 2D rendering.
					m_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)m_container->getWidth(), (float)m_container->getHeight(), screenNear, screenNear + screenDepth); //D3DXMatrixOrthoLH(&m_orthoMatrix, (float)m_container->getWidth(), (float)m_container->getHeight(), screenNear, screenDepth);
*/
					ARK2D::getLog()->i("ere 19");

					// Clear the second depth stencil state before setting the parameters.
					ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
 
					// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
					// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
					depthDisabledStencilDesc.DepthEnable = false;
					depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
					depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
					depthDisabledStencilDesc.StencilEnable = true;
					depthDisabledStencilDesc.StencilReadMask = 0xFF;
					depthDisabledStencilDesc.StencilWriteMask = 0xFF;
					depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
					depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
					depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
					depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
					depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
					depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
					depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
					depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

					ARK2D::getLog()->i("ere 20");
					// Create the state using the device.
					result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
					if (FAILED(result)) {
						ARK2D::getLog()->e("2d depth buffer creation failed");
						return false;
					}
					ARK2D::getLog()->i("ere 21");

					turnZBufferOffDX();

					ARK2D::getLog()->i("ere 22");
					

				#endif

				return true;
			}

			

			void GameContainerPlatform::beginDXFrame() {
				
				#ifdef ARK2D_RENDERER_DIRECTX

					float color[4];
					color[0] = m_container->m_clearColor.getRedf();
					color[1] = m_container->m_clearColor.getGreenf();
					color[2] = m_container->m_clearColor.getBluef();
					color[3] = m_container->m_clearColor.getAlphaf();

					// Reset render targets to the screen.
					ID3D11RenderTargetView *const targets[1] = { m_renderTargetView };
					m_deviceContext->OMSetRenderTargets(1, targets, m_depthStencilView);

					// Clear the back buffer.
					m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
			    
					// Clear the depth buffer.
					m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

					// 
					turnZBufferOffDX();
				#endif
			}

			void GameContainerPlatform::endDXFrame() {
				#ifdef ARK2D_RENDERER_DIRECTX
					// Present the back buffer to the screen since rendering is complete.
					if(m_vsync_enabled)
					{
						// Lock to screen refresh rate.
						HRESULT result = m_swapChain->Present(1, 0);
						if (FAILED(result)) {
							ErrorDialog::createAndShow(StringUtil::append("DirectX 11 SwapChain::Present failed: ", DX_GetError(result)));
							exit(0);
						}
					}
					else
					{
						// Present as fast as possible.
						HRESULT result = m_swapChain->Present(0, 0);
						if (FAILED(result)) {
							ErrorDialog::createAndShow(StringUtil::append("DirectX 11 SwapChain::Present failed: ", DX_GetError(result)));
							exit(0);
						}
					}

					//m_deviceContext->DiscardView(m_renderTargetView);
					//m_deviceContext->DiscardView(m_depthStencilView);
				#endif
			}
			string GameContainerPlatform::getDXVideoCardInfo()
			{
				#ifdef ARK2D_RENDERER_DIRECTX
					return StringUtil::append(m_videoCardDescription, m_videoCardMemory);
				#else
					return "";
				#endif
			}
			void GameContainerPlatform::turnZBufferOnDX() {
				#ifdef ARK2D_RENDERER_DIRECTX
					m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
				#endif
			} 
			void GameContainerPlatform::turnZBufferOffDX() {
				#ifdef ARK2D_RENDERER_DIRECTX
					m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
				#endif
			}

			void GameContainerPlatform::deinitDXDeviceResources() {
				#ifdef ARK2D_RENDERER_DIRECTX

					// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
					if(m_swapChain)
					{
						m_swapChain->SetFullscreenState(false, NULL);
					}

					if (m_d2dFactory) {
						m_d2dFactory->Release();
						m_d2dFactory = 0;
					}
					if (m_d2dDevice) {
						m_d2dDevice->Release();
						m_d2dDevice = 0;
					}
					if (m_d2dContext) {
						m_d2dContext->Release();
						m_d2dContext = 0;
					}

					if(m_depthDisabledStencilState)
					{
						m_depthDisabledStencilState->Release();
						m_depthDisabledStencilState = 0;
					}

					if(m_rasterState)
					{
						m_rasterState->Release();
						m_rasterState = 0;
					}

					if(m_depthStencilView)
					{
						m_depthStencilView->Release();
						m_depthStencilView = 0;
					}

					if(m_depthStencilState)
					{
						m_depthStencilState->Release();
						m_depthStencilState = 0;
					}

					if(m_depthStencilBuffer)
					{
						m_depthStencilBuffer->Release();
						m_depthStencilBuffer = 0;
					}

					if(m_renderTargetView)
					{
						m_renderTargetView->Release();
						m_renderTargetView = 0;
					}

					if(m_deviceContext)
					{
						m_deviceContext->Release();
						m_deviceContext = 0;
					}

					if(m_device)
					{
						m_device->Release();
						m_device = 0;
					}

					if(m_swapChain)
					{
						m_swapChain->Release();
						m_swapChain = 0;
					}

				#endif
			}

			#include <gl/wglext.h>		//WGL extensions
			//#include <gl/glext.h>		//GL extensions
			bool    arbMultisampleSupported = false;
			int 	arbMultisampleFormat    = 0;
			// Credit: Henry Goffin.
			// http://nehe.gamedev.net/tutorial/fullscreen_antialiasing/16008/
			bool WGLisExtensionSupported(const char *extension)
			{
			    const size_t extlen = strlen(extension);
			    const char *supported = NULL;
			 
			    // Try To Use wglGetExtensionStringARB On Current DC, If Possible
			    PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");
			 
			    if (wglGetExtString)
			        supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());
			 
			    // If That Failed, Try Standard Opengl Extensions String
			    if (supported == NULL) 
			        supported = (char*)glGetString(GL_EXTENSIONS);
			 
			    // If That Failed Too, Must Be No Extensions Supported
			    if (supported == NULL)
			        return false;
			 
			    // Begin Examination At Start Of String, Increment By 1 On False Match
			    for (const char* p = supported; ; p++)
			    {
			        // Advance p Up To The Next Possible Match
			        p = strstr(p, extension);
			 
			        if (p == NULL)
			            return false;                       // No Match
			 
			        // Make Sure That Match Is At The Start Of The String Or That
			        // The Previous Char Is A Space, Or Else We Could Accidentally
			        // Match "wglFunkywglExtension" With "wglExtension"
			 
			        // Also, Make Sure That The Following Character Is Space Or NULL
			        // Or Else "wglExtensionTwo" Might Match "wglExtension"
			        if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' '))
			            return true;                        // Match
			    }
			}

			bool InitMultisample(HINSTANCE hInstance,HWND hWnd,PIXELFORMATDESCRIPTOR pfd)
			{ 
			    // See If The String Exists In WGL!
			    if (!WGLisExtensionSupported("WGL_ARB_multisample"))
			    {
			        arbMultisampleSupported=false;
			        return false;
			    }
			 
			    // Get Our Pixel Format
			    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB =
			        (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
			 
			    if (!wglChoosePixelFormatARB)
			    {
			        // We Didn't Find Support For Multisampling, Set Our Flag And Exit Out.
			        arbMultisampleSupported=false;
			        return false;
			    }
			 
			    // Get Our Current Device Context. We Need This In Order To Ask The OpenGL Window What Attributes We Have
			    HDC hDC = GetDC(hWnd);
			 
			    int pixelFormat;
			    bool valid;
			    UINT numFormats;
			    float fAttributes[] = {0,0};
			 
			    // These Attributes Are The Bits We Want To Test For In Our Sample
			    // Everything Is Pretty Standard, The Only One We Want To
			    // Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
			    // These Two Are Going To Do The Main Testing For Whether Or Not
			    // We Support Multisampling On This Hardware
			    int iAttributes[] = { 
			    	WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			        WGL_ACCELERATION_ARB,	WGL_FULL_ACCELERATION_ARB,
			        WGL_COLOR_BITS_ARB,		24,
			        WGL_ALPHA_BITS_ARB,		8,
			        WGL_DEPTH_BITS_ARB,		16,
			        WGL_STENCIL_BITS_ARB,	8, //1,
			        WGL_DOUBLE_BUFFER_ARB,	GL_TRUE,
			        WGL_SAMPLE_BUFFERS_ARB,	GL_TRUE,
			        WGL_SAMPLES_ARB, 		4,                        // Check For 4x Multisampling
			        0,
			        0
			    };
			 
			    // First We Check To See If We Can Get A Pixel Format For 4 Samples
			    valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
			  
			    // if We Returned True, And Our Format Count Is Greater Than 1
			    if (valid && numFormats >= 1)
			    {
			        arbMultisampleSupported = true;
			        arbMultisampleFormat    = pixelFormat; 
			        return arbMultisampleSupported;
			    }
			 
			    // Our Pixel Format With 4 Samples Failed, Test For 2 Samples
			    iAttributes[19] = 2;
			    valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
			    if (valid && numFormats >= 1)
			    {
			        arbMultisampleSupported = true;
			        arbMultisampleFormat    = pixelFormat;  
			        return arbMultisampleSupported;
			    }
			       
			    // Return The Valid Format
			    return  arbMultisampleSupported;
			}

			// Enable OpenGL
			void GameContainerPlatform::enableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC)
			{
				

				#if defined( ARK2D_RENDERER_OPENGL )

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
					pfd.cColorBits = 32; 
					pfd.cDepthBits = 16;
					pfd.cStencilBits = 8;//1;
					pfd.iLayerType = PFD_MAIN_PLANE;
					format = ChoosePixelFormat( *hDC, &pfd );
					SetPixelFormat( *hDC, format, &pfd );

					// create and enable the render context (RC)
					*hRC = wglCreateContext( *hDC );
					wglMakeCurrent( *hDC, *hRC );

					// initialise that stupid glew crap.
					//GLenum err = glewInit();
					//if (err != GLEW_OK) {
					//	//problem: glewInit failed, something is seriously wrong
					//	char ErrorMessage[100];
					//	sprintf(ErrorMessage, "Error: %s\n", glewGetErrorString(err));
					//	return;
					//}
					//ARK2D::getLog()->i("Initialising OpenGL"); 

					if (!arbMultisampleSupported) //{// && CHECK_FOR_MULTISAMPLE)
					{
						if(InitMultisample(m_hInstance, hWnd, pfd))
						{
							ARK2D::getLog()->i("Multisample. We can do that!");
							//DestroyWindowGL (window);
							//return CreateWindowGL(window);

							// Destroy window things.
							//ReleaseDC (hWnd, hDC);							// Release Our Device Context
							//*hDC = 0;										// Zero The Device Context
							//DestroyWindow (hWnd);							// Destroy The Window
							//hWnd = 0;										// Whatttt?!

							//disableOpenGL( m_hWindow, *hDC, *hRC );
							ReleaseDC( hWnd, *hDC );
							*hDC = 0;
							DestroyWindow(hWnd);
							hWnd = 0;

							
							m_hWindow = doCreateWindow(); 
							hWnd = m_hWindow;

							m_container->resizeWindowToFitViewport();

							// get the device context (DC)
							*hDC = GetDC( hWnd );

							format = arbMultisampleFormat;
							SetPixelFormat( *hDC, format, &pfd );

							// create and enable the render context (RC)
							*hRC = wglCreateContext( *hDC );
							wglMakeCurrent( *hDC, *hRC );
						}
					}

					// that last DC/RC will have to be replaced by one with multisampling...
					/*int attributes[] = {
						WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
						WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
						WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
						WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
						WGL_COLOR_BITS_ARB, 24,//32,
						WGL_DEPTH_BITS_ARB, 16,//24,
						WGL_STENCIL_BITS_ARB, 1,
						WGL_SAMPLE_BUFFERS_ARB, 1, //Number of buffers (must be 1 at time of writing)
   						WGL_SAMPLES_ARB, 4,        //Number of samples
						0
					};*/


					int glinitint = gl3wInit();
					ARK2D::getLog()->i(StringUtil::append("GL INIT: ", glinitint));
					if (glinitint == -1) {
	                	ErrorDialog::createAndShow("Failed to initialise OpenGL for an unknown reason. Please update your graphics drivers.");
	                	exit(0);
	                	return;// -1;
	        		}
	        		if (!gl3wIsSupported(3, 2)) { 
	               		ARK2D::getLog()->w("OpenGL 3.2 is not supported on your computer. Trying OpenGL 3.1...");
	        		} else if (!gl3wIsSupported(3, 1)) { 
	               		ErrorDialog::createAndShow("OpenGL 3.1 is not supported on your computer. Please update your graphics drivers.");
	                	exit(0); 
	                	return;// -1; 
	        		}
	        		ARK2D::getLog()->i(StringUtil::append("Using OpenGL ", (const char*) glGetString(GL_VERSION)));
	        		ARK2D::getLog()->i(StringUtil::append("Using GLSL ", (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION)));
	        		//printf("OpenGL %s, GLSL %s\n",  glGetString(GL_VERSION), glGetString());

					// Setup OGL stuff.
					glEnable(GL_TEXTURE_2D);
					glViewport(0, 0, m_container->m_width, m_container->m_height);
					//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
					glClearColor(m_container->m_clearColor.getRed()/255.0f, m_container->m_clearColor.getGreen()/255.0f, m_container->m_clearColor.getBlue()/255.0f, m_container->m_clearColor.getAlpha()/255.0f);

					//	glViewport(0, 0, 1440, 900);

					glClear( GL_COLOR_BUFFER_BIT );
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					#ifndef NO_FIXED_FUNCTION_PIPELINE
						glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					#endif

					//glEnable(GL_SCISSOR_TEST);
					//glScissor(0, 0, m_container->m_width, m_container->m_height);

					//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);



				#endif


				
				ARK2D::getRenderer()->init();

				//m_container->m_bRunning = true;

			} 

			GameContainerARK2DResource GameContainerPlatform::getARK2DResourceWithLength(int resourceId, int resourceType) {
				// Load the DLL and default resources
				string resourceDebugStr = "getARK2DResource. Resource ID: ";
				resourceDebugStr += Cast::toString<int>(resourceId);
				resourceDebugStr += ". Resource Type: ";
				resourceDebugStr += Cast::toString<int>(resourceType);
				ARK2D::getLog()->v(resourceDebugStr);


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
				unsigned int resourceSizeInt = (unsigned int) resourceSize;
				unsigned int resourceSizeThis = resourceSizeInt+1;
				if (resourceType == ARK2D_RESOURCE_TYPE_SHADER) {
					resourceSizeThis--;
				}

				char* newPointer = (char*) malloc(resourceSizeThis);
				memcpy(newPointer, resourcePointer, resourceSizeThis);

				if (resourceType == ARK2D_RESOURCE_TYPE_TXT || resourceType == ARK2D_RESOURCE_TYPE_FNT) {
					char* newPointerEnd = newPointer+(resourceSizeThis-1);
					newPointerEnd = '\0';
				}

				FreeResource(resourceData);
				FreeLibrary(dllModule);

				GameContainerARK2DResource retval;
				retval.data = newPointer;
				retval.length = resourceSizeThis;

				return retval;
			}

			void* GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
				GameContainerARK2DResource retval = getARK2DResourceWithLength(resourceId, resourceType);
				return retval.data;
			}

			bool GameContainerPlatform::isAutoRotationAvailable() {
				BOOL success = TRUE;

				if (!IsWindows8OrGreater()) {
					return false;
				}

				// First, check the current state of the auto-rotation system.
				//AR_STATE autoRotationState;
				//ZeroMemory(&autoRotationState, sizeof(AR_STATE));
				//success = myGetAutoRotationState(&autoRotationState);
				AR_STATE autoRotationState;
				myGetAutoRotationState(&autoRotationState);
				// if auto rotation is disabled, use the current orientation instead of sensor value
				//if (autoRotationState != AR_ENABLED)
				//{

				/*ARK2D::getLog()->v("-----auto-rotation-----");
				ARK2D::getLog()->v(StringUtil::append("value: ", autoRotationState));
				ARK2D::getLog()->v(StringUtil::append("success: ", success ));
				ARK2D::getLog()->v(StringUtil::append("AR_ENABLED: ", Cast::boolToString( (autoRotationState & AR_ENABLED) )));
				ARK2D::getLog()->v(StringUtil::append("AR_DISABLED: ", Cast::boolToString( (autoRotationState & AR_DISABLED) )));
				ARK2D::getLog()->v(StringUtil::append("AR_SUPPRESSED: ", Cast::boolToString( (autoRotationState & AR_SUPPRESSED) )));
				ARK2D::getLog()->v(StringUtil::append("AR_REMOTESESSION: ", Cast::boolToString( (autoRotationState & AR_REMOTESESSION) )));
				ARK2D::getLog()->v(StringUtil::append("AR_MULTIMON: ", Cast::boolToString( (autoRotationState & AR_MULTIMON) )));
				ARK2D::getLog()->v(StringUtil::append("AR_NOSENSOR: ", Cast::boolToString( (autoRotationState & AR_NOSENSOR) )));
				ARK2D::getLog()->v(StringUtil::append("AR_NOT_SUPPORTED: ", Cast::boolToString( (autoRotationState & AR_NOT_SUPPORTED) )));
				ARK2D::getLog()->v(StringUtil::append("AR_DOCKED: ", Cast::boolToString( (autoRotationState & AR_DOCKED) )));
				ARK2D::getLog()->v(StringUtil::append("AR_LAPTOP: ", Cast::boolToString( (autoRotationState & AR_LAPTOP) )));

				if ((autoRotationState & AR_NOSENSOR) || (autoRotationState & AR_NOT_SUPPORTED)) {
					// If there's no sensor or rotation is not supported, we don't need 
					// to do anything about auto-rotation.
					return false;
				} else if (autoRotationState & AR_ENABLED) {
					return true;
				}*/
				if (autoRotationState == AR_ENABLED) {
					return true;
				}
				return false;
			}
			bool GameContainerPlatform::isAutoRotationEnabled() {
				if (isAutoRotationAvailable()) {
					MY_ORIENTATION_PREFERENCE prefs = MY_ORIENTATION_PREFERENCE_NONE;
					myGetDisplayAutoRotationPreferences(&prefs);
					if (prefs != MY_ORIENTATION_PREFERENCE_NONE) {
						return true;
					}
					return false;
				}
				return false;
			}
			void GameContainerPlatform::enableAutoRotation() {
				if (isAutoRotationAvailable()) {
					BOOL success = TRUE;

					MY_ORIENTATION_PREFERENCE prefs;
					success = myGetDisplayAutoRotationPreferences((MY_ORIENTATION_PREFERENCE*)&prefs);
					
					prefs = MY_ORIENTATION_PREFERENCE_NONE;
					success = mySetDisplayAutoRotationPreferences(prefs);
					// todo; check success var
				}
			}
			// in this circumstance, we don't force the orientation to anything...
			// we just want to keep it as-is. 
			// consider -- void GameContainerPlatform::lockAutoRotation() {} 
			void GameContainerPlatform::disableAutoRotation() {
				if (isAutoRotationAvailable()) {
					BOOL success = TRUE;
					
					MY_ORIENTATION_PREFERENCE prefs;
					success = myGetDisplayAutoRotationPreferences((MY_ORIENTATION_PREFERENCE*)&prefs);
					
					prefs = MY_ORIENTATION_PREFERENCE_LANDSCAPE;
					success = mySetDisplayAutoRotationPreferences(prefs);
					// todo; check success var
				}
			}

			void GameContainerPlatform::lockAutoRotation() {
				if (isAutoRotationAvailable()) {
					BOOL success = TRUE;
					
					MY_ORIENTATION_PREFERENCE prefs;
					success = myGetDisplayAutoRotationPreferences((MY_ORIENTATION_PREFERENCE*)&prefs);

					// Set the rotation modes your app can tolerate.
					int current = m_container->getScreenOrientation();
					if (current == GameContainer::ORIENTATION_LANDSCAPE) {
						prefs = MY_ORIENTATION_PREFERENCE_LANDSCAPE;
					} else if (current == GameContainer::ORIENTATION_PORTRAIT) {
						prefs = MY_ORIENTATION_PREFERENCE_PORTRAIT;
					}
					success = mySetDisplayAutoRotationPreferences(prefs);
					// todo; check success var
				}

			}


			int GameContainerPlatform::getSX() {
				int s_x = (m_screenResolutionRect.right/2) - (m_windowRect.right/2);
				return s_x;
			}
			int GameContainerPlatform::getSY() {
				//int s_y = (m_platformSpecific.m_screenResolutionRect.bottom/2) - (m_platformSpecific.m_windowRect.bottom/2);
				int s_y = (GetSystemMetrics(SM_CYFULLSCREEN)/2) - (m_windowRect.bottom/2);

				s_y -= (GetSystemMetrics(SM_CYBORDER)/2);
				//sy -= GetSystemMetrics(SM_CYSMCAPTION);
				return s_y;
			}
				
			HWND GameContainerPlatform::doCreateWindow() {
				int windowFlags = getWindowFlags2();
				int s_x = getSX();
				int s_y = getSY();
				#ifdef ARK2D_RENDERER_OPENGL
					return CreateWindow(
						TEXT("GameContainerWindows"), // lp Class Name
						TEXT(m_container->m_game.getTitle().c_str()), // lp Window Name
						windowFlags, // DW Style
						s_x, // x
						s_y, // y
						m_container->m_width, //m_windowRect.right - m_windowRect.left, // width
						m_container->m_height, // m_windowRect.bottom - m_windowRect.top, // height
						HWND_DESKTOP, // parent window
						NULL, // menu.
						m_hInstance, //m_platformSpecific.m_hInstance, // instance
						m_container//this  // LPVOID?
					);
				#elif defined(ARK2D_RENDERER_DIRECTX) 
					return CreateWindowEx(
						WS_EX_APPWINDOW, // lp Class Name
						TEXT(m_container->m_game.getTitle().c_str()), // lp Window Name
						TEXT(m_container->m_game.getTitle().c_str()),
						WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, // DW Style
						s_x, // x
						s_y, // y
						m_container->m_width, //m_windowRect.right - m_windowRect.left, // width
						m_container->m_height, // m_windowRect.bottom - m_windowRect.top, // height
						NULL, // parent window
						NULL, // menu.
						m_hInstance, //m_platformSpecific.m_hInstance, // instance
						m_container //this // LPVOID?
					); 
				#endif
				return NULL;
			}

			void GameContainer::start() {


				//char* fff = (char*) GameContainerPlatform::getARK2DResource(ARK2D_FONT_FNT, TYPE_FNT);
				//std::cout << fff << std::endl;

				//char* fff = (char*) GameContainerPlatform::getARK2DResource(ARK2D_TEST, ARK2D_RESOURCE_TYPE_TXT);
				//string s = string(fff)+ "123";
				//ErrorDialog::createAndShow(s);



				// Display modes!
				//! @todo: read the registry for display modes... O_O
				GameContainerDisplayMode dm_800x600x32;
				dm_800x600x32.width = 800;
				dm_800x600x32.height = 600;
				dm_800x600x32.bpp = 32;
				m_platformSpecific.m_availableDisplayModes.push_back(dm_800x600x32);

				HRESULT hr;

				#if defined(ARK2D_RENDERER_OPENGL)
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

					

				#elif defined(ARK2D_RENDERER_DIRECTX)

					// Initialize device-indpendent resources, such
				    // as the Direct2D factory.
				    /*hr = */m_platformSpecific.initDXDeviceIndependentResources();

					// Register the window class.
					m_platformSpecific.m_windowClass.cbSize        = sizeof(WNDCLASSEX);
					m_platformSpecific.m_windowClass.style		   = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			        m_platformSpecific.m_windowClass.lpfnWndProc   = StaticWndProc;
			        m_platformSpecific.m_windowClass.cbClsExtra    = 0;
			        m_platformSpecific.m_windowClass.cbWndExtra    = 0; //sizeof(LONG_PTR);
			        m_platformSpecific.m_windowClass.hInstance     = m_platformSpecific.m_hInstance;//HINST_THISCOMPONENT;
			        m_platformSpecific.m_windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);//IDI_APPLICATION);
			        m_platformSpecific.m_windowClass.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );	// black background //NULL;
			        m_platformSpecific.m_windowClass.lpszMenuName  = NULL;
			        m_platformSpecific.m_windowClass.lpszClassName = "GameContainerWindows";
			        m_platformSpecific.m_windowClass.hIconSm       = LoadIcon(NULL, IDI_WINLOGO);      // windows logo small icon
			        m_platformSpecific.m_windowClass.hIcon 		   = LoadIcon(NULL, IDI_APPLICATION);  // default icon

			       
				#endif

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

				// Because the CreateWindow function takes its size in pixels,
        		// obtain the system DPI and use it to scale the window size.
        		//FLOAT dpiX, dpiY;
        		//m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);


				

				//int s_x = m_platformSpecific.getSX();
				//int s_y = m_platformSpecific.getSY();
				//int windowFlags = m_platformSpecific.getWindowFlags2();

				//m_platformSpecific.m_hWindow = CreateWindow("GameContainer", // lp Class Name
				//string titleName = m_game.getTitle();  
				//const char* titleNameCStr = titleName.c_str();
				//LPCTSTR lpClassName = TEXT("GameContainerWindows"); 
				//LPCTSTR titleNameCStr = TEXT(titleName.c_str());
				
				m_platformSpecific.m_hWindow = m_platformSpecific.doCreateWindow();
				

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

				// Init 2-in-1 variables
				// TODO: This misbehaves on non-touch platforms, so take it out.
				// ARK2D::getLog()->i("Initialising 2-in-1 functionality... ");
				// int twoInOneValue = GetSystemMetrics(SM_CONVERTIBLESLATEMODE);
				// m_platformSpecific.m_twoInOneState = (twoInOneValue == 0) 
				// 	? GameContainerPlatform::TWOINONESTATE_TABLET 
				// 	: GameContainerPlatform::TWOINONESTATE_DESKTOP;
				// ARK2D::getLog()->i(StringUtil::append("SM_CONVERTIBLESLATEMODE: ", twoInOneValue));
				// ARK2D::getLog()->i(StringUtil::append("State: ", m_platformSpecific.m_twoInOneState));

				m_screenOrientationPrevious = getScreenOrientation();
				// Disable auto-rotation on the device.
				//m_platformSpecific.disableAutoRotation();

				// populate the gamepads.
				ARK2D::getLog()->i("Initialising Gamepads... ");
				initGamepads();
				ARK2D::getLog()->i("done.");

				// Init rendering
				#if defined( ARK2D_RENDERER_DIRECTX )
					ARK2D::getLog()->i("Initialising DirectX... ");
					bool b = m_platformSpecific.initDXDeviceResources();
					if (!b) { 
						ErrorDialog::createAndShow("Could not initialise DirectX 11. See log output.");
						exit(0);
					}
					ARK2D::getRenderer()->init();
					enable2D();
					ARK2D::getLog()->i("Initialised DirectX! ");
				#elif defined( ARK2D_RENDERER_OPENGL )
					ARK2D::getLog()->i("Initialising OpenGL... ");
					m_platformSpecific.enableOpenGL(m_platformSpecific.m_hWindow, &m_platformSpecific.m_hDeviceContext, &m_platformSpecific.m_hRenderingContext);
					this->enable2D();
					ARK2D::getLog()->i("done.");

					Image::showAnyGlErrorAndExit();
				#endif

				// Load default Font - relies on Image so must be done after OpenGL is initted.
				//BMFont* fnt = new BMFont("data/fonts/default.fnt", "data/fonts/default.png");
				//Image* fntImg = new Image((unsigned int) ARK2D_FONT_PNG, ARK2D_RESOURCE_TYPE_PNG);
				if (m_willLoadDefaultFont) { 
					ARK::Font::BMFont* fnt = new ARK::Font::BMFont(ARK2D_FONT_FNT, ARK2D_FONT_PNG, ARK2D_RESOURCE_TYPE_PNG);
					//ARK::Font::BMFont* fnt = Resource::get("ark2d/fonts/default.fnt")->asFont()->asBMFont();
					m_graphics.m_DefaultFont = fnt;
					m_graphics.m_Font = fnt; 
				} else {
					m_graphics.m_DefaultFont = NULL;
					m_graphics.m_Font = NULL;
				}

				Image::showAnyGlErrorAndExit();

				// Enable OpenAL
				ARK2D::getLog()->i("Initialising OpenAL... ");
				this->enableOpenAL();
				ARK2D::getLog()->i("done.");


				

				ARK2D::getLog()->i("Initialising Window... ");
				ARK2D::getLog()->i("Show..."); 

				#if defined(ARK2D_RENDERER_OPENGL)
					ShowWindow(m_platformSpecific.m_hWindow, SW_SHOWNORMAL);
				#elif defined(ARK2D_RENDERER_DIRECTX)
					ShowWindow(m_platformSpecific.m_hWindow, SW_SHOW);
					SetForegroundWindow(m_platformSpecific.m_hWindow);
					SetFocus(m_platformSpecific.m_hWindow);
				#endif
					m_platformSpecific.myRegisterTouchWindow(m_platformSpecific.m_hWindow, 0);

				ARK2D::getLog()->i("Update...");
				UpdateWindow(m_platformSpecific.m_hWindow);
				//ClipCursor(&m_windowRect);
				ARK2D::getLog()->i("done.");



				m_bRunning = true;

				

				ARK2D::getLog()->i("Initialising ");
				ARK2D::getLog()->i(m_game.getTitle());
				ARK2D::getLog()->i("...");
				m_game.init(this);

				ARK2D::getLog()->i("Initialised ");
				ARK2D::getLog()->i(m_game.getTitle());
				ARK2D::getLog()->i("...");

				Image::showAnyGlErrorAndExit();

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

			//	ARK2D::getLog()->v("Starting Loop");
				while (this->m_bRunning == true)
				{
					//ARK2D::getLog()->v("Tick");
					m_timer.tick();

					//ARK2D::getLog()->v("Peek");
					MSG msg;
					while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // process events one by one.
						if ( msg.message == WM_QUIT ) { this->m_bRunning = false; }
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}

					m_platformSpecific.m_hotplugGamepadsTimer += m_timer.getDelta();
					if (m_platformSpecific.m_hotplugGamepadsTimer >= 1.0f) {
						// initGamepads();
						// hotplugging disabled... :( 
						m_platformSpecific.m_hotplugGamepadsTimer -= 1.0f;
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
					//ARK2D::getLog()->v("Update Log");
					ARK2D::getLog()->update();



					//int delta = (int) (m_timer.getDelta() * 1000);
					//ARK2D::getLog()->v("Update Game");
					m_game.preUpdate(this, &m_timer);
					m_game.update(this, &m_timer);
					m_game.postUpdate(this, &m_timer);
					m_input.clearKeyPressedRecord();
					for (unsigned int i = 0; i < m_gamepads.size(); i++) {
						m_gamepads.at(i)->clearButtonPressedRecord();
					}
					Image::showAnyGlErrorAndExit();
					

					//this->m_game->update(this, myAverageDelta); // fix at 60 fps. bug.
					//this->m_game->update(this, 0.017); // fix at 60 fps. bug.


					//m_lastTime = GetTickCount() / 1000.0f;
					//std::cout << this->getGame()->getTitle().c_str() << std::endl;

					//float elapsedTime = this->getElapsedMilliseconds();
					//this->m_game->update(this, (17 / 1000.f)); // fix at 60 fps. bug.
					//this->m_game->update(this, (float) (1.f / 60.f)); // fix at 60 fps. bug.
					//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					
					RendererStats::reset();

					//ARK2D::getLog()->v("Render Reset");
					#if defined(ARK2D_RENDERER_OPENGL)
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					#elif defined(ARK2D_RENDERER_DIRECTX)
						/*D2D1_COLOR_F d;
						d.r = 1.0f;
						d.g = 0.0f;
						d.b = 0.5f; 
						d.a = 1.0f;
						//device.Clear(ClearFlags.Target | ClearFlags.ZBuffer, Color.CornflowerBlue, 1.0f, 0);
						//m_platformSpecific.m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
						m_platformSpecific.m_pRenderTarget->BeginDraw();
						m_platformSpecific.m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
						//m_platformSpecific.m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
						m_platformSpecific.m_pRenderTarget->Clear(d);
						m_platformSpecific.m_pRenderTarget->EndDraw();*/
						m_platformSpecific.beginDXFrame();
					#endif
					
					//ARK2D::getLog()->v("Pre-render Game");
					m_game.preRender(this, &m_graphics);
					//ARK2D::getLog()->v("Render Game");
					m_game.render(this, &m_graphics);
					//ARK2D::getLog()->v("Render Log");
					ARK2D::getLog()->render(this, &m_graphics);
					//ARK2D::getLog()->v("Post-render Game");
					m_game.postRender(this, &m_graphics); 
					//if (m_showingFPS) { renderFPS(); }

					
					Image::showAnyGlErrorAndExit();
					//myLastRenderTime = this->time();

					//m_graphics.flush(); 
					//m_graphics.finish();
					
					//glFinish(); 
					//ARK2D::getLog()->v("Swap Buffers");
					swapBuffers();

					//Sleep(delta/2);
				//	Sleep(10);
					m_timer.limit(60);
					//m_timer.sleep(10);
				}

				// Deinit rendering
				#if defined( ARK2D_RENDERER_DIRECTX )
					ARK2D::getLog()->i("TODO: Deinitialising DirectX... ");
					m_platformSpecific.deinitDXDeviceResources();
					// Umm...
					
				#elif defined( ARK2D_RENDERER_OPENGL )
					ARK2D::getLog()->i("Deinitialising OpenGL...");
					this->disable2D();
					m_platformSpecific.disableOpenGL( m_platformSpecific.m_hWindow, m_platformSpecific.m_hDeviceContext, m_platformSpecific.m_hRenderingContext );
					ARK2D::getLog()->i("...done!");
				#endif

				

				

				// destroy the window explicitly
				DestroyWindow( m_platformSpecific.m_hWindow );
				ARK2D::getLog()->i("Deinitialised Window...");
				exit(0);
			}



			void GameContainer::close() const {
				PostMessageA(m_platformSpecific.m_hWindow, WM_QUIT, 0, 0);
			}


			void GameContainer::swapBuffers() {
				#if defined( ARK2D_RENDERER_DIRECTX )
					m_platformSpecific.endDXFrame();
				#elif defined( ARK2D_RENDERER_OPENGL )
					SwapBuffers(m_platformSpecific.m_hDeviceContext);
				#endif
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
				#ifdef ARK2D_RENDERER_OPENGL
					wglMakeCurrent( NULL, NULL );
					wglDeleteContext( hRC );
					ReleaseDC( hWnd, hDC );
				#endif
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
				//#ifdef ARK2D_WINDOWS_VS
				//	return "data/"; 
				//#else 
				return "./data/";
				//#endif
			}
		}
	}

#endif
