/*
 * AbstractUIComponent.cpp
 *
 *  Created on: 07 Aug 2017
 *      Author: Ashley
 */

#include "SwipePageSelector.h"
#include "../../Core/GameContainer.h"
#include "../../Core/Tween/Easing.h"

namespace ARK {
	namespace UI {
		namespace Util { 
			SwipePageSelector::SwipePageSelector(): 
				m_pagePrevious(0),
				m_page(0),
				m_pageTarget(0),
				m_pages(1),

				m_dragging(false),
				m_draggingStart(0,0),
				m_draggingEnd(0,0),
				m_draggingTimer(0.4f),
				m_draggingOffsetX(0),
				m_draggingOffsetXReleased(0),
				m_draggingSideArrowsPressedTimer(0.0f),

				m_pageSpacing(320.0f),
				m_pageDragLimitSides(100.0f),
				m_pageDragLimitEach(300.0f)
			{

			}
			float SwipePageSelector::getOffsetX() {
				return m_draggingOffsetX * -1.0f;
			}
			float SwipePageSelector::getPageOffsetX() {
				return (m_draggingOffsetX - (m_page * m_pageSpacing))*-1.0f;
			}
			float SwipePageSelector::getPageContentScale(int currentlyRenderingPageIndex, float offScale, float onScale) {
				float scale = offScale;
				if (currentlyRenderingPageIndex == m_page) {
					scale = Easing::easebetween(Easing::LINEAR, abs(getPageOffsetX()), onScale, offScale, m_pageSpacing);
				} else if (currentlyRenderingPageIndex == m_page-1 || currentlyRenderingPageIndex == m_page+1) {
					scale = Easing::easebetween(Easing::LINEAR, abs(getPageOffsetX()), offScale, onScale, m_pageSpacing);
				} 
				return scale;
			}

			float SwipePageSelector::getAppearingContentScale() {
				float pageOffsetX = getPageOffsetX();
				float appearingContentScale = Easing::easebetween(Easing::LINEAR, abs(pageOffsetX), 1.0f, 0.0f, m_pageSpacing * 0.5f);
				if (abs(pageOffsetX) > m_pageSpacing*0.5f) {
					appearingContentScale = Easing::easebetween(Easing::LINEAR, abs(pageOffsetX) - (m_pageSpacing * 0.5f), 0.0f, 1.0f, m_pageSpacing * 0.5f);
				}
				return appearingContentScale;
			}

			int SwipePageSelector::getAppearingContentIndex() {
				int appearingContentIndex = m_page;
				if (abs(getPageOffsetX()) > m_pageSpacing * 0.5f) {
					appearingContentIndex = m_pageTarget;
				}
				return appearingContentIndex;
			}

			void SwipePageSelector::update() {
				// target page x;
				float targetPageX = (m_pageSpacing * m_page);
				if (!m_dragging) {
					m_draggingOffsetX = targetPageX;

					if (m_draggingTimer.getTime() > 0.0f)
					{
						m_draggingOffsetX = Easing::ease(Easing::QUADRATIC_OUT, m_draggingTimer.getTime(), m_draggingOffsetXReleased, targetPageX - m_draggingOffsetXReleased, m_draggingTimer.getDuration());

						if (m_draggingTimer.update()) {
							m_draggingTimer.reset();
							//m_page = m_pageTarget;
						}
					}

				} else {
					m_draggingOffsetX = m_draggingOffsetXReleased;
				}
			}

			bool SwipePageSelector::shouldDrawPage(int page) {
				return (page <= m_page+1 || page >= page-1);
			}

			void SwipePageSelector::pageLeftButtonEvent() {
				if (m_page > 0) {
					m_pageTarget = m_page;
					m_pagePrevious = m_page;
					m_page--;
					m_draggingTimer.reset();
					m_draggingTimer.update();

					m_draggingSideArrowsPressedTimer += 0.001f;
				}
			}
			void SwipePageSelector::pageRightButtonEvent() {
				if (m_page < m_pages-1) {
					m_pageTarget = m_page;
					m_pagePrevious = m_page;
					m_page++;
					m_draggingTimer.reset();
					m_draggingTimer.update();

					m_draggingSideArrowsPressedTimer += 0.001f;
				}
			}
			bool SwipePageSelector::keyPressed(unsigned int key) {
				Input* in = ARK2D::getInput();
				if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {
					if (!m_dragging) {
						m_pageTarget = m_page;
						m_pagePrevious = m_page;
						m_dragging = true;
						m_draggingStart.set(in->getMouseX(), in->getMouseY());

						m_draggingOffsetXReleased = (m_pageSpacing * m_page) + (m_draggingStart.getX() - in->getMouseX());
					}
				}
			}
			bool SwipePageSelector::keyReleased(unsigned int key) {
			    Input* in = ARK2D::getInput();
				if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT && m_dragging) {
					m_dragging = false;
					m_draggingEnd.set(in->getMouseX(), in->getMouseY());

					m_pageTarget = m_page;

					if (m_draggingStart.getX() - m_draggingEnd.getX() > 30.0f && m_page < m_pages-1) {
						// page right
						m_pagePrevious = m_page;
						m_page++;

					} else if (m_draggingStart.getX() - m_draggingEnd.getX() < -30.0f && m_page > 0) {
						// page left

						m_pagePrevious = m_page;
						m_page--;
						//m_draggingTimer.reset();
						//m_draggingTimer.update();
					}

					m_draggingTimer.reset();
					m_draggingTimer.update();
				}
			}
			bool SwipePageSelector::mouseMoved(int x, int y, int oldx, int oldy) {
				if (m_dragging) {
					m_draggingOffsetXReleased = (m_pageSpacing * m_page) + (m_draggingStart.getX() - x);

					// limit left side
					if (m_draggingOffsetXReleased < -m_pageDragLimitSides) {
						m_draggingOffsetXReleased = -m_pageDragLimitSides;
					} else if (m_draggingOffsetXReleased > ((m_pages-1) * m_pageSpacing) + m_pageDragLimitSides) {
						m_draggingOffsetXReleased = ((m_pages-1) * m_pageSpacing) + m_pageDragLimitSides;
					}

					// limit to screen width
					if (m_draggingOffsetXReleased > (m_pageSpacing * m_page) + m_pageDragLimitEach) {
						m_draggingOffsetXReleased = (m_pageSpacing * m_page) + m_pageDragLimitEach;
					} else if (
						(m_draggingOffsetXReleased < (m_pageSpacing * m_page) - m_pageDragLimitEach) &&
						(m_page > 0) &&
						!(m_draggingOffsetXReleased > (m_pageSpacing * m_page) )
					) {
						m_draggingOffsetXReleased = (m_pageSpacing * m_page) - m_pageDragLimitEach;
						//ARK2D::getLog()->g("derp");
					}

					if (m_draggingStart.getX() - x > 1.0f && m_page < m_pages-1) {
						m_pageTarget = m_page + 1;
					} else if (m_draggingStart.getX() - x < -1.0f && m_page > 0) {
						m_pageTarget = m_page - 1;
					} else {
						m_pageTarget = m_page;
					}

					m_draggingTimer.reset();
					m_draggingTimer.update();
				}
			}
		}
	}
}


