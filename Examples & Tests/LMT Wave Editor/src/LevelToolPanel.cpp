/*
 * LevelToolPanel.cpp
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#include "LevelToolPanel.h"
#include "DefaultGame.h"
#include "LevelEditorState.h"
#include "GamePanel.h"

LevelToolPanel::LevelToolPanel():
	ScrollPanel(),
	m_selectedTool(NULL)
{

}
void LevelToolPanel::renderBorder() {
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
LevelToolPanel::~LevelToolPanel() {

}
void LevelToolPanel::refreshDependencies() {
	DefaultGame* g = DefaultGame::getInstance();

	g->m_options->setVisible(false);
	g->m_levelState->m_gamePanel->setVisible(false);
	g->m_levelState->m_eventsPanel->setVisible(false);
	g->m_levelState->m_gamePanel->m_selectedIndex = -1;

	if (g->m_levelState->m_sideBar->m_selectedTool == g->m_levelState->m_selectButton) {
		g->m_levelState->m_gamePanel->setVisible(true);
	}
	else if (g->m_levelState->m_sideBar->m_selectedTool == g->m_levelState->m_eventsButton) {
		g->m_levelState->m_eventsPanel->setVisible(true);
	}
	else if (g->m_levelState->m_sideBar->m_selectedTool == g->m_levelState->m_optionsButton) {
		g->m_options->setVisible(true);
	}
}
void LevelToolPanel::select() {
	DefaultGame* g = DefaultGame::getInstance();
	g->m_levelState->m_sideBar->m_selectedTool = g->m_levelState->m_selectButton;

	refreshDependencies();
}
void LevelToolPanel::addWave() {
	//DefaultGame* g = DefaultGame::getInstance();
	//g->m_waveState->m_toolPanel->m_selectedTool = g->m_waveState->m_toolAddEnemy;

	refreshDependencies();
}

void LevelToolPanel::events() {
	DefaultGame* g = DefaultGame::getInstance();
	g->m_levelState->m_sideBar->m_selectedTool = g->m_levelState->m_eventsButton;

	refreshDependencies();
}
void LevelToolPanel::options() {
	DefaultGame* g = DefaultGame::getInstance();
	g->m_levelState->m_sideBar->m_selectedTool = g->m_levelState->m_optionsButton;

	refreshDependencies();
}
