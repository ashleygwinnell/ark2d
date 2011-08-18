/*
 * GamePanel.cpp
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "DefaultGame.h"
#include "GamePanel.h"
#include "WaveEditorState.h"


GamePanel::GamePanel(): Panel(), m_offsetX(50), m_offsetY(50), m_selectedIndex(-1),
	m_moving(false), m_movingEnemy(false) {
}
void GamePanel::keyPressed(unsigned int key) {
	Panel::keyPressed(key);
	DefaultGame* game = DefaultGame::getInstance();

	if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT)
	{
		if (game->m_waveState->m_toolPanel->m_selectedTool == game->m_waveState->m_toolSelect) {

			if (isMouseInBounds()) {

				bool s = false;
				for(unsigned int i = 0; i < game->m_waveState->m_wave->m_enemies.size(); i++)
				{
					int mx = ARK2D::getInput()->getMouseX() - getOnScreenX() - m_offsetX;
					int my = ARK2D::getInput()->getMouseY() - getOnScreenY() - m_offsetY;
					EditorEnemy* e = game->m_waveState->m_wave->m_enemies.get(i);
					bool collides = GigaRectangle<int>::s_contains(e->m_cx - 10, e->m_cy - 10, 20, 20, mx, my);
					if (collides) {
						game->m_waveState->selectEnemy(i);
						s = true;
						m_movingEnemy = true;
						break;
					}
				}
				if (s == false) {
					game->m_waveState->selectEnemy(-1);
				}
			}

		} else if (game->m_waveState->m_toolPanel->m_selectedTool == game->m_waveState->m_toolAddEnemy) {
			if (game->m_waveState->m_enemySelectPanel->m_enemySelected != 0) {
				Input* i = ARK2D::getInput();
				if (game->m_waveState->m_gamePanel->isMouseInBounds()) {
					int snap = game->m_snapField->getText().getAsInt();
					int mx = i->getMouseX() - game->m_waveState->m_gamePanel->getOnScreenX() - game->m_waveState->m_gamePanel->m_offsetX;
					int my = i->getMouseY() - game->m_waveState->m_gamePanel->getOnScreenY() - game->m_waveState->m_gamePanel->m_offsetY;
					MathUtil::snap(snap, mx, my);
					game->m_waveState->placeEnemy(
						game->m_waveState->m_enemySelectPanel->m_enemySelected,
						mx,
						my
					);
				}
			}
		}
	}

	if (key == (unsigned int) Input::MOUSE_BUTTON_RIGHT) {
		if (isMouseInBounds()) {
			m_moving = true;
		}
	}

	if (m_selectedIndex != -1) {
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
	}


}
void GamePanel::tabSelectedEnemy() {
	DefaultGame* game = DefaultGame::getInstance();
	m_selectedIndex++;
	if (m_selectedIndex >= (signed int) game->m_waveState->m_wave->m_enemies.size()) {
		m_selectedIndex = 0;
	}

	if (game->m_waveState->m_wave->m_enemies.size() == 0) {
		m_selectedIndex = -1;
		game->m_waveState->m_selectPanel->setVisible(false);
		return;
	}

	game->m_waveState->selectEnemy(m_selectedIndex);
}
void GamePanel::keyReleased(unsigned int key) {
	Panel::keyReleased(key);

	m_movingEnemy = false;
	if (key == (unsigned int) Input::MOUSE_BUTTON_RIGHT) {
		m_moving = false;
	}
}
void GamePanel::mouseMoved(int x, int y, int oldx, int oldy) {
	Panel::mouseMoved(x, y, oldx, oldy);

	DefaultGame* game = DefaultGame::getInstance();

	if (game->m_waveState->m_toolPanel->m_selectedTool == game->m_waveState->m_toolSelect) {
		if (m_movingEnemy && m_selectedIndex != -1) {
			EditorEnemy* e = game->m_waveState->m_wave->m_enemies.get(m_selectedIndex);

			Input* in = ARK2D::getInput();
			int snap = game->m_snapField->getText().getAsInt();
			int nx = in->getMouseX() - game->m_waveState->m_gamePanel->getOnScreenX() - game->m_waveState->m_gamePanel->m_offsetX;
			int ny = in->getMouseY() - game->m_waveState->m_gamePanel->getOnScreenY() - game->m_waveState->m_gamePanel->m_offsetY;
			MathUtil::snap(snap, nx, ny);
			//std::cout << nx << ":" << ny << std::endl;
			e->m_cx = nx;
			e->m_cy = ny;
		} else {
			for(unsigned int i = 0; i < game->m_waveState->m_wave->m_enemies.size(); i++) {
				int mx = ARK2D::getInput()->getMouseX() - getOnScreenX() - m_offsetX;
				int my = ARK2D::getInput()->getMouseY() - getOnScreenY() - m_offsetY;
				EditorEnemy* e = game->m_waveState->m_wave->m_enemies.get(i);
				bool collides = GigaRectangle<int>::s_contains(e->m_cx - 10, e->m_cy - 10, 20, 20, mx, my);
				e->m_mouseOver = collides;
			}
		}
	}

	if (m_moving == true && isMouseInBounds()) {
		m_offsetX += (x - oldx);
		m_offsetY += (y - oldy);
	}
}
