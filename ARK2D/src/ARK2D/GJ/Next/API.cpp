/*
 * API.cpp
 *
 *  Created on: 18 Jan 2015
 *      Author: Ashley
 */

#include "API.h"

namespace ARK {
	namespace GJ {
		namespace Next {

			void __internalGameJoltListener(ARK::GJ::Next::API* api, gjCallbackType type, void* data)
			{
				switch(type) {
					case GJ_USERS_AUTH_RESULT: 
					{
						gjUsersAuthResult* result = (gjUsersAuthResult*) data;
						if (!result->success) {
							ARK2D::getLog()->v(result->message);
							return;
						}
						api->m_userName = result->username;
						api->m_userToken = result->usertoken;
						
						/*Overlay* o = Overlay::getInstance();
						if (o != NULL) {

						}*/
						break;
					}
					case GJ_GENERAL_RESULT:
					case GJ_HIGHSCORES_RESULT:
					case GJ_HIGHSCORE_SUBMIT_RESULT:
					case GJ_HIGHSCORE_RANK_RESULT:
					case GJ_HIGHSCORE_TABLES_RESULT:
					case GJ_ACHIEVEMENTS_RESULT:
					case GJ_ACHIEVEMENT_SUBMIT_RESULT:
					case GJ_SESSION_OPEN_RESULT:
					case GJ_SESSION_PING_RESULT:
					case GJ_SESSION_CLOSE_RESULT:
					break;
				}
			}
			// 

			// Rapid XML util
			string rapidXmlUtil_value(xml_node<>* node) {
				string val = node->value();
				if (val.length() == 0) {
					if (node->first_node() != NULL) {
						val = node->first_node()->value();
					}
				}
				return val;
			}
			unsigned int rapidXmlUtil_countChildren(xml_node<>* node, string childname) {
				unsigned int i = 0;
				xml_node<>* child = 0;
				for (child = node->first_node(childname.c_str());
					child != NULL;
					child = child->next_sibling(childname.c_str())) {
					i++;
				}
				return i;

			}

			// ------------------------------------------------------------------
			// Objects (Highscore, Achievement, User)
			// ------------------------------------------------------------------

			// ------------------------------------------------------------------
			// Achievement
			// ------------------------------------------------------------------
			gjAchievement* gjAchievement_create() {
				gjAchievement* achievement = (gjAchievement*) malloc(sizeof(gjAchievement));
				achievement->id = 0;
				achievement->name = (char*) malloc(sizeof(unsigned char) * 255);
				achievement->description = (char*) malloc(sizeof(unsigned char) * 1024);
				achievement->difficulty = GJ_DIFFICULTY_BRONZE;
				achievement->imageUrl = (char*) malloc(sizeof(unsigned char) * 1024);
				achievement->achieved = false;
				achievement->achievedOn = (char*) malloc(sizeof(unsigned char) * 255);
				return achievement;
			}
			gjAchievement* gjAchievement_copy(gjAchievement* ach) {
				gjAchievement* ret = gjAchievement_create();
				ret->id = ach->id;
				memcpy(ret->name, ach->name, 255);
				memcpy(ret->description, ach->description, 1024);
				ret->difficulty = ach->difficulty; 
				memcpy(ret->imageUrl, ach->imageUrl, 1024);
				ret->achieved = ach->achieved;
				memcpy(ret->achievedOn, ach->achievedOn, 255);
				return ret;
			}
			const char* gjAchievement_getName(gjAchievement* achievement) {
				return achievement->name;
			}
			void gjAchievement_dispose(gjAchievement* achievement) {
				free((void*)achievement->name);
				achievement->name = NULL;

				free((void*)achievement->description);
				achievement->description = NULL;

				free((void*)achievement->imageUrl);
				achievement->imageUrl = NULL;

				free((void*)achievement->achievedOn);
				achievement->achievedOn = NULL;				

				free(achievement);
				achievement = NULL;
			}

			// ------------------------------------------------------------------
			// Highscores
			// ------------------------------------------------------------------
			gjHighscore* gjHighscore_create() { 
				gjHighscore* highscore = (gjHighscore*) malloc(sizeof(gjHighscore));
				highscore->userid = 0;
				highscore->username = (char*) malloc(sizeof(unsigned char) * 255);
				highscore->guest = false;
				highscore->guestname = (char*) malloc(sizeof(unsigned char) * 255);
				highscore->globalRank = 0;
				highscore->score = 0;
				return highscore;
			}
			gjHighscore* gjHighscore_copy(gjHighscore* highscore) {
				gjHighscore* copy = gjHighscore_create();
				copy->userid = highscore->userid;
				memcpy((void*) copy->username, highscore->username, 255);
				copy->guest = highscore->guest;
				memcpy((void*) copy->guestname, highscore->guestname, 255);
				copy->globalRank = highscore->globalRank;
				copy->score = highscore->score;
				return copy; 
			}
			const char* gjHighscore_getName(gjHighscore* highscore) {
				if (highscore->guest) {
					return highscore->guestname;
				}
				return highscore->username;
			}
			void gjHighscore_dispose(gjHighscore* highscore) {
				free((void*) highscore->username);
				highscore->username = NULL;
				free((void*) highscore->guestname);
				highscore->guestname = NULL;
				free(highscore);
				highscore = NULL;
			}

			gjHighscoreTable* gjHighscoreTable_create() { 
				gjHighscoreTable* table = (gjHighscoreTable*) malloc(sizeof(gjHighscoreTable));
				table->id = 0;
				table->name = (char*) malloc(sizeof(unsigned char) * 255);
				table->description = (char*) malloc(sizeof(unsigned char) * 1024);
				table->primary = false;
				return table;
			}
			gjHighscoreTable* gjHighscoreTable_copy(gjHighscoreTable* table) {
				gjHighscoreTable* copy = gjHighscoreTable_create();
				copy->id = table->id;
				memcpy((void*)copy->name, table->name, 255);
				memcpy((void*)copy->description, table->description, 1024);
				copy->primary = table->primary;
				return copy; 
			}
			void gjHighscoreTable_dispose(gjHighscoreTable* table) {
				free((void*) table->name);
				table->name = NULL;
				free((void*) table->description);
				table->description = NULL;
				free(table);
				table = NULL;
			}

