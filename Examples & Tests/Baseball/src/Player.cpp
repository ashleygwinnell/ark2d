/*
 * Player.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "DefaultGame.h"
#include "Player.h"
#include <math.h>

Player::Player(): GameObject()  {
	m_game = DefaultGame::getGame();

	m_centerPoint = new Vector2<int>(0, 0);

	m_image = m_game->sheet->getSubImage(m_game->sheet_desc->getItemByName("player.png")); //new Image("data/game/player.png");

	//m_animation = new Animation();
	//m_animation->setFrameTime(500);
	//m_animation->addImage(m_game->sheet->getSubImage(m_game->desc->getItemByName("couldron1.png"))->getScaledCopy(4, 4));
	//m_animation->addImage(m_game->sheet->getSubImage(m_game->desc->getItemByName("couldron2.png"))->getScaledCopy(4, 4));

	m_degrees = 270;

	m_bounds->setWidth(10);
	m_bounds->setHeight(10);
	//m_bounds->setWidth(m_animation->getCurrentFrame()->getWidth());
	//m_bounds->setHeight(m_animation->getCurrentFrame()->getHeight());

	GameContainer* container = ARK2D::getContainer();
	m_bounds->setLocationByCenter(float(container->getWidth()/2), float(container->getHeight()/2));

	m_velocity->set(0, 0);
	m_velocityMaximum->set(2000, 25);

	m_acceleration->set(400, 5);
}

void Player::update(GameContainer* container, GameTimer* timer) {

	Input i = container->getInput();
	if (i.isKeyDown(Input::KEY_LEFT)) {
		m_degrees -= 270 * timer->getDelta();
	}
	if (i.isKeyDown(Input::KEY_RIGHT)) {
		m_degrees += 270 * timer->getDelta();
	}

	m_centerPoint->set(m_game->getContainer()->getWidth()/2, m_game->getContainer()->getHeight()/2);
	MathUtil::moveAngle(m_centerPoint, m_degrees, 300);

	// update location
	m_bounds->setLocationByCenter(m_centerPoint->m_x, m_centerPoint->m_y);
	//m_bounds->setX(m_bounds->getX() + round(m_velocity->m_x * timer->getDelta()));
	//m_bounds->setY(m_bounds->getY() + m_velocity->m_y * timer->getDelta());

	DefaultGame* m_game = DefaultGame::getGame();
	vector<Baseball*> data = m_game->state_ingame->baseballs.getData();
	if(data.size() > 0) {
		vector<Baseball*>::iterator it = data.begin();
		while(it != data.end()) {
			Baseball* b = (*it);
			if (b == NULL) {
				break;
			}
			bool collides = Circle::s_collides(m_centerPoint->m_x, m_centerPoint->m_y, 80,
					b->getBounds()->getCenterX(), b->getBounds()->getCenterY(), 40);

			if (collides) {
				delete b;
				it = data.erase(it);
				m_game->SCORE += 10;
				//m_game->screamsSounds.
				int screamindex = MathUtil::randBetween(0, 4);
				m_game->screamsSounds.at(screamindex)->play();
			} else {
				it++;
			}
		}
		m_game->state_ingame->baseballs.setData(data);
	}

	//m_animation->update(timer);
}
void Player::render(GameContainer* container, Graphics* g) {
	//m_animation->draw(int(m_bounds->getX()), int(m_bounds->getY()));
	//g->fillCircle(int(m_bounds->getX()), int(m_bounds->getY()), 20, 30);

	//g->pushMatrix();
	//g->translate(int(m_bounds->getCenterX()), int(m_bounds->getCenterY()));
	//g->fillCircle(0, 0, 20, 30);
	//g->popMatrix();


	m_image->drawCentered(int(m_bounds->getCenterX()), int(m_bounds->getCenterY()));
}

Player::~Player() {
	delete m_centerPoint;
}
