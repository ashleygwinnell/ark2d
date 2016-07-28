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

#if defined(ARK2D_IPHONE)
	#include "../Core/Platform/GameContainerIPhone.h"
//#else
//	class GameCenterManager;
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

				static void addFile(string fname, string defaultcontents, bool create);
				static void addFile(string fname, string defaultcontents, bool create, void* changefunction);
				//static void setFile(string fname, string contents);
				static void init();
				static void push();

				static void setFileContents(string fname, string contents);
				static string getFileContents(string fname);

				static void _createFile(ICloudFile* icf);

			public:
				static signed int s_remainingFiles;

		};

	}
}

#endif /* ICLOUD_UTIL_H_ */
