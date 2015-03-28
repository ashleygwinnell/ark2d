/*
 * Cast.h
 *
 *  Created on: 24-Apr-2010
 *      Author: Ashley
 */

#ifndef CAST_H_
#define CAST_H_

#include "../ARK2D.h"
#include "../UI/ErrorDialog.h"

using namespace std;

namespace ARK {
	namespace Util {

		/*!
		 * \brief Casting to and from Strings and Integers, because dynamic types and casting in standard C++ are bad.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class Cast {
			public:
				static string inttohexcolor(unsigned int i) {
					char buffer[3]; // eg. FF
					sprintf(buffer, "%X", i);
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

				//template <class T>
				//static string toString(bool b) {
				//	return (b) ? string("true") : string("false");
				//}
				static string boolToString(bool b) {
					return (b) ? string("true") : string("false");	
				}

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
