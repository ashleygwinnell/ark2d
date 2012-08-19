/*
 * StringStore.cpp
 *
 *  Created on: 29 Mar 2011
 *      Author: Ashley
 */

#include "StringStore.h"

namespace ARK {
	namespace Util {

		StringStore StringStore::s_stringStore;
		StringStore& StringStore::getInstance() {
			return s_stringStore;
		}

		StringStore::StringStore(): m_map() {

		}

		string StringStore::get(string key) {
			return m_map[key];
		}
		void StringStore::put(string key, string value) {
			m_map[key] = value;
			//m_map.insert( pair<string,string>(key, value) );
		}
		bool StringStore::exists(string key) {
			return m_map.count(key) > 0;
		}
		void StringStore::clear(string key) {
			m_map.erase(key);
		}
		void StringStore::clear() {
			m_map.clear();
		}

		void StringStore::printKeys() {
			map<string, string>::iterator it = m_map.begin();
			while(it != m_map.end() ) {
				std::cout << it->first;
				std::cout << ", ";
				//ARK2D::getLog()->i(String(it->first).append(", ").get());
				it++; //make the iterator point to the next pair in the map
			}
			std::cout << std::endl;
		}

		StringStore::~StringStore() {

		}

	}
}
