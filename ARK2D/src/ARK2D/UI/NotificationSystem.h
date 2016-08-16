/*
 * NotificationSystem.h
 *
 *  Created on: 02 Sep 2012
 *      Author: Ashley
 */

#ifndef ARK_CORE_NOTIFICATIONSYS_H_
#define ARK_CORE_NOTIFICATIONSYS_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "../Core/ARK2D.h"
#include "../Core/Vector.h"

namespace ARK {
	namespace UI {
		class Notification;

		class ARK2D_API NotificationSystem {

			private:
				static NotificationSystem* s_notificationSystem;
				static NotificationSystem* getInstance();

			private:
                ARK::Core::Vector<ARK::UI::Notification* > m_notifications;
				float m_defaultDuration;

			public:
				NotificationSystem();
				void add(ARK::UI::Notification* n);
				void normal(string title, string body);
				void info(string body);
				void success(string body);
				void error(string body);
				void update();
				void render();
				virtual ~NotificationSystem();
		};
	}
}

#endif /* ARK_CORE_NOTIFICATIONSYS_H_ */
