/*
 * FallingPoint.cpp
 *
 *  Created on: 22 May 2011
 *      Author: Ashley
 */

#include "DefaultGame.h"
#include "FallingPoint.h"

FallingPoint::FallingPoint(): ARKGameObject(), items(), index(0), totalWeight(0), m_timer(0), m_time(0) {

}

void FallingPoint::setIndex(unsigned int i) {
	index = i;
}

void FallingPoint::setTime(float i) {
	m_time = i;
}
void FallingPoint::setTimer(float f) {
	m_timer = f;
}

void FallingPoint::add(FallingPointChance* c) {
	totalWeight += c->weight;
	items.push_back(c);
}
void FallingPoint::add(unsigned int type, unsigned int weight) {
	FallingPointChance* p = new FallingPointChance();
	p->type = type;
	p->weight = weight;
	add(p);
}

void FallingPoint::update(GameContainer* container, GameTimer* timer) {
	m_timer += timer->getDelta();

	//std::cout << m_timer << " " << time << std::endl;
	if (m_timer >= m_time) {
		unsigned int r = MathUtil::randBetween(0, totalWeight+1);
		unsigned int count = 0;
		for(unsigned int i = 0; i < items.size(); i++) {
			if (r >= count && r <= count + items.at(i)->weight) {
				DefaultGame* game = DefaultGame::getGame();

				Ingredient* ing = new Ingredient(); // int right = 540;
				ing->setType(items.at(i)->type);
				ing->getBounds()->setLocationByCenter(130 + (index+1) * (510/(game->state_game->recipes.get(game->levelNumber)->fallingPoints.size()+1)), 0);
				game->state_game->ingredients.add(ing);

				break;
			}
			count += items.at(i)->weight;
		}
		m_timer -= m_time;
	}
}
void FallingPoint::render(GameContainer* container, Graphics* g) {

}

FallingPoint::~FallingPoint() {

}
