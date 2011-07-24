/*
 * Gamepad.cpp
 *
 *  Created on: 8 Nov 2010
 *      Author: Ashley
 */

#include <stdlib.h>
#include <iostream>

#include "ARK2D.h"
#include "Gamepad.h"

Gamepad::Gamepad():
	initted(false),
	id(0),
	productId(0),
	vendorId(0),
	name(""),
	dpadPosition(DPAD_NONE),
	numButtons(0),
	buttonStates() {

}
int Gamepad::getDPadPosition() {
	return dpadPosition;
}

void Gamepad::clearButtonPressedRecord() {
	pressedEvents.clear();
	releasedEvents.clear();
}

void Gamepad::pressButton(unsigned int b) {
	//std::cout << "button " << b << " pressed" << std::endl;
	//buttonDownBuffer[b] = true;
	//buttonStates.assign(b, true);
	buttonStates[b] = true;
	pressedEvents.insert(b);
}

void Gamepad::releaseButton(unsigned int b) {
	//std::cout << "button " << b << " released" << std::endl;
	//buttonDownBuffer[b] = false;
	//buttonStates.assign(b, false);
	buttonStates[b] = false;
	releasedEvents.insert(b);
}

bool Gamepad::isButtonPressed(unsigned int b) {
	std::set<int>::iterator it = pressedEvents.find(b);
	if (it != pressedEvents.end()) {
		return true;
	} else {
		return false;
	}
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
	return buttonStates[b];
	//return buttonStates.at(b);
	//return buttonDownBuffer[b];
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
				char name[REG_STRING_MAX];
				DWORD nameSize;

				snprintf(value, REG_STRING_MAX, "Joystick%d%s", joystickID + 1, REGSTR_VAL_JOYOEMNAME);
				nameSize = sizeof(name);
				result = RegQueryValueEx(key, value, NULL, NULL, (LPBYTE) name, &nameSize);
				RegCloseKey(key);

				if (result == ERROR_SUCCESS) {
					snprintf(subkey, REG_STRING_MAX, "%s\\%s", REGSTR_PATH_JOYOEM, name);
					result = RegOpenKeyEx(topKey, subkey, 0, KEY_READ, &key);

					if (result == ERROR_SUCCESS) {
						nameSize = sizeof(name);
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



Gamepad::~Gamepad() {

}
