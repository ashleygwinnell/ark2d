
#include "../GameContainer.h"
#include "GameContainerLinux.h"



#if defined(ARK2D_UBUNTU_LINUX) && !defined(ARK2D_SDL2)

#include "../../Geometry/Shape.h"
#include "../../Geometry/Circle.h"
#include "../../Geometry/Line.h"
#include "../../Geometry/Rectangle.h"

	#include <dirent.h>
	#include <errno.h>
	#include <fcntl.h>
	#include <linux/input.h>
	
	#include <X11/Xatom.h>


	#include <X11/extensions/Xinerama.h>

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

			GLint GameContainerPlatform::att[11] = { 
				GLX_RGBA, 
				GLX_DEPTH_SIZE, 24, 
				GLX_DOUBLEBUFFER, 
				GLX_STENCIL_SIZE, 8,
				GLX_SAMPLE_BUFFERS, 1,
				GLX_SAMPLES, 4, 
				None
			};

			GameContainer::GameContainer(Game& game, int width, int height, int bpp, bool fullscreen):
				m_timer(),
				m_game(game),
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
				m_platformSpecific.m_iconPath = "";
				m_platformSpecific.m_screens = new vector<GameContainerPlatform_Screens>();
				m_platformSpecific.m_resourcePath = "data/";
				m_platformSpecific.m_globalMouseX = 0;
				m_platformSpecific.m_globalMouseY = 0;
				m_platformSpecific.m_initialisedGamepads = false;

				m_platformSpecific.keycodemap = new map<int, int>();
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_ESCAPE, 9));

				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_A, 38));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_S, 39));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_D, 40));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F, 41));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_G, 42));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_H, 43));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_J, 44));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_K, 45));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_L, 46));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_SEMICOLON, 47));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_APOSTROPHE, 48));
				
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_Z, 52));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_X, 53));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_C, 54)); 
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_V, 55));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_B, 56));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_N, 57));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_M, 58));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_COMMA, 59));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_PERIOD, 60));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_FORWARD_SLASH, 61));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_SPACE, 65));

				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_Q, 24));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_W, 25));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_E, 26));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_R, 27));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_T, 28));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_Y, 29));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_U, 30));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_I, 31));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_O, 32));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_P, 33));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_LEFT_SQUARE_BRACKET, 34));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_RIGHT_SQUARE_BRACKET, 35));

				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_1, 10));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_2, 11));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_3, 12));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_4, 13));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_5, 14));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_6, 15));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_7, 16));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_8, 17));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_9, 18));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_0, 19));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_HYPHEN, 20));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_EQUALS, 21));

				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_UP, 111));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_LEFT, 113));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_RIGHT, 114));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_DOWN, 116));

				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F1, 67));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F2, 68));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F3, 69));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F4, 70));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F5, 71));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F6, 72));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F7, 73));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F8, 74));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F9, 75));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F10, 76));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F11, 95));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_F12, 96));

				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_HOME, 110));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_PAGEUP, 112));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_END, 115));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_PAGEDOWN, 117));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_INSERT, 118));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_DELETE, 119));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_LCONTROL, 37));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_RCONTROL, 105));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_ALT, 64));
				//m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_RALT, 108));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_LWIN, 133));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_RWIN, 135));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_TAB, 23));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_CAPSLOCK, 66));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_LSHIFT, 50));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_RSHIFT, 62));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_BACKSPACE, 22));
				m_platformSpecific.keycodemap->insert(std::make_pair(Input::KEY_ENTER, 36));


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

				m_graphics.preinit();

			}

			void GameContainer::setIcon(const string& icon) {
				m_platformSpecific.m_iconPath = icon;
			}

			void GameContainer::setCursorVisible(bool b) {
 
			}

			void GameContainerPlatform::initGamepads() {
				if (!m_initialisedGamepads) {
					pthread_mutexattr_t recursiveLock;
					pthread_mutexattr_init(&recursiveLock);
					pthread_mutexattr_settype(&recursiveLock, PTHREAD_MUTEX_RECURSIVE);
					pthread_mutex_init(&devicesMutex, &recursiveLock);
					pthread_mutex_init(&eventQueueMutex, &recursiveLock);
					m_initialisedGamepads = true;
					detectGamepads();
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
				
				if (!m_initialisedGamepads) {
					return;
				}
				
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
						unsigned int gamepadIndex2;
						
						numDevices--;
						for (gamepadIndex2 = gamepadIndex; gamepadIndex2 < numDevices; gamepadIndex2++) {
							Gamepad* pad2 = container->m_gamepads[gamepadIndex2 + 1];
							container->m_gamepads[gamepadIndex2] = pad2;
						}
						gamepadIndex--;
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

			void GameContainer::start() {

				// Seed the random
				MathUtil::seedRandom();

				// NULL means the current computer.
				m_platformSpecific.dpy = XOpenDisplay(NULL);
				if (m_platformSpecific.dpy == NULL) {
					printf("\n\tcannot connect to X server\n\n");
					exit(0);
				}



				// Root window is "desktop background" window.
				m_platformSpecific.root = DefaultRootWindow(m_platformSpecific.dpy);

				// window on screen placement.
				int initOffsetX = 0;
				int initOffsetY = 0;
				Screen* defaultScreen = XDefaultScreenOfDisplay(m_platformSpecific.dpy);
				int displayWidth = WidthOfScreen(defaultScreen);
				int displayHeight = HeightOfScreen(defaultScreen);
				
				// extra bits for dual-screen setups
				// http://stackoverflow.com/questions/836086/programmatically-determining-individual-screen-widths-heights-in-linux-w-xinera
				int xineramaBaseEventCode;
 				int xineramaBaseErrorCode;
				bool hasXinerama = XineramaQueryExtension(m_platformSpecific.dpy, &xineramaBaseEventCode, &xineramaBaseErrorCode) && XineramaIsActive(m_platformSpecific.dpy);
				bool xineramaSuccess = false;
				if (hasXinerama) {
					int heads = 0;
					XineramaScreenInfo* p = XineramaQueryScreens(m_platformSpecific.dpy, &heads);
					if (heads > 0) { 
						for (int x = 0; x < heads; ++x) {
							cout << "Head " << x+1 << " of " << heads << ": " << p[x].width << "x" << p[x].height << " at " << p[x].x_org << "," << p[x].y_org << endl;
							
							GameContainerPlatform_Screens thisScreen;
							thisScreen.id = x;
							thisScreen.x = p[x].x_org;
							thisScreen.y = p[x].y_org;
							thisScreen.width = p[x].width;
							thisScreen.height = p[x].height;
							m_platformSpecific.m_screens->push_back(thisScreen);
						}
						xineramaSuccess = true;

						initOffsetX = p[0].x_org;
						initOffsetY = p[0].y_org;
						displayWidth = p[0].width;
						displayHeight = p[0].height;
					} else {
						ARK2D::getLog()->e("XineramaQueryScreens says there aren't any...");
					}
					XFree(p);
				} 

				int initialWindowX = initOffsetX + (displayWidth / 2) - (m_originalWidth / 2);
				int initialWindowY = initOffsetY + (displayHeight / 2) - (m_originalHeight / 2);	
				m_screenWidth = displayWidth;
				m_screenHeight = displayHeight;
				ARK2D::getLog()->i(StringUtil::append("Screen Width: ", displayWidth));
				ARK2D::getLog()->i(StringUtil::append("Screen Height: ", displayHeight));
				ARK2D::getLog()->i(StringUtil::append("Window X: ", initialWindowX));
				ARK2D::getLog()->i(StringUtil::append("Window Y: ", initialWindowY)); 


				

				// Pick a display mode: this can fail, so type glxinfo into the command line for detail.
				m_platformSpecific.vi = glXChooseVisual(m_platformSpecific.dpy, 0, m_platformSpecific.att);
				if (m_platformSpecific.vi == NULL) {
					ErrorDialog::createAndShow("no appropriate visual found (glXChooseVisual failed)");
					exit(0);
				} else {
					ARK2D::getLog()->i(StringUtil::append("visual selected: ", m_platformSpecific.vi->visualid));
				} /* %p creates hexadecimal output like in glxinfo */

				// make a colourmap for the window.
				m_platformSpecific.cmap = XCreateColormap(m_platformSpecific.dpy, m_platformSpecific.root, m_platformSpecific.vi->visual, AllocNone);

				// something else
				m_platformSpecific.swa.colormap = m_platformSpecific.cmap;
				m_platformSpecific.swa.event_mask = ExposureMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | FocusChangeMask; // respond to these events...
				m_platformSpecific.swa.override_redirect = True;

				// create the window
				m_platformSpecific.win = XCreateWindow( 
					m_platformSpecific.dpy,
					m_platformSpecific.root,
					initialWindowX, initialWindowY, // initial x,y pos 
					m_originalWidth, m_originalHeight, 0, // w, h, border...
					m_platformSpecific.vi->depth, InputOutput,
					m_platformSpecific.vi->visual, CWColormap | CWEventMask, &m_platformSpecific.swa
				);
				//m_platformSpecific.win.override_redirect = False;
				

				// set not resizable?
				//XSizeHints* szhints = new XSizeHints();
				//szhints->flags = PMinSize | PMaxSize;
				//szhints->min_width = m_originalWidth;
				//szhints->max_width = m_originalWidth;
				//szhints->min_height = m_originalHeight;
				//szhints->max_height = m_originalHeight;
				//XSetWMNormalHints(m_platformSpecific.dpy, m_platformSpecific.win, szhints);

				

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

						unsigned int pitch = png->getWidth() * 4;
						unsigned int sourcesize = 2 + (png->getWidth() * png->getHeight());
						unsigned char* sourcedata = (unsigned char*) png->getImageData();
						long* icondata = (long*) malloc(sizeof(long) * sourcesize * 4);
						icondata[0] = png->getWidth();
						icondata[1] = png->getHeight();

						long* dest = (long*) &icondata[2];
						unsigned int* source; 
						for (int y = 0; y < png->getHeight(); ++y) 
						{
							//source = (Uint32*)((Uint8*) icon->pixels + y * icon->pitch);
							source = (unsigned int*) (sourcedata + (y * pitch));
							for (int x = 0; x < png->getWidth(); ++x) 
							{
								//*dest++ = *source++;
								//memcpy(dest, source, sizeof(int));

								unsigned char* thisdest = (unsigned char*) dest;
								unsigned char* thissource = (unsigned char*) source;
								///char dummy = 0;
								//memcpy(thisdest+0, &dummy, sizeof(unsigned char));
								//memcpy(thisdest+2, &dummy, sizeof(unsigned char));
								//memcpy(thisdest+4, &dummy, sizeof(unsigned char));
								//memcpy(thisdest+6, &dummy, sizeof(unsigned char));

								memcpy(thisdest+1, thissource+3, sizeof(unsigned char));
								memcpy(thisdest+3, thissource+0, sizeof(unsigned char));
								memcpy(thisdest+5, thissource+1, sizeof(unsigned char));
								memcpy(thisdest+7, thissource+2, sizeof(unsigned char));

								dest++; 
								source++;
							}
						}

						for(unsigned int k = 2; k < sourcesize; k++) {
							//long* r = 
							k++;
						}


						Atom net_wm_icon = XInternAtom(m_platformSpecific.dpy, "_NET_WM_ICON", False);
						//Atom cardinal = XInternAtom(m_platformSpecific.dpy, "CARDINAL", False); //

						XChangeProperty(
							m_platformSpecific.dpy, 
							m_platformSpecific.win, 
							net_wm_icon, 
							XA_CARDINAL, 
							32, 
							PropModeReplace, 
							(const unsigned char*) icondata, 
							sourcesize
						);
					}
					
				}

				// make the window appear... 
				XMapWindow(m_platformSpecific.dpy, m_platformSpecific.win);
 
				// title bar name  
				XStoreName(m_platformSpecific.dpy, m_platformSpecific.win, m_strTitle.c_str());

				resizeWindowToFitViewport();

				// create an opengl context and bind it to the window
				m_platformSpecific.glc = glXCreateContext(m_platformSpecific.dpy, m_platformSpecific.vi, NULL, GL_TRUE); // GL_TRUE for rendering enabled.
				glXMakeCurrent(m_platformSpecific.dpy, m_platformSpecific.win, m_platformSpecific.glc);

				// Enable OpenGL
					int glinitint = gl3wInit();
					ARK2D::getLog()->i(StringUtil::append("GL INIT: ", glinitint));
					if (glinitint == -1) {
	                	ErrorDialog::createAndShow("Failed to initialise OpenGL for an unknown reason. Please update your graphics drivers.");
	                	exit(0);
	                	return;// -1;
	        		}
	        		if (!gl3wIsSupported(3, 2)) { 
	               		ErrorDialog::createAndShow("OpenGL 3.2 is not supported on your computer. Trying OpenGL 3.1...");
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

				ARK2D::getLog()->i("Initialising ");
				ARK2D::getLog()->i(m_game.getTitle());
				ARK2D::getLog()->i("...");
				m_game.init(this);

				ARK2D::getLog()->i("Initialised ");
				ARK2D::getLog()->i(m_game.getTitle());
				ARK2D::getLog()->i("...");

				Image::showAnyGlErrorAndExit();

				// http://content.gpwiki.org/index.php/OpenGL:Tutorials:Setting_up_OpenGL_on_X11
				while (m_bRunning) {
					while (XPending(m_platformSpecific.dpy) > 0) {
						XNextEvent(m_platformSpecific.dpy, &m_platformSpecific.xev);

						XEvent event = m_platformSpecific.xev; 
						if (event.type == Expose) {
							XGetWindowAttributes(m_platformSpecific.dpy, m_platformSpecific.win, &m_platformSpecific.gwa);
						} else if (event.type == ConfigureNotify) {
							//if ((event.xconfigure.width != m_width) || (event.xconfigure.height != m_height)) {
							//	resizeBehaviour((int) event.xconfigure.width, (int) event.xconfigure.height);
								//resizeWindowToFitViewport();
								/*if (m_resizeBehaviour == RESIZE_BEHAVIOUR_SCALE) {
									glViewport(0, 0, m_width, m_height);
								} else {
									disable2D(); 
									enable2D();
									glViewport(0, 0, m_width, m_height);
								}
								m_game.resize(this, m_width, m_height);*/
                        	//}
						} else if (event.type == FocusIn) {
							
							ARK2D::getLog()->v("focus in");
							if (m_fullscreen) {
								m_platformSpecific.setAlwaysOnTop(true);
								XRaiseWindow(m_platformSpecific.dpy, m_platformSpecific.win);
							}
								
						} else if (event.type == FocusOut) {
							ARK2D::getLog()->v("focus out");
							if (m_fullscreen) {
								m_platformSpecific.setAlwaysOnTop(false);
								XLowerWindow(m_platformSpecific.dpy, m_platformSpecific.win);
							}


						} else if (event.type == MotionNotify) {

							float thisx = event.xmotion.x;
							float thisy = event.xmotion.y;

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

							m_platformSpecific.m_globalMouseX = event.xmotion.x_root;
							m_platformSpecific.m_globalMouseY = event.xmotion.y_root;

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
							std::map<int, int>::iterator it = m_platformSpecific.keycodemap_reverse->find(event.xkey.keycode);
							if (it != m_platformSpecific.keycodemap_reverse->end()) {
								ARK2D::getInput()->pressKey(it->second);
							}
						} else if (event.type == KeyRelease) {
							std::map<int, int>::iterator it = m_platformSpecific.keycodemap_reverse->find(event.xkey.keycode);
							if (it != m_platformSpecific.keycodemap_reverse->end()) {
								ARK2D::getInput()->releaseKey(it->second);
							}
						} 
					}

					ARK2D::getLog()->v("Tick");
					m_timer.tick();

					//ARK2D::getLog()->v("Update Gamepads"); 
					//processGamepadInput();

					ARK2D::getLog()->v("Update Log");
					ARK2D::getLog()->update();

					ARK2D::getLog()->v("Update Game");
					m_game.preUpdate(this, &m_timer);
					m_game.update(this, &m_timer);
					m_game.postUpdate(this, &m_timer);
					m_input.clearKeyPressedRecord();
					for (unsigned int i = 0; i < m_gamepads.size(); i++) {
						m_gamepads.at(i)->clearButtonPressedRecord();
					}
					Image::showAnyGlErrorAndExit();

					ARK2D::getLog()->v("Render Reset");
					RendererStats::reset();
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

					ARK2D::getLog()->v("Pre-render Game");
					m_game.preRender(this, &m_graphics);
					ARK2D::getLog()->v("Render Game");
					m_game.render(this, &m_graphics);
					ARK2D::getLog()->v("Render Log");
					ARK2D::getLog()->render(this, &m_graphics);
					ARK2D::getLog()->v("Post-render Game"); 
					m_game.postRender(this, &m_graphics); 
					Image::showAnyGlErrorAndExit();


					glXSwapBuffers(m_platformSpecific.dpy, m_platformSpecific.win);
 
					//m_timer.limit(60);
				}

				m_platformSpecific.deinitOpenAL();

				glXMakeCurrent(m_platformSpecific.dpy, None, NULL);
				glXDestroyContext(m_platformSpecific.dpy, m_platformSpecific.glc);
				XDestroyWindow(m_platformSpecific.dpy, m_platformSpecific.win);
				XCloseDisplay(m_platformSpecific.dpy);

				exit(0);
			}

			void GameContainer::close() const {
				//m_bRunning = false;
 				exit(0);

				
			}

			void GameContainerPlatform::setTitle(std::string title) {
				
			}

			void GameContainer::resizeWindowToFitViewport() {
				XSizeHints* szhints = XAllocSizeHints();
				szhints->flags = PMinSize | PMaxSize;
				szhints->min_width = m_width;
				szhints->max_width = m_width;
				szhints->min_height = m_height;
				szhints->max_height = m_height;
				XSetWMNormalHints(m_platformSpecific.dpy, m_platformSpecific.win, szhints);
				XFree(szhints);

				// window on screen placement.
				//Screen* defaultScreen = DefaultScreenOfDisplay(m_platformSpecific.dpy);
				int displayWidth = m_screenWidth;// WidthOfScreen(defaultScreen);
				int displayHeight = m_screenHeight; //HeightOfScreen(defaultScreen);
				int initialWindowX = (displayWidth / 2) - (m_width / 2);
				int initialWindowY = (displayHeight / 2) - (m_height / 2);	

				int curscr = m_platformSpecific.getScreenForWindow();
				float xoffset = m_platformSpecific.m_screens->at(curscr).x;
				float yoffset = m_platformSpecific.m_screens->at(curscr).y;
				ARK2D::getLog()->e(StringUtil::append("screen internal: ", curscr));

				XResizeWindow(m_platformSpecific.dpy, m_platformSpecific.win, m_width, m_height);
				XMoveWindow(m_platformSpecific.dpy, m_platformSpecific.win, xoffset + initialWindowX, yoffset + initialWindowY);
   				XRaiseWindow(m_platformSpecific.dpy, m_platformSpecific.win);
   				XFlush(m_platformSpecific.dpy);
			}

			#define _NET_WM_STATE_REMOVE        0    /* remove/unset property */
			#define _NET_WM_STATE_ADD           1    /* add/set property */
			#define _NET_WM_STATE_TOGGLE        2    /* toggle property  */
			void GameContainer::setFullscreen(bool fullscreen) {
				if (!m_fullscreen && fullscreen) {
					ARK2D::getLog()->i("Setting fullscreen.");

					
					/*XSizeHints* szhints = XAllocSizeHints();
					szhints->flags = PMinSize | PMaxSize | PWinGravity;
					szhints->min_width = m_width;
					szhints->max_width = m_width;
					szhints->min_height = m_height;
					szhints->max_height = m_height;
					szhints->win_gravity = NorthWestGravity;
					XSetWMNormalHints(m_platformSpecific.dpy, m_platformSpecific.win, szhints);
					XFree(szhints);
					*/
				  

   					


					//XSetInputFocus(m_platformSpecific.dpy, m_platformSpecific.win, RevertToNone, CurrentTime);

				  	/*Atom wm_state = XInternAtom(m_platformSpecific.dpy, "_NET_WM_STATE", False);
					Atom fullscreen2 = XInternAtom(m_platformSpecific.dpy, "_NET_WM_STATE_FULLSCREEN", False);

					XEvent xev;
					memset(&xev, 0, sizeof(xev));
					xev.xclient.type = ClientMessage;
					xev.xclient.serial = 0;
					xev.xclient.send_event = True;
					xev.xclient.window = m_platformSpecific.win;
					xev.xclient.message_type = wm_state;
				    xev.xclient.format = 32;
				    xev.xclient.data.l[0] = 1;
				    xev.xclient.data.l[1] = fullscreen2;
				    xev.xclient.data.l[2] = 0;

				    XMapWindow(m_platformSpecific.dpy, m_platformSpecific.win);
					int ret = XSendEvent (m_platformSpecific.dpy, m_platformSpecific.root, False, SubstructureRedirectMask | SubstructureNotifyMask, &xev); // 0=SubstructureRedirectMask | SubstructureNotifyMask
					if (!ret) {
						ARK2D::getLog()->e("...nope coult not fullscreen");
					}*/
					int curscr = m_platformSpecific.getScreenForWindow();
					float xoffset = m_platformSpecific.m_screens->at(curscr).x;
					float yoffset = m_platformSpecific.m_screens->at(curscr).y;
					ARK2D::getLog()->e(StringUtil::append("screen: ", curscr));

					int displayWidth = m_screenWidth; //WidthOfScreen(defaultScreen);
					int displayHeight = m_screenHeight; //HeightOfScreen(defaultScreen);
				    setSize(displayWidth, displayHeight);

				    m_platformSpecific.setWindowBordered(false);
				    m_platformSpecific.setAlwaysOnTop(true);
				    m_platformSpecific.setFocus(true);
				    
					XMoveWindow(m_platformSpecific.dpy, m_platformSpecific.win, xoffset, yoffset);
   					XRaiseWindow(m_platformSpecific.dpy, m_platformSpecific.win);
					XFlush(m_platformSpecific.dpy);
				    
 
				    m_fullscreen = true;

				}  
				else if (m_fullscreen && !fullscreen) {
					ARK2D::getLog()->i("Setting windowed.");

					int curscr = m_platformSpecific.getScreenForWindow();
					GameContainerPlatform_Screens* s = &m_platformSpecific.m_screens->at(curscr);
					ARK2D::getLog()->e(StringUtil::append("screen: ", curscr));

					setSize(m_originalWidth, m_originalHeight);
					m_platformSpecific.setWindowBordered(true);
					m_platformSpecific.setAlwaysOnTop(false);
					m_platformSpecific.setFocus(true);

					XMoveWindow(m_platformSpecific.dpy, m_platformSpecific.win, s->x + (s->width/2) - (m_originalWidth/2), s->y + (s->height/2) - (m_originalHeight/2));
   					XRaiseWindow(m_platformSpecific.dpy, m_platformSpecific.win);
					XFlush(m_platformSpecific.dpy);

					m_fullscreen = false;
				}
			}
			void GameContainerPlatform::setFocus(bool b) {
				if (b) { 
					XRaiseWindow(dpy, win);
					XSetInputFocus(dpy, win, RevertToNone, CurrentTime);
				} else {
					//XLowerWindow(dpy, win);
				}
			}
			int GameContainerPlatform::getScreenForWindow() {
				int x = 0;
				int y = 0;
				Window child;
				XWindowAttributes xwa;
				XTranslateCoordinates( dpy, win, DefaultRootWindow(dpy), 0, 0, &x, &y, &child );
				//XTranslateCoordinates( dpy, win, root, m_screens->at(0).x, m_screens->at(0).y, &x, &y, &child );
				XGetWindowAttributes(dpy, win, &xwa);
				
				//int cx = (x - xwa.x) + (m_container->getDynamicWidth()/2);
				//int cy = (y - xwa.y) + (m_container->getDynamicHeight()/2); 
				//int cx = xwa.x + (m_container->getDynamicWidth()/2);
				//int cy = xwa.y + (m_container->getDynamicHeight()/2); 
				int cx = x + (m_container->getDynamicWidth()/2);
				int cy = y + (m_container->getDynamicHeight()/2); 
				ARK2D::getLog()->e(StringUtil::append("x: ", x));
				ARK2D::getLog()->e(StringUtil::append("y: ", y));
				ARK2D::getLog()->e(StringUtil::append("cx: ", cx));
				ARK2D::getLog()->e(StringUtil::append("cy: ", cy));
			
				for(unsigned int i = 0; i < m_screens->size(); ++i) {
					GameContainerPlatform_Screens* scr = &m_screens->at(i);
					if (Shape<int>::collision_rectangleRectangle(scr->x, scr->y, scr->width, scr->height, cx, cy, 1, 1)) {
						return scr->id;
					}
				}
				ARK2D::getLog()->e("Could not getScreenForWindow...");
				return 0;
			}

			void GameContainerPlatform::setWindowBordered(bool border) {
				Atom WM_HINTS = XInternAtom(dpy, "_MOTIF_WM_HINTS", True);
			    if (WM_HINTS != None) {

			    	static const unsigned long MWM_HINTS_FUNCTIONS   = 1 << 0;
		            static const unsigned long MWM_HINTS_DECORATIONS = 1 << 1;
		    
		            //static const unsigned long MWM_DECOR_ALL         = 1 << 0;
		            static const unsigned long MWM_DECOR_BORDER      = 1 << 1;
		            static const unsigned long MWM_DECOR_RESIZEH     = 1 << 2;
		            static const unsigned long MWM_DECOR_TITLE       = 1 << 3;
		            static const unsigned long MWM_DECOR_MENU        = 1 << 4;
		            static const unsigned long MWM_DECOR_MINIMIZE    = 1 << 5;
		            static const unsigned long MWM_DECOR_MAXIMIZE    = 1 << 6;

		            //static const unsigned long MWM_FUNC_ALL          = 1 << 0;
		            static const unsigned long MWM_FUNC_RESIZE       = 1 << 1;
		            static const unsigned long MWM_FUNC_MOVE         = 1 << 2;
		            static const unsigned long MWM_FUNC_MINIMIZE     = 1 << 3;
		            static const unsigned long MWM_FUNC_MAXIMIZE     = 1 << 4;
		            static const unsigned long MWM_FUNC_CLOSE        = 1 << 5;

			        /* Hints used by Motif compliant window managers */
			        struct WMHints {
			            unsigned long flags;
			            unsigned long functions;
			            unsigned long decorations;
			            long input_mode;
			            unsigned long status;
			        }; 

			        WMHints hints; 
			       	//hints.flags = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS; //(1L << 1);
			       	hints.flags = MWM_HINTS_DECORATIONS; //(1L << 1);
			       	hints.functions = 0;
			       	hints.decorations = 0; 
			       	hints.input_mode = 0;
			       	hints.status = 0;

			       	if (border) {
			       		hints.decorations |= MWM_DECOR_BORDER | MWM_DECOR_TITLE | MWM_DECOR_MINIMIZE | MWM_DECOR_MENU;
			       	} 
			        
			        const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&hints);
			        XChangeProperty(dpy, win, WM_HINTS, WM_HINTS, 32, PropModeReplace, ptr, 5);

			    } else {  /* set the transient hints instead, if necessary */
			        //XSetTransientForHint(dpy, win, RootWindow(dpy, DefaultScreenOfDisplay(dpy)));
			        XSetTransientForHint(dpy, win, win);
			    }
			}

			// http://stackoverflow.com/questions/20733215/how-to-make-a-window-always-on-top
			void GameContainerPlatform::setAlwaysOnTop(bool b) {

				int state = (b)?_NET_WM_STATE_ADD:_NET_WM_STATE_REMOVE;
				XEvent event;
			    event.xclient.type = ClientMessage;
			    event.xclient.serial = 0;
			    event.xclient.send_event = True;
			    event.xclient.display = dpy;
			    event.xclient.window  = win;
			    event.xclient.message_type = XInternAtom (dpy, "_NET_WM_STATE", False);
			    event.xclient.format = 32;

			    event.xclient.data.l[0] = state;
			    event.xclient.data.l[1] = XInternAtom (dpy, "_NET_WM_STATE_ABOVE", False);
			    event.xclient.data.l[2] = 0; //unused.
			    event.xclient.data.l[3] = 0;
			    event.xclient.data.l[4] = 0;

			    Status s = XSendEvent (dpy, DefaultRootWindow(dpy), False, SubstructureRedirectMask|SubstructureNotifyMask, &event);
			    // ...
			}


			int GameContainer::getGlobalMouseX() const {
				return m_platformSpecific.m_globalMouseX;
			}
			int GameContainer::getGlobalMouseY() const {
				return m_platformSpecific.m_globalMouseY;
			}

			bool GameContainerPlatform::initOpenAL() { 
				alutInit(0, NULL);
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
