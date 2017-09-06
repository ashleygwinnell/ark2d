/*
 * VerticalScrollArea.cpp
 *
 *  Created on: 09 Aug 2017
 *      Author: Ashley
 */

#include "VerticalScrollArea.h"
#include "../../Core/GameContainer.h"
#include "../../Core/Tween/Easing.h"

namespace ARK {
	namespace UI {
		namespace Util { 
			VerticalScrollArea::VerticalScrollArea():
				m_x(0),
				m_y(0),
				m_width(0),
				m_height(0),
				m_originalX(0),
				m_originalY(0),
				m_scrollY(0),
				m_scrollYReleased(0),
				m_scrollYSpeed(0.0f),
				m_scrollYDecelerate(200.0f),
				m_scrollYUnderflow(30.0f),

				m_dragging(false),
				m_draggingStartY(0),
				m_draggingStart(0,0),
				m_draggingEnd(0,0),
				m_releaseTimer(0.15f),
				m_dragTimer(0.0f),
				m_dragMinimumRequired(6.0f),

				m_innerHeight(0),
				m_actualInnerHeight(0),

				m_scissoring(true)
			{

			}
			void VerticalScrollArea::setInnerHeight(float height) {
				m_actualInnerHeight = height;
    			m_innerHeight = m_actualInnerHeight - m_height;
			}
			void VerticalScrollArea::recalculateHeight() {
				// do nothing. static height.
				m_innerHeight = m_actualInnerHeight - m_height;
			}
			void VerticalScrollArea::update() {
				GameTimer* timer = ARK2D::getContainer()->getTimer();

				// target page x;
				float targetY = 0;
				if (!m_dragging) {
					m_dragTimer = 0.0f;

					if (m_scrollYSpeed != 0.0f) {
						m_scrollYReleased += m_scrollYSpeed * timer->getDelta();
						if (m_scrollYSpeed > 0.0f) {
							m_scrollYSpeed -= m_scrollYDecelerate * timer->getDelta();
							if (m_scrollYSpeed < 0.0f) {
								m_scrollYSpeed = 0.0f;
							}
						} else if (m_scrollYSpeed < 0.0f) {
							m_scrollYSpeed += m_scrollYDecelerate * timer->getDelta();
							if (m_scrollYSpeed > 0.0f) {
								m_scrollYSpeed = 0.0f;
							}
						}
						//m_scrollY = m_scrollYReleased;
					}

					if (m_scrollYReleased < 0) {
						m_scrollY = targetY;

						if (m_releaseTimer.getTime() > 0.0f) {
							m_scrollY = Easing::ease(Easing::QUADRATIC_OUT, m_releaseTimer.getTime(), m_scrollYReleased, targetY - m_scrollYReleased, m_releaseTimer.getDuration());

							if (m_releaseTimer.update()) {
								m_releaseTimer.reset();
								m_scrollYSpeed = 0.0f;
								//m_page = m_pageTarget;
							}
						}
					} else if (m_scrollYReleased > m_innerHeight) {
						targetY = m_innerHeight;
						m_scrollY = targetY;

						if (m_releaseTimer.getTime() > 0.0f) {
							m_scrollY = Easing::ease(Easing::QUADRATIC_OUT, m_releaseTimer.getTime(), m_scrollYReleased, targetY - m_scrollYReleased, m_releaseTimer.getDuration());

							if (m_releaseTimer.update()) {
								m_releaseTimer.reset();
								m_scrollYSpeed = 0.0f;
								//m_page = m_pageTarget;
							}
						}
					} else {
						m_scrollY = m_scrollYReleased;
					}
				}
			 	else {
				//if (m_dragging) {
					m_scrollY = m_scrollYReleased;
					m_dragTimer += timer->getDelta();
				}
			}
			void VerticalScrollArea::preRender() {
			    GameContainer* container = ARK2D::getContainer();
				Renderer* r = ARK2D::getRenderer();
				int tx = ARK2D::getContainer()->getTranslateX();
				int ty = ARK2D::getContainer()->getTranslateY();
				float sc = ARK2D::getContainer()->getScale();

				recalculateHeight();

				r->pushMatrix();
				if (m_scissoring) {
					r->setScissorTestEnabled(true);
					r->scissor(tx + (m_x * sc), ty + (m_y * sc), m_width * sc, m_height * sc);
				}
			}
			void VerticalScrollArea::render() {

			}
			void VerticalScrollArea::postRender() {
				GameContainer* container = ARK2D::getContainer();
				Renderer* r = ARK2D::getRenderer();
				if (m_scissoring) {
			  		r->scissor(0,0,container->getDynamicWidth(), container->getDynamicHeight());
					r->setScissorTestEnabled(false);
				}
				r->popMatrix();
			}
			float VerticalScrollArea::getCurY() {
				return m_y - m_scrollY;
			}
			float VerticalScrollArea::getInsideTopDecorationY() {
				return m_y - ((m_scrollY > 0)?m_scrollY:0);
			}
			float VerticalScrollArea::getInsideBottomDecorationY() {
				return ceil(m_y - m_scrollY + m_height + m_innerHeight) + 
						((m_scrollY > m_innerHeight)?(m_scrollY - m_innerHeight):0);
			}
			bool VerticalScrollArea::keyPressed(unsigned int key) {
				bool collides = Shape<int>::collision_rectangleRectangle( m_x, m_y, m_width, m_height, ARK2D::getInput()->getMouseX(), ARK2D::getInput()->getMouseY(), 1, 1);
				if (!collides) { 
					return false; 
				}

				Input* in = ARK2D::getInput();
				if (key == (signed int) Input::MOUSE_BUTTON_LEFT) {
					if (!m_dragging) {
						m_dragging = true;
						m_draggingStartY = in->getMouseY();
						m_draggingStart.set(in->getMouseX(), in->getMouseY() + m_scrollY );
						m_scrollYReleased = m_scrollY;
					}
				}

				if (m_actualInnerHeight < m_height) {
					m_dragging = false;
					m_scrollYReleased = -100;
				}
				return true;
			}

