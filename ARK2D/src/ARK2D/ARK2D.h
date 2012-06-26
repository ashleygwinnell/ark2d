/*
 * ARK2D.h
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#ifndef ARK2D_H_
#define ARK2D_H_

#include "Namespaces.h"
#include "Includes.h"


/*!
 * \brief Main static way of accessing global variables such as container, game, input and log.
 *
 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
 */
class ARK2D {
	friend class ARK::Core::GameContainer;

	public:
		static GameContainer* getContainer();
		static Game* getGame();
		static Renderer* getRenderer();
		static Input* getInput();
		static Log* getLog();
		static unsigned int getPlatform();

		static bool is64BitArchitecture();
		static bool isBigEndian();

		static const unsigned int PLATFORM_WINDOWS = 0;
		static const unsigned int PLATFORM_OSX = 1;
		static const unsigned int PLATFORM_ANDROID = 2;
		static const unsigned int PLATFORM_IPHONE = 3;

	private:
		static GameContainer* s_container;
		static Game* s_game;
		static Renderer* s_graphics;
		static Input* s_input;
		static Log* s_log;

};

#endif /* ARK2D_H_ */
