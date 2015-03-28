 /*
 * Gamepad.cpp
 *
 *  Created on: 8 Nov 2010
 *      Author: Ashley
 */


#include "../Includes.h"

#include "../ARK2D.h"
#include "Gamepad.h" 
#include "../Util/Log.h" 
#include "../Core/GameContainer.h" 
 
#ifdef ARK2D_ANDROID
	#include "../Core/Platform/GameContainerAndroid.h" 
#endif 
   
namespace ARK { 
	namespace Controls {  

		Gamepad::Gamepad():   
			m_triggersSendBumperEvents(true),
			m_previousLTriggerValue(0.0f),
			m_previousRTriggerValue(0.0f), 

			pressedEvents(),
			releasedEvents(),
 
			initted(false),
			id(0),
			productId(0), 
			vendorId(0),
			name(""), 

			#if defined(ARK2D_WINDOWS)

				povXAxisIndex(0),
				povYAxisIndex(0),

			#endif

			dpadPosition(DPAD_NONE),

			numButtons(0),
			buttons(),

			numAxes(0),
			axes()
		{

		}
		int Gamepad::getDPadPosition() {
			return dpadPosition;
		}

		void Gamepad::update() {
 
			float currentLBumperValue = getAxisValue(Gamepad::TRIGGER_1);
			if (currentLBumperValue > 0.5f && m_previousLTriggerValue < 0.5f) {
				pressButton(Gamepad::BUTTON_LBUMPER);
			} else if (currentLBumperValue < 0.5f && m_previousLTriggerValue > 0.5f) {
				releaseButton(Gamepad::BUTTON_LBUMPER);	
			}

			float currentRBumperValue = getAxisValue(Gamepad::TRIGGER_2);
			if (currentRBumperValue > 0.5f && m_previousRTriggerValue < 0.5f) {
				pressButton(Gamepad::BUTTON_RBUMPER);
			} else if (currentRBumperValue < 0.5f && m_previousRTriggerValue > 0.5f) {
				releaseButton(Gamepad::BUTTON_RBUMPER);	
			} 
			m_previousLTriggerValue = currentLBumperValue;
			m_previousRTriggerValue = currentRBumperValue;
			
		}

