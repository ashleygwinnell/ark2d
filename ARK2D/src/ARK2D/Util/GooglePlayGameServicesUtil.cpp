/*
 * GooglePlayGameServicesUtil.cpp
 *
 *  Created on: 15 September 2013
 *      Author: Ashley
 */

#include "GooglePlayGameServicesUtil.h"

#include "../ARK2D.h"
//#include "../Includes.h"
#include "../Core/GameContainer.h"

#if defined(ARK2D_ANDROID)
	#include "../Core/Platform/GameContainerAndroid.h"
#endif
 
namespace ARK {
	namespace Util { 

		vector<string> GooglePlayGameServicesUtil::s_waitingAchievements;

		void GooglePlayGameServicesUtil::signIn() {
			#if defined(ARK2D_ANDROID) 
				ARK2D::getLog()->v("GooglePlayGameServices - signIn");
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_signIn();
			#endif
		}  
		void GooglePlayGameServicesUtil::signOut() {
			#if defined(ARK2D_ANDROID) 
				ARK2D::getLog()->v("GooglePlayGameServices - signOut");
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_signOut();
			#endif
		}  

		bool GooglePlayGameServicesUtil::isSignedIn() {
			#if defined(ARK2D_ANDROID) 
				//ARK2D::getLog()->v("GooglePlayGameServices - isSignedIn");
				return ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_isSignedIn();
			#endif
			return false;
		}
		bool GooglePlayGameServicesUtil::isSigningIn() {
			#if defined(ARK2D_ANDROID) 
				//ARK2D::getLog()->v("GooglePlayGameServices - isSigningIn");  
				return ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_isSigningIn();
			#endif
			return false;
		}

		void GooglePlayGameServicesUtil::viewAchievements() {
			#if defined(ARK2D_ANDROID) 
				ARK2D::getLog()->v("GooglePlayGameServices - viewAchievements"); 
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_viewAchievements();
			#endif
		}
		void GooglePlayGameServicesUtil::unlockAchievement(string id) {
			#if defined(ARK2D_ANDROID) 
				ARK2D::getLog()->v(StringUtil::append("GooglePlayGameServices - unlockAchievement - ", id)); 
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_unlockAchievement(id);
			#endif
		}

		void GooglePlayGameServicesUtil::viewScores(string id) {
			#if defined(ARK2D_ANDROID) 
				ARK2D::getLog()->v("GooglePlayGameServices - viewScores"); 
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_viewScores(id);
			#endif
		}
		void GooglePlayGameServicesUtil::submitScore(string id, int score) {
			#if defined(ARK2D_ANDROID) 
				ARK2D::getLog()->v("GooglePlayGameServices - submitScore"); 
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_submitScore(id, score);
			#endif
		}

		/*bool GooglePlayGameServicesUtil::isConnected() {
			#if defined(ARK2D_ANDROID) 
				ARK2D::getLog()->v("GooglePlayGameServices - isConnected"); 
				return ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_isConnected();
			#endif
			return false;
		}
		bool GooglePlayGameServicesUtil::isConnecting() {
			#if defined(ARK2D_ANDROID) 
				ARK2D::getLog()->v("GooglePlayGameServices - isConnecting"); 
				return ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_isConnecting();
			#endif
			return false;
		}*/

		void GooglePlayGameServicesUtil::addWaitingAchievement(string id) {
			#if defined(ARK2D_ANDROID) 
				s_waitingAchievements.push_back(id);
			#endif
		}
		void GooglePlayGameServicesUtil::submitWaitingAchievements() {
			#if defined(ARK2D_ANDROID) 
				for(unsigned int i = 0; i < s_waitingAchievements.size(); ++i) {
					string id = s_waitingAchievements.at(i);
					unlockAchievement(id);
				}
				s_waitingAchievements.clear();
			#endif
		}

	}
}



