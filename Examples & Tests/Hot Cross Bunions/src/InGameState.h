/*
 * InGameState.h
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#ifndef INGAMESTATE_H_
#define INGAMESTATE_H_

#include "ARK.h"

class DefaultGame;
#include "Player.h"
#include "FloatingFadingText.h"
#include "Ingredient.h"
#include "Recipe.h"

class InGameState : public GameState {
	public:

		ARKVector<Ingredient*> ingredients;
		ARKVector<Recipe*> recipes;
		vector<Image*> ingredientImages;
		vector<string> ingredientNames;
		vector<string> witchLines;
		unsigned int witchLineCurrent;

		Player* player;
		Image* bg;

		ARKVector<FloatingFadingText*> fadingText;

		bool paused;
		float witchWaitTimer;
		float witchDurationTimer;

	public:
		InGameState();

		unsigned int id();

		void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		void init(GameContainer* container, StateBasedGame* game);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void render(GameContainer* container, StateBasedGame* game, Graphics* g);

		void prepareRecipes();

		virtual ~InGameState();
};

#endif /* INGAMESTATE_H_ */
