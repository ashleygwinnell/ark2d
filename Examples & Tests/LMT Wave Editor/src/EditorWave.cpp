/*
 * EditorWave.cpp
 *
 *  Created on: 23 Jul 2011
 *      Author: Ashley
 */

#include "EditorWave.h"
#include "DefaultGame.h"
#include "WaveEditorState.h"

EditorWave::EditorWave():
	ARKGameObject(),
	fname(""),
	m_enemies(),
	m_offsetx(0),
	m_offsety(0),
	m_delay(0.0f),
	m_timer(0.0f),
	m_flippedH(false),
	m_flippedV(false),
	m_mouseOver(false)
{

}
void EditorWave::update(GameContainer* container, GameTimer* timer) {

}
void EditorWave::render(GameContainer* container, Graphics* g) {
	float thisTimer = m_timer;
	if (thisTimer > m_delay) {
		thisTimer -= m_delay;
	} else {
		thisTimer = 0;
	}
	for(unsigned int i = 0; i < m_enemies.size(); i++) {
		m_enemies.get(i)->m_timer = thisTimer;
	}

	g->pushMatrix();
	g->translate(m_offsetx, m_offsety);
	m_enemies.renderAll(container, g);
	g->popMatrix();
}

void EditorWave::renderPaths() {
	Graphics* g = ARK2D::getGraphics();
	for(unsigned int i = 0; i < m_enemies.size(); i++) {
		EditorEnemy* e = m_enemies.get(i);
		if (e->m_pathGroup != NULL) {
			g->pushMatrix();
			g->translate(e->m_cx + m_offsetx, e->m_cy + m_offsety);
			e->m_pathGroup->renderCurve();
			g->popMatrix();
		}
	}
}

EditorWave* EditorWave::createFromString(string s) {
	EditorWave* wave = new EditorWave();

	DefaultGame* game = DefaultGame::getInstance();

	JSONNode* root = libJSON::Parse(s);
	JSONNode* enemies = root->GetNode("enemies");
	for(unsigned int i = 0; i < enemies->NodeSize(); i++) {

		JSONNode* enemy = enemies->Children[i];

		EditorEnemy* enemyObj = new EditorEnemy();
		enemyObj->m_id = enemy->GetNode("id")->NodeAsInt();
		enemyObj->m_cx = enemy->GetNode("x")->NodeAsInt();
		enemyObj->m_cy = enemy->GetNode("y")->NodeAsInt();
		wave->m_enemies.add(enemyObj);

		JSONNode* path = enemy->GetNode("path");
		enemyObj->m_path = path->GetNode("file")->NodeAsString();
		enemyObj->m_delay = path->GetNode("delay")->NodeAsFloat();

		#ifdef ARK2D_WINDOWS
			StringUtil::str_replace("\\", "\\\\", enemyObj->m_path);
		#elif defined(ARK2D_MACINTOSH)
			StringUtil::str_replace("\\", "/", enemyObj->m_path);
		#endif

		if (path->GetNode("speed_modifier") == NULL) {
			enemyObj->m_speedModifier = 1.0f;
		} else {
			enemyObj->m_speedModifier = path->GetNode("speed_modifier")->NodeAsFloat();
		}

		string loadPath = game->m_waveState->pathDir(enemyObj->m_path);
		//std::cout << "opening: " << loadPath << std::endl;
		PathGroup* p = PathIO::createFromFile(loadPath);
		if (p == NULL) {
			return NULL;
		}
		p->setRelative(true);



		if (path->GetNode("flip_h") != NULL && path->GetNode("flip_v") != NULL) {
			p->setFlip(path->GetNode("flip_h")->NodeAsBool(), path->GetNode("flip_v")->NodeAsBool());
		} else {
			p->setFlip(false, false);
		}

		if (path->GetNode("rotation") == NULL) {
			p->setRotation(0.0f);
		} else {
			p->setRotation(path->GetNode("rotation")->NodeAsFloat());
		}


		enemyObj->m_pathGroup = p;

	}
	return wave;
}

