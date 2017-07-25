//
//  IPhoneAppDelegate.m
//  IPhone
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "GameContainerIPhoneAppDelegate.h"

#import "GameContainerIPhoneGLView.h"
#import "GameContainerIPhoneCloudDocument.h"

#include "../ARK2D.h"
#include "../GameContainer.h"

#include "../Graphics/TextureStore.h"
#include "../../Util/ICloudUtil.h"







iOSListener* iOSListener::s_instance = new iOSListener();

//GameContainerIPhoneGLView* _glView;

@implementation GameContainerIPhoneAppDelegate

@synthesize window = _window;
@synthesize glViewController = _glViewController;
//@synthesize glView = _glView;
//@synthesize document = _document;
@synthesize iclouddocuments = _iclouddocuments;
@synthesize query = _query;

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
			if (aBool && ![aBool boolValue] && ![fileURL hasDirectoryPath] ) {

				ARK2D::getLog()->i(StringUtil::append("Loading iCloud file: ", thsStrText));				

				IPhoneCloudDocument* doc = [[IPhoneCloudDocument alloc] initWithFileURL:fileURL];
				//_document = doc;
				doc.delegate = self.glViewController;
				self.glViewController.document = doc;

				//ARK2D::getLog()->i("Trying to assign ICloudFile... ");
				vector<ICloudFile>* fiz = ICloudUtil::getFiles();
				for(unsigned int i = 0; i < fiz->size(); ++i) {
					string cmpStr = string("Comparing ");
					cmpStr.append(fiz->at(i).filename);
					cmpStr.append(" to ");
					cmpStr.append(thsStrText);
					//ARK2D::getLog()->i(cmpStr);

					if (fiz->at(i).filename == thsStrText) {
						doc.mymeta = &fiz->at(i);
						break;
					}
				}

				ICloudUtil::s_remainingFiles++;
				[_iclouddocuments setValue:doc forKey:keyStr];

				[doc openWithCompletionHandler:^(BOOL success) {
					if (success) {
						// add to dictionary
                        if (doc.mymeta != NULL) {
                        	ARK2D::getLog()->i(StringUtil::append("AppDelegate: existing document opened from iCloud: ", doc.mymeta->filename));

                        	if (doc.mymeta->onchangefunction != NULL) { 
								void (*pt)() = (void(*)()) doc.mymeta->onchangefunction;
								pt();
							}
						}
					} else {
						ARK2D::getLog()->i("AppDelegate: existing document failed to open from iCloud");
					}

					ICloudUtil::s_remainingFiles--;
					if (ICloudUtil::s_remainingFiles == 0) {
						ICloudUtil::push();
					}
				}];
			}

		}



   } else {
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
	[query stopQuery];

	//[self loadData:query];
	// TODO update documents. 

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
	if (!_iclouddocuments) {
		_iclouddocuments = [[NSMutableDictionary alloc] init];
	}

	NSMetadataQuery* query = [[NSMetadataQuery alloc] init];
	[query setSearchScopes:[NSArray arrayWithObject:NSMetadataQueryUbiquitousDocumentsScope]];
	
	NSPredicate* pred = [NSPredicate predicateWithFormat:@"%K LIKE '*'", NSMetadataItemFSNameKey]; NSLog(@"%@",pred.description);
	[query setPredicate:pred];

	_query = query;

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
		object:_query];
	

	dispatch_async(dispatch_get_main_queue (), ^{
		// Start the query and let it run.
		BOOL b = [_query startQuery];
		if (!b) {
			ARK2D::getLog()->e("Did not start query?");
		}
	});


}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	ARK2D::getContainer()->m_platformSpecific.m_appDelegate = self;

	// Programatticly add GL views.
	self.window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
	//self.glView = [[GameContainerIPhoneGLView alloc] initWithFrame:self.window.bounds];
	self.glViewController = [[GameContainerIPhoneGLViewController alloc] init];
	//[self.window addSubview: self.glView];
	//self.window.rootViewController = self.glView.viewController;
	//[self.window setRootViewController: [self viewController]];
	[self.window setRootViewController: self.glViewController];
	[self.window makeKeyAndVisible];

	iOSListener::get()->didFinishLaunchingWithOptions(launchOptions);

	return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application
{
	/*
	 Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	 Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
	 */
   // [self.viewController stopAnimation];

	// stop...
	 Log* arklog = ARK2D::getLog();
	if (arklog != NULL) {
		arklog->i("native pause");
		if (!GameContainerPlatform::s_gamePaused) {
			arklog->i("native pause start");
			ARK2D::getGame()->pause();
			GameContainerPlatform::s_gamePaused = true;
			arklog->i("native pause end");
		} else {
			arklog->i("native pause: game paused when already paused?");
		}
	}
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
	/*
	 Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
	 If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
	 */
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	/*
	 Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
	 */
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	/*
	 Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
	 */
	// [self.viewController startAnimation];

	// resume...
	Log* arklog = ARK2D::getLog();
	if (arklog != NULL) {
		arklog->i("native resume");



		// have we just come from a resume?
		if (GameContainerPlatform::s_gamePaused == true) {
			arklog->i("native resume start");
			ARK2D::getGame()->resume();
			arklog->i("native resume end");
			//GameContainerPlatform::s_gamePaused = false;

			// do this after the context is created otherwise it won't work.
			Renderer* r = ARK2D::getRenderer();
			r->init();
			FBOStore::getInstance()->reloadFBOs();
			TextureStore::getInstance()->reloadTextures();
			GameContainerPlatform::s_gamePaused = false;

			//ARK2D::getContainer()->setSize((int) self.window.bounds.size.width, (int) self.window.bounds.size.height);

		} else {
			arklog->i("native resume: game resumed when already running?");
		   // ARK2D::getContainer()->setSize((int) self.window.bounds.size.width, (int) self.window.bounds.size.height);
		}
	}

}

- (void)applicationWillTerminate:(UIApplication *)application
{
	/*
	 Called when the application is about to terminate.
	 Save data if appropriate.
	 See also applicationDidEnterBackground:.
	 */
   // [self.viewController stopAnimation];
}

- (void)dealloc
{
	[_window release];
	[_glViewController release];
	//[_glView release];
	[super dealloc];
}

@end





@implementation AudioViewController

-(void)playAudio
{
	[audioPlayer play];
}
-(void)stopAudio
{
	[audioPlayer stop];
}
-(void)adjustVolume
{
	if (audioPlayer != nil)
	{
		//audioPlayer.volume = volumeControl.value;
	}
}

- (void) resumePlayback {

	UInt32 sessionCategory = kAudioSessionCategory_MediaPlayback;  // 1
	AudioSessionSetProperty (                                      // 2
							 kAudioSessionProperty_AudioCategory,                       // 3
							 sizeof (sessionCategory),                                  // 4
							 &sessionCategory                                           // 5
							 );
	AudioSessionSetActive (true);                                  // 6

//    [audioPlayer resume];                                     // 7
}

@end











