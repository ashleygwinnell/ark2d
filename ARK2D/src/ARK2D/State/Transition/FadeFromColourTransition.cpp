/*
 * FadeFromColourTransition.cpp
 *
 *  Created on: 22nd Nov 2010
 *      Author: Ashley
 */

#include "FadeFromColourTransition.h"
#include "../../ARK2D_GL.h"

FadeFromColourTransition::FadeFromColourTransition(float time):
	Transition(),
	m_time(time),
	m_time_current(0.0f),
	m_color(NULL),
	m_alpha(1.0f)
	{

}
void FadeFromColourTransition::init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) {
	Transition::init(container, game, from, to);
	m_time_current = 0.0f;
}
void FadeFromColourTransition::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	m_alpha = (float) Easing::ease(m_easing, double(m_time_current), double(m_color->getAlpha()/255.0f), double(m_color->getAlpha()/255.0f)*-1, double(m_time));
	m_time_current += timer->getDelta();
}
void FadeFromColourTransition::preRender(GameContainer* container, StateBasedGame* game, Graphics* g) {

}
void FadeFromColourTransition::postRender(GameContainer* container, StateBasedGame* game, Graphics* g) {

	m_color->setAlpha(int(m_alpha*255));
	g->setDrawColor(m_color->getRed(), m_color->getGreen(), m_color->getBlue(), m_color->getAlpha());

	//g->setDrawColor(Color::white);
	g->fillRect(0,0, container->getWidth(), container->getHeight());
}
bool FadeFromColourTransition::isComplete() {
	if (m_time_current >= m_time) {
		return true;
	}
	return false;
}

void FadeFromColourTransition::setColor(Color* c) {
	m_color = c;
}

FadeFromColourTransition::~FadeFromColourTransition() {

}
