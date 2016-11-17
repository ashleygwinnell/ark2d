/*
 * GooglePlayGameServicesUtil.cpp
 *
 *  Created on: 15 September 2013
 *      Author: Ashley
 */

#include "GooglePlayGameServicesUtil.h"

#include "../Core/ARK2D.h"
//#include "../Includes.h"
#include "../Core/GameContainer.h"

#if defined(ARK2D_ANDROID)
	#include "../Core/Platform/GameContainerAndroid.h"
#endif

#include "../Core/Util/StringUtil.h"
#include "../Core/Util/Callbacks.h"

namespace ARK {
	namespace Util {

		GooglePlayBillingListener* GooglePlayBillingListener::s_instance = new GooglePlayBillingListener();

		void __callback_androidBilling_initialise_success() {
			GooglePlayBillingListener::get()->onInitialiseSuccessful();
		}
		void __callback_androidBilling_initialise_fail() {
			GooglePlayBillingListener::get()->onInitialiseFailed();
		}
		void __callback_androidBilling_purchase_success(string sku) {
			GooglePlayBillingListener::get()->onPurchaseSuccessful(sku);
		}
		void __callback_androidBilling_purchase_fail(string sku) {
			GooglePlayBillingListener::get()->onPurchaseFailed(sku);
		}
		void __callback_androidBilling_queryInventory_success() {
			GooglePlayBillingListener::get()->onQueryInventorySuccessful();
		}
		void __callback_androidBilling_queryInventory_fail() {
			GooglePlayBillingListener::get()->onQueryInventoryFailed();
		}


		GooglePlayBillingListener::GooglePlayBillingListener() {
			Callbacks::createAndAdd(
				ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_BILLING_INITIALISE_SUCCESS,
				(void*) &__callback_androidBilling_initialise_success
			);

			Callbacks::createAndAdd(
				ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_BILLING_INITIALISE_FAIL,
				(void*) &__callback_androidBilling_initialise_fail
			);

			Callbacks::createAndAdd(
				ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_BILLING_PURCHASE_SUCCESS,
				(void*) &__callback_androidBilling_purchase_success
			);

			Callbacks::createAndAdd(
				ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_BILLING_PURCHASE_FAIL,
				(void*) &__callback_androidBilling_purchase_fail
			);

			Callbacks::createAndAdd(
				ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_BILLING_QUERYINVENTORY_SUCCESS,
				(void*) &__callback_androidBilling_queryInventory_success
			);

			Callbacks::createAndAdd(
				ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_BILLING_QUERYINVENTORY_FAIL,
				(void*) &__callback_androidBilling_purchase_fail
			);
		}


		//void GooglePlayBillingUtil::initialise(string commaSeparatedSkus) {
			//vector<string> ids = StringUtil::split(commaSeparatedSkus, ",");
		//}

		bool GooglePlayBillingUtil::isSetup() {
			#if defined(ARK2D_ANDROID)
				ARK2D::getLog()->v("GooglePlayBillingUtil - isSetup");
				return ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaybilling_isSetup();
			#endif
			return false;
		}
		void GooglePlayBillingUtil::startPurchase(string id, int referenceNumber, string extraToken) {
			#if defined(ARK2D_ANDROID)
				ARK2D::getLog()->v(StringUtil::append( "GooglePlayBillingUtil - startPurchase. id: ", id) );
				ARK2D::getLog()->v(StringUtil::append( "referenceNumber: ", referenceNumber) );
				ARK2D::getLog()->v(StringUtil::append( "extraToken: ", extraToken) );
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaybilling_startPurchase(id, referenceNumber, extraToken);
			#endif
		}
		bool GooglePlayBillingUtil::hasPurchase(string sku) {
			#if defined(ARK2D_ANDROID)
				ARK2D::getLog()->v(StringUtil::append( "GooglePlayBillingUtil - hasPurchase. id: ", sku) );
				return ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaybilling_hasPurchase(sku);
			#endif
		}







