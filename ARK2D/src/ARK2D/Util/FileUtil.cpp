/*
 * FileUtil.cpp
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#include "FileUtil.h"

#include "../Includes.h"

#include <stdio.h>
#include "../ARK2D.h"
#include "StringUtil.h"
#include "Log.h"
#include "../Core/GameContainer.h"
#include "../Core/String.h"
#include "Callbacks.h"

#if ( defined(ARK2D_WINDOWS_PHONE_8) )
 	#include <direct.h> 
	#include <windows.h>
	#define GetCurrentDirectoryMacro _getcwd
	#define DIRECTORY_SEPARATOR "\\"
#elif defined(ARK2D_WINDOWS) && defined(ARK2D_WINDOWS_VS)
	#include <direct.h>  
	#include <windows.h>
	#define GetCurrentDirectoryMacro _getcwd
	#define DIRECTORY_SEPARATOR "\\"
#else
	#include <unistd.h>
	#define GetCurrentDirectoryMacro getcwd
	#define DIRECTORY_SEPARATOR "/"
#endif

 

namespace ARK { 
	namespace Util {

		string FileUtil::getResourcePath() {
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

		string FileUtil::prependPlatform(string filename) {
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

		bool FileUtil::file_put_contents(string filename, string contents) 
		{
			return file_put_contents(filename, contents, true);
		}

		bool FileUtil::file_put_contents(string filename, string contents, bool doPrependPlatform) 
		{
			ARK2D::getLog()->i(StringUtil::append("Making file: ", filename));

			if (doPrependPlatform) { 
				filename = prependPlatform(filename);
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

		bool FileUtil::file_put_contents(string filename, const char* data, unsigned int len)
		{
			filename = prependPlatform(filename); 

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

		
		file_get_contents_binary_result FileUtil::file_get_contents_binary(string filename)
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

			/*char* fileUncompressed = (char*) malloc((int) fileStats.size);
			int fileUncompressedPointer = 0;
			unsigned int fileUncompressedBufferSize = 4096;
			if (fileUncompressedBufferSize > (int) fileStats.size) {
				fileUncompressedBufferSize = (int) fileStats.size;
			}
			for(signed int i = 0; i < (int) fileStats.size; i += fileUncompressedBufferSize) {
				char fileUncompressedBuffer[fileUncompressedBufferSize];
				signed int done = zip_fread(file, &fileUncompressedBuffer, fileUncompressedBufferSize);
				if (done == -1) { break; }
				memcpy(fileUncompressed + i, &fileUncompressedBuffer[0], done);
			}
			zip_fclose(file);*/

			/*RawDataReturns* rt = new RawDataReturns();
			rt->data = (void*) fileUncompressed;
			rt->size = (int) fileStats.size;
			return rt;*/

			//return StringUtil::file_get_contents(filename.c_str()).c_str();
		}

		file_get_contents_text_result FileUtil::file_get_contents_text(string filename)
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

		string FileUtil::getCurrentDirectory() {
			#if defined(ARK2D_WINDOWS_PHONE_8) || ( defined(ARK2D_WINDOWS) && defined(ARK2D_WINDOWS_VS) )
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
		string FileUtil::getSeparator() {
			return DIRECTORY_SEPARATOR;
		}

		string FileUtil::getOSDirectory() {
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

		void FileUtil::openBrowserToURL(string url_str) {
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

			#endif
		}
		void FileUtil::openGalleryToImageUrl(string url)  
		{
			ARK2D::getLog()->g("opening image");
			#if defined(ARK2D_ANDROID)
				ARK2D::getLog()->g("Android: opening gallery");

				string fname = FileUtil::prependPlatform(url);
				ARK2D::getLog()->g(StringUtil::append("filename:", fname));
				
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->openGalleryToImageUrl(fname);
				
			#elif defined(ARK2D_IPHONE)
				ARK2D::getLog()->g("iOS: opening gallery");
 
				string fname = FileUtil::prependPlatform(url);
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

 
				UIDocumentInteractionController* interactionController = [UIDocumentInteractionController interactionControllerWithURL: nsurl];
				[interactionController setDelegate:dele];  
				[interactionController presentPreviewAnimated:YES ]; 
 
 				[interactionController release];
				//[pool release];
 
			#elif defined(ARK2D_MACINTOSH) 
				ARK2D::getLog()->g("mac opening image");

				string fname = FileUtil::prependPlatform(url); 
				ARK2D::getLog()->g(StringUtil::append("filename:", fname));

				NSString* urlStr = [NSString stringWithCString:fname.c_str() encoding:NSUTF8StringEncoding];
				[[NSWorkspace sharedWorkspace] openFile:urlStr];
            	//[urlStr release];
			#endif
            ARK2D::getLog()->g("done");
		}

		void FileUtil::openGooglePlayStore(string packageName) {
			#if defined(ARK2D_ANDROID)
				ARK2D::getLog()->v("Opening Play Store...");

				ARK2D::getContainer()->m_platformSpecific.m_pluggable->openGooglePlayStore(packageName);

				ARK2D::getLog()->v("Done!");
			#endif
		}
 

		string FileUtil::getOSUsername() {
			#if defined(ARK2D_MACINTOSH)
				NSString *userName = NSUserName();
				NSString *fullUserName = NSFullUserName();

				string c_username = string([userName UTF8String]);
				string c_fullusername = string([fullUserName UTF8String]);

				return c_username;
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
				return "Linux User";
			#endif
			return "Unknown";
		}

	}
}
