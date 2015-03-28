/*
 * GameCircleUtil.h
 *
 *  Created on: 23 December 2014
 *      Author: Ashley
 */

#ifndef ARK_UTIL_GAMECIRCLE_H_
#define ARK_UTIL_GAMECIRCLE_H_

#include "../Includes.h"

namespace ARK {
	namespace Util {
		 
		class ARK2D_API GameCircleUtil {
			private:
				static vector<string> s_waitingAchievements;
				
			public:
				static bool isAvailable();
				
				//static void signIn();
				//static bool isSignedIn();
				
				static void viewAchievements();
				static void unlockAchievement(string id);
				
				static void viewScores();
				static void viewScores(string id);
				static void submitScore(string id, int score);
				
				static void addWaitingAchievement(string id);
				static void submitWaitingAchievements();
		};

	}
}

#endif /* ARK_UTIL_GAMECIRCLE_H_ */
