/*
 * ComboBox.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#include "ComboBox.h"
#include "ComboBoxItem.h"
#include "Button.h"

namespace ARK {
	namespace UI {
		ComboBox::ComboBox():
			AbstractUIComponent(),
			m_items(),
			m_selected(NULL),
			m_open(false),
			m_state(Button::STATE_OFF),
			m_itemChangedEvent(NULL)
			{

		}
		void ComboBox::addItem(ComboBoxItem* cbi) {
			if (m_items.size() == 0) {
				m_selected = cbi;
			}
			cbi->parent = this;
			m_items.push_back(cbi);

			addChild(cbi);
		}
		void ComboBox::setOpen(bool b) {
			m_open = b;

			if (m_open) {
				int newHeight = m_height;
				for(unsigned int i = 0; i < m_items.size(); i++) {
					ComboBoxItem* item = m_items.at(i);
					item->setSize(m_width, ARK2D::getRenderer()->getFont()->getLineHeight());
					item->m_parent = m_parent;
					newHeight += item->getHeight();
				}
				m_height = newHeight;
			} else {
				m_height = m_originalHeight;

				// state state to off for all children
				for(unsigned int i = 0; i < m_items.size(); i++) {
					m_items[i]->m_state = Button::STATE_OFF;
				}
			}
		}
		bool ComboBox::isOpen() {
			return m_open;
		}
		ComboBoxItem* ComboBox::getSelected() {
			return m_selected;
		}
		void ComboBox::setSelectedByValue(string value) {
			for(unsigned int i = 0; i < m_items.size(); i++) {
				if (value == m_items.at(i)->m_value) {
					m_selected = m_items.at(i);
					break;
				}
			}
		}

		void ComboBox::setItemChangedEvent(void* event) {
			m_itemChangedEvent = event;
		}
		void ComboBox::setSize(unsigned int w, unsigned int h) {
			AbstractUIComponent::setSize(w, h);
			m_originalHeight = h;
		}

		bool ComboBox::keyPressed(unsigned int key) {
			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {

				if (m_state == Button::STATE_OVER) {
                    m_state = Button::STATE_DOWN;
				} 
                
                if (m_open) {
                	bool consumed = false;
                    for(unsigned int i = 0; i < m_items.size(); i++) {
                        if (m_items.at(i)->keyPressed(key)) { consumed = true; } 
                    }
                    if (consumed) return consumed;
                }
                return m_state == Button::STATE_DOWN;
			}
			return false;
            

		}
		bool ComboBox::keyReleased(unsigned int key) {
            if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {

                Input* in = ARK2D::getInput();
                bool mib = isGlobalPositionInBounds(in->getMouseX(), in->getMouseY());
            	if (mib && m_state == Button::STATE_DOWN) {

            		bool consumed = false;
            		if (m_open) {
						
	        			for(unsigned int i = 0; i < m_items.size(); i++) {
	                	    if (m_items.at(i)->keyReleased(key)) { 
	                	    	consumed = true;
	                	    }
	                	}
	                	
	                }

					setOpen(!isOpen());
					m_state = (mib)?Button::STATE_OVER:Button::STATE_OFF;
					if (consumed) {
						return consumed;
					}
				} else {
					setOpen(false);
					m_state = Button::STATE_OFF;
				}
				
			}
            return false;
 
		}
		bool ComboBox::mouseMoved(int x, int y, int oldx, int oldy) {
            Input* in = ARK2D::getInput();
            bool mib = isGlobalPositionInBounds(x, y);
	//		ARK2D::getLog()->e(StringUtil::append("mib: ", mib));
	//		ARK2D::getLog()->e(StringUtil::append("m_state: ", m_state));

			if (m_state != Button::STATE_DOWN) { 
				if (mib) {
					m_state = Button::STATE_OVER;
				} else {
					m_state = Button::STATE_OFF;
				}
			}
			if (m_open) {
				bool consumed = false;
				for(unsigned int i = 0; i < m_items.size(); i++) {
					if (m_items.at(i)->mouseMoved(x,y,oldx,oldy)) {
						consumed = true;
						//break;
					}
				}
				if (consumed) return consumed;;
			}
            return (m_state == Button::STATE_OVER || m_state == Button::STATE_DOWN);
		}
		
		void ComboBox::render() {
			renderBackground();
			renderArrow();
			renderOverlay();

			Renderer* g = ARK2D::getRenderer();
			if (m_selected != NULL) {
				float tx = 0;
				float ty = 0;
				if (m_state == Button::STATE_DOWN || m_open) {
					tx += 2;
					ty += 2;
				}
				g->drawString(m_selected->m_text, tx, ty);
			}

			if (m_open) {
				int y = 0 + m_originalHeight;
				for(unsigned int i = 0; i < m_items.size(); i++) {
					m_items.at(i)->setLocation(0, y);
					y += m_items.at(i)->getHeight();
				}
				renderChildren();
			}
		}
		void ComboBox::renderBackground() {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::black_50a);
			g->fillRect(0, 0, m_width, m_height);
		}
		void ComboBox::renderArrow() {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::white_50a);
			if (m_open) {
				g->fillTriangle(0 + m_width - 20, 0+10, 10, 10);
			} else {
				g->fillTriangle(0 + m_width - 20, 0+20, 10, -10);
			}

		}
		void ComboBox::renderText(int x, int y) {

		}
		void ComboBox::renderOverlay() {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::white);
			if (m_state == Button::STATE_OVER || m_state == Button::STATE_DOWN || m_open) {
				g->setDrawColor(Color::white_50a);
			}
			g->drawRect(0, 0, m_width, m_height);
		}
	}
}
