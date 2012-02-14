/*
 * ARK2D.h
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#ifndef ARK2D_H_
#define ARK2D_H_

#if defined(ARK2D_ANDROID)
	#include <string>
	#include <vector>
	#include <list>
	#include <sstream>
	#include <iostream>
	#include <jni.h>
	#include <sys/time.h>
	#include <time.h>
	#include <android/log.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <stdlib.h>

#elif defined(__linux__)
	#define ARK2D_UBUNTU_LINUX
	#define STL_AVAILABLE
	#define EXCEPTIONS_AVAILABLE
#elif (defined(__MACH__) || defined(__APPLE__))
	#define ARK2D_MACINTOSH
	#define STL_AVAILABLE
	#define EXCEPTIONS_AVAILABLE
	#include <Cocoa/Cocoa.h>

	#include <stdlib.h>
	#include <string>
	#include <stdio.h>
	#include <vector>
	#include <fstream>
	#include <iostream>
	#include <cstdio>
	#include <sstream>
	#include <cstring>
	#include <math.h>
	#include <time.h>
	#include <ctime>
	#include <sys/time.h>
	#include <list>
#elif defined(_WIN32)
	#define ARK2D_WINDOWS
	#define STL_AVAILABLE
	#define EXCEPTIONS_AVAILABLE
	#include <windows.h>
	#include <mmsystem.h>
	#include <winbase.h>

	#include <stdlib.h>
	#include <string>
	#include <time.h>
	#include <sys/time.h>
	#include <list>

	// #include <limits> // bad in mingw 3.x
#endif

#include <stdint.h>

class GameContainer;
//#include "GameContainer.h"
class Game;
class Input;
class Graphics;
class ARKLog;

class ARK2D {
	friend class GameContainer;

	public:
		static GameContainer* getContainer();
		static Game* getGame();
		static Graphics* getGraphics();
		static Input* getInput();
		static ARKLog* getLog();

		static bool is64BitArchitecture();
		static bool isBigEndian();

	private:
		static GameContainer* s_container;
		static Game* s_game;
		static Graphics* s_graphics;
		static Input* s_input;
		static ARKLog* s_log;

};

#endif /* ARK2D_H_ */
