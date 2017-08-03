//
//  IPhoneViewController.h
//  IPhone
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

//#import <UIKit/UIKit.h>

#include "../../Includes.h"
#import "GameContainerAppleCloudDocument.h"
/*
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import <QuartzCore/QuartzCore.h>*/
//#import <Foundation/Foundation.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "GameContainerAppleShared.h"


@interface InputDialogViewController : UIViewController <UIAlertViewDelegate>
	@property unsigned int m_callbackId;
    - (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;

@end
                

@interface GameContainerIPhoneGLViewController : UIViewController <UIDocumentInteractionControllerDelegate, GameCenterManagerDelegate, GKGameCenterControllerDelegate> {
//@interface GameContainerIPhoneGLViewController : GAITrackedViewController <UIDocumentInteractionControllerDelegate, GameCenterManagerDelegate, GKGameCenterControllerDelegate> {
    GameCenterManager* gameCenterManager;
	AppleCloudDocument* document;
    
    
    
}
    @property (nonatomic, retain) GameCenterManager* gameCenterManager;
    @property (strong, nonatomic) AppleCloudDocument* document;
    
    - (GameCenterManager*) getGameCenter; 
	- (void)loadView; 
    - (void)viewDidLoad;
    - (void)dealloc;
    // - (void)viewDidAppear:(BOOL)animated;
    - (void)previewDocumentWithURL:(NSURL*)url;


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
