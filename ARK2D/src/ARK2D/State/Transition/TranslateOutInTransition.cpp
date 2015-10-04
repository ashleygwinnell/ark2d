/*
 * TranslateXTransition.cpp
 *
 *  Created on: 1 Aug 2010
 *      Author: Ashley
 */

#include "TranslateOutInTransition.h"
#include "../../Tween/Timeline.h"

 #include "../../Graphics/Color.h"
#include "../GameState.h"
#include "../StateBasedGame.h"
#include "../../Tween/Easing.h"

namespace ARK {
	namespace State {
		namespace Transition {

			TranslateOutInTransition::TranslateOutInTransition(unsigned int dir):
					m_direction(dir),
					m_current_time(0.0f),
					m_time(1.0f),
					m_offset_x(0.0f) {

			}

			TranslateOutInTransition::TranslateOutInTransition(unsigned int dir, float timeSeconds):
					m_direction(dir),
					m_current_time(0.0f),
					m_time(timeSeconds),
					m_offset_x(0.0f) {

			}

			void TranslateOutInTransition::init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) {
				Transition::init(container, game, from, to);
				m_offset_x = 0.0f;
				m_current_time = 0.0f;
				m_easing = Easing::LINEAR;
				switch(m_direction) {
					case TranslateOutInTransition::DIRECTION_UP:
						break;
					case TranslateOutInTransition::DIRECTION_DOWN:
						break;
					case TranslateOutInTransition::DIRECTION_LEFT:
						m_offset_x = 0;
						break;
					case TranslateOutInTransition::DIRECTION_RIGHT:
						m_offset_x = float(container->getWidth()) * -1;
						break;
				}
			}
			void TranslateOutInTransition::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

				m_current_time += timer->getDelta();

				switch(m_direction) {
					case TranslateOutInTransition::DIRECTION_UP:

						break;
					case TranslateOutInTransition::DIRECTION_DOWN:

						break;
					case TranslateOutInTransition::DIRECTION_LEFT:
						m_offset_x = Easing::ease(m_easing, m_current_time, 0, float(container->getWidth()) * -1, m_time);
						break;
					case TranslateOutInTransition::DIRECTION_RIGHT:
						m_offset_x = Easing::ease(m_easing, m_current_time, float(container->getWidth()) * -1, float(container->getWidth()), m_time);
						break;
				}
			}

			void TranslateOutInTransition::preRender(GameContainer* container, StateBasedGame* game, Renderer* g) {

			}

			void TranslateOutInTransition::postRender(GameContainer* container, StateBasedGame* game, Renderer* g) {

				switch(m_direction) {
					case TranslateOutInTransition::DIRECTION_UP:

						break;
					case TranslateOutInTransition::DIRECTION_DOWN:

						break;
					case TranslateOutInTransition::DIRECTION_LEFT:
						g->translate(m_offset_x, 0.0f);
						m_from->render(container, game, g);
						g->translate((float) 0 + container->getWidth(), 0.0f);
						m_to->render(container, game, g);
						g->translate((float) 0 - container->getWidth(), 0.0f);
						g->translate(m_offset_x * -1, 0.0f);

						break;
					case TranslateOutInTransition::DIRECTION_RIGHT:
						g->translate(m_offset_x, 0.0f);
						m_to->render(container, game, g);
						g->translate((float) 0 + container->getWidth(), 0.0f);
						m_from->render(container, game, g);
						g->translate((float) 0 - container->getWidth(), 0.0f);
						g->translate(m_offset_x * -1, 0.0f);
						break;
				}

			}

			bool TranslateOutInTransition::isComplete() {
				if (m_current_time >= m_time) {
					return true;
				}
				return false;
			}

			TranslateOutInTransition::~TranslateOutInTransition() {

			}
		}
	}
}
