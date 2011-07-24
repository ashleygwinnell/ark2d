/*
 * FileUtil.cpp
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#include "FileUtil.h"

#include <stdio.h>
#ifdef ARK2D_WINDOWS
	#include <direct.h>
	#define GetCurrentDirectoryMacro _getcwd
	#define DIRECTORY_SEPARATOR "\\"
#else
	#include <unistd.h>
	#define GetCurrentDirectoryMacro getcwd
	#define DIRECTORY_SEPARATOR "/"
#endif

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

string FileUtil::getCurrentDirectory() {
	char currentPath[FILENAME_MAX];
	if(!GetCurrentDirectoryMacro(currentPath, sizeof(currentPath))) {
		return "";
	}
	currentPath[sizeof(currentPath)-1] = '\0';
	return string(currentPath);
}
string FileUtil::getSeparator() {
	return DIRECTORY_SEPARATOR;
}
