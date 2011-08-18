/*
 * WaveEditorState.h
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#ifndef WAVEEDITORSTATE_H_
#define WAVEEDITORSTATE_H_

#include "ARK.h"

#include "EditorEnemy.h"
#include "EditorWave.h"

#include "ToolPanel.h"
#include "EnemySelectPanel.h"

class GamePanel;

class WaveEditorState: public GameState {
	public:
		EditorWave* m_wave;

		ToolPanel* m_toolPanel;
		Button* m_toolSelect;
		Button* m_toolAddEnemy;
		Button* m_toolOptions;
		Button* m_newButton;
		Button* m_loadButton;
		Button* m_saveButton;
		Button* m_saveAsButton;
		Button* m_playButton;
		Button* m_pauseButton;
		Button* m_stopButton;
		Button* m_forwardButton;
		Button* m_rewindButton;


		GamePanel* m_gamePanel;

		EnemySelectPanel* m_enemySelectPanel;

		// select panel.
		ScrollPanel* m_selectPanel;

			// set path stuff.
			Label* m_selectPathLabel;
			TextField* m_selectPathField;
			Button* m_selectPathButton;

			Label* m_selectPathDelayLabel;
			TextField* m_selectPathDelayField;

			Label* m_speedModifierLabel;
			TextField* m_speedModifierField;

			Label* m_flipHLabel;
			CheckBox* m_flipHCheckBox;

			Label* m_flipVLabel;
			CheckBox* m_flipVCheckBox;

			Label* m_rotationLabel;
			TextField* m_rotationField;

			Button* m_selectPathSet;

		bool m_playing;
		float m_timer;

		string m_currentFile;


	public:
		WaveEditorState();
		void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		unsigned int id();
		void init(GameContainer* container, StateBasedGame* game);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void render(GameContainer* container, StateBasedGame* game, Graphics* g);
		void renderEnemyPath(int index);
		void resize(GameContainer* container, int width, int height);

		void setTitle(string title);
		string pathDir(string s);

		virtual void keyPressed(unsigned int key);
		virtual void keyReleased(unsigned int key);
		virtual void mouseMoved(int x, int y, int oldx, int oldy);

		void selectEnemy(int index);
		EditorEnemy* placeEnemy(int id, int cx, int cy);

		static void selectEnemyPath();

		static void newWave();
		static void loadWave();
		static void saveWave();
		static void saveAsWave();

		static void playPressed();
		static void pausePressed();
		static void stopPressed();
		static void forwardPressed();
		static void rewindPressed();

		static void saveEnemy();

		virtual ~WaveEditorState();
};

#endif /* WAVEEDITORSTATE_H_ */
