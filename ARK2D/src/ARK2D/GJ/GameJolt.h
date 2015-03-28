/*
 * GameJolt.h
 *
 *  Created on: Jun 26, 2012
 *      Author: ashleygwinnell
 */

#ifndef ARK_GJ_GAMEJOLT_H_
#define ARK_GJ_GAMEJOLT_H_

#include "../Includes.h"
#include "../Namespaces.h"

#include "DataStore.h"
#include "Highscore.h"
#include "Trophy.h"
#include "User.h"
#include "MD5.h"

namespace ARK {
	namespace GJ {

		class ARK2D_API GameJolt {

			private: // protected properties
				unsigned int m_gameId;
				string m_privateKey;

				string m_username;
				string m_userToken;

				bool m_verbose;
				bool m_verified;

				string m_errorMessage;

				unsigned int m_usingFormat;
			public:
				static const unsigned int FORMAT_PLAINTEXT = 0;
				static const unsigned int FORMAT_JSON = 1;
				static const unsigned int FORMAT_XML = 2;

			protected: // protected functions
				string md5(string input);
				string open(string url);
				string url(string method, map<string, string> params);
				string url(string method, map<string, string> params, bool addUserToken);
				string url(string method, map<string, string> params, bool addUserToken, bool addKey);
				string request(string method, map<string, string> params);
				string request(string method, map<string, string> params, bool requiresVerified);
				void logError(string message);
				void logWarning(string message);
				void logInformation(string message);

				string getFormatString() {
					if (m_usingFormat == FORMAT_PLAINTEXT) { 
						return "keypair";
					} else if (m_usingFormat == FORMAT_JSON) {
						return "json";
					} else if (m_usingFormat == FORMAT_XML) {
						return "xml";
					}
					return "keypair";
				}


			public: // public functions
				GameJolt(unsigned int gameId, string privateKey);
				GameJolt(unsigned int gameId, string privateKey, string username, string userToken);

				void setVerbose(bool verbose);
				bool isVerbose();

				bool isVerified();

				bool hasQuickplay();
				User* getQuickplayUser();

				User* getVerifiedUser();
				bool verifyUser(string username, string token);

				string getErrorMessage();
 
				vector<Highscore> getHighscoresInTable(unsigned int tableId, unsigned int page, unsigned int limit);
				vector<Highscore*> getHighscores();
				vector<Highscore*> getHighscores(bool all);
				vector<Highscore*> getHighscores(bool all, unsigned int limit);
				bool addHighscore(string score, int sort);
				bool addHighscore(string score, int sort, string extra);
				bool addGuestHighscore(string username, string score, int sort);
				bool addGuestHighscore(string username, string score, int sort, string extra); 
				bool addGuestHighscore(string username, string score, int sort, string extra, unsigned int tableId);

				DataStore* setDataStore(unsigned int type, string key, string value);
				bool removeDataStore(unsigned int type, string key);
				vector<DataStore*> getDataStores(unsigned int type);
				vector<string> getDataStoreKeys(unsigned int type);
				DataStore* getDataStore(unsigned int type, string key);

				bool sessionOpen();
				bool sessionUpdate();
				bool sessionUpdate(bool active);
				bool sessionClose();

				inline void setFormat(unsigned int format) { m_usingFormat = format; }

				bool m_isBatching;
				int m_numBatchItems;
				string m_batchUrl;
				void startBatch();
				string endBatch();
				bool isBatching();

				// get rank
				//unsigned int getRank(signed int score);
				unsigned int getRank(signed int score, unsigned int tableId);

				string achieveTrophy(Trophy* trophy);
				string achieveTrophy(unsigned int trophyId);
				vector<Trophy*> getTrophies();
				vector<Trophy*> getTrophies(unsigned int achieved);
				Trophy* getTrophy(int trophyId);

				string request(string method, string params);
				string request(string method, string params, bool requiresVerified);
				
				virtual ~GameJolt();
		};
	}
}

#endif /* ARK_GJ_GAMEJOLT_H_ */
