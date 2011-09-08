/*
 * MenuState.cpp
 *
 *  Created on: 08 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "MenuState.h"

#include "DefaultGame.h"

MenuState::MenuState():
	GameState() {

}

unsigned int MenuState::id() {
	return StatesList::STATE_MENU;
}

void MenuState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}
void MenuState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void MenuState::init(GameContainer* container, StateBasedGame* game) {
	bg = new Image(container->getResourcePath() + "data/game/menu.png");
}

void MenuState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	DefaultGame* m_game = DefaultGame::getGame();

	Input* i = ARK2D::getInput();
	if (i->isKeyPressed(Input::KEY_ENTER)) {
		game->enterState(m_game->state_ingame);
	}
}

void MenuState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	//g->drawString("Menu State", 10, 10);
	bg->drawCentered(int(container->getWidth()/2), int(container->getHeight()/2));

	g->setDrawColor(Color::black_50a);
	g->fillRoundedRect(150, 390, 400, 30, 5);
	g->getFont()->drawStringCenteredAt("Press Enter To Start!", int(container->getWidth()/2), int(400));

	g->setDrawColor(Color::black_50a);
	g->fillRoundedRect(150, 590, 400, 30, 5);
	g->getFont()->drawStringCenteredAt("By Ash, Jeff and Marija.", int(container->getWidth()/2), int(600));
}

void MenuState::keyPressed(unsigned int key) {

}
void MenuState::keyReleased(unsigned int key) {

}

MenuState::~MenuState() {
	delete bg;
}

