/*
 * LevelEditorState.h
 * 31 Jul 2011
 */

#ifndef LEVELEDITORSTATE_H_
#define LEVELEDITORSTATE_H_

#include "ARK.h"
#include "EditorLevel.h"
#include "LevelGamePanel.h"
#include "LevelToolPanel.h"

class DefaultGame;

class LevelEditorState : public GameState {
	public:
		EditorLevel* m_level;

		LevelToolPanel* m_sideBar;

		Button* m_selectButton;
		Button* m_addWaveButton;
		Button* m_eventsButton;
		Button* m_optionsButton;

		Button* m_newButton;
		Button* m_loadButton;
		Button* m_saveButton;
		Button* m_saveAsButton;

		Button* m_playButton;
		Button* m_pauseButton;
		Button* m_stopButton;
		Button* m_forwardButton;
		Button* m_rewindButton;

		LevelGamePanel* m_gamePanel;

		ScrollPanel* m_levelDetailsPanel;
			Label* m_levelIDLabel;
			TextField* m_levelIDField;
			Label* m_levelNameLabel;
			TextField* m_levelNameField;


		ScrollPanel* m_waveDetailsPanel;

		Label* m_waveFileLabel;
		TextField* m_waveFileField;

		Label* m_waveOffsetXLabel;
		TextField* m_waveOffsetXField;

		Label* m_waveOffsetYLabel;
		TextField* m_waveOffsetYField;

		Label* m_waveDelayLabel;
		TextField* m_waveDelayField;

		Label* m_waveFlipHLabel;
		CheckBox* m_waveFlipHCheckBox;

		Label* m_waveFlipVLabel;
		CheckBox* m_waveFlipVCheckBox;

		Button* m_waveSetButton;

		ScrollPanel* m_eventsPanel;
		TextField* m_eventsScriptText;
		Button* m_eventsScriptSave;

		bool m_playing;
		float m_timer;

		string m_currentFile;
		RSSL* m_rssl;

	public:
		LevelEditorState();
		void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		unsigned int id();
		void init(GameContainer* container, StateBasedGame* game);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void render(GameContainer* container, StateBasedGame* game, Graphics* g);
		void resize(GameContainer* container, int width, int height);

		virtual void keyPressed(unsigned int key);
		virtual void keyReleased(unsigned int key);
		virtual void mouseMoved(int x, int y, int oldx, int oldy);

		void setTitle(string title);
		void selectWave(int index);

		static void saveScript();
		static void parseScript(string s);

		static void addWave();
		static void moveWave();
		static void saveWave();

		static void newLevel();
		static void loadLevel();
		static void saveLevel();
		static void saveAsLevel();

		static void playPressed();
		static void pausePressed();
		static void stopPressed();

		static void script_GameMode(RSSLFunction* f);
		static void script_StartBoss(RSSLFunction* f);
		static void script_CameraShake(RSSLFunction* f);
		static void script_SpawnTrinket(RSSLFunction* f);

		string pathDir(string s);

		~LevelEditorState();
};

#endif /* LEVELEDITORSTATE_H_ */