			// ------------------------------------------------------------------
			// Generic / empty result
			// ------------------------------------------------------------------
			gjGeneralResult* gjGeneralResult_create() {
				gjGeneralResult* res = (gjGeneralResult*) malloc(sizeof(gjGeneralResult));
				res->success = true;
				res->message = (char*) malloc(sizeof(unsigned char) * 255);
				return res;
			}
			void gjGeneralResult_dispose(gjGeneralResult* res) {
				free(res->message);
				res->message = NULL;
				free(res);
				res = NULL;
			}

			
			// ------------------------------------------------------------------
			// Highscores Results
			// ------------------------------------------------------------------
			gjHighscoresResult* gjHighscoresResult_create(unsigned int scoresCount) {
				gjHighscoresResult* res = (gjHighscoresResult*) malloc(sizeof(gjHighscoresResult));
				res->success = true;
				res->message = (char*) malloc(sizeof(unsigned char) * 255);
				res->scoresCount = scoresCount;
				if (res->scoresCount > 0) {
					res->scores = (gjHighscore**) malloc(sizeof(gjHighscore*) * scoresCount);
				}
				return res;
			}
			gjHighscoresResult* gjHighscoresResult_copy(gjHighscoresResult* result) {
				gjHighscoresResult* copy = gjHighscoresResult_create(result->scoresCount);
				copy->success = result->success;
				memcpy(copy->message, result->message, 255);
				copy->scoresCount = result->scoresCount;
				for(unsigned int i = 0; i < result->scoresCount; ++i) {
					copy->scores[i] = gjHighscore_copy(result->scores[i]);
				}
				return copy;
			}
			void gjHighscoresResult_dispose(gjHighscoresResult* res) {
				free((void*) res->message);
				res->message = NULL;
				for(unsigned int i = 0; i < res->scoresCount; ++i) {
					gjHighscore_dispose(res->scores[i]);
				}
				if (res->scoresCount > 0) {
					free(res->scores);
				}
				res->scores = NULL;
				free(res);
				res = NULL;
			}

			// ------------------------------------------------------------------
			// Highscore Tables Results
			// ------------------------------------------------------------------
			gjHighscoreTablesResult* gjHighscoreTablesResult_create(unsigned int tablesCount) {
				gjHighscoreTablesResult* res = (gjHighscoreTablesResult*) malloc(sizeof(gjHighscoreTablesResult));
				res->success = true;
				res->message = (char*) malloc(sizeof(unsigned char) * 255);
				res->tablesCount = tablesCount;
				res->tables = (gjHighscoreTable**) malloc(sizeof(gjHighscoreTable*) * tablesCount);
				return res;
			}
			gjHighscoreTablesResult* gjHighscoreTablesResult_copy(gjHighscoreTablesResult* result) {
				gjHighscoreTablesResult* copy = gjHighscoreTablesResult_create(result->tablesCount);
				copy->success = result->success;
				memcpy(copy->message, result->message, 255);
				copy->tablesCount = result->tablesCount;
				for(unsigned int i = 0; i < result->tablesCount; ++i) {
					copy->tables[i] = gjHighscoreTable_copy(result->tables[i]);
				}
				return copy;
			}
			void gjHighscoreTablesResult_dispose(gjHighscoreTablesResult* res) {
				free((void*) res->message);
				res->message = NULL;
				for(unsigned int i = 0; i < res->tablesCount; ++i) {
					gjHighscoreTable_dispose(res->tables[i]);
				}
				if (res->tablesCount > 0) { 
					free(res->tables);
					res->tables = NULL;
				}
				free(res);
				res = NULL;
			}



			// ------------------------------------------------------------------
			// Highscore Rank Results
			// ------------------------------------------------------------------
			gjHighscoreRankResult* gjHighscoreRankResult_create() {
				gjHighscoreRankResult* res = (gjHighscoreRankResult*) malloc(sizeof(gjHighscoreRankResult));
				res->success = true; 
				res->message = (char*) malloc(sizeof(unsigned char) * 255);
				res->rank = 0;
				return res;
			}
			gjHighscoreRankResult* gjHighscoreRankResult_copy(gjHighscoreRankResult* result) {
				gjHighscoreRankResult* copy = gjHighscoreRankResult_create();
				copy->success = result->success;
				memcpy(copy->message, result->message, 255);
				copy->rank = result->rank;
				return copy;
			}
			void gjHighscoreRankResult_dispose(gjHighscoreRankResult* res) {
				free((void*) res->message);
				res->message = NULL;
				free(res);
				res = NULL;
			}


			// ------------------------------------------------------------------
			// Achievements Results
			// ------------------------------------------------------------------
			gjAchievementsResult* gjAchievementsResult_create(unsigned int achievementsCount) {
				gjAchievementsResult* res = (gjAchievementsResult*) malloc(sizeof(gjAchievementsResult));
				res->success = true;
				res->message = (char*) malloc(sizeof(unsigned char) * 255);
				res->achievementsCount = achievementsCount;
				res->achievements = (gjAchievement**) malloc(sizeof(gjAchievement*) * achievementsCount);
				return res;
			}
			gjAchievementsResult* gjAchievementsResult_copy(gjAchievementsResult* res) {
				gjAchievementsResult* value = gjAchievementsResult_create(res->achievementsCount);
				value->success = res->success;
				memcpy(value->message, res->message, 255);
				for(unsigned int i = 0; i < res->achievementsCount; ++i) {
					value->achievements[i] = gjAchievement_copy(res->achievements[i]);
				}
				return value;
			}
			void gjAchievementsResult_dispose(gjAchievementsResult* res) {
				free((void*) res->message);
				res->message = NULL;
				for(unsigned int i = 0; i < res->achievementsCount; ++i) {
					gjAchievement_dispose(res->achievements[i]);
				}
				free(res->achievements);
				res->achievements = NULL;
				free(res);
				res = NULL;
			}

			// ------------------------------------------------------------------
			// Users/auth Results
			// ------------------------------------------------------------------
			gjUsersAuthResult* gjUsersAuthResult_create() {
				gjUsersAuthResult* res = (gjUsersAuthResult*) malloc(sizeof(gjUsersAuthResult));
				res->success = true;
				res->message = (char*) malloc(sizeof(unsigned char) * 255);
				res->username = (char*) malloc(sizeof(unsigned char) * 255);
				res->usertoken = (char*) malloc(sizeof(unsigned char) * 255);
				return res;
			}
			void gjUsersAuthResult_dispose(gjUsersAuthResult* res) {
				free((void*) res->message);
				res->message = NULL;
				free((void*) res->username);
				res->username = NULL;
				free((void*) res->usertoken);
				res->usertoken = NULL;
				free(res);
				res = NULL;
			}

			gjUsersAuthRequestAttachment* gjUsersAuthRequestAttachment_create() {
				gjUsersAuthRequestAttachment* res = (gjUsersAuthRequestAttachment*) malloc(sizeof(gjUsersAuthRequestAttachment));
				res->username = (char*) malloc(sizeof(unsigned char) * 255);
				res->usertoken = (char*) malloc(sizeof(unsigned char) * 255);
				return res;
			}
			void gjUsersAuthRequestAttachment_dispose(gjUsersAuthRequestAttachment* res) {
				free((void*) res->username);
				res->username = NULL;
				free((void*) res->usertoken);
				res->usertoken = NULL;
				free(res);
				res = NULL;
			}

