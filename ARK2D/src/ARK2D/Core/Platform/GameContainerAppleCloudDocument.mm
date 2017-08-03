//
//  AppleCloudDocument.m
//  IPhone
//  macOS
//
//  Created by Ashley Gwinnell on 16/05/2012.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#if defined(ARK2D_MACINTOSH) || defined(ARK2D_IPHONE) 

#import "GameContainerAppleCloudDocument.h"
#import "../../Util/ICloudUtil.h"
#import "../../Core/ARK2D.h"
#import "../../Core/Log.h"
#import "../../Core/Util/StringUtil.h"


@implementation AppleCloudDocument

@synthesize mymeta;
@synthesize defaultCreationText = _defaultCreationText;
@synthesize documentText = _documentText;
@synthesize delegate = _delegate;
@synthesize initFileURL;

static std::function<void(AppleCloudDocument*, bool)> openSuccessfulHandler = nil;

+ (std::function<void(AppleCloudDocument*, bool)>) getOpenSuccessfulHandler {
    return openSuccessfulHandler;
}
+ (void) setOpenSuccessfulHandler:(std::function<void(AppleCloudDocument*, bool)>)value {
    openSuccessfulHandler = value;
}


#ifdef ARK2D_IPHONE
    // ** READING **

    - (BOOL)loadFromContents:(id)contents ofType:(NSString *)typeName error:(NSError **)outError
    {
        NSLog(@"UIDocument: iCloud loadFromContents: state = %lu, typeName=%@", (unsigned long)self.documentState, typeName);

        if (contents && [contents length] > 0) {
            self.documentText = [[NSString alloc] initWithBytes:[contents bytes] length:[contents length] encoding:NSUTF8StringEncoding];
        }
        else {
            self.documentText = self.defaultCreationText;
        }
     
        //NSLog(@"UIDocument: Loaded the following text from iCloud key '%s': %@", self.mymeta->filename.c_str(), self.documentText);
        //if (self.mymeta != NULL) {
        NSLog(@"UIDocument: Loaded from iCloud key '%@' sz='%i'", [self fileURL], [self.documentText length]);//self.mymeta->filename.c_str());
        //} 

        //if (self.documentText != nil) {

            //if (self.mymeta != NULL && self.mymeta->onchangefunction != NULL) {
            //    void (*pt)() = (void(*)()) self.mymeta->onchangefunction;
            //    pt();
            //}

        //}

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

        //NSLog(@"UIDocument: Will save the following text in the cloud: %@", self.documentText);
        AppleCloudDocument* t = self;
        ARK::Util::ICloudFile* icf = (t->mymeta != NULL)
            ? t->mymeta
        : NULL; //(t->_mymeta != NULL)
                //? t->_mymeta
                //: NULL;
        if (icf != NULL) {
            NSLog(@"UIDocument: Will save key '%s' in the iCloud.", icf->filename.c_str());
        } else {
            NSLog(@"UIDocument: Will save key 'unknown' in the iCloud.");
        }

        return [NSData dataWithBytes:[self.documentText UTF8String] length:[self.documentText length]];
    }


#elif defined(ARK2D_MACINTOSH)

    - (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError {
        
        NSLog(@"NSDocument: iCloud loadFromData: typeName=%@", typeName);
        if (data && [data length] > 0) {
            self.documentText = [[NSString alloc] initWithBytes:[data bytes] length:[data length] encoding:NSUTF8StringEncoding];
        }
        else {
            self.documentText = self.defaultCreationText;
        }
        NSLog(@"NSDocument: Loaded from iCloud key '%@' sz='%lu'.", [self fileURL], (unsigned long)[self.documentText length]);//self.mymeta->filename.c_str());
        
        AppleCloudDocument* t = self;
        openSuccessfulHandler(t, true);
        
        
        return YES;
    }

    - (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError {
        
        if ([self.documentText length] == 0) {
            self.documentText = self.defaultCreationText;
        }
        
        AppleCloudDocument* t = self;
        NSLog(@"NSDocument: Will save key '%s' in the iCloud.", t->mymeta->filename.c_str());
        
        return [NSData dataWithBytes:[self.documentText UTF8String] length:[self.documentText length]];
    }

#endif

-(void) moveToICloudSyncDir {
    dispatch_async(dispatch_get_global_queue (DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{

        AppleCloudDocument* t = self;

        string thisFilename = ICloudUtil::_getICloudTempDir() + t->mymeta->filename;

        NSString* filename_pure_nsstr = [NSString stringWithCString:t->mymeta->filename.c_str() encoding:[NSString defaultCStringEncoding]];
        NSString* filename_nsstr = [NSString stringWithCString:thisFilename.c_str() encoding:[NSString defaultCStringEncoding]];

        NSURL* ubiquitousURL = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];
        if (ubiquitousURL == NULL || ubiquitousURL == nil) {
            ARK2D::getLog()->e(StringUtil::append("Could not create icloud file. Maybe iCloud is switched off in settings. ", t->mymeta->filename));
            return;
        }

        NSURL* ubiquitousPackageDestinationURL = [[ubiquitousURL URLByAppendingPathComponent:@"Documents" ] URLByAppendingPathComponent:filename_pure_nsstr];
        NSURL* fileSourceURL = [[NSURL alloc] initFileURLWithPath:filename_nsstr];

                            
        NSError* error;
        bool b;

        // setUbiquitous fails if the file already exists. so delete it.
        // should it? hmmm...
        // 
        ARK2D::getLog()->i("Remove if exists");        
        NSFileManager* fileManager = [NSFileManager defaultManager];
        b = [fileManager removeItemAtPath:[ubiquitousPackageDestinationURL path] error:&error];
        if (b) {
            ARK2D::getLog()->i("success");
        } else {
            ARK2D::getLog()->i("fail");
            NSLog(@"%@", error);
        }

        ARK2D::getLog()->i("Moving to ubiquitous dir");
        NSLog(@"from %@ to %@.", fileSourceURL, ubiquitousPackageDestinationURL);
        b = [[NSFileManager defaultManager] setUbiquitous:YES
                    itemAtURL:fileSourceURL
                    destinationURL:ubiquitousPackageDestinationURL
                    error:&error];
        if (b) {
            ARK2D::getLog()->i("success");
        } else {
            ARK2D::getLog()->i("fail");
            NSLog(@"%@", error);
        }

        
    }); 
}

@end

#endif










