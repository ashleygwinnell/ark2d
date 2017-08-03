/*
 * SystemUtil.cpp
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#include "SystemUtil.h"

//#include "../Includes.h"

#include <stdio.h>
#include "../ARK2D.h"
#include "StringUtil.h"
#include "../Log.h"
#include "../GameContainer.h"
#include "../String.h"
#include "../Util/Callbacks.h"

#if defined(ARK2D_WINDOWS_PHONE_8)
 	#include <direct.h>
	#include <windows.h>
	#define GetCurrentDirectoryMacro _getcwd
	#define DIRECTORY_SEPARATOR "\\"
#elif defined(ARK2D_XBOXONE) || defined(ARK2D_WINDOWS_STORE)
 	#include <direct.h>
	#include <windows.h>
	#define GetCurrentDirectoryMacro GetCurrentDirectory
	#define DIRECTORY_SEPARATOR "\\"
#elif defined(ARK2D_WINDOWS) && defined(ARK2D_WINDOWS_VS)
	#include <direct.h>
	#include <windows.h>
	#include <Shlobj.h>
	#define GetCurrentDirectoryMacro _getcwd
	#define DIRECTORY_SEPARATOR "\\"
#else
	#include <unistd.h>
    #include <sys/stat.h>
	#define GetCurrentDirectoryMacro getcwd
	#define DIRECTORY_SEPARATOR "/"
#endif

#ifdef ARK2D_EMSCRIPTEN_JS
//#include <emscripten/bind.h>
#include <emscripten.h>

#endif



namespace ARK {
	namespace Core {
		namespace Utils {

			string SystemUtil::getResourcePath() {
				#if defined(ARK2D_ANDROID)
					return ARK2D::getContainer()->m_platformSpecific.m_externalDataStr;
				#elif defined(ARK2D_IPHONE)
					// append Documents folder first.
					NSArray* searchPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
					NSString* documentsPath = [searchPaths objectAtIndex:0];
					string respath( [ documentsPath UTF8String ] );

					return respath + string("/");
				#elif defined(ARK2D_FLASCC)
					return string("/local");
				#endif
				return ARK2D::getContainer()->getResourcePath();
			}

			string SystemUtil::prependPlatform(string filename) {
				#if defined(ARK2D_ANDROID)
					filename = ARK2D::getContainer()->m_platformSpecific.m_externalDataStr + filename;
				#elif defined(ARK2D_IPHONE)
					// append Documents folder first.
					NSArray* searchPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
					NSString* documentsPath = [searchPaths objectAtIndex:0];
					string respath( [ documentsPath UTF8String ] );

					filename = respath + string("/") + filename;
				#else
					if (filename.substr(1,1).compare(":") == 0 || filename.substr(0,1).compare("/") == 0) {

					} else {
						filename = ARK2D::getContainer()->getResourcePath() + filename;
					}
				#endif

				#if defined(ARK2D_FLASCC)
					filename = string("/local") + filename;
				#endif
				return filename;
			}

			bool SystemUtil::file_put_contents(string filename, string contents)
			{
				return file_put_contents(filename, contents, true);
			}

			bool SystemUtil::file_put_contents(string filename, string contents, bool doPrependPlatform)
			{
				ARK2D::getLog()->i(StringUtil::append("Making file: ", filename));

				if (doPrependPlatform) {
					#ifdef ARK2D_MACINTOSH
						if (ARK2D::getContainer()->getPlatformSpecific()->isSandboxed()) {
							filename = ARK2D::getContainer()->getPlatformSpecific()->m_resourcePathSandbox + filename;
							createDirectoryForFile(filename);
						} else {
							filename = prependPlatform(filename);
						}
					#else 
						filename = prependPlatform(filename);
					#endif
				}

				ARK2D::getLog()->i(StringUtil::append("Making file: ", filename));

				// yarp
				fstream File;
				File.open(filename.c_str(), ios::out);
				if (File.is_open()) {
					File << contents;
					File.close();
					return true;
				}
				ARK2D::getLog()->e(StringUtil::append("Making file failed. :( ", filename));
				return false;
			}

			bool SystemUtil::file_put_contents(string filename, const char* data, unsigned int len)
			{
				#ifdef ARK2D_MACINTOSH
					if (ARK2D::getContainer()->getPlatformSpecific()->isSandboxed()) {
						filename = ARK2D::getContainer()->getPlatformSpecific()->m_resourcePathSandbox + filename;
						createDirectoryForFile(filename);
					} else {
						filename = prependPlatform(filename);
					}
				#else 
					filename = prependPlatform(filename);
				#endif
				

				ARK2D::getLog()->i(StringUtil::append("Making file: ", filename));

				FILE* fp = NULL;
				#if defined( ARK2D_WINDOWS_PHONE_8 )
					fopen_s(&fp, filename.c_str(), "wb");
				#else
					fp = fopen(filename.c_str(), "wb");
				#endif

	 			fwrite(data, sizeof(char), len, fp);
	 			fclose(fp);

				//ARK2D::getLog()->e(StringUtil::append("Making file failed. :( ", filename));
				return true;
			}


			file_get_contents_binary_result SystemUtil::file_get_contents_binary(string filename)
			{
				// Prepend shit for each OS.
				filename = StringUtil::internalOSAppends(filename);

				//ARK2D::getLog()->v("pre fopen");
				FILE* file = NULL;// fopen(filename.c_str(), "rt");
				#if defined( ARK2D_WINDOWS_PHONE_8 )
					fopen_s(&file, filename.c_str(), "rb");
				#else
					file = fopen(filename.c_str(), "rb");
				#endif

				if (file == NULL) {
					string str = "Could not open file ["; str += filename; str += "] as it does not exist.";
					ARK2D::getLog()->e(str);
					file_get_contents_binary_result r;
					r.data = NULL;
					r.len = 0;
					return r;
				}

				//ARK2D::getLog()->v("pre seeking");
				fseek(file, 0, SEEK_END);
				int count = ftell(file);
				int allcount = count;

				//ARK2D::getLog()->v("pre rewind");
				rewind(file);

				// ***********************
				//! @todo: memory management: memory leak. yay.
				// *************************
				char* data = NULL; //new char[count];

				//ARK2D::getLog()->v("pre count/malloc");
				if (count > 0) {
					data = (char*) malloc(sizeof(char) * (count));
					count = fread(data, sizeof(char), count, file);
				}

				//ARK2D::getLog()->v("pre fclose");
				fclose(file);

				file_get_contents_binary_result r;
				r.data = data;
				r.len = allcount;

				//ARK2D::getLog()->v("pre return");
				return r;

			}

			// TODO: We don't know if this converts from UTF-16BE or UTF-16LE or anything.
			// Generally, always use UTF-8 file types.
			file_get_contents_binary_result SystemUtil::file_get_contents_utf8binary(string filename)
			{
				// Prepend shit for each OS.
				filename = StringUtil::internalOSAppends(filename);

				//ARK2D::getLog()->v("pre fopen");
				FILE* file = NULL;// fopen(filename.c_str(), "rt");
				#if defined( ARK2D_WINDOWS_PHONE_8 )
					fopen_s(&file, filename.c_str(), "rt, ccs=UTF-8");
				#else
					file = fopen(filename.c_str(), "rt, ccs=UTF-8");
				#endif

				if (file == NULL) {
					string str = "Could not open file ["; str += filename; str += "] as it does not exist.";
					ARK2D::getLog()->e(str);
					file_get_contents_binary_result r;
					r.data = NULL;
					r.len = 0;
					return r;
				}

				//ARK2D::getLog()->v("pre seeking");
				fseek(file, 0, SEEK_END);
				int count = ftell(file);
				int allcount = count;

				//ARK2D::getLog()->v("pre rewind");
				rewind(file);

				// ***********************
				//! @todo: memory management: memory leak. yay.
				// *************************
				char* data = NULL; //new char[count];

				//ARK2D::getLog()->v("pre count/malloc");
				if (count > 0) {
					data = (char*) malloc(sizeof(char) * (count));
					count = fread(data, sizeof(char), count, file);
				}

				//ARK2D::getLog()->v("pre fclose");
				fclose(file);

				file_get_contents_binary_result r;
				r.data = data;
				r.len = allcount;

				//ARK2D::getLog()->v("pre return");
				return r;

			}

			file_get_contents_text_result SystemUtil::file_get_contents_text(string filename)
			{
				string contents = StringUtil::file_get_contents(filename.c_str());

				char* newData = (char*) malloc((int) contents.size()+1);
				#ifdef ARK2D_WINDOWS_PHONE_8
					strcpy_s(newData, contents.size() + 1, contents.c_str());
				#else
					strcpy(newData, contents.c_str());
				#endif

				//newData[(int) contents.size()] = '\0';

				file_get_contents_text_result rt;
				rt.data = (char*) newData;
				rt.len = (int) contents.length();
				return rt;

			}
			uint64_t SystemUtil::file_get_modifiedtime(string passedFilename) {
				string filename = StringUtil::internalOSAppends(passedFilename);

				file_get_contents_binary_result r;
				
				#ifdef ARK2D_IPHONE
					if (Resource::__iPhoneInternal_should_try_local_resource(filename) && 
						Resource::__iPhoneInternal_local_resource_exists(filename)) {
						filename = Resource::__iPhoneInternal_get_local_resource_path(filename);
					} 
				#elif defined(ARK2D_MACINTOSH)
					if (ARK2D::getContainer()->getPlatformSpecific()->isSandboxed()) {
						if (!Resource::exists(passedFilename, false)) {
							return 0;
						}
						filename = ARK2D::getContainer()->getPlatformSpecific()->m_resourcePathSandbox + passedFilename;
					} 
				#endif

				
				struct stat sb;
				stat(filename.c_str(), &sb);
				return sb.st_mtime;
				/*
                ARK2D::getLog()->e(StringUtil::append("ctimespec: ", Cast::toString<unsigned long>(sb.st_ctimespec.tv_sec)));
				ARK2D::getLog()->e(StringUtil::append("mtimespec: ", Cast::toString<unsigned long>(sb.st_mtimespec.tv_sec)));
				ARK2D::getLog()->e(StringUtil::append("atimespec: ", Cast::toString<unsigned long>(sb.st_atimespec.tv_sec)));
                
                ARK2D::getLog()->e(StringUtil::append("ctime: ", Cast::toString<unsigned long>(sb.st_ctime)));
                ARK2D::getLog()->e(StringUtil::append("mtime: ", Cast::toString<unsigned long>(sb.st_mtime)));
                ARK2D::getLog()->e(StringUtil::append("atime: ", Cast::toString<unsigned long>(sb.st_atime)));
                
				ARK2D::getLog()->e(StringUtil::append("size: ", Cast::toString<unsigned long>(sb.st_size)));
				//return (sb.st_mtimespec.tv_sec * 1000000000 + sb.st_mtimespec.tv_nsec) / 1000;
                //return sb.st_mtimespec.tv_sec + sb.st_mtimespec.tv_nsec;
                //return [[NSDate dateWithTimeIntervalSinceReferenceDate:sb.st_mtime] timeIntervalSince1970] + sb.st_mtime;
                //return [[NSDate dateWithTimeIntervalSinceReferenceDate:sb.st_mtime] timeIntervalSince1970];

                
                NSString* filePath = [NSString stringWithCString:filename.c_str() encoding:[NSString defaultCStringEncoding]];
                NSLog(@"filePath %@", filePath);
                
                uint64_t modifiedtime = 0;
                
                NSFileManager* fileManager = [NSFileManager defaultManager];

                NSDate *creationDate = nil;
                if ([fileManager fileExistsAtPath:filePath]) {
                    NSDictionary* attributes = [fileManager attributesOfItemAtPath:filePath error:nil];
                    
                    NSDate* createdDate = attributes[NSFileCreationDate];
                    NSDate* modifiedDate = attributes[NSFileModificationDate];
                    NSTimeInterval seconds = [modifiedDate timeIntervalSince1970];
                    modifiedtime = seconds;
                } else {
                    NSLog(@"does not exist");
                }
                
                
                return modifiedtime;*/
			}

			string SystemUtil::getCurrentDirectory() {
				#if defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_XBOXONE) || ( defined(ARK2D_WINDOWS) && defined(ARK2D_WINDOWS_VS) ) || defined(ARK2D_WINDOWS_STORE)
					return "";
				#else
					char currentPath[FILENAME_MAX];
					if(!GetCurrentDirectoryMacro(currentPath, sizeof(currentPath))) {
						return "";
					}
					currentPath[sizeof(currentPath)-1] = '\0';
					return string(currentPath);
				#endif
				return "";
			}
			string SystemUtil::getSeparator() {
				return DIRECTORY_SEPARATOR;
			}

			string SystemUtil::getOSDirectory() {
				#if defined(ARK2D_WINDOWS)
					char dir[MAX_PATH];
					dir[MAX_PATH-1] = '\0';
					GetWindowsDirectory((LPSTR) &dir, MAX_PATH);

					// never add a backslash.
					// if windows is installed to C:\ then it returns C:
					string ret(dir);
					if (ret.substr(ret.size()-1, 1) == DIRECTORY_SEPARATOR) {
						ret = ret.substr(0, ret.size()-1);
					}
					return ret;

				#elif defined(ARK2D_UBUNTU_LINUX)

				#endif
				return "whoops?";
			}
			bool SystemUtil::createDirectoryForFile(string file) {
				// Extract directory data from file.
				string newfile = file.substr(0, file.rfind("/"));
				return createDirectory(newfile);
			}
			bool SystemUtil::createDirectory(string directoryPath) {
				#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)

					NSString* filePathAndDirectory = [NSString stringWithCString:directoryPath.c_str() encoding:[NSString defaultCStringEncoding]];
					NSError* error;

					if (![[NSFileManager defaultManager] createDirectoryAtPath:filePathAndDirectory
							withIntermediateDirectories:YES
							attributes:nil
							error:&error]) {
						//NSLog(@"Create directory error: %@", error);
						return false;
					}

					return true;
				#endif
                return false;
			}


			void SystemUtil::openBrowserToURL(string url_str) {
				#if defined(ARK2D_WINDOWS)
					ShellExecute(ARK2D::getContainer()->m_platformSpecific.m_hWindow, "open", url_str.c_str(), NULL, NULL, SW_SHOWDEFAULT);
				#elif defined(ARK2D_IPHONE)

					NSString* urlNSString = [NSString stringWithCString:url_str.c_str() encoding:[NSString defaultCStringEncoding]];
					NSURL* url = [NSURL URLWithString:urlNSString];

					if (![[UIApplication sharedApplication] openURL:url]) {
	  					ErrorDialog::createAndShow(StringUtil::append("Unable to open URL: ", url_str));
	  				}

	  			#elif defined(ARK2D_WINDOWS_PHONE_8)

					wchar_t* wcstring = Cast::charToWideChar(url_str.c_str());

					Platform::String^ s = ref new Platform::String(wcstring);
	    			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(s);
	    			Windows::System::Launcher::LaunchUriAsync(uri);

					delete wcstring;


				#elif defined(ARK2D_ANDROID)

					//return;

					// the game activity gets reloaded on resume and it breaks. can't enable this until that is fixed.
					ARK2D::getContainer()->m_platformSpecific.m_pluggable->openBrowserToUrl(url_str);

				#elif defined(ARK2D_MACINTOSH)
					CFURLRef url = CFURLCreateWithBytes (
						NULL,                        // allocator
						(UInt8*)url_str.c_str(),     // URLBytes
						url_str.length(),            // length
						kCFStringEncodingASCII,      // encoding
						NULL                         // baseURL
					);
					LSOpenCFURLRef(url, 0);
					CFRelease(url);
				#elif defined(ARK2D_FLASCC)

					inline_as3(
						"import com.adobe.flascc.Console;\n"\
						"Console.s_console.goToUrl(CModule.readString(%0, %1));\n"
						: : "r"(url_str.c_str()), "r"(url_str.length())
					);

				#elif defined(ARK2D_UBUNTU_LINUX)



					if (ARK2D::isSteam()) {
						Callbacks::invoke(Callbacks::CALLBACK_STEAM_OPENURLINBROWSER_LINUX, url_str);
					} else {

						string cmdstr = "xdg-open \"";
						cmdstr += url_str;
						cmdstr += "\"";
						ARK2D::getLog()->i(cmdstr);
						int ret = system(cmdstr.c_str());

						//#ifndef ARK2D_SDL2
						ARK2D::getContainer()->getPlatformSpecific()->setFocus(false);
						//#endif
					}
				#elif defined(ARK2D_EMSCRIPTEN_JS)
					// using namespace emscripten;

					 string s = string("window.open(\"");
					 s += url_str;
					 s += string("\")");

					 emscripten_run_script(s.c_str());
					

				#endif
			}
			void SystemUtil::openGalleryToImageUrl(string url)
			{
				ARK2D::getLog()->g("opening image");
				#if defined(ARK2D_ANDROID)
					ARK2D::getLog()->g("Android: opening gallery");

					string fname = SystemUtil::prependPlatform(url);
					ARK2D::getLog()->g(StringUtil::append("filename:", fname));

					ARK2D::getContainer()->m_platformSpecific.m_pluggable->openGalleryToImageUrl(fname);

				#elif defined(ARK2D_IPHONE)
					ARK2D::getLog()->g("iOS: opening gallery");

					string fname = SystemUtil::prependPlatform(url);
					//string fname = string("data/") + url;
					ARK2D::getLog()->v(StringUtil::append("file: ", fname));

					//NSString* urlNSString = [NSString stringWithCString:fname.c_str() encoding:[NSString defaultCStringEncoding]];
					//ARK2D::getLog()->v(StringUtil::append("file: ", fname));

					//NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

					NSString* urlNSString = [NSString stringWithCString:fname.c_str() encoding:[NSString defaultCStringEncoding]];
					NSURL* nsurl = [NSURL fileURLWithPath:urlNSString];
					//if (nsurl == nil) {
					//	ARK2D::getLog()->e("NSURL was nil");
					//	return;
					//}

					//[[NSBundle mainBundle] resourcePath]

	//				UIViewController* dele = ARK2D::getContainer()->m_platformSpecific.m_appDelegate.glViewController;

					GameContainerIPhoneGLViewController* dele = ARK2D::getContainer()->m_platformSpecific.m_appDelegate.glViewController;
					//GameContainerIPhoneGLViewController* dele2 = [dele copy];

                [dele previewDocumentWithURL:nsurl];

					/*NSLog(@"%@",dele.navigationController);

					UIDocumentInteractionController* interactionController = [UIDocumentInteractionController interactionControllerWithURL: nsurl];
					[interactionController setDelegate:dele];
					[interactionController presentPreviewAnimated:YES ];

	 				[interactionController release];*/
					//[pool release];

				#elif defined(ARK2D_MACINTOSH)
					ARK2D::getLog()->g("mac opening image");

					string fname = SystemUtil::prependPlatform(url);
					ARK2D::getLog()->g(StringUtil::append("filename:", fname));

					NSString* urlStr = [NSString stringWithCString:fname.c_str() encoding:NSUTF8StringEncoding];
					[[NSWorkspace sharedWorkspace] openFile:urlStr];
	            	//[urlStr release];
				#endif
	            ARK2D::getLog()->g("done");
			}

			void SystemUtil::openGooglePlayStore(string packageName) {
				#if defined(ARK2D_ANDROID)
					ARK2D::getLog()->v("Opening Play Store...");

					ARK2D::getContainer()->m_platformSpecific.m_pluggable->openGooglePlayStore(packageName);

					ARK2D::getLog()->v("Done!");
				#endif
			}
			bool SystemUtil::openSoftwareKeyboard() {
				#if defined(ARK2D_WINDOWS)

					// https://software.intel.com/en-us/blogs/2013/06/11/touch-keyboard-access-for-windows-8-desktop-apps
					// From Intel example.
					// The recommended way to invoke the Win8 touch keyboard is to derive an editable control from
					// the TextAutomationPeer class in .NET and allow the OSK to automatically show/hide the keyboard
					// as focus goes in/out of the control.
					//
					// Assuming the above recommendation isn't feasible, it is possible to force show the
					// touch keyboard by launching the executable via ShellExecute().

					// https://msdn.microsoft.com/en-us/library/bb762204(VS.85).aspx
					// TODO: don't hardcore C:/
					TCHAR pf[MAX_PATH];
					SHGetSpecialFolderPath(0, pf, CSIDL_PROGRAM_FILES, FALSE );
					string programFilesPath(pf);
					ARK2D::getLog()->e(programFilesPath);

					// This is (the default) full path to the touch keyboard for Win7 / Win8
					string cmdLine("C:\\Program Files\\Common Files\\Microsoft Shared\\ink\\tabtip.exe");
					HINSTANCE result = ShellExecute( NULL,
					                                 NULL,
					                                 cmdLine.c_str(),
					                                 NULL,
					                                 NULL,
					                                 SW_SHOW );
					// result is not a true HINSTANCE, just an error code.  result > 32 is success.
					return ((int) result > 32);


				#elif defined(ARK2D_ANDROID)
					GameContainer* container = ARK2D::getContainer();
					container->getPlatformSpecific()->m_pluggable->openSoftwareKeyboard();
					return true;
				#endif
				ARK2D::getLog()->e("openSoftwareKeyboard not implemented on this platform.");
				return false;
			}


			string SystemUtil::getOSUsername() {
				#if defined(ARK2D_MACINTOSH)
					NSString *userName = NSUserName();
					NSString *fullUserName = NSFullUserName();

					string c_username = string([userName UTF8String]);
					string c_fullusername = string([fullUserName UTF8String]);

					return c_username;
				#elif defined(ARK2D_WINDOWS_STORE)
					return "Unknown";
				#elif defined(ARK2D_WINDOWS)
					unsigned int namesz = 255;
					unsigned char name[255];
					if (GetUserName((LPSTR) &name[0], (LPDWORD)&namesz) != 0) {
						return string((const char*) name);
					}
					DWORD w = GetLastError();
					//ARK2D::getLog()->e(StringUtil::append("could not get os username: ", (unsigned int)w));
					ErrorDialog::createAndShow(StringUtil::append("could not get os username: ", (unsigned int)w));
					// TODO: GetLastError();
					return "Unknown";
				#elif defined(ARK2D_UBUNTU_LINUX)
					// TODO: linux support for this function.
					char u[256];
					int v = getlogin_r(u, 255);
					if (v != 0) {
			            return "Linux User";
			        }
			        return string((const char*) u);
			    #elif defined(ARK2D_IPHONE)
			        NSString* name = [[UIDevice currentDevice] name];
	                string c_username = string([name UTF8String]);
	                return c_username;
				#endif
				return "Unknown";
			}
		}
	}
}
