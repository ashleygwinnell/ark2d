/*
 * VerticalScrollArea.h
 *
 *  Created on: 09 Aug 2017
 *      Author: Ashley
 */

#ifndef ARK_UI_UTIL_VERTICALSCROLLAREA_H_
#define ARK_UI_UTIL_VERTICALSCROLLAREA_H_

#include "../../Common/DLL.h"
#include "../../Namespaces.h"

#include "../../Core/Tween/Timer.h"
#include "../../Core/Geometry/Vector2.h"

namespace ARK {
	namespace UI {
		namespace Util {
		
			/*!
			 * \brief An util class for creating vertically scrolling content (with optional scissoring).
			 *
			 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
			 */
			class ARK2D_API VerticalScrollArea {
				public:
					int m_x;
					int m_y;
					int m_width;
					int m_height;
					int m_originalX;
					int m_originalY;
					float m_scrollY;
					float m_scrollYReleased;
					float m_scrollYSpeed;
					float m_scrollYDecelerate;
					float m_scrollYUnderflow; // How far can we scroll past the top/bottoms

					bool m_dragging;
					float m_draggingStartY;
					Vector2<float> m_draggingStart;
					Vector2<float> m_draggingEnd;
					Timer m_releaseTimer;
					float m_dragTimer;
					float m_dragMinimumRequired;

					int m_innerHeight;
					int m_actualInnerHeight;

					bool m_scissoring;

					VerticalScrollArea();
					virtual void recalculateHeight();
					virtual void update();

					virtual void preRender();
					virtual void render();
					virtual void postRender();

					virtual float getCurY();

					float getInsideTopDecorationY();
					float getInsideBottomDecorationY();
					
					virtual ~VerticalScrollArea();

					virtual bool keyPressed(unsigned int key);
					virtual bool keyReleased(unsigned int key);
					virtual bool mouseMoved(int x, int y, int oldx, int oldy);
			};
		}
	}
}

#endif
