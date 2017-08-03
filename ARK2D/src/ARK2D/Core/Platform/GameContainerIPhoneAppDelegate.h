//
//  GameContainerIPhoneAppDelegate.h
//  IPhone
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//



#include "../../Includes.h"
#include "../../Common/DLL.h"
#import "GameContainerIPhoneGLView.h"
#import "GameContainerAppleCloudDocument.h"
#import "GameContainerAppleShared.h"

//#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>


class ARK2D_API iOSListener {
	public:
		static iOSListener* s_instance;

		static iOSListener* get() { return s_instance; }
		static void set(iOSListener* instance) { s_instance = instance; }

		virtual void didFinishLaunchingWithOptions(NSDictionary* ns) { }
		virtual void didReceiveMemoryWarning() { }
		// flesh this out with events.

};



//GameContainerIPhoneGLView* glView;
@interface GameContainerIPhoneAppDelegate : UIResponder <UIApplicationDelegate> {
    //NSMetadataQuery* _query;

}

@property (nonatomic, retain) IBOutlet UIWindow* window;
@property (nonatomic, retain) IBOutlet GameContainerIPhoneGLViewController* glViewController;

//@property (nonatomic, retain) IBOutlet GameContainerIPhoneGLView* glView;
@property (strong, nonatomic) GameICloudManager* icloud;

@end




#import <AVFoundation/AVFoundation.h>

@interface AudioViewController : UIViewController
<AVAudioPlayerDelegate>
{
	AVAudioPlayer* audioPlayer;
//	UISlider* volumeControl;
}
//@property (strong, nonatomic) IBOutlet UISlider *volumeControl;
-(IBAction) playAudio;
-(IBAction) stopAudio;
-(IBAction) adjustVolume;
-(void) resumePlayback;
@end

@class GKLeaderboard, GKAchievement, GKPlayer;





