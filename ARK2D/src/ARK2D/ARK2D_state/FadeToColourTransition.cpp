/*
 * FadeTransition.cpp
 *
 *  Created on: 22nd Nov 2010
 *      Author: Ashley
 */

#include "FadeToColourTransition.h"
#include "../ARK2D_GL.h"

FadeToColourTransition::FadeToColourTransition(float time):
	Transition(),
	m_time(time),
	m_time_current(0.0f),
	m_color(NULL),
	m_alpha(1.0f)
	{

}
void FadeToColourTransition::init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) {
	Transition::init(container, game, from, to);
	m_time_current = 0.0f;
}
void FadeToColourTransition::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	m_alpha = (float) Easing::ease(m_easing, double(m_time_current), 0.0f, double(m_color->getAlpha()/255.0f), double(m_time));
	m_time_current += timer->getDelta();
}
void FadeToColourTransition::preRender(GameContainer* container, StateBasedGame* game, Graphics* g) {

}
void FadeToColourTransition::postRender(GameContainer* container, StateBasedGame* game, Graphics* g) {

	m_color->setAlpha(m_alpha*255);
	g->setDrawColor(m_color->getRed(), m_color->getGreen(), m_color->getBlue(), m_color->getAlpha());

	//g->setDrawColor(Color::white);
	g->fillRect(0,0, container->getWidth(), container->getHeight());
}
bool FadeToColourTransition::isComplete() {
	if (m_time_current >= m_time) {
		return true;
	}
	return false;
}

void FadeToColourTransition::setColor(Color* c) {
	m_color = c;
}

FadeToColourTransition::~FadeToColourTransition() {

}