		GooglePlayGamesListener* GooglePlayGamesListener::s_instance = new GooglePlayGamesListener();

		void __callback_androidServices_signin_success() {
			GooglePlayGamesListener::get()->onSignInSuccessful();
		}
		void __callback_androidServices_signin_fail() {
			GooglePlayGamesListener::get()->onSignInFailed();
		}
		void __callback_androidServices_savedGame_loadStarted() {
			GooglePlayGamesListener::get()->onSavedGameLoadStarted();
		}
		void __callback_androidServices_savedGame_loadSuccessful(string data) {
			GooglePlayGamesListener::get()->onSavedGameLoadSuccessful(data);
		}
		void __callback_androidServices_savedGame_loadFailed(string errmsg) {
			GooglePlayGamesListener::get()->onSavedGameLoadFailed(errmsg);
		}
		void __callback_androidServices_savedGame_updateStarted() {
			GooglePlayGamesListener::get()->onSavedGameUpdateStarted();
		}
		void __callback_androidServices_savedGame_updateSuccessful() {
			GooglePlayGamesListener::get()->onSavedGameUpdateSuccessful();
		}
		void __callback_androidServices_savedGame_updateFailed() {
			GooglePlayGamesListener::get()->onSavedGameUpdateFailed();
		}

		GooglePlayGamesListener::GooglePlayGamesListener() {
			Callbacks::createAndAdd(
				ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_SIGNIN_SUCCESSFUL,
				(void*) &__callback_androidServices_signin_success
			);
			Callbacks::createAndAdd(
				ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_SIGNIN_UNSUCCESSFUL,
				(void*) &__callback_androidServices_signin_fail
			);
			Callbacks::createAndAdd(
				ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_SAVEDGAME_LOAD_START,
				(void*) &__callback_androidServices_savedGame_loadStarted
			);
			Callbacks::createAndAdd(
				ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_SAVEDGAME_LOAD_SUCCESS,
				(void*) &__callback_androidServices_savedGame_loadSuccessful
			);
			Callbacks::createAndAdd(
				ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_SAVEDGAME_LOAD_FAIL,
				(void*) &__callback_androidServices_savedGame_loadFailed
			);
			Callbacks::createAndAdd(
				ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_SAVEDGAME_UPDATE_START,
				(void*) &__callback_androidServices_savedGame_updateStarted
			);
			Callbacks::createAndAdd(
				ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_SAVEDGAME_UPDATE_SUCCESS,
				(void*) &__callback_androidServices_savedGame_updateSuccessful
			);
			Callbacks::createAndAdd(
				ARK::Core::Utils::Callbacks::CALLBACK_ANDROID_SAVEDGAME_UPDATE_FAIL,
				(void*) &__callback_androidServices_savedGame_updateFailed
			);
		}





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
				//return ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_isSigningIn();
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
				ARK2D::getLog()->v("GooglePlayGameServicesUtil - submitScore");
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_submitScore(id, score);
			#endif
		}

		void GooglePlayGameServicesUtil::selectSavedGame(bool allowAddButton, bool allowDeleteButton) {
			#if defined(ARK2D_ANDROID)
				ARK2D::getLog()->v("GooglePlayGameServicesUtil - selectSavedGame");
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_savedGamesSelect(allowAddButton, allowDeleteButton);
			#endif
		}
		void GooglePlayGameServicesUtil::loadSavedGame(string name) {
			#if defined(ARK2D_ANDROID)
				ARK2D::getLog()->v("GooglePlayGameServicesUtil - loadSavedGame");
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_savedGamesLoad(name);
			#endif
		}
		void GooglePlayGameServicesUtil::saveSavedGame(string name, string data, bool createIfMissing) {
			#if defined(ARK2D_ANDROID)
				ARK2D::getLog()->v("GooglePlayGameServicesUtil - saveSavedGame");
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->googleplaygameservices_savedGamesUpdate(name, data, createIfMissing);
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



