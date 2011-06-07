/*
 * EmptyTransition.cpp
 *
 *  Created on: 1 Aug 2010
 *      Author: Ashley
 */

#include "EmptyTransition.h"

EmptyTransition::EmptyTransition() {

}

void EmptyTransition::init(GameContainer* container, StateBasedGame* game, GameState* first, GameState* second) {
	// No operations, durr. That's why it's empty!
}
void EmptyTransition::update(GameContainer* container, StateBasedGame* game, GameTimer* delta) {
	// No operations, durr. That's why it's empty!
}
void EmptyTransition::preRender(GameContainer* container, StateBasedGame* game, Graphics* g) {
	// No operations, durr. That's why it's empty!
}
void EmptyTransition::postRender(GameContainer* container, StateBasedGame* game, Graphics* g) {
	// No operations, durr. That's why it's empty!
}
bool EmptyTransition::isComplete() {
	return true;
}

EmptyTransition::~EmptyTransition() {

}
