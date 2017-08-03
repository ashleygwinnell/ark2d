/*
 * GameCenterUtil.h
 *
 *  Created on: 8 October 2013
 *      Author: Ashley
 */

#ifndef GAMECENTER_UTIL_H_
#define GAMECENTER_UTIL_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#if defined(ARK2D_IPHONE)
	#include "../Core/Platform/GameContainerIPhone.h"
 	#import <StoreKit/StoreKit.h>
#elif defined(ARK2D_MACINTOSH)
	#include "../Core/Platform/GameContainerAppleShared.h"
#else
	class GameCenterManager;
#endif

#include <vector>
#include <string>
using std::string;
using std::vector;

#if defined(ARK2D_IPHONE)

@interface ARK2D_ProductRequestDelegate : NSObject <SKProductsRequestDelegate>
@property (retain) NSArray* products;

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response;
- (void)request:(SKRequest *)request didFailWithError:(NSError *)error;
- (void)requestDidFinish:(SKRequest *)request;

@end

@interface ARK2D_TransactionObserver : NSObject<SKPaymentTransactionObserver>
- (void)addToList:(SKPaymentTransaction*)transaction;
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;

@end

#endif

namespace ARK {
	namespace Util {

		class ARK2D_API GameCenterBillingListener {
			public:
				static GameCenterBillingListener* s_instance;

				static GameCenterBillingListener* get() { return s_instance; }
				static void set(GameCenterBillingListener* instance) { s_instance = instance; }

				virtual void onGameCenterInitialiseCompleted() { }
				virtual void onGameCenterInitialiseFailed() { }
				virtual void onGameCenterPurchaseFailed(string productId) { }
				virtual void onGameCenterPurchaseRestored(string productId) { }
				virtual void onGameCenterPurchaseCompleted(string productId) { }


		};
		class ARK2D_API GameCenterBillingUtil {
			public:
				#ifdef ARK2D_IPHONE
					static SKProductsRequest* s_productRequest;
					static ARK2D_TransactionObserver* s_transactionObserver;
					static ARK2D_ProductRequestDelegate* s_productRequestDelegate;
					static vector<string>* s_purchasedIds;
				#endif
				static bool s_initialised;

                static bool canMakePayments();
                static void initialise(string commaSeparatedIds);
                static void initialise(vector<string> ids);
                static bool isSetup();
				static void startPurchase(string id);
				static bool hasPurchased(string id);

				#ifdef ARK2D_IPHONE
					static void persistValue(string key, string val);
					static void persistReceipt(SKPaymentTransaction* transaction);
					static void finishTransaction(SKPaymentTransaction* transaction);
				#endif

		};

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
