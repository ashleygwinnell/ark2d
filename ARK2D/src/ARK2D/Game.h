/*
 * Game.h
 *  Created on: 15-Oct-2009
 *      Author: Ashley Gwinnell
 */

#ifndef GAME_H_
#define GAME_H_

#include <string>

class GameContainer;
class Graphics;
class GameTimer;
class Timeline;

class Game {
	public:

		Game(const char* title);
		const char* getTitle();
		Timeline* getTimeline();
		virtual void init(GameContainer* container) = 0;
		virtual void update(GameContainer* container, GameTimer* timer) = 0;
		virtual void render(GameContainer* container, Graphics* g) = 0;
		virtual void resize(GameContainer* container, int width, int height) = 0;
		virtual void keyPressed(unsigned int key);
		virtual void keyReleased(unsigned int key);
		virtual void mouseMoved(int x, int y, int oldx, int oldy);
		virtual ~Game();

	protected:
		GameContainer* m_container;
		const char* m_title;
		Timeline* m_timeline;
};

#endif /* GAME_H_ */