			// ------------------------------------------------------------------
			// Callbacks
			// ------------------------------------------------------------------
			gjCallback* gjCallback_create(gjCallbackType type, void* data) { 
				gjCallback* cb = (gjCallback*) malloc(sizeof(gjCallback));
				cb->id = API::s_callbackId++;
				cb->type = type;
				cb->data = data;
				return cb;
			}
			void gjCallback_dispose(gjCallback* cb) {
				switch(cb->type) {
					case GJ_GENERAL_RESULT:
						gjGeneralResult_dispose((gjGeneralResult*) cb->data);
						break;
					case GJ_HIGHSCORES_RESULT:
						gjHighscoresResult_dispose((gjHighscoresResult*) cb->data);
						break;
					case GJ_HIGHSCORE_SUBMIT_RESULT:
						gjHighscoreSubmitResult_dispose((gjHighscoreSubmitResult*) cb->data);
						break;
					case GJ_HIGHSCORE_RANK_RESULT:
						gjHighscoreRankResult_dispose((gjHighscoreRankResult*) cb->data);
						break;
					case GJ_HIGHSCORE_TABLES_RESULT:
						gjHighscoreTablesResult_dispose((gjHighscoreTablesResult*) cb->data);
						break;
					case GJ_ACHIEVEMENTS_RESULT:
						gjAchievementsResult_dispose((gjAchievementsResult*) cb->data);
						break;
					case GJ_ACHIEVEMENT_SUBMIT_RESULT:
						gjAchievementSubmitResult_dispose((gjAchievementSubmitResult*) cb->data);
						break;
					case GJ_USERS_AUTH_RESULT:
						gjUsersAuthResult_dispose((gjUsersAuthResult*) cb->data);
						break;
					case GJ_SESSION_OPEN_RESULT:
						gjSessionOpenResult_dispose((gjSessionOpenResult*) cb->data);
						break;
					case GJ_SESSION_PING_RESULT:
						gjSessionPingResult_dispose((gjSessionPingResult*) cb->data);
						break;
					case GJ_SESSION_CLOSE_RESULT:
						gjSessionCloseResult_dispose((gjSessionCloseResult*) cb->data);
						break;
				}
				free(cb);
				cb = NULL;
			}
				

			// ------------------------------------------------------------------
			// Internal requests
			// ------------------------------------------------------------------
			gjUrlRequest* gjUrlRequest_create() {
				gjUrlRequest* req = (gjUrlRequest*) malloc(sizeof(gjUrlRequest));
				req->id = API::s_requestId++;
				req->request = new URLRequest();
				return req; 
			}
			void gjUrlRequest_setUrl(gjUrlRequest* req, const char* url) {
				req->request->setUrl(url);
			}
			void gjUrlRequest_start(gjUrlRequest* req, void* callback, void* callbackObj) {
				req->request->start(callback, callbackObj, (void*) req);
			}
			void gjUrlRequest_dispose(gjUrlRequest* req) {
				delete req->request;
				req->request = NULL;
				free(req);
				req = NULL;
			}


			

			unsigned int API::s_requestId = 0;
			unsigned int API::s_callbackId = 0;

			API::API(int gameId, string gameKey):
				m_gameId(gameId),	
				m_gameKey(gameKey),
				m_userName(""),
				m_userToken(""),
				m_requests(),
				m_callbacks(),
				m_internalListener(NULL),
				m_overlayListener(NULL),
				m_listener(NULL),
				m_format(GJ_FORMAT_JSON)
			{
				m_internalListener = __internalGameJoltListener;
				m_callbackAddMutex = new Mutex();

				#ifdef ARK2D_WINDOWS
					m_format = GJ_FORMAT_XML;
				#endif
			}
			void API::setListener(gjListener listener) {
				m_listener = listener;
			}
			void API::setFormat(gjFormatType format) {
				m_format = format;
			}
			void API::logout() {
				m_userName = "";
				m_userToken = "";
			}

			void API::authUser(string name, string token) {
				map<string, string> params;
				params["username"] = name;
				params["user_token"] = token;
				string requestUrl = url("users/auth/", params, true, true);

				params["signature"] = md5(requestUrl);
				requestUrl = url("users/auth/", params, true, false);

				ARK2D::getLog()->v("-- request url -- ");
				ARK2D::getLog()->v(requestUrl);

				gjUrlRequest* req = gjUrlRequest_create();
				gjUrlRequest_setUrl(req, requestUrl.c_str());
				gjUrlRequest_start(req, (void*) &authUserInternal, this);

				gjUsersAuthRequestAttachment* attachment = gjUsersAuthRequestAttachment_create();
				strncpy(attachment->username, name.c_str(), 255);
				strncpy(attachment->usertoken, token.c_str(), 255);
				req->attachment = attachment;
				m_requests.push_back(req);
			}
			void API::authUserInternal(API* gamejolt, string result, gjUrlRequest* req) {
				// Check for errors
				if (req->request->hasError()) 
				{
					gjUsersAuthResult* res = gjUsersAuthResult_create();
					res->success = false;
					strncpy(res->message, req->request->getError().c_str(), 255);

					gjUsersAuthRequestAttachment_dispose((gjUsersAuthRequestAttachment*) req->attachment);
					gamejolt->removeRequest(req);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_USERS_AUTH_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}
				
				if (result.substr(0, 1) != "{") 
				{
					gjUsersAuthResult* res = gjUsersAuthResult_create();
					res->success = false;
					strncpy(res->message, "API did not return JSON.", 255);

					gjUsersAuthRequestAttachment_dispose((gjUsersAuthRequestAttachment*) req->attachment);
					gamejolt->removeRequest(req);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_USERS_AUTH_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();
					return;	
				}

				JSONNode* root = libJSON::Parse(result);
				JSONNode* response = root->GetNode("response");
				if (response->GetNode("success")->NodeAsString() == "false") 
				{
					gjUsersAuthResult* res = gjUsersAuthResult_create();
					res->success = false;
					strncpy(res->message, response->GetNode("message")->NodeAsString().c_str(), 255);

					gjUsersAuthRequestAttachment_dispose((gjUsersAuthRequestAttachment*) req->attachment);
					gamejolt->removeRequest(req);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_USERS_AUTH_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}

				// We are go! 
				gjUsersAuthRequestAttachment* attachment = (gjUsersAuthRequestAttachment*) req->attachment;
				gjUsersAuthResult* res = gjUsersAuthResult_create();
				res->success = true;
				strncpy(res->username, attachment->username, 255);
				strncpy(res->usertoken, attachment->usertoken, 255);

				gjUsersAuthRequestAttachment_dispose(attachment);
				gamejolt->removeRequest(req);

				// Add a callback to execute on the main thread. 
				gamejolt->m_callbackAddMutex->lock();
				gjCallback* cb = gjCallback_create(GJ_USERS_AUTH_RESULT, res);
				gamejolt->m_callbacks.push_back(cb);
				gamejolt->m_callbackAddMutex->unlock();
			}
			
			void API::submitHighscore(unsigned int tableid, unsigned int score, string unit, string extradata) {
				map<string, string> params;
				params["table_id"] = Cast::toString<unsigned int>(tableid);
				params["username"] = m_userName;
				params["user_token"] = m_userToken;
				params["sort"] = Cast::toString<unsigned int>(score);
				params["score"] = unit;
				params["extra_data"] = extradata;
				string requestUrl = url("scores/add/", params, true, true);

				params["signature"] = md5(requestUrl);
				requestUrl = url("scores/add/", params, true, false);

				ARK2D::getLog()->v("-- request url -- ");
				ARK2D::getLog()->v(requestUrl);

				gjUrlRequest* req = gjUrlRequest_create();
				gjUrlRequest_setUrl(req, requestUrl.c_str());
				gjUrlRequest_start(req, (void*) &submitHighscoreInternal, this);
				m_requests.push_back(req);
			}
			void API::submitHighscore(unsigned int tableid, string guestname, unsigned int score, string unit, string extradata) {
				map<string, string> params;
				params["table_id"] = Cast::toString<unsigned int>(tableid);
				params["guest"] = guestname;
				params["sort"] = Cast::toString<unsigned int>(score);
				params["score"] = unit;
				params["extra_data"] = extradata;
				string requestUrl = url("scores/add/", params, true, true);

				params["signature"] = md5(requestUrl);
				requestUrl = url("scores/add/", params, true, false);

				ARK2D::getLog()->v("-- request url -- ");
				ARK2D::getLog()->v(requestUrl);

				gjUrlRequest* req = gjUrlRequest_create();
				gjUrlRequest_setUrl(req, requestUrl.c_str());
				gjUrlRequest_start(req, (void*) &submitHighscoreInternal, this);
				m_requests.push_back(req);
			}

