/*
 * Camera.cpp
 *
 *  Created on: 02 Sep 2012
 *      Author: Ashley
 */

#include "Camera.h"

namespace ARK {
	namespace Core {

		Camera::Camera(int w, int h): m_rect(NULL) 
		{
			m_rect = new ARK::Core::Rectangle<int>(0, 0, w, h);
		}

		void Camera::reposition() {
			
		}

		void Camera::setX(int x) {
			m_rect->setX(x);
		}
		void Camera::setY(int y) {
			m_rect->setY(y);
		}
		void Camera::setViewRectangle(int x, int y, int w, int h) {
			m_rect->setLocation(x, y);
			m_rect->setSize(w, h);
		}

		Camera::~Camera() {
			delete m_rect;
		}
	}
}
