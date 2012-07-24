/*
 * FadeFromColourTransition.cpp
 *
 *  Created on: 22nd Nov 2010
 *      Author: Ashley
 */

#include "FadeFromColourTransition.h"
#include "../../Includes.h"

namespace ARK {
	namespace State {
		namespace Transition {

			FadeFromColourTransition::FadeFromColourTransition(float time):
				Transition(),
				m_time(time),
				m_time_current(0.0f),
				m_color(NULL),
				m_alpha(1.0f)
				{

			}
			FadeFromColourTransition::FadeFromColourTransition(float time, Color* c):
				Transition(),
				m_time(time),
				m_time_current(0.0f),
				m_color(c),
				m_alpha(1.0f) {
				m_colorOriginalAlpha = m_color->getAlphaf();
			}
			void FadeFromColourTransition::init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) {
				Transition::init(container, game, from, to);
				m_time_current = 0.0f;
				m_alpha = m_colorOriginalAlpha;
			}
			void FadeFromColourTransition::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
				m_alpha = (float) Easing::ease(m_easing, double(m_time_current), double(m_colorOriginalAlpha), double(m_colorOriginalAlpha)*-1, double(m_time));
				m_time_current += timer->getDelta();
			}
			void FadeFromColourTransition::preRender(GameContainer* container, StateBasedGame* game, Renderer* g) {

			}
			void FadeFromColourTransition::postRender(GameContainer* container, StateBasedGame* game, Renderer* g) {

				m_color->setAlpha(m_alpha);
				g->setDrawColor(m_color->getRed(), m_color->getGreen(), m_color->getBlue(), m_color->getAlpha());

				//g->setDrawColor(Color::white);
				g->fillRect(0,0, container->getWidth(), container->getHeight());
			}
			bool FadeFromColourTransition::isComplete() {
				if (m_time_current >= m_time) {
					m_color->setAlpha(m_colorOriginalAlpha);
					return true;
				}
				return false;
			}

			void FadeFromColourTransition::setColor(Color* c) {
				m_color = c;
				m_colorOriginalAlpha = m_color->getAlphaf();
			}

			FadeFromColourTransition::~FadeFromColourTransition() {

			}
		}
	}
}
