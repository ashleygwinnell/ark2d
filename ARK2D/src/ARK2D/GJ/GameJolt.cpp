
#include "GameJolt.h"
#include "../../ARK.h"

namespace ARK {
	namespace GJ { 

		GameJolt::GameJolt(unsigned int gameId, string privateKey): 
			m_gameId(gameId), m_privateKey(privateKey),  
			m_username(""), m_userToken(""), 
			m_version(1), m_verbose(false), m_verified(false),
			m_errorMessage("")
			{ 

		}
 
		GameJolt::GameJolt(unsigned int gameId, string privateKey, string username, string userToken): 
			m_gameId(gameId), m_privateKey(privateKey), 
			m_username(username), m_userToken(userToken), 
			m_version(1), m_verbose(false), m_verified(false) 
			{
			verifyUser(username, userToken);
		}

		void GameJolt::setVersion(unsigned int version) {
			m_version = version;
		}
		unsigned int GameJolt::getVersion() {
			return m_version;
		}

		void GameJolt::setVerbose(bool verbose) {
			m_verbose = verbose;
		}
		bool GameJolt::isVerbose() {
			return m_verbose;
		}

		bool GameJolt::isVerified() {
			return m_verified;
		}

		bool GameJolt::verifyUser(string username, string token) {
			m_verified = false;

			map<string, string> params;
			params["username"] = username;
			params["user_token"] = token;

			string json = request("users/auth/", params, false);
			JSONNode* root = libJSON::Parse(json);
			JSONNode* response = root->GetNode("response");
			if (response->GetNode("success")->NodeAsString() == "false") {
				logError(response->GetNode("message")->NodeAsString());
				return false;
			} 
			m_username = username;
    		m_userToken = token;
    		m_verified = true;
			logInformation("Verified User Successfully");
			return true;
			
		}
		User* GameJolt::getVerifiedUser() {
			if (!m_verified) {
				logError("Could not get the (currently verified) user.");
				return NULL;
			}
			
			map<string, string> params;
			params["username"] = m_username;
			params["username"] += m_privateKey;

			string requestUrl = url("users/", params, false);
			
			params["username"] = m_username;
			params["signature"] = md5(requestUrl);

			requestUrl = url("users/", params, false);
			string json = open(requestUrl);
			
			JSONNode* root = libJSON::Parse(json);
			JSONNode* response = root->GetNode("response");
			if (response->GetNode("success")->NodeAsString() == "false") {
				logError(response->GetNode("message")->NodeAsString());
				return NULL;
			} 

			JSONNode* user = response->GetNode("users")->NodeAt(0);

			User* u = new User();
			u->addProperty("id", user->GetNode("id")->NodeAsString());
			u->addProperty("type", user->GetNode("type")->NodeAsString());
			u->addProperty("username", m_username);
			u->addProperty("user_token", m_userToken);
			u->addProperty("avatar_url", user->GetNode("avatar_url")->NodeAsString());
			u->addProperty("signed_up", user->GetNode("signed_up")->NodeAsString());
			u->addProperty("last_logged_in", user->GetNode("last_logged_in")->NodeAsString());
			u->addProperty("status", user->GetNode("status")->NodeAsString());
			//"developer_name": "Ashley Gwinnell",
            //"developer_website": "http://www.ashleygwinnell.co.uk/",
            //"developer_description": "Game Jolt is actually cereal.My name is Ashley. I'm one of the moderators here.You can see that I don't have many games to date. I get busy with real life, and I'm too much of an elitist programmer to learn time-saving tools such as Construct, GM or Stencyl.\\o"
			return u;
		}
		
		bool GameJolt::hasQuickplay() { return false; }
		User* GameJolt::getQuickplayUser() { return NULL; }

		string GameJolt::getErrorMessage() {
			return m_errorMessage;
		}

