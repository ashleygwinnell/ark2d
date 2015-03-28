//
//  IPhoneCloudDocument.m
//  IPhone
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//



#import "GameContainerIPhoneCloudDocument.h"
#import "../../Util/ICloudUtil.h"


@implementation IPhoneCloudDocument

@synthesize mymeta = _mymeta;
@synthesize defaultCreationText = _defaultCreationText;
@synthesize documentText = _documentText;
@synthesize delegate = _delegate;

// ** READING **

- (BOOL)loadFromContents:(id)contents ofType:(NSString *)typeName error:(NSError **)outError
{
    NSLog(@"UIDocument: loadFromContents: state = %d, typeName=%@", self.documentState, typeName);

    if ([contents length] > 0) {
        self.documentText = [[NSString alloc] initWithBytes:[contents bytes] length:[contents length] encoding:NSUTF8StringEncoding];
    }
    else {
        self.documentText = self.defaultCreationText;
    }

    NSLog(@"UIDocument: Loaded the following text from the cloud: %@", self.documentText);

    if (self.documentText != nil) { 

        // if (self.mymeta->onchangefunction != NULL) {
        //     void (*pt)() = (void(*)()) self.mymeta->onchangefunction;
        //     pt();
        // }

    }

    // update textView in delegate...
    //if ([_delegate respondsToSelector:@selector(noteDocumentContentsUpdated:)]) {
    //    [_delegate noteDocumentContentsUpdated:self];
    //}

    return YES;

}

// ** WRITING **

-(id)contentsForType:(NSString *)typeName error:(NSError **)outError
{
    if ([self.documentText length] == 0) {
        self.documentText = self.defaultCreationText;
    }

    NSLog(@"UIDocument: Will save the following text in the cloud: %@", self.documentText);

    return [NSData dataWithBytes:[self.documentText UTF8String] length:[self.documentText length]];
}
@end














