/*
 * MenuState.cpp
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "MenuState.h"

#include "DefaultGame.h"

MenuState::MenuState():
	GameState(), m_game(NULL), bg(NULL), m_timer(0), m_switchTimer(0.0f) {

}

unsigned int MenuState::id() {
	return 0;
}

void MenuState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}
void MenuState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void MenuState::init(GameContainer* container, StateBasedGame* game) {
	m_game = DefaultGame::getGame();

	bg = m_game->sheet->getSubImage(m_game->desc->getItemByName("title.png"))->getScaledCopy(8, 8);
}

void MenuState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	Input* i = ARK2D::getInput();

	if ((DefaultGame::ARCADE_VERSION && DefaultGame::isAnyKeyPressed())
			|| i->isKeyPressed(DefaultGame::KEY_ENTER)
			|| (i->getGamepads().size() > 0 && i->isGamepadButtonPressed(Gamepad::BUTTON_A))
		) {
		m_game->snd_enter->play();

		SlideRectanglesAcrossTransition* t = new SlideRectanglesAcrossTransition(
				const_cast<Color*>(&Color::darker_grey), 50, Constants::DIRECTION_LEFT, 1.0f
		);
		game->enterState(m_game->state_instructions, t, NULL);
	}

	m_timer += timer->getDelta();
	if (m_timer > 1.0f) {
		m_timer -= 1.0f;
	}

	m_switchTimer += timer->getDelta();
	if (m_switchTimer >= 6) {
		m_switchTimer -= 6;
		SlideRectanglesAcrossTransition* t = new SlideRectanglesAcrossTransition(
				const_cast<Color*>(&Color::darker_grey), 50, Constants::DIRECTION_LEFT, 1.0f
		);
		game->enterState(m_game->state_viewhighscores, t, NULL);
	}


}

void MenuState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	bg->draw(0, 0);

	if (m_timer <= 0.5f) {
		m_game->font->drawStringCenteredAt("PRESS ANY BUTTON TO START!", int(ORIGINAL_WIDTH/2), 320);
	}

	string byme = "BY ASHLEY GWINNELL";
	m_game->font->drawString(byme, 10, 480 - 11 - m_game->font->getStringHeight(byme));

	string bristolgamejam = "BRISTOL GAME JAM (21/05/2011)";
	m_game->font_small->drawString(bristolgamejam, 640 - 12 - m_game->font_small->getStringWidth(bristolgamejam), 16);

	if (DefaultGame::ARCADE_VERSION) {
		string arcade_edition = "HACKSPACE ARCADE EDITION";
		m_game->font_small->drawString(
				arcade_edition,
				ORIGINAL_WIDTH - 14 - m_game->font_small->getStringWidth(arcade_edition),
				ORIGINAL_HEIGHT - 14 -m_game->font_small->getStringHeight(arcade_edition)
			);
	}

}

MenuState::~MenuState() {

}

