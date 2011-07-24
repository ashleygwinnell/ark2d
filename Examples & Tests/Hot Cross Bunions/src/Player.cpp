/*
 * Player.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "DefaultGame.h"
#include "Player.h"
#include "FloatingFadingText.h"
#include <math.h>

Player::Player(): GameObject(), m_facingLeft(false)  {
	m_game = DefaultGame::getGame();

	m_animation = new Animation();
	m_animation->setFrameTime(500);
	m_animation->addImage(m_game->sheet->getSubImage(m_game->desc->getItemByName("couldron1.png"))->getScaledCopy(4, 4));
	m_animation->addImage(m_game->sheet->getSubImage(m_game->desc->getItemByName("couldron2.png"))->getScaledCopy(4, 4));

	m_bounds->setWidth(m_animation->getCurrentFrame()->getWidth());
	m_bounds->setHeight(m_animation->getCurrentFrame()->getHeight());
	//GameContainer* container = ARK2D::getContainer();
	m_bounds->setLocationByCenter(float(ORIGINAL_WIDTH/2), float(ORIGINAL_HEIGHT - 75 - (m_animation->getCurrentFrame()->getHeight()/2)));

	m_velocity->set(0, 0);
	m_velocityMaximum->set(2000, 25);

	m_acceleration->set(400, 5);
}

void Player::update(GameContainer* container, GameTimer* timer) {

	Input i = container->getInput();

	// changes to x-axis velocities
	//if (i.isKeyDown(Input::KEY_D) || i.isKeyDown(Input::KEY_RIGHT)
	if (i.isKeyDown(DefaultGame::KEY_RIGHT)
			|| (i.getGamepads().size() > 0 && i.isGamepadButtonDown(Gamepad::DPAD_RIGHT))) {
		m_velocity->m_x = m_acceleration->m_x; // timer->getDelta();
		m_facingLeft = false;
	//} else if (i.isKeyDown(Input::KEY_A) || i.isKeyDown(Input::KEY_LEFT)
	} else if (i.isKeyDown(DefaultGame::KEY_LEFT)
			|| (i.getGamepads().size() > 0 && i.isGamepadButtonDown(Gamepad::DPAD_LEFT))) {
		m_velocity->m_x = m_acceleration->m_x * -1; // * timer->getDelta();
		m_facingLeft = true;
	} else {
		m_velocity->frictionX(m_acceleration->m_x);
	}

	if (m_velocity->getX() > m_velocityMaximum->getX()) {
		m_velocity->setX(m_velocityMaximum->getX());
	} else if (m_velocity->getX() < m_velocityMaximum->getX() * -1) {
		m_velocity->setX(m_velocityMaximum->getX() * -1);
	}

	// changes to y-axis velocities (jump)
	//if (i.isKeyPressed(Input::KEY_W) || i.isKeyPressed(Input::KEY_SPACE) || i.isKeyPressed(Input::KEY_UP)) {
		//m_game->sound_jump->play();
	//	m_velocity->setY(-100);
	//}

	m_velocity->m_y += m_acceleration->m_y;
	if (m_velocity->getY() > m_velocityMaximum->getY()) {
		m_velocity->setY(m_velocityMaximum->getY());
	}

	//if (i.isKeyDown(Input::KEY_S) || i.isKeyDown(Input::KEY_DOWN)) {
	//	m_velocity->setY(m_velocityMaximum->getY() * 2.0f);
	//}

	// update location
	m_bounds->setX(m_bounds->getX() + round(m_velocity->m_x * timer->getDelta()));
	m_bounds->setY(m_bounds->getY() + m_velocity->m_y * timer->getDelta());

	// check bounds and don't wrap.
	if (m_bounds->getX() < 40) {
		m_bounds->setX(40);
	} else if (m_bounds->getMaxX() > ORIGINAL_WIDTH - 40) {
		m_bounds->setX( ORIGINAL_WIDTH - m_bounds->getWidth() - 40);
	}
	if (m_bounds->getY() < 0) {
		m_bounds->setY(0);
	}
	if (m_bounds->getMaxY() > ORIGINAL_HEIGHT - 80) {
		m_bounds->setY(ORIGINAL_HEIGHT - m_bounds->getHeight() - 80);
	}

	// check bounds and wrap around.
	/*if (m_bounds->getCenterX() > (signed int) DefaultGame::ORIGINAL_WIDTH) {
		m_bounds->setX(0 - (m_bounds->getWidth()/2));
	} else if (m_bounds->getCenterX() < 0) {
		m_bounds->setX(DefaultGame::ORIGINAL_WIDTH - (m_bounds->getWidth()/2));
	}

	if (m_bounds->getCenterY() > (signed int) DefaultGame::ORIGINAL_HEIGHT) {
		m_bounds->setY(0 - (m_bounds->getHeight()/2));
	} else if (m_bounds->getCenterY() < 0) {
		m_bounds->setY(DefaultGame::ORIGINAL_HEIGHT - (m_bounds->getHeight()/2));
	}*/

	//if (m_bounds->getCenterX() > (signed int) DefaultGame::ORIGINAL_WIDTH) {
	//	DefaultGame::s_defaultGame->m_renderMapX = 1;
	//} else if (m_bounds->getCenterX() < 0) {
	//	DefaultGame::s_defaultGame->m_renderMapX = 0;
	//}

	//int r = int(m_bounds->getCenterX()) % DefaultGame::ORIGINAL_WIDTH;
	//float screenx = (m_bounds->getCenterX() - r) / DefaultGame::ORIGINAL_WIDTH;

	//DefaultGame::s_defaultGame->m_renderMapX = (unsigned int) screenx;


	vector<Ingredient*> ings = m_game->state_game->ingredients.getData();
	vector<Ingredient*>::iterator it = ings.begin();
	while(it != ings.end()) {
		Ingredient* in = (*it);
		if (GigaRectangle<float>::s_collides(m_bounds, in->getBounds())) {
			map<unsigned int, unsigned int>& ings2 = m_game->getCurrentRecipe()->ingredients;
			if (ings2.find(in->getType()) != ings2.end() && ings2[in->getType()] > 0) {
				//std::cout << "good" << std::endl;
				ings2[in->getType()] -= 1;

				m_game->contamination -= 10;
				if (m_game->contamination < 0) {
					m_game->multiplier += 1;
					m_game->contamination = 0;
				}

				m_game->snd_good->play();

				int addScore = 10 * m_game->multiplier;
				m_game->score += addScore;

				string addScoreString("+");
				addScoreString = StringUtil::append(addScoreString, addScore);

				FloatingFadingText* f = new FloatingFadingText(addScoreString);
				//f->m_color = const_cast<Color*>(&Color::white);
				f->getBounds()->setLocationByCenter(in->getBounds()->getCenterX(), in->getBounds()->getCenterY());
				m_game->state_game->fadingText.add(f);

			} else {
				//std::cout << "bad" << std::endl;
				m_game->score -= 10;
				m_game->contamination += 10;
				m_game->multiplier = 1;

				m_game->snd_bad->play();


				FloatingFadingText* f = new FloatingFadingText("-10");
				//f->m_color = const_cast<Color*>(&Color::red);
				f->getAcceleration()->setY(f->getAcceleration()->getY() * -1);
				f->getBounds()->setLocationByCenter(in->getBounds()->getCenterX(), in->getBounds()->getCenterY());
				m_game->state_game->fadingText.add(f);
			}

			it = ings.erase(it);
			continue;
		} else if (in->getBounds()->isOffscreen()) {
			it = ings.erase(it);
			continue;
		}
		++it;
	}
	m_game->state_game->ingredients.setData(ings);

	m_animation->update(timer);
}
void Player::render(GameContainer* container, Graphics* g) {
	//m_image->draw(m_bounds->getX() - m_game->state_game->offsetX, m_bounds->getY());
	//m_image->draw(m_bounds->getX(), m_bounds->getY());
	//DefaultGame::getGame()->particles->render(int(m_bounds->getX()), int(m_bounds->getY()));
	m_animation->draw(int(m_bounds->getX()), int(m_bounds->getY()));

}

Player::~Player() {

}
