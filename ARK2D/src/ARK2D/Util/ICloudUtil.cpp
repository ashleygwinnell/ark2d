/*
 * ICloudUtil.cpp
 *
 *  Created on: 15 Nov 2013
 *      Author: Ashley
 */

#include "ICloudUtil.h"

#include "../Core/ARK2D.h"
//#include "../Includes.h"
#include "../Core/GameContainer.h"

#if defined(ARK2D_IPHONE)
	#include "../Core/Platform/GameContainerIPhone.h"
#endif
#include "../Core/Util/SystemUtil.h"

namespace ARK {
	namespace Util { 

		vector<ICloudFile>* ICloudUtil::s_iCloudFiles = new vector<ICloudFile>();
		signed int ICloudUtil::s_remainingFiles = 0; 

		unsigned int ICloudUtil::countFiles() { 
			return s_iCloudFiles->size(); 
		}

		void ICloudUtil::init() {
            if (!_isEnabledInUserDefaults()) {
                ARK2D::getLog()->i("iCloud disabled in Settings.");
                return;
            }
			ARK2D::getLog()->i("Initialise iCloud.");

			#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)
			    
			    NSURL* ubiq = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];
			    if (ubiq) {
			    	NSString* ubiqStr = [ubiq absoluteString];
					string thsStrText = [ubiqStr cStringUsingEncoding:[NSString defaultCStringEncoding]];
			        ARK2D::getLog()->i(StringUtil::append("iCloud access at ", thsStrText));
			        
			        GameICloudManager* icloud = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.icloud;
			        if (icloud == NULL) {
			        	ARK2D::getLog()->w("GameICloudManager instance was null.");
			        	return;
			        }
			        [icloud loadDocuments];
			        
			    } else {
			        ARK2D::getLog()->w("No iCloud access");
			    }
			#endif
		}


		void ICloudUtil::addFile(string fname, string defaultcontents, bool create) {
			addFile(fname, defaultcontents, create, NULL);	
		}
		void ICloudUtil::addFile(string fname, string defaultcontents, bool create, void* changefunction) {
			ARK2D::getLog()->v(StringUtil::append("Add iCloud file: ", fname)); 
			ICloudFile f;
			f.filename = fname;
			f.defaultcontents = defaultcontents;
			f.createifnotexists = create;
			f.onchangefunction = changefunction;
			s_iCloudFiles->push_back(f);
		}
		
		void ICloudUtil::purge() {
			#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)

				if (!_isEnabledInUserDefaults()) {
                    ARK2D::getLog()->i("iCloud (purge) disabled in Settings.");
                    return;
                }
		

		

				ARK2D::getLog()->i("iCloudUtil - purge");
				GameICloudManager* icloud = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.icloud;
				
				[icloud->query disableUpdates];
				//[icloud->query stopQuery];
				
				NSURL* ubiquitousURL = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];
				if (ubiquitousURL == NULL || ubiquitousURL == nil) {
					ARK2D::getLog()->e("Could not purge icloud files. Maybe iCloud is switched off in settings.");
					return;
				}

				NSFileManager* fileManager = [NSFileManager defaultManager];
                NSError* error;

				for(id theKey in icloud->iclouddocuments) {
					AppleCloudDocument* doc = icloud->iclouddocuments[theKey];
					ARK2D::getLog()->i(StringUtil::append("Deleting iCloud file: ", doc->mymeta->filename));  

					NSString* filename_pure_nsstr = [NSString stringWithCString:doc->mymeta->filename.c_str() encoding:[NSString defaultCStringEncoding]];
					NSURL* ubiquitousPackageDestinationURL = [[ubiquitousURL URLByAppendingPathComponent:@"Documents" ] URLByAppendingPathComponent:filename_pure_nsstr];

					bool b = [fileManager removeItemAtPath:[ubiquitousPackageDestinationURL path] error:&error];
					if (b) {
						ARK2D::getLog()->i("success");
					} else {
						ARK2D::getLog()->i("fail");
						NSLog(@"%@", error);
					}
				}
				[icloud->iclouddocuments removeAllObjects];

