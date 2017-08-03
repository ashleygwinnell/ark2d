//
//  IPhoneViewController.m
//  IPhone
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//



#import "GameContainerIPhoneGLView.h"
#import "GameContainerIPhone.h"
#import "GameContainerAppleCloudDocument.h"


#include "../../Includes.h"
#include "../../Namespaces.h"
#include "../ARK2D.h"
#include "../GameContainer.h"
#include "../Graphics/Color.h"
#include "../../UI/Dialog.h"
#include "../Util/Callbacks.h"

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
        if (self.navigationController) {
            return self.navigationController;
        }
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
        //ErrorDialog::createAndShow("Low memory!");
        iOSListener::get()->didReceiveMemoryWarning();
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

        Input* in = ARK2D::getInput();
        in->mouse_x = (int) thisx;
        in->mouse_y = (int) thisy;
        in->pressKey(Input::MOUSE_BUTTON_LEFT);

        // Multi-touch
        for (UITouch* touch in touches)
        {
             //found a touch.  Is it already on our list?
			int fingerID = in->getTouchByInternalData(touch);

			if (fingerID == -1) {
				// add it to our list
				CGPoint pt = [touch locationInView:self];
				Pointer p;
				p.x = transformTouchPointX(pt.x);
				p.y = transformTouchPointY(pt.y);
				p.data = touch;
				fingerID = in->addTouch(p);

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

        Input* in = ARK2D::getInput();
        //ARK2D::getLog()->mouseMoved((int) thisx, (int) thisy, in->mouse_x, in->mouse_y);
        ARK2D::getGame()->mouseMoved((int) thisx, (int) thisy, in->mouse_x, in->mouse_y);
        in->mouse_prev_x = in->mouse_x;
        in->mouse_prev_y = in->mouse_y;
        in->mouse_x = (int) thisx;
        in->mouse_y = (int) thisy;

        // Multi-touch
		for (UITouch* touch in touches)
		{
			// found a touch.  Is it already on our list?
			int fingerID = in->getTouchByInternalData(touch);
			if (fingerID != -1) {
				//found it
			} else {
				//wasn't on our list?!
				continue;
			}

            ARK2D::getLog()->v("touch moved");
			CGPoint pt = [touch locationInView:self];
			in->m_touchPointers[fingerID].x = transformTouchPointX(pt.x);
			in->m_touchPointers[fingerID].y = transformTouchPointY(pt.y);
			in->m_touchPointers[fingerID].data = touch;
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

        Input* in = ARK2D::getInput();
        in->mouse_x = (int) thisx;
        in->mouse_y = (int) thisy;
        in->releaseKey(Input::MOUSE_BUTTON_LEFT);

        // Multi-touch
		for (UITouch *touch in touches)
		{
			//found a touch.  Is it already on our list?
			int fingerID = in->getTouchByInternalData(touch);
			if (fingerID != -1) {
				in->removeTouch(fingerID);
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








