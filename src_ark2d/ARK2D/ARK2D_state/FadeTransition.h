/*
 * FadeTransition.h
 *
 *  Created on: 22nd Nov 2010
 *      Author: Ashley
 */

#ifndef FADETRANSITION_H_
#define FADETRANSITION_H_

#include "Transition.h"
#include "../Timeline.h"

class FadeTransition : public Transition {
	public:
		FadeTransition(unsigned int timeMillis);
		void init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void preRender(GameContainer* container, StateBasedGame* game, Graphics* g);
		void postRender(GameContainer* container, StateBasedGame* game, Graphics* g);
		bool isComplete();
		~FadeTransition();

	private:
		unsigned int m_time;
		unsigned int m_time_current;

		float m_alpha;

		unsigned int m_textureId;
};

#endif /* EMPTYTRANSITION_H_ */
