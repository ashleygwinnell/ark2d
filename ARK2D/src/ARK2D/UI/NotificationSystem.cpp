/*
 * NotificationSystem.cpp
 *
 *  Created on: 02 Sep 2012
 *      Author: Ashley
 */

#include "Notification.h"
#include "NotificationSystem.h"

  
namespace ARK {
	namespace UI {

		NotificationSystem* NotificationSystem::s_notificationSystem = NULL;
		NotificationSystem* NotificationSystem::getInstance() {
			if (s_notificationSystem == NULL) {
				s_notificationSystem = new NotificationSystem(); 
			}
			return s_notificationSystem;
		}


		NotificationSystem::NotificationSystem():  
			m_notifications(),
			m_defaultDuration(2.0f)
		{ 
			
		}

		void NotificationSystem::add(ARK::UI::Notification* n) {
			m_notifications.add(n);
		}
		void NotificationSystem::normal(string title, string body) {
			Notification* n = new Notification();
			n->m_icon = NULL;
			n->m_title = title;
			n->m_body = body;
			n->m_timer = 0.0f;
			n->m_duration = m_defaultDuration;
			n->m_type = Notification::TYPE_NORMAL;
			m_notifications.add(n);
		}
		void NotificationSystem::info(string body) {
			Notification* n = new Notification();
			n->m_icon = NULL;
			n->m_title = "Information";
			n->m_body = body;
			n->m_timer = 0.0f;
			n->m_duration = m_defaultDuration;
			n->m_type = Notification::TYPE_INFORMATION;  
			m_notifications.add(n);
		}
		void NotificationSystem::success(string body) {
			Notification* n = new Notification();
			n->m_icon = NULL;
			n->m_title = "Huzzah!";
			n->m_body = body; 
			n->m_timer = 0.0f;  
			n->m_duration = m_defaultDuration; 
			n->m_type = Notification::TYPE_SUCCESS;
			m_notifications.add(n);
		}
		void NotificationSystem::error(string body) {
			Notification* n = new Notification(); 
			n->m_icon = NULL;
			n->m_title = "Error!";
			n->m_body = body;
			n->m_timer = 0.0f;
			n->m_duration = m_defaultDuration;
			n->m_type = Notification::TYPE_ERROR;
			m_notifications.add(n);
		}

		void NotificationSystem::update() {

		}
		void NotificationSystem::render() {

		}

		NotificationSystem::~NotificationSystem() {
			
		}
	}
}


