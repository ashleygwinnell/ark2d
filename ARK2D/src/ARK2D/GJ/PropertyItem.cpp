
#include "PropertyItem.h"

namespace ARK {
	namespace GJ {

		PropertyItem::PropertyItem(): m_properties() {

		}  

		void PropertyItem::addProperty(string key, string val) {
			m_properties[key] = val;
		} 
		string PropertyItem::getProperty(string key) {
			return m_properties[key];
		}
		 
		PropertyItem::~PropertyItem() {
			m_properties.clear();
		}

	}
}
