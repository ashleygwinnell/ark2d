/*
 * IntelligentGameState.cpp
 *
 *  Created on: Dec 9, 2011
 *      Author: ashleygwinnell
 */

#include "IntelligentGameState.h"
#include "../GameObject.h"
#include "../Vector.h"
#include "../Pool.h"

namespace ARK {
    namespace Core {
        namespace State {

            IntelligentGameState::IntelligentGameState(): GameState(), m_entities() {

            }

            void IntelligentGameState::add(string group, GameObject* entity) {
                m_entities[group].add(entity);
            }
            void IntelligentGameState::pruneEntities() {
                // Automatically remove any objects  that are cruddy.
                map<string, Vector<GameObject*> >::iterator it;
                for (it = m_entities.begin(); it != m_entities.end(); ++it) {
                    it->second.pruneAll();
                    Vector<GameObject*> thisEntities = it->second;

                    bool anyRemoved = false;
                    for(unsigned int i = 0; i < thisEntities.size(); i++) {
                        GameObject* thisObject = thisEntities.get(i);
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

            bool IntelligentGameState::keyPressed(unsigned int key) {
                return GameState::keyPressed(key);
            }
            bool IntelligentGameState::keyReleased(unsigned int key) {
                return GameState::keyReleased(key);
            }
            bool IntelligentGameState::mouseMoved(int x, int y, int oldx, int oldy) {
                return GameState::mouseMoved(x, y, oldx, oldy);
            }

            void IntelligentGameState::init(GameContainer* container, StateBasedGame* game) {

            }
            void IntelligentGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
                pruneEntities();
            }
            void IntelligentGameState::render(GameContainer* container, StateBasedGame* game, Renderer* g) {

            }

            IntelligentGameState::~IntelligentGameState() {

            }
        }

	}
}

