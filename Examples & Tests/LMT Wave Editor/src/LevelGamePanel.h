/*
 * LevelGamePanel.h
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#ifndef LEVELGAMEPANEL_H_
#define LEVELGAMEPANEL_H_

#include "ARK.h"

class LevelGamePanel : public Panel {
	public:
		int m_offsetX;
		int m_offsetY;
		int m_selectedIndex;
		bool m_moving;
		bool m_movingWave;

		LevelGamePanel();
		void keyPressed(unsigned int key);
		void keyReleased(unsigned int key);
		void mouseMoved(int x, int y, int oldx, int oldy);

		void tabSelectedEnemy();
		virtual ~LevelGamePanel();
};

#endif /* LEVELGAMEPANEL_H_ */
