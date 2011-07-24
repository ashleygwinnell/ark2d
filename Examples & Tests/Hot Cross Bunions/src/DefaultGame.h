/*
 * DefaultGame.h
 *
 *  Created on: 17 Jan 2011
 *      Author: Ashley
 */

#ifndef DEFAULTGAME_H_
#define DEFAULTGAME_H_

//#define ARCADE_VERSION_DEFINE true
#define ORIGINAL_WIDTH 640
#define ORIGINAL_HEIGHT 480


#include "ARK.h"
#include "Player.h"
#include "Ingredient.h"
#include "Recipe.h"
#include "FloatingFadingText.h"

#include "BlankState.h"
#include "MenuState.h"
#include "ViewHighscoresState.h"
#include "InstructionsState.h"
#include "InGameState.h"
#include "WinState.h"

#include <map>

using namespace std;

class DefaultGame : public StateBasedGame {
	public:
		BMFont* font;
		BMFont* font_small;
		Image* sheet;
		SpriteSheetDescription* desc;

		Image* witch;
		Image* witch_fat;
		Image* arrow_up;
		Image* arrow_down;

		Sound* music;
		Sound* snd_enter;
		Sound* snd_good;
		Sound* snd_bad;
		Sound* snd_keypress;


		unsigned int levelNumber;
		int score;
		int multiplier;
		int contamination;

		BlankState* state_blank;
		MenuState* state_menu;
		ViewHighscoresState* state_viewhighscores;
		InstructionsState* state_instructions;
		InGameState* state_game;
		WinState* state_win;

		ParticleSystem* particles;

		LocalHighscores* local_highscores;

		DefaultGame(const char* title);
		void reset();
		virtual void initStates(GameContainer* container);
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Graphics* g);
		void resize(GameContainer* container, int width, int height);

		Recipe* getCurrentRecipe();

		static DefaultGame* getGame();
		static const bool ARCADE_VERSION = false;

		#ifdef ARCADE_VERSION_DEFINE
			static const int KEY_ENTER = Input::KEY_1;
			static const int KEY_BACK = Input::KEY_ESCAPE;
			static const int KEY_UP = Input::KEY_UP;
			static const int KEY_LEFT = Input::KEY_LEFT;
			static const int KEY_RIGHT = Input::KEY_RIGHT;
			static const int KEY_DOWN = Input::KEY_DOWN;
		#else
			static const int KEY_ENTER = Input::KEY_ENTER;
			static const int KEY_BACK = Input::KEY_BACKSPACE;
			static const int KEY_UP = Input::KEY_UP;
			static const int KEY_LEFT = Input::KEY_LEFT;
			static const int KEY_RIGHT = Input::KEY_RIGHT;
			static const int KEY_DOWN = Input::KEY_DOWN;
		#endif

		static bool isAnyKeyPressed();

};

#endif /* DEFAULTGAME_H_ */
