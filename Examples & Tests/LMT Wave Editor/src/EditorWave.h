/*
 * EditorWave.h
 *
 *  Created on: 23 Jul 2011
 *      Author: Ashley
 */

#ifndef EDITORWAVE_H_
#define EDITORWAVE_H_

#include "ARK.h"

#include "EditorEnemy.h"

#include <string>
using namespace std;


class EditorWave : public ARKGameObject {
	public:
		string fname;
		ARKVector<EditorEnemy*> m_enemies;
		int m_offsetx;
		int m_offsety;
		float m_delay;
		float m_timer;
		bool m_flippedH;
		bool m_flippedV;
		bool m_mouseOver;
	public:
		EditorWave();
		void update(GameContainer* container, GameTimer* timer);
		void render(GameContainer* container, Graphics* g);
		void renderPaths();
		float getDuration();
		string toString();

		static EditorWave* createFromString(string s);

		void setFlipped(bool h, bool v);

		int getMinX();
		int getMaxX();
		int getMinY();
		int getMaxY();

		virtual ~EditorWave();
};

#endif /* EDITORWAVE_H_ */
