/*
 * Game.h
 *  Created on: 15-Oct-2009
 *      Author: Ashley Gwinnell
 */

#ifndef GAME_H_
#define GAME_H_

#include <string>
using namespace std;

#include "../Namespaces.h"

namespace ARK {
	namespace Core {

		/*!
		 * \brief A game with one game state should extend this class. e.g. test cases.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class Game {
			public:

				Game(string title);
				string getTitle();
				Timeline* getTimeline();
				virtual void init(GameContainer* container) = 0;

				virtual void preUpdate(GameContainer* container, GameTimer* timer);
				virtual void update(GameContainer* container, GameTimer* timer) = 0;
				virtual void postUpdate(GameContainer* container, GameTimer* timer);

				virtual void preRender(GameContainer* container, Renderer* g);
				virtual void render(GameContainer* container, Renderer* g) = 0;
				virtual void postRender(GameContainer* container, Renderer* g);

				virtual void resize(GameContainer* container, int width, int height);
				virtual void keyPressed(unsigned int key);
				virtual void keyReleased(unsigned int key);
				virtual void mouseMoved(int x, int y, int oldx, int oldy);
				virtual ~Game();

			protected:
				GameContainer* m_container;
				string m_title;
				Timeline* m_timeline;
		};
	}
}

#endif /* GAME_H_ */
