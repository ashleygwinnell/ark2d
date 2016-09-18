/*
 * LocalHighscores.cpp
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#include "KeyPairFile.h"

#include "../Core/ARK2D.h"
#include "../Core/GameContainer.h"

namespace ARK {
	namespace Util {

		KeyPairFile::KeyPairFile(string filename):
			ARK::Core::Resource(),
			m_filename(filename),
			m_data(NULL),
			m_map(),
			m_mapUnsaved() {
			parse();
		}
		KeyPairFile::KeyPairFile(string filename, void* data):
			ARK::Core::Resource(),
			m_filename(filename),
			m_data(data),
			m_map(),
			m_mapUnsaved() {
			parse();
		}

		string KeyPairFile::getFilename() {
			return m_filename;
		}

		void KeyPairFile::read(string data) {

			vector<string> lines = StringUtil::split(data, "\n");
			for(unsigned int i = 0; i < lines.size(); ++i) {
				string line = lines.at(i);
				StringUtil::trim(line, " \r\n");

				if (line.length() == 0 || line.substr(0, 2) == "//") { continue; }

				int spaceIndex = line.find(" ");
				string key = line.substr(0, spaceIndex);
				string value = line.substr(spaceIndex+1);

				add(key, value);
			}
		}
		void KeyPairFile::add(string key, string val) {
			m_map[key] = val;
			m_mapUnsaved[key] = val;
		}
		void KeyPairFile::add(string key, bool val) {
			m_map[key] = Cast::boolToString(val);
			m_mapUnsaved[key] = Cast::boolToString(val);
		}
		void KeyPairFile::add(string key, unsigned int val) {
			m_map[key] = Cast::toString<unsigned int>(val);
			m_mapUnsaved[key] = Cast::toString<unsigned int>(val);
		}
		void KeyPairFile::add(string key, float val) {
			m_map[key] = Cast::toString<float>(val);
			m_mapUnsaved[key] = Cast::toString<float>(val);
		}

		// why not called set eh?
		void KeyPairFile::set(string key, string val) {
			m_map[key] = val;
			m_mapUnsaved[key] = val;
		}
		void KeyPairFile::set(string key, bool val) {
			m_map[key] = Cast::boolToString(val);
			m_mapUnsaved[key] = Cast::boolToString(val);
		}
		void KeyPairFile::set(string key, unsigned int val) {
			m_map[key] = Cast::toString<unsigned int>(val);
			m_mapUnsaved[key] = Cast::toString<unsigned int>(val);
		}
		void KeyPairFile::set(string key, float val) {
			m_map[key] = Cast::toString<float>(val);
			m_mapUnsaved[key] = Cast::toString<float>(val);
		}

		bool KeyPairFile::getBoolean(string key) {
			return Cast::boolFromString(m_map[key]);
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

		float KeyPairFile::getFloat(string key) {
			return Cast::fromString<float>(m_map[key]);
		}
		float KeyPairFile::getFloat(string key, float defaultValue) {
			if (m_map.find(key) == m_map.end()) {
				return defaultValue;
			}
			return Cast::fromString<float>(m_map[key]);
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

		void KeyPairFile::clear() {
			m_map.clear();
		}
		void KeyPairFile::clearUnsaved() {

			map<string, string>::const_iterator it = m_mapUnsaved.begin();
			while(it != m_mapUnsaved.end() ) {


				map<string, string>::iterator it2 = m_map.find(it->first);
				if (it2 != m_map.end()) {
					m_map.erase(it2);
				}
				it++;

			}

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
			/*#elif defined(ARK2D_IPHONE)
				if (m_data != NULL) {
					s = string((char*) m_data);
				} else {

					bool exists = Resource::get(m_filename);
					if (!exists) { FileUtil::file_put_contents(m_filename, ""); }

					s = StringUtil::file_get_contents(m_filename.c_str());
				}
			*/
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


		bool KeyPairFile::save() {

			m_mapUnsaved.clear();

			//! @todo: sort highscores.
			string s = toString();

			string usefilename = m_filename;
			#if defined(ARK2D_ANDROID)
				bool useoldref = (m_filename.substr(0,7).compare("assets/") == 0);
				if (useoldref) {
					usefilename = m_filename.substr(7, string::npos);
				}
			#elif defined(ARK2D_IPHONE)

				int findit = m_filename.find(".app/data/");
				bool useoldref = (findit != string::npos);
				if (useoldref) {
					string oldref = m_filename.substr(findit+10, string::npos);
					usefilename = oldref; //FileUtil::prependPlatform(oldref);
				}
				//bool usingGoodRef = (m_filename.substr(0,5).compare("data/") == 0);
				//if (!usingGoodRef) {
				//	usefilename = StringUtil::append("data/", m_filename);
				//}

			#endif
			ARK2D::getLog()->v(StringUtil::append("filename: ", m_filename));

			bool success = FileUtil::file_put_contents(usefilename, s);
			if (success) {
				ARK2D::getLog()->v("KeyPairFile saved successfully!");
			} else {
				ARK2D::getLog()->v("KeyPairFile could not be saved!");
			}
			return success;
		}

		KeyPairFile::~KeyPairFile() {
			m_map.clear();
			ARK2D::getLog()->i("Deleting KeyPairFile");
		}

	}
}
