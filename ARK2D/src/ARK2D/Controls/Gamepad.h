/*
 * Gamepad.h
 *
 *  Created on: 8 Nov 2010
 *      Author: Ashley
 */

#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include <vector>
#include <set>
#include <map>

#ifdef ARK2D_WINDOWS
	#include <windows.h>
	#include <regstr.h>
#endif

#include <stdlib.h>
#include <stdio.h>

using std::vector;
using std::map;
using std::set;

namespace ARK {
	namespace Controls {

		/*!
		 * \brief A way to determine gamepads/controllers and use them to control your game.
		 *
		 * @todo Controller API on OS X and other platforms
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class Gamepad {
			public:

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

				Gamepad();

				int getDPadPosition();

				bool isButtonPressed(unsigned int b);
				bool isButtonReleased(unsigned int b);
				bool isButtonDown(unsigned int b);

				void pressButton(unsigned int key);
				void releaseButton(unsigned int key);

				void clearButtonPressedRecord();

				virtual ~Gamepad();



				//static double currentTime();


				//map<int, bool> buttonDownBuffer;
				set<int> pressedEvents;
				set<int> releasedEvents;

				bool initted;
				unsigned int id; // id in the vector
				int productId; // USB product ID as returned by the driver. Used to determine the model of the device.
				int vendorId; // Same as product ID.
				const char* name; // Human readable device name.

				int xAxisIndex;
				int yAxisIndex;
				int zAxisIndex;

				int rAxisIndex;
				int uAxisIndex;
				int vAxisIndex;

				int povXAxisIndex;
				int povYAxisIndex;
				int povZAxisIndex;

				int dpadPosition;

				unsigned int numButtons;
				vector<bool> buttonStates;

				#ifdef ARK2D_WINDOWS
					DWORD win32_dwButtons;
					static const char* getGamepadDescription(unsigned int id, JOYINFOEX info, JOYCAPS caps);
				#else
					static const char* getGamepadDescription(unsigned int id);
				#endif
		};
	}
}

#endif /* GAMEPAD_H_ */
