/*
 * LocalHighscores.cpp
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#include "LocalHighscores.h"

#include "../ARK2D.h"
#include "../Core/GameContainer.h"

namespace ARK {
	namespace Util {

		bool LocalHighscores::s_isThreadedOverride = false;

		LocalHighscores::LocalHighscores(string filename):
			ARK::Core::Resource(),
			m_filename(filename),
			m_data(NULL),
			m_items(),
			m_threaded(false)
			{
			m_threaded = LocalHighscores::s_isThreadedOverride; 
			parse();
		}
		LocalHighscores::LocalHighscores(string filename, void* data):
			ARK::Core::Resource(),
			m_filename(filename),
			m_data(data),
			m_items(),
			m_threaded(false)
			{
			m_threaded = LocalHighscores::s_isThreadedOverride; 
			parse();  
		}

		vector<LocalHighscoreItem*> LocalHighscores::data() {
			return m_items;
		}
		void LocalHighscores::set(vector<LocalHighscoreItem*> newarray) {
			m_items = newarray;
		}

		string LocalHighscores::getFilename() {
			return m_filename;
		}
		void LocalHighscores::parse() {
			ARK2D::getLog()->v("Parsing LocalHighscores");

			string s;
			#if defined(ARK2D_ANDROID)  
				if (m_threaded && m_data != NULL) { 
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
			ARK2D::getLog()->v(StringUtil::append("JSON String is: ", s));

			if (s.length() == 0) {
				ARK2D::getLog()->e("JSON String was empty. Huh?");
				return;
			} 

			if (s == "[]") { 
				ARK2D::getLog()->v("Local highscores were blank. That's fine. ");
			} else {
	 
				JSONNode* arr = libJSON::Parse(s);
				if (arr == NULL) {
					ARK2D::getLog()->e("Could not parse json");
					return;
				}
				for(unsigned int i = 0; i < arr->NodeSize(); i++) { 
					JSONNode* item = arr->NodeAt(i);

					LocalHighscoreItem* it = new LocalHighscoreItem();
					it->name = item->GetNode("name")->NodeAsString();
					it->score = item->GetNode("score")->NodeAsInt();
	 
					//StringUtil::str_replace("\"", "\\\"", it->name); 

					m_items.push_back(it);
				}
				ARK2D::getLog()->i("loaded local highscores");

				// fix memory leaks
				ARK2D::getLog()->i("Freeing up memory.");
				libJSON::Delete(arr);

			}

			this->sort();
		}
		/*void LocalHighscores::load() {
			char* data = StringUtil::file_get_contents(filename.c_str());
			if (data == NULL) {
				save();
				load();
			} else {
				string s = "";
				s.append(data);

			}
		}*/

		void LocalHighscores::sort() {
			std::sort(m_items.begin(), m_items.end(), LocalHighscores::mysortcomparator);
			//std::cout << "sorted local highscores" << std::endl;
		}

		bool LocalHighscores::mysortcomparator(LocalHighscoreItem* one, LocalHighscoreItem* two) {
			return (one->score > two->score);
		}

		void LocalHighscores::addItem(string name, int score) {
			LocalHighscoreItem* item = new LocalHighscoreItem();
			item->name = name;
			item->score = score;
			m_items.push_back(item);
		}
		void LocalHighscores::clear() {
			ARK2D::getLog()->i("Clearing LocalHighscores");
			for (unsigned int i = 0; i < m_items.size(); ++i) { 
				LocalHighscoreItem* item = m_items.at(i);
				delete item;
			}
			m_items.clear();
		}

		string LocalHighscores::saveString() { 
			string s = "[";
				for(unsigned int i = 0; i < m_items.size(); i++) {
					LocalHighscoreItem* it = m_items.at(i);
					s.append("{");
						s.append("\"name\": \"");
						s.append(it->name);
						s.append("\",");
						s.append("\"score\": ");
						s.append(Cast::toString<int>(it->score));
					s.append("}");
					if (i != m_items.size()-1) {
						s.append(",");
					}
				}
			s.append("]");
			return s;
		}

		void LocalHighscores::save() {

			//! @todo: sort highscores.

			string s = saveString();

			string usefilename = m_filename;
			#if defined(ARK2D_ANDROID) 
				bool useoldref = (m_filename.substr(0,7).compare("assets/") == 0);
				if (useoldref) { 
					usefilename = m_filename.substr(7, string::npos);
				}
			#endif

			bool success = FileUtil::file_put_contents(usefilename, s);
			if (success) {
				std::cout << "saved local highscores" << std::endl;
			} else {
				std::cout << "could not save local highscores" << std::endl;
			}
		}
 
		LocalHighscores::~LocalHighscores() { 
			clear();
			ARK2D::getLog()->i("Deleting LocalHighscores");
			
		}	

	}
}
