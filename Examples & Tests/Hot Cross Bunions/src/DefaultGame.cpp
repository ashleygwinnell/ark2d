/*
 * DefaultGame.cpp
 *
 *  Created on: 17 Jan 2011
 *      Author: Ashley
 */

#include "DefaultGame.h"

#define START_LEVEL_NUMBER 0 // 1

//bool DefaultGame::ARCADE_VERSION;

DefaultGame::DefaultGame(const char* title):
	StateBasedGame(title),
	font(NULL), font_small(NULL),
	sheet(NULL), desc(NULL),
	witch(NULL), witch_fat(NULL),
	arrow_up(NULL), arrow_down(NULL),
	music(NULL),
	snd_enter(NULL), snd_good(NULL), snd_bad(NULL), snd_keypress(NULL),
	levelNumber(START_LEVEL_NUMBER), score(0), multiplier(1), contamination(0),
	particles(NULL) {

}

void DefaultGame::initStates(GameContainer* container) {
	sheet = new Image("data/game/sheet.png");
	desc = new SpriteSheetDescription("data/game/sheet.json");

	font = new BMFont("data/game/04b-09.fnt", "data/game/04b-09.png", Color::magenta);
	font_small = new BMFont("data/game/04b-09-small.fnt", "data/game/04b-09-small.png", Color::magenta);

	arrow_up = sheet->getSubImage(desc->getItemByName("arrow.png"))->getScaledCopy(4, 4);
	arrow_down = arrow_up->getFlippedCopy(false, true);

	witch = sheet->getSubImage(desc->getItemByName("witch.png"))->getScaledCopy(4, 4);
	witch_fat = sheet->getSubImage(desc->getItemByName("witch-fat.png"))->getScaledCopy(4, 4);

	music = new Sound("data/game/hotcrossbunions-ogg.ogg");

	snd_enter = new Sound("data/game/enter.wav");
	snd_good = new Sound("data/game/good.wav");
	snd_bad = new Sound("data/game/bad.wav");
	snd_keypress = new Sound("data/game/keypress.wav");

	local_highscores = new LocalHighscores();
	local_highscores->load();


	state_blank = new BlankState();
	state_menu = new MenuState();
	state_viewhighscores = new ViewHighscoresState();
	state_instructions = new InstructionsState();
	state_game = new InGameState();
	state_win = new WinState();

	//particles = ParticleIO::loadConfiguredSystem("data/game/particles.xml");/
	//particles->m_visible = true;

	addState(state_blank);
	addState(state_menu);
	addState(state_viewhighscores);
	addState(state_instructions);
	addState(state_game);
	addState(state_win);

	if (DefaultGame::ARCADE_VERSION) {
		if (!container->isFullscreen()) {
			container->setFullscreen(true);
		}
	}


	container->getTimer()->flush();

	enterState(state_blank);
	//enterState(state_menu);
	//enterState(state_win);

	levelNumber = 0;

	container->getTimer()->flush();
}

void DefaultGame::reset() {
	levelNumber = START_LEVEL_NUMBER;
	score = 0;
	multiplier = 1;
	contamination = 0;

	state_menu->m_timer = 0.0f;

	state_game->fadingText.removeAll();
	state_game->ingredients.removeAll();
	state_game->prepareRecipes();

	//state_win->nameEntered = false;
	state_win->name = "";
}

void DefaultGame::update(GameContainer* container, GameTimer* timer) {
	StateBasedGame::update(container, timer);
	if (!music->isPlaying()) {
		music->play();
	}

	if (DefaultGame::ARCADE_VERSION) {
		container->setCursorVisible(false);
	}

	//particles->update(timer);

	Input* i = ARK2D::getInput();
	if (i->isKeyPressed(Input::KEY_F4)) {
		container->setFullscreen(!container->isFullscreen());
	}
}

void DefaultGame::render(GameContainer* container, Graphics* g) {
	if (ARCADE_VERSION) {
		/*float scalex = float(container->getWidth()) / float(ORIGINAL_WIDTH);
		float scaley = float(container->getHeight()) / float(ORIGINAL_HEIGHT);
		float scale = 1.0f;
		if (scalex < scaley) {
			scale = scalex;
		} else {
			scale = scaley;
		}*/
		//std::cout << scale << std::endl;
		//g->pushMatrix();
		//g->scale(scale, scale);
		StateBasedGame::render(container, g);
		//g->popMatrix();
	} else {
		StateBasedGame::render(container, g);
	}
	//particles->render(0, 0);
	//particles->render(100, 100);
}

void DefaultGame::resize(GameContainer* container, int width, int height) {

}

Recipe* DefaultGame::getCurrentRecipe() {
	return state_game->recipes.get(levelNumber);
}

DefaultGame* DefaultGame::getGame() {
	return dynamic_cast<DefaultGame*>(ARK2D::getGame());
}

bool DefaultGame::isAnyKeyPressed() {
	Input* i = ARK2D::getInput();
	if (DefaultGame::ARCADE_VERSION) {
		return (
			i->isKeyPressed(Input::KEY_1) ||
			i->isKeyPressed(Input::KEY_Z) ||
			i->isKeyPressed(Input::KEY_X) ||
			i->isKeyPressed(Input::KEY_C)
		);
	}
	return (
		i->isKeyPressed(Input::KEY_ENTER)
	);
}
