/*
 * GameState.cpp
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#include "GameState.h"
#include "StateBasedGame.h"

#include "../../Namespaces.h"
//#include "../Controls/KeyListener.h"
//#include "../UI/AbstractUIComponent.h"

namespace ARK {
    namespace Core {
        namespace State {

            GameState::GameState():
                SceneNode("GameState",TYPE_NODE)
            {
                visible = false;
            }
            GameState::GameState(string nm):
                SceneNode(nm,TYPE_NODE)
            {
                visible = false;
            }

            void GameState::preEnter(GameContainer* container, StateBasedGame* game, GameState* from) {
                ARK2D::getLog()->v("GameState::preenter");
            }
            void GameState::postLeave(GameContainer* container, StateBasedGame* game, GameState* to) {
                ARK2D::getLog()->v("GameState::postleave");
            }
            void GameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {
                ARK2D::getLog()->v("GameState::enter");
            }
            void GameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {
                ARK2D::getLog()->v("GameState::leave");
            }

            void GameState::pause() {
                ARK2D::getLog()->v("GameState::pause");
            }
            void GameState::resume() {
                ARK2D::getLog()->v("GameState::resume");
            }

            // SceneNode render, not the real one.
            void GameState::render() {
                if (!visible) { return; }
                StateBasedGame* sbg = dynamic_cast<StateBasedGame*>(ARK2D::getGame());
                render(ARK2D::getContainer(), sbg, ARK2D::getRenderer());
            }


            bool GameState::keyPressed(unsigned int key) {
                return SceneNode::keyPressed(key);
            }
            bool GameState::keyReleased(unsigned int key) {
                return SceneNode::keyReleased(key);
            }
            bool GameState::mouseMoved(int x, int y, int oldx, int oldy) {
                return SceneNode::mouseMoved(x,y,oldx,oldy);
            }
            void GameState::orientationChanged(int orientation) {

            }
            GameState::~GameState() {
                // hmm
            }
        }

	}
}
