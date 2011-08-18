/*
 * LevelGamePanel.cpp
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "DefaultGame.h"
#include "LevelGamePanel.h"
#include "LevelEditorState.h"
#include "WaveEditorState.h"


LevelGamePanel::LevelGamePanel():
	Panel(),
	m_offsetX(50),
	m_offsetY(50),
	m_selectedIndex(-1),
	m_moving(false),
	m_movingWave(false) {

}
void LevelGamePanel::keyPressed(unsigned int key) {
	Panel::keyPressed(key);
	DefaultGame* game = DefaultGame::getInstance();

	if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT)
	{
		//if (game->m_levelState->m_sideBar->m_selectedTool == game->m_levelState->m_selectButton) {

			if (isMouseInBounds()) {

				bool s = false;
				for(unsigned int i = 0; i < game->m_levelState->m_level->m_waves.size(); i++)
				{
					int mx = ARK2D::getInput()->getMouseX() - getOnScreenX() - m_offsetX;
					int my = ARK2D::getInput()->getMouseY() - getOnScreenY() - m_offsetY;
					EditorWave* e = game->m_levelState->m_level->m_waves.get(i);
					bool collides = GigaRectangle<int>::s_contains(
						e->getMinX(),// + e->m_offsetx,
						e->getMinY(),// + e->m_offsety,
						e->getMaxX() - e->getMinX(),
						e->getMaxY() - e->getMinY(),
						mx, my);
					if (collides) {
						game->m_levelState->selectWave(i);
						s = true;
						m_movingWave = true;
						break;
					}
				}
				if (s == false) {
					game->m_levelState->selectWave(-1);
				}
			}

		//}
	}

	if (key == (unsigned int) Input::MOUSE_BUTTON_RIGHT) {
		if (isMouseInBounds()) {
			m_moving = true;
		}
	}

	/*if (m_selectedIndex != -1) {
		if (!game->m_waveState->m_selectPathField->isFocussed() &&
			!game->m_waveState->m_selectPathDelayField->isFocussed() &&
			!game->m_waveState->m_speedModifierField->isFocussed() &&
			!game->m_waveState->m_selectPathSet->isFocussed()
		) {
			if (key == (unsigned int) Input::KEY_DELETE) {
				game->m_waveState->m_wave->m_enemies.remove(m_selectedIndex);
				tabSelectedEnemy();
			}

			if (key == (unsigned int) Input::KEY_TAB) {
				tabSelectedEnemy();
			}
		}
	}*/


}
void LevelGamePanel::tabSelectedEnemy() {
	DefaultGame* game = DefaultGame::getInstance();
	m_selectedIndex++;
	if (m_selectedIndex >= (signed int) game->m_levelState->m_level->m_waves.size()) {
		m_selectedIndex = 0;
	}

	if (game->m_levelState->m_level->m_waves.size() == 0) {
		m_selectedIndex = -1;
		game->m_levelState->m_waveDetailsPanel->setVisible(false);
		return;
	}

	game->m_levelState->selectWave(m_selectedIndex);
}
void LevelGamePanel::keyReleased(unsigned int key) {
	Panel::keyReleased(key);

	m_movingWave = false;
	if (key == (unsigned int) Input::MOUSE_BUTTON_RIGHT) {
		m_moving = false;
	}
}
void LevelGamePanel::mouseMoved(int x, int y, int oldx, int oldy) {
	Panel::mouseMoved(x, y, oldx, oldy);

	DefaultGame* game = DefaultGame::getInstance();

	//if (game->m_levelState->m_toolPanel->m_selectedTool == game->m_waveState->m_toolSelect) {
		if (m_movingWave && m_selectedIndex != -1) {
			EditorWave* e = game->m_levelState->m_level->m_waves.get(m_selectedIndex);

			Input* in = ARK2D::getInput();
			int snap = game->m_snapField->getText().getAsInt();
			int nx = in->getMouseX() - game->m_levelState->m_gamePanel->getOnScreenX() - game->m_levelState->m_gamePanel->m_offsetX;
			int ny = in->getMouseY() - game->m_levelState->m_gamePanel->getOnScreenY() - game->m_levelState->m_gamePanel->m_offsetY;
			MathUtil::snap(snap, nx, ny);
			//std::cout << nx << ":" << ny << std::endl;
			//e->m_x = nx;
			//e->m_y = ny;
			e->m_offsetx += x - oldx;
			e->m_offsety += y - oldy;
		} else {
			for(unsigned int i = 0; i < game->m_levelState->m_level->m_waves.size(); i++) {
				int mx = ARK2D::getInput()->getMouseX() - getOnScreenX() - m_offsetX;
				int my = ARK2D::getInput()->getMouseY() - getOnScreenY() - m_offsetY;
				EditorWave* e = game->m_levelState->m_level->m_waves.get(i);
				bool collides = GigaRectangle<int>::s_contains(
						e->getMinX(),// + e->m_offsetx,
						e->getMinY(),// + e->m_offsety,
						e->getMaxX() - e->getMinX(),
						e->getMaxY() - e->getMinY(),
						mx, my);
				e->m_mouseOver = collides;
			}
		}
	//}

	if (m_moving == true && isMouseInBounds()) {
		m_offsetX += (x - oldx);
		m_offsetY += (y - oldy);
	}
}

LevelGamePanel::~LevelGamePanel() {

}
