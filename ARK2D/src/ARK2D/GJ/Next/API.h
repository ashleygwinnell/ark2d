
#ifndef ARK_GAMEJOLT_NEXT_API_H_
#define ARK_GAMEJOLT_NEXT_API_H_

#define GAMEJOLT_SHORT_NAMES

#include "../../Threading/Mutex.h"
#include "../../Util/URLRequest.h"
#include "../MD5.h"
#include <string.h>

namespace ARK {
	namespace GJ {
		namespace Next {

			// ------------------------------------------------------------------			
			// https://github.com/hworld/gj-doc-game-api/blob/master/v1.x/index.md
			class API;

			// ------------------------------------------------------------------
			// Object structs (Highscores, Achievements, Users)
			// ------------------------------------------------------------------
			// Highscores
			// ------------------------------------------------------------------
			typedef struct gjHighscore {
				unsigned int userid; 
				char* username;
				bool guest;
				char* guestname;

				unsigned int globalRank;
				unsigned int score;
			} gjHighscore;

			gjHighscore* gjHighscore_create();
			gjHighscore* gjHighscore_copy(gjHighscore* highscore);
			const char* gjHighscore_getName(gjHighscore* highscore);
			void gjHighscore_dispose(gjHighscore* highscore);

			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjHighscore Highscore;
				#define Highscore_create(...) gjHighscore_create(__VA_ARGS__)
				#define Highscore_copy(...) gjHighscore_copy(__VA_ARGS__)
				#define Highscore_getName(...) gjHighscore_getName(__VA_ARGS__)
				#define Highscore_dispose(...) gjHighscore_dispose(__VA_ARGS__)
			#endif

			typedef struct gjHighscoreTable {
				unsigned int id; 
				char* name;
				char* description;
				bool primary;
			} gjHighscoreTable;

			gjHighscoreTable* gjHighscoreTable_create();
			gjHighscoreTable* gjHighscoreTable_copy(gjHighscoreTable* table);
			void gjHighscoreTable_dispose(gjHighscoreTable* highscore);

			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjHighscoreTable HighscoreTable;
				#define HighscoreTable_create(...) gjHighscoreTable_create(__VA_ARGS__)
				#define HighscoreTable_copy(...) gjHighscoreTable_copy(__VA_ARGS__)
				#define HighscoreTable_dispose(...) gjHighscoreTable_dispose(__VA_ARGS__)
			#endif

			// ------------------------------------------------------------------
			// Achievements
			// ------------------------------------------------------------------
			typedef enum { GJ_DIFFICULTY_BRONZE, GJ_DIFFICULTY_SILVER, GJ_DIFFICULTY_GOLD, GJ_DIFFICULTY_PLATINUM } gjAchievementDifficulty;
			typedef struct gjAchievement {
				unsigned int id;
				char* name;
				char* description;
				gjAchievementDifficulty difficulty;
				char* imageUrl;
				bool achieved;
				char* achievedOn;
			} gjAchievement;
			gjAchievement* gjAchievement_create();
			gjAchievement* gjAchievement_copy(gjAchievement* achievement);
			const char* gjAchievement_getName(gjAchievement* achievement);
			void gjAchievement_dispose(gjAchievement* achievement);

			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjAchievementDifficulty AchievementDifficulty;
				#define DIFFICULTY_BRONZE GJ_DIFFICULTY_BRONZE
				#define DIFFICULTY_SILVER GJ_DIFFICULTY_SILVER
				#define DIFFICULTY_GOLD GJ_DIFFICULTY_GOLD
				#define DIFFICULTY_PLATINUM GJ_DIFFICULTY_PLATINUM
				typedef gjAchievement Achievement;
				#define Achievement_create(...) gjAchievement_create(__VA_ARGS__)
				#define Achievement_copy(...) gjAchievement_copy(__VA_ARGS__)
				#define Achievement_getName(...) gjAchievement_getName(__VA_ARGS__)
				#define Achievement_dispose(...) gjAchievement_dispose(__VA_ARGS__)
			#endif

			// ------------------------------------------------------------------
			// Generic / empty result
			// ------------------------------------------------------------------
			typedef struct gjGeneralResult {
				bool success;
				char* message;
			} gjGeneralResult;
			gjGeneralResult* gjGeneralResult_create();
			void gjGeneralResult_dispose(gjGeneralResult* res);
			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjGeneralResult GeneralResult;
				#define GeneralResult_create(...) gjGeneralResult_create(__VA_ARGS__)
				#define GeneralResult_dispose(...) gjGeneralResult_dispose(__VA_ARGS__)
			#endif

