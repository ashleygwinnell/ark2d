/*
 * EnemySelectPanel.cpp
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "DefaultGame.h"
#include "EnemySelectPanel.h"

EnemySelectPanel::EnemySelectPanel(): ScrollPanel(), m_enemySelected(0) {
	m_visible = false;
}
void EnemySelectPanel::setVisible(bool b) {
	ScrollPanel::setVisible(b);
	if (!b) {
		m_enemySelected = 0;
	}
}

EnemySelectPanel::~EnemySelectPanel() {

}
