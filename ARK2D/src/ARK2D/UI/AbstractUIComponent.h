/*
 * AbstractUIComponent.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef ABSTRACTUICOMPONENT_H_
#define ABSTRACTUICOMPONENT_H_

#include "UIComponent.h"
#include "../Graphics/Renderer.h"
#include "../Core/GameContainer.h"
#include "../Controls/Input.h"
#include "../Util/Vector4.h"
#include "../Geometry/GigaRectangle.h"

namespace ARK {
	namespace UI {
		/*!
		 * \brief An abstract UI element that implements the UIComponent interface.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class AbstractUIComponent : public UIComponent {
			public:
				AbstractUIComponent* m_parent;
				float m_x;
				float m_y;
				unsigned int m_width;
				unsigned int m_height;
				Vector4<int> m_padding; // left, top, right, bottom.
				Vector4<int> m_margin; // left, top, right, bottom.
				bool m_clipping;
				bool m_visible;
				bool m_enabled;

				AbstractUIComponent();
				virtual float getX();
				virtual float getY();
				virtual float getCenterX();
				virtual float getCenterY();
				virtual float getMinX();
				virtual float getMinY();
				virtual float getMaxX();
				virtual float getMaxY();
				virtual unsigned int getWidth();
				virtual unsigned int getHeight();
				virtual void setX(float x);
				virtual void setY(float y);
				virtual void setLocation(float x, float y);
				virtual void setLocationByCenter(float x, float y);
				virtual void setSize(unsigned int w, unsigned int h);

				void setWidth(unsigned int w);
				void setHeight(unsigned int h);

				void setPadding(int l, int t, int r, int b);
				void setPadding(int all);
				int getPaddingLeft();
				int getPaddingTop();
				int getPaddingRight();
				int getPaddingBottom();
				int getPaddingX();
				int getPaddingY();

				void setMargin(int l, int t, int r, int b);
				void setMargin(int all);
				int getMarginLeft();
				int getMarginTop();
				int getMarginRight();
				int getMarginBottom();
				int getMarginX();
				int getMarginY();

				void setClipping(bool b);

				AbstractUIComponent* getParent();
				void setParent(AbstractUIComponent* p);
				bool hasParent();

				int getOnScreenX();
				int getOnScreenY();

				void setVisible(bool b);
				bool isVisible();

				void setEnabled(bool b);
				bool isEnabled();

				virtual void render() = 0;
				virtual void preRender();
				virtual void postRender();

				virtual void keyPressed(unsigned int key);
				virtual void keyReleased(unsigned int key);
				virtual void mouseMoved(int x, int y, int oldx, int oldy);

				bool isMouseInBounds();

				virtual ~AbstractUIComponent();
		};
	}
}

#endif /* ABSTRACTUICOMPONENT_H_ */
