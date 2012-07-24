/*
 * Input.h
 *
 *  Created on: 12-Dec-2009
 *      Author: Ashley
 */

#ifndef INPUT_H_
#define INPUT_H_

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <set>
#include <vector>
#include <string>

#include "../Namespaces.h"
#include "../Includes.h"

using namespace std;


namespace ARK {
	namespace Controls {

		class Gamepad;


		/*!
		 * \brief The main class for getting Input from the user.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class Input {

			public:
				//#ifdef ARK2D_WINDOWS
					static const int ANDROID_HOME   = 0x400; // 1000 -- made up value that (probably) won't clash.
					static const int ANDROID_MENU   = 0x401; // 1000 -- made up value that (probably) won't clash.
					static const int ANDROID_BACK   = 0x402; // 1000 -- made up value that (probably) won't clash.
					static const int ANDROID_SEARCH = 0x403; // 1000 -- made up value that (probably) won't clash.

					static const int MOUSE_BUTTON_LEFT = 0x01;
					static const int MOUSE_BUTTON_RIGHT = 0x02;
					static const int MOUSE_BUTTON_MIDDLE = 0x04;
					static const int KEY_ENTER = 0x0D;
					static const int KEY_TAB = 0x09;
					static const int KEY_BACKSPACE = 0x08;
					static const int KEY_SHIFT = 0x10;
					static const int KEY_CONTROL = 0x11;
					static const int KEY_ALT = 0x12;
					static const int KEY_CAPSLOCK = 0x14;
					static const int KEY_ESCAPE = 0x1B;
					static const int KEY_SPACE = 0x20;
					static const int KEY_PAGEUP = 0x21;
					static const int KEY_PAGEDOWN = 0x22;
					static const int KEY_END = 0x23;
					static const int KEY_HOME = 0x24;
					static const int KEY_LEFT = 0x25;
					static const int KEY_UP = 0x26;
					static const int KEY_RIGHT = 0x27;
					static const int KEY_DOWN = 0x28;
					static const int KEY_PRINTSCREEN = 0x2C;
					static const int KEY_INSERT = 0x2D;
					static const int KEY_DELETE = 0x2E;

					static const int KEY_0 = 0x30;
					static const int KEY_1 = 0x31;
					static const int KEY_2 = 0x32;
					static const int KEY_3 = 0x33;
					static const int KEY_4 = 0x34;
					static const int KEY_5 = 0x35;
					static const int KEY_6 = 0x36;
					static const int KEY_7 = 0x37;
					static const int KEY_8 = 0x38;
					static const int KEY_9 = 0x39;

					static const int KEY_A = 0x41;
					static const int KEY_B = 0x42;
					static const int KEY_C = 0x43;
					static const int KEY_D = 0x44;
					static const int KEY_E = 0x45;
					static const int KEY_F = 0x46;
					static const int KEY_G = 0x47;
					static const int KEY_H = 0x48;
					static const int KEY_I = 0x49;
					static const int KEY_J = 0x4A;
					static const int KEY_K = 0x4B;
					static const int KEY_L = 0x4C;
					static const int KEY_M = 0x4D;
					static const int KEY_N = 0x4E;
					static const int KEY_O = 0x4F;
					static const int KEY_P = 0x50;
					static const int KEY_Q = 0x51;
					static const int KEY_R = 0x52;
					static const int KEY_S = 0x53;
					static const int KEY_T = 0x54;
					static const int KEY_U = 0x55;
					static const int KEY_V = 0x56;
					static const int KEY_W = 0x57;
					static const int KEY_X = 0x58;
					static const int KEY_Y = 0x59;
					static const int KEY_Z = 0x5A;

					static const int KEY_LWIN = 0x5B;
					static const int KEY_RWIN = 0x5C;
					static const int KEY_SELECT = 0x5D;
					static const int KEY_SLEEP = 0x5F;

					static const int KEY_NUMPAD_0 = 0x60;
					static const int KEY_NUMPAD_1 = 0x61;
					static const int KEY_NUMPAD_2 = 0x62;
					static const int KEY_NUMPAD_3 = 0x63;
					static const int KEY_NUMPAD_4 = 0x64;
					static const int KEY_NUMPAD_5 = 0x65;
					static const int KEY_NUMPAD_6 = 0x66;
					static const int KEY_NUMPAD_7 = 0x67;
					static const int KEY_NUMPAD_8 = 0x68;
					static const int KEY_NUMPAD_9 = 0x69;
					static const int KEY_NUMPAD_MULTIPLY = 0x6A;
					static const int KEY_NUMPAD_ADD = 0x6B;
					static const int KEY_NUMPAD_SUBTRACT = 0x6D;
					static const int KEY_NUMPAD_DECIMAL = 0x6E;
					static const int KEY_NUMPAD_DIVIDE = 0x6F;

					static const int KEY_F1 = 0x70;
					static const int KEY_F2 = 0x71;
					static const int KEY_F3 = 0x72;
					static const int KEY_F4 = 0x73;
					static const int KEY_F5 = 0x74;
					static const int KEY_F6 = 0x75;
					static const int KEY_F7 = 0x76;
					static const int KEY_F8 = 0x77;
					static const int KEY_F9 = 0x78;
					static const int KEY_F10 = 0x79;
					static const int KEY_F11 = 0x7A;
					static const int KEY_F12 = 0x7B;

					static const int KEY_NUMLOCK = 0x90;
					static const int KEY_SCROLLLOCK = 0x91;

					static const int KEY_SEMICOLON = 0xBA; // 186;
					static const int KEY_EQUALS = 0xBB; // 187;
					static const int KEY_COMMA = 0xBC; // 188;
					static const int KEY_HYPHEN = 0xBD; // 189;
					static const int KEY_PERIOD = 0xBE; // 190;
					static const int KEY_FORWARD_SLASH = 0xBF; // 191;
					static const int KEY_BACK_SLASH = 0xDC; // 220;
					static const int KEY_OPEN_BRACKET = 0xDB; // 219;
					static const int KEY_CLOSE_BRACKET = 0xDD; // 221;
					static const int KEY_APOSTROPHE = 0xDE; // 222;

					static const int KEY_LSHIFT = 0xA0;
					static const int KEY_RSHIFT = 0xA1;
					static const int KEY_LCONTROL = 0xA2;
					static const int KEY_RCONTROL = 0xA3;
					static const int KEY_LMENU = 0xA4;
					static const int KEY_RMENU = 0xA5;

					static const int KEY_AT = 2000;
					static const int KEY_HASH = 2001;
					static const int KEY_LEFT_SQUARE_BRACKET = 2002;
					static const int KEY_RIGHT_SQUARE_BRACKET = 2003;

					static const char* s_keyboardState;
				//#endif

				//#ifdef _OSX

				//#endif
				//
				//#ifdef ARK2D_UBUNTU_LINUX
				//	static const int KEY_ENTER = 0x0D;
				//	static const int KEY_ESCAPE = 0x1B;
				//
				//	static const int KEY_LEFT = 0x25;
				//	static const int KEY_UP = 0x26;
				//	static const int KEY_RIGHT = 0x27;
				//	static const int KEY_DOWN = 0x28;
				//
				//#endif
		/*
				static const unsigned int KEY_0 = 11;
				static const int KEY_1 = 2;
				static const int KEY_2 = 3;
				static const int KEY_3 = 4;
				static const int KEY_4 = 5;
				static const int KEY_5 = 6;
				static const int KEY_6 = 7;
				static const int KEY_7 = 8;
				static const int KEY_8 = 9;
				static const int KEY_9 = 10;
				static const int KEY_A = 30;
				static const int KEY_ADD = 78;
				static const int KEY_APOSTROPHE = 40;
				static const int KEY_APPS = 221;
				static const int KEY_AT = 145;
				static const int KEY_AX = 150;
				static const int KEY_B = 48;
				static const int KEY_BACK = 14;
				static const int KEY_BACKSLASH = 43;
				static const int KEY_C = 46;
				static const int KEY_CAPITAL = 58;
				static const int KEY_CIRCUMFLEX = 144;
				static const int KEY_COLON = 146;
				static const int KEY_COMMA = 51;
				static const int KEY_CONVERT = 121;
				static const int KEY_D = 32;
				static const int KEY_DECIMAL = 83;
				static const int KEY_DELETE = 211;
				static const int KEY_DIVIDE = 181;
				static const int KEY_DOWN = 208;
				static const int KEY_E = 18;
				static const int KEY_END = 207;
				static const int KEY_ENTER = 28;
				static const int KEY_EQUALS = 13;
				static const int KEY_ESCAPE = 1;
				static const int KEY_F = 33;
				static const int KEY_F1 = 59;
				static const int KEY_F10 = 68;
				static const int KEY_F11 = 87;
				static const int KEY_F12 = 88;
				static const int KEY_F13 = 100;
				static const int KEY_F14 = 101;
				static const int KEY_F15 = 102;
				static const int KEY_F2 = 60;
				static const int KEY_F3 = 61;
				static const int KEY_F4 = 62;
				static const int KEY_F5 = 63;
				static const int KEY_F6 = 64;
				static const int KEY_F7 = 65;
				static const int KEY_F8 = 66;
				static const int KEY_F9 = 67;
				static const int KEY_G = 34;
				static const int KEY_GRAVE = 41;
				static const int KEY_H = 35;
				static const int KEY_HOME = 199;
				static const int KEY_I = 23;
				static const int KEY_INSERT = 210;
				static const int KEY_J = 36;
				static const int KEY_K = 37;
				static const int KEY_KANA = 112;
				static const int KEY_KANJI = 148;
				static const int KEY_L = 38;
				static const int KEY_LALT = 56;
				static const int KEY_LBRACKET = 26;
				static const int KEY_LCONTROL = 29;
				static const int KEY_LEFT = 203;
				static const int KEY_LMENU = 56;
				static const int KEY_LSHIFT = 42;
				static const int KEY_LWIN = 219;
				static const int KEY_M = 50;
				static const int KEY_MINUS = 12;
				static const int KEY_MULTIPLY = 55;
				static const int KEY_N = 49;
				static const int KEY_NEXT = 209;
				static const int KEY_NOCONVERT = 123;
				static const int KEY_NUMLOCK = 69;
				static const int KEY_NUMPAD_0 = 82;
				static const int KEY_NUMPAD_1 = 79;
				static const int KEY_NUMPAD_2 = 80;
				static const int KEY_NUMPAD_3 = 81;
				static const int KEY_NUMPAD_4 = 75;
				static const int KEY_NUMPAD_5 = 76;
				static const int KEY_NUMPAD_6 = 77;
				static const int KEY_NUMPAD_7 = 71;
				static const int KEY_NUMPAD_8 = 72;
				static const int KEY_NUMPAD_9 = 73;
				static const int KEY_NUMPAD_COMMA = 179;
				static const int KEY_NUMPAD_ENTER = 156;
				static const int KEY_NUMPAD_EQUALS = 141;
				static const int KEY_O = 24;
				static const int KEY_P = 25;
				static const int KEY_PAUSE = 197;
				static const int KEY_PERIOD = 52;
				static const int KEY_POWER = 222;
				static const int KEY_PRIOR = 201;
				static const int KEY_Q = 16;
				static const int KEY_R = 19;
				static const int KEY_RALT = 184;
				static const int KEY_RBRACKET = 27;
				static const int KEY_RCONTROL = 157;
				static const int KEY_RETURN = 28;
				static const int KEY_RIGHT = 205;
				static const int KEY_RMENU = 184;
				static const int KEY_RSHIFT = 54;
				static const int KEY_RWIN = 220;
				static const int KEY_S = 31;
				static const int KEY_SCROLL = 70;
				static const int KEY_SEMICOLON = 39;
				static const int KEY_SLASH = 53;
				static const int KEY_SLEEP = 223;
				static const int KEY_SPACE = 57;
				static const int KEY_STOP = 49;
				static const int KEY_SUBTRACT = 74;
				static const int KEY_SYSRQ = 183;
				static const int KEY_T = 20;
				static const int KEY_TAB = 15;
				static const int KEY_U = 22;
				static const int KEY_UNDERLINE = 147;
				static const int KEY_UNLABELED = 151;
				static const int KEY_UP = 200;
				static const int KEY_V = 47;
				static const int KEY_W = 17;
				static const int KEY_X = 45;
				static const int KEY_Y = 21;
				static const int KEY_YEN = 125;
				static const int KEY_Z = 44;
				static const int KEY_MOUSE_LEFT_BUTTON = 0;
				static const int KEY_MOUSE_MIDDLE_BUTTON = 2;
				static const int KEY_MOUSE_RIGHT_BUTTON = 3;*/



