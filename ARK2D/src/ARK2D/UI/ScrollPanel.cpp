/*
 * ScrollPanel.cpp
 *
 *  Created on: 21 Jul 2011
 *      Author: Ashley
 */

#include "ScrollPanel.h"
#include "../Core/String.h"
#include "../Core/Graphics/Renderer.h"

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
			m_upButton.setBounds(15, 15 ,0);
			//m_upButton.setParent(this);
			Panel::addChild(&m_upButton);

			m_downButton.setEvent((void*) ScrollPanel::downButtonPressedStatic);
			m_downButton.setEventObj(this);
			m_downButton.setBounds(15, 15, 0);
			//m_downButton.setParent(this);
			Panel::addChild(&m_downButton);

			m_scrollYButton.setBounds(15, 15, 0);
			//m_scrollYButton.setParent(this);
			Panel::addChild(&m_scrollYButton);
		}

		void ScrollPanel::onChildAdded(SceneNode* newChild) {
			//calculateSize();
		}

		void ScrollPanel::calculateSize() {
			/*int currentX = 0;
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
			m_calculatedHeight = maxHeight;*/

			// need to take into account bounds of children.
			m_calculatedWidth = 0;
			m_calculatedHeight = 0;

			for (signed int i = children.size() - 1; i >= 0; i--) {
                ARK::Core::Geometry::Cube* bounds = children[i]->getBounds();
                float maxX = children[i]->transform.position.getX() + (bounds->getWidth()*(1.0f - children[i]->pivot.getX()));
                float maxY = children[i]->transform.position.getY() + (bounds->getHeight()*(1.0f - children[i]->pivot.getY()));
				if (maxX > m_calculatedWidth) {
                    m_calculatedWidth = maxX;
				}
				if (maxY > m_calculatedHeight) {
					m_calculatedHeight = maxY;
				}
			}
		}

		//void ScrollPanel::renderScrollbars() {
			
				
		//}

		void ScrollPanel::render() {
			if (!visible) { return; }



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

            ARK::Core::Geometry::Cube* bounds = getBounds();
            Renderer* r = ARK2D::getRenderer();

			int syh = (int) ((float(bounds->getHeight()-30) / float(m_calculatedHeight)) * bounds->getHeight());
			
			m_upButton.transform.position.set(bounds->getWidth()-15, 0);
			m_scrollYButton.setHeight(syh);
			m_scrollYButton.transform.position.set(bounds->getWidth()-15, 15); // set position based on m_offsetY;
			m_downButton.transform.position.set(bounds->getWidth()-15, bounds->getHeight()-15);

			if (m_layout == LAYOUT_FLOW) {

				preRenderFromPivot();

				//Panel::preRender();

				/*int cx = getPaddingLeft() + m_offsetX;
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

				}*/

				
                r->setDrawColor(Color::white);
				r->drawLine(bounds->getWidth()-15.0f, 0.0f, bounds->getWidth()-15.0f, bounds->getHeight()*1.0f);

				postRenderFromPivot();

				renderChildren();

				if (m_calculatedWidth >= (int) m_width) {

				}
				if (m_calculatedHeight >= (int) m_height) {
					// vertical scroll bars.
					//g->setDrawColor(Color::white);
					//g->fillRect(0, 0, 10, 10);
					//renderScrollbars();
				}

				//Panel::postRender();
			} else {

				// @TODO: this depends on panel layout used
				m_calculatedWidth = m_width;
				m_calculatedHeight = m_height;

				preRenderFromPivot();

				postRenderFromPivot();

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
			return false;
		}
		bool ScrollPanel::keyReleased(unsigned int key) {
			if (Panel::keyReleased(key)) return true;
			return false;
		}
		bool ScrollPanel::mouseMoved(int x, int y, int oldx, int oldy) {
			if (Panel::mouseMoved(x, y, oldx, oldy)) return true;
			return false;
		}
	}
}
