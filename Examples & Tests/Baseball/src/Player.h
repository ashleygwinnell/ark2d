/*
 * Player.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "ARK.h"
#include "GameObject.h"
class DefaultGame;

class Player : public GameObject {
	public:
		Player();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Graphics* g);
		virtual ~Player();

		Vector2<int>* m_centerPoint;
		DefaultGame* m_game;
		Image* m_image;

		float m_degrees;

};

#endif /* PLAYER_H_ */
