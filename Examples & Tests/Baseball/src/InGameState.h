/*
 * InGameState.h
 * 08 Jun 2011
 */

#ifndef INGAMESTATE_H_
#define INGAMESTATE_H_

#include "ARK.h"
#include "Player.h"
#include "Boss.h"
#include "Baseball.h"

class DefaultGame;

class InGameState : public GameState {
	public:

		Player* player;
		Boss* boss;
		ARKVector<Baseball*> baseballs;

		float m_timer;
		float m_timerLevelOne;
		float m_timerLevelTwo;
		float m_timerLevelThree;
		float m_timerLevelFour;
		float m_timerLevelFive;
		float m_timerLevelSix;

		InGameState();
		void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		unsigned int id();
		void init(GameContainer* container, StateBasedGame* game);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void render(GameContainer* container, StateBasedGame* game, Graphics* g);

		virtual void keyPressed(unsigned int key);
		virtual void keyReleased(unsigned int key);

		virtual ~InGameState();
};

#endif /* INGAMESTATE_H_ */
