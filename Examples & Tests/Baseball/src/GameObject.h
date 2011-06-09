/*
 * GameObject.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "ARK.h"

class GameObject : public ARKGameObject {
	public:
		GameObject();
		virtual void update(GameContainer* container, GameTimer* timer) = 0;
		virtual void render(GameContainer* container, Graphics* g) = 0;

		GigaRectangle<float>* getBounds();
		Vector2<float>* getVelocity();
		Vector2<float>* getAcceleration();

		virtual ~GameObject();

	protected:
		GigaRectangle<float>* m_bounds;
		Vector2<float>* m_velocity;
		Vector2<float>* m_acceleration;
		Vector2<float>* m_velocityMaximum;

};

#endif /* GAMEOBJECT_H_ */
