
#include "DataStore.h"
#include "../Includes.h"
#include "../Util/Cast.h"

namespace ARK {
	namespace GJ {

		DataStore::DataStore(string key, string val, unsigned int type): PropertyItem() {
			m_properties["key"] = key;
			m_properties["value"] = val;
			m_properties["type"] = Cast::toString<unsigned int>(type);
		}
		string DataStore::getKey() { 
			return m_properties["key"];
		}
		string DataStore::getValue() {
			return m_properties["value"];
		}
		unsigned int DataStore::getType() {
			return Cast::fromString<unsigned int>(m_properties["type"]);
		}
		DataStore::~DataStore() {
			m_properties.clear();
		}
	}
}