			// ------------------------------------------------------------------
			// Highscores Results
			// ------------------------------------------------------------------
			typedef struct gjHighscoresResult { 
				bool success; 
				char* message;
				unsigned int scoresCount;
				gjHighscore** scores;
			} gjHighscoresResult;
			gjHighscoresResult* gjHighscoresResult_create(unsigned int scoresCount);
			gjHighscoresResult* gjHighscoresResult_copy(gjHighscoresResult*);
			void gjHighscoresResult_dispose(gjHighscoresResult* res);

			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjHighscoresResult HighscoresResult;
				#define HighscoresResult_create(...) gjHighscoresResult_create(__VA_ARGS__)
				#define HighscoresResult_copy(...) gjHighscoresResult_copy(__VA_ARGS__)
				#define HighscoresResult_dispose(...) gjHighscoresResult_dispose(__VA_ARGS__)
			#endif

			// Fake
			#define gjHighscoreSubmitResult gjGeneralResult
			#define gjHighscoreSubmitResult_create(...) gjGeneralResult_create(__VA_ARGS__)
			#define gjHighscoreSubmitResult_dispose(...) gjGeneralResult_dispose(__VA_ARGS__)
			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjGeneralResult HighscoreSubmitResult;
				#define HighscoreSubmitResult_create(...) gjGeneralResult_create(__VA_ARGS__)
				#define HighscoreSubmitResult_dispose(...) gjGeneralResult_dispose(__VA_ARGS__)
			#endif

			// ------------------------------------------------------------------
			// Highscore Tables Results
			// ------------------------------------------------------------------
			typedef struct gjHighscoreTablesResult { 
				bool success; 
				char* message;
				unsigned int tablesCount;
				gjHighscoreTable** tables;
			} gjHighscoreTablesResult;
			gjHighscoreTablesResult* gjHighscoreTablesResult_create(unsigned int scoresCount);
			gjHighscoreTablesResult* gjHighscoreTablesResult_copy(gjHighscoreTablesResult* result);
			void gjHighscoreTablesResult_dispose(gjHighscoreTablesResult* res);

			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjHighscoreTablesResult HighscoreTablesResult;
				#define HighscoreTablesResult_create(...) gjHighscoreTablesResult_create(__VA_ARGS__)
				#define HighscoreTablesResult_copy(...) gjHighscoreTablesResult_copy(__VA_ARGS__)
				#define HighscoreTablesResult_dispose(...) gjHighscoreTablesResult_dispose(__VA_ARGS__)
			#endif

			// ------------------------------------------------------------------
			// Highscore Rank Result
			// ------------------------------------------------------------------
			typedef struct gjHighscoreRankResult { 
				bool success; 
				char* message;
				unsigned int rank;
			} gjHighscoreRankResult;
			gjHighscoreRankResult* gjHighscoreRankResult_create();
			gjHighscoreRankResult* gjHighscoreRankResult_copy(gjHighscoreRankResult* result);
			void gjHighscoreRankResult_dispose(gjHighscoreRankResult* res);

			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjHighscoreRankResult HighscoreRankResult;
				#define HighscoreRankResult_create(...) gjHighscoreRankResult_create(__VA_ARGS__)
				#define HighscoreRankResult_copy(...) gjHighscoreRankResult_copy(__VA_ARGS__)
				#define HighscoreRankResult_dispose(...) gjHighscoreRankResult_dispose(__VA_ARGS__)
			#endif
				 
			// ------------------------------------------------------------------
			// Achievements Results
			// ------------------------------------------------------------------
			typedef struct gjAchievementsResult { 
				bool success; 
				char* message;
				unsigned int achievementsCount;
				gjAchievement** achievements;
			} gjAchievementsResult;
			gjAchievementsResult* gjAchievementsResult_create(unsigned int achievementsCount);
			gjAchievementsResult* gjAchievementsResult_copy(gjAchievementsResult* res);
			void gjAchievementsResult_dispose(gjAchievementsResult* res);

			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjAchievementsResult AchievementsResult;
				#define AchievementsResult_create(...) gjAchievementsResult_create(__VA_ARGS__)
				#define AchievementsResult_copy(...) gjAchievementsResult_copy(__VA_ARGS__)
				#define AchievementsResult_dispose(...) gjAchievementsResult_dispose(__VA_ARGS__)
			#endif

