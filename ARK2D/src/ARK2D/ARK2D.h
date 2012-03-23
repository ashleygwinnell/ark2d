/*
 * ARK2D.h
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#ifndef ARK2D_H_
#define ARK2D_H_

#include "ARK2D_namespaces.h"

#if defined(ARK2D_ANDROID)
	#include <string>
	#include <vector>
	#include <list>
	#include <map>
	#include <sstream>
	#include <iostream>
	#include <fstream>
	#include <jni.h>
	#include <sys/time.h>
	#include <time.h>
	#include <android/log.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <zip.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <math.h>
	#define STL_AVAILABLE

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
	#include <list>
	#include <map>
	#include <fstream>
	#include <iostream>
	#include <cstdio>
	#include <sstream>
	#include <cstring>
	#include <math.h>
	#include <time.h>
	#include <ctime>
	#include <sys/time.h>

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
class Graphics;

/*!
 * \brief Main static way of accessing global variables such as container, game, input and log.
 *
 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
 */
class ARK2D {
	friend class GameContainer;

	public:
		static GameContainer* getContainer();
		static Game* getGame();
		static Graphics* getGraphics();
		static Input* getInput();
		static Log* getLog();
		static unsigned int getPlatform();

		static bool is64BitArchitecture();
		static bool isBigEndian();

		static const unsigned int PLATFORM_WINDOWS = 0;
		static const unsigned int PLATFORM_OSX = 1;
		static const unsigned int PLATFORM_ANDROID = 2;

	private:
		static GameContainer* s_container;
		static Game* s_game;
		static Graphics* s_graphics;
		static Input* s_input;
		static Log* s_log;

};

#endif /* ARK2D_H_ */
