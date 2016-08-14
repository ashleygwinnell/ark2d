//
//  IPhoneViewController.m
//  IPhone
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//



#import "GameContainerIPhoneGLView.h"
#import "GameContainerIPhone.h"
#import "GameContainerIPhoneCloudDocument.h"


#include "../../Includes.h"
#include "../../Namespaces.h"
#include "../../ARK2D.h"
#include "../../Core/GameContainer.h"
#include "../../Graphics/Color.h"
#include "../../UI/Dialog.h"
#include "../../Util/Callbacks.h"

#include "../../Util/ICloudUtil.h"


@implementation GameContainerIPhoneGLViewController

@synthesize gameCenterManager;
@synthesize document = _document;

    - (GameCenterManager*) getGameCenter {
        return gameCenterManager;
    }
    - (void)loadView {
     CGRect frame = [[UIScreen mainScreen] bounds];
     self.view = [[GameContainerIPhoneGLView alloc] initWithFrame:frame];
     self.view.multipleTouchEnabled = YES;
    }

   // - (void)viewDidAppear:(BOOL)animated {
   //     [super viewDidAppear:animated];
   //     self.trackedViewName = @"Toast Time";
   // }

    - (void)viewDidLoad {
        [super viewDidLoad];
    }



    - (void)dealloc {
     [super dealloc];
    }



    - (void)previewDocumentWithURL:(NSURL*)url
    {
        UIDocumentInteractionController* preview = [UIDocumentInteractionController interactionControllerWithURL:url];
        preview.delegate = self;
        [preview presentPreviewAnimated:YES];
        [preview retain];
    }

    - (void)documentInteractionControllerDidEndPreview:(UIDocumentInteractionController *)controller
    {
        [controller autorelease];
    }

    - (UIViewController *)documentInteractionControllerViewControllerForPreview:(UIDocumentInteractionController *)controller{
        return self;
    }

    - (CGRect)documentInteractionControllerRectForPreview:(UIDocumentInteractionController *)controller{
        return self.view.frame;
    }

    - (UIView *)documentInteractionControllerViewForPreview:(UIDocumentInteractionController *)controller{
        return self.view;
    }

    //Called when the player is done interacting with the GKGameCenterViewController
    - (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)gameCenterViewController
    {
        [self dismissViewControllerAnimated:YES completion:nil];
    }

    - (void)didReceiveMemoryWarning
    {
        ErrorDialog::createAndShow("Low memory!");
    }

@end

// ----------------------------



@implementation InputDialogViewController

     - (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
     {
          NSString* inputText = [[alertView textFieldAtIndex:0] text];
          //NSLog(@"Entered: %@",inputText);

          string thsStrText = [inputText cStringUsingEncoding:[NSString defaultCStringEncoding]];
          ARK::UI::Dialog::setInputDialogText( thsStrText );
          //ARK2D::getLog()->v(StringUtil::append("inputted: ", thsStrText ));

            Callbacks::invoke(_m_callbackId);


     }

@end


// ----------------------------

@implementation GameContainerIPhoneGLView

+ (Class) layerClass {
    return [CAEAGLLayer class];
}

- (id) initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (!self) {
        NSLog(@"Did not initWithFrame");
        exit(1);
    }


    CAEAGLLayer* eaglLayer = self.layer;
    //eaglLayer.drawableProperties = @{
    //    kEAGLDrawablePropertyRetainedBacking: [NSNumber numberWithBool:YES],
    //    kEAGLDrawablePropertyColorFormat: kEAGLColorFormatRGBA8
    //};
	//ll.kEAGLDrawablePropertyRetainedBacking = YES;

    // Adjust for retina displays
    if ([self respondsToSelector:@selector(setContentScaleFactor:)])
    {
        self.contentScaleFactor = [[UIScreen mainScreen] scale];
    }

    int width = self.bounds.size.width * [UIScreen mainScreen].scale;
    int height = self.bounds.size.height * [UIScreen mainScreen].scale;


    GameContainerPlatform* p = ARK::Core::GameContainerPlatform::getInstance();
    p->m_container->setSize(width, height, false);
    p->initOpenGL(self);
    p->initOpenGL2D(width, height);

    //const Color& cc = p->m_container->getClearColor();
    //glClearColor(cc.getRedf(), cc.getGreenf(), cc.getBluef(), cc.getAlpha());

    //p->initOpenAL(); // done in start()
    p->initGame();
    //p->start();

    // init iCloud
    //ICloudUtil::init();

    CADisplayLink* aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:(id)self selector:@selector(updateAndRender)];
    [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    p->_displayLink = aDisplayLink;


    return self;
}
- (void) updateAndRender
{
    ARK::Core::GameContainerPlatform::getInstance()->updateAndRender();
}