		vector<Highscore*> GameJolt::getHighscores() { return getHighscores(true, 100); }
		vector<Highscore*> GameJolt::getHighscores(bool all) { return getHighscores(all, 100);   }
		vector<Highscore*> GameJolt::getHighscores(bool all, unsigned int limit) { 
			vector<Highscore*> highscores; 
			if (all == false && !m_verified) { 
				logError("User must be verified to get user Highscores");
				return highscores;
			}

			string json;
			map<string, string> params;
			if (all == true) { // all highscores
				params["limit"] = Cast::toString<unsigned int>(limit);
				string requestUrl = url("scores", params, false, true);
				//ErrorDialog::createAndShow(requestUrl);
				//params["limit"] = Cast::toString<unsigned int>(limit);
				params["signature"] = md5(requestUrl);
				requestUrl = url("scores", params, false);
				//ErrorDialog::createAndShow(requestUrl);
				json = open(requestUrl);
						
			} else { // verified user's highscores.
				params["username"] = m_username;
				params["user_token"] = m_userToken + m_privateKey;  
				params["limit"] = Cast::toString<unsigned int>(limit);
				string requestUrl = url("scores", params, true);
				
				params["user_token"] = m_userToken;  
				params["signature"] = md5(requestUrl);
				requestUrl = url("scores", params, true);
				json = open(requestUrl);
			}
			//ErrorDialog::createAndShow(json);
			logInformation(json);

			JSONNode* root = libJSON::Parse(json);
			if (root == NULL) { return highscores; }

			JSONNode* response = root->GetNode("response");
			if (response->GetNode("success")->NodeAsString() == "false") {
				logError(response->GetNode("message")->NodeAsString());
				m_errorMessage = response->GetNode("message")->NodeAsString();
				return highscores;
			} 

			JSONNode* scores = response->GetNode("scores");
			for(unsigned int i = 0; i < scores->NodeSize(); i++) {
				// {"score":"3 Stars","sort":"3","extra_data":"","user":"","user_id":"","guest":"lol90","stored":"10 hours ago"}
				JSONNode* score = scores->Children[i];
				Highscore* highscore = new Highscore();
				highscore->addProperty("score", score->GetNode("score")->NodeAsString());
				highscore->addProperty("sort", score->GetNode("sort")->NodeAsString());
				highscore->addProperty("extra_data", score->GetNode("extra_data")->NodeAsString());
				highscore->addProperty("user", score->GetNode("user")->NodeAsString());
				highscore->addProperty("user_id", score->GetNode("user_id")->NodeAsString());
				highscore->addProperty("guest", score->GetNode("guest")->NodeAsString());
				highscore->addProperty("stored", score->GetNode("stored")->NodeAsString());

				if (score->GetNode("guest")->NodeAsString().length() > 0) {
					highscore->addProperty("name", score->GetNode("guest")->NodeAsString());					
				} else {
					highscore->addProperty("name", score->GetNode("user")->NodeAsString());
				}
				highscores.push_back(highscore);
			}

			return highscores;  
		}
		
		bool GameJolt::addHighscore(string score, int sort) { return addHighscore(score, sort, ""); }
		bool GameJolt::addHighscore(string score, int sort, string extra) { 
			if (!m_verified) {
				logError("GameJoltAPI: Could not add the High Score because the user is not verified.");
				return false;
			}
			
			map<string, string> params;
			params["username"] = m_username;
			params["user_token"] = m_userToken + m_privateKey;
			params["score"] = score;
			params["extra_data"] = extra;
			params["sort"] = Cast::toString<int>(sort);

			string requestUrl = url("scores/add", params, true);
			params["user_token"] = m_userToken;
			params["signature"] = md5(requestUrl);
				
			requestUrl = url("scores/add", params, true);
			
			string json = open(requestUrl);
			logInformation(json);

			JSONNode* root = libJSON::Parse(json);
			JSONNode* response = root->GetNode("response");

			if (response->GetNode("success")->NodeAsString() == "false") {
				logError(response->GetNode("message")->NodeAsString());
				m_errorMessage = response->GetNode("message")->NodeAsString();
				return false;
			} 
			return true;
		}
		bool GameJolt::addGuestHighscore(string username, string score, int sort) { return addGuestHighscore(username, score, sort, ""); }
		bool GameJolt::addGuestHighscore(string username, string score, int sort, string extra) { 
			
			username = StringUtil::str_replace(" ", "%20", username);
			username = StringUtil::str_replace("\r", "", username);
			username = StringUtil::str_replace("\n", "", username);

			score = StringUtil::str_replace(" ", "%20", score);

			string requestUrlLog1, requestUrlLog2;

			map<string, string> params;
			params["guest"] = username;
			params["score"] = score;
			params["sort"] = Cast::toString<int>(sort);
			if (extra.length() > 0) {
				params["extra_data"] = extra;
			}

			string requestUrl = url("scores/add/", params, false, true);
			requestUrlLog1 = requestUrl;
			
			//params["sort"] = Cast::toString<int>(sort);
			params["signature"] = md5(requestUrl);
			
			requestUrl = url("scores/add/", params, false, false);
			requestUrlLog2 = requestUrl;

			string json = open(requestUrl);
			logInformation(json);
			if (json.length() == 0) {
				return false;
			}
			//ErrorDialog::createAndShow(json);
			JSONNode* root = libJSON::Parse(json);
			JSONNode* response = root->GetNode("response");

			if (response->GetNode("success")->NodeAsString() == "false") {
				logError(response->GetNode("message")->NodeAsString());
				//logError(requestUrlLog1);
				logError(requestUrlLog2);
				m_errorMessage = response->GetNode("message")->NodeAsString();
				return false;
			} 
			return true;
		}

