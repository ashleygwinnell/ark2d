//
//  GameContainerIPhoneAppDelegate.h
//  IPhone
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//



#include "../../Includes.h"
#import "GameContainerIPhoneGLView.h"
#import "GameContainerIPhoneCloudDocument.h"

//#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>






//GameContainerIPhoneGLView* glView;
@interface GameContainerIPhoneAppDelegate : UIResponder <UIApplicationDelegate> {
	NSMetadataQuery *_query;

}

-(void)loadData:(NSMetadataQuery *)query;
-(void)loadDocuments;

@property (nonatomic, retain) IBOutlet UIWindow* window;
@property (nonatomic, retain) IBOutlet GameContainerIPhoneGLViewController* glViewController;
//@property (nonatomic, retain) IBOutlet GameContainerIPhoneGLView* glView;
//@property (strong, nonatomic) IPhoneCloudDocument* document;
@property (strong, nonatomic) NSMutableDictionary* iclouddocuments; // = [NSDictionary dictionary];

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
 




