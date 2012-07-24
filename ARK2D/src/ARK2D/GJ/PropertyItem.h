/*
 * PropertyItem.h
 *
 *  Created on: Jun 26, 2012
 *      Author: ashleygwinnell
 */

#ifndef ARK_GJ_PROPERTY_ITEM_H_
#define ARK_GJ_PROPERTY_ITEM_H_

#include "../Includes.h"
#include "../Namespaces.h"

namespace ARK {
	namespace GJ {

		class PropertyItem {
			protected:
				map<string, string> m_properties;

			public:
				PropertyItem();
				void addProperty(string key, string val);
				string getProperty(string key);
				virtual ~PropertyItem();
		};

	}
}

#endif /* ARK_GJ_PROPERTY_ITEM_H_ */
