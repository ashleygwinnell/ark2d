/*
 * FloatingFadingText.cpp
 *
 *  Created on: 22 May 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "DefaultGame.h"
#include "FloatingFadingText.h"

FloatingFadingText::FloatingFadingText(string s):
	GameObject(), m_text(s), m_alpha(1.0f)
	{
	m_velocity->set(0, 0);
	m_velocityMaximum->set(0, 100);
	m_acceleration->set(0, -20);
}

void FloatingFadingText::update(GameContainer* container, GameTimer* timer) {
	m_velocity->m_y += m_acceleration->m_y;
	if (m_velocity->getY() > m_velocityMaximum->getY()) {
		m_velocity->setY(m_velocityMaximum->getY());
	}
	m_bounds->setY(m_bounds->getY() + m_velocity->m_y * timer->getDelta());

	m_alpha -= 2.0f * timer->getDelta();
}
void FloatingFadingText::render(GameContainer* container, Graphics* g) {
	DefaultGame* game = DefaultGame::getGame();
	g->setFont(game->font_small);
	g->getFont()->getImage()->setAlpha(m_alpha);
	//g->setDrawColor(*m_color);
	g->drawStringCenteredAt(m_text, int(m_bounds->getX()), int(m_bounds->getY()));
	g->getFont()->getImage()->setAlpha(1.0f);
}

FloatingFadingText::~FloatingFadingText() {
	// TODO Auto-generated destructor stub
}
