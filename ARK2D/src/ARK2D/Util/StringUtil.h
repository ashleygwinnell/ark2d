/*
 * StringUtil.h
 *
 *  Created on: 13-Dec-2009
 *      Author: Ashley
 */

#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <algorithm>

#include "../UI/ErrorDialog.h"
#include "../ToString.h"

class ARKString;

using namespace std;

class StringUtil {
	public:

		static string prepend(string str, int i);
		static string append(string str, int i);
		static string append(string str, string str2);
		static string pathToFile(string file);

		static const string base64_chars;
		static bool is_base64(unsigned char c);
		static string encodeBase64(unsigned char const* , unsigned int len);
		static string decodeBase64(string const& s);

		static bool file_exists(const char * filename);
		static char* file_get_contents(const char* fileName);

		static void toUpper(string& str);
		static void toLower(string& str);

		static void trimLeft(string& str, const char* chars2remove);
		static void trimRight(string& str, const char* chars2remove);
		static void trim(string& str, const char* chars2remove);
		static string trimret(string str, string charsToRemove);
		static void trimAll(vector<ARKString>& strings, string chars2remove);

		static string& str_replace(const string& search, const string& replace, string& subject);

		static vector<string> split(string s, const char* delimiter);
		static string getExtension(string s);
};

#endif /* STRINGUTIL_H_ */
