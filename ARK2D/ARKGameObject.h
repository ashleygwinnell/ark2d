/*
 * ARKGameObject.h
 *
 *  Created on: 22 May 2011
 *      Author: Ashley
 */

#ifndef ARKGAMEOBJECT_H_
#define ARKGAMEOBJECT_H_

class GameContainer;
class GameTimer;
class Graphics;

class ARKGameObject {
	public:
		ARKGameObject();
		virtual void update(GameContainer* container, GameTimer* timer) = 0;
		virtual void render(GameContainer* container, Graphics* g) = 0;

		virtual ~ARKGameObject();
};

#endif /* ARKGAMEOBJECT_H_ */
