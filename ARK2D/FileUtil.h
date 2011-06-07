
/*
 * FileUtil.h
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#ifndef FILEUTIL_H_
#define FILEUTIL_H_

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class FileUtil {
	public:
		static bool file_put_contents(string filename, string contents);
};

#endif /* FILEUTIL_H_ */
