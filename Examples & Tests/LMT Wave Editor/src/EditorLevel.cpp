/*
 * EditorLevel.cpp
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#include "EditorLevel.h"
#include "DefaultGame.h"

EditorLevel::EditorLevel():
	ARKGameObject(),
	m_waves(),
	m_timer(0.0f)
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
	s += "	]" + nl;
	s += "}";
	return s;
}
EditorLevel::~EditorLevel() {

}
