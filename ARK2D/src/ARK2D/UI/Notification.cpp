/*
 * UIComponent.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#include "Notification.h"

namespace ARK { 
	namespace UI {

		Notification::Notification():
			m_icon(NULL),
			m_title(""),
			m_body(""),
			m_timer(0.0f),
			m_duration(1.0f),
			m_type(TYPE_NORMAL)
			{

		}
		void Notification::update() {

		}
		void Notification::render() {

		} 
		Notification::~Notification() {

		}

	}
}


