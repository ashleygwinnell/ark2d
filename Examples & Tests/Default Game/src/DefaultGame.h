/*
 * DefaultGame.h
 *
 *  Created on: 17 Jan 2011
 *      Author: Ashley
 */

#ifndef DEFAULTGAME_H_
#define DEFAULTGAME_H_

#include "ARK.h"

class DefaultGame : public Game {
	public:
		DefaultGame(const char* title);
		void init(GameContainer* container);
		void update(GameContainer* container, GameTimer* timer);
		void render(GameContainer* container, Graphics* g);
		void resize(GameContainer* container, int width, int height);
		virtual ~DefaultGame();
};

#endif /* DEFAULTGAME_H_ */
