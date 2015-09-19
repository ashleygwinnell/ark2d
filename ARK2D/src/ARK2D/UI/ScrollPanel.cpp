/*
 * ScrollPanel.cpp
 *
 *  Created on: 21 Jul 2011
 *      Author: Ashley
 */

#include "ScrollPanel.h"

namespace ARK {
	namespace UI {
		ScrollPanel::ScrollPanel():
			Panel(),
			m_upButton(), m_downButton(), m_scrollYButton(),
			m_offsetX(0), m_offsetY(0),
			m_calculatedWidth(1), m_calculatedHeight(1),
			m_layout(LAYOUT_FLOW)
		{
			m_upButton.setEvent((void*) ScrollPanel::upButtonPressedStatic);
			m_upButton.setEventObj(this);
			m_upButton.setSize(15, 15);
			m_upButton.setParent(this);

			m_downButton.setEvent((void*) ScrollPanel::downButtonPressedStatic);
			m_downButton.setEventObj(this);
			m_downButton.setSize(15, 15);
			m_downButton.setParent(this);

			m_scrollYButton.setSize(15, 15);
			m_scrollYButton.setParent(this);
		}

		void ScrollPanel::onChildAdded(SceneNode* newChild) {
			calculateSize();
		}

		void ScrollPanel::calculateSize() {
			int currentX = 0;
			int currentY = 0;
			int maxWidth = 0;
			int maxHeight = 0;
			currentX += getPaddingLeft();
			currentY += getPaddingTop();

			bool startedNewLine = true;
			for(unsigned int i = 0; i < children.size(); i++)
			{
                AbstractUIComponent* current = (AbstractUIComponent*) children.at(i);
				if (startedNewLine) {
					currentY += current->getMarginTop();
					startedNewLine = false;
				}

				int newcx = currentX + current->getWidth() + current->getMarginRight();
				if (newcx > (signed int) m_width - getPaddingX()) {
					if (currentX > maxWidth) {
						maxWidth = currentX;
					}

					currentY += current->getHeight() + current->getMarginBottom();
					currentX = getPaddingLeft();
					startedNewLine = true;
				}

				currentX += current->getWidth() + current->getMarginRight();

				int newcy = currentY + current->getHeight() + current->getMarginBottom();
				if (newcy > maxHeight) {
					maxHeight = newcy;
				}
			}
			maxWidth += getPaddingRight();
			maxHeight += getPaddingBottom();

			m_calculatedWidth = maxWidth;
			m_calculatedHeight = maxHeight;
		}

		void ScrollPanel::renderScrollbars() {
			m_upButton.setLocation(m_width-15, 0);
			m_upButton.render();

			int syh = (int) ((float(m_height-30) / float(m_calculatedHeight)) * m_height);
			m_scrollYButton.setHeight(syh);
			m_scrollYButton.setLocation(m_width-15, 15); // set position based on m_offsetY;
			m_scrollYButton.render();

			m_downButton.setLocation(m_width-15, m_height-15);
			m_downButton.render();

			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::white);
			g->setLineWidth(1);
			g->drawLine(m_width-15, 0, m_width-15, m_height);
		}

