//
//  GameContainerIPhoneAppDelegate.h
//  IPhone
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


//#import "GameContainerIPhoneGLView.h"

//#import <UIKit/UIKit.h>

#if defined(ARK2D_MACINTOSH) || defined(ARK2D_IPHONE) 

	#import <Foundation/Foundation.h>

	#if defined(ARK2D_MACINTOSH)
	    #import <Cocoa/Cocoa.h>
	#endif
	#include <functional>

	namespace ARK { 
		namespace Util {
			class ICloudFile;
		}
	}
	#ifdef ARK2D_IPHONE
	@interface AppleCloudDocument : UIDocument {
	#elif defined(ARK2D_MACINTOSH)
	@interface AppleCloudDocument : NSDocument {
		//NSAttributedString* mString;
	#endif
	    @public ARK::Util::ICloudFile* mymeta;
	    NSString* documentText;
	    NSURL* initFileURL;
	    id delegate;
	}
	#if defined(ARK2D_MACINTOSH)
	//@property (nonatomic) NSAttributedString* mString;
	#endif


	@property (nonatomic, retain) NSString* defaultCreationText;
	@property (nonatomic) ARK::Util::ICloudFile* mymeta;
	@property (nonatomic, retain) NSString* documentText;
	@property (nonatomic, retain) NSURL* initFileURL;
	@property (nonatomic, assign) id delegate;

	- (void) moveToICloudSyncDir;
	    
	+ (std::function<void(AppleCloudDocument*, bool)>) getOpenSuccessfulHandler;
	+ (void) setOpenSuccessfulHandler:(std::function<void(AppleCloudDocument*, bool)>)value;

	@end

#endif