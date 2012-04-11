/*
 * FadeTransition.h
 *
 *  Created on: 22nd Nov 2010
 *      Author: Ashley
 */

#ifndef FADETRANSITION_H_
#define FADETRANSITION_H_

#include "Transition.h"
#include "../../Tween/Timeline.h"

namespace ARK {
	namespace State {
		namespace Transition {

			/*!
			 * \brief Fade directly from one GameState in to another.
			 *
			 * @warning This generates a new OpenGL Texture each time.
			 * @todo Optimise this to use the same texture each time.
			 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
			 */
			class FadeTransition : public Transition {
				public:
					FadeTransition(unsigned int timeMillis);
					void init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to);
					void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
					void preRender(GameContainer* container, StateBasedGame* game, Renderer* g);
					void postRender(GameContainer* container, StateBasedGame* game, Renderer* g);
					bool isComplete();
					virtual ~FadeTransition();

				private:
					unsigned int m_time;
					unsigned int m_time_current;

					float m_alpha;

					unsigned int m_textureId;
			};
		}
	}
}

#endif /* EMPTYTRANSITION_H_ */