			void API::submitHighscoreInternal(API* gamejolt, string result, gjUrlRequest* req) {

				// Check for errors
				if (req->request->hasError()) 
				{
					gjHighscoreSubmitResult* res = gjHighscoreSubmitResult_create();
					res->success = false;
					strncpy(res->message, req->request->getError().c_str(), 255);

					gamejolt->removeRequest(req);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_SUBMIT_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}
				
				gamejolt->removeRequest(req);
				
				if (gamejolt->m_format == GJ_FORMAT_JSON) 
				{
					if (result.substr(0, 1) != "{") 
					{
						gjHighscoreSubmitResult* res = gjHighscoreSubmitResult_create();
						res->success = false;
						strncpy(res->message, "API did not return JSON.", 255);

						gamejolt->m_callbackAddMutex->lock();
						gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_SUBMIT_RESULT, res);
						gamejolt->m_callbacks.push_back(cb);
						gamejolt->m_callbackAddMutex->unlock();
						return;	
					}

					// {"response":{"success":"true","scores":[{"score":"1416","sort":"1416","extra_data":"","user":"","user_id":"","guest":"Ashley","stored":"6 months ago"},{"score":"537","sort":"537","extra_data":"","user":"","user_id":"","guest":"Ted","stored":"1 year ago"},
					JSONNode* root = libJSON::Parse(result);
					JSONNode* response = root->GetNode("response");
					if (response->GetNode("success")->NodeAsString() == "false") 
					{
						gjHighscoreSubmitResult* res = gjHighscoreSubmitResult_create();
						res->success = false;
						strncpy(res->message, response->GetNode("message")->NodeAsString().c_str(), 255);

						gamejolt->m_callbackAddMutex->lock();
						gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_SUBMIT_RESULT, res);
						gamejolt->m_callbacks.push_back(cb);
						gamejolt->m_callbackAddMutex->unlock();

						return;
					}
				} 
				else if (gamejolt->m_format == GJ_FORMAT_XML) 
				{
					if (result.substr(0, 1) != "<") 
					{
						gjHighscoreSubmitResult* res = gjHighscoreSubmitResult_create();
						res->success = false;
						strncpy(res->message, "API did not return XML.", 255);

						gamejolt->m_callbackAddMutex->lock();
						gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_SUBMIT_RESULT, res);
						gamejolt->m_callbacks.push_back(cb);
						gamejolt->m_callbackAddMutex->unlock();
						return;	
					}

					vector<char> xml_copy = vector<char>(result.begin(), result.end());
					xml_copy.push_back('\0');
					
					xml_document<> xmldocument;
					xmldocument.parse<0>((char*) &xml_copy[0]);

					xml_node<>* root = xmldocument.first_node("response");
					string successStr = rapidXmlUtil_value(root->first_node("success")); 

					bool success = Cast::boolFromString( successStr );
					if (!success) {
						gjHighscoreSubmitResult* res = gjHighscoreSubmitResult_create();
						res->success = false; 
						strncpy(res->message, rapidXmlUtil_value(root->first_node("message")).c_str(), 255);
						
						gamejolt->m_callbackAddMutex->lock();
						gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_SUBMIT_RESULT, res);
						gamejolt->m_callbacks.push_back(cb);
						gamejolt->m_callbackAddMutex->unlock();

