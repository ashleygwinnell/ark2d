/*
 * ToolPanel.h
 *
 *  Created on: 23 Jul 2011
 *      Author: Ashley
 */

#ifndef TOOLPANEL_H_
#define TOOLPANEL_H_

#include "ARK.h"

class ToolPanel : public ScrollPanel {
	public:
		Button* m_selectedTool;
		ToolPanel();
		void renderBorder();
		static void refreshDependencies();
		static void select();
		static void addEnemy();
		static void options();
};

#endif /* TOOLPANEL_H_ */
