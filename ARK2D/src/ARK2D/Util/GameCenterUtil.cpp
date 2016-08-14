/*
 * GameCenterUtil.cpp
 *
 *  Created on: 8 October 2013
 *      Author: Ashley
 */

#include "GameCenterUtil.h"

#include "../ARK2D.h"
//#include "../Includes.h"
#include "../Core/GameContainer.h"


#if defined(ARK2D_IPHONE)
	#include "../Core/Platform/GameContainerIPhone.h"
	#import <StoreKit/StoreKit.h>
#endif

GameCenterBillingListener* GameCenterBillingListener::s_instance = new GameCenterBillingListener();

#if defined(ARK2D_IPHONE)
 	SKProductsRequest* GameCenterBillingUtil::s_productRequest = NULL;
 	ARK2D_TransactionObserver* GameCenterBillingUtil::s_transactionObserver = NULL;
 	ARK2D_ProductRequestDelegate* GameCenterBillingUtil::s_productRequestDelegate = NULL;
 	vector<string>* GameCenterBillingUtil::s_purchasedIds = NULL;
 	bool GameCenterBillingUtil::s_initialised = false;

	@implementation ARK2D_ProductRequestDelegate

	    // SKProductsRequestDelegate protocol method
	    -(void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
	      	ARK2D::getLog()->v("GameCenterBillingUtil   ARK2D_ProductRequestDelegate::didReceiveResponse ");
	        self.products = response.products;

	        for (NSString *invalidIdentifier in response.invalidProductIdentifiers) {
	            // Handle any invalid product identifiers.
                NSLog(@"Invalid?%@", invalidIdentifier);
	        }
	        GameCenterBillingListener::get()->onGameCenterInitialiseCompleted();
	        GameCenterBillingUtil::s_initialised = true;

	        //[self displayStoreUI]; // Custom method

	    }

	    - (void)request:(SKRequest *)request didFailWithError:(NSError *)error {
	    	ARK2D::getLog()->v("GameCenterBillingUtil   ARK2D_ProductRequestDelegate::didFailWithError ");
	    	GameCenterBillingListener::get()->onGameCenterInitialiseFailed();
	    }
	    - (void)requestDidFinish:(SKRequest *)request {
	    	ARK2D::getLog()->v("GameCenterBillingUtil   ARK2D_ProductRequestDelegate::requestDidFinish ");
	    }

	@end

	@implementation ARK2D_TransactionObserver
	    - (void)addToList:(SKPaymentTransaction*)transaction
	    {
	    	 NSString* productID = transaction.originalTransaction.payment.productIdentifier;
            string thsStrText = [productID cStringUsingEncoding:[NSString defaultCStringEncoding]];
    		GameCenterBillingUtil::s_purchasedIds->push_back( thsStrText );
		}

	    - (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
		{
		    ARK2D::getLog()->v("GameCenterBillingUtil   ARK2D_TransactionObserver::updatedTransactions ");
		    for (SKPaymentTransaction *transaction in transactions) {
		    	switch (transaction.transactionState) {
		            // Call the appropriate custom method for the transaction state.
		            case SKPaymentTransactionStatePurchasing:
		            	ARK2D::getLog()->v("State - purchasing");
		                //[self showTransactionAsInProgress:transaction deferred:NO];
		                break;
		            case SKPaymentTransactionStateDeferred:
		            	ARK2D::getLog()->v("State - deferred");
		                //[self showTransactionAsInProgress:transaction deferred:YES];
		                break;
                    case SKPaymentTransactionStateFailed:{
                    	ARK2D::getLog()->v("State - failed");
		                //[self failedTransaction:transaction];
		            	NSString* productID = transaction.payment.productIdentifier;
            			string thsStrText = [productID cStringUsingEncoding:[NSString defaultCStringEncoding]];
		                GameCenterBillingListener::get()->onGameCenterPurchaseFailed(thsStrText);

                        break;
                    }
                    case SKPaymentTransactionStatePurchased: {
                    	ARK2D::getLog()->v("State - purchased");
		                GameCenterBillingUtil::persistReceipt(transaction);
		                [self addToList:transaction];

		                NSString* productID = transaction.payment.productIdentifier;
            			string thsStrText = [productID cStringUsingEncoding:[NSString defaultCStringEncoding]];
		                GameCenterBillingListener::get()->onGameCenterPurchaseCompleted(thsStrText);

		                GameCenterBillingUtil::finishTransaction(transaction);
						break;
                    }
                    case SKPaymentTransactionStateRestored: {
                    	ARK2D::getLog()->v("State - restored");
		                [self addToList:transaction.originalTransaction];
		                GameCenterBillingUtil::finishTransaction(transaction.originalTransaction);
		                GameCenterBillingUtil::finishTransaction(transaction);
		                break;
                    }
		            default:
		                // For debugging
		                NSLog(@"Unexpected transaction state %@", @(transaction.transactionState));
		                break;
		        }

		    }
		}
		- (void) paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
		{
			NSLog(@"ARK2D_TransactionObserver::paymentQueueRestoreCompletedTransactionsFinished");
			//purchasedItemIDs = [[NSMutableArray alloc] init];

			NSLog(@"received restored transactions: %i", (unsigned int) queue.transactions.count);
			for (SKPaymentTransaction* transaction in queue.transactions)
			{
			    [self addToList:transaction];
			}


		}
	@end

#endif

namespace ARK {
	namespace Util {

		bool GameCenterBillingUtil::canMakePayments() {
			#ifdef ARK2D_IPHONE
            	return ( [SKPaymentQueue canMakePayments] == YES)?true:false;
            #endif
            return false;
		}

		bool GameCenterBillingUtil::isSetup() {
			return s_initialised;
		}

		void GameCenterBillingUtil::initialise(string commaSeparatedIds) {
			vector<string> ids = StringUtil::split(commaSeparatedIds, ",");
			initialise(ids);
		}

		void GameCenterBillingUtil::initialise( vector<string> iapIds ) {
			#ifdef ARK2D_IPHONE
				ARK2D::getLog()->v("GameCenterBillingUtil::initialise()");

				s_purchasedIds = new vector<string>();

				// set up observer queue
                s_transactionObserver = [[ARK2D_TransactionObserver alloc] init];
			    [[SKPaymentQueue defaultQueue] addTransactionObserver:s_transactionObserver];

				//iapIds
				NSMutableArray* productIdentifiers = [[NSMutableArray alloc] init];
				for(unsigned int i = 0; i < iapIds.size(); i++) {
					ARK2D::getLog()->v( iapIds.at(i) );

					NSString* nsstr = [NSString stringWithCString:iapIds.at(i).c_str() encoding:[NSString defaultCStringEncoding]];
					NSLog(@"item %@", nsstr);
					[productIdentifiers addObject:nsstr];
				}

				SKProductsRequest* productsRequest = [[SKProductsRequest alloc]
				initWithProductIdentifiers:[NSSet setWithArray:productIdentifiers]];

				// Keep a strong reference to the request.
			    s_productRequest = productsRequest;

	            s_productRequestDelegate = [[ARK2D_ProductRequestDelegate alloc] init];
	            productsRequest.delegate = s_productRequestDelegate;
				[productsRequest start];

				// fetch things we've already bought
				[[SKPaymentQueue defaultQueue] restoreCompletedTransactions]; // invokes paymentQueueRestoreCompletedTransactionsFinished

			#endif
		}

		void GameCenterBillingUtil::startPurchase(string id) {
            #ifdef ARK2D_IPHONE
            	ARK2D::getLog()->v(StringUtil::append("GameCenterBillingUtil::startPurchase() ", id));
                // Find the Product in the list.
                SKProduct* foundproduct = nil;
                for (SKProduct* product in [s_productRequestDelegate products]) {
                    NSString* idNSString = [product productIdentifier];
                    string thsStrText = [idNSString cStringUsingEncoding:[NSString defaultCStringEncoding]];
                    ARK2D::getLog()->v(thsStrText);
                    if (thsStrText == id){
                        foundproduct = product;
                        break;
                    }
                }
                if (foundproduct == nil) {
                	ARK2D::getLog()->v("not found");
                	GameCenterBillingListener::get()->onGameCenterPurchaseFailed(id);
                	return;
                }

                SKMutablePayment* payment = [SKMutablePayment paymentWithProduct:foundproduct];
                payment.quantity = 1;

                [[SKPaymentQueue defaultQueue] addPayment:payment];
            #endif
		}
		bool GameCenterBillingUtil::hasPurchased(string id) {
			#ifdef ARK2D_IPHONE
				ARK2D::getLog()->v("GameCenterBillingUtil::hasPurchased()");
				NSURL* receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
				NSData* receiptData = [NSData dataWithContentsOfURL:receiptURL];

                if (!receiptData) {
                    return false;
                }

				// Custom method to work with receipts
				//BOOL rocketCarEnabled = [self receipt:receiptData includesProductID:@"com.example.rocketCar"];
                for(unsigned int i = 0; i < s_purchasedIds->size(); i++) {
                	if (s_purchasedIds->at(i) == id) {
                		return true;
                	}
                }
                return false;

			#endif
			return false;
		}

		void GameCenterBillingUtil::persistValue(string key, string val) {
			#ifdef ARK2D_IPHONE
				ARK2D::getLog()->v(StringUtil::append("GameCenterBillingUtil::persistValue() ", key));

				#if USE_ICLOUD_STORAGE
					NSUbiquitousKeyValueStore* storage = [NSUbiquitousKeyValueStore defaultStore];
				#else
					NSUserDefaults* storage = [NSUserDefaults standardUserDefaults];
				#endif

				[storage setBool:YES forKey:@"enable_rocket_car"];
				[storage setObject:@15 forKey:@"highest_unlocked_level"];

				[storage synchronize];
			#endif
		}
		#ifdef ARK2D_IPHONE
			void GameCenterBillingUtil::persistReceipt(SKPaymentTransaction* transaction) {
				ARK2D::getLog()->v("GameCenterBillingUtil::persistReceipt()");
				#if USE_ICLOUD_STORAGE
					NSUbiquitousKeyValueStore* storage = [NSUbiquitousKeyValueStore defaultStore];
				#else
					NSUserDefaults* storage = [NSUserDefaults standardUserDefaults];
				#endif
                //transaction.transacionRe
				NSData* newReceipt = transaction.transactionReceipt;
				NSArray* savedReceipts = [storage arrayForKey:@"receipts"];
				if (!savedReceipts) {
					// Storing the first receipt
					[storage setObject:@[newReceipt] forKey:@"receipts"];
				} else {
					// Adding another receipt
					NSArray *updatedReceipts = [savedReceipts arrayByAddingObject:newReceipt];
					[storage setObject:updatedReceipts forKey:@"receipts"];
				}

				[storage synchronize];
			}
			void GameCenterBillingUtil::finishTransaction(SKPaymentTransaction* transaction) {
				ARK2D::getLog()->v("GameCenterBillingUtil::finishTransaction()");
				[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
			}
		#endif


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