		DataStore* GameJolt::setDataStore(unsigned int type, string key, string value) { return NULL; }
		bool GameJolt::removeDataStore(unsigned int type, string key) { return false; }
		vector<DataStore*> GameJolt::getDataStores(unsigned int type) { vector<DataStore*> ds; return ds; }
		vector<string> GameJolt::getDataStoreKeys(unsigned int type) { vector<string>ds; return ds; }
		DataStore* GameJolt::getDataStore(unsigned int type, string key) { return NULL; }

		bool GameJolt::sessionOpen() { return false; }
		bool GameJolt::sessionUpdate() { return false; }
		bool GameJolt::sessionUpdate(bool active) { return false; }
		bool GameJolt::sessionClose() { return false; }

		string GameJolt::achieveTrophy(Trophy* trophy) { return ""; }
		string GameJolt::achieveTrophy(unsigned int trophyId) { return ""; }
		vector<Trophy*> GameJolt::getTrophies() { vector<Trophy*> ts; return ts; }
		vector<Trophy*> GameJolt::getTrophies(unsigned int achieved) { vector<Trophy*> ts; return ts; }
		Trophy* GameJolt::getTrophy(int trophyId) { return NULL; }

		GameJolt::~GameJolt() {}


		// protected things

		void GameJolt::logError(string message) {
			ARK2D::getLog()->e(message);
		}
		void GameJolt::logWarning(string message) {
			ARK2D::getLog()->w(message);
		}
		void GameJolt::logInformation(string message) {
			ARK2D::getLog()->i(message);
		}

		string GameJolt::request(string method, string params) { 
			// TODO: 
			return "";
		}
		string GameJolt::request(string method, string params, bool requiresVerified) { 
			// TODO:
			return "";
		}

		string GameJolt::request(string method, map<string, string> params) {
			return request(method, params, true);
		}
		string GameJolt::request(string method, map<string, string> params, bool requiresVerified) {

			if (requiresVerified && !m_verified) {
				return "require authentication first";
			}

			if (!m_verified) {
				string user_token = params["user_token"];
				params["user_token"] = params["user_token"] + m_privateKey;

				string urlString = url(method, params);
				string signature = md5(urlString);
				
				params["user_token"] = user_token;
				params["signature"] = signature;
			} else {
				params["user_token"] = m_userToken + m_privateKey;
				params["username"] = m_username;

				string urlString = url(method, params);
				string signature = md5(urlString);
				
				params["user_token"] = m_userToken;
				params["signature"] = signature;
			}

			string urlString = url(method, params);
			return open(urlString);;
		}
		
		string GameJolt::open(string url) {
			URLRequest req;
			req.setUrl(url);
			return req.start(); 
		}
		string GameJolt::url(string method, map<string, string> params) {
			return url(method, params, true);
		}
		string GameJolt::url(string method, map<string, string> params, bool addUserToken) {
			return url(method, params, addUserToken, false);
		}
		string GameJolt::url(string method, map<string, string> params, bool addUserToken, bool addKey) {
			string protocol("http://");
			string apiRoot("gamejolt.com/api/game/");
			string urlString = protocol + apiRoot + string("v") + Cast::toString<unsigned int>(m_version) + string("/") + method + "?game_id=" + Cast::toString<unsigned int>(m_gameId);
			
			string user_token("");
			map<string, string>::iterator it;
			params["format"] = "json";
			for (it = params.begin(); it != params.end(); it++ ) {
				string key = it->first;
				string value = it->second;
				if (key == "user_token") {
					user_token = value;
					continue; 
				}
				urlString += string("&") + key + string("=") + value;
			}
			if (addUserToken) {
				urlString += string("&user_token=") + user_token;
			}
			if (addKey) {
				urlString += m_privateKey;
			}
			return urlString;
		}
		string GameJolt::md5(string input) {
			return ARK::GJ::md5(input);
		}

	}
}
