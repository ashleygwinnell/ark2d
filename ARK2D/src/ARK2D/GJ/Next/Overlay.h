/*
 * Overlay.h
 *
 *  Created on: Jan 22, 2015
 *      Author: ashleygwinnell
 */

#ifndef ARK_GAMEJOLT_NEXT_OVERLAY_H_
#define ARK_GAMEJOLT_NEXT_OVERLAY_H_

#include "../../../ARK.h"
#include "API.h"

namespace ARK {
	namespace GJ {
		namespace Next {

			string gjUtil_strtoupper(string s);

			class ARK2D_API GJTextField : public ARK::UI::TextField {
				public:
					GJTextField();
					virtual void renderBackground(); 
					virtual void renderText(int x, int y);
					virtual void renderOverlay();
					virtual void renderCaret(int x1, int y1, int x2, int y2);
					virtual void setFocussed(bool b);
					virtual ~GJTextField();
			};
			class ARK2D_API GJButton : public ARK::UI::Button {
				public:
					GJButton();
					//virtual void renderText(int x, int y);
					virtual void renderBackground();
					virtual void renderText(int x, int y);
					virtual void renderOverlay();
					virtual ~GJButton();
			};

			class ARK2D_API GJScrollablePanel {
				public:	  
					int m_x;
					int m_y;
					int m_width;
					int m_height;
					int m_originalX;
					int m_originalY;
					float m_scrollY;
					float m_scrollYReleased;
					float m_scrollYSpeed;
					float m_scrollYDecelerate;

					bool m_dragging; 
					float m_draggingStartY;
					Vector2<float> m_draggingStart;
					Vector2<float> m_draggingEnd;
					Timer m_releaseTimer;
					float m_dragTimer;

					int m_innerHeight;
					int m_actualInnerHeight;

					GJScrollablePanel(); 
					void init(int x, int y, int w, int h);
					void setHeight(float innerHeight, float outerHeight); 
					float getOffsetY() { return m_scrollY * -1.0f; }
					float getScrollY() { return getOffsetY(); }
					void update();
					//void render();
					virtual ~GJScrollablePanel();

					virtual void keyPressed(unsigned int key);
					virtual void keyReleased(unsigned int key);
					virtual void mouseMoved(int x, int y, int oldx, int oldy);
			};


			class ARK2D_API GJLoginState : public GameState {
				public:
					float m_iconY;
					GJTextField* username;
					GJTextField* usertoken;
					GJButton* close;
					GJButton* login;
					GJButton* signup;
					GJButton* help;

					GJLoginState();
					void enter(GameContainer* container, StateBasedGame* game, GameState* from);
					void leave(GameContainer* container, StateBasedGame* game, GameState* to);

					unsigned int id();
					void init(GameContainer* container, StateBasedGame* game);
					void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
					void render(GameContainer* container, StateBasedGame* game, Renderer* g);

					virtual void keyPressed(unsigned int key);
					virtual void keyReleased(unsigned int key);
					virtual void mouseMoved(int x, int y, int oldx, int oldy);

					static void onLogin(GJLoginState* ls);
					void onLoginFailed(string message);
					void onLoginSuccess();
					static void onSignup(GJLoginState* ls);
					static void onHelp(GJLoginState* ls);
					static void onClose(GJLoginState* ls);

					virtual ~GJLoginState();
			};

			class ARK2D_API GJOverviewState : public GameState {
				public:

					GJButton* achievements;
					GJButton* leaderboards;
					GJButton* stats;
					GJButton* close;
					GJButton* logout;

					GJOverviewState();
					void enter(GameContainer* container, StateBasedGame* game, GameState* from);
					void leave(GameContainer* container, StateBasedGame* game, GameState* to);

					unsigned int id();
					void init(GameContainer* container, StateBasedGame* game);
					void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
					void render(GameContainer* container, StateBasedGame* game, Renderer* g);

					virtual void keyPressed(unsigned int key);
					virtual void keyReleased(unsigned int key);
					virtual void mouseMoved(int x, int y, int oldx, int oldy);

					static void onAchievements(GJOverviewState* ls);
					static void onLeaderboards(GJOverviewState* ls);
					static void onStats(GJOverviewState* ls);
					static void onLogout(GJOverviewState* ls);
					static void onClose(GJOverviewState* ls);

					virtual ~GJOverviewState();
			};

			class ARK2D_API GJAchievementsState : public GameState {
				public:
					GJButton* back;
					GJScrollablePanel* scrollablePanel;
					ARK::GJ::Next::gjAchievementsResult* achievements;
					map<unsigned int, Image*> achievementIcons;
					unsigned long timestamp;

					vector<ARK::GJ::Next::gjUrlRequest*> m_requests;
					vector<ARK::GJ::Next::gjCallback*> m_callbacks;
					Mutex* m_callbackAddMutex;

					Image* trophyBronze;
					Image* trophySilver;
					Image* trophyGold;
					Image* trophyPlatinum;
					Image* getImageForTrophy(unsigned int id);

					void fetchIcon(unsigned int id);
					static void fetchIconInternal(GJAchievementsState* as, string result, ARK::GJ::Next::gjUrlRequest* req);

					ARK::GJ::Next::gjAchievement* findAchievementById(unsigned int id);

					GJAchievementsState();
					void enter(GameContainer* container, StateBasedGame* game, GameState* from);
					void leave(GameContainer* container, StateBasedGame* game, GameState* to);