			bool VerticalScrollArea::keyReleased(unsigned int key) {
				
				Input* in = ARK2D::getInput();
				if (key == (signed int) Input::MOUSE_BUTTON_LEFT) {
					if (m_dragging) {
						m_dragging = false;
						m_draggingEnd.set(in->getMouseX(), in->getMouseY());

						m_releaseTimer.reset();
						m_releaseTimer.update();

						if (m_dragTimer != 0) {
							m_scrollYSpeed = (m_draggingStartY - m_draggingEnd.getY()) / m_dragTimer;
						}
					}
				}

			 	if (m_actualInnerHeight < m_height) {
					m_dragging = false;
					m_scrollYReleased = -100;
				}
				return false;
			}
			bool VerticalScrollArea::mouseMoved(int x, int y, int oldx, int oldy) {
				
				if (m_dragging) {

					float smoothoffsety = m_draggingStart.getY() - m_scrollY - y;
					if (m_draggingStart.getY() - m_scrollY - y > m_dragMinimumRequired) { 
						smoothoffsety = m_dragMinimumRequired;
					} else if (m_draggingStart.getY() - m_scrollY - y < m_dragMinimumRequired * -1.0f) {
						smoothoffsety = (m_dragMinimumRequired * -1.0f);
					}
					
					m_scrollYReleased = (m_draggingStart.getY() - y - smoothoffsety);;//in->getMouseY() + m_scrollY;
					
					// limit top/bottom side
					if (m_scrollYReleased < m_scrollYUnderflow * -1.0f) {
						m_scrollYReleased = m_scrollYUnderflow * -1.0f;
					} else if (m_scrollYReleased > m_innerHeight + m_scrollYUnderflow) {
						m_scrollYReleased = m_innerHeight + m_scrollYUnderflow;
					}
					m_releaseTimer.reset();
					m_releaseTimer.update();

				}

			 	if (m_actualInnerHeight < m_height) {
					m_dragging = false;
					m_scrollYReleased = -100;
				}
				return false;
			}
			VerticalScrollArea::~VerticalScrollArea() {

			}

		}
	}
}