						return;
					}

				}

				// We are go! 
				gjHighscoreSubmitResult* res = gjHighscoreSubmitResult_create();
				res->success = true;

				// Add a callback to execute on the main thread. 
				gamejolt->m_callbackAddMutex->lock();
				gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_SUBMIT_RESULT, res);
				gamejolt->m_callbacks.push_back(cb);
				gamejolt->m_callbackAddMutex->unlock();
			}

			void API::getHighscores(unsigned int tableid, unsigned int startOffset, unsigned int endOffset)
			{
				map<string, string> params;
				params["table_id"] = Cast::toString<unsigned int>(tableid);
				params["offset"] = Cast::toString<unsigned int>(startOffset);
				params["limit"] = Cast::toString<unsigned int>(endOffset - startOffset);
				//params["user_token"] = m_userToken;
				string requestUrl = url("scores/", params, false, true);

				params["signature"] = md5(requestUrl);
				requestUrl = url("scores/", params, false, false);

				ARK2D::getLog()->v("-- request url -- ");
				ARK2D::getLog()->v(requestUrl);

				gjUrlRequest* req = gjUrlRequest_create();
				gjUrlRequest_setUrl(req, requestUrl.c_str());
				gjUrlRequest_start(req, (void*) &getHighscoresInternal, this);
				m_requests.push_back(req);
			}
			void API::getHighscoresInternal(API* gamejolt, string result, gjUrlRequest* req) { // static.
				
				ARK2D::getLog()->v("getHighscoresInternal");
				// Check for errors
				if (req->request->hasError()) 
				{
					gjHighscoresResult* res = gjHighscoresResult_create(0);
					res->success = false;
					strncpy(res->message, req->request->getError().c_str(), 255);

					gamejolt->removeRequest(req);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_HIGHSCORES_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}
				
				gamejolt->removeRequest(req);
				
				gjHighscoresResult* res = NULL;
				if (gamejolt->m_format == GJ_FORMAT_JSON)
				{
					// Make sure we have JSON response.
					if (result.substr(0, 1) != "{") 
					{
						gjHighscoresResult* res = gjHighscoresResult_create(0);
						res->success = false;
						strncpy(res->message, "API did not return JSON.", 255);

						gamejolt->m_callbackAddMutex->lock();
						gjCallback* cb = gjCallback_create(GJ_HIGHSCORES_RESULT, res);
						gamejolt->m_callbacks.push_back(cb);
						gamejolt->m_callbackAddMutex->unlock();
						return;	
					} 

					// {"response":{"success":"true","scores":[{"score":"1416","sort":"1416","extra_data":"","user":"","user_id":"","guest":"Ashley","stored":"6 months ago"},{"score":"537","sort":"537","extra_data":"","user":"","user_id":"","guest":"Ted","stored":"1 year ago"},
					ARK2D::getLog()->v("parse json");
					JSONNode* root = libJSON::Parse(result);
					if (root == NULL) {
						gjHighscoresResult* res = gjHighscoresResult_create(0);
						res->success = false;
						strncpy(res->message, "API could not parse JSON.", 255);
					}

					JSONNode* response = root->GetNode("response");
					ARK2D::getLog()->v("did");
					if (response->GetNode("success")->NodeAsString() == "false") 
					{
						gjHighscoresResult* res = gjHighscoresResult_create(0);
						res->success = false;
						strncpy(res->message, response->GetNode("message")->NodeAsString().c_str(), 255);

						gamejolt->m_callbackAddMutex->lock();
						gjCallback* cb = gjCallback_create(GJ_HIGHSCORES_RESULT, res);
						gamejolt->m_callbacks.push_back(cb);
						gamejolt->m_callbackAddMutex->unlock();

						return;
					}

					// We are go! 
					JSONNode* scores = response->GetNode("scores");
					res = gjHighscoresResult_create(scores->NodeSize());
					res->success = true;

					for(unsigned int i = 0; i < scores->NodeSize(); i++) {
						JSONNode* score = scores->Children[i];
						
						res->scores[i] = gjHighscore_create();
						res->scores[i]->userid = Cast::fromString<unsigned int>(score->GetNode("user_id")->NodeAsString());
						memcpy(res->scores[i]->username, score->GetNode("user")->NodeAsString().c_str(), 255);
						memcpy(res->scores[i]->guestname, score->GetNode("guest")->NodeAsString().c_str(), 255);
						res->scores[i]->guest = (score->GetNode("guest")->NodeAsString().length() > 0);
						res->scores[i]->globalRank = 1;
						res->scores[i]->score = Cast::fromString<unsigned int>(score->GetNode("score")->NodeAsString());
					}
				} 
				else if (gamejolt->m_format == GJ_FORMAT_XML) 
				{
					// Make sure we have XML
					if (result.substr(0, 1) != "<") 
					{
						gjHighscoresResult* res = gjHighscoresResult_create(0);
						res->success = false;
						strncpy(res->message, "API did not return XML.", 255);

						gamejolt->m_callbackAddMutex->lock();
						gjCallback* cb = gjCallback_create(GJ_HIGHSCORES_RESULT, res);
						gamejolt->m_callbacks.push_back(cb);
						gamejolt->m_callbackAddMutex->unlock();
						return;	
					} 
					
					// <?xml version="1.0" encoding="UTF-8"?><response><success><![CDATA[true]]></success><scores><score><score>8</score><sort>8</sort><extra_data></extra_data><user></user><user_id></user_id><guest><![CDATA[Player]]></guest><stored><![CDATA[6 minutes ago]]></stored></score></scores></response>
					vector<char> xml_copy = vector<char>(result.begin(), result.end());
					xml_copy.push_back('\0');
					
					xml_document<> xmldocument;
					xmldocument.parse<0>((char*) &xml_copy[0]);

					xml_node<>* root = xmldocument.first_node("response");
					string successStr = rapidXmlUtil_value(root->first_node("success")); 

					bool success = Cast::boolFromString( successStr );
					if (!success) {
						gjHighscoresResult* res = gjHighscoresResult_create(0);
						res->success = false; 

						if (root->first_node("message") == NULL) { 
							strncpy(res->message, "Success was false but error message was empty.", 255);
						} else {
							strncpy(res->message, rapidXmlUtil_value(root->first_node("message")).c_str(), 255);
							ARK2D::getLog()->i("done strncpy");
						}

						gamejolt->m_callbackAddMutex->lock();
						gjCallback* cb = gjCallback_create(GJ_HIGHSCORES_RESULT, res);
						gamejolt->m_callbacks.push_back(cb);
						gamejolt->m_callbackAddMutex->unlock();

						return;
					}

					
					// We are go!
					unsigned int numScores = rapidXmlUtil_countChildren(root->first_node("scores"), "score");
					res = gjHighscoresResult_create(numScores);
					res->success = true;
					
					unsigned int i = 0;
					xml_node<>* scores = root->first_node("scores");
					xml_node<>* score = 0;
					for (score = scores->first_node("score");
						score != NULL;
						score = score->next_sibling("score")) 
					{
						res->scores[i] = gjHighscore_create();
						res->scores[i]->userid = Cast::fromString<unsigned int>( rapidXmlUtil_value(score->first_node("user_id")) );
						strncpy(res->scores[i]->username, rapidXmlUtil_value(score->first_node("user")).c_str(), 255);
						strncpy(res->scores[i]->guestname, rapidXmlUtil_value(score->first_node("guest")).c_str(), 255);
						res->scores[i]->guest = rapidXmlUtil_value(score->first_node("guest")).length() > 0;
						res->scores[i]->globalRank = 1;
						res->scores[i]->score = Cast::fromString<unsigned int>( rapidXmlUtil_value(score->first_node("score")) );
						i++;
					}
					
				}
 
				// Add a callback to execute on the main thread. 
				gamejolt->m_callbackAddMutex->lock();
				gjCallback* cb = gjCallback_create(GJ_HIGHSCORES_RESULT, res);
				gamejolt->m_callbacks.push_back(cb);
				gamejolt->m_callbackAddMutex->unlock();
			}

			void API::getRank(unsigned int tableId, signed int score) {
				map<string, string> params;
				params["table_id"] = Cast::toString<unsigned int>(tableId);
				params["sort"] = Cast::toString<signed int>(score);
				
				string requestUrl = url("scores/get-rank/", params, false, true);

				params["signature"] = md5(requestUrl);
				requestUrl = url("scores/get-rank/", params, false, false);

				ARK2D::getLog()->v("-- request url -- ");
				ARK2D::getLog()->v(requestUrl); 

				gjUrlRequest* req = gjUrlRequest_create();
				gjUrlRequest_setUrl(req, requestUrl.c_str());
				gjUrlRequest_start(req, (void*) &getRankInternal, this);
				m_requests.push_back(req);
			}
			void API::getRankInternal(API* gamejolt, string result, gjUrlRequest* req) {

				// Check for errors
				if (req->request->hasError()) 
				{
					gjHighscoreRankResult* res = gjHighscoreRankResult_create();
					res->success = false;
					strncpy(res->message, req->request->getError().c_str(), 255);

					gamejolt->removeRequest(req);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_RANK_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}
				
				gamejolt->removeRequest(req);

				gjHighscoreRankResult* res = NULL;
				if (gamejolt->m_format == GJ_FORMAT_JSON) 
				{
					
					if (result.substr(0, 1) != "{") 
					{
						res = gjHighscoreRankResult_create();
						res->success = false;
						strncpy(res->message, "API did not return JSON.", 255);

						gamejolt->m_callbackAddMutex->lock();
						gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_RANK_RESULT, res);
						gamejolt->m_callbacks.push_back(cb);
						gamejolt->m_callbackAddMutex->unlock();
						return;	
					}

					// {"response":{"success":"true","scores":[{"score":"1416","sort":"1416","extra_data":"","user":"","user_id":"","guest":"Ashley","stored":"6 months ago"},{"score":"537","sort":"537","extra_data":"","user":"","user_id":"","guest":"Ted","stored":"1 year ago"},
					JSONNode* root = libJSON::Parse(result);
					JSONNode* response = root->GetNode("response");
					if (response->GetNode("success")->NodeAsString() == "false") 
					{
						res = gjHighscoreRankResult_create();
						res->success = false;
						strncpy(res->message, response->GetNode("message")->NodeAsString().c_str(), 255);

						gamejolt->m_callbackAddMutex->lock();
						gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_RANK_RESULT, res);
						gamejolt->m_callbacks.push_back(cb);
						gamejolt->m_callbackAddMutex->unlock();

						return;
					}

					// We are go! 
					res = gjHighscoreRankResult_create();
					res->success = true;
					res->rank = response->GetNode("rank")->NodeAsInt();
				} 
				else if (gamejolt->m_format == GJ_FORMAT_XML)
				{
					if (result.substr(0, 1) != "<") 
					{
						res = gjHighscoreRankResult_create();
						res->success = false;
						strncpy(res->message, "API did not return XML.", 255);

						gamejolt->m_callbackAddMutex->lock();
						gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_RANK_RESULT, res);
						gamejolt->m_callbacks.push_back(cb);
						gamejolt->m_callbackAddMutex->unlock();
						return;	
					}

					vector<char> xml_copy = vector<char>(result.begin(), result.end());
					xml_copy.push_back('\0');
					
					xml_document<> xmldocument;
					xmldocument.parse<0>((char*) &xml_copy[0]);

					xml_node<>* root = xmldocument.first_node("response");
					string successStr = rapidXmlUtil_value(root->first_node("success")); 

					bool success = Cast::boolFromString( successStr );
					if (!success) {
						gjHighscoreRankResult* res = gjHighscoreRankResult_create();
						res->success = false; 

						if (root->first_node("message") == NULL) { 
							strncpy(res->message, "Success was false but error message was empty.", 255);
						} else {
							strncpy(res->message, rapidXmlUtil_value(root->first_node("message")).c_str(), 255);
							ARK2D::getLog()->i("done strncpy");
						}

						gamejolt->m_callbackAddMutex->lock();
						gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_RANK_RESULT, res);
						gamejolt->m_callbacks.push_back(cb);
						gamejolt->m_callbackAddMutex->unlock();

						return;
					}

					
					// We are go!
					res = gjHighscoreRankResult_create();
					res->success = true;
					res->rank = Cast::fromString<unsigned int>( rapidXmlUtil_value(root->first_node("rank")) );
	
				}

				// Add a callback to execute on the main thread. 
				gamejolt->m_callbackAddMutex->lock();
				gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_RANK_RESULT, res);
				gamejolt->m_callbacks.push_back(cb);
				gamejolt->m_callbackAddMutex->unlock();
			}

			void API::getHighscoreTables() {
				map<string, string> params;
				string requestUrl = url("scores/tables/", params, false, true);

				params["signature"] = md5(requestUrl);
				requestUrl = url("scores/tables/", params, false, false);

				ARK2D::getLog()->v("-- request url -- ");
				ARK2D::getLog()->v(requestUrl);

				gjUrlRequest* req = gjUrlRequest_create();
				gjUrlRequest_setUrl(req, requestUrl.c_str());
				gjUrlRequest_start(req, (void*) &getHighscoreTablesInternal, this);
				m_requests.push_back(req);
			}
			void API::getHighscoreTablesInternal(API* api, string result, gjUrlRequest* req) {
				// Check for errors
				if (req->request->hasError()) 
				{
					gjHighscoreTablesResult* res = gjHighscoreTablesResult_create(0);
					res->success = false;
					strncpy(res->message, req->request->getError().c_str(), 255);

					api->removeRequest(req);

					api->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_TABLES_RESULT, res);
					api->m_callbacks.push_back(cb);
					api->m_callbackAddMutex->unlock();

					return;
				}
				
				api->removeRequest(req);
				
				if (result.substr(0, 1) != "{") 
				{
					gjHighscoreTablesResult* res = gjHighscoreTablesResult_create(0);
					res->success = false;
					strncpy(res->message, "API did not return JSON.", 255);

					api->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_TABLES_RESULT, res);
					api->m_callbacks.push_back(cb);
					api->m_callbackAddMutex->unlock();
					return;	
				}

				JSONNode* root = libJSON::Parse(result);
				JSONNode* response = root->GetNode("response");
				if (response->GetNode("success")->NodeAsString() == "false") 
				{
					gjHighscoreTablesResult* res = gjHighscoreTablesResult_create(0);
					res->success = false;
					strncpy(res->message, response->GetNode("message")->NodeAsString().c_str(), 255);

					api->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_TABLES_RESULT, res);
					api->m_callbacks.push_back(cb);
					api->m_callbackAddMutex->unlock();

					return;
				}

				// We are go! 
				JSONNode* tables = response->GetNode("tables");
				gjHighscoreTablesResult* res = gjHighscoreTablesResult_create(tables->NodeSize());
				res->success = true;

				for(unsigned int i = 0; i < tables->NodeSize(); i++) {
					JSONNode* table = tables->Children[i];
					
					res->tables[i] = gjHighscoreTable_create();
					res->tables[i]->id = Cast::fromString<unsigned int>(table->GetNode("id")->NodeAsString());
					strncpy(res->tables[i]->name, table->GetNode("name")->NodeAsString().c_str(), 255);
					strncpy(res->tables[i]->description, table->GetNode("description")->NodeAsString().c_str(), 1024);
					res->tables[i]->primary = Cast::boolFromString(table->GetNode("primary")->NodeAsString());
				}

				// Add a callback to execute on the main thread. 
				api->m_callbackAddMutex->lock();
				gjCallback* cb = gjCallback_create(GJ_HIGHSCORE_TABLES_RESULT, res);
				api->m_callbacks.push_back(cb);
				api->m_callbackAddMutex->unlock();
			}

			void API::getFriends() {
				map<string, string> params;
				params["username"] = m_userName;
				params["user_token"] = m_userToken;
				string requestUrl = url("friends/", params, true, true);

				params["signature"] = md5(requestUrl);
				requestUrl = url("friends/", params, true, false);

				ARK2D::getLog()->v("-- request url -- ");
				ARK2D::getLog()->v(requestUrl);

				gjUrlRequest* req = gjUrlRequest_create();
				gjUrlRequest_setUrl(req, requestUrl.c_str());
				gjUrlRequest_start(req, (void*) &getFriendsInternal, this);
				m_requests.push_back(req);
			}
			void API::getFriendsInternal(API* gamejolt, string result, gjUrlRequest* req) {

				ARK2D::getLog()->e(result);

				/*// Check for errors
				if (req->request->hasError()) 
				{
					gjSessionOpenResult* res = gjSessionOpenResult_create();
					res->success = false;
					strncpy(res->message, req->request->getError().c_str(), 255);

					gamejolt->removeRequest(req);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_SESSION_OPEN_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}
				
				gamejolt->removeRequest(req);
				
				if (result.substr(0, 1) != "{") 
				{
					gjSessionOpenResult* res = gjSessionOpenResult_create();
					res->success = false;
					strncpy(res->message, "API did not return JSON.", 255);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_SESSION_OPEN_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();
					return;	
				}

				JSONNode* root = libJSON::Parse(result);
				JSONNode* response = root->GetNode("response");
				if (response->GetNode("success")->NodeAsString() == "false") 
				{
					gjSessionOpenResult* res = gjSessionOpenResult_create();
					res->success = false;
					strncpy(res->message, response->GetNode("message")->NodeAsString().c_str(), 255);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_SESSION_OPEN_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}

				// We are go! 
				gjSessionOpenResult* res = gjSessionOpenResult_create();
				res->success = true;

				// Add a callback to execute on the main thread. 
				gamejolt->m_callbackAddMutex->lock();
				gjCallback* cb = gjCallback_create(GJ_SESSION_OPEN_RESULT, res);
				gamejolt->m_callbacks.push_back(cb);
				gamejolt->m_callbackAddMutex->unlock();*/
			}

			void API::getAchievements() {
				map<string, string> params;
				params["username"] = m_userName;
				params["user_token"] = m_userToken;
				string requestUrl = url("trophies/", params, true, true);

				params["signature"] = md5(requestUrl);
				requestUrl = url("trophies/", params, true, false);

				ARK2D::getLog()->v("-- request url -- ");
				ARK2D::getLog()->v(requestUrl);

				gjUrlRequest* req = gjUrlRequest_create();
				gjUrlRequest_setUrl(req, requestUrl.c_str());
				gjUrlRequest_start(req, (void*) &getAchievementsInternal, this);
				m_requests.push_back(req);
			}
			void API::getAchievements(bool achieved) {
				map<string, string> params;
				params["username"] = m_userName;
				params["user_token"] = m_userToken;
				params["achieved"] = Cast::boolToString(achieved);
				string requestUrl = url("trophies/", params, true, true);

				params["signature"] = md5(requestUrl);
				requestUrl = url("trophies/", params, true, false);

				ARK2D::getLog()->v("-- request url -- ");
				ARK2D::getLog()->v(requestUrl);

				gjUrlRequest* req = gjUrlRequest_create();
				gjUrlRequest_setUrl(req, requestUrl.c_str());
				gjUrlRequest_start(req, (void*) &getAchievementsInternal, this);
				m_requests.push_back(req);
			}
			void API::getAchievementsInternal(API* gamejolt, string result, gjUrlRequest* req) {
				// Check for errors
				if (req->request->hasError()) 
				{
					gjAchievementsResult* res = gjAchievementsResult_create(0);
					res->success = false;
					strncpy(res->message, req->request->getError().c_str(), 255);

					gamejolt->removeRequest(req);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_ACHIEVEMENTS_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}
				
				gamejolt->removeRequest(req);
				ARK2D::getLog()->e(result);
				
				if (result.substr(0, 1) != "{") 
				{
					gjAchievementsResult* res = gjAchievementsResult_create(0);
					res->success = false;
					strncpy(res->message, "API did not return JSON.", 255);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_ACHIEVEMENTS_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();
					return;	
				}

				JSONNode* root = libJSON::Parse(result);
				JSONNode* response = root->GetNode("response");
				if (response->GetNode("success")->NodeAsString() == "false") 
				{
					gjAchievementsResult* res = gjAchievementsResult_create(0);
					res->success = false;
					strncpy(res->message, response->GetNode("message")->NodeAsString().c_str(), 255);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_ACHIEVEMENTS_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}

				// We are go! 
				JSONNode* trophies = response->GetNode("trophies");
				gjAchievementsResult* res = gjAchievementsResult_create(trophies->NodeSize());
				res->success = true;

				for(unsigned int i = 0; i < trophies->NodeSize(); i++) {
					JSONNode* trophy = trophies->Children[i];
					
					
					res->achievements[i] = gjAchievement_create();
					res->achievements[i]->id = Cast::fromString<unsigned int>(trophy->GetNode("id")->NodeAsString());
//					res->achievements[i]->name = trophy->GetNode("title")->NodeAsString().c_str();
					strncpy(res->achievements[i]->name, trophy->GetNode("title")->NodeAsString().c_str(), 255);
					strncpy(res->achievements[i]->description, trophy->GetNode("description")->NodeAsString().c_str(), 1024);

					string difficultyStr = trophy->GetNode("difficulty")->NodeAsString();
					if (difficultyStr == "Platinum") { 
						res->achievements[i]->difficulty = GJ_DIFFICULTY_PLATINUM;
					} else if (difficultyStr == "Gold") { 
						res->achievements[i]->difficulty = GJ_DIFFICULTY_GOLD;
					} else if (difficultyStr == "Silver") { 
						res->achievements[i]->difficulty = GJ_DIFFICULTY_SILVER;
					} else {
						res->achievements[i]->difficulty = GJ_DIFFICULTY_BRONZE;
					}

					strncpy(res->achievements[i]->imageUrl, trophy->GetNode("image_url")->NodeAsString().c_str(), 1024);
					// (trophy->GetNode("image_url")->NodeAsString().length() > 0);
					res->achievements[i]->achieved = true; 
					//res->achievements[i]->achievedOn = "";
				}

				// Add a callback to execute on the main thread. 
				gamejolt->m_callbackAddMutex->lock();
				gjCallback* cb = gjCallback_create(GJ_ACHIEVEMENTS_RESULT, res);
				gamejolt->m_callbacks.push_back(cb);
				gamejolt->m_callbackAddMutex->unlock();

			}

			void API::sessionOpen() {
				map<string, string> params;
				params["username"] = m_userName;
				params["user_token"] = m_userToken;
				string requestUrl = url("sessions/open/", params, true, true);

				params["signature"] = md5(requestUrl);
				requestUrl = url("sessions/open/", params, true, false);

				ARK2D::getLog()->v("-- request url -- ");
				ARK2D::getLog()->v(requestUrl);

				gjUrlRequest* req = gjUrlRequest_create();
				gjUrlRequest_setUrl(req, requestUrl.c_str());
				gjUrlRequest_start(req, (void*) &sessionOpenInternal, this);
				m_requests.push_back(req);
			}
			void API::sessionPing(bool active) {
				map<string, string> params;
				params["username"] = m_userName;
				params["user_token"] = m_userToken;
				params["status"] = (active)?string("active"):string("idle");
				string requestUrl = url("sessions/ping/", params, true, true);

				params["signature"] = md5(requestUrl);
				requestUrl = url("sessions/ping/", params, true, false);

				ARK2D::getLog()->v("-- request url -- ");
				ARK2D::getLog()->v(requestUrl);

				gjUrlRequest* req = gjUrlRequest_create();
				gjUrlRequest_setUrl(req, requestUrl.c_str());
				gjUrlRequest_start(req, (void*) &sessionPingInternal, this);
				m_requests.push_back(req);
			}
			void API::sessionClose() {
				map<string, string> params;
				params["username"] = m_userName;
				params["user_token"] = m_userToken;
				string requestUrl = url("sessions/close/", params, true, true);

				params["signature"] = md5(requestUrl);
				requestUrl = url("sessions/close/", params, true, false);

				ARK2D::getLog()->v("-- request url -- ");
				ARK2D::getLog()->v(requestUrl);

				gjUrlRequest* req = gjUrlRequest_create();
				gjUrlRequest_setUrl(req, requestUrl.c_str());
				gjUrlRequest_start(req, (void*) &sessionCloseInternal, this);
				m_requests.push_back(req);
			}
					
			void API::sessionOpenInternal(API* gamejolt, string result, gjUrlRequest* req) {
				// Check for errors
				if (req->request->hasError()) 
				{
					gjSessionOpenResult* res = gjSessionOpenResult_create();
					res->success = false;
					strncpy(res->message, req->request->getError().c_str(), 255);

					gamejolt->removeRequest(req);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_SESSION_OPEN_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}
				
				gamejolt->removeRequest(req);
				
				if (result.substr(0, 1) != "{") 
				{
					gjSessionOpenResult* res = gjSessionOpenResult_create();
					res->success = false;
					strncpy(res->message, "API did not return JSON.", 255);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_SESSION_OPEN_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();
					return;	
				}

				JSONNode* root = libJSON::Parse(result);
				JSONNode* response = root->GetNode("response");
				if (response->GetNode("success")->NodeAsString() == "false") 
				{
					gjSessionOpenResult* res = gjSessionOpenResult_create();
					res->success = false;
					strncpy(res->message, response->GetNode("message")->NodeAsString().c_str(), 255);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_SESSION_OPEN_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}

				// We are go! 
				gjSessionOpenResult* res = gjSessionOpenResult_create();
				res->success = true;

				// Add a callback to execute on the main thread. 
				gamejolt->m_callbackAddMutex->lock();
				gjCallback* cb = gjCallback_create(GJ_SESSION_OPEN_RESULT, res);
				gamejolt->m_callbacks.push_back(cb);
				gamejolt->m_callbackAddMutex->unlock();
			}
			void API::sessionPingInternal(API* gamejolt, string result, gjUrlRequest* req) {
				// Check for errors
				if (req->request->hasError()) 
				{
					gjSessionPingResult* res = gjSessionPingResult_create();
					res->success = false;
					strncpy(res->message, req->request->getError().c_str(), 255);

					gamejolt->removeRequest(req);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_SESSION_PING_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}
				
				gamejolt->removeRequest(req);
				
				if (result.substr(0, 1) != "{") 
				{
					gjSessionPingResult* res = gjSessionPingResult_create();
					res->success = false;
					strncpy(res->message, "API did not return JSON.", 255);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_SESSION_PING_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();
					return;	
				}

				JSONNode* root = libJSON::Parse(result);
				JSONNode* response = root->GetNode("response");
				if (response->GetNode("success")->NodeAsString() == "false") 
				{
					gjSessionPingResult* res = gjSessionPingResult_create();
					res->success = false;
					strncpy(res->message, response->GetNode("message")->NodeAsString().c_str(), 255);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_SESSION_PING_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}

				// We are go! 
				gjSessionPingResult* res = gjSessionPingResult_create();
				res->success = true;

				// Add a callback to execute on the main thread. 
				gamejolt->m_callbackAddMutex->lock();
				gjCallback* cb = gjCallback_create(GJ_SESSION_PING_RESULT, res);
				gamejolt->m_callbacks.push_back(cb);
				gamejolt->m_callbackAddMutex->unlock();
			}
			void API::sessionCloseInternal(API* gamejolt, string result, gjUrlRequest* req) {
				// Check for errors
				if (req->request->hasError()) 
				{
					gjSessionCloseResult* res = gjSessionCloseResult_create();
					res->success = false;
					strncpy(res->message, req->request->getError().c_str(), 255);

					gamejolt->removeRequest(req);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_SESSION_CLOSE_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}
				
				gamejolt->removeRequest(req);
				
				if (result.substr(0, 1) != "{") 
				{
					gjSessionCloseResult* res = gjSessionCloseResult_create();
					res->success = false;
					strncpy(res->message, "API did not return JSON.", 255);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_SESSION_CLOSE_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();
					return;	
				}

				JSONNode* root = libJSON::Parse(result);
				JSONNode* response = root->GetNode("response");
				if (response->GetNode("success")->NodeAsString() == "false") 
				{
					gjSessionCloseResult* res = gjSessionCloseResult_create();
					res->success = false;
					strncpy(res->message, response->GetNode("message")->NodeAsString().c_str(), 255);

					gamejolt->m_callbackAddMutex->lock();
					gjCallback* cb = gjCallback_create(GJ_SESSION_CLOSE_RESULT, res);
					gamejolt->m_callbacks.push_back(cb);
					gamejolt->m_callbackAddMutex->unlock();

					return;
				}

				// We are go! 
				gjSessionCloseResult* res = gjSessionCloseResult_create();
				res->success = true;

				// Add a callback to execute on the main thread. 
				gamejolt->m_callbackAddMutex->lock();
				gjCallback* cb = gjCallback_create(GJ_SESSION_CLOSE_RESULT, res);
				gamejolt->m_callbacks.push_back(cb);
				gamejolt->m_callbackAddMutex->unlock();
			}

			void API::update() {
				// loop through done events and invoke callbacks.
				m_callbackAddMutex->lock();

				if (m_callbacks.size() > 0) {
					gjCallback* cb = m_callbacks[0];
					
					m_internalListener(this, cb->type, cb->data);
					if (m_overlayListener != NULL) {
						m_overlayListener(this, cb->type, cb->data);
					}
					if (m_listener != NULL) {
						m_listener(this, cb->type, cb->data);
					}
					/*if (cb->data != NULL) { 
						free(cb->data); 
						cb->data = NULL; 
					}*/
					gjCallback_dispose(cb);

					m_callbacks.erase(m_callbacks.begin() + 0);
				}

				m_callbackAddMutex->unlock();

			}

			void API::render(GameContainer* container, Renderer* r) {

			}

			string API::url(string method, map<string, string> params, bool addUserToken, bool addKey) {
				string protocol("http://"); 
				string apiRoot("api.gamejolt.com/api/game/");
				string urlString = protocol + apiRoot + string("v1_1/") + method + "?game_id=" + Cast::toString<unsigned int>(m_gameId);
				
				string user_token("");
				map<string, string>::iterator it;
				if (m_format == GJ_FORMAT_JSON) { 
					params["format"] = "json";
				} else if (m_format == GJ_FORMAT_XML) {
					params["format"] = "xml";
				}
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
					urlString += m_gameKey;
				}
				return urlString;
			}
			string API::md5(string input) { 
				return ARK::GJ::md5(input);
			} 

			void API::removeRequest(gjUrlRequest* req) {
				// // Remove from m_requests because it's done...
				// for(unsigned int i = 0; i < m_requests.size(); i++) {
				// 	gjUrlRequest* thisReq = m_requests[i];
				// 	if (req == thisReq) {
				// 		gjUrlRequest_dispose(thisReq);
				// 		m_requests.erase(m_requests.begin() + i);
				// 		break;
				// 	}
				// }
				removeRequestStatic(&m_requests, req);
			}
			void API::removeRequestStatic(vector<gjUrlRequest* >* requests, gjUrlRequest* req) {
				// Remove from m_requests because it's done...
				for(unsigned int i = 0; i < requests->size(); i++) {
					gjUrlRequest* thisReq = requests->at(i);
					if (req == thisReq) {
						gjUrlRequest_dispose(thisReq);
						requests->erase(requests->begin() + i);
						break;
					}
				}
			}

			API::~API() {
				 
			}
		}
	}
}
