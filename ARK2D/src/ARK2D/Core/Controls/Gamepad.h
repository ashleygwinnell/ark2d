/*
 * Gamepad.h
 *
 *  Created on: 8 Nov 2010
 *      Author: Ashley
 */

#ifndef ARK_CORE_CONTROLS_GAMEPAD_H_
#define ARK_CORE_CONTROLS_GAMEPAD_H_

#if (defined(ARK2D_WINDOWS)) //|| defined(ARK2D_WINDOWS_STORE))
 	#include <winsock2.h>
	#include <windows.h>
	#include <regstr.h>
	#include <mmsystem.h>
	#include <Xinput.h>
#elif defined(ARK2D_MACINTOSH)
	#include <IOKit/hid/IOHIDLib.h>
	#include <limits.h>
	#include <mach/mach.h>
	#include <mach/mach_time.h>
#endif

#include "../../Namespaces.h"

#include "Input.h"


//#if defined(ARK2D_XBOXONE) || defined(ARK2D_WINDOWS_STORE)
	//using namespace Windows::Xbox::Input;
    //using namespace Windows::Foundation; // this will be redefined twice and that's bad.
//#endif

#if defined(ARK2D_WINDOWS_STORE)
 	using namespace Windows::UI::Input;
#endif

// Linux help/examples:
// http://scaryreasoner.wordpress.com/2008/02/22/programming-joysticks-with-linux/
// http://ludobloom.com/svn/StemLibProjects/gamepad/tags/1.3.0/source/gamepad/
namespace ARK {
	namespace Core {
		namespace Controls {

			/*!
			 * \brief A way to determine gamepads/controllers and use them to control your game.
			 *
			 * @todo Controller API on OS X and other platforms
			 *
			 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
			 */

			class ARK2D_API GamepadButton {
				public:
					unsigned int id;
					#ifdef ARK2D_MACINTOSH
						IOHIDElementCookie cookie;
					#endif
					bool down;
			};

			class ARK2D_API GamepadAxis {
				public:
					unsigned int id;
					float value;
					#ifdef ARK2D_MACINTOSH
						IOHIDElementCookie cookie;
						CFIndex logicalMin;
						CFIndex logicalMax;
						bool hasNullState;
						bool isHatSwitch;
						bool isHatSwitchSecondAxis;
						GamepadAxis* nextHatSwitch;
					#elif defined(ARK2D_WINDOWS)
						unsigned int axisId;
						unsigned int rangeMin;
						unsigned int rangeMax;
					#elif defined(ARK2D_ANDROID)
						unsigned int axisId;
					#elif defined(ARK2D_UBUNTU_LINUX)
						// ...
					#endif

			};




			// This stores the values that the controller passes through on button presses.
			// When we receive button events we convert these to ARK2D values.
			// e.g. if (button == mapping->a) { return Gamepad::BUTTON_A; }
			class GamepadMapping {
				public:
					string name;
					unsigned int vendorId;
					unsigned int productId;
					std::map<unsigned int, signed int> buttons; // Gamepad::BUTTON_A -> SomeController::BUTTON_A.
					std::map<unsigned int, signed int> axes;
					std::map<signed int, unsigned int> buttonsInverse; // SomeController::BUTTON_A -> Gamepad::BUTTON_A.
					std::map<signed int, unsigned int> axesInverse;
					bool shared_triggers_axis;
					bool xinput;
					void toInverse();
					void toRegular();
					string toString();

			};

			class ARK2D_API Gamepad {
				public:

					static const unsigned int ANALOG_STICK_1_X = 0;
					static const unsigned int ANALOG_STICK_1_Y = 1;
					static const unsigned int ANALOG_STICK_2_X = 2;
					static const unsigned int ANALOG_STICK_2_Y = 3;
					static const unsigned int TRIGGER_1 = 4;
					static const unsigned int TRIGGER_2 = 5;


					static const unsigned int BUTTON_A = 0;
					static const unsigned int BUTTON_B = 1;
					static const unsigned int BUTTON_X = 2;
					static const unsigned int BUTTON_Y = 3;
					static const unsigned int BUTTON_LBUMPER = 4;
					static const unsigned int BUTTON_RBUMPER = 5;
					static const unsigned int BUTTON_BACK = 6;
					static const unsigned int BUTTON_START = 7;
					static const unsigned int BUTTON_L3 = 8;
					static const unsigned int BUTTON_R3 = 9;
					static const unsigned int BUTTON_ACTIVATE = 10;
					static const unsigned int BUTTON_LTRIGGER = 11;
					static const unsigned int BUTTON_RTRIGGER = 12;
					static const unsigned int BUTTON_NULL = 255;

					static const unsigned int DPAD_NONE = 100;
					static const unsigned int DPAD_LEFT = 101;
					static const unsigned int DPAD_UP_LEFT = 102;
					static const unsigned int DPAD_UP = 103;
					static const unsigned int DPAD_UP_RIGHT = 104;
					static const unsigned int DPAD_RIGHT = 105;
					static const unsigned int DPAD_DOWN_RIGHT = 106;
					static const unsigned int DPAD_DOWN = 107;
					static const unsigned int DPAD_DOWN_LEFT = 108;

