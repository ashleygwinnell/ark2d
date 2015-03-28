/*
 * GameCenterUtil.cpp
 *
 *  Created on: 8 October 2013
 *      Author: Ashley
 */

#include "GameCenterUtil.h"

#include "../ARK2D.h"
#include "../Includes.h"
#include "../Core/GameContainer.h"

#if defined(ARK2D_IPHONE)
	#include "../Core/Platform/GameContainerIPhone.h"
#endif

namespace ARK {
	namespace Util { 
 
		vector<string> GameCenterUtil::s_waitingAchievements;

		GameCenterManager* GameCenterUtil::getManager() {
			#if defined(ARK2D_IPHONE)
				GameCenterManager* man = [ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.glViewController getGameCenter];
				
				if (man == NULL) {
					if([GameCenterManager isGameCenterAvailable])
			        {
			            GameContainerIPhoneGLViewController* glview = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.glViewController;
			            glview.gameCenterManager = [[[GameCenterManager alloc] init] autorelease];
			            [glview.gameCenterManager setDelegate: glview];

			            man = glview.gameCenterManager;
			        }
			        else
			        {
						ErrorDialog::createAndShow("Game Center Support Required: The current device does not support Game Center, which this game requires.");
			        }
				}

				return man;
			#endif
			return NULL;

		}
		bool GameCenterUtil::isAvailable() {
			#if defined(ARK2D_IPHONE)
			 	GameCenterManager* manager = getManager();
			 	if (manager == NULL) { ARK2D::getLog()->w("GameCenterManager is null."); return false; }

				return [GameCenterManager isGameCenterAvailable];
			 	
			#endif
			return false;
		}  
		void GameCenterUtil::signIn() {
			#if defined(ARK2D_IPHONE)
				GameCenterManager* manager = getManager();
			 	if (manager == NULL) { ARK2D::getLog()->w("GameCenterManager is null."); return; }

				[manager authenticateLocalUser];

			#endif
		}  
		// void GameCenterUtil::signOut() {
		
		// }  

		bool GameCenterUtil::isSignedIn() {
			#if defined(ARK2D_IPHONE) 
				GameCenterManager* manager = getManager();
			 	if (manager == NULL) { ARK2D::getLog()->w("GameCenterManager is null."); return false; }

			 	return [manager isAuthenticatedLocalUser];

			#endif
			 return false;
		}
		// bool GameCenterUtil::isSigningIn() {
		// 	#if defined(ARK2D_IPHONE) 
		// 		GameCenterManager* manager = getManager();
		// 	 	if (manager == NULL) { ARK2D::getLog()->w("GameCenterManager is null."); return false; }


		// 	#endif
		// 	return false;
		// }

		void GameCenterUtil::viewAchievements() {
			#if defined(ARK2D_IPHONE) 
				ARK2D::getLog()->v("GameCenter - viewAchievements"); 
				GameCenterManager* manager = getManager();
			 	if (manager == NULL) { ARK2D::getLog()->w("GameCenterManager is null."); return; }
			 	[manager openGameCenter];
			#endif
		}

		void GameCenterUtil::unlockAchievement(string id) {
			#if defined(ARK2D_IPHONE) 
				ARK2D::getLog()->v(StringUtil::append("GameCenter - unlockAchievement - ", id)); 
				GameCenterManager* manager = getManager();
			 	if (manager == NULL) { ARK2D::getLog()->w("GameCenterManager is null."); return; }

				//- (void) submitAchievement: (NSString*) identifier percentComplete: (double) percentComplete

				NSString* nsid = [NSString stringWithCString:id.c_str() encoding:[NSString defaultCStringEncoding]];
				double pc = 100.0; 
				[manager submitAchievement:nsid percentComplete:pc]; 
				//[manager openGameCenter];  

			#endif 
		}

		void GameCenterUtil::challengeAchievement(string id) {
			#if defined(ARK2D_IPHONE) 
				ARK2D::getLog()->v(StringUtil::append("GameCenter - challengeAchievement - ", id)); 
				GameCenterManager* manager = getManager();
			 	if (manager == NULL) { ARK2D::getLog()->w("GameCenterManager is null."); return; }

				//NSString* nsid = [NSString stringWithCString:id.c_str() encoding:[NSString defaultCStringEncoding]];
				//double pc = 100.0; 
				//[manager challengeAchievement:nsid percentComplete:pc]; 
			#endif 
		}

	 
	 	void GameCenterUtil::viewScores() {
	 		viewAchievements();
	 	}
		void GameCenterUtil::submitScore(string id, int score) {
			#if defined(ARK2D_IPHONE) 
				ARK2D::getLog()->v(StringUtil::append("GameCenter - submitScore - ", id)); 
				GameCenterManager* manager = getManager();
			 	if (manager == NULL) { ARK2D::getLog()->w("GameCenterManager is null."); return; }

			 	NSString* nsid = [NSString stringWithCString:id.c_str() encoding:[NSString defaultCStringEncoding]];
			 	int64_t ss = (int64_t) score;
			 	[manager reportScore:ss forCategory:nsid]; 

			#endif
		}
		void GameCenterUtil::challengeScore(string id, int score) {
			#if defined(ARK2D_IPHONE) 
				ARK2D::getLog()->v(StringUtil::append("GameCenter - challengeScore - ", id)); 
				GameCenterManager* manager = getManager();
			 	if (manager == NULL) { ARK2D::getLog()->w("GameCenterManager is null."); return; }

			 	//NSString* nsid = [NSString stringWithCString:id.c_str() encoding:[NSString defaultCStringEncoding]];
			 	//int64_t ss = (int64_t) score;
			 	//[manager challengeScore:ss forCategory:nsid]; 

			#endif
		}

		void GameCenterUtil::addWaitingAchievement(string id) {
			s_waitingAchievements.push_back(id);
		}
		void GameCenterUtil::submitWaitingAchievements() {
			for(unsigned int i = 0; i < s_waitingAchievements.size(); ++i) {
				string id = s_waitingAchievements.at(i);
				unlockAchievement(id);
			}
			s_waitingAchievements.clear();
		}

	}
}



