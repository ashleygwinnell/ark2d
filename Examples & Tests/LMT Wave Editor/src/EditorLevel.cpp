/*
 * EditorLevel.cpp
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#include "EditorLevel.h"
#include "DefaultGame.h"
#include "LevelEditorState.h"

EditorLevel::EditorLevel():
	ARKGameObject(),
	m_id(1),
	m_name(""),
	m_waves(),
	m_timer(0.0f),
	m_script()
{

}
void EditorLevel::update(GameContainer* container, GameTimer* timer) {

}
void EditorLevel::render(GameContainer* container, Graphics* g) {
	for(unsigned int i = 0; i < m_waves.size(); i++) {
		EditorWave* e = m_waves.get(i);
		e->m_timer = m_timer;
		e->render(container, g);

		if (e->m_mouseOver) {
			g->setDrawColor(Color::grey);
		} else {
			g->setDrawColor(Color::white);
		}
		g->drawRect(
				e->getMinX() -4,
				e->getMinY() -5,
				e->getMaxX() - e->getMinX() + 9,
				e->getMaxY() - e->getMinY() + 10);
	}
}
float EditorLevel::getDuration() {
	float longest = 0.0f;
	for(unsigned int i = 0; i < m_waves.size(); i++) {
		float f = m_waves.get(i)->getDuration();
		if (f > longest) {
			longest = f;
		}
	}
	return longest;
}
string EditorLevel::toString() {
	const string nl = "\n";
	string s = "";
	s += "{" + nl;
	s += "	\"id\": " + Cast::toString<int>(m_id) + "," + nl;
	s += "	\"name\": \"" + m_name + "\"," + nl;
	s += "	\"waves\": [" + nl;
	for(unsigned int i = 0; i < m_waves.size(); i++) {
		EditorWave* w = m_waves.get(i);
		s += "		{" + nl;
		s += "			\"file\": \"" + w->fname + "\"," + nl;
		s += "			\"offsetx\": " + Cast::toString<int>(w->m_offsetx) + "," + nl;
		s += "			\"offsety\": " + Cast::toString<int>(w->m_offsety) + "," + nl;
		s += "			\"flip_h\": " + string(w->m_flippedH?"true":"false") + "," + nl;
		s += "			\"flip_v\": " + string(w->m_flippedV?"true":"false") + "," + nl;
		s += "			\"delay\": " + Cast::toString<float>(w->m_delay)  + nl;
		if (i != m_waves.size()-1) {
			s += "		}," + nl;
		} else {
			s += "		}" + nl;
		}
	}
	s += "	]," + nl;

	string scr = m_script.get();
	scr = StringUtil::str_replace("\n", "\\n", scr);
	s += "	\"script\": \"" + scr + "\"" + nl;


	s += "}";
	return s;
}
EditorLevel::~EditorLevel() {

}

EditorLevel* EditorLevel::createFromString(string s) {
	EditorLevel* level = new EditorLevel();

	DefaultGame* game = DefaultGame::getInstance();

	JSONNode* root = libJSON::Parse(s);

	level->m_id = root->GetNode("id")->NodeAsInt();
	level->m_name = root->GetNode("name")->NodeAsString();

	JSONNode* waves = root->GetNode("waves");
	for(unsigned int i = 0; i < waves->NodeSize(); i++) {
		JSONNode* wave = waves->Children[i];

		string filename = wave->GetNode("file")->NodeAsString();
		string filename2 = game->m_levelState->pathDir(filename);
		//filename = game->m_levelState->pathDir(filename);

		string filecontents = StringUtil::file_get_contents(filename2.c_str());

		EditorWave* ewave = EditorWave::createFromString(filecontents);
		ewave->fname = filename;
		ewave->m_delay = wave->GetNode("delay")->NodeAsFloat();
		ewave->m_offsetx = wave->GetNode("offsetx")->NodeAsInt();
		ewave->m_offsety = wave->GetNode("offsety")->NodeAsInt();
		ewave->setFlipped(wave->GetNode("flip_h")->NodeAsBool(), wave->GetNode("flip_v")->NodeAsBool());

		level->m_waves.add(ewave);
	}
	string script = root->GetNode("script")->NodeAsString();
	level->m_script = StringUtil::str_replace("\\n", "\n", script);

	return level;
}
