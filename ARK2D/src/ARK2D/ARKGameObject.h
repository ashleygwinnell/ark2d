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
	protected:
		bool m_pendingRemoval;

	public:
		ARKGameObject();
		virtual bool isPendingRemoval();
		virtual void setPendingRemoval(bool b);
		virtual void update(GameContainer* container, GameTimer* timer) = 0;
		virtual void render(GameContainer* container, Graphics* g) = 0;

		virtual ~ARKGameObject();
};

#endif /* ARKGAMEOBJECT_H_ */
