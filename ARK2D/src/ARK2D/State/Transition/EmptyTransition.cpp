/*
 * EmptyTransition.cpp
 *
 *  Created on: 1 Aug 2010
 *      Author: Ashley
 */

#include "EmptyTransition.h"

namespace ARK {
	namespace State { 
		namespace Transition {

			EmptyTransition::EmptyTransition() {
				//ARK2D::getLog()->v("EmptyTransition::__construct");
			}

			void EmptyTransition::init(GameContainer* container, StateBasedGame* game, GameState* first, GameState* second) {
				// No operations, durr. That's why it's empty!
				//ARK2D::getLog()->v("EmptyTransition::init");
				Transition::init(container, game, first, second);
			}
			void EmptyTransition::update(GameContainer* container, StateBasedGame* game, GameTimer* delta) {
				// No operations, durr. That's why it's empty!
				//ARK2D::getLog()->v("EmptyTransition::update");
			}
			void EmptyTransition::preRender(GameContainer* container, StateBasedGame* game, Renderer* g) {
				// No operations, durr. That's why it's empty!
				//ARK2D::getLog()->v("EmptyTransition::preRender");
			}
			void EmptyTransition::postRender(GameContainer* container, StateBasedGame* game, Renderer* g) {
				// No operations, durr. That's why it's empty!
				//ARK2D::getLog()->v("EmptyTransition::postRender");
			}
			bool EmptyTransition::isComplete() {
				//ARK2D::getLog()->v("EmptyTransition::isComplete"); 
				return true;
			}

			EmptyTransition::~EmptyTransition() {
				//ARK2D::getLog()->v("EmptyTransition::__destruct");
			}
		}
	}
}
