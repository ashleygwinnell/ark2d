/*
 * GameCircleUtil.cpp
 *
 *  Created on: 23 December 2014
 *      Author: Ashley
 */

#include "GameCircleUtil.h"

#include "../Core/ARK2D.h"
//#include "../Includes.h"
#include "../Core/GameContainer.h"

namespace ARK {
	namespace Util { 
 
		vector<string> GameCircleUtil::s_waitingAchievements;

		bool GameCircleUtil::isAvailable() {
			#if defined(ARK2D_IPHONE)
			#elif defined(ARK2D_ANDROID)
				return ARK2D::getContainer()->getPlatformSpecific()->getPluggable()->firetv_isAmazonFireTV();
			#endif
			return false;
		}   
		
		/*void GameCircleUtil::signIn() {
			#if defined(ARK2D_IPHONE)
				
			#elif defined(ARK2D_ANDROID)

			#endif
		}  
		bool GameCircleUtil::isSignedIn() {
			#if defined(ARK2D_IPHONE) 

			#elif defined(ARK2D_ANDROID)

			#endif
			return false;
		}*/
	
		void GameCircleUtil::viewAchievements() {
			#if defined(ARK2D_IPHONE) 
			#elif defined(ARK2D_ANDROID)
				if (isAvailable()) {
					AndroidPluggable* plug = ARK2D::getContainer()->getPlatformSpecific()->getPluggable();
					plug->firetv_viewAchievements();
				}
			#endif
		}

		void GameCircleUtil::unlockAchievement(string id) {
			#if defined(ARK2D_IPHONE) 
			#elif defined(ARK2D_ANDROID)
				if (isAvailable()) {
					AndroidPluggable* plug = ARK2D::getContainer()->getPlatformSpecific()->getPluggable();
					plug->firetv_unlockAchievement(id);
				}
			#endif 
		}
	 
	 	void GameCircleUtil::viewScores() {
	 		#if defined(ARK2D_ANDROID)
	 			if (isAvailable()) {
	 				AndroidPluggable* plug = ARK2D::getContainer()->getPlatformSpecific()->getPluggable();
					plug->firetv_viewScores();
				}
	 		#endif
	 	}
	 	void GameCircleUtil::viewScores(string id) {
	 		#if defined(ARK2D_ANDROID)
	 			if (isAvailable()) {
	 				AndroidPluggable* plug = ARK2D::getContainer()->getPlatformSpecific()->getPluggable();
					plug->firetv_viewScores(id);
				}
	 		#endif
	 	}
		void GameCircleUtil::submitScore(string id, int score) {
			#if defined(ARK2D_IPHONE) 
			#elif defined(ARK2D_ANDROID)
				if (isAvailable()) {
					AndroidPluggable* plug = ARK2D::getContainer()->getPlatformSpecific()->getPluggable();
					plug->firetv_submitScore(id, score);
				}
			#endif
		}
		
		void GameCircleUtil::addWaitingAchievement(string id) {
			s_waitingAchievements.push_back(id);
		}
		void GameCircleUtil::submitWaitingAchievements() {
			for(unsigned int i = 0; i < s_waitingAchievements.size(); ++i) {
				string id = s_waitingAchievements.at(i);
				unlockAchievement(id);
			}
			s_waitingAchievements.clear();
		}

	}
}



