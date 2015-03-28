
#include "PropertyItem.h"

namespace ARK {
	namespace GJ {

		PropertyItem::PropertyItem(): m_properties() {

		}  

		void PropertyItem::addProperty(string key, string val) {
			m_properties[key] = val;
		} 
		string PropertyItem::getProperty(string key) const {
			return m_properties.find(key)->second;
		}
		  
		PropertyItem::~PropertyItem() {
			m_properties.clear();
		}

	}
}
