/*
 * DefaultGame.h
 *
 *  Created on: 08 Jun 2011
 *      Author: Ashley
 */

#ifndef DEFAULTGAME_H_
#define DEFAULTGAME_H_

#include "ARK.h"

#include "BlankState.h"
#include "MenuState.h"
#include "InstructionsState.h"
#include "InGameState.h"
#include "WinState.h"
#include "FailState.h"

class StatesList {
	public:
		static const unsigned int STATE_BLANK = 0;
		static const unsigned int STATE_MENU = 1;
		static const unsigned int STATE_INSTRUCTIONS = 2;
		static const unsigned int STATE_INGAME = 3;
		static const unsigned int STATE_WIN = 4;
		static const unsigned int STATE_FAIL = 5;
		static const unsigned int END = 6;

};

class DefaultGame : public StateBasedGame {
	public:

		BlankState* state_blank;
		MenuState* state_menu;
		InstructionsState* state_instructions;
		InGameState* state_ingame;
		WinState* state_win;
		FailState* state_fail;

		BMFont* myfont;

		vector<Sound*> screamsSounds;
		vector<Sound*> pukeSounds;
		Sound* musicHappy;
		Sound* musicSad;

		Image* back;
		Image* baseballImage1;
		Image* baseballImage2;

		int SCORE;
		int LIVES;

		DefaultGame(const char* title);
		void initStates(GameContainer* container);
		void update(GameContainer* container, GameTimer* timer);
		void render(GameContainer* container, Graphics* g);
		void resize(GameContainer* container, int width, int height);
		virtual ~DefaultGame();

		static DefaultGame* getGame();
};

#endif /* DEFAULTGAME_H_ */
