/*
 * ToolPanel.cpp
 *
 *  Created on: 23 Jul 2011
 *      Author: Ashley
 */

#include "ToolPanel.h"
#include "DefaultGame.h"
#include "WaveEditorState.h"
#include "GamePanel.h"

ToolPanel::ToolPanel():
	ScrollPanel(),
	m_selectedTool(NULL)
	{

}
void ToolPanel::renderBorder() {
	if (m_selectedTool != NULL) {
		Graphics* g = ARK2D::getGraphics();
		g->setDrawColor(Color::red);
		g->drawRect(
			m_x + m_selectedTool->getX(),
			m_y + m_selectedTool->getY(),
			m_selectedTool->getWidth(),
			m_selectedTool->getHeight()
		);
	}
}
void ToolPanel::refreshDependencies() {
	DefaultGame* g = DefaultGame::getInstance();

	g->m_waveState->m_gamePanel->setVisible(false);
	g->m_options->setVisible(false);

	g->m_waveState->m_selectPanel->setVisible(false);
	g->m_waveState->m_enemySelectPanel->setVisible(false);
	g->m_waveState->m_gamePanel->m_selectedIndex = -1;

	if (g->m_waveState->m_toolPanel->m_selectedTool == g->m_waveState->m_toolSelect) {
		g->m_waveState->m_gamePanel->setVisible(true);
	}

	if (g->m_waveState->m_toolPanel->m_selectedTool == g->m_waveState->m_toolAddEnemy) {
		g->m_waveState->m_enemySelectPanel->setVisible(true);
		g->m_waveState->m_gamePanel->setVisible(true);
	}

	if (g->m_waveState->m_toolPanel->m_selectedTool == g->m_waveState->m_toolOptions) {
		g->m_options->setVisible(true);
	}
}
void ToolPanel::select() {
	DefaultGame* g = DefaultGame::getInstance();
	g->m_waveState->m_toolPanel->m_selectedTool = g->m_waveState->m_toolSelect;

	refreshDependencies();
}
void ToolPanel::addEnemy() {
	DefaultGame* g = DefaultGame::getInstance();
	g->m_waveState->m_toolPanel->m_selectedTool = g->m_waveState->m_toolAddEnemy;

	refreshDependencies();
}
void ToolPanel::options() {
	DefaultGame* g = DefaultGame::getInstance();
	g->m_waveState->m_toolPanel->m_selectedTool = g->m_waveState->m_toolOptions;

	refreshDependencies();
}
