

#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)

	#import "GameContainerAppleShared.h"
    #import "GameContainerAppleCloudDocument.h"
	#import <GameKit/GameKit.h>
	#include "../ARK2D.h"
	#include "../Log.h"
	#include "../Util/Callbacks.h"
	#include "../GameContainer.h"
	#include "../../Util/ICloudUtil.h"


	@implementation GameCenterManager

	@synthesize earnedAchievementCache;
	@synthesize delegate;

	- (id) init
	{
		self = [super init];
		if(self!= NULL)
		{
			earnedAchievementCache= NULL;
		}
		return self;
	}

	- (void) dealloc
	{
		self.earnedAchievementCache= NULL;
		[super dealloc];
	}




	- (void) callDelegate: (SEL) selector withArg: (id) arg error: (NSError*) err
	{
		assert([NSThread isMainThread]);
		if([delegate respondsToSelector: selector])
		{
			if(arg != NULL)
			{
				[delegate performSelector: selector withObject: arg withObject: err];
			}
			else
			{
				[delegate performSelector: selector withObject: err];
			}
		}
		else
		{
			NSLog(@"Missed Method");
		}
	}


	- (void) callDelegateOnMainThread: (SEL) selector withArg: (id) arg error: (NSError*) err
	{
		dispatch_async(dispatch_get_main_queue(), ^(void)
		{
		   [self callDelegate: selector withArg: arg error: err];
		});
	}

	+ (BOOL) isGameCenterAvailable
	{
		#ifdef ARK2D_IPHONE
			// check for presence of GKLocalPlayer API
			Class gcClass = (NSClassFromString(@"GKLocalPlayer"));

			// check if the device is running iOS 4.1 or later
			NSString *reqSysVer = @"4.1";
			NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
			BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);

			return (gcClass && osVersionSupported);
		#endif
		
		#ifdef ARK2D_MACINTOSH
			BOOL isGameCenterAPIAvailable = (NSClassFromString(@"GKLocalPlayer")) != nil;
			return isGameCenterAPIAvailable;
		#endif
		return false;
	}


	- (void) authenticateLocalUser
	{


		if([GKLocalPlayer localPlayer].authenticated == NO)
		{
			GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
			#if defined(ARK2D_IPHONE)
				[localPlayer setAuthenticateHandler:(^(UIViewController *viewController, NSError *error) {

					if (GKLocalPlayer.localPlayer.authenticated || GKLocalPlayer.localPlayer.isAuthenticated) {
						// Already authenticated
						ARK2D::getLog()->e("authenticateHandler: already authed");
						ARK::Core::Utils::Callbacks::invoke(ARK::Core::Utils::Callbacks::CALLBACK_GAMECENTER_SIGNIN_SUCCESSFUL);
					}

					else if (viewController) {
						ARK2D::getLog()->e("authenticateHandler: viewcontroller set.");
						UIViewController* cc = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.glViewController;
						[cc presentViewController:viewController animated:YES completion:nil];
					}

					else {
						// Problem with authentication, probably bc the user doesn't use Game Center.
						ARK2D::getLog()->e("authenticateHandler: probably bc the user doesn't use Game Center.");
						ARK::Core::Utils::Callbacks::invoke(ARK::Core::Utils::Callbacks::CALLBACK_GAMECENTER_SIGNIN_UNSUCCESSFUL);
					} 
				})];
			#elif defined(ARK2D_MACINTOSH)
				[localPlayer setAuthenticateHandler:(^(NSViewController *viewController, NSError *error) {
					
					if (GKLocalPlayer.localPlayer.authenticated || GKLocalPlayer.localPlayer.isAuthenticated) {
						// Already authenticated
						ARK2D::getLog()->e("authenticateHandler: already authed");
						ARK::Core::Utils::Callbacks::invoke(ARK::Core::Utils::Callbacks::CALLBACK_GAMECENTER_SIGNIN_SUCCESSFUL);
					}
					else {
						// Problem with authentication, probably bc the user doesn't use Game Center.
						ARK2D::getLog()->e("authenticateHandler: probably bc the user doesn't use Game Center.");
						ARK::Core::Utils::Callbacks::invoke(ARK::Core::Utils::Callbacks::CALLBACK_GAMECENTER_SIGNIN_UNSUCCESSFUL);
					}
					
				})];
			#endif

			//[[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error)
			//{
			   // ARK::Core::Utils::Callbacks::invoke(ARK::Core::Utils::Callbacks::CALLBACK_GAMECENTER_SIGNIN_SUCCESSFUL);
				//[self callDelegateOnMainThread: @selector(processGameCenterAuth:) withArg: NULL error: error];
			//}];
		}
	}
	- (BOOL) isAuthenticatedLocalUser
	{
		return ([GKLocalPlayer localPlayer].authenticated == YES);
	}

	- (void) reloadHighScoresForCategory: (NSString*) category
	{
		GKLeaderboard* leaderBoard= [[[GKLeaderboard alloc] init] autorelease];
		leaderBoard.category= category;
		leaderBoard.timeScope= GKLeaderboardTimeScopeAllTime;
		leaderBoard.range= NSMakeRange(1, 1);

		[leaderBoard loadScoresWithCompletionHandler:  ^(NSArray *scores, NSError *error)
		{
			[self callDelegateOnMainThread: @selector(reloadScoresComplete:error:) withArg: leaderBoard error: error];
		}];
	}

	- (void) challengeScore: (int64_t) score forCategory: (NSString*) category
	{
		GKScore* scoreReporter = [[[GKScore alloc] initWithCategory:category] autorelease];
		scoreReporter.value = score;
		
		#if defined(ARK2D_IPHONE)
			UIViewController* cc = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.glViewController;
			[cc performSegueWithIdentifier:@"scoreChallenge" sender:scoreReporter];
		#elif defined(ARK2D_MACINTOSH)
			ARK2D::getLog()->e("GameCenter challenges not implemented.");
		#endif
	}
	- (void) reportScore: (int64_t) score forCategory: (NSString*) category
	{
		GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:category] autorelease];
		scoreReporter.value = score;
		[scoreReporter reportScoreWithCompletionHandler: ^(NSError *error)
		 {
			 //[self callDelegateOnMainThread: @selector(scoreReported:) withArg: NULL error: error];
		 }];
	}

	- (void) challengeAchievement: (NSString*) identifier percentComplete: (double) percentComplete
	{
		ARK2D::getLog()->v("Challenge Achievement.");
		if(self.earnedAchievementCache == NULL)
		{
			[GKAchievement loadAchievementsWithCompletionHandler: ^(NSArray *scores, NSError *error)
			{
				if(error == NULL)
				{
					NSMutableDictionary* tempCache= [NSMutableDictionary dictionaryWithCapacity: [scores count]];
					for (GKAchievement* score in scores)
					{
						score.showsCompletionBanner = YES;
						[tempCache setObject: score forKey: score.identifier];
					}
					self.earnedAchievementCache= tempCache;
					[self challengeAchievement: identifier percentComplete: percentComplete];
				}
				else
				{
					//Something broke loading the achievement list.  Error out, and we'll try again the next time achievements submit.
					//[self callDelegateOnMainThread: @selector(achievementSubmitted:error:) withArg: NULL error: error];
				}

			}];
		}
		else
		{
			 //Search the list for the ID we're using...
			GKAchievement* achievement= [self.earnedAchievementCache objectForKey: identifier];
			if(achievement != NULL)
			{
				if((achievement.percentComplete >= 100.0) || (achievement.percentComplete >= percentComplete))
				{
					//Achievement has already been earned so we're done.
					achievement= NULL;
				}
				achievement.percentComplete= percentComplete;
			}
			else
			{
				achievement= [[[GKAchievement alloc] initWithIdentifier: identifier] autorelease];
				achievement.percentComplete= percentComplete;
				//Add achievement to achievement cache...
				[self.earnedAchievementCache setObject: achievement forKey: achievement.identifier];
			}
			if(achievement!= NULL)
			{
				// Challenge the Achievement...
				#if defined(ARK2D_IPHONE)
					UIViewController* cc = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.glViewController;
					[cc performSegueWithIdentifier:@"achievementChallenge" sender:achievement];
				#elif defined(ARK2D_MACINTOSH)
					ARK2D::getLog()->e("GameCenter challengeAchievement not implemented.");
				#endif
			}
		}
	}
	- (void) submitAchievement: (NSString*) identifier percentComplete: (double) percentComplete
	{
		//GameCenter check for duplicate achievements when the achievement is submitted, but if you only want to report
		// new achievements to the user, then you need to check if it's been earned
		// before you submit.  Otherwise you'll end up with a race condition between loadAchievementsWithCompletionHandler
		// and reportAchievementWithCompletionHandler.  To avoid this, we fetch the current achievement list once,
		// then cache it and keep it updated with any new achievements.
		ARK2D::getLog()->v("Submitting Achievement.");
		if(self.earnedAchievementCache == NULL)
		{
			[GKAchievement loadAchievementsWithCompletionHandler: ^(NSArray *scores, NSError *error)
			{
				if(error == NULL)
				{
					NSMutableDictionary* tempCache= [NSMutableDictionary dictionaryWithCapacity: [scores count]];
					for (GKAchievement* score in scores)
					{
						score.showsCompletionBanner = YES;
						[tempCache setObject: score forKey: score.identifier];
					}
					self.earnedAchievementCache= tempCache;
					[self submitAchievement: identifier percentComplete: percentComplete];
				}
				else
				{
					//Something broke loading the achievement list.  Error out, and we'll try again the next time achievements submit.
					//[self callDelegateOnMainThread: @selector(achievementSubmitted:error:) withArg: NULL error: error];
				}

			}];
		}
		else
		{
			 //Search the list for the ID we're using...
			GKAchievement* achievement= [self.earnedAchievementCache objectForKey: identifier];
			if(achievement != NULL)
			{
				if((achievement.percentComplete >= 100.0) || (achievement.percentComplete >= percentComplete))
				{
					//Achievement has already been earned so we're done.
					achievement= NULL;
				}
				achievement.percentComplete= percentComplete;
			}
			else
			{
				achievement= [[[GKAchievement alloc] initWithIdentifier: identifier] autorelease];
				achievement.percentComplete= percentComplete;
				//Add achievement to achievement cache...
				[self.earnedAchievementCache setObject: achievement forKey: achievement.identifier];
			}
			if(achievement!= NULL)
			{
				//Submit the Achievement...
				[achievement reportAchievementWithCompletionHandler: ^(NSError *error)
				{
					//[self callDelegateOnMainThread: @selector(achievementSubmitted:error:) withArg: achievement error: error];

				   //[GKNotificationBanner showBannerWithTitle:@"Achievement" message:@"Completed!" completionHandler:^{}];

				}];
			}
		}
	}

	- (void) resetAchievements
	{
		self.earnedAchievementCache= NULL;
		[GKAchievement resetAchievementsWithCompletionHandler: ^(NSError *error)
		{
			 [self callDelegateOnMainThread: @selector(achievementResetResult:) withArg: NULL error: error];
		}];
	}

	- (void) openGameCenter
	{
		GKGameCenterViewController* gameCenterController = [[GKGameCenterViewController alloc] init];
		if (gameCenterController != nil)
		{
			#if defined(ARK2D_IPHONE) 
				UIViewController* cc = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.glViewController;
				gameCenterController.gameCenterDelegate = (id<GKGameCenterControllerDelegate>) cc;
				[cc presentViewController: gameCenterController animated: YES completion:nil];
			#elif defined(ARK2D_MACINTOSH)
				NSViewController* view = [[NSApp keyWindow] contentViewController];
				[view insertChildViewController:gameCenterController atIndex:0];
				//[view presentViewControllerAsModalWindow:gameCenterController];
			#endif
		}
	}

	- (void) mapPlayerIDtoPlayer: (NSString*) playerID
	{
		[GKPlayer loadPlayersForIdentifiers: [NSArray arrayWithObject: playerID] withCompletionHandler:^(NSArray *playerArray, NSError *error)
		{
			GKPlayer* player= NULL;
			for (GKPlayer* tempPlayer in playerArray)
			{
				if([tempPlayer.playerID isEqualToString: playerID])
				{
					player= tempPlayer;
					break;
				}
			}
			[self callDelegateOnMainThread: @selector(mappedPlayerIDToPlayer:error:) withArg: player error: error];
		}];

	}
	@end









	@implementation GameICloudManager

	@synthesize iclouddocuments;
	@synthesize query = _query;

	- (void) processUpdates:(NSMetadataQuery*) query {

		ARK2D::getLog()->i("iCloud AppDelegate: processUpdates.");

		NSURL* ubiquitousURL = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];
		NSURL* ubiquitousDocumentsURL = [ubiquitousURL URLByAppendingPathComponent:@"Documents"];
		NSString* ubiquitousDocumentsStr = [ubiquitousDocumentsURL absoluteString];
		string ubiquitousDocumentsCStr = [ubiquitousDocumentsStr cStringUsingEncoding:[NSString defaultCStringEncoding]];

		if ([query resultCount] >= 1) {
			NSArray* queryResults = [query results];
			for (NSMetadataItem* result in queryResults)
			{
                //[self echoMetadataItem:result];
				if ([[result valueForAttribute:NSMetadataUbiquitousItemIsDownloadedKey] boolValue]// && 
					//[[item valueForAttribute:NSMetadataUbiquitousItemIsUploadedKey] boolValue]
					) {
					NSURL* fileURL = [result valueForAttribute:NSMetadataItemURLKey];
					NSLog(@"%@ downloaded.", [fileURL absoluteString]);  

					NSString* fileUrlStr = [fileURL absoluteString];
					string thsStrText = [fileUrlStr cStringUsingEncoding:[NSString defaultCStringEncoding]];
					thsStrText = thsStrText.substr(ubiquitousDocumentsCStr.length());
					NSString* keyStr = [NSString stringWithCString:thsStrText.c_str() encoding:[NSString defaultCStringEncoding]];

					
					AppleCloudDocument* doc = iclouddocuments[keyStr];
					if (doc != NULL) {
						NSLog(@"%@ downloaded and found in documents.", [fileURL absoluteString]);  
					}
				}
			}
		}
		
	}

	- (void)echoMetadataItem:(NSMetadataItem *)item {
		NSString *path = [item valueForAttribute:NSMetadataItemPathKey];
		NSString *display = [item valueForAttribute:NSMetadataItemDisplayNameKey];
		NSURL *url = [item valueForAttribute:NSMetadataItemURLKey];
		NSString *name = [item valueForAttribute:NSMetadataItemFSNameKey];
		NSNumber *downloaded = [item valueForAttribute:NSMetadataUbiquitousItemIsDownloadedKey];
		NSNumber *downloading = [item valueForAttribute:NSMetadataUbiquitousItemIsDownloadingKey];
		NSNumber *uploaded = [item valueForAttribute:NSMetadataUbiquitousItemIsUploadedKey];
		NSNumber *uploading = [item valueForAttribute:NSMetadataUbiquitousItemIsUploadingKey];
		NSDate *createData = [item valueForAttribute:NSMetadataItemFSCreationDateKey];
		NSDate *updateDate = [item valueForAttribute:NSMetadataItemFSContentChangeDateKey];
		NSNumber *hasConflicts = [item valueForAttribute:NSMetadataUbiquitousItemHasUnresolvedConflictsKey];

		NSLog(@"Metadata item name: %@, display: %@, path: %@, url: %@, ded: %@, ding: %@, ued:%@, uing: %@, created: %@, updated: %@, conflicts: %@", 
								  name,     display,     path,     url, downloaded, downloading, uploaded, uploading, createData, updateDate, hasConflicts);
	}

	- (void)loadData:(NSMetadataQuery *)query {

		// (4) iCloud: the heart of the load mechanism: if texts was found, open it and put it into _document; if not create it an then put it into _document

		ARK2D::getLog()->i("iCloud AppDelegate: loadData.");

		if ([query resultCount] >= 1) {
			ARK2D::getLog()->i("Found iCloud files...");

			NSURL* ubiquitousURL = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];
			NSURL* ubiquitousDocumentsURL = [ubiquitousURL URLByAppendingPathComponent:@"Documents"];
			NSString* ubiquitousDocumentsStr = [ubiquitousDocumentsURL absoluteString];
			string ubiquitousDocumentsCStr = [ubiquitousDocumentsStr cStringUsingEncoding:[NSString defaultCStringEncoding]];

			// for each result in the query...
			NSArray* queryResults = [query results];
			for (NSMetadataItem* result in queryResults)
			{
				//NSString* fileNameStr = [result valueForAttribute:NSMetadataItemFSNameKey];
				NSURL* fileURL = [result valueForAttribute:NSMetadataItemURLKey];
				NSNumber* aBool = nil;

				NSString* fileUrlStr = [fileURL absoluteString];
				string thsStrText = [fileUrlStr cStringUsingEncoding:[NSString defaultCStringEncoding]];
				thsStrText = thsStrText.substr(ubiquitousDocumentsCStr.length());
				NSString* keyStr = [NSString stringWithCString:thsStrText.c_str() encoding:[NSString defaultCStringEncoding]];

				

				// Don't include hidden files or directories
				[fileURL getResourceValue:&aBool forKey:NSURLIsHiddenKey error:nil];
				if (//aBool && ![aBool boolValue] && 
					![fileURL hasDirectoryPath] ) {

					ARK2D::getLog()->i(StringUtil::append("Found iCloud file: ", thsStrText));  
					NSLog(@"(%@)", [fileURL absoluteString]);  

                    [AppleCloudDocument setOpenSuccessfulHandler:[] (AppleCloudDocument* ldoc, bool success) {
                    	ARK2D::getLog()->i("AppleCloudDocument::OpenSuccessfulHandler");
						if (success) {
							// add to dictionary
                            if (ldoc->mymeta != NULL) {
								ARK2D::getLog()->i(StringUtil::append("AppDelegate: existing document opened from iCloud: ", ldoc->mymeta->filename));

								if (ldoc->mymeta->onchangefunction != NULL) { 
									void (*pt)() = (void(*)()) ldoc->mymeta->onchangefunction;
									pt();
								}
                                else {
                                    ARK2D::getLog()->i("AppDelegate: ldoc->mymeta->onchangefunction == NULL");
                                }
                            } else {
                                ARK2D::getLog()->i("AppDelegate: ldoc->mymeta == NULL");
                            }
						} else {
							ARK2D::getLog()->i("AppDelegate: existing document failed to open from iCloud");
						}

						ICloudUtil::s_remainingFiles--;
						if (ICloudUtil::s_remainingFiles == 0) {
							ICloudUtil::push();
						}
                    }];;

                    #ifdef ARK2D_IPHONE
                        AppleCloudDocument* doc = [[AppleCloudDocument alloc] initWithFileURL:fileURL];
                        //doc.delegate = self.glViewController;
                        //self.glViewController.document = doc;
                    #elif defined(ARK2D_MACINTOSH)
                        NSError* outError;
                        AppleCloudDocument* doc = [AppleCloudDocument alloc];
                    #endif
                    doc->initFileURL = fileURL;

					//ARK2D::getLog()->i("Trying to assign ICloudFile... ");
					bool foundMeta = false;
					vector<ICloudFile>* fiz = ICloudUtil::getFiles();
					for(unsigned int i = 0; i < fiz->size(); ++i) {
						string cmpStr = string("Comparing ");
						cmpStr.append(fiz->at(i).filename);
						cmpStr.append(" to ");
						cmpStr.append(thsStrText);
						//ARK2D::getLog()->i(cmpStr);

						if (fiz->at(i).filename == thsStrText) {
                            //ARK2D::getLog()->i("-- Setting doc meta");
							doc->mymeta = &fiz->at(i);
							foundMeta = true;
							break;
						}
					}
					if (foundMeta == true) { 
						ICloudUtil::s_remainingFiles++;
						[iclouddocuments setValue:doc forKey:keyStr];
					}


					#if defined(ARK2D_MACINTOSH)
					//NSError* outError;
                
                    // bool success = [doc initWithContentsOfURL:doc->initFileURL ofType:NSPlainTextDocumentType error:&outError];
                    // if (!success) {
                    // 	NSLog(@"Error opening file icloud %@ because %@.", doc->initFileURL, outError);
                    // }
					#endif
				}

			}

			for(id theKey in iclouddocuments) {
				AppleCloudDocument* doc = iclouddocuments[theKey];
				ARK2D::getLog()->i(StringUtil::append("Loading iCloud file: ", doc->mymeta->filename));  

				#if defined(ARK2D_IPHONE)
                    [doc openWithCompletionHandler:^(BOOL success) {
                    	auto cb = [AppleCloudDocument getOpenSuccessfulHandler];
                    	cb(doc, success);
                }];
                #elif defined(ARK2D_MACINTOSH)
                    NSError* outError;
                
                    bool success = [doc initWithContentsOfURL:doc->initFileURL ofType:NSPlainTextDocumentType error:&outError];
                    if (!success) {
                    	NSLog(@"Error opening file icloud %@ because %@.", doc->initFileURL, outError);
                    }

                #endif
            }
	   	} 

	   	if ([query resultCount] == 0 || 
	   		([iclouddocuments count] == 0 && ICloudUtil::countFiles() > 0) // found files but not any that we defined.
	   		) {
		   // Nothing in iCloud: create a container for file and give it URL
		   ARK2D::getLog()->i(StringUtil::append("AppDelegate: no documents found in iCloud... make them?! ", (int) [query resultCount]));

		   vector<ICloudFile>* createfiles = ICloudUtil::getFiles();
		   for(unsigned int i = 0; i < createfiles->size(); ++i) {

			ICloudFile* icf = &createfiles->at(i);
			ICloudUtil::_createFile(icf);
		   }
		   ICloudUtil::push();
	   }
	}

	/*- (void)processFiles:(NSNotification*)aNotification {
		NSMutableArray* discoveredFiles = [NSMutableArray array];

		// Always disable updates while processing results.
		[_query disableUpdates];

		// The query reports all files found, every time.
		NSArray* queryResults = [_query results];
		for (NSMetadataItem* result in queryResults) {
			NSURL* fileURL = [result valueForAttribute:NSMetadataItemURLKey];
			NSNumber* aBool = nil;

			// Don't include hidden files.
			[fileURL getResourceValue:&aBool forKey:NSURLIsHiddenKey error:nil];
			if (aBool && ![aBool boolValue]) {
				[discoveredFiles addObject:fileURL];
			}
		}

		// Update the list of documents.
		[documents removeAllObjects];
		[documents addObjectsFromArray:discoveredFiles];
		[self.tableView reloadData];

		// Reenable query updates.
		[_query enableUpdates];
	}*/


	- (void)queryDidUpdateNotification:(NSNotification *)notification {
		ARK2D::getLog()->i("iCloud - queryDidUpdateNotification");   
		//[self queryDidFinishGathering:notification];

		NSMetadataQuery* query = [notification object];
		[query disableUpdates];
		//[query stopQuery];

		[self processUpdates:query];
		
		[query enableUpdates];
	}

	- (void)queryDidStartGathering:(NSNotification *)notification {
		ARK2D::getLog()->i("iCloud - queryDidStartGathering");
	}

	- (void)queryDidFinishGathering:(NSNotification *)notification {

		ARK2D::getLog()->i("iCloud - queryDidFinishGathering");
		// (3) if Query is finished, this will send the result (i.e. either it found our text.dat or it didn't) to the next function

		NSMetadataQuery* query = [notification object];
		[query disableUpdates];
		//[query stopQuery];

		

		[[NSNotificationCenter defaultCenter] removeObserver:self name:NSMetadataQueryDidFinishGatheringNotification object:query];
		//_query = nil; // we're done with it

		[self loadData:query];

		[query enableUpdates];
	}

	-(NSMetadataQuery*)namedDocumentQuery {

		ARK2D::getLog()->i("iCloud - AppDelegate: namedDocumentQuery");
		// (2) iCloud query: Looks if there exists a file called text.txt in the cloud

		NSMetadataQuery* query = [[NSMetadataQuery alloc] init];
		if (query) {
			//_query = query;
			//SCOPE
			[query setSearchScopes:[NSArray arrayWithObject:NSMetadataQueryUbiquitousDocumentsScope]];

			//PREDICATE
			//NSPredicate *pred = [NSPredicate predicateWithFormat: @"%K == %@", NSMetadataItemFSNameKey, @"text.txt"];
			//[query setPredicate:pred];
	//        NSString* filePattern = [NSString stringWithFormat:@"%@", @"*"];

			NSPredicate* pred = [NSPredicate predicateWithFormat:@"kMDItemFSName LIKE 'iCloud' || kMDItemDisplayName LIKE 'iCloud'"];
			NSLog(@"%@",pred.description);

			[query setPredicate:pred];

		}
		else {
			ARK2D::getLog()->i("iCloud - AppDelegate: query was null");
		}

		//FINISHED?
		//[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(queryDidFinishGathering:) name:NSMetadataQueryDidFinishGatheringNotification object:query];
		//[query startQuery];

		return query;
	}

	-(void)loadDocuments {

		ARK2D::getLog()->i("iCloud - AppDelegate: load documents.");
		if (!iclouddocuments) {
			iclouddocuments = [[NSMutableDictionary alloc] init];
		}

		NSMetadataQuery* q = [[NSMetadataQuery alloc] init];
		query = q;

		[query setSearchScopes:[NSArray arrayWithObject:NSMetadataQueryUbiquitousDocumentsScope]];
		
		NSPredicate* pred = [NSPredicate predicateWithFormat:@"%K LIKE '*'", NSMetadataItemFSNameKey]; NSLog(@"%@",pred.description);
		[query setPredicate:pred];

		

		// Register for the metadata query notifications.
		[[NSNotificationCenter defaultCenter] 
			 addObserver:self 
			 selector:@selector(queryDidStartGathering:)
			 name:NSMetadataQueryDidStartGatheringNotification 
			 object:query
		];
		// Listen for first phase gathering
		[[NSNotificationCenter defaultCenter] 
			addObserver:self
			selector:@selector(queryDidFinishGathering:)
			name:NSMetadataQueryDidFinishGatheringNotification
			object:query];

		// Listen for the second phase live-updating
		[[NSNotificationCenter defaultCenter] 
			addObserver:self
			selector:@selector(queryDidUpdateNotification:)
			name:NSMetadataQueryDidUpdateNotification
			object:query];
		

		dispatch_async(dispatch_get_main_queue (), ^{
			// Start the query and let it run.
			BOOL b = [query startQuery];
			if (!b) {
				ARK2D::getLog()->e("Did not start query?");
			}
		});


	}

	@end

#endif
