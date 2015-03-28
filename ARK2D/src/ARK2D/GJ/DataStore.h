/*
 * DataStore.h
 *
 *  Created on: Jun 26, 2012
 *      Author: ashleygwinnell
 */

#ifndef ARK_GJ_DATASTORE_H_
#define ARK_GJ_DATASTORE_H_

#include "PropertyItem.h"

namespace ARK {
	namespace GJ {

		class ARK2D_API DataStore : public PropertyItem {
			public:
				static const unsigned int TYPE_USER = 0;
				static const unsigned int TYPE_GAME = 1;

			public:
				DataStore(string key, string val, unsigned int type);
				string getKey();
				string getValue();
				unsigned int getType();
				virtual ~DataStore();
		};
	}
}

#endif /* ARK_GJ_DATASTORE_H_ */
