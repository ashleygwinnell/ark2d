/*
 * LocalHighscores.h
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#ifndef LOCALHIGHSCORES_H_
#define LOCALHIGHSCORES_H_

#include "ARK2D_JSON.h"
#include "FileUtil.h"
#include "Cast.h"
#include "StringUtil.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
class LocalHighscoreItem {
	public:
		string name;
		int score;
};
class LocalHighscores {
	private:
		string filename;
		vector<LocalHighscoreItem*> items;
	public:
		LocalHighscores();
		void load();
		void addItem(string name, int score);
		void save();
		vector<LocalHighscoreItem*> data();
		void sort();
		virtual ~LocalHighscores();

	private:
		static bool mysortcomparator(LocalHighscoreItem* one, LocalHighscoreItem* two);
};

#endif /* LOCALHIGHSCORES_H_ */
