/*
 * TitledPanel.h
 *
 *  Created on: 1 Oct 2015
 *      Author: Ashley
 */

#ifndef ARK_UI_TITLEDPANEL_H_
#define ARK_UI_TITLEDPANEL_H_

//#include "../Includes.h"
#include "../Namespaces.h"
#include "Panel.h"

namespace ARK {
	namespace UI {

		class ARK2D_API TitledPanel : public ARK::UI::Panel {
			public:
				string m_title;
				Panel* m_content;
			public:
				TitledPanel();
				TitledPanel(Panel* content, string title);
				void setTitle(string title);
				virtual void addChild(SceneNode* node);
				virtual void setBounds(float w, float h, float d);
				virtual void render();
		};
	}
}

#endif