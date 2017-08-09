/*
 * SwipePageSelector.h
 *
 *  Created on: 07 Aug 2017
 *      Author: Ashley
 */

#ifndef ARK_UI_UTIL_SWIPEPAGESELECTOR_H_
#define ARK_UI_UTIL_SWIPEPAGESELECTOR_H_

#include "../../Common/DLL.h"
#include "../../Namespaces.h"

#include "../../Core/Tween/Timer.h"
#include "../../Core/Geometry/Vector2.h"

namespace ARK {
	namespace UI {
		namespace Util {
			/*!
			 * \brief An util class for creating horizontal 'pages' of content.
			 * Used in Toast Time and Toast Up!
			 *
			 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
			 */
			class ARK2D_API SwipePageSelector {
				public:
					unsigned int m_pagePrevious;
					unsigned int m_page;
					unsigned int m_pageTarget;
					unsigned int m_pages;

					bool m_dragging;
					Vector2<float> m_draggingStart;
					Vector2<float> m_draggingEnd;
					Timer m_draggingTimer;
					float m_draggingOffsetX;
					float m_draggingOffsetXReleased;
					float m_draggingSideArrowsPressedTimer;

					float m_pageSpacing;
					float m_pageDragLimitSides;
					float m_pageDragLimitEach;

					SwipePageSelector();

					float getOffsetX();
					float getPageOffsetX();

					float getPageContentScale(int currentlyRenderingPageIndex, float offScale, float onScale);
					
					float getAppearingContentScale();
					int getAppearingContentIndex();

					bool shouldDrawPage(int page);

					void update();
					void pageLeftButtonEvent();
					void pageRightButtonEvent();
					
					bool keyPressed(unsigned int key);
					bool keyReleased(unsigned int key);
					bool mouseMoved(int x, int y, int oldx, int oldy);
			};
		}
	}
}
#endif
