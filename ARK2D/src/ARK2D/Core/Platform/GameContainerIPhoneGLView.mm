//
//  IPhoneViewController.m
//  IPhone
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//



#import "GameContainerIPhoneGLView.h"
#import "GameContainerIPhone.h"

#include "../../Includes.h"
#include "../../Namespaces.h"
#include "../../ARK2D.h"
#include "../../Core/GameContainer.h"

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
    p->initOpenGL2D(self.bounds.size.width, self.bounds.size.height);
    p->initOpenAL();
    p->initGame();
    //p->start();
    
    CADisplayLink* aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:(id)self selector:@selector(updateAndRender)];
    [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    p->_displayLink = aDisplayLink;


    return self;
}
- (void) updateAndRender
{
    ARK::Core::GameContainerPlatform::getInstance()->updateAndRender();
}




- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    GameContainer* container = ARK2D::getContainer();
    if (container != NULL) 
    {
        //NSUInteger touchCount = [touches count];
        //NSUInteger tapCount = [[touches anyObject] tapCount];

       // CGPoint lastPoint = [touch previousLocationInView:self];
        CGPoint newPoint = [[touches anyObject] locationInView:self];

        float thisx = newPoint.x;
        float thisy = newPoint.y;
        //float thisx = (float) x;
        //float thisy = (float) y;
        thisx /= container->getScale();
        thisy /= container->getScale();

        thisx -= container->getTranslateX();
        thisy -= container->getTranslateY();

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
    }
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    GameContainer* container = ARK2D::getContainer();
    if (container != NULL) 
    {
        CGPoint newPoint = [[touches anyObject] locationInView:self];

        float thisx = newPoint.x;
        float thisy = newPoint.y;
        thisx /= container->getScale();
        thisy /= container->getScale();

        thisx -= container->getTranslateX();
        thisy -= container->getTranslateY();

        // *******
        // TODO: landscape transform coordinates
        // *******

        Input* i = ARK2D::getInput();
        ARK2D::getGame()->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);
        i->mouse_x = (int) thisx;
        i->mouse_y = (int) thisy;
    }
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    GameContainer* container = ARK2D::getContainer();
    if (container != NULL) 
    {
        CGPoint newPoint = [[touches anyObject] locationInView:self];

        float thisx = newPoint.x;
        float thisy = newPoint.y;
        thisx /= container->getScale();
        thisy /= container->getScale();

        thisx -= container->getTranslateX();
        thisy -= container->getTranslateY();

        // *******
        // TODO: landscape transform coordinates
        // *******

        Input* i = ARK2D::getInput();
        i->mouse_x = (int) thisx;
        i->mouse_y = (int) thisy;
        i->releaseKey(Input::MOUSE_BUTTON_LEFT);
    }
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {

}

- (void)dealloc
{
    ARK::Core::GameContainerPlatform::getInstance()->end();
    [super dealloc];
}

@end
 
