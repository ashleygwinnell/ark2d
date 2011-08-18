/*
 * GamePanel.h
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#ifndef GAMEPANEL_H_
#define GAMEPANEL_H_

#include "ARK.h"

class GamePanel : public Panel {
	public:
		int m_offsetX;
		int m_offsetY;
		int m_selectedIndex;
		bool m_moving;
		bool m_movingEnemy;

		GamePanel();
		void keyPressed(unsigned int key);
		void keyReleased(unsigned int key);
		void mouseMoved(int x, int y, int oldx, int oldy);

		void tabSelectedEnemy();
};

#endif /* GAMEPANEL_H_ */
