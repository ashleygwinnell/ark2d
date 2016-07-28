/*
 * Notification.h
 *
 *  Created on: 02 Sep 2012
 *      Author: Ashley
 */

#ifndef ARK_UI_NOTIFICATION_H_
#define ARK_UI_NOTIFICATION_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include <string>
using namespace std;

namespace ARK {
	namespace UI {

		class ARK2D_API Notification {
			
			public:
				Image* m_icon; 
				string m_title;
				string m_body;
				float m_timer;
				float m_duration;

				unsigned int m_type;
				static const unsigned int TYPE_NORMAL = 0;
				static const unsigned int TYPE_INFORMATION = 1;
				static const unsigned int TYPE_SUCCESS = 2;
				static const unsigned int TYPE_ERROR = 3;

			public:
				Notification();
				void update();
				void render();  
				virtual ~Notification();
		};

	}
}


#endif /* ARK_UI_NOTIFICATION_H_ */
