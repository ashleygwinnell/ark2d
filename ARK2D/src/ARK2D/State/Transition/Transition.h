/*
 * Transition.h
 *
 *  Created on: 1 Aug 2010
 *      Author: Ashley
 */

#ifndef TRANSITION_H_
#define TRANSITION_H_

#include "../../Namespaces.h"
#include "../../Common/DLL.h"

namespace ARK {
	namespace State {
		namespace Transition {

			/*!
			 * \brief The interface to implement for a GameState Transition.
			 *
			 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
			 */
			class ARK2D_API Transition {
				public:
					Transition();
					void setEasing(unsigned int easing);
					unsigned int getEasing();
					virtual void init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) = 0;
					virtual void update(GameContainer* container, StateBasedGame* game, GameTimer* delta);
					virtual void preRender(GameContainer* container, StateBasedGame* game, Renderer* g) = 0;
					virtual void postRender(GameContainer* container, StateBasedGame* game, Renderer* g) = 0;
					virtual bool isComplete() = 0;

					virtual void postEnter(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to);
					virtual void postLeave(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to);

					virtual void setUpdatingStates(bool fromState, bool toState);

					virtual ~Transition();

				protected:
					GameContainer* m_container;
					GameState* m_from;
					GameState* m_to;
					unsigned int m_easing;

					bool m_updatingFromState;
					bool m_updatingToState;
			};
		}
	}
}

#endif /* TRANSITION_H_ */

