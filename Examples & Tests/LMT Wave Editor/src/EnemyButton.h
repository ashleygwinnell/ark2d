/*
 * EnemyButton.h
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#ifndef ENEMYBUTTON_H_
#define ENEMYBUTTON_H_

#include "ARK.h"

class EnemyButton : public Button {
	public:
		int m_id;
		EnemyButton();
		virtual void renderOverlay();
		static void pressedStatic(void* b);
		void pressed();
		virtual ~EnemyButton();
};

#endif /* ENEMYBUTTON_H_ */
