/*
 * StringStore.h
 *
 *  Created on: 29 Mar 2011
 *      Author: Ashley
 */

#ifndef STRINGSTORE_H_
#define STRINGSTORE_H_
 
/*#include <map>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;*/

#include "../Includes.h"
#include "../Namespaces.h"

namespace ARK {
	namespace Util {

		/*!
		 * \brief Stores a bunch of strings by key. Useful as a cache for loading json text files one-time only.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API StringStore {
			private:
				static StringStore s_stringStore;
			public:
				static StringStore& getInstance();

			public:
				StringStore();
				string get(string key);
				void put(string key, string value);
				bool exists(string key);
				void clear(string key);
				void clear();
				void printKeys();
				virtual ~StringStore();

			private:
				map<string, string> m_map;

		};
	}
}

#endif /* STRINGSTORE_H_ */
