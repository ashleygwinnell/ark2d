/*
 * GameState.h
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "../Includes.h"
#include "../Namespaces.h"
#include "../Controls/KeyListener.h"
#include "../UI/AbstractUIComponent.h"

namespace ARK {
	namespace State {

		/*!
		 * \brief All game states (FSM model) should extend this class.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
        class ARK2D_API GameState : public SceneNode {
			public:
				GameState();
				GameState(string name);

				virtual void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				virtual void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				virtual unsigned int id() = 0;
				virtual void init(GameContainer* container, StateBasedGame* game) = 0;
				virtual void update(GameContainer* container, StateBasedGame* game, GameTimer* timer) = 0;
				virtual void render(GameContainer* container, StateBasedGame* game, Renderer* g) = 0;
				
				virtual void render();

				virtual void pause(); // android events
				virtual void resume();

				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);

				virtual void orientationChanged(int orientation);

				virtual ~GameState();

		};
	}
}

#endif /* GAMESTATE_H_ */
