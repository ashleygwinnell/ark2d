/*
 * Boss.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef BOSS_H_
#define BOSS_H_

#include "ARK.h"
#include "GameObject.h"
class DefaultGame;

class Boss : public GameObject {
	public:
		Boss();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Graphics* g);

		void playSound();

		virtual ~Boss();

		DefaultGame* m_game;
		Image* m_image;
		Animation* m_animation;
		float m_timer;

		float m_spitTimer;

};

#endif /* PLAYER_H_ */
