/*
 * HighscoresComponent.cpp
 *
 *  Created on: 11 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "DefaultGame.h"
#include "HighscoresComponent.h"

HighscoresComponent::HighscoresComponent(): y(50) {

}

void HighscoresComponent::update(float t) {

}
void HighscoresComponent::render() {
	int localy = y;
	//GameContainer* container = ARK2D::getContainer();
	DefaultGame* m_game = DefaultGame::getGame();
	Graphics* g = ARK2D::getGraphics();

	vector<LocalHighscoreItem*> items = m_game->local_highscores->data();
	for(unsigned int i = 0; i < items.size() && i < 14; i++) {
		g->setFont(m_game->font);
		g->drawString(items.at(i)->name, (ORIGINAL_WIDTH/2) - 10 - m_game->font->getStringWidth(items.at(i)->name), localy);
		string ss = Cast::toString<int>(items.at(i)->score);
		g->drawString(ss, (ORIGINAL_WIDTH/2) + 10, localy);
		localy += 26;
	}
}

HighscoresComponent::~HighscoresComponent() {

}
