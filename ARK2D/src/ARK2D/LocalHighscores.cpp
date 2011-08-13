/*
 * LocalHighscores.cpp
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#include "LocalHighscores.h"

#include "ARK2D.h"
#include "GameContainer.h"

LocalHighscores::LocalHighscores():
	filename(""),
	items()
	{
	filename = ARK2D::getContainer()->getResourcePath() + "local_highscores.dat";
}

vector<LocalHighscoreItem*> LocalHighscores::data() {
	return items;
}

void LocalHighscores::load() {
	char* data = StringUtil::file_get_contents(filename.c_str());
	if (data == NULL) {
		save();
		load();
	} else {
		string s = "";
		s.append(data);
		JSONNode* arr = libJSON::Parse(s);
		for(unsigned int i = 0; i < arr->NodeSize(); i++) {
			JSONNode* item = arr->NodeAt(i);

			LocalHighscoreItem* it = new LocalHighscoreItem();
			it->name = item->GetNode("name")->NodeAsString();
			it->score = item->GetNode("score")->NodeAsInt();
			items.push_back(it);
		}
		std::cout << "loaded local highscores" << std::endl;

		this->sort();
	}
}

void LocalHighscores::sort() {
	std::sort(items.begin(), items.end(), LocalHighscores::mysortcomparator);
	std::cout << "sorted local highscores" << std::endl;
}

bool LocalHighscores::mysortcomparator(LocalHighscoreItem* one, LocalHighscoreItem* two) {
	return (one->score > two->score);
}

void LocalHighscores::addItem(string name, int score) {
	LocalHighscoreItem* item = new LocalHighscoreItem();
	item->name = name;
	item->score = score;
	items.push_back(item);
}
void LocalHighscores::save() {

	// TODO: sort.

	string s = "[";
		for(unsigned int i = 0; i < items.size(); i++) {
			LocalHighscoreItem* it = items.at(i);
			s.append("{");
				s.append("\"name\": \"");
				s.append(it->name);
				s.append("\",");
				s.append("\"score\": ");
				s.append(Cast::toString<int>(it->score));
			s.append("}");
			if (i != items.size()-1) {
				s.append(",");
			}
		}
	s.append("]");

	bool success = FileUtil::file_put_contents(filename, s);
	if (success) {
		std::cout << "saved local highscores" << std::endl;
	} else {
		std::cout << "could not save local highscores" << std::endl;
	}
}

LocalHighscores::~LocalHighscores() {

}