					static const signed int XBOX360_DPAD_LEFT = 27000;
					static const signed int XBOX360_DPAD_UP = 0;
					static const signed int XBOX360_DPAD_RIGHT = 9000;
					static const signed int XBOX360_DPAD_DOWN = 18000;
					static const signed int XBOX360_DPAD_DOWN_RIGHT = 13500;
					static const signed int XBOX360_DPAD_DOWN_LEFT = 22500;
					static const signed int XBOX360_DPAD_UP_RIGHT = 4500;
					static const signed int XBOX360_DPAD_UP_LEFT = 31500;

					bool m_triggersSendBumperEvents;
					float m_previousLTriggerValue;
					float m_previousRTriggerValue;
					bool m_sharedTriggerAxis; // true if Left and Right triggers share the same input axis.
					bool virtualpad; // true if is a fake gamepad from the debug screen.

					Gamepad();
					void addStandardAxesAndButtons();
					void update();

					int getDPadPosition();

					bool isButtonPressed(unsigned int b);
					bool isButtonReleased(unsigned int b);
					bool isButtonDown(unsigned int b);
					bool isAnyButtonPressed();

					void pressButton(unsigned int key);
					void releaseButton(unsigned int key);
					bool hasButton(unsigned int key);

					void moveAxis(unsigned int axis, float val);


					void clearButtonPressedRecord();

					string toString();
					GamepadMapping* getMapping();
					virtual ~Gamepad();



					//static double currentTime();


					//map<int, bool> buttonDownBuffer;
					set<int> pressedEvents;
					set<int> releasedEvents;

					bool initted;
					unsigned int id; // id in the vector
					int productId; // USB product ID as returned by the driver. Used to determine the model of the device.
					int vendorId; // Same as product ID.
					string name; // Human readable device name.

					#if defined(ARK2D_XBOXONE)// || defined(ARK2D_WINDOWS_STORE)

						Windows::Xbox::Input::IGamepad^ m_currentGamepad;
	        			Windows::Xbox::Input::IGamepadReading^ m_currentGamepadReading;
					#elif defined(ARK2D_WINDOWS_STORE)

						Windows::Gaming::Input::Gamepad^ m_currentGamepad;
						Windows::Gaming::Input::GamepadReading m_currentGamepadReading;

					#elif defined(ARK2D_WINDOWS) || defined(ARK2D_WINDOWS_STORE)

						JOYINFOEX lastState;
						int povXAxisIndex;
						int povYAxisIndex;
						bool xinput;
						XINPUT_STATE lastStateXInput;

					#elif defined(ARK2D_ANDROID)

						unsigned int deviceId;

					#elif defined(ARK2D_UBUNTU_LINUX)

						//unsigned int deviceID;
						pthread_t thread;
						int fd;
						char * path;
						int buttonMap[KEY_CNT - BTN_MISC];
						int axisMap[ABS_CNT];
						struct input_absinfo axisInfo[ABS_CNT];
						char* description;



					#endif





					int dpadPosition;

					unsigned int numButtons;
					vector<GamepadButton*> buttons;

					signed int numAxes;
					vector<GamepadAxis*> axes;

					inline string getName() { return name; }
					inline unsigned int getId() { return id; }


					#ifdef ARK2D_WINDOWS
						DWORD win32_dwButtons;
						static const char* getGamepadDescription(unsigned int id, JOYINFOEX info, JOYCAPS caps);
					#else
						static const char* getGamepadDescription(unsigned int id);
					#endif

					bool isAxis(unsigned int axisId);
					GamepadAxis* getAxis(unsigned int axisId);
					float getAxisValue(unsigned int axisId);

					static unsigned int convertButtonToId(Gamepad* gamepad, unsigned int button, bool printInfo = true);
					//static unsigned int convertIdToButton(Gamepad* gamepad, unsigned int id);
					static unsigned int convertAxisToId(Gamepad* gamepad, unsigned int axis);

					static unsigned int convertSystemButtonToARKButton(Gamepad* gamepad, unsigned int button);
					static unsigned int convertSystemAxisToARKAxis(Gamepad* gamepad, unsigned int axis);

					#ifdef ARK2D_MACINTOSH
						IOHIDDeviceRef deviceRef;
						static string getGamepadDescription(IOHIDDeviceRef device);
						unsigned int convertCookieToButton(unsigned int cookie);
					#endif

					static unsigned int s_nextGamepadId;
					static unsigned int getNextGamepadId();


					static vector<GamepadMapping>* s_gamepadMapping;
					static void initMapping();
					bool isXbox360Controller();
					bool isPS4Controller();
					bool isConfigured();


				};
		}
	}
}

#endif /* GAMEPAD_H_ */