					unsigned int id();
					void init(GameContainer* container, StateBasedGame* game);
					void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
					void render(GameContainer* container, StateBasedGame* game, Renderer* g);

					virtual void keyPressed(unsigned int key);
					virtual void keyReleased(unsigned int key);
					virtual void mouseMoved(int x, int y, int oldx, int oldy);

					static void onBackPressed(GJAchievementsState* ls);

					virtual ~GJAchievementsState();
			};


			class ARK2D_API GJLeaderboardsState : public GameState {
				public:
					GJButton* back;
					GJScrollablePanel* scrollablePanel;
					ARK::GJ::Next::gjHighscoreTablesResult* tables;
					unsigned long timestamp;
					vector<GJButton*> buttons;

					void refreshUI();
					
					GJLeaderboardsState();
					void enter(GameContainer* container, StateBasedGame* game, GameState* from);
					void leave(GameContainer* container, StateBasedGame* game, GameState* to);

					unsigned int id();
					void init(GameContainer* container, StateBasedGame* game);
					void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
					void render(GameContainer* container, StateBasedGame* game, Renderer* g);

					ARK::GJ::Next::gjHighscoreTable* findTableById(unsigned int tableId);

					virtual void keyPressed(unsigned int key);
					virtual void keyReleased(unsigned int key);
					virtual void mouseMoved(int x, int y, int oldx, int oldy);

					static void onBackPressed(GJLeaderboardsState* ls);
					static void onLeaderboardPressed(GJLeaderboardsState* ls);

					virtual ~GJLeaderboardsState();
			};



			class ARK2D_API GJLeaderboardState : public GameState {
				public:
					GJButton* back;
					GJScrollablePanel* scrollablePanel;
					unsigned int tableId;
					ARK::GJ::Next::gjHighscoresResult* scores;
					unsigned long timestamp;

					
					GJLeaderboardState();
					void setTableId(unsigned int id);
					void enter(GameContainer* container, StateBasedGame* game, GameState* from);
					void leave(GameContainer* container, StateBasedGame* game, GameState* to);

					unsigned int id();
					void init(GameContainer* container, StateBasedGame* game);
					void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
					void render(GameContainer* container, StateBasedGame* game, Renderer* g);

					virtual void keyPressed(unsigned int key);
					virtual void keyReleased(unsigned int key);
					virtual void mouseMoved(int x, int y, int oldx, int oldy);

					static void onBackPressed(GJLeaderboardState* ls);

					virtual ~GJLeaderboardState();
			};

			class ARK2D_API GJStatsState : public GameState {
				public:
					GJButton* back;
					GJScrollablePanel* scrollablePanel;
					
					GJStatsState();
					void enter(GameContainer* container, StateBasedGame* game, GameState* from);
					void leave(GameContainer* container, StateBasedGame* game, GameState* to);

					unsigned int id();
					void init(GameContainer* container, StateBasedGame* game);
					void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
					void render(GameContainer* container, StateBasedGame* game, Renderer* g);

					virtual void keyPressed(unsigned int key);
					virtual void keyReleased(unsigned int key);
					virtual void mouseMoved(int x, int y, int oldx, int oldy);

					static void onBackPressed(GJStatsState* ls);

					virtual ~GJStatsState();
			};


			class ARK2D_API GameJolt : public StateBasedGame {
				public:
					API* api;
					bool m_loggedin;

					Image* logo;
					Image* icon;
					Color* colorGreen;
					Color* colorBackground;

					ARK::Font::BMFont* font;
					KeyPairFile* file;

					float m_openTimer;
					float m_openDuration;
					unsigned int m_openState;
					static const unsigned int STATE_OPEN = 0;
					static const unsigned int STATE_OPENING = 1;
					static const unsigned int STATE_CLOSING = 2;
					static const unsigned int STATE_CLOSED = 3;

				public:
					static GameJolt* s_instance;

					static const unsigned int STATE_LOGIN = 0;
					static const unsigned int STATE_OVERVIEW = 1;
					static const unsigned int STATE_ACHIEVEMENTS = 2;
					static const unsigned int STATE_LEADERBOARDS = 3;
					static const unsigned int STATE_LEADERBOARD = 4;
					static const unsigned int STATE_STATS = 5;
					
				public:
					GameJolt(int gameId, string gameKey);
					void open();
					void toggle();
					void close();
					void showAchievements();
					void showHighscoreTable();
					void showHighscoreTable(unsigned int id);
					void showStats();
					void tick();
					void render();
					bool isOpen();
					float getAlpha();

					void renderBackground();
					void renderForeground();

					virtual void keyPressed(unsigned int key);
					virtual void keyReleased(unsigned int key);
					virtual void mouseMoved(int x, int y, int oldx, int oldy);
					virtual void resize(GameContainer* container, int width, int height);

					static GameJolt* getInstance();

				private:
					virtual void initStates(GameContainer* container);
					virtual void update(GameContainer* container, GameTimer* timer);
					virtual void render(GameContainer* container, Renderer* g);
					virtual void preRender(GameContainer* container, Renderer* r);
					virtual void postRender(GameContainer* container, Renderer* r);
					

					virtual ~GameJolt();
			};

		}
	}
}



#endif /* ARK_GAMEJOLT_NEXT_OVERLAY_H_ */
