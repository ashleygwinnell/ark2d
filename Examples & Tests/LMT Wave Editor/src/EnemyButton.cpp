/*
 * EnemyButton.cpp
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "DefaultGame.h"
#include "WaveEditorState.h"
#include "EnemyButton.h"

EnemyButton::EnemyButton(): Button(), m_id(0) {

}
void EnemyButton::renderOverlay() {
	DefaultGame* game = DefaultGame::getInstance();
	if (game->m_waveState->m_enemySelectPanel->m_enemySelected == m_id) {
	//if (UIComponent::s_currentFocus == this) {
		Graphics* g = ARK2D::getGraphics();
		g->setDrawColor(Color::red);
		g->drawRect(m_x, m_y, m_width, m_height);
	} else {
		Button::renderOverlay();
	}
}
void EnemyButton::pressedStatic(void* b) {
	EnemyButton* eb = (EnemyButton*) b;
	eb->pressed();
}
void EnemyButton::pressed() {
	EnemySelectPanel* p = (EnemySelectPanel*) getParent();
	p->m_enemySelected = m_id;
}
EnemyButton::~EnemyButton() {

}
