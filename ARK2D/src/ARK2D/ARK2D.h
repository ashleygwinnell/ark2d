/*
 * ARK2D.h
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#ifndef ARK2D_H_
#define ARK2D_H_

#if defined(ARK2D_ANDROID)

#elif defined(__linux__)
	#define ARK2D_UBUNTU_LINUX
#elif (defined(__MACH__) || defined(__APPLE__))
	#define ARK2D_MACINTOSH
#elif defined(_WIN32)
	#define ARK2D_WINDOWS
	#include <windows.h>
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