/*- (UIViewController *)viewController {
    if ([self.nextResponder isKindOfClass:UIViewController.class])
        return (UIViewController *)self.nextResponder;
    else
        return nil;
}*/



float transformTouchPointX(float x) {
    GameContainer* container = ARK2D::getContainer();
    x *= [UIScreen mainScreen].scale;;
    x /= container->getScale();
    x -= container->getTranslateX();
    return x;
}
float transformTouchPointY(float y) {
    GameContainer* container = ARK2D::getContainer();
    y *= [UIScreen mainScreen].scale;
    y /= container->getScale();
    y -= container->getTranslateY();
    return y;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    GameContainer* container = ARK2D::getContainer();
    if (container != NULL)
    {
        //NSUInteger touchCount = [touches count];
        //NSUInteger tapCount = [[touches anyObject] tapCount];

       // CGPoint lastPoint = [touch previousLocationInView:self];
        CGPoint newPoint = [[touches anyObject] locationInView:self];
       /* newPoint.x *= [UIScreen mainScreen].scale;
        newPoint.y *= [UIScreen mainScreen].scale;

        float thisx = newPoint.x;
        float thisy = newPoint.y;
        //float thisx = (float) x;
        //float thisy = (float) y;
        thisx /= container->getScale();
        thisy /= container->getScale();

        thisx -= container->getTranslateX();
        thisy -= container->getTranslateY();*/
        float thisx = transformTouchPointX(newPoint.x);
        float thisy = transformTouchPointY(newPoint.y);

        // *******
        // TODO: landscape transform coordinates
        // *******

       /* string logstr = "touch-begin: ";
        logstr += Cast::toString<int>((int) thisx);
        logstr += ",";
        logstr += Cast::toString<int>((int) thisy);
        ARK2D::getLog()->i(logstr);*/

        Input* i = ARK2D::getInput();
        i->mouse_x = (int) thisx;
        i->mouse_y = (int) thisy;
        i->pressKey(Input::MOUSE_BUTTON_LEFT);

        // Multi-touch
        for (UITouch* touch in touches)
        {
             //found a touch.  Is it already on our list?
			int fingerID = i->getTouchByInternalData(touch);

			if (fingerID == -1) {
				// add it to our list
				CGPoint pt = [touch locationInView:self];
				Pointer p;
				p.x = transformTouchPointX(pt.x);
				p.y = transformTouchPointY(pt.y);
				p.data = touch;
				fingerID = i->addTouch(p);

                ARK2D::getLog()->v("adding touch point");
			} else {
				//already on the list.  Don't send this
				ARK2D::getLog()->w(StringUtil::append("Ignoring touch ", fingerID));
				continue;
			}
        }
    }
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    GameContainer* container = ARK2D::getContainer();
    if (container != NULL)
    {
        CGPoint newPoint = [[touches anyObject] locationInView:self];
        /*newPoint.x *= [UIScreen mainScreen].scale;
        newPoint.y *= [UIScreen mainScreen].scale;

        float thisx = newPoint.x;
        float thisy = newPoint.y;
        thisx /= container->getScale();
        thisy /= container->getScale();

        thisx -= container->getTranslateX();
        thisy -= container->getTranslateY();*/
        float thisx = transformTouchPointX(newPoint.x);
        float thisy = transformTouchPointY(newPoint.y);

        // *******
        // TODO: landscape transform coordinates
        // *******

        Input* i = ARK2D::getInput();
        ARK2D::getLog()->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);
        ARK2D::getGame()->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);
        i->mouse_x = (int) thisx;
        i->mouse_y = (int) thisy;

        // Multi-touch
		for (UITouch* touch in touches)
		{
			// found a touch.  Is it already on our list?
			int fingerID = i->getTouchByInternalData(touch);
			if (fingerID != -1) {
				//found it
			} else {
				//wasn't on our list?!
				continue;
			}

            ARK2D::getLog()->v("touch moved");
			CGPoint pt = [touch locationInView:self];
			i->m_touchPointers[fingerID].x = transformTouchPointX(pt.x);
			i->m_touchPointers[fingerID].y = transformTouchPointY(pt.y);
			i->m_touchPointers[fingerID].data = touch;
		}
    }
}



- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    GameContainer* container = ARK2D::getContainer();
    if (container != NULL)
    {
        CGPoint newPoint = [[touches anyObject] locationInView:self];
        //newPoint.x *= [UIScreen mainScreen].scale;
        //newPoint.y *= [UIScreen mainScreen].scale;

        float thisx = transformTouchPointX(newPoint.x);
        float thisy = transformTouchPointY(newPoint.y);
        /*thisx /= container->getScale();
        thisy /= container->getScale();

        thisx -= container->getTranslateX();
        thisy -= container->getTranslateY();*/

        // *******
        // TODO: landscape transform coordinates
        // *******

        Input* i = ARK2D::getInput();
        i->mouse_x = (int) thisx;
        i->mouse_y = (int) thisy;
        i->releaseKey(Input::MOUSE_BUTTON_LEFT);

        // Multi-touch
		for (UITouch *touch in touches)
		{
			//found a touch.  Is it already on our list?
			int fingerID = i->getTouchByInternalData(touch);
			if (fingerID != -1) {
				i->removeTouch(fingerID);
                ARK2D::getLog()->v("remove touch point");
			} else {
				//wasn't on our list
				continue;
			}
		}
    }
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
	GameContainer* container = ARK2D::getContainer();
    if (container != NULL)
    {
     	Input* i = ARK2D::getInput();

     	// Multi-touch
		for (UITouch* touch in touches)
		{
			//found a touch.  Is it already on our list?
			int fingerID = i->getTouchByInternalData(touch);
			if (fingerID != -1) {
				i->removeTouch(fingerID);
			} else {
				//wasn't on our list
				continue;
			}
		}
	}
}

- (void)dealloc
{
    ARK::Core::GameContainerPlatform::getInstance()->end();
    [super dealloc];
}

@end










#import <GameKit/GameKit.h>


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
    // check for presence of GKLocalPlayer API
    Class gcClass = (NSClassFromString(@"GKLocalPlayer"));

    // check if the device is running iOS 4.1 or later
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);

    return (gcClass && osVersionSupported);
}


- (void) authenticateLocalUser
{
    if([GKLocalPlayer localPlayer].authenticated == NO)
    {
        [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error)
        {
            ARK::Util::Callbacks::invoke(ARK::Util::Callbacks::CALLBACK_GAMECENTER_SIGNIN_SUCCESSFUL);
            //[self callDelegateOnMainThread: @selector(processGameCenterAuth:) withArg: NULL error: error];
        }];
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

    UIViewController* cc = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.glViewController;
    [cc performSegueWithIdentifier:@"scoreChallenge" sender:scoreReporter];
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
            UIViewController* cc = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.glViewController;
            [cc performSegueWithIdentifier:@"achievementChallenge" sender:achievement];
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
       UIViewController* cc = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.glViewController;
       gameCenterController.gameCenterDelegate = (id<GKGameCenterControllerDelegate>) cc;
       [cc presentViewController: gameCenterController animated: YES completion:nil];
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

