
#include "GameJolt.h"
#include "../Includes.h"
#include "../Util/Cast.h"
#include "../Util/StringUtil.h"
#include "../Util/URLRequest.h"
#include "../Util/Log.h"
#include "../vendor/tinyxml/tinyxml.h"
#include "../Common/Libraries/libjson.h"

namespace ARK {
	namespace GJ { 

		GameJolt::GameJolt(unsigned int gameId, string privateKey): 
			m_gameId(gameId), 
			m_privateKey(privateKey),  
			m_username(""), 
			m_userToken(""), 
			m_verbose(false),  
			m_verified(false),
			m_errorMessage(""), 
			m_usingFormat(FORMAT_XML),

			m_isBatching(false),
			m_numBatchItems(0),
			m_batchUrl("")
			{ 

		}
 
		GameJolt::GameJolt(unsigned int gameId, string privateKey, string username, string userToken): 
			m_gameId(gameId), 
			m_privateKey(privateKey), 
			m_username(username), 
			m_userToken(userToken), 
			m_verbose(false), 
			m_verified(false),
			m_errorMessage(""), 
			m_usingFormat(FORMAT_XML),

			m_isBatching(false),
			m_numBatchItems(0),
			m_batchUrl("")
			{
			verifyUser(username, userToken);
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
			m_errorMessage = "";

			map<string, string> params;
			params["username"] = username;
			params["user_token"] = token; 

			string json = request("users/auth/", params, false);

			if (m_usingFormat == FORMAT_JSON) { 
				JSONNode* root = libJSON::Parse(json);
				JSONNode* response = root->GetNode("response");
				if (response->GetNode("success")->NodeAsString() == "false") {
					m_errorMessage = response->GetNode("message")->NodeAsString();
					logError(m_errorMessage);
					return false;
				} 
			} else if (m_usingFormat == FORMAT_XML) {
				TiXmlDocument doc;
				doc.Parse(json.c_str(), 0, TIXML_ENCODING_UTF8);

				if (doc.Error()) {
					m_errorMessage = doc.ErrorDesc();
					logError(m_errorMessage);
					return false;
				}
 
				TiXmlNode* responseNode = doc.FirstChild("response");
				TiXmlElement* responseElement = responseNode->ToElement();
				
				TiXmlNode* successNode = responseElement->FirstChild("success");
				TiXmlElement* successElement = successNode->ToElement();
				const char* data_c_str = successElement->GetText();

				if (string(data_c_str) == "false") {
					TiXmlNode* messageNode = responseElement->FirstChild("message");
					TiXmlElement* messageElement = messageNode->ToElement();
					
					m_errorMessage = messageElement->GetText();
					logError(m_errorMessage);
					return false; 
				} 
			}

			m_username = username;
    		m_userToken = token;
    		m_verified = true;
			logInformation("Verified User Successfully");
			return true;
			
		}
		User* GameJolt::getVerifiedUser() {
			m_errorMessage = "";

			if (!m_verified) {
				m_errorMessage = "Could not get the (currently verified) user.";
				logError(m_errorMessage);
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

			if (json.length() == 0) { 
				m_errorMessage = "No internet connection / see log.";
				logError(m_errorMessage);
				return NULL;
			}

			User* u = NULL;
			if (m_usingFormat == FORMAT_JSON) 
			{ 
				JSONNode* root = libJSON::Parse(json);
				if (root == NULL) {
					m_errorMessage = "Could not parse JSON.";
					logError(m_errorMessage);
					return NULL;
				}

				JSONNode* response = root->GetNode("response");
				if (response->GetNode("success")->NodeAsString() == "false") {
					m_errorMessage = response->GetNode("message")->NodeAsString();
					logError(m_errorMessage);
					return NULL;
				} 

				JSONNode* user = response->GetNode("users")->NodeAt(0);

				u = new User();
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
			} else if (m_usingFormat == FORMAT_XML) {
				m_errorMessage = "XML not implemented for getVerifiedUser";
				logError(m_errorMessage);
			}
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
			m_errorMessage = "";

			vector<Highscore*> highscores; 
			if (all == false && !m_verified) { 
				m_errorMessage = "User must be verified to get user Highscores";
				logError(m_errorMessage);
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

			if (json.length() == 0) { 
				m_errorMessage = "No internet connection / see log.";
				logError(m_errorMessage);
				return highscores;
			} 

			if (m_usingFormat == FORMAT_JSON) 
			{

				JSONNode* root = libJSON::Parse(json);
				if (root == NULL) { 
					m_errorMessage = "Could not parse JSON."; 
					logError(m_errorMessage); 
					return highscores; 
				}

				JSONNode* response = root->GetNode("response");
				if (response->GetNode("success")->NodeAsString() == "false") {
					m_errorMessage = response->GetNode("message")->NodeAsString();
					logError(m_errorMessage);
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
			}
			else if (m_usingFormat == FORMAT_XML) 
			{
				m_errorMessage = "XML not implemented for getHighscores";
				logError(m_errorMessage);
			}

			return highscores;  
		}
		
 
		vector<Highscore> GameJolt::getHighscoresInTable(unsigned int tableId, unsigned int page, unsigned int limit)
		{
 			m_errorMessage = "";
 
 			vector<Highscore> highscores; 
			
			string json;
			map<string, string> params;
			params["limit"] = Cast::toString<unsigned int>(limit);
			params["table_id"] = Cast::toString<unsigned int>(tableId);
			
			string requestUrl = url("scores", params, false, true);
			//ErrorDialog::createAndShow(requestUrl);
			//params["limit"] = Cast::toString<unsigned int>(limit); 
			params["signature"] = md5(requestUrl);
			requestUrl = url("scores", params, false);
			
			logInformation(requestUrl);
			
			json = open(requestUrl);
						
			//ErrorDialog::createAndShow(json);
			logInformation(json);

			if (json.length() == 0) {
				m_errorMessage = "No internet connection / see log.";
				logError(m_errorMessage);
				return highscores;
			}

			if (m_usingFormat == FORMAT_JSON) 
			{

				JSONNode* root = libJSON::Parse(json);
				if (root == NULL) { 
					m_errorMessage = "Could not parse JSON.";
					logError(m_errorMessage); 
					return highscores; 
				}

				JSONNode* response = root->GetNode("response");
				if (response == NULL) {
					m_errorMessage = "Could not parse JSON. No response tag.";
					logError(m_errorMessage); 
					return highscores; 
				}
				if (response->GetNode("success")->NodeAsString() == "false") {
					logError(response->GetNode("message")->NodeAsString());
					m_errorMessage = response->GetNode("message")->NodeAsString();

					libJSON::Delete(root);
					root = NULL;

					return highscores;
				} 

				JSONNode* scores = response->GetNode("scores");
				for(unsigned int i = 0; i < scores->NodeSize(); i++) {
					// {"score":"3 Stars","sort":"3","extra_data":"","user":"","user_id":"","guest":"lol90","stored":"10 hours ago"}
					JSONNode* score = scores->Children[i];
					Highscore highscore;
					highscore.addProperty("score", score->GetNode("score")->NodeAsString());
					highscore.addProperty("sort", score->GetNode("sort")->NodeAsString());
					highscore.addProperty("extra_data", score->GetNode("extra_data")->NodeAsString());
					highscore.addProperty("user", score->GetNode("user")->NodeAsString());
					highscore.addProperty("user_id", score->GetNode("user_id")->NodeAsString());
					highscore.addProperty("guest", score->GetNode("guest")->NodeAsString());
					highscore.addProperty("stored", score->GetNode("stored")->NodeAsString());

					if (score->GetNode("guest")->NodeAsString().length() > 0) {
						highscore.addProperty("name", score->GetNode("guest")->NodeAsString());					
					} else {
						highscore.addProperty("name", score->GetNode("user")->NodeAsString());
					}
					highscores.push_back(highscore);
				} 

				libJSON::Delete(root);
				root = NULL;

			} 
			else if (m_usingFormat == FORMAT_XML) 
			{
				/*
				
				<?xml version="1.0" encoding="UTF-8"?>
				<response>
					<success><![CDATA[true]]></success>
					<scores>
						<score>
							<score>100</score>
							<sort>100</sort>
							<extra_data><![CDATA[{\"name\": \"William\", \"hat\": 0, \"face\": 8,\"misc\": 0,\"duration\": 10}]]></extra_data>
							<user></user>
							<user_id></user_id>
							<guest><![CDATA[William]]></guest>
							<stored><![CDATA[3 seconds ago]]></stored>
						</score>
					</scores>
				</response>
				
				*/

				//m_errorMessage = "XML not implemented for getHighscoresInTable";
				//logError(m_errorMessage);

				/*xml_document<> xmldocument;
				
				xmldocument.parse<0>((char*)json.c_str()); 

				xml_node<>* responseNode = xmldocument.first_node("response");
				xml_node<>* successNode = responseNode->first_node("success");

				const char* successNode_cstr = successNode->value();
	 			string successNode_str = string(successNode_cstr);

	 			if (successNode_str == "false") {
	 				xml_node<>* messageNode = responseNode->first_node("message");
	 				const char* messageNode_cstr = messageNode->value();
	 				m_errorMessage = string(messageNode_cstr);

	 				logError(m_errorMessage);
					return highscores; 
	 			}

	 			// parse data now
				xml_node<>* scoresNode = responseNode->first_node("scores");
				
				// layers please!
				xml_node<>* scoreNode = 0;
				for (scoreNode = scoresNode->first_node("score");
					scoreNode;
					scoreNode = scoreNode->next_sibling("score")) 
				{
					Highscore highscore;   
					highscore.addProperty("score", scoreNode->first_node("score")->value() );
					highscore.addProperty("sort", scoreNode->first_node("sort")->value() );
					highscore.addProperty("extra_data", scoreNode->first_node("extra_data")->value() );
					
					if (scoreNode->first_node("user") != NULL) { 
						highscore.addProperty("user", scoreNode->first_node("user")->value());
					} else {
						highscore.addProperty("user", "");
					}

					if (scoreNode->first_node("user_id") != NULL) { 
						highscore.addProperty("user_id", scoreNode->first_node("user_id")->value());
					} else {
						highscore.addProperty("user_id", "");
					}

					if (scoreNode->first_node("guest") != NULL) { 
						highscore.addProperty("guest", scoreNode->first_node("guest")->value());
					} else {
						highscore.addProperty("guest", "");
					}

					highscore.addProperty("stored", scoreNode->first_node("stored")->value() );

					bool isGuest = scoreNode->first_node("guest") != NULL;
					if (isGuest) {
						if (strlen(scoreNode->first_node("guest")->value()) > 0) {
							isGuest = true;
						} else { 
							isGuest = false;
						}
					}
					if (isGuest) { 
						highscore.addProperty("name", scoreNode->first_node("guest")->value() );
					} else { 
						highscore.addProperty("name", scoreNode->first_node("user")->value() );
					}
					//highscore.addProperty("score", "122");
					//highscore.addProperty("name", "Boo");   
					highscores.push_back(highscore);	
				}
				
				*/

				
				// ------------------------
				//	TINYXML
				// ------------------------

				TiXmlDocument doc;
				doc.Parse(json.c_str(), 0, TIXML_ENCODING_UTF8);

				if (doc.Error()) {
					m_errorMessage = doc.ErrorDesc();
					logError(m_errorMessage);
					return highscores;
				}
 
				TiXmlNode* responseNode = doc.FirstChild("response");
				TiXmlElement* responseElement = responseNode->ToElement();
				
				TiXmlNode* successNode = responseElement->FirstChild("success");
				TiXmlElement* successElement = successNode->ToElement();
				const char* data_c_str = successElement->GetText();

				if (string(data_c_str) == "false") {
					TiXmlNode* messageNode = responseElement->FirstChild("message");
					TiXmlElement* messageElement = messageNode->ToElement();
					
					m_errorMessage = messageElement->GetText();
					logError(m_errorMessage);
					return highscores; 
				} 
 
				// parse data now
				TiXmlNode* scoresNode = responseElement->FirstChild("scores");
				TiXmlElement* scoresElement = scoresNode->ToElement();

				// layers please!
				TiXmlNode* scoreNode = 0;
				for (scoreNode = scoresElement->FirstChild("score");
								scoreNode;  
								scoreNode = scoreNode->NextSibling("score")) 
				{
					Highscore highscore;   
					highscore.addProperty("score", scoreNode->FirstChild("score")->ToElement()->GetText() );
					highscore.addProperty("sort", scoreNode->FirstChild("sort")->ToElement()->GetText() );
					highscore.addProperty("extra_data", scoreNode->FirstChild("extra_data")->ToElement()->GetText() );
					
					if (scoreNode->FirstChild("user")->ToElement()->GetText() != NULL) { 
						highscore.addProperty("user", scoreNode->FirstChild("user")->ToElement()->GetText());
					} else {
						highscore.addProperty("user", "");
					}

					if (scoreNode->FirstChild("user_id")->ToElement()->GetText() != NULL) { 
						highscore.addProperty("user_id", scoreNode->FirstChild("user_id")->ToElement()->GetText());
					} else {
						highscore.addProperty("user_id", "");
					}

					if (scoreNode->FirstChild("guest")->ToElement()->GetText() != NULL) { 
						highscore.addProperty("guest", scoreNode->FirstChild("guest")->ToElement()->GetText());
					} else {
						highscore.addProperty("guest", "");
					}

					highscore.addProperty("stored", scoreNode->FirstChild("stored")->ToElement()->GetText() );

					const char* guestStr = scoreNode->FirstChildElement("guest")->GetText();
					if (guestStr != NULL && strlen(guestStr) > 0) { 
						highscore.addProperty("name", scoreNode->FirstChild("guest")->ToElement()->GetText() );
					} else { 
						highscore.addProperty("name", scoreNode->FirstChild("user")->ToElement()->GetText() );
					}
					//highscore.addProperty("score", "122");
					//highscore.addProperty("name", "Boo");   
					highscores.push_back(highscore);	
 
				}

				
				
			}

			return highscores;   
		}

		// get rank
		//unsigned int getRank(signed int score);
		unsigned int GameJolt::getRank(signed int score, unsigned int tableId) 
		{
			m_errorMessage = "";

			if (score == 0) { 
				m_errorMessage = "Cannot get rank with a score of zero.";
				logError(m_errorMessage);
				return 0;
			}
 			
			string json;
			map<string, string> params;
			params["sort"] = Cast::toString<signed int>(score);
			params["table_id"] = Cast::toString<unsigned int>(tableId);
			
			string requestUrl = url("scores/get-rank", params, false, true);
			params["signature"] = md5(requestUrl);
			requestUrl = url("scores/get-rank", params, false);
			
			logInformation(requestUrl);
			
			json = open(requestUrl);
						
			//ErrorDialog::createAndShow(json);
			logInformation(json);

			if (json.length() == 0) {
				m_errorMessage = "No internet connection / see log.";
				logError(m_errorMessage);
				return 0;
			}

			if (m_usingFormat == FORMAT_JSON)
			{
				JSONNode* root = libJSON::Parse(json);
				if (root == NULL) {
					m_errorMessage = "Could not parse JSON.";
					logError(m_errorMessage);
					return 0;
				}

				JSONNode* response = root->GetNode("response");
				if (response->GetNode("success")->NodeAsString() == "false") {
					m_errorMessage = response->GetNode("message")->NodeAsString();
					logError(m_errorMessage);
					return 0;
				} 

				return response->GetNode("rank")->NodeAsInt();
			}
			else if (m_usingFormat == FORMAT_XML) 
			{

				/*
					SUCCESS:
					<?xml version="1.0" encoding="UTF-8"?>
					<response>
						<success><![CDATA[true]]></success>
						<rank>5</rank>
					</response>
					
					FAIL:
					<?xml version="1.0" encoding="UTF-8"?>
					<response>
						<success><![CDATA[false]]></success>
						<message><![CDATA[Could not get a rank for the parameters you entered.]]></message>
					</response>
				*/

				TiXmlDocument doc;
				doc.Parse(json.c_str(), 0, TIXML_ENCODING_UTF8);

				if (doc.Error()) {
					m_errorMessage = doc.ErrorDesc();
					logError(m_errorMessage);
					return 0;
				}  
 
				TiXmlNode* responseNode = doc.FirstChild("response");
				TiXmlElement* responseElement = responseNode->ToElement();
				
				TiXmlNode* successNode = responseElement->FirstChild("success");
				TiXmlElement* successElement = successNode->ToElement();
				const char* data_c_str = successElement->GetText();

				if (string(data_c_str) == "false") {
					TiXmlNode* messageNode = responseElement->FirstChild("message");
					TiXmlElement* messageElement = messageNode->ToElement();
					 
					m_errorMessage = messageElement->GetText();
					logError(m_errorMessage); 
					return 0; 
				} 

				TiXmlNode* rankNode = responseElement->FirstChild("rank");
				TiXmlElement* rankElement = rankNode->ToElement();
				return Cast::fromString<signed int>( rankElement->GetText() );
			}
			m_errorMessage = "Invalid Format";
			logError(m_errorMessage); 
			return 0;
		}
		
		bool GameJolt::addHighscore(string score, int sort) { return addHighscore(score, sort, ""); }
		bool GameJolt::addHighscore(string score, int sort, string extra) { 
			m_errorMessage = "";

			if (!m_verified) {
				m_errorMessage = "GameJoltAPI: Could not add the High Score because the user is not verified.";
				logError(m_errorMessage);
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

			if (m_errorMessage.length() > 0) {
				return false;
			}
 
			if (json.length() == 0) {
				m_errorMessage = "No internet connection!";
				logError(m_errorMessage);
				return false;
			}

			if (m_usingFormat == FORMAT_JSON) 
			{
				JSONNode* root = libJSON::Parse(json);
				if (root == NULL) {
					m_errorMessage = "Could not parse JSON";
					logError(m_errorMessage);
					return false;
				}

				JSONNode* response = root->GetNode("response");
				if (response->GetNode("success")->NodeAsString() == "false") {
					logError(response->GetNode("message")->NodeAsString());
					m_errorMessage = response->GetNode("message")->NodeAsString();
					return false;
				} 
			}
			else if (m_usingFormat == FORMAT_XML) 
			{
				TiXmlDocument doc;
				doc.Parse(json.c_str(), 0, TIXML_ENCODING_UTF8);

				if (doc.Error()) {
					m_errorMessage = doc.ErrorDesc();
					logError(m_errorMessage);
					return false;
				}
 
				TiXmlNode* responseNode = doc.FirstChild("response");
				TiXmlElement* responseElement = responseNode->ToElement();
				
				TiXmlNode* successNode = responseElement->FirstChild("success");
				TiXmlElement* successElement = successNode->ToElement();
				const char* data_c_str = successElement->GetText();

				if (string(data_c_str) == "false") {
					TiXmlNode* messageNode = responseElement->FirstChild("message");
					TiXmlElement* messageElement = messageNode->ToElement();
					
					m_errorMessage = messageElement->GetText();
					logError(m_errorMessage);
					return false; 
				} 
			}
			return true; 
		}

		bool GameJolt::addGuestHighscore(string username, string score, int sort) { 
			return addGuestHighscore(username, score, sort, ""); 
		}
		bool GameJolt::addGuestHighscore(string username, string score, int sort, string extra) { 
			return addGuestHighscore(username, score, sort, "", 0);
		}
		bool GameJolt::addGuestHighscore(string username, string score, int sort, string extra, unsigned int tableId)
		{
			m_errorMessage = "";

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
			if (tableId > 0) { 
				params["table_id"] = Cast::toString<unsigned int>(tableId); 
			}

			string requestUrl = url("scores/add/", params, false, true);
			requestUrlLog1 = requestUrl;
			
			//params["sort"] = Cast::toString<int>(sort);
			params["signature"] = md5(requestUrl);
			
			requestUrl = url("scores/add/", params, false, false);
			requestUrlLog2 = requestUrl;

			//ARK2D::getLog()->v(requestUrl);

			string json = open(requestUrl);
			logInformation(json);

			if (m_errorMessage.length() > 0) { 
				return false;
			}

			if (json.length() == 0) {
				m_errorMessage = "No internet connection.";
				logError(m_errorMessage);
				return false;
			}

			if (m_usingFormat == FORMAT_JSON) 
			{
				//ErrorDialog::createAndShow(json);
				JSONNode* root = libJSON::Parse(json);
				if (root == NULL) {
					m_errorMessage = "Could not parse JSON.";
					logError(m_errorMessage);
					return false;
				}
				JSONNode* response = root->GetNode("response");

				if (response->GetNode("success")->NodeAsString() == "false") {
					logError(requestUrlLog2);
					m_errorMessage = response->GetNode("message")->NodeAsString();
					logError(m_errorMessage);
					return false;
				} 
			}
			else if (m_usingFormat == FORMAT_XML) 
			{
				TiXmlDocument doc;
				doc.Parse(json.c_str(), 0, TIXML_ENCODING_UTF8);

				if (doc.Error()) {
					m_errorMessage = doc.ErrorDesc();
					logError(m_errorMessage);
					return false;
				}
 
				TiXmlNode* responseNode = doc.FirstChild("response");
				TiXmlElement* responseElement = responseNode->ToElement();
				
				TiXmlNode* successNode = responseElement->FirstChild("success");
				TiXmlElement* successElement = successNode->ToElement();
				const char* data_c_str = successElement->GetText();

				if (string(data_c_str) == "false") {
					TiXmlNode* messageNode = responseElement->FirstChild("message");
					TiXmlElement* messageElement = messageNode->ToElement();
					
					m_errorMessage = messageElement->GetText();
					logError(m_errorMessage);
					return false; 
				} 
			}
			return true;
		}

		DataStore* GameJolt::setDataStore(unsigned int type, string key, string value) { 
			m_errorMessage = "";

			string requestUrlLog1, requestUrlLog2;

			map<string, string> params;
			params["game_id"] = Cast::toString<unsigned int>(m_gameId);
			params["key"] = key;
			params["data"] = value;
			
			string requestUrl = url("data-store/set/", params, false, true);
			requestUrlLog1 = requestUrl;
			
			//params["sort"] = Cast::toString<int>(sort);
			params["signature"] = md5(requestUrl);
			
			requestUrl = url("data-store/set/", params, false, false);
			requestUrlLog2 = requestUrl;

			string json = open(requestUrl); 
			logInformation(json);
			if (json.length() == 0) {
				m_errorMessage = "No internet connection / see log.";
				logError(m_errorMessage);
				return NULL; 
			}

			if (m_usingFormat == FORMAT_JSON) { 
				//ErrorDialog::createAndShow(json);
				JSONNode* root = libJSON::Parse(json);
				if (root == NULL) {
					m_errorMessage = "Could not parse JSON.";
					logError(m_errorMessage);
					return NULL;
				}

				JSONNode* response = root->GetNode("response");
				if (response->GetNode("success")->NodeAsString() == "false") {
					logError(response->GetNode("message")->NodeAsString());
					//logError(requestUrlLog1);
					logError(requestUrlLog2);
					m_errorMessage = response->GetNode("message")->NodeAsString();
					return NULL;
				} 
				
			} else if (m_usingFormat == FORMAT_XML) {

				TiXmlDocument doc;
				doc.Parse(json.c_str(), 0, TIXML_ENCODING_UTF8);

				if (doc.Error()) {
					m_errorMessage = doc.ErrorDesc();
					logError(m_errorMessage);
					return NULL;
				}
 
				TiXmlNode* responseNode = doc.FirstChild("response");
				TiXmlElement* responseElement = responseNode->ToElement();
				
				TiXmlNode* successNode = responseElement->FirstChild("success");
				TiXmlElement* successElement = successNode->ToElement();
				const char* data_c_str = successElement->GetText();

				if (string(data_c_str) == "false") {
					TiXmlNode* messageNode = responseElement->FirstChild("message");
					TiXmlElement* messageElement = messageNode->ToElement();
					
					m_errorMessage = messageElement->GetText();
					logError(m_errorMessage);
					return NULL;
				} 
			} 
			// TODO: cacheing... 
			return new DataStore(key, value, type);

		}
		bool GameJolt::removeDataStore(unsigned int type, string key) { return false; }
		vector<DataStore*> GameJolt::getDataStores(unsigned int type) { vector<DataStore*> ds; return ds; }
		vector<string> GameJolt::getDataStoreKeys(unsigned int type) { vector<string>ds; return ds; }
		DataStore* GameJolt::getDataStore(unsigned int type, string key) { 
			m_errorMessage = "";

			string requestUrlLog1, requestUrlLog2;

			map<string, string> params;
			params["game_id"] = Cast::toString<unsigned int>(m_gameId);
			params["key"] = key;
			
			string requestUrl = url("data-store/", params, false, true);
			requestUrlLog1 = requestUrl;
			
			//params["sort"] = Cast::toString<int>(sort);
			params["signature"] = md5(requestUrl);
			
			requestUrl = url("data-store/", params, false, false);
			requestUrlLog2 = requestUrl;

			string json = open(requestUrl);
			logInformation(json);
			if (json.length() == 0) { 
				m_errorMessage = "No internet connection / see log.";
				logError(m_errorMessage);
				return NULL;
			}

			string valueStr("");

			if (m_usingFormat == FORMAT_JSON) { 

				//ErrorDialog::createAndShow(json);
				logInformation("creating json node from above str");
				JSONNode* root = libJSON::Parse(json); 
				if (root == NULL) { 
					m_errorMessage = "Could not parse JSON / see log.";
					logError(m_errorMessage); 
					return NULL; 
				}
				JSONNode* response = root->GetNode("response");

				logInformation("checking response code");
				if (response->GetNode("success")->NodeAsString() == "false") {
					logError(response->GetNode("message")->NodeAsString());
					//logError(requestUrlLog1);
					logError(requestUrlLog2);
					m_errorMessage = response->GetNode("message")->NodeAsString();
					return NULL;
				} 

				valueStr = response->GetNode("data")->NodeAsString();

				//ARK2D::getLog()->i("libjson::delete");
				libJSON::Delete(root); 

			} else if (m_usingFormat == FORMAT_XML) {
				TiXmlDocument doc;
				doc.Parse(json.c_str(), 0, TIXML_ENCODING_UTF8);

				if (doc.Error()) {
					m_errorMessage = doc.ErrorDesc();
					logError(m_errorMessage);
					return NULL;
				}

				TiXmlNode* responseNode = doc.FirstChild("response");
				TiXmlElement* responseElement = responseNode->ToElement();
				
				TiXmlNode* successNode = responseElement->FirstChild("success");
				TiXmlElement* successElement = successNode->ToElement();
				const char* data_c_str = successElement->GetText();

				if (string(data_c_str) == "false") {
					TiXmlNode* messageNode = responseElement->FirstChild("message");
					TiXmlElement* messageElement = messageNode->ToElement();
					
					m_errorMessage = messageElement->GetText();
					logError(m_errorMessage);
					return NULL;
				} 

				TiXmlNode* dataNode = responseElement->FirstChild("data");
				TiXmlElement* dataElement = dataNode->ToElement();
				valueStr += dataElement->GetText();
 
			}
				 
			// TODO: cacheing... 
			//ARK2D::getLog()->v("returning data store");
			return new DataStore(key, valueStr, type);

		}

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
			if (URLRequest::isThreadedStatic()) { ARK2D::getLog()->t(message); return; }
			ARK2D::getLog()->e(message);
		}
		void GameJolt::logWarning(string message) {
			if (URLRequest::isThreadedStatic()) { ARK2D::getLog()->t(message); return; }
			ARK2D::getLog()->w(message);
		}
		void GameJolt::logInformation(string message) {
			if (URLRequest::isThreadedStatic()) { ARK2D::getLog()->t(message); return; }
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
			string resp = req.start(); 
			if (req.hasError()) {
				m_errorMessage = req.getError();
				logError(m_errorMessage);
			}
			return resp;
		}
		string GameJolt::url(string method, map<string, string> params) {
			return url(method, params, true);
		}
		string GameJolt::url(string method, map<string, string> params, bool addUserToken) {
			return url(method, params, addUserToken, false);
		}
		string GameJolt::url(string method, map<string, string> params, bool addUserToken, bool addKey) {
			string protocol("http://"); 
			string apiRoot("api.gamejolt.com/api/game/");
			string urlString = protocol + apiRoot + string("v1_1/") + method + "?game_id=" + Cast::toString<unsigned int>(m_gameId);
			
			string user_token("");
			map<string, string>::iterator it;
			params["format"] = getFormatString();
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




		void GameJolt::startBatch() {
			/*
				http://gamejolt.com/api/game/v1_1/batch/
				?requests[]=http%3A%2F%2Fgamejolt.com%2Fapi%2Fgame%2Fv1_1%2Fdata-store%2Fset%2F%3Fkey%3Dglobally%26data%3Dthis%2Bis%2Bthe%2Bdata%26game_id%3D937%26format%3Djson%26signature%3Da0dd0d0126e11cc24a10b9aee391595d
				&requests[]=http%3A%2F%2Fgamejolt.com%2Fapi%2Fgame%2Fv1_1%2Fdata-store%2Fget%2F%3Fkey%3Dglobally%26game_id%3D937%26format%3Djson%26signature%3Dfad1c3762d4c1b52eafa48774d5f72bc
				&game_id=937
				&format=json
				&signature=42b74b968152b291e031972525991d8e
			*/
			m_isBatching = true;
			m_numBatchItems = 0;
			m_batchUrl = "http://api.gamejolt.com/api/game/v1_1/batch/";

			
			
		}
		string GameJolt::endBatch() {
			return m_batchUrl;
			//m_batchUrl += "&game_id=" + Cast::toString<unsigned int>(m_gameId);
			//m_batchUrl += "&format=json";
			//m_batchUrl += "&signature=xxxxxxxxxx";

			m_isBatching = false;
			return "";
		}
		bool GameJolt::isBatching() {
			return m_isBatching;
		}

	}
}
