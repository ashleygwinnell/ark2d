/*
 * FileUtil.cpp
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#include "FileUtil.h"

bool FileUtil::file_put_contents(string filename, string contents) {
	// yarp
	fstream File;
	File.open(filename.c_str(), ios::out);
	if (File.is_open()) {
		File << contents;
		File.close();
		return true;
	}
	return false;
}
