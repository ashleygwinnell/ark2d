/*
 * ARK2D.h
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#ifndef ARK2D_H_
#define ARK2D_H_

#include "Includes.h"
#include "Namespaces.h"

/*!
 * \brief Main static way of accessing global variables such as container, game, input and log.
 *
 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
 */

class ARK2D_API ARK2D {
	friend class ARK::Core::GameContainer;

	public: 
		static void main(int args, char** argv);
		
		static GameContainer* getContainer();
		static Game* getGame();
		static Renderer* getRenderer();
		static Input* getInput();
		static GameTimer* getTimer();
		static Log* getLog();
		static Strings* getStrings();
		static unsigned int getPlatform();
		static std::string getPlatformString();

		static bool is64BitArchitecture();
		static bool isBigEndian();

		static bool isDebug();
		static void setDebug(bool b);
		
		static bool isExpoMode();
		static void setExpoMode(bool b);

		static bool isSteam();
		static void setSteam(bool b);

		static vector<string>* getCommandLineParameters();
		static bool hasCommandLineParameter(string key);
		static string getCommandLineParameter(string key, string def);
		static void printCommandLineParameters();

		static const unsigned int PLATFORM_WINDOWS = 0;
		static const unsigned int PLATFORM_OSX = 1;
		static const unsigned int PLATFORM_ANDROID = 2;
		static const unsigned int PLATFORM_IPHONE = 3;
		static const unsigned int PLATFORM_IOS = 3;
		static const unsigned int PLATFORM_BROWSER_JS = 4;
		static const unsigned int PLATFORM_BROWSER_FLASCC = 5;
		static const unsigned int PLATFORM_WINDOWS_PHONE_8 = 6;
		static const unsigned int PLATFORM_LINUX = 7;
		static const unsigned int PLATFORM_XBOXONE = 8;
		static const unsigned int PLATFORM_PS4 = 9;
		static const unsigned int PLATFORM_WIIU = 10;

	private:
		static GameContainer* s_container;
		static Game* s_game;
		static Renderer* s_graphics;
		static Input* s_input;
		static Log* s_log;
		static Strings* s_strings;
		static bool s_debug;
		static bool s_expo;
		static bool s_steam;
		static vector<string> s_commandLineParameters;

};

#endif /* ARK2D_H_ */
