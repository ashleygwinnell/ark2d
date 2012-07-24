/*
 * ARK2D.cpp
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#include "ARK2D.h"

GameContainer* ARK2D::s_container = 0;
Game* ARK2D::s_game = 0;
Renderer* ARK2D::s_graphics = 0;
Input* ARK2D::s_input = 0;
Log* ARK2D::s_log = 0;

GameContainer* ARK2D::getContainer() {
	return s_container;
}

Game* ARK2D::getGame() {
	return s_game;
}

Renderer* ARK2D::getRenderer() {
	return s_graphics;
}

Input* ARK2D::getInput() {
	return s_input;
}

Log* ARK2D::getLog() {
	return s_log;
}

		
unsigned int ARK2D::getPlatform() {
	#if defined(ARK2D_WINDOWS)
		return PLATFORM_WINDOWS;
	#elif defined(ARK2D_MACINTOSH)
		return PLATFORM_OSX;
	#elif defined(ARK2D_IPHONE)
		return PLATFORM_IPHONE;
	#elif defined(ARK2D_ANDROID)
		return PLATFORM_ANDROID;
	#endif
}
string ARK2D::getPlatformString() {
	#if defined(ARK2D_WINDOWS)
		return "windows";
	#elif defined(ARK2D_MACINTOSH)
		return "osx";
	#elif defined(ARK2D_IPHONE)
		return "iphone";
	#elif defined(ARK2D_ANDROID)
		return "android";
	#endif
}

bool ARK2D::is64BitArchitecture() {

	#ifdef ARK2D_WINDOWS

		bool ret = false;

		typedef bool (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, bool*);
		LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process");

		if (fnIsWow64Process != NULL) {
			if (!fnIsWow64Process(GetCurrentProcess(), &ret)) {
				return false;
			}
		}
		return ret;

	#endif

	return false;
}

bool ARK2D::isBigEndian() {
	/*union {
		uint32_t i;
		char c[4];
	} bint = {0x01020304};

	return bint.c[0] == 1;*/
	static unsigned long x(1);
	static bool result(reinterpret_cast<unsigned char*>(&x)[0] == 0);
	return result;
}


