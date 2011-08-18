/*
 * EditorEnemy.h
 *
 *  Created on: 21 Jul 2011
 *      Author: Ashley
 */

#ifndef EDITORENEMY_H_
#define EDITORENEMY_H_

#include "ARK.h"

#include <string>
using namespace std;

class EditorEnemy : public ARKGameObject {
	public:
		int m_id;
		int m_cx;
		int m_cy;
		bool m_mouseOver;

		string m_path;
		float m_delay;
		float m_timer;
		float m_speedModifier;

		PathGroup* m_pathGroup;

		EditorEnemy();
		int getXAtTime(float f);
		int getYAtTime(float f);
		float getDuration();
		void update(GameContainer* container, GameTimer* timer);
		void render(GameContainer* container, Graphics* g);
		virtual ~EditorEnemy();
};

#endif /* EDITORENEMY_H_ */
