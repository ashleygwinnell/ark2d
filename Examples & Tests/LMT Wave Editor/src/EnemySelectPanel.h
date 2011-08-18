/*
 * EnemySelectPanel.h
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#ifndef ENEMYSELECTPANEL_H_
#define ENEMYSELECTPANEL_H_

#include "ARK.h"

class EnemySelectPanel : public ScrollPanel {
	public:
		int m_enemySelected;

		EnemySelectPanel();
		void setVisible(bool b);
		virtual ~EnemySelectPanel();
};

#endif /* ENEMYSELECTPANEL_H_ */
