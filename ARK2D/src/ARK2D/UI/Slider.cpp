/*
 * Slider.cpp
 *
 *  Created on: 16 Apr 2014
 *      Author: Ashley
 */

#include "Slider.h"
#include "../Tween/Easing.h"
#include "../Geometry/Shape.h"
#include "../Graphics/Image.h"

namespace ARK {
	namespace UI {

		Slider::Slider():
			AbstractUIComponent(),
			m_buttonLocation(0, 0),

			bar(NULL),
			button(NULL),

			m_value(0.0f),
			m_actualValue(0.0f),
			lowValue(0.0f),
			highValue(1.0f),

			m_dragging(false),
			m_alpha(1.0f),

			m_snapping(false),
			m_snapTo(0.0f)
		{
			setBounds(40, 0, 240, 8);
		}

		void Slider::setRange(float low, float high) {
			lowValue = low;
			highValue = high;
		}

		void Slider::updateValue()
		{
			float offsetX = 0;
			float offsetY = 0;
			if ( parent == NULL ) {
				offsetX += transform.position.getX();
				offsetY += transform.position.getY();
			}

			float intermediary = Easing::ease(Easing::LINEAR, m_buttonLocation.getX()+offsetX, 0.0f, 1.0f, m_width+offsetX);
			m_value = intermediary;

			if (m_snapping) {
				signed int divisions = round(intermediary / m_snapTo);
				m_value = divisions * m_snapTo;
			}

			m_actualValue = lowValue + (m_value * highValue);
		}

		void Slider::setButtonPosition(float percentage) {
			m_value = percentage;
			m_actualValue = lowValue + (m_value * highValue);

			refreshButtonPosition();
		}
		void Slider::refreshButtonPosition() {

			float offsetX = 0;
			float offsetY = 0;
			if ( parent == NULL ) {
				offsetX += transform.position.getX();
				offsetY += transform.position.getY();
			}

            float val = m_value * getWidth();
			m_buttonLocation.set(offsetX + val, offsetY);
			//m_buttonLocation.setY(getHeight()*0.5f);
		}
		void Slider::setBarImage(Image* img)
		{
			bar = img;
		}
		void Slider::setButtonImage(Image* img)
		{
			button = img;
		}
		void Slider::setBounds(float x, float y, float w, float h)
		{
			setSize(w, h);
			setLocation(x, y);

			refreshButtonPosition();
		}
		void Slider::setLocation(float x, float y) {
			AbstractUIComponent::setLocation(x, y);
			refreshButtonPosition();
		}
		void Slider::setLocationByCenter(float x, float y) {
			AbstractUIComponent::setLocationByCenter(x, y);
			refreshButtonPosition();
		}
		void Slider::render()
		{
			Renderer* r = ARK2D::getRenderer();

			float offsetX = 0;
			float offsetY = 0;
			if ( parent == NULL ) {
				offsetX += transform.position.getX();
				offsetY += transform.position.getY();
			}


			if (bar != NULL) {
				bar->setAlpha(m_alpha);
				bar->drawCentered( offsetX + (m_width*0.5f), offsetY );
			} else {
				r->setDrawColor(Color::white);
				r->fillRect( offsetX, offsetY, m_width, m_height );
			}

			//r->drawString(m_buttonLocation.toString(), 0, 0);

			if (button != NULL) {
				button->setAlpha(m_alpha);
				button->drawCentered(m_buttonLocation.getX(), m_buttonLocation.getY());
			} else {
				r->setDrawColor(Color::white_50a);
				r->fillCircle(m_buttonLocation.getX(), m_buttonLocation.getY(), 10, 20);
				r->setLineWidth(2);
				r->drawCircle(m_buttonLocation.getX(), m_buttonLocation.getY(), 10, 20);
				r->setLineWidth(1);
			}
		 }

		bool Slider::keyPressed(unsigned int key)
		{
			if (key != (unsigned int) Input::MOUSE_BUTTON_LEFT) { return false; }

			Input* i = ARK2D::getInput();
			if (!m_dragging)
			{
				Vector3<float> worldpos = localPositionToGlobalPosition();
				bool collides = Shape<float>::collision_circleCircle(i->getMouseX(), i->getMouseY(), 15.0f, worldpos.x + m_buttonLocation.getX(), worldpos.y + m_buttonLocation.getY(), 15.0f);
				if (collides) {
					m_dragging = true;
					updateValue();
                    return true;
				}
			}
            return false;
		}

		bool Slider::keyReleased(unsigned int key)
		{
			if (key != (unsigned int) Input::MOUSE_BUTTON_LEFT) { return false; }

			m_dragging = false;
			updateValue();
            return false;
		}

		bool Slider::mouseMoved(int x, int y, int oldx, int oldy)
		{
			if (m_dragging)
			{

				int newx = x;
				if (m_snapping) {
					int snapx = (m_width) * m_snapTo;
					signed int divisions = round(newx / snapx);
					newx = divisions * snapx;
				}

				if (newx < 0.0) { newx = (int) 0; }
				else if (newx > m_width) { newx = (int) m_width; }

				m_buttonLocation.setX(newx);

				updateValue();
                return true;
			}
            return false;
		}

		Slider::~Slider()
		{

		}

	}
}




