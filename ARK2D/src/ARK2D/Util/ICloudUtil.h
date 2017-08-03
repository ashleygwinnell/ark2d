/*
 * ICloudUtil.h
 *
 *  Created on: 15 Nov 2013
 *      Author: Ashley
 */

#ifndef ICLOUD_UTIL_H_
#define ICLOUD_UTIL_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)
    #include "../Core/Platform/GameContainerAppleCloudDocument.h"
#else
    class AppleCloudDocument {};
#endif

namespace ARK {
	namespace Util {

		class ARK2D_API ICloudFile {
			public:
				string filename;
				string defaultcontents;
				bool createifnotexists;
				void* onchangefunction;
		};

		class ARK2D_API ICloudUtil {
			private:
				static vector<ICloudFile>* s_iCloudFiles;

			public:
				static vector<ICloudFile>* getFiles() { return s_iCloudFiles; }
				static unsigned int countFiles();

				static void addFile(string fname, string defaultcontents, bool create);
				static void addFile(string fname, string defaultcontents, bool create, void* changefunction);
				//static void setFile(string fname, string contents);
				static void init();
				static void push();
				static void purge();
				
				static void setFileContents(string fname, string contents);
				static void setFileContentsAndPushAsync(string fname, string contents);
				static string getFileContents(string fname);
				static uint64_t getFileModifiedTime(string fname);

				static void _createFile(ICloudFile* icf);
				static bool _fileExists(string fname);

				static void _setFileContentsInternal(string fname, string contents);
				static void _pushInternal();

				static string _getICloudTempDir();

			private: 
				static AppleCloudDocument* _findFile(string filename);
				static bool _isEnabledInUserDefaults();

			public:
				static signed int s_remainingFiles;

		};

	}
}

#endif /* ICLOUD_UTIL_H_ */
