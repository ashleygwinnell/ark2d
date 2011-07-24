/*
 * BlankState.cpp
 *
 *  Created on: 05 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "InstructionsState.h"

#include "DefaultGame.h"

InstructionsState::InstructionsState():
	GameState(), m_timer(0.0f) {

}

unsigned int InstructionsState::id() {
	return 5;
}

void InstructionsState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}
void InstructionsState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void InstructionsState::init(GameContainer* container, StateBasedGame* game) {

}

void InstructionsState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	DefaultGame* m_game = DefaultGame::getGame();


	m_timer += timer->getDelta();
	if (m_timer > 1.0f) {
		m_timer -= 1.0f;
	}


	Input* i = ARK2D::getInput();
	if ((DefaultGame::ARCADE_VERSION && DefaultGame::isAnyKeyPressed())
			|| (i->isKeyPressed(DefaultGame::KEY_ENTER)
			|| (i->getGamepads().size() > 0) && i->isGamepadButtonPressed(Gamepad::BUTTON_A))) {
		m_game->reset();
		SlideRectanglesAcrossTransition* t = new SlideRectanglesAcrossTransition(
				const_cast<Color*>(&Color::darker_grey), 50, Constants::DIRECTION_LEFT, 1.0f
		);
		game->enterState(m_game->state_game, t, NULL);
	}
	if (i->isKeyPressed(DefaultGame::KEY_BACK)
			|| (i->getGamepads().size() > 0) && i->isGamepadButtonPressed(Gamepad::BUTTON_BACK)) {
		m_game->snd_enter->play();

		SlideRectanglesAcrossTransition* t = new SlideRectanglesAcrossTransition(
				const_cast<Color*>(&Color::darker_grey), 50, Constants::DIRECTION_LEFT, 1.0f
		);
		game->enterState(m_game->state_menu, t, NULL);
	}



}

void InstructionsState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	DefaultGame* m_game = DefaultGame::getGame();

	m_game->state_game->bg->draw(0, 0);

	g->setDrawColor(Color::black_50a);
	g->fillRect(0, 0, container->getWidth(), container->getHeight());

	g->setDrawColor(Color::white);
	m_game->font->drawString("INSTRUCTIONS: ", 10, 12);

	int centerX = int(ORIGINAL_WIDTH/2);
	m_game->font->drawString("TO MOVE LEFT AND RIGHT: ", 10, 90);
	if (DefaultGame::ARCADE_VERSION) {
		m_game->font_small->drawStringCenteredAt("ARCADE STICK", centerX, 150);
	} else {
		m_game->font_small->drawStringCenteredAt("ARROW KEYS", centerX, 150);
	}
	//m_game->font_small->drawStringCenteredAt("ARROW KEYS", centerX, 120);

	//m_game->font_small->drawStringCenteredAt("CONTROLLER / GAMEPAD", centerX, 170);

	m_game->font->drawString("TO FULFILL A RECIPE: ", 10, 230);

	m_game->font_small->drawStringCenteredAt("COLLECT ITEMS THAT FALL FROM THE CEILING WITH YOUR CAULDRON", centerX, 270);
	m_game->font_small->drawStringCenteredAt("RECIPES ARE SHOWN ON THE LEFT OF THE SCREEN", centerX, 290);

	m_game->font_small->drawStringCenteredAt("A PERFECT RECIPE GIVES BONUS SCORE", centerX, 320);
	m_game->font_small->drawStringCenteredAt("DON'T CONTAMINATE THE DISH OR IT'S GAME OVER!", centerX, 340);

	if (m_timer <= 0.5f) {
		m_game->font->drawStringCenteredAt("PRESS ANY BUTTON TO START!", centerX, 400);
	}

}

void InstructionsState::keyPressed(unsigned int key) {

}
void InstructionsState::keyReleased(unsigned int key) {

}

InstructionsState::~InstructionsState() {

}

