/*
 * Button.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */
 
#include "Button.h"

#include "../Core/String.h"
#include "../Controls/Input.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Image.h"


namespace ARK {
	namespace UI {
		Button::Button():
			AbstractUIComponent(),
			m_text(""),
			m_image(NULL),
			m_event(NULL),
			m_eventObj(NULL)
		{
			
		}
		Button::Button(string text):
			AbstractUIComponent(),
			m_text(text),
			m_image(NULL),
			m_event(NULL),
			m_eventObj(NULL)
		{

		}
		void Button::setEvent(void* e) {
			m_event = e;
		}
		void Button::setEventObj(void* o) {
			m_eventObj = o;
		}
		void* Button::getEvent() {
			return m_event;
		}
		void* Button::getEventObj() {
			return m_eventObj;
		}
		void Button::setImage(Image* i) {
			m_image = i;
		}
		bool Button::keyPressed(unsigned int key) {
			if (!m_enabled) { return false; }

			Input* i = ARK2D::getInput();
			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT
				&& isPointerOver()) {
					m_state = STATE_DOWN;
					setFocussed(true);
					return true;

					//ARK2D::getLog()->e("set state to down");
			}
            return false;
		}

		bool Button::keyReleased(unsigned int key) {
			if (!m_enabled) { return false; }

			Input* i = ARK2D::getInput();
			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {
				//ARK2D::getLog()->e("released");
				if (isPointerOver()) {
					if (m_state == STATE_DOWN) {
						m_state = STATE_OVER;
						doEvent();
						//ARK2D::getLog()->e("doEvent");
						return true;
					}
					//ARK2D::getLog()->e(StringUtil::append("state: ", m_state));
				} else {
					m_state = STATE_OFF;
					setFocussed(false);
					//ARK2D::getLog()->e("pointer wasn't over..?");
				}
			}
            return false;
		}
		bool Button::isPointerOver() {
			Input* i = ARK2D::getInput();
			//Vector3<float> worldpos = localPositionToGlobalPosition();
			return isGlobalPositionInBounds(i->getMouseX(), i->getMouseY());// GigaRectangle<int>::s_contains(worldpos.getX(), worldpos.getY(), (signed int) (m_width), (signed int)(m_height), (signed int) (i->getMouseX()), (signed int) (i->getMouseY()));
		}

		void Button::doEvent() {
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


		bool Button::mouseMoved(int x, int y, int oldx, int oldy) {
			return AbstractUIComponent::mouseMoved(x, y, oldx, oldy);
		}
		void Button::setText(string s) {
			m_text.clear();
			m_text += s;
		}
		const String& Button::getText() {
			return m_text;
		}
		
		void Button::render() {
			if (!visible) { return; }

			if (parent == NULL) { preRender(); }
			preRenderFromPivot();

			renderBackground();

			float renderTextX = (m_width/2);// - (g->getFont()->getStringWidth(m_text.get())/2);
			float renderTextY = (m_height/2);// - (g->getFont()->getLineHeight()/2);
			if (m_state == STATE_DOWN) {
				renderTextX += 2;
				renderTextY += 2;
			}
			renderText((int) renderTextX, (int) renderTextY);

			if (m_image != NULL) {
				int rx = int( (m_width/2)) - int(m_image->getWidth()/2);
				int ry = int( (m_height/2)) - int(m_image->getHeight()/2);
				renderImage(rx, ry);
			}

			renderOverlay();
			postRenderFromPivot();

			if (parent == NULL) { postRender(); }

			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::white);
		}
		void Button::renderBackground() {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::black_50a);
			g->fillRect(0, 0, m_width, m_height);
		}
		void Button::renderText(int x, int y) { 
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::white);
			g->drawString(m_text.get(), x, y, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER);
		}
		void Button::renderImage(int x, int y) {
			m_image->draw(x, y); 
		}
		void Button::renderOverlay() {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::white);
			if (m_state == STATE_OVER || m_state == STATE_DOWN) {
				g->setDrawColor(Color::white_50a);
			}
			g->drawRect(0, 0, m_width, m_height);
		}
	}
}
