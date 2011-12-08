/*
 * FadeTransition.h
 *
 *  Created on: 22nd Nov 2010
 *      Author: Ashley
 */

#ifndef FADETOCOLOURTRANSITION_H_
#define FADETOCOLOURTRANSITION_H_

#include "Transition.h"
#include "../../Timeline.h"

class FadeToColourTransition : public Transition {
	public:
		FadeToColourTransition(float time);
		void init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void preRender(GameContainer* container, StateBasedGame* game, Graphics* g);
		void postRender(GameContainer* container, StateBasedGame* game, Graphics* g);
		bool isComplete();
		virtual ~FadeToColourTransition();

		void setColor(Color* c);


		float m_time;
		float m_time_current;

		Color* m_color;
		float m_alpha;

		unsigned int m_textureId;
};

#endif /* EMPTYTRANSITION_H_ */