			// Fake
			#define gjAchievementSubmitResult gjGeneralResult
			#define gjAchievementSubmitResult_create(...) gjGeneralResult_create(__VA_ARGS__)
			#define gjAchievementSubmitResult_dispose(...) gjGeneralResult_dispose(__VA_ARGS__)
			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjGeneralResult AchievementSubmitResult;
				#define AchievementSubmitResult_create(...) gjGeneralResult_create(__VA_ARGS__)
				#define AchievementSubmitResult_dispose(...) gjGeneralResult_dispose(__VA_ARGS__)
			#endif


			// ------------------------------------------------------------------
			// Users/auth Results
			// ------------------------------------------------------------------
			typedef struct gjUsersAuthResult {
				bool success;
				char* message;
				char* username;
				char* usertoken;
			} gjUsersAuthResult;
			gjUsersAuthResult* gjUsersAuthResult_create();
			void gjUsersAuthResult_dispose(gjUsersAuthResult* res);

			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjUsersAuthResult UsersAuthResult;
				#define UsersAuthResult_create(...) gjUsersAuthResult_create(__VA_ARGS__)
				#define UsersAuthResult_dispose(...) gjUsersAuthResult_dispose(__VA_ARGS__)
			#endif
			
			typedef struct gjUsersAuthRequestAttachment {
				char* username;
				char* usertoken;
			} gjUsersAuthRequestAttachment;
			gjUsersAuthRequestAttachment* gjUsersAuthRequestAttachment_create();
			void gjUsersAuthRequestAttachment_dispose(gjUsersAuthRequestAttachment* res);

			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjUsersAuthRequestAttachment UsersAuthRequestAttachment;
				#define UsersAuthRequestAttachment_create(...) gjUsersAuthRequestAttachment_create(__VA_ARGS__)
				#define UsersAuthRequestAttachment_dispose(...) gjUsersAuthRequestAttachment_dispose(__VA_ARGS__)
			#endif


			// ------------------------------------------------------------------
			// Sessions
			// ------------------------------------------------------------------
			#define gjSessionOpenResult gjGeneralResult
			#define gjSessionOpenResult_create(...) gjGeneralResult_create(__VA_ARGS__)
			#define gjSessionOpenResult_dispose(...) gjGeneralResult_dispose(__VA_ARGS__)
			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjGeneralResult SessionOpenResult;
				#define gjSessionOpenResult_create(...) gjGeneralResult_create(__VA_ARGS__)
				#define gjSessionOpenResult_dispose(...) gjGeneralResult_dispose(__VA_ARGS__)
			#endif

			#define gjSessionPingResult gjGeneralResult
			#define gjSessionPingResult_create(...) gjGeneralResult_create(__VA_ARGS__)
			#define gjSessionPingResult_dispose(...) gjGeneralResult_dispose(__VA_ARGS__)
			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjGeneralResult SessionPingResult;
				#define gjSessionPingResult_create(...) gjGeneralResult_create(__VA_ARGS__)
				#define gjSessionPingResult_dispose(...) gjGeneralResult_dispose(__VA_ARGS__)
			#endif

			#define gjSessionCloseResult gjGeneralResult
			#define gjSessionCloseResult_create(...) gjGeneralResult_create(__VA_ARGS__)
			#define gjSessionCloseResult_dispose(...) gjGeneralResult_dispose(__VA_ARGS__)
			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjGeneralResult SessionCloseResult;
				#define gjSessionCloseResult_create(...) gjGeneralResult_create(__VA_ARGS__)
				#define gjSessionCloseResult_dispose(...) gjGeneralResult_dispose(__VA_ARGS__)
			#endif

			// ------------------------------------------------------------------
			// Callbacks
			// ------------------------------------------------------------------
			typedef enum { 
				GJ_GENERAL_RESULT,
				GJ_HIGHSCORES_RESULT,
				GJ_HIGHSCORE_RANK_RESULT,
				GJ_HIGHSCORE_SUBMIT_RESULT,
				GJ_HIGHSCORE_TABLES_RESULT,
				GJ_ACHIEVEMENTS_RESULT,
				GJ_ACHIEVEMENT_SUBMIT_RESULT,
				GJ_USERS_AUTH_RESULT,
				GJ_SESSION_OPEN_RESULT,
				GJ_SESSION_PING_RESULT,
				GJ_SESSION_CLOSE_RESULT
			} gjCallbackType;
			#define GJOVERLAY_USERS_ICON_RESULT 1000;
			typedef struct gjCallback {
				unsigned int id; 
				gjCallbackType type;
				void* data;
			} gjCallback;
			gjCallback* gjCallback_create(gjCallbackType type, void* data);
			void gjCallback_dispose(gjCallback* cb);

			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjCallbackType CallbackType;
				#define HIGHSCORES_RESULT GJ_HIGHSCORES_RESULT
				// ... TODO
				typedef gjCallback Callback;
				#define Callback_create(...) gjCallback_create(__VA_ARGS__)
				#define Callback_dispose(...) gjCallback_dispose(__VA_ARGS__)
			#endif