float EditorWave::getDuration() {
	float max = 0.0f;
	for(unsigned int i = 0; i < m_enemies.size(); i++) {
		float d = m_enemies.get(i)->getDuration();
		if (d > max) {
			max = d;
		}
	}
	return max + m_delay;
}
void EditorWave::setFlipped(bool h, bool v) {
	if ((h && !m_flippedH) || (!h && m_flippedH)) {
		for (unsigned int i = 0; i < m_enemies.size(); i++) {
			EditorEnemy* e = m_enemies.get(i);
			if (e->m_pathGroup != NULL) { e->m_pathGroup->flip(true, false); }

			int dx = abs(e->m_cx - 240);
			if (e->m_cx > 240) { e->m_cx -= 2*dx; }
			else { e->m_cx += 2*dx; }
		}
		//m_offsetx *= -1;
	}
	if ((v && !m_flippedV) || (!v && m_flippedV)) {
		for (unsigned int i = 0; i < m_enemies.size(); i++) {
			EditorEnemy* e = m_enemies.get(i);
			if (e->m_pathGroup != NULL) { e->m_pathGroup->flip(false, true); }

			int dy = abs(e->m_cy - 320);
			if (e->m_cy > 320) {e->m_cy -= 2*dy; }
			else { e->m_cy += 2*dy; }
		}
		//m_offsety *= -1;
	}

	m_flippedH = h;
	m_flippedV = v;
}
int EditorWave::getMinX() {
	int min = std::numeric_limits<int>::max();
	for(unsigned int i = 0; i < m_enemies.size(); i++) {
		EditorEnemy* e = m_enemies.get(i);
		int md = DefaultGame::getInstance()->m_enemyImages.at(e->m_id - 1)->getWidth()/2;
		if (e->m_cx-md < min) {
			min = e->m_cx-md ;
		}
	}
	return min + m_offsetx;
}
int EditorWave::getMaxX() {
	int max = std::numeric_limits<int>::min();
	for(unsigned int i = 0; i < m_enemies.size(); i++) {
		EditorEnemy* e = m_enemies.get(i);
		int md = DefaultGame::getInstance()->m_enemyImages.at(e->m_id - 1)->getWidth()/2;
		if (e->m_cx+md > max) {
			max = e->m_cx+md ;
		}
	}
	return max + m_offsetx;
}
int EditorWave::getMinY() {
	int min = std::numeric_limits<int>::max();
	for(unsigned int i = 0; i < m_enemies.size(); i++) {
		EditorEnemy* e = m_enemies.get(i);
		int md = DefaultGame::getInstance()->m_enemyImages.at(e->m_id - 1)->getWidth()/2;
		if (e->m_cy - md < min) {
			min = e->m_cy-md ;
		}
	}
	return min + m_offsety;
}
int EditorWave::getMaxY() {
	int max = std::numeric_limits<int>::min();
	for(unsigned int i = 0; i < m_enemies.size(); i++) {
		EditorEnemy* e = m_enemies.get(i);
		int md = DefaultGame::getInstance()->m_enemyImages.at(e->m_id - 1)->getWidth()/2;
		if (e->m_cy+md > max) {
			max = e->m_cy+md ;
		}
	}
	return max + m_offsety;
}

string EditorWave::toString() {
	const string nl = "\n";
	string s = "";
	s += "{" + nl;
	s += "	\"enemies\": [" + nl;
	for(unsigned int i = 0; i < m_enemies.size(); i++) {
		EditorEnemy* e = m_enemies.get(i);
		s += "		{" + nl;
			s += "			\"id\": " + Cast::toString<int>(e->m_id) + "," + nl;
			s += "			\"x\": " + Cast::toString<int>(e->m_cx) + "," + nl;
			s += "			\"y\": " + Cast::toString<int>(e->m_cy) + "," + nl;
			s += "			\"path\": {" + nl;
				s += "				\"file\": \"" + e->m_path + "\"," + nl;
				s += "				\"speed_modifier\": " + Cast::toString<float>(e->m_speedModifier) + "," + nl;

				if (e->m_pathGroup != NULL ) {
					s += "				\"rotation\": " + Cast::toString<float>(e->m_pathGroup->getRotation()) + "," + nl;
					s += "				\"flip_h\": " + string(e->m_pathGroup->isFlippedH()?"true":"false") + "," + nl;
					s += "				\"flip_v\": " + string(e->m_pathGroup->isFlippedV()?"true":"false") + "," + nl;
				}

				s += "				\"delay\": " + Cast::toString<float>(e->m_delay) + nl;
			s += "			}" + nl;
		if (i != m_enemies.size()-1) {
			s += "		}," + nl;
		} else {
			s += "		}" + nl;
		}
	}
	s += "	]" + nl;
	s += "}";
	return s;
}
EditorWave::~EditorWave() {

}
