/*
 * FadeFromColourTransition.h
 *
 *  Created on: 22nd Nov 2010
 *      Author: Ashley
 */

#ifndef FADEFROMCOLOURTRANSITION_H_
#define FADEFROMCOLOURTRANSITION_H_

#include "Transition.h"
#include "../Timeline.h"

class FadeFromColourTransition : public Transition {
	public:
		FadeFromColourTransition(float time);
		void init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void preRender(GameContainer* container, StateBasedGame* game, Graphics* g);
		void postRender(GameContainer* container, StateBasedGame* game, Graphics* g);
		bool isComplete();
		virtual ~FadeFromColourTransition();

		void setColor(Color* c);


		float m_time;
		float m_time_current;

		Color* m_color;
		float m_alpha;

		unsigned int m_textureId;
};

#endif /* EMPTYTRANSITION_H_ */
