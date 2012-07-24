/*
 * User.h
 *
 *  Created on: Jun 26, 2012
 *      Author: ashleygwinnell
 */

#ifndef ARK_GJ_USER_H_
#define ARK_GJ_USER_H_

#include "PropertyItem.h"

namespace ARK {
	namespace GJ {

		class User : public PropertyItem {
			public:
				static const unsigned int TYPE_USER = 0;
				static const unsigned int TYPE_DEVELOPER = 1;
				static const unsigned int TYPE_MODERATOR = 2;
				static const unsigned int TYPE_ADMIN = 3;

				static const unsigned int STATUS_ACTIVE = 0;
				static const unsigned int STATUS_BANNED = 0;

			public:
				User();
				string getName();
				string getToken();
				unsigned int getType();
				unsigned int getStatus();
				virtual ~User();
		};

	}
}

#endif /* ARK_GJ_USER_H_ */
