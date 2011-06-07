/*
 * Transition.h
 *
 *  Created on: 1 Aug 2010
 *      Author: Ashley
 */

#ifndef TRANSITION_H_
#define TRANSITION_H_

//#include "../GameContainer.h"
class GameContainer;
#include "../Game.h"
#include "../GameTimer.h"
#include "../Graphics.h"

#include "GameState.h"
#include "StateBasedGame.h"

class Transition {
	public:
		Transition();
		void setEasing(unsigned int easing);
		unsigned int getEasing();
		virtual void init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) = 0;
		virtual void update(GameContainer* container, StateBasedGame* game, GameTimer* delta) = 0;
		virtual void preRender(GameContainer* container, StateBasedGame* game, Graphics* g) = 0;
		virtual void postRender(GameContainer* container, StateBasedGame* game, Graphics* g) = 0;
		virtual bool isComplete() = 0;
		virtual ~Transition();

	protected:
		GameContainer* m_container;
		GameState* m_from;
		GameState* m_to;
		unsigned int m_easing;
};

#endif /* TRANSITION_H_ */