				[icloud->query enableUpdates];

	        #endif
		}
		
		void ICloudUtil::push() {
			#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)
                
                ARK2D::getLog()->i("iCloudUtil - push start");

				dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
					ARK2D::getLog()->i("iCloudUtil - push async start");
					_pushInternal();
				});

			#endif
		}
		void ICloudUtil::_pushInternal() {
			#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)
				if (!_isEnabledInUserDefaults()) {
                    ARK2D::getLog()->i("iCloud (push) disabled in Settings.");
                    return;
                }

                GameICloudManager* icloud = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.icloud;
				NSMutableDictionary* docs = (NSMutableDictionary*) [icloud iclouddocuments];

				for(unsigned int i = 0; i < s_iCloudFiles->size(); i++) {


					ICloudFile* icf = &s_iCloudFiles->at(i);;

					ARK2D::getLog()->i(StringUtil::append("Trying to push ", icf->filename));

					// if is in iclouddocuments, check timestamp?
					bool found = false;
					for (NSString* key in docs) {
						
						string keycStr = string([key UTF8String]);
						
						string sss = string("Comparing ");
						sss += keycStr;
						sss += " to "; 
						sss += icf->filename;
						sss += "."; 
						//ARK2D::getLog()->i(sss);


						if (keycStr == icf->filename) {
							ARK2D::getLog()->i(StringUtil::append("iCloudUtil - already in cloud (try overwrite): ", icf->filename));
							found = true;


							string thisFilename = _getICloudTempDir() + icf->filename;
		        			NSString* filename_nsstr = [NSString stringWithCString:thisFilename.c_str() encoding:[NSString defaultCStringEncoding]];
		        			NSURL* fileSourceURL = [[NSURL alloc] initFileURLWithPath:filename_nsstr];

							AppleCloudDocument* doc = (AppleCloudDocument*) [docs objectForKey:key];
                            [doc saveToURL: fileSourceURL
                                #ifdef ARK2D_IPHONE
                                    forSaveOperation:UIDocumentSaveForOverwriting
                                    completionHandler:^(BOOL success) 
                                {
                                #elif defined(ARK2D_MACINTOSH)
                                    ofType: NSPlainTextDocumentType
                                    forSaveOperation: NSSaveOperation //NSAutosaveInPlaceOperation
                                    completionHandler: ^(NSError* errorOnNil) 
                                {
                                        BOOL success = errorOnNil == nil;
                                #endif

                                    string cfileurl = string([[fileSourceURL path]UTF8String]);
                                    if (!success) {
					        			ARK2D::getLog()->e(StringUtil::append("ICloudUtil: Could not save (overwrite) document to iCloud local folder ", cfileurl));
					        			#if defined(ARK2D_MACINTOSH)
					        				NSLog(@"Error:%@.", errorOnNil);
					        			#endif
					        			return;
					        		}
						      		//NSLog(@"Attempt to save (overwrite) to key %@ URL (local folder) %@ %@", key, fileSourceURL, success ? @"succeeded" : @"failed");

						      		ARK2D::getLog()->i(StringUtil::append("ICloudUtil: document successfully saved (overitten) to iCloud local folder ", cfileurl));

						      		[doc moveToICloudSyncDir];
						    	}
						    ];   

						}

					}

					if (!found) {
						// push to icloud server.
						_createFile(icf);
					}

				}
			#endif
		}
		bool ICloudUtil::_fileExists(string fname) {
			#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)
				GameICloudManager* icloud = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.icloud;
				NSMutableDictionary* docs = (NSMutableDictionary*) [icloud iclouddocuments];

				for (NSString* key in docs) {
					string keycStr = string([key UTF8String]);
					if (keycStr == fname) { 
						return true;
					}
				}
			#endif

			return false;
		}

		AppleCloudDocument* ICloudUtil::_findFile(string fname) {
			#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)
				GameICloudManager* icloud = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.icloud;
				NSMutableDictionary* docs = (NSMutableDictionary*) [icloud iclouddocuments];

				for(unsigned int i = 0; i < s_iCloudFiles->size(); i++) {
					ICloudFile* icf = &s_iCloudFiles->at(i);

					if (icf->filename != fname) { continue; }

					for (NSString* key in docs) {
						
						string keycStr = string([key UTF8String]);
						if (keycStr == fname) {
							AppleCloudDocument* value = (AppleCloudDocument*) [docs objectForKey:key];
							return value;
						}
					}
				}

				ARK2D::getLog()->w(StringUtil::append("Could not file in iCloud: ", fname));
			#endif

			return NULL;
		}

		void ICloudUtil::setFileContents(string fname, string contents) {
			#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)
				_setFileContentsInternal(fname, contents);
			#endif
		} 
		void ICloudUtil::setFileContentsAndPushAsync(string fname, string contents) {
			#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)
				
				dispatch_async(dispatch_get_global_queue (DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{

					_setFileContentsInternal(fname, contents);
					_pushInternal();

				});

			#endif
		}
		void ICloudUtil::_setFileContentsInternal(string fname, string contents) {
			#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)
                if (!_isEnabledInUserDefaults()) {
                    ARK2D::getLog()->i("iCloud (setFileContents) disabled in Settings.");
                    return;
                }
            
				AppleCloudDocument* value = _findFile(fname);
				if (value != NULL) {
					ARK2D::getLog()->i(StringUtil::append("iCloudUtil - updating file contents: ", fname));

					NSString* contents_nsstr = [NSString stringWithCString:contents.c_str() encoding:[NSString defaultCStringEncoding]];
					value.documentText = contents_nsstr;
				}
				else {
					ARK2D::getLog()->e(StringUtil::append("Could not iCloud::setFileContents as key was not found in iCloud documents: ", fname));
				}
			#endif
		}

	

		string ICloudUtil::getFileContents(string fname) {
			#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)
				AppleCloudDocument* value = _findFile(fname);
				if (value != NULL) {
					string valuecStr = string([[value documentText] UTF8String]);
					return valuecStr;
				}

				ARK2D::getLog()->w(StringUtil::append("Could not file contents in iCloud for file: ", fname));
			#endif	
			return "";
		}
		
		uint64_t ICloudUtil::getFileModifiedTime(string fname) {
			#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)

				AppleCloudDocument* value = _findFile(fname);
				if (value != NULL) {
					return [[value fileModificationDate] timeIntervalSince1970];
				}

				ARK2D::getLog()->w(StringUtil::append("Could not file modified time in iCloud for file: ", fname));
			#endif

			return 0;
		}

		string ICloudUtil::_getICloudTempDir() {
			string icloudTempDir;
			#if defined(ARK2D_IPHONE)
				icloudTempDir = Resource::__iPhoneInternal_get_local_resource_path() + string("iCloud-temp/");
				SystemUtil::createDirectory(icloudTempDir);
			#elif defined(ARK2D_MACINTOSH)
				bool sandboxed = ARK2D::getContainer()->getPlatformSpecific()->isSandboxed();
				if (sandboxed) {
				    icloudTempDir = ARK2D::getContainer()->getPlatformSpecific()->m_resourcePathSandbox;
				} else {
				    icloudTempDir = ARK2D::getContainer()->getPlatformSpecific()->m_resourcePath;
				}
				icloudTempDir += string("iCloud-temp/");
				SystemUtil::createDirectory(icloudTempDir);
			#endif
				return icloudTempDir;
		}

		void ICloudUtil::_createFile(ICloudFile* icf) {
			#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)

				ARK2D::getLog()->i(StringUtil::append("iCloudUtil - _createFile: ", icf->filename));
				
				if (icf->createifnotexists) { 

					if (_fileExists(icf->filename)) {
						ARK2D::getLog()->i(StringUtil::append("iCloudUtil - file exists in cloud ", icf->filename));
						return; 
					} 
					ARK2D::getLog()->i(StringUtil::append("iCloudUtil - make file true ", icf->filename));


                    GameICloudManager* icloud = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.icloud;
					
					string thisFilename = _getICloudTempDir() + icf->filename;
	        		
	        		NSString* filename_pure_nsstr = [NSString stringWithCString:icf->filename.c_str() encoding:[NSString defaultCStringEncoding]];
	        		NSString* filename_nsstr = [NSString stringWithCString:thisFilename.c_str() encoding:[NSString defaultCStringEncoding]];
	        		NSString* defaultstr_nsstr = [NSString stringWithCString:icf->defaultcontents.c_str() encoding:[NSString defaultCStringEncoding]];

					NSURL* ubiquitousURL = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];
					if (ubiquitousURL == NULL || ubiquitousURL == nil) {
						ARK2D::getLog()->e(StringUtil::append("Could not create icloud file. Maybe iCloud is switched off in settings. ", icf->filename));
						return;
					}

					//NSURL* ubiquitousPackageFolderURL = [ubiquitousURL URLByAppendingPathComponent:@"Documents" ];
					//SystemUtil::createDirectory(string([[ubiquitousPackageFolderURL path]UTF8String]));

		        	NSURL* ubiquitousPackageDestinationURL = [[ubiquitousURL URLByAppendingPathComponent:@"Documents" ] URLByAppendingPathComponent:filename_pure_nsstr];
		        	NSURL* fileSourceURL = [[NSURL alloc] initFileURLWithPath:filename_nsstr];

		        	#if defined(ARK2D_IPHONE)
                        AppleCloudDocument* doc = [[AppleCloudDocument alloc] initWithFileURL:fileSourceURL];
                    #elif defined(ARK2D_MACINTOSH)
                        NSError* outError;
                        AppleCloudDocument* doc = [[AppleCloudDocument alloc] initWithContentsOfURL:fileSourceURL ofType:NSRTFTextDocumentType error:&outError];
                    #endif
		        	doc.mymeta = icf;
		        	doc.defaultCreationText = defaultstr_nsstr;
                    
                    #if defined(ARK2D_IPHONE)
                        doc.delegate = [ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate glViewController];
                    #elif defined(ARK2D_MACINTOSH)
                        //doc.delegate = [ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate];
                    #endif
                    
                    
                    
		        	[doc saveToURL:[doc fileURL]
                        #if defined(ARK2D_IPHONE)
                            forSaveOperation:UIDocumentSaveForCreating
                            completionHandler:^(BOOL success) {
                        #elif defined(ARK2D_MACINTOSH)
                            ofType: NSPlainTextDocumentType
                            forSaveOperation: NSAutosaveInPlaceOperation
                            completionHandler: ^(NSError* errorOnNil) {
                            BOOL success = errorOnNil == nil;
                        #endif

		        		string cfileurl = string([[[doc fileURL] path]UTF8String]);
		        		if (!success) {
		        			ARK2D::getLog()->e(StringUtil::append("ICloudUtil: Could not save document to iCloud local folder ", cfileurl));
		        			return;
		        		}

		        		
			            ARK2D::getLog()->i(StringUtil::append("ICloudUtil: new document successfully saved to iCloud local folder ", cfileurl));

						
						// [doc openWithCompletionHandler:^(BOOL success) {
						//     ARK2D::getLog()->i("ICloudUtil: new document opened from iCloud");

						//     // add to dictionary
						//        if (doc.mymeta->onchangefunction != NULL) {
						//            void (*pt)() = (void(*)()) doc.mymeta->onchangefunction;
						//            pt(); 
						//        }
						// }];

						[doc moveToICloudSyncDir];
						

			        }];

			      
                    
		        	[[icloud iclouddocuments] setValue:doc forKey:filename_nsstr];

		        }

			#endif

		}

		bool ICloudUtil::_isEnabledInUserDefaults() {
			#if defined(ARK2D_IPHONE)
				bool enabled = !![[NSUserDefaults standardUserDefaults] boolForKey:@"setting_icloud"];
                return enabled;
			#elif defined(ARK2D_MACINTOSH)
			    return true;
			#endif
			return false;
		}

	}
}



