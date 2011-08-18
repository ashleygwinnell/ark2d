/*
 * EditorLevel.h
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#ifndef EDITORLEVEL_H_
#define EDITORLEVEL_H_

#include "ARK.h"
#include "EditorWave.h"

class LevelEvent {
	public:
		static const unsigned int TYPE__SET_GAME_MODE = 0;
		static const unsigned int TYPE__START_BOSS = 1;
		static const unsigned int TYPE__SPAWN_TRINKET = 2;

		unsigned int m_type;			// the type of this event.
		float m_time; 					// when this event happens.
		unsigned int m_setGameModeType;	// which game mode to start.
		unsigned int m_startBossType;	// which boss to start.
		unsigned int m_spawnTrinketX;	// where to spawn the trinket.
};

class EditorLevel : public ARKGameObject {
	public:
		ARKVector<EditorWave*> m_waves;
		vector<LevelEvent*> m_events;
		float m_timer;


	public:
		EditorLevel();
		void update(GameContainer* container, GameTimer* timer);
		void render(GameContainer* container, Graphics* g);
		float getDuration();
		string toString();
		virtual ~EditorLevel();
};

#endif /* EDITORLEVEL_H_ */
