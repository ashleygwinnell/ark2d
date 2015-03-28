/*
 * Button.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "../Includes.h"
#include "../Namespaces.h"
#include "AbstractUIComponent.h"

#include "../Core/String.h"
#include "../Controls/Input.h"
#include "../Graphics/Renderer.h"
#include "../Geometry/GigaRectangle.h"

namespace ARK {
	namespace UI {
		/*!
		 * \brief A clickable button.
		 *
		 * @todo Improve rendering on mobile devices not using primitive geometry.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Button : public AbstractUIComponent {
			protected:
				String m_text;
				Image* m_image;
				unsigned int m_state; // 0 for mouse-off, 1 for mouse-over, 2 for mouse-down.
				void* m_event;
				void* m_eventObj;
			public:
				static const unsigned int STATE_OFF = 0;
				static const unsigned int STATE_OVER = 1;
				static const unsigned int STATE_DOWN = 2;

				Button():
					AbstractUIComponent(),
					m_text(""),
					m_image(NULL),
					m_state(0),
					m_event(NULL),
					m_eventObj(NULL)
				{

				}
				Button(string text):
					AbstractUIComponent(),
					m_text(text),
					m_image(NULL),
					m_state(0),
					m_event(NULL),
					m_eventObj(NULL)
				{

				}
				void setEvent(void* e) {
					m_event = e;
				}
				void setEventObj(void* o) {
					m_eventObj = o;
				}
				void* getEvent() {
					return m_event;
				}
				void* getEventObj() {
					return m_eventObj;
				}
				void setImage(Image* i) {
					m_image = i;
				}
				virtual void keyPressed(unsigned int key) {
					if (!m_enabled) { return; } 

					Input* i = ARK2D::getInput();
					if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT
						&& isPointerOver()) {
							m_state = STATE_DOWN;
							setFocussed(true);
					}
				}

				virtual void keyReleased(unsigned int key) {
					if (!m_enabled) { return; } 

					Input* i = ARK2D::getInput();
					if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {
						if (isPointerOver()) {
							if (m_state == STATE_DOWN) {
								m_state = STATE_OVER;
								doEvent();
							}
						} else {
							m_state = STATE_OFF;
							setFocussed(false);
						}
					}
				}
				virtual bool isPointerOver() {
					Input* i = ARK2D::getInput();
					return GigaRectangle<int>::s_contains(getOnScreenX(), getOnScreenY(), (signed int) (m_width), (signed int)(m_height), (signed int) (i->getMouseX()), (signed int) (i->getMouseY()));
				}

				virtual void doEvent() {
					if (m_event != NULL) {
						if (m_eventObj == NULL) {
							
							void (*pt)() = (void(*)()) m_event;
							//typedef void fnct();
							//fnct* pt = (fnct*) m_event;
							pt();
						} else {
							void (*pt)(void*) = (void(*)(void*)) m_event;
							pt(m_eventObj);
						}
					}
				}


				virtual void mouseMoved(int x, int y, int oldx, int oldy) {
					if (!m_enabled) { return; } 
					
					if (m_state == STATE_DOWN) { return; }
					if (GigaRectangle<int>::s_contains(getOnScreenX(), getOnScreenY(), m_width, m_height, x, y)) {
						m_state = STATE_OVER;
					} else {
						m_state = STATE_OFF;
					}
				}
				void setText(string s) {
					m_text.clear();
					m_text += s;
				}
				const String& getText() {
					return m_text;
				}
				void setState(unsigned int i) {
					m_state = i;
				}
				unsigned int getState() {
					return m_state;
				}
				virtual void render() {

					//AbstractUIComponent::preRender();

					Renderer* g = ARK2D::getRenderer();
					renderBackground();

					float renderTextX = m_x + (m_width/2) - (g->getFont()->getStringWidth(m_text.get())/2);
					float renderTextY = m_y + (m_height/2) - (g->getFont()->getLineHeight()/2);
					if (m_state == STATE_DOWN) {
						renderTextX += 2;
						renderTextY += 2;
					}
					renderText((int) renderTextX, (int) renderTextY);

					if (m_image != NULL) {
						int rx = int(m_x + (m_width/2)) - int(m_image->getWidth()/2);
						int ry = int(m_y + (m_height/2)) - int(m_image->getHeight()/2);
						renderImage(rx, ry);
					}

					renderOverlay();

					//AbstractUIComponent::postRender();
				}
				virtual void renderBackground() {
					Renderer* g = ARK2D::getRenderer();
					g->setDrawColor(Color::black_50a);
					g->fillRect(m_x, m_y, m_width, m_height);
				}
				virtual void renderText(int x, int y) {
					Renderer* g = ARK2D::getRenderer();
					g->setDrawColor(Color::white);
					g->drawString(m_text.get(), x, y);
				}
				virtual void renderImage(int x, int y) {
					m_image->draw(x, y);
				}
				virtual void renderOverlay() {
					Renderer* g = ARK2D::getRenderer();
					g->setDrawColor(Color::white);
					if (m_state == STATE_OVER || m_state == STATE_DOWN) {
						g->setDrawColor(Color::white_50a);
					}
					g->drawRect(m_x, m_y, m_width, m_height);
				}
		};
	}
}

#endif /* BUTTON_H_ */
