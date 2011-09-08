/*
 * InGameState.cpp
 *
 *  Created on: 08 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "InGameState.h"

#include "DefaultGame.h"

InGameState::InGameState():
	GameState(),
	player(NULL),
	boss(NULL),
	baseballs(),
	m_timer(0),
	m_timerLevelOne(0.0f),
	m_timerLevelTwo(15.0f),
	m_timerLevelThree(30.0f),
	m_timerLevelFour(45.0f),
	m_timerLevelFive(60.0f),
	m_timerLevelSix(90.0f)

{

}

unsigned int InGameState::id() {
	return StatesList::STATE_INGAME;
}

void InGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

	DefaultGame* g = DefaultGame::getGame();
	g->LIVES = 3;
	g->SCORE = 0;
	g->state_ingame->baseballs.removeAll();
	g->state_ingame->m_timer = 0.0f;
	g->state_ingame->boss->m_timer = 0.0f;
	g->state_ingame->boss->m_spitTimer = 0.0f;

}
void InGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {
	DefaultGame* m_game = DefaultGame::getGame();
	m_game->musicSad->stop();
}

void InGameState::init(GameContainer* container, StateBasedGame* game) {
	player = new Player();
	boss = new Boss();
}

void InGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	m_timer += timer->getDelta();

	DefaultGame* m_game = DefaultGame::getGame();
	if (!m_game->musicSad->isPlaying()) {
		m_game->musicSad->play();
	}

	player->update(container, timer);
	baseballs.updateAll(container, timer);
	boss->update(container, timer);

	vector<Baseball*> data = baseballs.getData();
	if(data.size() > 0) {
		vector<Baseball*>::iterator it = data.begin();
		while(it != data.end()) {
			Baseball* b = (*it);
			if (b == NULL) {
				break;
			}

			if (b->m_dead) {
				delete b;

				it = data.erase(it);
				m_game->LIVES--;
			} else {
				it++;
			}
		}
		baseballs.setData(data);
	}

	if (m_game->LIVES < 0) {
		m_game->enterState(m_game->state_fail);
	}

}

void InGameState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	DefaultGame* m_game = DefaultGame::getGame();

	//g->drawString("InGame State", 10, 10);
	player->render(container, g);
	baseballs.renderAll(container, g);
	boss->render(container, g);

	g->setDrawColor(Color::black_50a);
	g->fillRoundedRect(5,5,container->getWidth()-10, 35, 5);

	string scoreStr = "Score: ";
	scoreStr = StringUtil::append(scoreStr, m_game->SCORE);
	g->drawString(scoreStr, 12, 8);

	string winStr = "Win in: ";
	winStr = StringUtil::append(winStr, int(m_game->state_ingame->m_timerLevelSix - m_game->state_ingame->m_timer));
	g->drawString(winStr, container->getWidth() - 12 - g->getFont()->getStringWidth(winStr), 8);

	string livesStr = "Lives: ";
	livesStr = StringUtil::append(livesStr, m_game->LIVES);
	g->getFont()->drawStringCenteredAt(livesStr, int(container->getWidth()/2), 19);
}

void InGameState::keyPressed(unsigned int key) {

}
void InGameState::keyReleased(unsigned int key) {

}

InGameState::~InGameState() {
	delete player;
	delete boss;
}