				map<int, bool> keyDownBuffer;
				const GameContainer* m_container;

				map<int, string> keyNames;
				map<int, string> keyChars;

				Input();
				Input(const GameContainer* c);

				bool isKeyDown(unsigned int key);
				bool isKeyPressed(unsigned int key);
				bool isKeyReleased(unsigned int key);

				bool isGamepadButtonDown(unsigned int button);
				bool isGamepadButtonDown(Gamepad* gamepad, unsigned int button);
				bool isGamepadButtonPressed(unsigned int button);
				bool isGamepadButtonPressed(Gamepad* gamepad, unsigned int button);

				void pressKey(unsigned int key);
				void releaseKey(unsigned int key);

				const string getKeyName(unsigned int key);
				const string getKeyChar(unsigned int key);
				bool isKeyAlphanumeric(unsigned int key);
				bool isCapsLockOn();

				void clearKeyPressedRecord();

				int getGlobalMouseX();
				int getGlobalMouseY();

				int getMouseX() const;
				int getMouseY() const;

				vector<Gamepad*> getGamepads() const;

				static void setSoftwareKeyboardOpen(bool b);

				~Input();
			public:
				int mouse_x;
				int mouse_y;
				void setGameContainer(const GameContainer* c);
				set<int> pressedEvents;
				set<int> releasedEvents;

		};
	}

}

#endif /* INPUT_H_ */
