/*
 * DefaultGame.cpp
 *
 *  Created on: 08 Jun 2011
 *      Author: Ashley
 */

#include "DefaultGame.h"

DefaultGame::DefaultGame(const char* title): StateBasedGame(title),
	screamsSounds(),
	pukeSounds()
	{

}

void DefaultGame::initStates(GameContainer* container) {
	state_blank = new BlankState();
	state_menu = new MenuState();
	state_instructions = new InstructionsState();
	state_ingame = new InGameState();
	state_win = new WinState();
	state_fail = new FailState();

	SCORE = 0;
	LIVES = 3;

	//myfont = new BMFont("data/game/04b-09.fnt", "data/game/04b-09.png");

	screamsSounds.push_back(new Sound("data/game/sound/scream1.ogg"));
	screamsSounds.push_back(new Sound("data/game/sound/scream2.ogg"));
	screamsSounds.push_back(new Sound("data/game/sound/scream3.ogg"));
	screamsSounds.push_back(new Sound("data/game/sound/scream4.ogg"));

	pukeSounds.push_back(new Sound("data/game/sound/puke1.ogg"));
	pukeSounds.push_back(new Sound("data/game/sound/puke2.ogg"));
	pukeSounds.push_back(new Sound("data/game/sound/puke3.ogg"));
	pukeSounds.push_back(new Sound("data/game/sound/puke4.ogg"));
	pukeSounds.push_back(new Sound("data/game/sound/puke5.ogg"));

	musicHappy = new Sound("data/game/sound/soundtrack-happy.ogg");
	musicSad = new Sound("data/game/sound/soundtrack-dark.ogg");

	sheet = new Image("data/game/sheet.png");
	sheet_desc = new SpriteSheetDescription("data/game/sheet.json");

	back = sheet->getSubImage(sheet_desc->getItemByName("back.png")); //new Image("data/game/back.png");

	baseballImage1 = sheet->getSubImage(sheet_desc->getItemByName("baseball1.png")); //new Image("data/game/baseball1.png");
	baseballImage2 = sheet->getSubImage(sheet_desc->getItemByName("baseball2.png")); //new Image("data/game/baseball2.png");

	addState(state_blank);
	addState(state_menu);
	addState(state_instructions);
	addState(state_ingame);
	addState(state_win);
	addState(state_fail);

	container->getTimer()->flush();

	enterState(state_menu);
}

void DefaultGame::update(GameContainer* container, GameTimer* timer) {
	StateBasedGame::update(container, timer);

	Input* i = ARK2D::getInput();
	if (i->isKeyPressed(Input::KEY_NUMPAD_ADD)) {
		unsigned int newStateId = getCurrentStateID() + 1;
		if (newStateId == StatesList::END) {
			newStateId = 0;
		}
		enterState(newStateId);
	}
}

void DefaultGame::render(GameContainer* container, Graphics* g) {
	back->draw(0, 0);
	StateBasedGame::render(container, g);
	//ARK2D::getGraphics()->setFont(myfont);

}

void DefaultGame::resize(GameContainer* container, int width, int height) {

}

DefaultGame* DefaultGame::getGame() {
	return dynamic_cast<DefaultGame*>(ARK2D::getGame());
}

DefaultGame::~DefaultGame() {

}
