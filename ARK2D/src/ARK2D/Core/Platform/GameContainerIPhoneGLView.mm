//
//  IPhoneViewController.m
//  IPhone
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "GameContainerIPhoneGLView.h"
#import "GameContainerIPhone.h"

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

    GameContainerPlatform* p = ARK::Core::GameContainerPlatform::getInstance();
    p->initOpenGL(self);
    p->initOpenAL();
    p->initGame();
    p->start();

    return self;
}

- (void)dealloc
{
    ARK::Core::GameContainerPlatform::getInstance()->end();
    [super dealloc];
}

@end
 
