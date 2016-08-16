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

namespace ARK {
	namespace Util { 

		vector<ICloudFile>* ICloudUtil::s_iCloudFiles = new vector<ICloudFile>();
		signed int ICloudUtil::s_remainingFiles = 0; 

		void ICloudUtil::init() {
			ARK2D::getLog()->v("Initialise iCloud.");

			#if defined(ARK2D_IPHONE)
			    
			    NSURL* ubiq = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];
			    if (ubiq) {
			    	NSString* ubiqStr = [ubiq absoluteString];
					string thsStrText = [ubiqStr cStringUsingEncoding:[NSString defaultCStringEncoding]];
			        ARK2D::getLog()->v(StringUtil::append("iCloud access at ", thsStrText));
			        
			        GameContainerIPhoneAppDelegate* delegate = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate;
			        [delegate loadDocuments];
			    } else {
			        ARK2D::getLog()->v("No iCloud access");
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
 
				ARK2D::getLog()->v("iCloudUtil - push");

				GameContainerIPhoneAppDelegate* delegate = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate;
				NSMutableDictionary* docs = (NSMutableDictionary*) [delegate iclouddocuments];

				for(unsigned int i = 0; i < s_iCloudFiles->size(); i++) {

					ICloudFile* icf = &s_iCloudFiles->at(i);;

					// if is in iclouddocuments, check timestamp?
					bool found = false;
					for (NSString* key in docs) {
						
						string keycStr = string([key UTF8String]);
						
						string sss = string("Comparing ");
						sss += keycStr;
						sss += " to "; 
						sss += icf->filename;
						sss += "."; 
						ARK2D::getLog()->v(sss);


						if (keycStr == icf->filename) {
							ARK2D::getLog()->v(StringUtil::append("iCloud Util - already in cloud: ", icf->filename));
							found = true;

							IPhoneCloudDocument* doc = (IPhoneCloudDocument*) [docs objectForKey:key];
							//[doc documentText] = 
							[doc saveToURL: [doc fileURL] forSaveOperation:UIDocumentSaveForOverwriting  
								completionHandler:^(BOOL success) {
						      		NSLog(@"Attempt to save to URL %@", success ? @"succeeded" : @"failed");    
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

		void ICloudUtil::setFileContents(string fname, string contents) {
			#if defined(ARK2D_IPHONE) 
				GameContainerIPhoneAppDelegate* delegate = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate;
				NSMutableDictionary* docs = (NSMutableDictionary*) [delegate iclouddocuments];

				for (NSString* key in docs) {
					string keycStr = string([key UTF8String]);
					if (keycStr == fname) { 
					
						NSString* contents_nsstr = [NSString stringWithCString:contents.c_str() encoding:[NSString defaultCStringEncoding]];

						IPhoneCloudDocument* doc = (IPhoneCloudDocument*) [docs objectForKey:key];
						doc.documentText = contents_nsstr;// ;]; //documentText
					}
				}
			#endif

		} 

		string ICloudUtil::getFileContents(string fname) {
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
							string valuecStr = string([[value documentText] UTF8String]);
							return valuecStr;
						}
					}
				}

				ARK2D::getLog()->w(StringUtil::append("Could not file contents in iCloud for file: ", fname));
				return "";

			#else 
				return "";
			#endif	
		}

		void ICloudUtil::_createFile(ICloudFile* icf) {
			#if defined(ARK2D_IPHONE)

				ARK2D::getLog()->v(StringUtil::append("iCloud Util - _createFile: ", icf->filename));
				if (icf->createifnotexists) { 

					GameContainerIPhoneAppDelegate* delegate = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate;
					
	        		ARK2D::getLog()->v(StringUtil::append("make iCloud file: ", icf->filename));
		        	
	        		NSString* filename_nsstr = [NSString stringWithCString:icf->filename.c_str() encoding:[NSString defaultCStringEncoding]];
	        		NSString* defaultstr_nsstr = [NSString stringWithCString:icf->defaultcontents.c_str() encoding:[NSString defaultCStringEncoding]];

					NSURL* ubiq = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];
		        	NSURL* ubiquitousPackage = [[ubiq URLByAppendingPathComponent:@"Documents"] URLByAppendingPathComponent:filename_nsstr];

		        	IPhoneCloudDocument* doc = [[IPhoneCloudDocument alloc] initWithFileURL:ubiquitousPackage];
		        	doc.mymeta = icf;
		        	doc.defaultCreationText = defaultstr_nsstr;
		        	doc.delegate = [delegate glViewController];
		        	[doc saveToURL:[doc fileURL] forSaveOperation:UIDocumentSaveForCreating completionHandler:^(BOOL success) {
			            ARK2D::getLog()->v("ICloudUtil: new document save to iCloud");
			            [doc openWithCompletionHandler:^(BOOL success) {
			                ARK2D::getLog()->v("ICloudUtil: new document opened from iCloud");

			                // add to dictionary
		                    if (doc.mymeta->onchangefunction != NULL) {
		                        void (*pt)() = (void(*)()) doc.mymeta->onchangefunction;
		                        pt(); 
		                    }
			            }];
			        }]; 

		        	[[delegate iclouddocuments] setValue:doc forKey:filename_nsstr];

		        }

			#endif

		}

	}
}



