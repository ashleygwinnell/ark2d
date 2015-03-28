/*
 * GameCenterUtil.h
 *
 *  Created on: 8 October 2013
 *      Author: Ashley
 */

#ifndef GAMECENTER_UTIL_H_
#define GAMECENTER_UTIL_H_

#include "../Includes.h"

#if defined(ARK2D_IPHONE)
	#include "../Core/Platform/GameContainerIPhone.h"
#else
	class GameCenterManager;
#endif

namespace ARK {
	namespace Util {
		 
		class ARK2D_API GameCenterUtil {
			private:
				static vector<string> s_waitingAchievements;
				

			public:
				static bool isAvailable();
				
				static void signIn();
				//static void signOut();
				static bool isSignedIn();
				//static bool isSigningIn();

				static void viewAchievements();
				static void unlockAchievement(string id);
				static void challengeAchievement(string id);

				static void viewScores();
				static void submitScore(string id, int score);
				static void challengeScore(string id, int score);

				static void addWaitingAchievement(string id);
				static void submitWaitingAchievements();

			private:
				static GameCenterManager* getManager();

		};
		
	}
}

#endif /* GAMECENTER_UTIL_H_ */
