/*
 * Panel.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef PANEL_H_
#define PANEL_H_

#include <vector>
using namespace std;

//#include "../ARK2D.h"
#include "../Namespaces.h"
#include "AbstractUIComponent.h"

//#include "../Graphics/Renderer.h"
//#include "../Core/GameContainer.h"
//#include "../Geometry/GigaRectangle.h"
//#include "../Controls/Input.h"

namespace ARK {
	namespace UI {

		/*!
		 * \brief A panel that holds a bunch of other UIComponents.
		 *
		 * @todo Layout Managers.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Panel : public AbstractUIComponent{
			public:
				//vector<AbstractUIComponent*> m_children;
				bool m_translate;
				bool m_showBorder;
				ARK::Geometry::Cube* m_bounds;
				Panel();
				//void add(AbstractUIComponent* c);
				//void setTranslating(bool b);
				void setShowBorder(bool b);

				virtual void render();
				virtual void renderBorder();

				virtual ARK::Geometry::Cube* getBounds();

				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);

				virtual ~Panel();
		};
	}
}

#endif /* PANEL_H_ */
