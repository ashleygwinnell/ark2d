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

#include "StringUtil.h"
#include "Callbacks.h"

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
			ARK::Util::Callback callback_initialiseSuccess;
			callback_initialiseSuccess.setId(ARK::Util::Callbacks::CALLBACK_ANDROID_BILLING_INITIALISE_SUCCESS);
			callback_initialiseSuccess.setFunctionPointer((void*) &__callback_androidBilling_initialise_success);
			Callbacks::add(callback_initialiseSuccess);

			ARK::Util::Callback callback_initialiseFail;
			callback_initialiseFail.setId(ARK::Util::Callbacks::CALLBACK_ANDROID_BILLING_INITIALISE_FAIL);
			callback_initialiseFail.setFunctionPointer((void*) &__callback_androidBilling_initialise_fail);
			Callbacks::add(callback_initialiseFail);

			ARK::Util::Callback callback_purchaseSuccess;
			callback_purchaseSuccess.setId(ARK::Util::Callbacks::CALLBACK_ANDROID_BILLING_PURCHASE_SUCCESS);
			callback_purchaseSuccess.setFunctionPointer((void*) &__callback_androidBilling_purchase_success);
			Callbacks::add(callback_purchaseSuccess);

			ARK::Util::Callback callback_purchaseFail;
			callback_purchaseFail.setId(ARK::Util::Callbacks::CALLBACK_ANDROID_BILLING_PURCHASE_FAIL);
			callback_purchaseFail.setFunctionPointer((void*) &__callback_androidBilling_purchase_fail);
			Callbacks::add(callback_purchaseFail);

			ARK::Util::Callback callback_queryInventorySuccess;
			callback_queryInventorySuccess.setId(ARK::Util::Callbacks::CALLBACK_ANDROID_BILLING_QUERYINVENTORY_SUCCESS);
			callback_queryInventorySuccess.setFunctionPointer((void*) &__callback_androidBilling_queryInventory_success);
			Callbacks::add(callback_queryInventorySuccess);

			ARK::Util::Callback callback_queryInventoryFail;
			callback_queryInventoryFail.setId(ARK::Util::Callbacks::CALLBACK_ANDROID_BILLING_QUERYINVENTORY_FAIL);
			callback_queryInventoryFail.setFunctionPointer((void*) &__callback_androidBilling_purchase_fail);
			Callbacks::add(callback_queryInventoryFail);
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



