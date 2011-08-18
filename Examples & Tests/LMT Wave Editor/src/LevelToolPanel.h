/*
 * LevelToolPanel.h
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#ifndef LEVELTOOLPANEL_H_
#define LEVELTOOLPANEL_H_

#include "ARK.h"

class LevelToolPanel : public ScrollPanel {
	public:
		Button* m_selectedTool;
		LevelToolPanel();
		void renderBorder();
		~LevelToolPanel();

		static void refreshDependencies();
		static void select();
		static void addWave();
		static void events();
		static void options();
};

#endif /* LEVELTOOLPANEL_H_ */
