//
//  IPhoneAppDelegate.m
//  IPhone
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "GameContainerIPhoneAppDelegate.h"

#import "GameContainerIPhoneGLView.h"
#import "GameContainerAppleCloudDocument.h"

#include "../ARK2D.h"
#include "../GameContainer.h"

#include "../Graphics/TextureStore.h"
#include "../../Util/ICloudUtil.h"







iOSListener* iOSListener::s_instance = new iOSListener();

//GameContainerIPhoneGLView* _glView;

@implementation GameContainerIPhoneAppDelegate

@synthesize window = _window;
@synthesize glViewController = _glViewController;
@synthesize icloud = _icloud;
//@synthesize glView = _glView;
//@synthesize document = _document;



- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	ARK2D::getContainer()->m_platformSpecific.m_appDelegate = self;
	self.icloud = [[GameICloudManager alloc] init];

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