		void Gamepad::clearButtonPressedRecord() {
			pressedEvents.clear();
			releasedEvents.clear();
		}
		bool Gamepad::hasButton(unsigned int b) {
			#if defined(ARK2D_ANDROID)
				bool found = false;
				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == b) {
						buttons.at(i)->down = true;
						found = true;
						break;
					}
				} 
				return found;
			#else 
				bool found = false;
				unsigned int nb = convertButtonToId(this, b);
				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == nb) {
						buttons.at(i)->down = true;
						found = true;
						break;
					}
				} 
				return found;
			#endif
		}
		void Gamepad::pressButton(unsigned int b) {

			#if defined(ARK2D_ANDROID)
				bool found = false;
				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == b) {
						buttons.at(i)->down = true;
						found = true;
						break;
					}
				} 

				if (!found && b != BUTTON_NULL) {
					ARK2D::getLog()->e(StringUtil::append("Could not find Gamepad button (on press) with id: ", b));
					return;
				}
				pressedEvents.insert(b);

			#else

				//ARK2D::getLog()->v(StringUtil::append("buttons size: ", buttons.size()));
				bool found = false;
				unsigned int nb = convertButtonToId(this, b);
				
				#if defined(ARK2D_WINDOWS)
					ARK2D::getLog()->i(StringUtil::append("button press initial: ", b));
					ARK2D::getLog()->i(StringUtil::append("button press converted: ", nb));
				#endif

				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == nb) {
						buttons.at(i)->down = true;
						found = true;
						break;
					}
				} 

				if (!found && b != BUTTON_NULL) {
					ARK2D::getLog()->e(StringUtil::append("Could not find Gamepad button (on press) with id: ", b));
					return;
				}

				//buttons[b]->down = true;
				pressedEvents.insert(b);
			#endif
		} 

		void Gamepad::releaseButton(unsigned int b) {
			#if defined(ARK2D_ANDROID)
				bool found = false;
				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == b) {
						buttons.at(i)->down = false;
						found = true;
						break;
					}
				} 

				if (!found && b != BUTTON_NULL) {
					ARK2D::getLog()->e(StringUtil::append("Could not find Gamepad button (on release) with id: ", b));
					return;
				}
				releasedEvents.insert(b);

			#else

				bool found = false;
				unsigned int nb = convertButtonToId(this, b, false);
				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == nb) {
						buttons.at(i)->down = false;
						found = true; 
						break;
					}
				}

				if (!found && b != BUTTON_NULL) {
					ARK2D::getLog()->e(StringUtil::append("Could not find Gamepad button (on release) with id: ", b));
					return;
				}

				//buttons[b]->down = false;
				releasedEvents.insert(b);
			#endif
		}

		bool Gamepad::isButtonPressed(unsigned int b) {
			std::set<int>::iterator it = pressedEvents.find(b);
			if (it != pressedEvents.end()) {
				return true;
			} else {
				return false;
			}
		}
		bool Gamepad::isAnyButtonPressed() {
			return (pressedEvents.size() > 0);
		}

		bool Gamepad::isButtonReleased(unsigned int b) {
			std::set<int>::iterator it = releasedEvents.find(b);
			if (it != releasedEvents.end()) {
				return true;
			} else {
				return false;
			}
		}

		bool Gamepad::isButtonDown(unsigned int b) {
			#if defined(ARK2D_ANDROID)
				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == b) {
						return buttons.at(i)->down;
					}
				}
				return false; 
			#else 
				//return buttons[b]->down;
				unsigned int nb = convertButtonToId(this, b, false);
				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == nb) {
						return buttons.at(i)->down;
					}
				}
				return false; 
				//return buttonStates.at(b);
				//return buttonDownBuffer[b];
			#endif
		}

		bool Gamepad::isAxis(unsigned int axisId) {
			unsigned int newAxisId = convertAxisToId(this, axisId);
			for(unsigned int i = 0; i < axes.size(); ++i) {
				if (axes.at(i)->id == newAxisId) {
					return true;
				}
			}
			return false;
		}
		float Gamepad::getAxisValue(unsigned int axisId) {

			// Windows multimedia api maps xbox 360 triggers on to A SINGLE AXIS LIKE A FUCKING IDIOT.
			// It's fixed in Xinput apis but that rquires a Visual Studio compiler set up. :( 
			#if defined(ARK2D_WINDOWS)  
				float trigger1RawValue = 0.0f; 
				if (axisId == TRIGGER_2) {
					unsigned int newAxisId = convertAxisToId(this, TRIGGER_1);
					for(unsigned int i = 0; i < axes.size(); ++i) {
						if (axes.at(i)->id == newAxisId) { 
							trigger1RawValue = axes.at(i)->value;
						}
					}
				}
			#endif

			unsigned int newAxisId = convertAxisToId(this, axisId);
			for(unsigned int i = 0; i < axes.size(); ++i) {
				if (axes.at(i)->id == newAxisId) { 
 
					#if defined(ARK2D_WINDOWS)  
						if (axisId == TRIGGER_1) {
							float val = axes.at(i)->value;
							if (val < 0.0f) { val = 0.0f; }
							return val; 
						}  else if (axisId == TRIGGER_2) {
							float val = trigger1RawValue;
							if (val > 0.0f) { val = 0.0f; }
							return val * -1.0f;
						}
					#endif

					if (axisId == TRIGGER_1 || axisId == TRIGGER_2) {
						return (axes.at(i)->value + 1.0f)/2.0f;
					}

					return axes.at(i)->value; 
				}
			}
			return 0.0f;
		}

		// Fire TV
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 0 to gamepad id: 14 at index: 0 (X1)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 1 to gamepad id: 14 at index: 1 (Y1)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 11 to gamepad id: 14 at index: 2 (X2)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 14 to gamepad id: 14 at index: 3 (Y2)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 22 to gamepad id: 14 at index: 4 (RT)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 23 to gamepad id: 14 at index: 5 (LT)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 15 to gamepad id: 14 at index: 6 (DPAD X)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 16 to gamepad id: 14 at index: 7 (DPAD Y)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 32 to gamepad id: 14 at index: 8 (no idea...)

		unsigned int Gamepad::convertAxisToId(Gamepad* gamepad, unsigned int axis) {
			#if defined(ARK2D_MACINTOSH)
				bool isPS3 = StringUtil::str_contains(gamepad->name, "PLAYSTATION(R)3");
				if (isPS3) {
					if (axis == TRIGGER_1) { 
						return 14;  
					} else if (axis == TRIGGER_2) {
						return 15;
					} 
				}
				return axis;
			#elif defined(ARK2D_WINDOWS) 
				switch(axis) {
					case ANALOG_STICK_2_X: 
						return TRIGGER_1;
					case TRIGGER_1:
						return ANALOG_STICK_2_X;
					
				} 
			#elif defined(ARK2D_ANDROID)
				
				if (ARK2D::getContainer()->getPlatformSpecific()->getPluggable()->firetv_isAmazonFireTV()) {
					if (axis == TRIGGER_1) {
						return 5;
					} else if (axis == TRIGGER_2) {
						return 4;
					} 
				} 

				/*if (axis == ANALOG_STICK_1_X) {
					return axis;
				} else if (axis == ANALOG_STICK_1_Y) {
					return axis;
				} else if (axis == ANALOG_STICK_2_X) {
					return TRIGGER_1;
				} else if (axis == ANALOG_STICK_2_Y) {
					return ANALOG_STICK_2_X;
				} else if (axis == TRIGGER_1) {
					return ANALOG_STICK_2_Y;
				} else if (axis == TRIGGER_2) {
					return axis;
				}*/

				if (axis == ANALOG_STICK_1_X) {
					return axis;
				} else if (axis == ANALOG_STICK_1_Y) {
					return axis;
				} else if (axis == ANALOG_STICK_2_X) {
					return ANALOG_STICK_2_Y;
				} else if (axis == ANALOG_STICK_2_Y) {
					return TRIGGER_1;
				} else if (axis == TRIGGER_1) {
					return ANALOG_STICK_2_X;
				} else if (axis == TRIGGER_2) {
					return axis;
				}


				/*static const unsigned int ANALOG_STICK_1_X = 0;
				static const unsigned int ANALOG_STICK_1_Y = 1;
				static const unsigned int ANALOG_STICK_2_X = 2;
				static const unsigned int ANALOG_STICK_2_Y = 3;
				static const unsigned int TRIGGER_1 = 4;
				static const unsigned int TRIGGER_2 = 5;

				10-20 20:30:55.583: I/ARK2D(24226): Index: 0. Axis Id: 0. Value: 0.027451.		// 0, LS_X
				10-20 20:30:55.583: I/ARK2D(24226): Index: 1. Axis Id: 1. Value: 0.00392163.	// 1, LS_Y
				10-20 20:30:55.583: I/ARK2D(24226): Index: 2. Axis Id: 11. Value: -1.			// 2, LT
				10-20 20:30:55.583: I/ARK2D(24226): Index: 3. Axis Id: 12. Value: 0.00392163.	// 3, RS_X
				10-20 20:30:55.583: I/ARK2D(24226): Index: 4. Axis Id: 13. Value: 0.00392163.	// 4, RS_Y
				10-20 20:30:55.583: I/ARK2D(24226): Index: 5. Axis Id: 14. Value: -1.			// 5, RT
*/


			#endif
			return axis;
		}

		// CONTROLLER KEY IDS (android)
			// DPAD_UP 		104
			// DPAD_DOWN 	105
			// DPAD_LEFT	109
			// DPAD_RIGHT	108
			// L3			102
			// R3			103
			// BUTTON_A 	96 		(button o)
			// BUTTON_B 	99 		(button a)
			// BUTTON_X		97		(button u)
			// BUTTON_Y		98		(button y)
			// LBUMPER		100
			// RBUMPER		101
			// LTRIGGER		110
			// RTRIGGER		106
			// ACTIVATE		107		(button OUYA)
		// CONTROLLER KEY IDS (OUYA)

		// CONTROLLER KEY IDS (android / firetv)
			// L3			106
			// R3			107
			// BUTTON_A 	96 		
			// BUTTON_B 	97 		
			// BUTTON_X		99		
			// BUTTON_Y		100		
			// LBUMPER		102
			// RBUMPER		103

		unsigned int Gamepad::convertIdToButton(Gamepad* gamepad, unsigned int id) {
			#if defined(ARK2D_ANDROID)
				if (ARK2D::getContainer()->getPlatformSpecific()->getPluggable()->firetv_isAmazonFireTV()) {
					switch(id) {
						case 106:
							return BUTTON_L3;
						case 107:
							return BUTTON_R3;
						case 96:
							return BUTTON_A;
						case 97:
							return BUTTON_B;
						case 99:
							return BUTTON_X;
						case 100:
							return BUTTON_Y;
						case 102:
							return BUTTON_LBUMPER;
						case 103:
							return BUTTON_RBUMPER;
					}
					return id; 
				} else if (ARK2D::getContainer()->getPlatformSpecific()->getPluggable()->ouya_isOuya()) {
					switch(id) {
						case 19:
							return DPAD_UP;
						case 20:
							return DPAD_DOWN;
						case 21:
							return DPAD_LEFT;
						case 22:
							return DPAD_RIGHT;
						case 82:
							return BUTTON_ACTIVATE;
						case 102:
							return BUTTON_LBUMPER;
						case 103:
							return BUTTON_RBUMPER;
						case 108:
							return BUTTON_START;
						case 96:
							return BUTTON_A;
						case 97:
							return BUTTON_B;
						case 99: 
							return BUTTON_X;
						case 100:
							return BUTTON_Y;
						case 106:
							return BUTTON_L3;
						case 107:
							return BUTTON_R3;
						case 104:
							return BUTTON_LTRIGGER;
						case 105:
							return BUTTON_RTRIGGER;

					}
				} else { 
					switch(id) {
						case 104:
							return DPAD_UP;
						case 105:
							return DPAD_DOWN;
						case 109:
							return DPAD_LEFT;
						case 108:
							return DPAD_RIGHT;
						case 102:
							return BUTTON_L3;
						case 103:
							return BUTTON_R3;
						case 96:
							return BUTTON_A;
						case 99:
							return BUTTON_B;
						case 97:
							return BUTTON_X;
						case 98:
							return BUTTON_Y;
						case 100:
							return BUTTON_LBUMPER;
						case 101:
							return BUTTON_RBUMPER;
						//case 110:
						//	return TRIGGER_1;
						//case 106:
						//	return TRIGGER_2;
						case 107: 
							return BUTTON_ACTIVATE;
					}
				}
			#elif defined(ARK2D_WINDOWS)
				bool isSaitekCyborgRumble = StringUtil::str_contains(gamepad->name, "Saitek Cyborg Rumble Pad");
				if (isSaitekCyborgRumble) { 
					//ARK2D::getLog()->i(StringUtil::append("saitek convertIdToButton: ", id));
					
					switch(id) { 
						/*case 0:
							return BUTTON_B; 
						case 1:
							return BUTTON_X;
						case 2:
							return BUTTON_A;
						case 3:
							return BUTTON_Y;*/

						/*case 0:
							return BUTTON_A; 
						case 1:
							return BUTTON_B;
						case 2:
							return BUTTON_X; 
						case 3:
							return BUTTON_Y;*/

						case 1:
							return BUTTON_A; 
						case 2:
							return BUTTON_B;
						case 0:
							return BUTTON_X; 
						case 3:
							return BUTTON_Y;

						/*case 4:
							return BUTTON_LBUMPER;
						case 5:
							return BUTTON_RBUMPER;
						case 8:
							return BUTTON_BACK;
						case 9:
							return BUTTON_START;
						case 10:
							return BUTTON_L3;
						case 11:
							return BUTTON_R3;*/



						
						/*case 1:
							return BUTTON_A;
						case BUTTON_X:
							return BUTTON_A;
						case BUTTON_A:
							return BUTTON_B;*/


						/*case BUTTON_A:
							return 1;
						case BUTTON_B:
							return 2;
						case BUTTON_X:
							return 0;
						case BUTTON_Y:
							return 3;
						case BUTTON_LBUMPER:
							return 4;
						case BUTTON_RBUMPER:
							return 5;
						case BUTTON_BACK:
							return 8;
						case BUTTON_START:
							return 9;*/
					}

					
				}
			#endif
			return id;
		}

		

		unsigned int Gamepad::convertButtonToId(Gamepad* gamepad, unsigned int button, bool printInfo) {
			#if defined(ARK2D_MACINTOSH) 
				bool isPS3 = StringUtil::str_contains(gamepad->name, "PLAYSTATION(R)3");
				if (isPS3) {
					//ARK2D::getLog()->v("ps3 convertButtonToId");
					switch(button) {
						case BUTTON_LBUMPER:
							return 10;
						case BUTTON_RBUMPER:
							return 11; 
						case BUTTON_BACK:
							return 0;
						case BUTTON_L3:
							return 1;
						case BUTTON_R3:
							return 2;

						case BUTTON_START:
							return 3;
						case DPAD_UP:
							return 4;
						case DPAD_RIGHT: 
							return 5;
						case DPAD_DOWN:
							return 6;
						case DPAD_LEFT:
							return 7;
						
						case BUTTON_A:
							return 14;
						case BUTTON_B:
							return 13;
						
						case BUTTON_X:
							return 15;
						case BUTTON_ACTIVATE: 
							return 16;
						case BUTTON_Y:
							return 12;
					}
				} 
				switch (button) {
					case DPAD_UP:
						return 5;
					case DPAD_DOWN:
						return 6;
					case DPAD_LEFT:
						return 7; 
					case DPAD_RIGHT:
						return 8;
					case BUTTON_START:
						return 9; 
					case BUTTON_BACK:
						return 10;
					case BUTTON_L3:
						return 11;
					case BUTTON_R3:
						return 12;
					case BUTTON_LBUMPER:
						return 13;
					case BUTTON_RBUMPER:
						return 14; 
					case BUTTON_ACTIVATE: 
						return 15;
					case BUTTON_A:
						return 16;
					case BUTTON_B:
						return 17;
					case BUTTON_X:
						return 18;
					case BUTTON_Y:
						return 19;
				}
			#elif defined(ARK2D_ANDROID)
				if (ARK2D::getContainer()->getPlatformSpecific()->getPluggable()->ouya_isOuya()) {
					switch (button) {
						case DPAD_UP:
							return 19;
						case DPAD_DOWN:
							return 20;
						case DPAD_LEFT:
							return 21;
						case DPAD_RIGHT:
							return 22;
						case BUTTON_LBUMPER:
							return 102;
						case BUTTON_RBUMPER:
							return 103;
						case BUTTON_START:
							return 108;
						case BUTTON_A:
							return 96;
						case BUTTON_B:
							return 97;
						case BUTTON_X: 
							return 99;
						case BUTTON_Y:
							return 100;
						case BUTTON_L3:
							return 106;
						case BUTTON_R3:
							return 107; 
						case BUTTON_LTRIGGER:
							return 104;
						case BUTTON_RTRIGGER:
							return 105;
						case BUTTON_ACTIVATE:
							return 82;
					}
				} else { 
					switch(button) {
						case DPAD_UP: 
							return 104;
						case DPAD_DOWN:
							return 105;
						case DPAD_LEFT:
							return 109;
						case DPAD_RIGHT:
							return 108;
						case BUTTON_L3:
							return 102;
						case BUTTON_R3:
							return 103;
						case BUTTON_A:
							return 96;
						case BUTTON_B:
							return 99;
						case BUTTON_X:
							return 97;
						case BUTTON_Y:
							return 98;
						case BUTTON_LBUMPER:
							return 100;
						case BUTTON_RBUMPER:
							return 101;
						//case 110:
						//	return TRIGGER_1;
						//case 106:
						//	return TRIGGER_2;
						case BUTTON_ACTIVATE: 
							return 107;
					}
				}
			#endif
			return button;
		}

		#define REG_STRING_MAX 255

		#ifdef ARK2D_WINDOWS
			const char* Gamepad::getGamepadDescription(unsigned int joystickID, JOYINFOEX into, JOYCAPS caps) {

				char * description = "";
				char subkey[REG_STRING_MAX];
				HKEY topKey, key;
				LONG result;

				snprintf(subkey, REG_STRING_MAX, "%s\\%s\\%s", REGSTR_PATH_JOYCONFIG, caps.szRegKey, REGSTR_KEY_JOYCURR);
					result = RegOpenKeyEx(topKey = HKEY_LOCAL_MACHINE, subkey, 0, KEY_READ, &key);
					if (result != ERROR_SUCCESS) {
						result = RegOpenKeyEx(topKey = HKEY_CURRENT_USER, subkey, 0, KEY_READ, &key);
					}
					if (result == ERROR_SUCCESS) {
						char value[REG_STRING_MAX];
						char nam[REG_STRING_MAX];
						DWORD nameSize;

						snprintf(value, REG_STRING_MAX, "Joystick%d%s", joystickID + 1, REGSTR_VAL_JOYOEMNAME);
						nameSize = sizeof(nam);
						result = RegQueryValueEx(key, value, NULL, NULL, (LPBYTE) nam, &nameSize);
						RegCloseKey(key);

						if (result == ERROR_SUCCESS) {
							snprintf(subkey, REG_STRING_MAX, "%s\\%s", REGSTR_PATH_JOYOEM, nam);
							result = RegOpenKeyEx(topKey, subkey, 0, KEY_READ, &key);

							if (result == ERROR_SUCCESS) {
								nameSize = sizeof(nam);
								result = RegQueryValueEx(key, REGSTR_VAL_JOYOEMNAME, NULL, NULL, NULL, &nameSize);

								if (result == ERROR_SUCCESS) {
									description = (char*) malloc(nameSize);
									result = RegQueryValueEx(key, REGSTR_VAL_JOYOEMNAME, NULL, NULL, (LPBYTE) description, &nameSize);
								}
								RegCloseKey(key);

								if (result == ERROR_SUCCESS) {
									return description;
								}
								free(description);
							}
						}
					}

					description = (char*) malloc(strlen(caps.szPname) + 1);
					strcpy(description, caps.szPname);

					return description;

			}
		#elif defined(ARK2D_MACINTOSH)
			string Gamepad::getGamepadDescription(IOHIDDeviceRef device) {
				CFStringRef cfProductName;
				string s = "";

				CFStringRef cfProductKey = CFSTR(kIOHIDProductKey);
				cfProductName = (CFStringRef) IOHIDDeviceGetProperty(device, cfProductKey);
				if (cfProductName == NULL || CFGetTypeID(cfProductName) != CFStringGetTypeID()) {
					s = string("Unknown");
				} else {

					const char* controllerName = CFStringGetCStringPtr(cfProductName, CFStringGetSmallestEncoding(cfProductName));
					if (controllerName != NULL) {
						s += string(controllerName);
					}

				}
				return s;
			}

			unsigned int Gamepad::convertCookieToButton(unsigned int cookie) {
				bool isPS3 = StringUtil::str_contains(this->name, string("PLAYSTATION(R)3"));
				//ARK2D::getLog()->v("name...");
				//ARK2D::getLog()->v(this->name);
				if (isPS3) {
					//ARK2D::getLog()->v("ps3 convertCookieToButton");
					switch(cookie) {
						case 7:
							return BUTTON_BACK;
						case 8:
							return BUTTON_L3;
						case 9:
							return BUTTON_R3;
						case 10:
							return BUTTON_START;
						case 11:
							return DPAD_UP;
						case 12: 
							return DPAD_RIGHT;
						case 13:
							return DPAD_DOWN;
						case 14:
							return DPAD_LEFT;
						case 15: 
							// PS3 LEFT TRIGGER
							return BUTTON_NULL;
						case 16: 
							// PS3 RIGHT TRIGGER
							return BUTTON_NULL;
						case 17:
							return BUTTON_LBUMPER;
						case 18:
							return BUTTON_RBUMPER; 
						case 19:
							return BUTTON_Y;
						case 20:
							return BUTTON_B;
						case 21:
							return BUTTON_A;
						case 22:
							return BUTTON_X;
						case 23: 
							return BUTTON_ACTIVATE;
					}
					//ARK2D::getLog()->w("Button press on PS3 controller not recognised.");
					return BUTTON_ACTIVATE;
				} 
				switch(cookie) {
					case 10:
						return DPAD_UP;
					case 11:
						return DPAD_DOWN;
					case 12:
						return DPAD_LEFT;
					case 13: 
						return DPAD_RIGHT;
					case 14:
						return BUTTON_START;
					case 15:
						return BUTTON_BACK;
					case 16:
						return BUTTON_L3;
					case 17:
						return BUTTON_R3;
					case 18:
						return BUTTON_LBUMPER;
					case 19:
						return BUTTON_RBUMPER; 
					case 20: 
						return BUTTON_ACTIVATE;
					case 21:
						return BUTTON_A;
					case 22:
						return BUTTON_B;
					case 23:
						return BUTTON_X;
					case 24:
						return BUTTON_Y;
				}
				return BUTTON_ACTIVATE;
			}

			
		#elif defined(ARK2D_UBUNTU_LINUX)
			const char* Gamepad::getGamepadDescription(unsigned int joystickID) {
				string burp = ARK2D::getInput()->getGamepad(joystickID)->description;
				return burp.c_str();
			}
		#else
			const char* Gamepad::getGamepadDescription(unsigned int joystickID) {
				return "gamepad desc";
			}
		#endif

		/*
		double Gamepad::currentTime() {
			// HACK: No timestamp data from joyGetInfoEx, so we make it up
			static LARGE_INTEGER frequency;
			LARGE_INTEGER currentTime;

			if (frequency.QuadPart == 0) {
				QueryPerformanceFrequency(&frequency);
			}
			QueryPerformanceCounter(&currentTime);

			return (double) currentTime.QuadPart / frequency.QuadPart;
		}
		*/

		unsigned int Gamepad::s_nextGamepadId = 0;
		unsigned int Gamepad::getNextGamepadId() 
		{
			vector<Gamepad*>* pads = ARK2D::getContainer()->getGamepads();
			if (pads->size() == 0) {
				s_nextGamepadId++;
				return 0;
			}

			unsigned int maxId = 0;
			for(unsigned int i = 0; i < pads->size(); ++i) {
				if (pads->at(i)->id >= maxId) {
					maxId = pads->at(i)->id;
				}
			}

			for(unsigned int i = 0; i <= maxId; ++i) {
				Gamepad* boop = ARK2D::getInput()->getGamepad(i);
				if (boop == NULL) {
					//ARK2D::getLog()->i(StringUtil::append("getNextGamepadId boop: ", i));
					return i;
				}
			}

			//ARK2D::getLog()->i(StringUtil::append("getNextGamepadId return: ", m_gamepadId));
			s_nextGamepadId = maxId + 1;
			return s_nextGamepadId;

		}

		string Gamepad::toString() {

			string nl = "\n";
			string s = "";

			s += "{" + nl;
			s += "	\"id\": \"" + Cast::toString<unsigned int>(id) + "\"," + nl;
			s += "	\"productId\": \"" + Cast::toString<signed int>(productId) + "\"," + nl;
			s += "	\"vendorId\": \"" + Cast::toString<signed int>(vendorId) + "\"," + nl;
			s += "	\"name\": \"" + name + "\"," + nl;
			//s += "	\"description\": \"" + description + "\"," + nl;

			#ifdef ARK2D_ANDROID
				s += "	\"deviceId\": \"" + Cast::toString<unsigned int>(deviceId) + "\"," + nl;
			#endif

			s += "	\"numButtons\": \"" + Cast::toString<unsigned int>(numButtons) + "\"," + nl;
			s += "	\"buttons\": [ "+ nl;
			for(unsigned int i = 0; i < buttons.size(); ++i) {
				s += "		{ "+ nl;
				
				s += "			\"id\": \"" + Cast::toString<unsigned int>(buttons.at(i)->id) + "\"" + nl;
				#ifdef ARK2D_MACINTOSH
					//s += "			\"cookie\": \"" + buttons.at(i)->cookie + "\"," + nl;
				#endif

				if (i != buttons.size() - 1) {
					s += "		},"+ nl;
				} else {
					s += "		}" + nl;
				}
			}
			s += "	], "+ nl;
			
			s += "	\"numAxes\": \"" + Cast::toString<signed int>(numAxes) + "\"," + nl;
			s += "	\"axes\": [ "+ nl;
			for(unsigned int i = 0; i < axes.size(); ++i) {
				s += "		{ "+ nl;
				
				s += "			\"id\": \"" + Cast::toString<unsigned int>(axes.at(i)->id) + "\"," + nl;
				#ifdef ARK2D_WINDOWS
					s += "			\"axisId\": \"" + Cast::toString<unsigned int>(axes.at(i)->axisId) + "\"," + nl;
					s += "			\"rangeMin\": \"" + Cast::toString<unsigned int>(axes.at(i)->rangeMin) + "\"," + nl;
					s += "			\"rangeMax\": \"" + Cast::toString<unsigned int>(axes.at(i)->rangeMax) + "\"," + nl;
				#endif
				s += "			\"attached\": \"true\"" + nl;

				if (i != axes.size() - 1) {
					s += "		},"+ nl;
				} else {
					s += "		}" + nl;
				}
			}
			s += "	]"+ nl;
			s += "}"+ nl;
			return s;
		}
		

		Gamepad::~Gamepad() {

		}
	}
}
