/*
 * LocalHighscores.cpp
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#include "KeyPairFile.h"

#include "../ARK2D.h"
#include "../Core/GameContainer.h"

namespace ARK {
	namespace Util {

		KeyPairFile::KeyPairFile(string filename): 
			ARK::Core::Resource(),
			m_filename(filename),
			m_data(NULL),
			m_map() {
			parse();
		}
		KeyPairFile::KeyPairFile(string filename, void* data): 
			ARK::Core::Resource(),
			m_filename(filename),
			m_data(data),
			m_map() {
			parse();
		}	

		string KeyPairFile::getFilename() {
			return m_filename;
		}

		void KeyPairFile::read(string data) {
			vector<string> lines = StringUtil::split(data, "\n");
			for(unsigned int i = 0; i < lines.size(); ++i) {
				string line = lines.at(i);
				int spaceIndex = line.find(" ");
				string key = line.substr(0, spaceIndex);
				string value = line.substr(spaceIndex+1);

				add(key, value);
			}
		}
		void KeyPairFile::add(string key, string val) {
			m_map[key] = val;
		}
		void KeyPairFile::add(string key, bool val) { 
			m_map[key] = Cast::boolToString(val);
		}
		void KeyPairFile::add(string key, unsigned int val) {
			m_map[key] = Cast::toString<unsigned int>(val);
		} 

		bool KeyPairFile::getBoolean(string key) {
			return Cast::fromString<bool>(m_map[key]);
		}
		bool KeyPairFile::getBoolean(string key, bool defaultValue) {
			if (m_map.find(key) == m_map.end()) {
				return defaultValue;
			}
			return getBoolean(key);
		}

		unsigned int KeyPairFile::getInteger(string key) {
			return Cast::fromString<unsigned int>(m_map[key]);
		}
		unsigned int KeyPairFile::getInteger(string key, unsigned int defaultValue) {
			if (m_map.find(key) == m_map.end()) {
				return defaultValue;
			}
			return Cast::fromString<unsigned int>(m_map[key]);
		}


		string KeyPairFile::getString(string key) {
			return m_map[key];
		}
		string KeyPairFile::getString(string key, string defaultValue) {
			if (m_map.find(key) == m_map.end()) {
				return defaultValue;
			}
			return m_map[key];
		}

		string KeyPairFile::toString() {
			string s = "";
			map<string, string>::const_iterator it = m_map.begin();
			while(it != m_map.end() ) {
				s += it->first;
				s += " ";
				s += it->second;
				it++; //make the iterator point to the next pair in the map

				if (it != m_map.end()) {
					s += "\n";
				}
			}
			return s;
		}
		
		void KeyPairFile::parse() {
			ARK2D::getLog()->v("Parsing KeyPairFile");

			string s;
			#if defined(ARK2D_ANDROID)  
				//if (m_threaded && m_data != NULL) { 
				if (m_data != NULL) { 
					s = string((char*) m_data);
				} else if (StringUtil::file_exists(m_filename.c_str())) {
					s = StringUtil::file_get_contents(m_filename.c_str());
				} else {
					s = string((char*) m_data);
				}
			#else

				if (m_data != NULL) {
					s = string((char*) m_data);
				} else {
					s = StringUtil::file_get_contents(m_filename.c_str());
				}

			#endif
			ARK2D::getLog()->v(StringUtil::append("String is: ", s));

			if (s.length() == 0) {
				ARK2D::getLog()->e("String was empty. Huh?");
				return;
			} 
 			
			read(s);

		}
		

		void KeyPairFile::save() {

			//! @todo: sort highscores.
			string s = toString();

			string usefilename = m_filename;
			#if defined(ARK2D_ANDROID) 
				bool useoldref = (m_filename.substr(0,7).compare("assets/") == 0);
				if (useoldref) { 
					usefilename = m_filename.substr(7, string::npos);
				}
			#endif

			bool success = FileUtil::file_put_contents(usefilename, s);
			if (success) {
				ARK2D::getLog()->v("KeyPairFile saved successfully!");
			} else {
				ARK2D::getLog()->v("KeyPairFile could not be saved!");
			}
		}
 
		KeyPairFile::~KeyPairFile() { 
			m_map.clear();
			ARK2D::getLog()->i("Deleting KeyPairFile");
		}	

	}
}
