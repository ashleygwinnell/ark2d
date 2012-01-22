/*
 * IntelligentGameState.cpp
 *
 *  Created on: Dec 9, 2011
 *      Author: ashleygwinnell
 */

#include "IntelligentGameState.h"
#include "../ARKGameObject.h"
#include "../ARKVector.h" 
#include "../ARKPool.h"

IntelligentGameState::IntelligentGameState(): GameState(), m_entities() {

}

void IntelligentGameState::add(string group, ARKGameObject* entity) { 
	m_entities[group].add(entity);
}   
void IntelligentGameState::pruneEntities() { 
	// Automatically remove any objects  that are cruddy.
	map<string, ARKVector<ARKGameObject*> >::iterator it; 
	for (it = m_entities.begin(); it != m_entities.end(); ++it) { 
		it->second.pruneAll();
		ARKVector<ARKGameObject*> thisEntities = it->second;

		bool anyRemoved = false; 	 
		for(unsigned int i = 0; i < thisEntities.size(); i++) {
			ARKGameObject* thisObject = thisEntities.get(i);
			if (thisObject->isPendingRemoval()) {
				thisEntities.remove(i);
				anyRemoved = true;
			}
		}
		if (anyRemoved) {
			it->second = thisEntities;
		}
	}
}

void IntelligentGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {
	GameState::enter(container, game, from);
}
void IntelligentGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {
	GameState::leave(container, game, to);
}

void IntelligentGameState::keyPressed(unsigned int key) {
	GameState::keyPressed(key);
}
void IntelligentGameState::keyReleased(unsigned int key) {
	GameState::keyReleased(key);
}
void IntelligentGameState::mouseMoved(int x, int y, int oldx, int oldy) {
	GameState::mouseMoved(x, y, oldx, oldy);
}

void IntelligentGameState::init(GameContainer* container, StateBasedGame* game) {

}
void IntelligentGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	pruneEntities();
}
void IntelligentGameState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {

}

IntelligentGameState::~IntelligentGameState() {

}

