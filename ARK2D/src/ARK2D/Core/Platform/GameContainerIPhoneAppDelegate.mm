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

#include "../../ARK2D.h"
#include "../../Core/GameContainer.h"

#include "../../Graphics/TextureStore.h"
#include "../../Util/ICloudUtil.h"








 
//GameContainerIPhoneGLView* _glView;

@implementation GameContainerIPhoneAppDelegate

@synthesize window = _window;
@synthesize glViewController = _glViewController;
//@synthesize glView = _glView;
//@synthesize document = _document;
@synthesize iclouddocuments = _iclouddocuments;

- (void)loadData:(NSMetadataQuery *)query {

    // (4) iCloud: the heart of the load mechanism: if texts was found, open it and put it into _document; if not create it an then put it into _document

    ARK2D::getLog()->v("AppDelegate: loadData.");

    if ([query resultCount] >= 1) { 


        ARK2D::getLog()->i("Found iCloud files...");

        // for each result in the query...
        NSArray* queryResults = [query results];
        for (NSMetadataItem* result in queryResults) 
        {
            NSURL* fileURL = [result valueForAttribute:NSMetadataItemURLKey];
            //NSNumber* aBool = nil;

            NSString* fileUrlStr = [fileURL absoluteString];
            string thsStrText = [fileUrlStr cStringUsingEncoding:[NSString defaultCStringEncoding]];

            ARK2D::getLog()->i(StringUtil::append("Found iCloud file: ", thsStrText));
     
             // found the file in iCloud
            //NSMetadataItem *item = [query resultAtIndex:0];
            //NSURL *url = [item valueForAttribute:NSMetadataItemURLKey];

            IPhoneCloudDocument* doc = [[IPhoneCloudDocument alloc] initWithFileURL:fileURL];
            //_document = doc;
            doc.delegate = self.glViewController;
            self.glViewController.document = doc;

            vector<ICloudFile>* fiz = ICloudUtil::getFiles();
            for(unsigned int i = 0; i < fiz->size(); ++i) {
                if (fiz->at(i).filename == thsStrText) { 
                    doc.mymeta = &fiz->at(i);
                    break;
                }
            }

            ICloudUtil::s_remainingFiles++;
            [_iclouddocuments setValue:doc forKey:fileUrlStr];

            [doc openWithCompletionHandler:^(BOOL success) {
                if (success) {
                    ARK2D::getLog()->i("AppDelegate: existing document opened from iCloud");
                    
                    // add to dictionary
                    if (doc.mymeta->onchangefunction != NULL) {
                        void (*pt)() = (void(*)()) doc.mymeta->onchangefunction;
                        pt(); 
                    }
                } else {
                    ARK2D::getLog()->i("AppDelegate: existing document failed to open from iCloud");
                }

                ICloudUtil::s_remainingFiles--;
                if (ICloudUtil::s_remainingFiles == 0) {
                    ICloudUtil::push();    
                }
            }];

            

            // Don't include hidden files.
            //[fileURL getResourceValue:&aBool forKey:NSURLIsHiddenKey error:nil];
            //if (aBool && ![aBool boolValue]) { 
            //    [discoveredFiles addObject:fileURL];
            //}
        }


        
    } else {
        // Nothing in iCloud: create a container for file and give it URL
        ARK2D::getLog()->i("AppDelegate: no documents found in iCloud... make them?!");

        vector<ICloudFile>* createfiles = ICloudUtil::getFiles();
        for(unsigned int i = 0; i < createfiles->size(); ++i) {
 
        	ICloudFile* icf = &createfiles->at(i);
        	ICloudUtil::_createFile(icf);
        }
        ICloudUtil::push();  

       /* 

       
        //_document = doc;
       
        self.glViewController.document = doc;

       */
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


- (void)queryDidFinishGathering:(NSNotification *)notification {

	ARK2D::getLog()->v("iCloud - queryDidFinishGathering");
    // (3) if Query is finished, this will send the result (i.e. either it found our text.dat or it didn't) to the next function

    NSMetadataQuery *query = [notification object];
    [query disableUpdates];
    [query stopQuery];

    [self loadData:query];

    [[NSNotificationCenter defaultCenter] removeObserver:self name:NSMetadataQueryDidFinishGatheringNotification object:query];
    _query = nil; // we're done with it
}

-(NSMetadataQuery*)namedDocumentQuery {

    // (2) iCloud query: Looks if there exists a file called text.txt in the cloud

    NSMetadataQuery* query = [[NSMetadataQuery alloc] init];
    if (query) {
        //_query = query;
        //SCOPE
        [query setSearchScopes:[NSArray arrayWithObject:NSMetadataQueryUbiquitousDocumentsScope]];
        
        //PREDICATE
        //NSPredicate *pred = [NSPredicate predicateWithFormat: @"%K == %@", NSMetadataItemFSNameKey, @"text.txt"];
        //[query setPredicate:pred];
        NSString* filePattern = [NSString stringWithFormat:@"*.%@", @"*"];
        [query setPredicate:[NSPredicate predicateWithFormat:@"%K LIKE %@", NSMetadataItemFSNameKey, filePattern]];
    }

    //FINISHED?
    //[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(queryDidFinishGathering:) name:NSMetadataQueryDidFinishGatheringNotification object:query];
    //[query startQuery];

    return query;
}

-(void)loadDocuments {

    ARK2D::getLog()->v("AppDelegate: load documents.");
    if (!_iclouddocuments) {
        _iclouddocuments = [[NSMutableDictionary alloc] init];
    }

    if (!_query) { 
        _query = [self namedDocumentQuery];
    }
    
     // Register for the metadata query notifications. 
   [[NSNotificationCenter defaultCenter] addObserver:self 
        //selector:@selector(processFiles:)
        selector:@selector(queryDidFinishGathering:) 
        name:NSMetadataQueryDidFinishGatheringNotification
        object:_query];
        //object:nil];
   [[NSNotificationCenter defaultCenter] addObserver:self
        //selector:@selector(processFiles:)
        selector:@selector(queryDidFinishGathering:)
        name:NSMetadataQueryDidUpdateNotification
        object:_query];
        //object:nil];
 
   // Start the query and let it run.
   [_query startQuery];

    
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











