/*
 * TranslateXTransition.h
 *
 *  Created on: 1 Aug 2010
 *      Author: Ashley
 */

#ifndef TRANSLATEOUTINTRANSITION_H_
#define TRANSLATEOUTINTRANSITION_H_

#include "Transition.h"

/*!
 * \brief Slide the whole state left or right in to another. Like an Android activity transition.
 *
 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
 */
class TranslateOutInTransition : public Transition {
	public:
		TranslateOutInTransition(unsigned int dir);
		TranslateOutInTransition(unsigned int dir, float timeSeconds);
		void init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void preRender(GameContainer* container, StateBasedGame* game, Graphics* g);
		void postRender(GameContainer* container, StateBasedGame* game, Graphics* g);
		bool isComplete();
		virtual ~TranslateOutInTransition();

		static const unsigned int DIRECTION_UP = 0;
		static const unsigned int DIRECTION_DOWN = 1;
		static const unsigned int DIRECTION_LEFT = 2;
		static const unsigned int DIRECTION_RIGHT = 3;

	private:
		unsigned int m_direction;
		float m_current_time;
		float m_time;
		float m_offset_x;
};

#endif /* TRANSLATEXTRANSITION_H_ */
