/*
 * LocalHighscores.h
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#ifndef ARK_UTIL_LOCALHIGHSCORES_H_
#define ARK_UTIL_LOCALHIGHSCORES_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "../Core/Util/SystemUtil.h"
#include "../Core/Cast.h"
#include "../Core/Util/StringUtil.h"
#include <vector>
#include <string>
#include <algorithm>
#include "../Core/Resource.h"

using namespace std;

namespace ARK {
	namespace Util {
		class ARK2D_API LocalHighscoreItem {
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
		class ARK2D_API LocalHighscores : public ARK::Core::Resource {
			private:
				string m_filename;
				void* m_data;
				vector<LocalHighscoreItem*> m_items;
				bool m_threaded;

				static bool s_isThreadedOverride;

			public:
				static LocalHighscores* createFromFile(string ref, bool appendPath = true);

			private:
				LocalHighscores(string filename);
				LocalHighscores(string filename, void* data);
			public:
				void addItem(string name, int score);
				void save();
				string saveString();
				vector<LocalHighscoreItem*> data();
				void set(vector<LocalHighscoreItem*> newarray);
				void sort();
				void clear();
				string getFilename();
				virtual ~LocalHighscores();

			private:
				static bool mysortcomparator(LocalHighscoreItem* one, LocalHighscoreItem* two);
				void parse();

			public:
				inline static bool isThreadedStatic() { return s_isThreadedOverride; }
				inline static void setThreadedStatic(bool b) { s_isThreadedOverride = b; }
		};
	}
}

#endif /* LOCALHIGHSCORES_H_ */
