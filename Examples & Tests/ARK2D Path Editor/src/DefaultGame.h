/*
 * DefaultGame.h
 *
 *  Created on: 17 Jan 2011
 *      Author: Ashley
 */

#ifndef DEFAULTGAME_H_
#define DEFAULTGAME_H_

#include "ARK.h"

/**
 * TODO:
 *  - editor software
 *	 - fix PathGroup destructor.
 *	 - mousewheel to zoom.
 *	 - button images.
 *	 - store guide size in preferences.
 */
#include <string>
using namespace std;

class DefaultGame : public Game {
	public:
		PathGroup* pathGroup;

		TextField* durationTextField;
		TextField* pointXTextField;
		TextField* pointYTextField;
		ComboBox* easingComboBox;

		TextField* gameAreaWidthField;
		TextField* gameAreaHeightField;

		string m_currentFile;

		bool gamePanelMoving;
		int gamePanelPanX;
		int gamePanelPanY;
		Panel* gamePanel;

		Panel* detailsPanel;
		Button* selectButton;
		Button* majorPointButton;
		Button* minorPointButton;

		Button* playButton;
		Button* pauseButton;
		Button* stopButton;
		Button* forwardButton;
		Button* rewindButton;
		Button* newButton;
		Button* loadPathButton;
		Button* savePathButton;
		Button* saveAsPathButton;

		Button* flipHorizontallyButton;
		Button* flipVerticallyButton;
		Button* rotate90Button;

		bool m_playing;
		bool m_unsavedChanges;
		unsigned int m_toolSelected; // 0 for select, 1 for major point, 2 for minor point.
		static const unsigned int TOOL_SELECT = 0;
		static const unsigned int TOOL_MAJOR_POINT = 1;
		static const unsigned int TOOL_MINOR_POINT = 2;

		Vector2<float>* pointSelected;
		Vector2<float>* pointSelectedJoin;
		bool pointSelectedMoving;
		int pointSelectedIndexInGroup;
		int pointSelectedIndexInPath;

		DefaultGame(const char* title);
		void init(GameContainer* container);
		void update(GameContainer* container, GameTimer* timer);
		void render(GameContainer* container, Graphics* g);
		void resize(GameContainer* container, int width, int height);
		void keyPressed(unsigned int key);
		void keyReleased(unsigned int key);
		void mouseMoved(int x, int y, int oldx, int oldy);

		void unselectPoint();

		void setTitle(string s);
		void flip(bool hf, bool vf);
		void flipSingular(bool hf, bool vf);
		void rotate(int degrees);

		inline int gpmx() { return gamePanel->getX() + gamePanelPanX; }
		inline int gpmy() { return gamePanel->getY() + gamePanelPanY; }

		static DefaultGame* getInstance();

		static void easingChangedEvent();

		static void toolSelectPressed();
		static void toolMajorPointPressed();
		static void toolMinorPointPressed();

		static void playPressed();
		static void pausePressed();
		static void stopPressed();

		static void newPressed();
		static void loadPressed();
		static void savePressed();
		static void saveAsPressed();

		static void flipHPressed();
		static void flipVPressed();
		static void rotate90Pressed();

		virtual ~DefaultGame();
};

#endif /* DEFAULTGAME_H_ */

