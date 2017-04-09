/*
 * Cast.h
 *
 *  Created on: 24-Apr-2010
 *      Author: Ashley
 */

#ifndef ARK_CORE_CAST_H_
#define ARK_CORE_CAST_H_

#include <string>
#include <sstream>
#include <cstring>
#include <stdio.h>
#include <iostream>
using namespace std;
#ifndef ARK2D_WINDOWS
#define sprintf_s sprintf
#endif

namespace ARK {
	namespace Core {

		/*!
		 * \brief Casting to and from Strings and Integers, because dynamic types and casting in standard C++ are bad.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class Cast {
			public:
				static string inttohexcolor(unsigned int i) {
					char buffer[3]; // eg. FF
					sprintf_s(buffer, "%X", i);
					return string(buffer);
				}
				static unsigned int hextoint(char* hexChars) {
					unsigned int x;
					stringstream ss;
					ss << std::hex << hexChars;
					ss >> x;
					return x;
				}

				template<class T>
				static string toString(const T& t)
				{
					 ostringstream stream;
					 stream << t;
					 return stream.str();
				}

                // template<class T>
                // static wstring toWideString(const T& t)
                // {

                //     wstringstream stream;
                //     stream << t;
                //     return stream.str();
                // }

				//template <class T>
				//static string toString(bool b) {
				//	return (b) ? string("true") : string("false");
				//}
				static string boolToString(bool b) {
					return (b) ? string("true") : string("false");
				}
				// static wstring boolToWideString(bool b) {
				// 	return (b) ? wstring(L"true") : wstring(L"false");
				// }

				template<class T>
				static T fromString(const string& s)
				{
					istringstream stream (s);
					T t;
					stream >> t;
					return t;
				};
				static bool boolFromString(string s) {
					return (s == "true")?true:false;
				}
				static bool boolFromString(wstring s) {
					return (s == L"true")?true:false;
				}
				static bool boolFromString(const char* str) {
					return (strcmp(str, "true") == 0)?true:false;
				}

				// must manually call delete on the new string.
				static wchar_t* charToWideChar(const char* orig) {
					#ifdef ARK2D_WINDOWS_VS
						size_t newsize = strlen(orig) + 1;
						wchar_t * wcstring = new wchar_t[newsize];
						size_t convertedChars = 0;
						mbstowcs_s(&convertedChars, wcstring, newsize, orig, _TRUNCATE);
						return wcstring;
					#endif
					//ARK2D::getLog()->e("Cast::charToWideChar not implemented on this platform.");
					return NULL;
				}

		};
	}
}

#endif /* CAST_H_ */
