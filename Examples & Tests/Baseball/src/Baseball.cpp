/*
 * Boss.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "DefaultGame.h"
#include "Baseball.h"
#include <math.h>

Baseball::Baseball(): GameObject(), m_dead(false),
	m_width(20),
	m_degrees(0.0f),
	m_speed_pps(100)

{
	m_game = DefaultGame::getGame();

	m_animation = new Animation();
	m_animation->setFrameTime(100);
	m_animation->addImage(m_game->baseballImage1);
	m_animation->addImage(m_game->baseballImage2);
	//m_animation->addImage(m_game->sheet->getSubImage(m_game->desc->getItemByName("couldron1.png"))->getScaledCopy(4, 4));
	//m_animation->addImage(m_game->sheet->getSubImage(m_game->desc->getItemByName("couldron2.png"))->getScaledCopy(4, 4));

	m_bounds->setWidth(30);
	m_bounds->setHeight(30);
	//m_bounds->setWidth(m_animation->getCurrentFrame()->getWidth());
	//m_bounds->setHeight(m_animation->getCurrentFrame()->getHeight());

	GameContainer* container = ARK2D::getContainer();
	m_bounds->setLocationByCenter(float(container->getWidth()/2), float(container->getHeight()/2));

	m_velocity->set(0, 0);
	m_velocityMaximum->set(2000, 25);

	m_acceleration->set(400, 5);
}

void Baseball::update(GameContainer* container, GameTimer* timer) {

	m_animation->update(timer);

	Vector2<int>* dist = MathUtil::moveAngleFromOrigin<int>(m_degrees, m_speed_pps * timer->getDelta());
	m_bounds->setLocationByCenter(m_bounds->getCenterX() + dist->m_x, m_bounds->getCenterY() + dist->m_y);
	//m_animation->update(timer);

	/*DefaultGame* m_game = DefaultGame::getGame();
	if (m_game->SCORE > 0 && m_game->SCORE % 50 == 0) {
		// lean towards player.
		float angle = MathUtil::angle(
				int(m_bounds->getCenterX()),
				int(m_bounds->getCenterY()),
				int(m_game->state_ingame->player->getBounds()->getCenterX()),
				int(m_game->state_ingame->player->getBounds()->getCenterY())
			);
		if (angle == m_degrees) {

		} else if (angle < m_degrees) {
			m_degrees -= 1;
		} else {
			m_degrees += 1;
		}
	}*/

	//if (GigaRectangle::s_isOffscreen<float>(m_bounds)) {
	if (m_bounds->isOffscreen()) {
		m_dead = true;
	}
}
void Baseball::render(GameContainer* container, Graphics* g) {
	m_animation->draw(int(m_bounds->getCenterX()), int(m_bounds->getCenterY()));

	//g->pushMatrix();
	//g->translate(int(m_bounds->getCenterX()), int(m_bounds->getCenterY()));
	//g->fillCircle(0, 0, int(m_width/2), 40);
	//g->popMatrix();
}

Baseball::~Baseball() {

}
