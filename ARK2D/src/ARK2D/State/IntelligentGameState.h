/*
 * IntelligentGameState.h
 *
 *  Created on: Dec 9, 2011
 *      Author: ashleygwinnell
 */

#ifndef INTELLIGENTGAMESTATE_H_
#define INTELLIGENTGAMESTATE_H_

using namespace std;
#include <map>
#include <string>
#include "GameState.h"

#include "../Namespaces.h"
#include "../Util/Containers/Vector.h"

namespace ARK {
	namespace State {

		/*!
		 * \brief This game state holds, updates and renders entities intelligently using ARKGameObject properties.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class IntelligentGameState : public GameState {
			private:

				map<string, Vector<GameObject*> > m_entities;

			public:
				IntelligentGameState();

				void add(string group, GameObject* entity);
				void pruneEntities();

				virtual void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				virtual void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				virtual unsigned int id() = 0;
				virtual void init(GameContainer* container, StateBasedGame* game);
				virtual void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				virtual void render(GameContainer* container, StateBasedGame* game, Renderer* g);

				virtual void keyPressed(unsigned int key);
				virtual void keyReleased(unsigned int key);
				virtual void mouseMoved(int x, int y, int oldx, int oldy);

				virtual ~IntelligentGameState();
		};
	}
}

#endif /* INTELLIGENTGAMESTATE_H_ */
