//
//  GameContainerIPhoneAppDelegate.h
//  IPhone
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//



#include "../../Includes.h"
#import "GameContainerIPhoneGLView.h"

//#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

//GameContainerIPhoneGLView* glView;
@interface GameContainerIPhoneAppDelegate : NSObject <UIApplicationDelegate>

@property (nonatomic, retain) IBOutlet UIWindow* window;
@property (nonatomic, retain) IBOutlet GameContainerIPhoneGLView* glView;

@end