			// ------------------------------------------------------------------
			// Formats
			// ------------------------------------------------------------------
			typedef enum {
				GJ_FORMAT_JSON,
				GJ_FORMAT_XML
			} gjFormatType;


			// ------------------------------------------------------------------
			// Callback Listener
			// ------------------------------------------------------------------
			typedef void (*gjListener) (API* api, gjCallbackType type, void* data);
			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjListener Listener;
			#endif

			// ------------------------------------------------------------------
			// Internal requests
			// ------------------------------------------------------------------
			typedef struct gjUrlRequest {
				unsigned int id;
				URLRequest* request;
				void* attachment;
			} gjUrlRequest;

			gjUrlRequest* gjUrlRequest_create();
			void gjUrlRequest_setUrl(gjUrlRequest* req, const char* url);
			void gjUrlRequest_start(gjUrlRequest* req, void* callback, void* callbackObj);
			void gjUrlRequest_dispose(gjUrlRequest* req);

			#ifdef GAMEJOLT_SHORT_NAMES
				typedef gjUrlRequest UrlRequest;
				#define UrlRequest_create(...) gjUrlRequest_create(__VA_ARGS__);
				#define UrlRequest_setUrl(...) gjUrlRequest_setUrl(__VA_ARGS__);
				#define UrlRequest_start(...) gjUrlRequest_start(__VA_ARGS__);
				#define UrlRequest_dispose(...) gjUrlRequest_dispose(__VA_ARGS__);
			#endif
			
			// ------------------------------------------------------------------
			// Public facing API
			// ------------------------------------------------------------------
			class ARK2D_API API {
				public:
					unsigned int m_gameId;
					string m_gameKey;

					string m_userName;
					string m_userToken;

					vector<gjUrlRequest*> m_requests;
					vector<gjCallback*> m_callbacks;
					Mutex* m_callbackAddMutex;
					gjListener m_internalListener; // one to call before giving it to the user.
					gjListener m_overlayListener; // another to call before giving it to the user.
					gjListener m_listener;

					gjFormatType m_format;
					

				public:
					static unsigned int s_requestId;
					static unsigned int s_callbackId;

				public:
					API(int gameId, string gameKey);
					void setListener(gjListener eventsCallback);
					void setFormat(gjFormatType format);
					void logout();
					
					void authUser(string name, string token);
					static void authUserInternal(API* api, string result, gjUrlRequest* req);

					void getHighscores(unsigned int tableid, unsigned int startOffset, unsigned int endOffset);
					static void getHighscoresInternal(API* api, string result, gjUrlRequest* req);

					void getRank(unsigned int tableId, signed int score);
					static void getRankInternal(API* api, string result, gjUrlRequest* req);

					void getHighscoreTables();
					static void getHighscoreTablesInternal(API* api, string result, gjUrlRequest* req);

					void submitHighscore(unsigned int tableid, unsigned int score, string unit, string extradata);
					void submitHighscore(unsigned int tableid, string guestname, unsigned int score, string unit, string extradata);
					static void submitHighscoreInternal(API* api, string result, gjUrlRequest* req);

					void getFriends();
					static void getFriendsInternal(API* api, string result, gjUrlRequest* req);

					void getAchievements();
					void getAchievements(bool achieved);
					static void getAchievementsInternal(API* api, string result, gjUrlRequest* req);

					void sessionOpen();
					void sessionPing(bool active);
					//void sessionCheck();
					void sessionClose();
					
					static void sessionOpenInternal(API* api, string result, gjUrlRequest* req);
					static void sessionPingInternal(API* api, string result, gjUrlRequest* req);
					//static void sessionCheckInternal(API* api, string result, gjUrlRequest* req);
					static void sessionCloseInternal(API* api, string result, gjUrlRequest* req);

					virtual void update();
					virtual void render(GameContainer* container, Renderer* r);

					string url(string method, map<string, string> params, bool addUserToken, bool addKey);
					string md5(string input);

				private:
					void removeRequest(gjUrlRequest* req);
				public:
					static void removeRequestStatic(vector<gjUrlRequest* >* requests, gjUrlRequest* req);

					virtual ~API();
			};

		}
	}
}

#endif

