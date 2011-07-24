/*
 * Ingredient.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "DefaultGame.h"
#include "Ingredient.h"
#include <math.h>

const unsigned int Ingredient::TYPE_FLOUR;
const unsigned int Ingredient::TYPE_EGG;
const unsigned int Ingredient::TYPE_BACON;
const unsigned int Ingredient::TYPE_BEANS;
const unsigned int Ingredient::TYPE_BROOMSTICK;
const unsigned int Ingredient::TYPE_CAT;
const unsigned int Ingredient::TYPE_MUSHROOM;
const unsigned int Ingredient::TYPE_PUMPKIN;
const unsigned int Ingredient::TYPE_RAT;
const unsigned int Ingredient::TYPE_SAUSAGE;
const unsigned int Ingredient::TYPE_TOAST;
const unsigned int Ingredient::TYPE_WATER;
const unsigned int Ingredient::TYPE_FISH;
const unsigned int Ingredient::TYPE_SHOE;
const unsigned int Ingredient::TYPE_CHEESE;
const unsigned int Ingredient::TYPE_LICE;
const unsigned int Ingredient::TYPE_CROSS;
const unsigned int Ingredient::TYPE_MUD;
const unsigned int Ingredient::TYPE_POTATO;
const unsigned int Ingredient::TYPE_PURPLE_SYRUP;
const unsigned int Ingredient::TYPE_BLOOD;


Ingredient::Ingredient(): GameObject() {
	m_game = DefaultGame::getGame();

	m_velocity->set(0, 0);
	m_velocityMaximum->set(0, 200);
	m_acceleration->set(0, 20);
}
void Ingredient::setType(unsigned int i) {
	m_type = i;

	m_image = m_game->state_game->ingredientImages.at(i);
	m_bounds->setWidth(m_image->getWidth());
	m_bounds->setHeight(m_image->getHeight());
}
unsigned int Ingredient::getType() {
	return m_type;
}

void Ingredient::update(GameContainer* container, GameTimer* timer) {

	m_velocity->m_y += m_acceleration->m_y;
	if (m_velocity->getY() > m_velocityMaximum->getY()) {
		m_velocity->setY(m_velocityMaximum->getY());
	}
	m_bounds->setY(m_bounds->getY() + m_velocity->m_y * timer->getDelta());

	if (GigaRectangle<float>::s_collides(m_bounds, m_game->state_game->player->getBounds())) {
		//m_game->sound_coral->play();

	}
}
void Ingredient::render(GameContainer* container, Graphics* g) {
	m_image->draw(m_bounds->getX(), m_bounds->getY());
}

Ingredient::~Ingredient() {

}

