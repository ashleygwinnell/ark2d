/*
 * SplitPane.h
 *
 *  Created on: 1 Oct 2015
 *      Author: Ashley
 */

#ifndef ARK_UI_SPLITPANE_H_
#define ARK_UI_SPLITPANE_H_

#include "../Namespaces.h"
#include "AbstractUIComponent.h"
#include "../Core/Geometry/Cube.h"

namespace ARK {
	namespace UI {

		/*!
		 * \brief A UI component that splits it's parent down the middle.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API SplitPane : public AbstractUIComponent {
			public:
                ARK::Core::Geometry::Cube m_bounds;
				unsigned int m_splitType;
				float m_splitLocation;

				SceneNode* m_firstComponent;
				SceneNode* m_secondComponent;

			public:
				static const unsigned int SPLIT_HORIZONTAL = 0; // Two items side by side.
				static const unsigned int SPLIT_VERTICAL = 1; // Two items one above the other.

			public:
				SplitPane();
				SplitPane(unsigned int splitType);
				SplitPane(unsigned int splitType, SceneNode* firstComponent, SceneNode* secondComponent);
				SplitPane(unsigned int splitType, SceneNode* firstComponent, SceneNode* secondComponent, float splitLocation);
				void setSplitLocation(float f);
				void setLeftComponent(SceneNode* left);
				void setRightComponent(SceneNode* right);
				void setTopComponent(SceneNode* top);
				void setBottomComponent(SceneNode* bottom);

				virtual void render();

				virtual	void setBounds(float w, float h, float z);
                virtual ARK::Core::Geometry::Cube* getBounds();

				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);

				virtual void onResize();

				virtual ~SplitPane();
		};
	}
}

#endif /* ARK_UI_SPLITPANE_H_ */
