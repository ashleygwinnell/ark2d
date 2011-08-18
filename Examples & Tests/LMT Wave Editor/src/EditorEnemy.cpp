/*
 * EditorEnemy.cpp
 *
 *  Created on: 21 Jul 2011
 *      Author: Ashley
 */

#include "EditorEnemy.h"
#include "DefaultGame.h"
#include "WaveEditorState.h"

EditorEnemy::EditorEnemy():
	ARKGameObject(),
	m_id(0),
	m_cx(0),
	m_cy(0),
	m_mouseOver(false),
	m_delay(0.0f),
	m_timer(0.0f),
	m_speedModifier(1.0f),
	m_pathGroup(NULL) {

}
int EditorEnemy::getXAtTime(float f) {
	if (m_pathGroup == NULL) { return m_cx; }

	if (f > m_delay) {
		f -= m_delay;
		f *= m_speedModifier;
		m_pathGroup->setTimer(f);
		m_pathGroup->updateCurrent();
		return (int) (m_cx + m_pathGroup->getLocation()->getX());
	}
	return m_cx;
}
int EditorEnemy::getYAtTime(float f) {
	if (m_pathGroup == NULL) { return m_cy; }

	if (f > m_delay) {
		f -= m_delay;
		f *= m_speedModifier;
		m_pathGroup->setTimer(f);
		m_pathGroup->updateCurrent();
		return (int) (m_cy + m_pathGroup->getLocation()->getY());
	}
	return m_cy;
}
float EditorEnemy::getDuration() {
	if (m_pathGroup == NULL) {
		return m_delay;
	}
	return m_delay + (m_pathGroup->getDuration() * (1.0f/m_speedModifier));
}
void EditorEnemy::update(GameContainer* container, GameTimer* timer) {

}
void EditorEnemy::render(GameContainer* container, Graphics* g) {
	DefaultGame* game = DefaultGame::getInstance();
	//game->m_enemyImages.at(m_id-1)->drawCentered(m_cx, m_cy);

	//float t = game->m_waveState->m_timer;
	game->m_enemyImages.at(m_id-1)->drawCentered(getXAtTime(m_timer), getYAtTime(m_timer));

	if (m_mouseOver) {
		g->setDrawColor(Color::white_50a);
		g->drawCircle(m_cx, m_cy, 20, 30);
	}
}
EditorEnemy::~EditorEnemy() {

}
