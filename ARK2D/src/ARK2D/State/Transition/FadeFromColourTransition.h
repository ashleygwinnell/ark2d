/*
 * FadeFromColourTransition.h
 *
 *  Created on: 22nd Nov 2010
 *      Author: Ashley
 */

#ifndef FADEFROMCOLOURTRANSITION_H_
#define FADEFROMCOLOURTRANSITION_H_

#include "Transition.h"
//#include "../../Tween/Timeline.h"

namespace ARK {
	namespace State {
		namespace Transition {

			/*!
			 * \brief Fade from the colour to a game state as part 2 of a two part transition.
			 *
			 * \warning Do NOT use default immutable colours from the Color class. This class modifies the colour passed in.
			 *
			 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
			 * @todo This transition does not work.
			 */
			class ARK2D_API FadeFromColourTransition : public Transition {
				public:
					FadeFromColourTransition(float time);
					FadeFromColourTransition(float time, Color* c);
					void init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to);
					void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
					void preRender(GameContainer* container, StateBasedGame* game, Renderer* g);
					void postRender(GameContainer* container, StateBasedGame* game, Renderer* g);
					bool isComplete();
					virtual ~FadeFromColourTransition();

					void setColor(Color* c);


					float m_time;
					float m_time_current;

					Color* m_color;
					float m_colorOriginalAlpha;
					float m_alpha;

					unsigned int m_textureId;
			};
		}
	}
}

#endif /* EMPTYTRANSITION_H_ */
