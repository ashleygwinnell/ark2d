/*
 * DefaultGame.h
 *
 *  Created on: 19 July 2011
 *      Author: Ashley
 */

#ifndef DEFAULTGAME_H_
#define DEFAULTGAME_H_

/**
 * TODO:
 * - settings file with paths path, grid size, etc, saved.
 * - better list of paths.
 */

#include "ARK.h"

class WaveEditorState;
class LevelEditorState;
class GamePanel;


#include <string>
#include <map>
using namespace std;


/*
class Layout {

};
class AbsoluteLayout : public Layout {

};

class RelativeLayout : public Layout {

};

class RootPanel : public Panel {
	public:
		vector<UIComponent*> m_children;
		RootPanel(): Panel() {

		}
		void update(GameContainer* container, GameTimer* timer) {
			m_x = 0;
			m_y = 0;
			m_width = container->getWidth();
			m_height = container->getHeight();
		}
		~RootPanel() {

		}
};
*/

class DefaultGame : public StateBasedGame {
	public:
		static const int m_gameWidth = 480;
		static const int m_gameHeight = 640;

		vector<Image*> m_enemyImages;

		WaveEditorState* m_waveState;
		LevelEditorState* m_levelState;

		// options
		ScrollPanel* m_options;

			//snapping
			Label* m_snapLabel;
			TextField* m_snapField;

			// path locations.
			Label* m_pathsLocationLabel;
			TextField* m_pathsLocationField;

			// wave locations.
			Label* m_wavesLocationLabel;
			TextField* m_wavesLocationField;

			// render all paths.
			Label* m_renderAllPathsLabel;
			CheckBox* m_renderAllPathsCheckBox;

		DefaultGame(string title);
		void initStates(GameContainer* container);
		void update(GameContainer* container, GameTimer* timer);
		void render(GameContainer* container, Graphics* g);
		void resize(GameContainer* container, int width, int height);

		virtual void keyPressed(unsigned int key);
		virtual void keyReleased(unsigned int key);
		virtual void mouseMoved(int x, int y, int oldx, int oldy);

		static DefaultGame* getInstance();

		virtual ~DefaultGame();
};

#endif /* DEFAULTGAME_H_ */
