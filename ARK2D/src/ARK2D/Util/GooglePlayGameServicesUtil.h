
/*
 * GooglePlayGameServicesUtil.h
 *
 *  Created on: 19 May 2013
 *      Author: Ashley
 */

#ifndef GOOGLEPLAYGAMESERVICES_UTIL_H_
#define GOOGLEPLAYGAMESERVICES_UTIL_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

namespace ARK {
	namespace Util {

		class ARK2D_API GooglePlayGamesListener {
			public:
				virtual void onSignInSuccessful() = 0;
				virtual void onSignInUnsuccessful() = 0;
		};
		class ARK2D_API GooglePlayBillingListener {
			public:
				virtual void onPurchaseSuccessful() = 0;
		};
		class ARK2D_API GooglePlayBillingUtil {
			public:
				static bool isSetup();
				static void startPurchase(string id, int referenceNumber, string extraToken);
		};

		class ARK2D_API GooglePlayGameServicesUtil {
			private:
				static vector<string> s_waitingAchievements;

			public:
				static void signIn();
				static void signOut();
				static bool isSignedIn();
				static bool isSigningIn();

				static void viewAchievements();
				static void unlockAchievement(string id);

				static void viewScores(string id);
				static void submitScore(string id, int score);

				//static bool isConnected();
				//static bool isConnecting();

				static void addWaitingAchievement(string id);
				static void submitWaitingAchievements();
		};

	}
}

#endif /* GOOGLEPLAYGAMESERVICES_UTIL_H_ */
