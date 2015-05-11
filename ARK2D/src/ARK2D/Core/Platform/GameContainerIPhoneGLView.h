//
//  IPhoneViewController.h
//  IPhone
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

//#import <UIKit/UIKit.h>

#include "../../Includes.h"
#import "GameContainerIPhoneCloudDocument.h"
/*
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import <QuartzCore/QuartzCore.h>*/
//#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//#import "../../vendor/iphone/ga/GAITrackedViewController.h"

// @interface MyDICDController : UIViewController <UIDocumentInteractionControllerDelegate>

// @end


@protocol GameCenterManagerDelegate <NSObject>
@optional
- (void) processGameCenterAuth: (NSError*) error;
- (void) scoreReported: (NSError*) error;
- (void) reloadScoresComplete: (GKLeaderboard*) leaderBoard error: (NSError*) error;
- (void) achievementSubmitted: (GKAchievement*) ach error:(NSError*) error;
- (void) achievementResetResult: (NSError*) error;
- (void) mappedPlayerIDToPlayer: (GKPlayer*) player error: (NSError*) error;
@end




// GameCenter?
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


@interface GameCenterManager : NSObject
{
    NSMutableDictionary* earnedAchievementCache;
    
    id <GameCenterManagerDelegate, NSObject> delegate;
}

//This property must be attomic to ensure that the cache is always in a viable state...
@property (retain) NSMutableDictionary* earnedAchievementCache;

@property (nonatomic, assign)  id <GameCenterManagerDelegate> delegate;

+ (BOOL) isGameCenterAvailable;

- (void) authenticateLocalUser;
- (BOOL) isAuthenticatedLocalUser;

- (void) reportScore: (int64_t) score forCategory: (NSString*) category;
- (void) challengeScore: (int64_t) score forCategory: (NSString*) category;
- (void) reloadHighScoresForCategory: (NSString*) category;

- (void) submitAchievement: (NSString*) identifier percentComplete: (double) percentComplete;
- (void) resetAchievements;
- (void) challengeAchievement: (NSString*) identifier percentComplete: (double) percentComplete;

- (void) openGameCenter;

- (void) mapPlayerIDtoPlayer: (NSString*) playerID;
@end


@interface InputDialogViewController : UIViewController <UIAlertViewDelegate>
	@property unsigned int m_callbackId;
    - (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;

@end
                

@interface GameContainerIPhoneGLViewController : UIViewController <UIDocumentInteractionControllerDelegate, GameCenterManagerDelegate, GKGameCenterControllerDelegate> {
//@interface GameContainerIPhoneGLViewController : GAITrackedViewController <UIDocumentInteractionControllerDelegate, GameCenterManagerDelegate, GKGameCenterControllerDelegate> {
    GameCenterManager* gameCenterManager;
	IPhoneCloudDocument* document;
}
    @property (nonatomic, retain) GameCenterManager* gameCenterManager;
    @property (strong, nonatomic) IPhoneCloudDocument* document;
    
    - (GameCenterManager*) getGameCenter; 
	- (void)loadView; 
    - (void)viewDidLoad;
    - (void)dealloc;
    // - (void)viewDidAppear:(BOOL)animated;

    - (UIViewController *)documentInteractionControllerViewControllerForPreview:(UIDocumentInteractionController *)controller;
    - (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)gameCenterViewController;

    - (void)didReceiveMemoryWarning;

@end
 
@interface GameContainerIPhoneGLView : UIView 
  
  	//- (UIViewController *)viewController;
	/*CAEAGLLayer* _eaglLayer;
    EAGLContext* _context;
    CADisplayLink* _displayLink;

    // OpenGL names for the renderbuffer and framebuffers used to render to this view
    GLuint _viewRenderBuffer;
    GLuint _viewFrameBuffer;*/



@end
