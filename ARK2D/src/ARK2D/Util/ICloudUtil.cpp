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
    #include "../Core/Util/SystemUtil.h"
#endif

namespace ARK {
	namespace Util { 

		vector<ICloudFile>* ICloudUtil::s_iCloudFiles = new vector<ICloudFile>();
		signed int ICloudUtil::s_remainingFiles = 0; 

		void ICloudUtil::init() {
			ARK2D::getLog()->i("Initialise iCloud.");

			#if defined(ARK2D_IPHONE)
			    
			    NSURL* ubiq = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];
			    if (ubiq) {
			    	NSString* ubiqStr = [ubiq absoluteString];
					string thsStrText = [ubiqStr cStringUsingEncoding:[NSString defaultCStringEncoding]];
			        ARK2D::getLog()->i(StringUtil::append("iCloud access at ", thsStrText));
			        
			        GameContainerIPhoneAppDelegate* delegate = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate;
			        [delegate loadDocuments];
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
		
		
		void ICloudUtil::push() {
			#if defined(ARK2D_IPHONE)

				ARK2D::getLog()->i("iCloudUtil - push start");

				dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
					ARK2D::getLog()->i("iCloudUtil - push async start");
					_pushInternal();
				});

			#endif
		}
		void ICloudUtil::_pushInternal() {
			#if defined(ARK2D_IPHONE) 
				GameContainerIPhoneAppDelegate* delegate = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate;
				NSMutableDictionary* docs = (NSMutableDictionary*) [delegate iclouddocuments];

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
							ARK2D::getLog()->i(StringUtil::append("iCloudUtil - already in cloud: ", icf->filename));
							found = true;

							IPhoneCloudDocument* doc = (IPhoneCloudDocument*) [docs objectForKey:key];
							//[doc documentText] = 
							[doc saveToURL: [doc fileURL] forSaveOperation:UIDocumentSaveForOverwriting  
								completionHandler:^(BOOL success) {
						      		NSLog(@"Attempt to save (overwrite) to key %@ URL %@ %@", key, [doc fileURL], success ? @"succeeded" : @"failed");
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
			#if defined(ARK2D_IPHONE) 
				GameContainerIPhoneAppDelegate* delegate = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate;
				NSMutableDictionary* docs = (NSMutableDictionary*) [delegate iclouddocuments];

				for (NSString* key in docs) {
					string keycStr = string([key UTF8String]);
					if (keycStr == fname) { 
						return true;
					}
				}
			#endif

			return false;
		}

		IPhoneCloudDocument* ICloudUtil::_findFile(string fname) {
			#if defined(ARK2D_IPHONE) 
				GameContainerIPhoneAppDelegate* delegate = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate;
				NSMutableDictionary* docs = (NSMutableDictionary*) [delegate iclouddocuments];

				for(unsigned int i = 0; i < s_iCloudFiles->size(); i++) {
					ICloudFile* icf = &s_iCloudFiles->at(i);

					if (icf->filename != fname) { continue; }

					for (NSString* key in docs) {
						
						string keycStr = string([key UTF8String]);
						if (keycStr == fname) {
							IPhoneCloudDocument* value = (IPhoneCloudDocument*) [docs objectForKey:key];
							return value;
						}
					}
				}

				ARK2D::getLog()->w(StringUtil::append("Could not file in iCloud: ", fname));
			#endif

			return NULL;
		}

		void ICloudUtil::setFileContents(string fname, string contents) {
			#if defined(ARK2D_IPHONE) 
				_setFileContentsInternal(fname, contents);
			#endif
		} 
		void ICloudUtil::setFileContentsAndPushAsync(string fname, string contents) {
			#if defined(ARK2D_IPHONE) 
				
				dispatch_async(dispatch_get_global_queue (DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{

					_setFileContentsInternal(fname, contents);
					_pushInternal();

				});

			#endif
		}
		void ICloudUtil::_setFileContentsInternal(string fname, string contents) {
			#if defined(ARK2D_IPHONE)
				IPhoneCloudDocument* value = _findFile(fname); 
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
			#if defined(ARK2D_IPHONE) 
				IPhoneCloudDocument* value = _findFile(fname);
				if (value != NULL) {
					string valuecStr = string([[value documentText] UTF8String]);
					return valuecStr;
				}

				ARK2D::getLog()->w(StringUtil::append("Could not file contents in iCloud for file: ", fname));
			#endif	
			return "";
		}
		
		uint64_t ICloudUtil::getFileModifiedTime(string fname) {
			#if defined(ARK2D_IPHONE) 

				IPhoneCloudDocument* value = _findFile(fname);
				if (value != NULL) {
					return [[value fileModificationDate] timeIntervalSince1970];
				}

				ARK2D::getLog()->w(StringUtil::append("Could not file modified time in iCloud for file: ", fname));
			#endif

			return 0;
		}

		void ICloudUtil::_createFile(ICloudFile* icf) {
			#if defined(ARK2D_IPHONE)

				ARK2D::getLog()->i(StringUtil::append("iCloudUtil - _createFile: ", icf->filename));
				
				if (icf->createifnotexists) { 

					if (_fileExists(icf->filename)) {
						ARK2D::getLog()->i(StringUtil::append("iCloudUtil - file exists in cloud ", icf->filename));
						return; 
					} 
					ARK2D::getLog()->i(StringUtil::append("iCloudUtil - make file true ", icf->filename));


					GameContainerIPhoneAppDelegate* delegate = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate;
					
	        		//ARK2D::getLog()->v(StringUtil::append("make iCloud file: ", icf->filename));

	        		string icloudTempDir = Resource::__iPhoneInternal_get_local_resource_path() + string("iCloud-temp/");
	        		SystemUtil::createDirectory(icloudTempDir);




	        		string thisFilename = StringUtil::internalOSAppends(icf->filename);
	        		
	        		thisFilename = icloudTempDir + icf->filename;
	        		//thisFilename = ARK2D::getContainer()->getResourcePath() + string("iCloud-temp/") + icf->filename;
					
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


		        	IPhoneCloudDocument* doc = [[IPhoneCloudDocument alloc] initWithFileURL:fileSourceURL];
		        	doc.mymeta = icf;
		        	doc.defaultCreationText = defaultstr_nsstr;
		        	doc.delegate = [delegate glViewController];
		        	[doc saveToURL:[doc fileURL] forSaveOperation:UIDocumentSaveForCreating completionHandler:^(BOOL success) {

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

						dispatch_async(dispatch_get_global_queue (DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
               				
               				NSError * error;

							ARK2D::getLog()->i("Moving to ubiquitous dir");
							NSLog(@"from %@ to %@.", fileSourceURL, ubiquitousPackageDestinationURL);
							bool b = [[NSFileManager defaultManager] setUbiquitous:YES
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
						

			        }];

			      

		        	[[delegate iclouddocuments] setValue:doc forKey:filename_nsstr];

		        }

			#endif

		}

	}
}



