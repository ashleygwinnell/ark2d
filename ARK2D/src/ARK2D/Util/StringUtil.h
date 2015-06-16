/*
 * StringUtil.h
 *
 *  Created on: 13-Dec-2009
 *      Author: Ashley
 */

#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_

#include "../ARK2D.h"
#ifndef ARK2D_ANDROID
	#include <vector> 
	#include <string>
	#include <fstream>
	#include <iostream>
	#include <sstream>
	#include <stdio.h>
	#include <algorithm>
#endif

#include "../UI/ErrorDialog.h"
#include "../Core/ToString.h"
#include "../Namespaces.h"
#include "../Includes.h"
#include "../Graphics/SpriteSheetDescription.h"
#include "Cast.h"

using namespace std;

namespace ARK {
	namespace Util {

		/*!
		 * \brief String utility functions that affect primitive string types.
		 *
		 * @todo Plug these in to the ARK::String container for object-based use.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API StringUtil {
			friend class ARK::Graphics::SpriteSheetDescription;
			public:

				static string prepend(string str, int i);
				static string append(string str, int i);
				static string appendf(string str, float i);
				static string append(string str, string str2); 
				static string pathToFile(string file);

				static const string base64_chars;
				static bool is_base64(unsigned char c);
				static string encodeBase64(unsigned char const* , unsigned int len);
				static string decodeBase64(string const& s);

				static bool file_exists(const char * filename);
				static string file_get_contents(const char* fileName);

				static void toUpper(string& str);
				static void toLower(string& str);

				static string urlencode(const string& s);
				static string char2hex( char dec );

				static string zeropad(int number, int digits);

				static int utf8strlen(const char* s);

				static void trimLeft(string& str, const char* chars2remove);
				static void trimRight(string& str, const char* chars2remove);
				static void trim(string& str, const char* chars2remove);
				static string trimret(string str, string charsToRemove);
				static void trimAll(vector<ARK::Core::String>& strings, std::string chars2remove);

				static bool str_contains(const string& haystack, const string& needle);
				static string& str_replace(const string& search, const string& replace, string& subject);
				static string str_replace_copy(string search, string replace, string subject);

				static vector<string> split(string s, const char* delimiter);
				static vector<string> getLinesForWordWrap(string str, int maxWidth);
				static string getExtension(string s);

				static string caesarShift(const string& s, int amount);

				static string getOrdinal(int i);
				static string ordinalise(int i) { return getOrdinal(i); }

				static string getMinuteSecondsFormatFromSeconds(float seconds);
				static string getMinuteSecondsLongFormatFromSeconds(float seconds);

				static string restrictToAlphanumericsSpaces(string str);
				static string restrictToCharset(string str, vector<char> chars);

				// Dont use wstrings.
				static wstring stringToWstring(const string& str);
				static string wstringToString(const wstring& str);

				template <class T> 
				static string getDenomination(T value) {
					int inserted = 0; 
		    		string ret = Cast::toString<T>(value);
		    		for(unsigned int i = 0; i < ret.length(); ++i) { 
		    			if (i > 0 && (i+1) % 3 == inserted && (i+1) < ret.length()) {
		    				ret.insert(ret.length() - (i+1), ",");
		    				inserted++;
		    				if (inserted > 3) { inserted = 0; }
		    				i++;  
		    			}
		    		} 
		    		return ret; 
				}

			public: 
				static string internalOSAppends(string strFilename);
		};
	}
}

#endif /* STRINGUTIL_H_ */
