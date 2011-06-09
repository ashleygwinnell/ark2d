/*
 * Boss.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef BASEBALL_H_
#define BASEBALL_H_

#include "ARK.h"
#include "GameObject.h"
class DefaultGame;

class Baseball : public GameObject {
	public:
		Baseball();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Graphics* g);
		virtual ~Baseball();

		DefaultGame* m_game;
		Animation* m_animation;

		bool m_dead;

		int m_width;
		float m_degrees;
		float m_speed_pps;

};

#endif /* PLAYER_H_ */
