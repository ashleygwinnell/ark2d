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
		virtual void init(GameContainer* container);
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Graphics* g);
		virtual void resize(GameContainer* container, int width, int height);
		virtual ~DefaultGame();
};

#endif /* DEFAULTGAME_H_ */
