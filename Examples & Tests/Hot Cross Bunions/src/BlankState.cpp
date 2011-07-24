/*
 * BlankState.cpp
 *
 *  Created on: 02 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "BlankState.h"

#include "DefaultGame.h"

BlankState::BlankState():
	GameState() {

}

unsigned int BlankState::id() {
	return 3;
}

void BlankState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}
void BlankState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void BlankState::init(GameContainer* container, StateBasedGame* game) {

}

void BlankState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	DefaultGame* g = DefaultGame::getGame();

	SlideRectanglesAcrossTransition* t = new SlideRectanglesAcrossTransition(
		const_cast<Color*>(&Color::darker_grey), 50, Constants::DIRECTION_LEFT, 1.0f
	);

	g->enterState(g->state_menu, t, NULL);

}

void BlankState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	g->setDrawColor(Color::darker_grey);
	g->fillRect(0, 0, container->getWidth(), container->getHeight());

	g->setDrawColor(Color::white);
}

void BlankState::keyPressed(unsigned int key) {

}
void BlankState::keyReleased(unsigned int key) {

}

BlankState::~BlankState() {

}

