/*
 * Boss.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "DefaultGame.h"
#include "Boss.h"
#include "Baseball.h"
#include <math.h>

Boss::Boss(): GameObject()  {
	m_game = DefaultGame::getGame();

	m_spitTimer = 0;

	m_image = new Image("data/game/boss1.png");

	m_animation = new Animation();
	m_animation->setFrameTime(500);
	m_animation->addImage(new Image("data/game/boss2.png"));
	m_animation->addImage(new Image("data/game/boss3.png"));

	//m_animation->addImage(m_game->sheet->getSubImage(m_game->desc->getItemByName("couldron1.png"))->getScaledCopy(4, 4));
	//m_animation->addImage(m_game->sheet->getSubImage(m_game->desc->getItemByName("couldron2.png"))->getScaledCopy(4, 4));

	m_timer = 0;

	m_bounds->setWidth(100);
	m_bounds->setHeight(100);
	//m_bounds->setWidth(m_animation->getCurrentFrame()->getWidth());
	//m_bounds->setHeight(m_animation->getCurrentFrame()->getHeight());

	GameContainer* container = ARK2D::getContainer();
	m_bounds->setLocationByCenter(float(container->getWidth()/2), float(container->getHeight()/2));

	m_velocity->set(0, 0);
	m_velocityMaximum->set(2000, 25);

	m_acceleration->set(400, 5);
}

void Boss::update(GameContainer* container, GameTimer* timer) {

	m_timer += timer->getDelta();

	m_spitTimer -= timer->getDelta();

	if (m_spitTimer > 0) {
		m_animation->update(timer);
	}

	if (m_game->state_ingame->m_timer >= m_game->state_ingame->m_timerLevelSix) {
		m_game->enterState(m_game->state_win);
	} else if (m_game->state_ingame->m_timer >= m_game->state_ingame->m_timerLevelFive) {

		if (m_timer > 3.0f) {

			for(int i = 0; i < 10; i++) {
				Baseball* b = new Baseball();
				GigaRectangle<float>* bb = b->getBounds();

				b->m_degrees = MathUtil::randBetween(0, 360) + (i * 36);

				Vector2<int>* v = MathUtil::moveAngleFromOrigin<int>(b->m_degrees, m_bounds->getWidth()/4);
				bb->setLocationByCenter(bb->getCenterX() + v->m_x, bb->getCenterY() + v->m_y);
				m_game->state_ingame->baseballs.add(b);
			}

			m_timer -= 3.0f;
			playSound();
		}

	} else if (m_game->state_ingame->m_timer >= m_game->state_ingame->m_timerLevelFour) {
		if (m_timer > 2.0f) {
			Baseball* b = new Baseball();
			GigaRectangle<float>* bb = b->getBounds();

			b->m_degrees = MathUtil::randBetween(0, 360);

			Vector2<int>* v = MathUtil::moveAngleFromOrigin<int>(b->m_degrees, m_bounds->getWidth()/4);
			bb->setLocationByCenter(bb->getCenterX() + v->m_x, bb->getCenterY() + v->m_y);
			m_game->state_ingame->baseballs.add(b);


					Baseball* b2 = new Baseball();
					GigaRectangle<float>* bb2 = b2->getBounds();

					b2->m_degrees = b->m_degrees + 120;

					Vector2<int>* v2 = MathUtil::moveAngleFromOrigin<int>(b2->m_degrees, m_bounds->getWidth()/4);
					bb2->setLocationByCenter(bb2->getCenterX() + v2->m_x, bb2->getCenterY() + v2->m_y);
					m_game->state_ingame->baseballs.add(b2);


							Baseball* b3 = new Baseball();
							GigaRectangle<float>* bb3 = b3->getBounds();

							b3->m_degrees = b->m_degrees + 240;

							Vector2<int>* v3 = MathUtil::moveAngleFromOrigin<int>(b3->m_degrees, m_bounds->getWidth()/4);
							bb3->setLocationByCenter(bb3->getCenterX() + v3->m_x, bb3->getCenterY() + v3->m_y);
							m_game->state_ingame->baseballs.add(b3);

			m_timer -= 2.0f;
			playSound();
		}
	} else if (m_game->state_ingame->m_timer >= m_game->state_ingame->m_timerLevelThree) {
		if (m_timer > 1.5f) {
			Baseball* b = new Baseball();
			GigaRectangle<float>* bb = b->getBounds();

			b->m_degrees = MathUtil::randBetween(0, 360);

			Vector2<int>* v = MathUtil::moveAngleFromOrigin<int>(b->m_degrees, m_bounds->getWidth()/4);
			bb->setLocationByCenter(bb->getCenterX() + v->m_x, bb->getCenterY() + v->m_y);
			m_game->state_ingame->baseballs.add(b);

			m_timer -= 1.5f;
			playSound();
		}
	} else if (m_game->state_ingame->m_timer >= m_game->state_ingame->m_timerLevelTwo) {
		if (m_timer > 1.5f) {
			Baseball* b = new Baseball();
			GigaRectangle<float>* bb = b->getBounds();

			b->m_degrees = MathUtil::randBetween(0, 360);

			Vector2<int>* v = MathUtil::moveAngleFromOrigin<int>(b->m_degrees, m_bounds->getWidth()/4);
			bb->setLocationByCenter(bb->getCenterX() + v->m_x, bb->getCenterY() + v->m_y);
			m_game->state_ingame->baseballs.add(b);

			m_timer -= 1.5f;
			playSound();
		}
	} else if (m_game->state_ingame->m_timer >= m_game->state_ingame->m_timerLevelOne) {
		if (m_timer > 2.0f) {
			Baseball* b = new Baseball();
			GigaRectangle<float>* bb = b->getBounds();

			b->m_degrees = MathUtil::randBetween(0, 360);

			Vector2<int>* v = MathUtil::moveAngleFromOrigin<int>(b->m_degrees, m_bounds->getWidth()/4);
			bb->setLocationByCenter(bb->getCenterX() + v->m_x, bb->getCenterY() + v->m_y);
			m_game->state_ingame->baseballs.add(b);

			m_timer -= 2.0f;

			playSound();
		}
	}

	//m_animation->update(timer);
}

void Boss::playSound() {
	int index = MathUtil::randBetween(0, m_game->pukeSounds.size());
	m_game->pukeSounds.at(index)->play();

	m_spitTimer = 1.0f;
}

void Boss::render(GameContainer* container, Graphics* g) {
	//m_animation->draw(int(m_bounds->getX()), int(m_bounds->getY()));
	//g->pushMatrix();
	//g->translate(int(container->getWidth()/2), int(container->getHeight()/2));
	//g->fillCircle(0, 0, 100, 40);
	//g->popMatrix();

	DefaultGame* m_game = DefaultGame::getGame();

	if (m_spitTimer <= 0) {
		m_image->setCenterOfRotation(m_image->getWidth()/2, m_image->getHeight()/2);
		m_image->setRotation(MathUtil::angle(350, 350,
				m_game->state_ingame->player->getBounds()->getCenterX(),
				m_game->state_ingame->player->getBounds()->getCenterY()));
		m_image->drawCentered(int(container->getWidth()/2), int(container->getHeight()/2));
	} else {
		m_animation->getCurrentFrame()->drawCentered(int(container->getWidth()/2), int(container->getHeight()/2));
	}
}

Boss::~Boss() {

}
