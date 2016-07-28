/*
 * PropertyItem.h
 *
 *  Created on: Jun 26, 2012
 *      Author: ashleygwinnell
 */

#ifndef ARK_GJ_PROPERTY_ITEM_H_
#define ARK_GJ_PROPERTY_ITEM_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include <string>
#include <map>
using std::string;
using std::map;

namespace ARK {
	namespace GJ {

		class ARK2D_API PropertyItem {
			protected:
				map<string, string> m_properties;

			public:
				PropertyItem();
				void addProperty(string key, string val);
				string getProperty(string key) const;
				virtual ~PropertyItem();
		};

	}
}

#endif /* ARK_GJ_PROPERTY_ITEM_H_ */
