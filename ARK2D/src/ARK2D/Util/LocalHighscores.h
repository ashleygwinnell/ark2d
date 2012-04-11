/*
 * LocalHighscores.h
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#ifndef LOCALHIGHSCORES_H_
#define LOCALHIGHSCORES_H_

#include "../Includes.h"
#include "FileUtil.h"
#include "Cast.h"
#include "StringUtil.h"
#include <vector>
#include <string>
#include <algorithm>
#include "../Core/Resource.h"

using namespace std;

namespace ARK {
	namespace Util {
		class LocalHighscoreItem {
			public:
				string name;
				int score;
		};

		/*!
		 * \brief Locally stored highscores. Use Resource class to load and save these.
		 *
		 * They are stored as JSON.
		 *
		 * @see Resource
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class LocalHighscores : public ARK::Core::Resource {
			private:
				string m_filename;
				void* m_data;
				vector<LocalHighscoreItem*> m_items;
			public:
				LocalHighscores(string filename);
				LocalHighscores(string filename, void* data);
				void addItem(string name, int score);
				void save();
				vector<LocalHighscoreItem*> data();
				void sort();
				string getFilename();
				virtual ~LocalHighscores();

			private:
				static bool mysortcomparator(LocalHighscoreItem* one, LocalHighscoreItem* two);
				void parse();
		};
	}
}

#endif /* LOCALHIGHSCORES_H_ */