		void ScrollPanel::render() {
			if (!m_visible) { return; }

			// buttons are down.
			if (m_downButton.getState() == Button::STATE_DOWN) {
				m_offsetY -= 3;
			} else if (m_upButton.getState() == Button::STATE_DOWN) {
				m_offsetY += 3;
			}

			// keep in bounds.
			if (m_offsetY >= 0) {
				m_offsetY = 0;
			} else if (m_offsetY < (signed int) (m_height) - m_calculatedHeight) {
				m_offsetY = (signed int) (m_height) - m_calculatedHeight;
			}

			if (m_layout == LAYOUT_FLOW) {
				//Panel::preRender();

				int cx = getPaddingLeft() + m_offsetX;
				int cy = getPaddingTop() + m_offsetY;
				//int maxWidth = 0;
				//int maxHeight = 0;
				bool startedNewLine = true;
				int startedNewLineAtIndex = 0;
				for(unsigned int i = 0; i < children.size(); i++)
				{
					AbstractUIComponent* c = (AbstractUIComponent*) children.at(i);
					cx += c->getMarginLeft();
					if (startedNewLine) {
						cy += c->getMarginTop();
						startedNewLine = false;
					}

					int newcx = cx + c->getWidth() + c->getMarginRight();
					if (newcx > (signed int) m_width - getPaddingX()) {
						//cy += c->getHeight() + c->getMarginBottom();

						// get the tallest item in the row...
						unsigned int tallestHeight = 0;
						AbstractUIComponent* tallestComponent = NULL;
						for(unsigned int j = startedNewLineAtIndex; j < i; j++) {
                            AbstractUIComponent* ch = (AbstractUIComponent*) children.at(j);
							if (ch->getHeight() + ch->getPaddingY() > tallestHeight) {
                                tallestComponent = ch;
							}
						}
						if (tallestComponent != NULL) {
							cy += tallestComponent->getHeight() + tallestComponent->getMarginBottom();
						} else {
							cy += c->getHeight() + c->getMarginBottom();
						}

						cx = getPaddingLeft();
						cx += c->getMarginLeft();
						cy += c->getMarginTop();
						startedNewLine = true;
						startedNewLineAtIndex = i;
					}
					c->setLocation(cx, cy);
					//c->render();

					cx += c->getWidth() + c->getMarginRight();

					if (startedNewLine) { cy -= c->getMarginTop(); }

				}

				renderChildren();

				if (m_calculatedWidth >= (int) m_width) {

				}
				if (m_calculatedHeight >= (int) m_height) {
					// vertical scroll bars.
					//g->setDrawColor(Color::white);
					//g->fillRect(0, 0, 10, 10);
					renderScrollbars();
				}

				//Panel::postRender();
			} else {

				// @TODO: this depends on panel layout used
				m_calculatedWidth = m_width;
				m_calculatedHeight = m_height;

				//Panel::preRender();
				renderChildren();
				//Panel::postRender();
			}
		}

		void ScrollPanel::upButtonPressedStatic(void* p) {
			ScrollPanel* scrollPanel = (ScrollPanel*) p;
			scrollPanel->upButtonPressed();
		}
		void ScrollPanel::upButtonPressed() {
			m_offsetY += 3;
			std::cout << "scroll up pressed" << std::endl;
		}

		void ScrollPanel::downButtonPressedStatic(void* p) {
			ScrollPanel* scrollPanel = (ScrollPanel*) p;
			scrollPanel->downButtonPressed();
		}
		void ScrollPanel::downButtonPressed() {
			std::cout << "scroll down pressed" << std::endl;
		}


		bool ScrollPanel::keyPressed(unsigned int key) {
            if (Panel::keyPressed(key)) return true;
			if (m_upButton.keyPressed(key)) return true;
			if (m_downButton.keyPressed(key)) return true;
			//if (m_scrollYButton.keyPressed(key)) return true;
            return false;
		}
		bool ScrollPanel::keyReleased(unsigned int key) {
			if (Panel::keyReleased(key)) return true;
			if (m_upButton.keyReleased(key)) return true;
			if (m_downButton.keyReleased(key)) return true;
			//if (m_scrollYButton.keyReleased(key);
            return false;
		}
		bool ScrollPanel::mouseMoved(int x, int y, int oldx, int oldy) {
			if (Panel::mouseMoved(x, y, oldx, oldy)) return true;
			if (m_upButton.mouseMoved(x, y, oldx, oldy)) return true;
			if (m_downButton.mouseMoved(x, y, oldx, oldy)) return true;
			//if (m_scrollYButton.mouseMoved(x, y, oldx, oldy)) return true;
            return false;
		}
	}
}
