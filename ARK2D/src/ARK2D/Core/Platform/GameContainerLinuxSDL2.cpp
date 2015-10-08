
#include "../GameContainer.h"
#include "GameContainerLinux.h"



#if defined(ARK2D_UBUNTU_LINUX) && defined(ARK2D_SDL2)

#include "../../Geometry/Shape.h"
#include "../../Geometry/Circle.h"
#include "../../Geometry/Line.h"
#include "../../Geometry/Rectangle.h"

	#include <dirent.h>
	#include <errno.h>
	#include <fcntl.h>
	#include <linux/input.h>
	


	// http://www.libsdl.org/tmp/SDL/src/video/x11/SDL_x11window.c  --//--  http://hg.libsdl.org/SDL/file/704a0bfecf75/src/video/x11/SDL_x11window.c
 
	namespace ARK {
		namespace Core {

			pthread_mutex_t GameContainerPlatform::devicesMutex;
			unsigned int GameContainerPlatform::numDevices = 0;
			unsigned int GameContainerPlatform::nextDeviceID = 0;
			
			Gamepad_queuedEvent* GameContainerPlatform::eventQueue = NULL;
			size_t GameContainerPlatform::eventQueueSize = 0;
			size_t GameContainerPlatform::eventCount = 0;
			pthread_mutex_t GameContainerPlatform::eventQueueMutex;

			GameContainer::GameContainer(Game& game, int width, int height, int bpp, bool fullscreen):
				m_timer(),
				m_game(game),
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
				m_platformSpecific.m_iconPath = "";
				m_platformSpecific.m_resourcePath = "data/";
				m_platformSpecific.m_globalMouseX = 0;
				m_platformSpecific.m_globalMouseY = 0;
				m_platformSpecific.m_initialisedGamepads = false;

				// https://wiki.libsdl.org/SDLKeycodeLookup
				m_platformSpecific.keycodemap = new map<int, int>();
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_ESCAPE, (int) SDLK_ESCAPE));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_SPACE, (int)SDLK_SPACE));
				
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_1, (int)SDLK_1));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_2, (int)SDLK_2));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_3, (int)SDLK_3));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_4, (int)SDLK_4));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_5, (int)SDLK_5));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_6, (int)SDLK_6));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_7, (int)SDLK_7));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_8, (int)SDLK_8));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_9, (int)SDLK_9));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_0, (int)SDLK_0));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_HYPHEN, (int)SDLK_MINUS));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_EQUALS, (int)SDLK_EQUALS));

				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_Q, (int)SDLK_q));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_W, (int)SDLK_w));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_E, (int)SDLK_e));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_R, (int)SDLK_r));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_T, (int)SDLK_t));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_Y, (int)SDLK_y));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_U, (int)SDLK_u));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_I, (int)SDLK_i));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_O, (int)SDLK_o));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_P, (int)SDLK_p));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_LEFT_SQUARE_BRACKET, (int) SDLK_LEFTBRACKET));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_RIGHT_SQUARE_BRACKET, (int)SDLK_RIGHTBRACKET));
				
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_A, (int)SDLK_a));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_S, (int)SDLK_s));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_D, (int)SDLK_d));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F, (int)SDLK_f));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_G, (int)SDLK_g));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_H, (int)SDLK_h));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_J, (int)SDLK_j));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_K, (int)SDLK_k));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_L, (int)SDLK_l));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_SEMICOLON, (int)SDLK_SEMICOLON));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_APOSTROPHE, (int)SDLK_QUOTE));
				
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_Z, (int)SDLK_z));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_X, (int)SDLK_x));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_C, (int)SDLK_c)); 
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_V, (int)SDLK_v));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_B, (int)SDLK_b));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_N, (int)SDLK_n));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_M, (int)SDLK_m));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_COMMA, (int)SDLK_COMMA));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_PERIOD, (int)SDLK_PERIOD));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_FORWARD_SLASH, (int)SDLK_SLASH));
				
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_UP, (int)SDLK_UP));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_LEFT, (int)SDLK_LEFT));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_RIGHT, (int)SDLK_RIGHT));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_DOWN, (int)SDLK_DOWN));

				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F1, (int)SDLK_F1));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F2, (int)SDLK_F2));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F3, (int)SDLK_F3));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F4, (int)SDLK_F4));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F5, (int)SDLK_F5));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F6, (int)SDLK_F6));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F7, (int)SDLK_F7));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F8, (int)SDLK_F8));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F9, (int)SDLK_F9));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F10, (int)SDLK_F10));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F11, (int)SDLK_F11));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F12, (int)SDLK_F12));

				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_HOME, (int)SDLK_HOME));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_PAGEUP, (int)SDLK_PAGEUP));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_END,(int) SDLK_END));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_PAGEDOWN, (int)SDLK_PAGEDOWN));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_INSERT, (int)SDLK_INSERT));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_DELETE, (int)SDLK_DELETE));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_LCONTROL, (int)(int)SDLK_LCTRL));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_RCONTROL, (int)SDLK_RCTRL));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_ALT, (int) SDLK_LALT));
				//m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_RALT, 108));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_LWIN, (int)SDLK_LGUI));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_RWIN, (int)SDLK_RGUI));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_TAB, (int)SDLK_TAB));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_CAPSLOCK, (int) SDLK_CAPSLOCK));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_LSHIFT, (int) SDLK_LSHIFT));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_RSHIFT, (int) SDLK_RSHIFT));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_BACKSPACE, (int) SDLK_BACKSPACE));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_ENTER, (int)SDLK_RETURN));


				m_platformSpecific.keycodemap_reverse = new map<int, int>(); 
				map<int, int>::iterator it = m_platformSpecific.keycodemap->begin();
				while(it != m_platformSpecific.keycodemap->end() ) {
					m_platformSpecific.keycodemap_reverse->insert(std::make_pair(it->second, it->first));
					it++;
				}
					
				
				m_strTitle = m_game.getTitle();
				
				m_input.setGameContainer(this);

				ARK2D::s_container = this;
				ARK2D::s_game = &m_game;
				ARK2D::s_graphics = &m_graphics;
				ARK2D::s_input = &m_input;
				ARK2D::s_log = ARK::Util::Log::getInstance(); 
				scene = new Scene();

				m_graphics.preinit();

			}

			void GameContainer::setIcon(const string& icon) {
				m_platformSpecific.m_iconPath = icon;
			}

			void GameContainer::setCursorVisible(bool b) {
 				if (!b) {
 					SDL_ShowCursor(SDL_DISABLE);
 				} else {
 					SDL_ShowCursor(SDL_ENABLE);
 				}
			}

			void GameContainerPlatform::initGamepads() {
				if (!m_initialisedGamepads) {
					Gamepad::initMapping();

					pthread_mutexattr_t recursiveLock;
					pthread_mutexattr_init(&recursiveLock);
					pthread_mutexattr_settype(&recursiveLock, PTHREAD_MUTEX_RECURSIVE);
					pthread_mutex_init(&devicesMutex, &recursiveLock);
					pthread_mutex_init(&eventQueueMutex, &recursiveLock);
					detectGamepads();
					m_initialisedGamepads = true;
				}
			}
			
			void GameContainerPlatform::detectGamepads() {
				struct input_id id;
				DIR * dev_input;
				struct dirent * entity;
				unsigned int charsConsumed;
				int num;
				int fd;
				int evCapBits[(EV_CNT - 1) / sizeof(int) * 8 + 1];
				int evKeyBits[(KEY_CNT - 1) / sizeof(int) * 8 + 1];
				int evAbsBits[(ABS_CNT - 1) / sizeof(int) * 8 + 1];
				char fileName[PATH_MAX];
				bool duplicate; 
				unsigned int gamepadIndex;
				struct stat statBuf;
				Gamepad* gamepad;
				char name[128];
				char * description;
				int bit;
				time_t currentTime;
				static time_t lastInputStatTime;
				
				//if (!m_initialisedGamepads) {
				//	return;
				//}
				
				pthread_mutex_lock(&devicesMutex);
				 
				dev_input = opendir("/dev/input"); 
				currentTime = time(NULL);
				if (dev_input != NULL) {
					while ((entity = readdir(dev_input)) != NULL) {
						charsConsumed = 0;
						if (sscanf(entity->d_name, "event%d%n", &num, &charsConsumed) && charsConsumed == strlen(entity->d_name)) {
							snprintf(fileName, PATH_MAX, "/dev/input/%s", entity->d_name);
							if (stat(fileName, &statBuf) || statBuf.st_mtime < lastInputStatTime) {
								continue;
							}
							
							duplicate = false;
							for (gamepadIndex = 0; gamepadIndex < numDevices; gamepadIndex++) {
								if (!strcmp(m_container->m_gamepads[gamepadIndex]->path, fileName)) {
									duplicate = true;
									break;
								}
							} 
							if (duplicate) {
								continue; 
							}
							  
							fd = open(fileName, O_RDONLY, 0);
							memset(evCapBits, 0, sizeof(evCapBits)); 
							memset(evKeyBits, 0, sizeof(evKeyBits));
							memset(evAbsBits, 0, sizeof(evAbsBits));
							if (ioctl(fd, EVIOCGBIT(0, sizeof(evCapBits)), evCapBits) < 0 ||
							    ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(evKeyBits)), evKeyBits) < 0 ||
							    ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(evAbsBits)), evAbsBits) < 0) {
								close(fd);
								continue;
							}
							if (!test_bit(EV_KEY, evCapBits) || !test_bit(EV_ABS, evCapBits) ||
							    !test_bit(ABS_X, evAbsBits) || !test_bit(ABS_Y, evAbsBits) ||
							    (!test_bit(BTN_TRIGGER, evKeyBits) && !test_bit(BTN_A, evKeyBits) && !test_bit(BTN_1, evKeyBits))) {
								close(fd);
								continue;
							}
							
							gamepad = new Gamepad();
							gamepad->id = nextDeviceID++;
							m_container->m_gamepads.push_back(gamepad);
							numDevices++;

							gamepad->fd = fd;
							gamepad->path = (char*) malloc(strlen(fileName) + 1);
							strcpy(gamepad->path, fileName);

							memset(gamepad->buttonMap, 0xFF, sizeof(gamepad->buttonMap));
							memset(gamepad->axisMap, 0xFF, sizeof(gamepad->axisMap));
													
							if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) > 0) {
								description = (char*) malloc(strlen(name) + 1);
								strcpy(description, name);
							} else {
								description = (char*) malloc(strlen(fileName) + 1);
								strcpy(description, fileName);
							}
							gamepad->description = description;
							
							if (!ioctl(fd, EVIOCGID, &id)) {
								gamepad->vendorId = id.vendor; 
								gamepad->productId = id.product;
							} else {
								gamepad->vendorId = gamepad->productId = 0;
							}
							
							memset(evKeyBits, 0, sizeof(evKeyBits));
							memset(evAbsBits, 0, sizeof(evAbsBits));
							ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(evKeyBits)), evKeyBits);
							ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(evAbsBits)), evAbsBits);
							
							gamepad->numAxes = 0;
							for (bit = 0; bit < ABS_CNT; bit++) {
								if (test_bit(bit, evAbsBits)) {
									if (ioctl(fd, EVIOCGABS(bit), &gamepad->axisInfo[bit]) < 0 ||
										  gamepad->axisInfo[bit].minimum == gamepad->axisInfo[bit].maximum) {
										continue;
									}

									GamepadAxis* axis = new GamepadAxis();
									axis->id = gamepad->numAxes;
									//axis->axisId = bit; 
									//axis->rangeMin = gamepad->axisInfo[bit].minimum
									//axis->rangeMax = gamepad->axisInfo[bit].maximum
									gamepad->axes.push_back(axis);

									gamepad->axisMap[bit] = gamepad->numAxes;
									gamepad->numAxes++;

									
								}
							}

							gamepad->numButtons = 0;
							for (bit = BTN_MISC; bit < KEY_CNT; bit++) {
								if (test_bit(bit, evKeyBits)) {

									GamepadButton* button = new GamepadButton();
									button->id = gamepad->numButtons;
									button->down = false;  
									gamepad->buttons.push_back(button);

									gamepad->buttonMap[bit - BTN_MISC] = gamepad->numButtons;
									gamepad->numButtons++;

									
								}
							}
							  
							if (strcmp(gamepad->description, "Microsoft X-Box 360 pad") == 0 || 
								strcmp(gamepad->description, "Xbox Gamepad (userspace driver)") == 0 || 
								strcmp(gamepad->description, "Sony Computer Entertainment Wireless Controller") == 0) {
								GamepadButton* button = NULL;

								button = new GamepadButton();
								button->id = Gamepad::DPAD_LEFT;
								button->down = false;  
								gamepad->buttons.push_back(button);

								button = new GamepadButton();
								button->id = Gamepad::DPAD_RIGHT;
								button->down = false;  
								gamepad->buttons.push_back(button);

								button = new GamepadButton();
								button->id = Gamepad::DPAD_UP;
								button->down = false;  
								gamepad->buttons.push_back(button);

								button = new GamepadButton();
								button->id = Gamepad::DPAD_DOWN;
								button->down = false;  
								gamepad->buttons.push_back(button);
							}

							//gamepad->axisStates = calloc(sizeof(float), gamepad->numAxes);
							//gamepad->buttonStates = calloc(sizeof(bool), gamepad->numButtons);
							
							//if (Gamepad_deviceAttachCallback != NULL) {
							//	Gamepad_deviceAttachCallback(gamepad, Gamepad_deviceAttachContext);
							//}
							ARK2D::getLog()->i(StringUtil::append("Gamepad attached: ", gamepad->description));

							// Give event to game
							Game* g = ARK2D::getGame();
							GamepadListener* gl = NULL;
							gl = dynamic_cast<GamepadListener*>(g);
							if (gl != NULL) {
								gl->gamepadDisconnected(gamepad);
							}
								
							pthread_create(&gamepad->thread, NULL, &deviceThread, gamepad);
						}
					}
					closedir(dev_input); 
				} 
				
				lastInputStatTime = currentTime;
				pthread_mutex_unlock(&devicesMutex);
			}

			void* GameContainerPlatform::deviceThread(void* context) {
				GameContainer* container = ARK2D::getContainer();
				Gamepad* device;
				unsigned int gamepadIndex; 
				struct input_event event;
				
				device = (Gamepad*) context; 
				//devicePrivate = device->privateData;
				
				while (read(device->fd, &event, sizeof(struct input_event)) > 0) {
					if (event.type == EV_ABS) {
					//	ARK2D::getLog()->i(StringUtil::append("axis event 1: ", event.code));

						float value;
						
						if (event.code > ABS_MAX || device->axisMap[event.code] == -1) {
							continue;
						}
						
						// remap axes
						unsigned int axis = device->axisMap[event.code];
						if (strcmp(device->description, "Microsoft X-Box 360 pad") == 0 || 
							strcmp(device->description, "Xbox Gamepad (userspace driver)") == 0) {
 							if (axis == 2) { axis = Gamepad::TRIGGER_1; }
 							else if (axis == 4) { axis = Gamepad::ANALOG_STICK_2_Y; }
 							else if (axis == 3) { axis = Gamepad::ANALOG_STICK_2_X; }
 						} else if (strcmp(device->description, "Sony Computer Entertainment Wireless Controller") == 0) {
 							if (axis == 2) {      axis = Gamepad::ANALOG_STICK_2_X; }
 							else if (axis == 3) { axis = Gamepad::TRIGGER_1; }
 							else if (axis == 4) { axis = Gamepad::TRIGGER_2; }
 							else if (axis == 5) { axis = Gamepad::ANALOG_STICK_2_Y; } 
 						}

 						// dpads are on an axis
 						if (strcmp(device->description, "Microsoft X-Box 360 pad") == 0 || 
							strcmp(device->description, "Xbox Gamepad (userspace driver)") == 0 || 
							strcmp(device->description, "Sony Computer Entertainment Wireless Controller") == 0) {

 							if (axis == 6 || axis == 7) {
 								value = (event.value - device->axisInfo[event.code].minimum) / (float) (device->axisInfo[event.code].maximum - device->axisInfo[event.code].minimum) * 2.0f - 1.0f;			
 								
 								ARK2D::getLog()->e(StringUtil::append("axis: ", axis));
								ARK2D::getLog()->e(StringUtil::appendf("value: ", value));

								if (axis == 6) {
									if (value == 0.0f && device->isButtonDown(Gamepad::DPAD_LEFT)) {
										device->releaseButton(Gamepad::DPAD_LEFT);
									} else if (value == 0.0f && device->isButtonDown(Gamepad::DPAD_RIGHT)) {
										device->releaseButton(Gamepad::DPAD_RIGHT);
									}
									if (value == -1.0f && !device->isButtonDown(Gamepad::DPAD_LEFT)) {
										device->pressButton(Gamepad::DPAD_LEFT);
									} else if (value == 1.0f && !device->isButtonDown(Gamepad::DPAD_RIGHT)) {
										device->pressButton(Gamepad::DPAD_RIGHT);
									}
								} else if (axis == 7) {
									if (value == 0.0f && device->isButtonDown(Gamepad::DPAD_UP)) {
										device->releaseButton(Gamepad::DPAD_UP);
									} else if (value == 0.0f && device->isButtonDown(Gamepad::DPAD_DOWN)) {
										device->releaseButton(Gamepad::DPAD_DOWN);
									}
									if (value == -1.0f && !device->isButtonDown(Gamepad::DPAD_UP)) {
										device->pressButton(Gamepad::DPAD_UP);
									} else if (value == 1.0f && !device->isButtonDown(Gamepad::DPAD_DOWN)) {
										device->pressButton(Gamepad::DPAD_DOWN); 
									}
								}
								continue;

 							}
						}
						
						value = (event.value - device->axisInfo[event.code].minimum) / (float) (device->axisInfo[event.code].maximum - device->axisInfo[event.code].minimum) * 2.0f - 1.0f;
						container->m_platformSpecific.queueAxisEvent(device,
						               event.time.tv_sec + event.time.tv_usec * 0.000001,
						               axis,
						               value,
						              // device->axisStates[device->axisMap[event.code]]);
						               device->axes[axis]->value);
						//device->axisStates[device->axisMap[event.code]] = value;
						device->axes[axis]->value = value;
 
						//ARK2D::getLog()->e(StringUtil::append("axis: ", axis));
						//ARK2D::getLog()->e(StringUtil::appendf("value: ", value));
						
					} else if (event.type == EV_KEY) {
						//ARK2D::getLog()->i(StringUtil::append("button event 1: ", event.code));
						if (event.code < BTN_MISC || event.code > KEY_MAX || device->buttonMap[event.code - BTN_MISC] == -1) {
							continue;
						}
						 
						container->m_platformSpecific.queueButtonEvent(device,
						                 event.time.tv_sec + event.time.tv_usec * 0.000001,
						                 device->buttonMap[event.code - BTN_MISC],
						                 !!event.value);
						
						//device->buttonStates[device->buttonMap[event.code - BTN_MISC]] = !!event.value;
						bool down = !!event.value;
						unsigned int key = device->buttonMap[event.code - BTN_MISC];
						if (strcmp(device->description, "Microsoft X-Box 360 pad") == 0 || 
							strcmp(device->description, "Xbox Gamepad (userspace driver)") == 0) {
							if (key == 8) { key = Gamepad::BUTTON_ACTIVATE; }
							else if (key == 9) { key = Gamepad::BUTTON_L3; }
							else if (key == 10) { key = Gamepad::BUTTON_R3; }
						}
						else if (strcmp(device->description, "Sony Computer Entertainment Wireless Controller") == 0) {
							if (key == 1) { key = Gamepad::BUTTON_A; }
							else if (key == 2) { key = Gamepad::BUTTON_B; }
							else if (key == 0) { key = Gamepad::BUTTON_X; }
							else if (key == 12) { key = Gamepad::BUTTON_ACTIVATE; }
							else if (key == 6) { /* left trigger */ key = Gamepad::BUTTON_NULL; }
							else if (key == 7) { /* right trigger */ key = Gamepad::BUTTON_NULL; }
							else if (key == 13) { /* touch pad */ key = Gamepad::BUTTON_NULL; }
							else if (key == 8) { key = Gamepad::BUTTON_BACK; }
							else if (key == 9) { key = Gamepad::BUTTON_START; } 
							else if (key == 10) { key = Gamepad::BUTTON_L3; }
							else if (key == 11) { key = Gamepad::BUTTON_R3; } 
						}

						//ARK2D::getLog()->i(StringUtil::append("button event: ", key));
						if (key < device->buttons.size()) { 

							if (!down && device->buttons[key]->down) {
								device->releaseButton(key);
							} else if (down && !device->buttons[key]->down) {
								device->pressButton(key);
							}

						}

						
					}
				}  
				
				ARK2D::getLog()->i(StringUtil::append("Gamepad detached: ", device->description));
				container->m_platformSpecific.queueEvent(device->id, GAMEPAD_EVENT_DEVICE_REMOVED, device);
				
				pthread_mutex_lock(&devicesMutex);
				for (gamepadIndex = 0; gamepadIndex < numDevices; gamepadIndex++) {
					if (container->m_gamepads[gamepadIndex] == device) {
						/*unsigned int gamepadIndex2;
						
						numDevices--;
						for (gamepadIndex2 = gamepadIndex; gamepadIndex2 < numDevices; gamepadIndex2++) {
							Gamepad* pad2 = container->m_gamepads[gamepadIndex2 + 1];
							container->m_gamepads[gamepadIndex2] = pad2;
						}
						gamepadIndex--;*/
						Gamepad* pad = container->m_gamepads[gamepadIndex];
						container->m_gamepads.erase(container->m_gamepads.begin() + gamepadIndex);
						numDevices--;

						// Give event to game
						Game* g = ARK2D::getGame();
						GamepadListener* gl = NULL;
						gl = dynamic_cast<GamepadListener*>(g);
						if (gl != NULL) {
							gl->gamepadDisconnected(pad);
						}

						break;
					}
				}


				pthread_mutex_unlock(&devicesMutex);
				
				return NULL;
			}



			void GameContainerPlatform::queueEvent(unsigned int deviceID, enum Gamepad_eventType eventType, void * eventData) {
				struct Gamepad_queuedEvent queuedEvent;
				
				queuedEvent.deviceID = deviceID; 
				queuedEvent.eventType = eventType;
				queuedEvent.eventData = eventData;
				 
				pthread_mutex_lock(&eventQueueMutex);
				if (eventCount >= eventQueueSize) {
					eventQueueSize = eventQueueSize == 0 ? 1 : eventQueueSize * 2;
					eventQueue = (Gamepad_queuedEvent*) realloc(eventQueue, sizeof(struct Gamepad_queuedEvent) * eventQueueSize);
				}
				eventQueue[eventCount++] = queuedEvent;
				pthread_mutex_unlock(&eventQueueMutex);
			}

			void GameContainerPlatform::queueAxisEvent(struct Gamepad* device, double timestamp, unsigned int axisID, float value, float lastValue) {
				struct Gamepad_axisEvent * axisEvent;
				 
				axisEvent = (Gamepad_axisEvent*) malloc(sizeof(struct Gamepad_axisEvent));
				axisEvent->device = device;
				axisEvent->timestamp = timestamp;
				axisEvent->axisID = axisID; 
				axisEvent->value = value;
				axisEvent->lastValue = lastValue;
				
				queueEvent(device->id, GAMEPAD_EVENT_AXIS_MOVED, axisEvent);
			}
 
			
			void GameContainerPlatform::queueButtonEvent(struct Gamepad* device, double timestamp, unsigned int buttonID, bool down) {
				struct Gamepad_buttonEvent * buttonEvent;
				
				buttonEvent = (Gamepad_buttonEvent*) malloc(sizeof(struct Gamepad_buttonEvent));
				buttonEvent->device = device;
				buttonEvent->timestamp = timestamp;
				buttonEvent->buttonID = buttonID;
				buttonEvent->down = down;
				
				queueEvent(device->id, down ? GAMEPAD_EVENT_BUTTON_DOWN : GAMEPAD_EVENT_BUTTON_UP, buttonEvent);
			}
			void GameContainerPlatform::setFocus(bool b) {
				if (!b) {
					SDL_MinimizeWindow(window);
				} else {
					// ... 
				}
			}

			void GameContainer::start() {

				// Seed the random
				MathUtil::seedRandom();

				//Initialize SDL
				if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
				{
					printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
					exit(0);
				}
				
				//const SDL_VideoInfo* info = NULL;
				//info = SDL_GetVideoInfo();
				//if (!info) {
				//	fprintf( stderr, "Video query failed: %s\n", SDL_GetError() );
				//}

				SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
				SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
				SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
				SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
				SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
				SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
				SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
				SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 );

				//int flags = SDL_OPENGL | SDL_WINDOWED;

				//if( SDL_SetVideoMode( m_originalWidth, m_originalHeight, m_container->m_bpp, flags ) == 0 ) {
        			// This could happen for a variety of reasons, including DISPLAY not being set, the specified resolution not being available, etc.
        		//	fprintf( stderr, "Video mode set failed: %s\n", SDL_GetError() );
        		//	exit(0); 
    			//}


				// Create window
		        m_platformSpecific.window = SDL_CreateWindow( m_strTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_originalWidth, m_originalHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		        if( m_platformSpecific.window == NULL )
		        {
		            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		            exit(0);
		        }

		        m_platformSpecific.glContext = SDL_GL_CreateContext(m_platformSpecific.window);

		        // Get current display mode of all displays.
		        SDL_DisplayMode current;
				for(unsigned int i = 0; i < SDL_GetNumVideoDisplays(); ++i) 
				{
					int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);
					if (should_be_zero != 0) { 
						SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());
					} else { 
						SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", i, current.w, current.h, current.refresh_rate);
						m_screenWidth = current.w;
						m_screenHeight = current.h;
					}
				}


		        //SDL_SetWindowGrab(m_platformSpecific.window, SDL_TRUE);

				//SDL_GetCurrentDisplayMode
				/*int initialWindowX = initOffsetX + (displayWidth / 2) - (m_originalWidth / 2);
				int initialWindowY = initOffsetY + (displayHeight / 2) - (m_originalHeight / 2);	
				m_screenWidth = displayWidth;
				m_screenHeight = displayHeight;
				ARK2D::getLog()->i(StringUtil::append("Screen Width: ", displayWidth));
				ARK2D::getLog()->i(StringUtil::append("Screen Height: ", displayHeight));
				ARK2D::getLog()->i(StringUtil::append("Window X: ", initialWindowX));
				ARK2D::getLog()->i(StringUtil::append("Window Y: ", initialWindowY)); 
				*/

				

				// set icon
				if (m_platformSpecific.m_iconPath.length() > 0) {
					string thisIcon = m_platformSpecific.m_iconPath;
					string thisIconExtension = StringUtil::getExtension(thisIcon);
					if (thisIconExtension == "ico") {
						thisIcon = thisIcon.substr(0, thisIcon.rfind(".")) + ".png";
					}

					ARK2D::getLog()->e(StringUtil::append("Setting Window icon: ", thisIcon));
					PNGImage* png = new PNGImage(thisIcon);
					png->load();
					if (!png->isRGBA()) {  
						ARK2D::getLog()->e("Could not set window icon");
					} else {
						SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(png->getImageData(), png->getWidth(), png->getHeight(), 32, png->getWidth()*4, 0x0000FF00,0x00FF0000,0xFF000000,0);
						SDL_SetWindowIcon(m_platformSpecific.window, surface); 
						SDL_FreeSurface(surface);
					}

					
				}

				// make the window appear... 
				//XMapWindow(m_platformSpecific.dpy, m_platformSpecific.win);
 
				// title bar name  
				//XStoreName(m_platformSpecific.dpy, m_platformSpecific.win, m_strTitle.c_str());

				resizeWindowToFitViewport();

				// create an opengl context and bind it to the window
				//m_platformSpecific.glc = glXCreateContext(m_platformSpecific.dpy, m_platformSpecific.vi, NULL, GL_TRUE); // GL_TRUE for rendering enabled.
				//glXMakeCurrent(m_platformSpecific.dpy, m_platformSpecific.win, m_platformSpecific.glc);

				// Enable OpenGL
					int glinitint = gl3wInit();
					ARK2D::getLog()->i(StringUtil::append("GL INIT: ", glinitint));
					if (glinitint == -1) {
	                	ErrorDialog::createAndShow("Failed to initialise OpenGL for an unknown reason. Please update your graphics drivers.");
	                	exit(0);
	                	return;// -1;
	        		}
	        		if (!gl3wIsSupported(3, 2)) { 
	               		//ErrorDialog::createAndShow("OpenGL 3.2 is not supported on your computer. Trying OpenGL 3.1...");
	        		} else if (!gl3wIsSupported(3, 1)) { 
	               		ErrorDialog::createAndShow("OpenGL 3.1 is not supported on your computer. Please update your graphics drivers.");
	                	exit(0); 
	                	return;// -1; 
	        		}
	        		ARK2D::getLog()->i(StringUtil::append("Using OpenGL ", (const char*) glGetString(GL_VERSION)));
	        		ARK2D::getLog()->i(StringUtil::append("Using GLSL ", (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION)));
	        		//printf("OpenGL %s, GLSL %s\n",  glGetString(GL_VERSION), glGetString());

				glEnable(GL_TEXTURE_2D);
				glViewport(0, 0, m_originalWidth, m_originalHeight);
				glClearColor(m_clearColor.getRed()/255.0f, m_clearColor.getGreen()/255.0f, m_clearColor.getBlue()/255.0f, m_clearColor.getAlpha()/255.0f);

				glClear( GL_COLOR_BUFFER_BIT );
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				#ifndef NO_FIXED_FUNCTION_PIPELINE
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				#endif
				ARK2D::getRenderer()->init();
				enable2D();

				// load default font.
				if (m_willLoadDefaultFont) { 
					ARK::Font::BMFont* fnt = ARK::Core::Resource::get("ark2d/fonts/default.fnt")->asFont()->asBMFont(); // BMFont("ark2d/fonts/default.fnt", "ark2d/fonts/default.png");
					fnt->scale(0.5f);
					m_graphics.m_DefaultFont = fnt;
					m_graphics.m_Font = fnt; 
				} else { 
					m_graphics.m_DefaultFont = NULL;
					m_graphics.m_Font = NULL;
				}


				// tssk. start game.
				//glEnable(GL_DEPTH_TEST);

				// Gamepads
				ARK2D::getLog()->i("Initialising Gamepads... ");
				m_platformSpecific.initGamepads();

				// Enable OpenAL
				ARK2D::getLog()->i("Initialising OpenAL... ");
				this->enableOpenAL();
				ARK2D::getLog()->i("done.");
				m_bRunning = true;

				ARK2D::getLog()->i("Initialising Log");
				ARK2D::s_log->init();

				ARK2D::getLog()->i("Initialising Localisations");
				initLocalisation();

				ARK2D::getLog()->i("Initialising ");
				ARK2D::getLog()->i(m_game.getTitle());
				ARK2D::getLog()->i("...");
				m_game.init(this);

				ARK2D::getLog()->i("Initialised ");
				ARK2D::getLog()->i(m_game.getTitle());
				ARK2D::getLog()->i("...");

				Image::showAnyGlErrorAndExit();

				SDL_Event event;
				// http://content.gpwiki.org/index.php/OpenGL:Tutorials:Setting_up_OpenGL_on_X11
				while (m_bRunning) {
					
					while(SDL_PollEvent(&event)) {
						if (event.type == SDL_QUIT) {
							m_bRunning = false;
						} else if (event.type == SDL_KEYDOWN) { 
							//cout << event.key.type << endl;
							//cout << "key pressed: " << event.key.keysym.sym << endl;
 
							std::map<int, int>::iterator it = m_platformSpecific.keycodemap_reverse->find(event.key.keysym.sym);
							if (it != m_platformSpecific.keycodemap_reverse->end()) {
								ARK2D::getInput()->pressKey(it->second);
							}
							
						} else if (event.type == SDL_KEYUP) { 
							std::map<int, int>::iterator it = m_platformSpecific.keycodemap_reverse->find(event.key.keysym.sym);
							if (it != m_platformSpecific.keycodemap_reverse->end()) {
								ARK2D::getInput()->releaseKey(it->second);
							}

						} else if (event.type == SDL_WINDOWEVENT) {
							switch(event.window.event)
							{ 
								case SDL_WINDOWEVENT_ENTER:
									cout << "window event enter " << endl;
									break;
								case SDL_WINDOWEVENT_LEAVE:
									cout << "window event leave " << endl;
									break;
								case SDL_WINDOWEVENT_CLOSE:
									m_bRunning = false;
									break;
								case SDL_WINDOWEVENT_RESIZED:
									setSize(event.window.data1, event.window.data2);
									break;
							}
						}
						else if (event.type == SDL_MOUSEMOTION) {
							float thisx = event.motion.x;
							float thisy = event.motion.y;
							
							thisx -= getTranslateX(); 
						    thisy -= getTranslateY();

						    thisx /= getScale();
						    thisy /= getScale();

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

							m_platformSpecific.m_globalMouseX = 0;//event.xmotion.x_root;
							m_platformSpecific.m_globalMouseY = 0; //event.xmotion.y_root;
						} 
						else if (event.type == SDL_MOUSEBUTTONDOWN)
						{
							if (event.button.button == SDL_BUTTON_LEFT) {
								ARK2D::getInput()->pressKey(Input::MOUSE_BUTTON_LEFT);
							} else if (event.button.button == SDL_BUTTON_MIDDLE) {
								ARK2D::getInput()->pressKey(Input::MOUSE_BUTTON_MIDDLE);
							} else if (event.button.button == SDL_BUTTON_RIGHT) {
								ARK2D::getInput()->pressKey(Input::MOUSE_BUTTON_RIGHT);
							}
						}
						else if (event.type == SDL_MOUSEBUTTONUP)
						{
							if (event.button.button == SDL_BUTTON_LEFT) {
								ARK2D::getInput()->releaseKey(Input::MOUSE_BUTTON_LEFT);
							} else if (event.button.button == SDL_BUTTON_MIDDLE) {
								ARK2D::getInput()->releaseKey(Input::MOUSE_BUTTON_MIDDLE);
							} else if (event.button.button == SDL_BUTTON_RIGHT) {
								ARK2D::getInput()->releaseKey(Input::MOUSE_BUTTON_RIGHT);
							}
						}
					}


					//ARK2D::getLog()->v("Tick");
					m_timer.tick();

					//ARK2D::getLog()->v("Update Gamepads"); 
					//processGamepadInput();

					//ARK2D::getLog()->v("Update Log");
					ARK2D::getLog()->update();

					m_platformSpecific.detectGamepads();

					//ARK2D::getLog()->v("Update Game");
					m_game.preUpdate(this, &m_timer);
					m_game.update(this, &m_timer);
					m_game.postUpdate(this, &m_timer);
					m_input.clearKeyPressedRecord();
					for (unsigned int i = 0; i < m_gamepads.size(); i++) {
						m_gamepads.at(i)->clearButtonPressedRecord();
					}
					Image::showAnyGlErrorAndExit();

					//ARK2D::getLog()->v("Render Reset");
					RendererStats::reset();
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

					//ARK2D::getLog()->v("Pre-render Game");
					m_game.preRender(this, &m_graphics);
					//ARK2D::getLog()->v("Render Game");
					m_game.render(this, &m_graphics);
					//ARK2D::getLog()->v("Render Log");
					ARK2D::getLog()->render(this, &m_graphics);
					//ARK2D::getLog()->v("Post-render Game"); 
					m_game.postRender(this, &m_graphics); 
					Image::showAnyGlErrorAndExit();

					SDL_GL_SwapWindow(m_platformSpecific.window);
					//glXSwapBuffers(m_platformSpecific.dpy, m_platformSpecific.win);
 
					//m_timer.limit(60);
				}

				m_platformSpecific.deinitOpenAL();

				// Destroy context
				SDL_GL_DeleteContext(m_platformSpecific.glContext);

				//Destroy window
				SDL_DestroyWindow( m_platformSpecific.window );

				//Quit SDL subsystems
				SDL_Quit();

				exit(0);
			}

			void GameContainer::close() const {
				//m_bRunning = false;
 				exit(0);

				
			}

			void GameContainerPlatform::setTitle(std::string title) {
				
			}

			void GameContainer::resizeWindowToFitViewport() {
				SDL_SetWindowSize(m_platformSpecific.window, m_width, m_height);
				SDL_SetWindowPosition(m_platformSpecific.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			
			}

			void GameContainer::setFullscreen(bool fullscreen) {
				if (!m_fullscreen && fullscreen) {
					ARK2D::getLog()->i("Setting fullscreen.");

					SDL_SetWindowFullscreen(m_platformSpecific.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				    m_fullscreen = true;

				}  
				else if (m_fullscreen && !fullscreen) {
					ARK2D::getLog()->i("Setting windowed.");

					SDL_SetWindowFullscreen(m_platformSpecific.window, 0);
					m_fullscreen = false;
				}
			}
		



			int GameContainer::getGlobalMouseX() const {
				return m_platformSpecific.m_globalMouseX;
			}
			int GameContainer::getGlobalMouseY() const {
				return m_platformSpecific.m_globalMouseY;
			}

			bool GameContainerPlatform::initOpenAL() { 
				bool alutInitialised = alutInit(0, NULL);
				if (!alutInitialised) {
					ALenum err = alGetError();
					ErrorDialog::createAndShow(StringUtil::append("Could not initialise OpenAL sound engine: ", Sound::getALErrorStringStatic(err))); 
					return false;
				}
				alGetError();
				
				return true; 
			} 
			bool GameContainerPlatform::deinitOpenAL() {
				alutExit();
				return true;
			}
 
			void GameContainer::setSize(int width, int height, bool docallback) {
				if (width == (signed int) m_width && height == (signed int) m_height) { return; }

				resizeBehaviour(width, height, docallback); 
				resizeWindowToFitViewport();

				//ARK2D::getLog()->w(StringUtil::appendf("tx: ", getTranslateX()));
				//ARK2D::getLog()->w(StringUtil::appendf("ty: ", getTranslateY()));
			}

			string ARK::Core::GameContainerPlatform::getResourcePath() const {
				return m_resourcePath;
			}

			void* ARK::Core::GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
				return new string("");
			}
		}
	}

#endif
