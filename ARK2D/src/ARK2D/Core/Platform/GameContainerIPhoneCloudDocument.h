//
//  GameContainerIPhoneAppDelegate.h
//  IPhone
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


//#import "GameContainerIPhoneGLView.h"

//#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
namespace ARK { 
	namespace Util {
		class ICloudFile;
	}
}

@interface IPhoneCloudDocument : UIDocument {
	ARK::Util::ICloudFile* mymeta;
    NSString* documentText;
    id delegate;

}

@property (nonatomic, retain) NSString* defaultCreationText;
@property (nonatomic) ARK::Util::ICloudFile* mymeta;
@property (nonatomic, retain) NSString* documentText;
@property (nonatomic, assign) id delegate;

@end





