/*
 * ViewHighscoresState.cpp
 *
 *  Created on: 11 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "ViewHighscoresState.h"

#include "DefaultGame.h"

ViewHighscoresState::ViewHighscoresState():
	GameState(), m_fromEndGame(false), m_timer(0.0f), m_switchTimer(0.0f), highscores(NULL) {

}

unsigned int ViewHighscoresState::id() {
	return 6;
}

void ViewHighscoresState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}
void ViewHighscoresState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void ViewHighscoresState::init(GameContainer* container, StateBasedGame* game) {
	highscores = new HighscoresComponent();
}

void ViewHighscoresState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	DefaultGame* m_game = DefaultGame::getGame();

	m_timer += timer->getDelta();
	if (m_timer >= 1.0f) {
		m_timer -= 1.0f;
	}

	Input* i = ARK2D::getInput();

	if (!m_fromEndGame) {
		if ((DefaultGame::ARCADE_VERSION && DefaultGame::isAnyKeyPressed())
				|| i->isKeyPressed(Input::KEY_ENTER)) {
			m_game->snd_enter->play();

			SlideRectanglesAcrossTransition* t = new SlideRectanglesAcrossTransition(
					const_cast<Color*>(&Color::darker_grey), 50, Constants::DIRECTION_LEFT, 1.0f
			);
			game->enterState(m_game->state_instructions, t, NULL);
			return;
		}
		m_switchTimer += timer->getDelta();
		if (m_switchTimer > 6.0f) {
			m_switchTimer -= 6.0f;

			SlideRectanglesAcrossTransition* t = new SlideRectanglesAcrossTransition(
					const_cast<Color*>(&Color::darker_grey), 50, Constants::DIRECTION_LEFT, 1.0f
			);
			game->enterState(m_game->state_menu, t, NULL);
		}
	} else {
		if ((DefaultGame::ARCADE_VERSION && DefaultGame::isAnyKeyPressed())
				|| i->isKeyPressed(DefaultGame::KEY_ENTER)) {
			m_game->snd_enter->play();
			m_fromEndGame = false;
			m_game->state_menu->m_timer = 0.0f;
			SlideRectanglesAcrossTransition* t = new SlideRectanglesAcrossTransition(
					const_cast<Color*>(&Color::darker_grey), 50, Constants::DIRECTION_RIGHT, 1.0f
			);
			game->enterState(m_game->state_menu, t, NULL);
		}
	}

}

void ViewHighscoresState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	DefaultGame* m_game = DefaultGame::getGame();

	m_game->state_game->bg->draw(0, 0);

	g->setDrawColor(Color::black_50a);
	g->fillRect(0, 0, container->getWidth(), container->getHeight());

	g->setDrawColor(Color::white);
	m_game->font->drawString("HIGHSCORES: ", 10, 12);

	highscores->render();

	if (m_timer <= 0.5f) {
		if (!m_fromEndGame) {
			string sss("PRESS ANY BUTTON TO START!");
			g->drawStringCenteredAt(sss, int(ORIGINAL_WIDTH/2), int(ORIGINAL_HEIGHT) - 5 - m_game->font->getStringHeight(sss));
		} else {
			string sss("PRESS ANY BUTTON TO CONTINUE...");
			g->drawStringCenteredAt(sss, int(ORIGINAL_WIDTH/2), int(ORIGINAL_HEIGHT) - 5 - m_game->font->getStringHeight(sss));
		}
	}

}

void ViewHighscoresState::keyPressed(unsigned int key) {

}
void ViewHighscoresState::keyReleased(unsigned int key) {

}

ViewHighscoresState::~ViewHighscoresState() {

}

