/*
 * SquaresInTransition.h
 *
 *  Created on: 13 Oct 2012
 *      Author: Ashley
 */

#ifndef ARK_STATE_TRANSITION_SQUARESINTRANSITION_H_
#define ARK_STATE_TRANSITION_SQUARESINTRANSITION_H_

#include "../../Includes.h"
#include "Transition.h"
#include "../../Tween/Timeline.h"

namespace ARK {
	namespace State {
		namespace Transition {

			/*!
			 * \brief  SHEEEEIT OPTIMISE AS THERE IS TWO LOOPS.
			 * 
			 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
			 * @todo This transition does not work.
			 */
			class SquaresInTransition : public Transition {
				public:
					SquaresInTransition(float duration, float squareSize);
					SquaresInTransition(float duration, float squareSize, Color* c);
					void init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to);
					void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
					void preRender(GameContainer* container, StateBasedGame* game, Renderer* g);
					void postRender(GameContainer* container, StateBasedGame* game, Renderer* g);
					bool isComplete();
					virtual ~SquaresInTransition();

					float m_timer; 
					float m_duration;
					float m_squareSize;
					vector<bool> m_squares;
					vector<int> m_squaresFilled;
					int m_squaresEmptied;
					int m_squaresTotal;
					Color* m_color;

					int m_squaresX;
					int m_squaresY;

					// TODO: optimise.
					// 		 - maintain a list of indices that are not filled and use that data to select a new index to fill.

			};
		}
	} 
}

#endif /* ARK_STATE_TRANSITION_SQUARESOUTTRANSITION_H_ */
