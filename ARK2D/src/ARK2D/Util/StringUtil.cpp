/*
 * StringUtil.cpp
 *
 *  Created on: 13-Dec-2009
 *      Author: Ashley
 */

#include "StringUtil.h"
//#include "../Core/ToString.h"
#include "../Core/String.h"
#include "../Core/GameContainer.h"
#include "../Util/Log.h"
//#include "../UI/ErrorDialog.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <algorithm>

namespace ARK {
	namespace Util {



		int digitsInNumber(int number);
		int digitsInNumber(int number) {
			// todo: this, properly.
			if (number >= 1000000) {
				return 7;
			} else if (number >= 100000) {
				return 6;
			} else if (number >= 10000) {
				return 5;
			} else if (number >= 1000) {
				return 4;
			} else if (number >= 100) {
				return 3;
			} else if (number >= 10) {
				return 2;
			}
			return 1;
		}
		std::string StringUtil::zeropad(int number, int digits) {
			// how many digits in this number.
			int numdigits = digitsInNumber(number);

			// diff between that and digits.
			int diff = digits - numdigits;

			// padd if appropriate.
			string s = Cast::toString<int>(number);
			while (diff > 0) {
				s = "0" + s;
				diff--;
			}

			// return string
			return s;
		}

		string StringUtil::getMinuteSecondsFormatFromSeconds(float totaltime) {
			int minutes = floor(totaltime/60.0f);
			int seconds = int(totaltime) % 60;

			string scoreStr = "";
			scoreStr += Cast::toString<int>(minutes);
			scoreStr += string(":");
			scoreStr += StringUtil::zeropad(seconds, 2);
			return scoreStr;
		}

		string StringUtil::getMinuteSecondsLongFormatFromSeconds(float totaltime) {
			int minutes = floor(totaltime/60.0f);
			int seconds = int(totaltime) % 60;

			string scoreStr = "";
			scoreStr += Cast::toString<int>(minutes);
			scoreStr += string(" minutes ");
			scoreStr += StringUtil::zeropad(seconds, 2);
			scoreStr += string(" seconds");
			return scoreStr;
		}

		std::string StringUtil::append(string str, int i) {
			std::ostringstream os;
			os << i;
			string newString = os.str();
			str += newString;
			return str;
		}std::string StringUtil::appendf(string str, float i) {
			std::ostringstream os;
			os << i;
			string newString = os.str();
			str += newString;
			return str;
		}
		string StringUtil::append(string str, string str2) {
			return str.append(str2);
		}
		string StringUtil::prepend(string str, int i) {
			std::ostringstream os;
			os << i;
			string newString = os.str();
			str = newString + str;
			return str;
		}

		bool StringUtil::str_contains(const string& haystack, const string& needle) {
			return (haystack.find(needle) != string::npos);
		}

		string& StringUtil::str_replace(const string& search, const string& replace, string& subject) {
			string buffer;

			int sealeng = search.length();
			int strleng = subject.length();

			if (sealeng==0)
				return subject;//no change

			for(int i=0, j=0; i<strleng; j=0 )
			{
				while (i+j<strleng && j<sealeng && subject[i+j]==search[j])
					j++;
				if (j==sealeng)//found 'search'
				{
					buffer.append(replace);
					i+=sealeng;
				}
				else
				{
					buffer.append( &subject[i++], 1);
				}
			}
			subject = buffer;
			return subject;
		}
		string StringUtil::str_replace_copy(string search, string replace, string subject) {
			string newString = subject;
			str_replace(search, replace, newString);
			return newString;
		}

		std::vector<string> StringUtil::split(string s, const char* delimiter) {
			/*char str[] = s.c_str(); //"- This, a sample string.";
			char* pch;
			//printf ("Splitting string \"%s\" into tokens:\n",str);
			pch = strtok (str, delimiter);

			std::vector<string> strings;
			while (pch != NULL) {
				//printf ("%s\n",pch);
				strings.push_back(string(pch));
				pch = strtok (NULL, delimiter);
			}

			return strings;*/
			std::vector<string> elems;
			stringstream ss(s);
			string item;
			while(std::getline(ss, item, *delimiter)) {
				elems.push_back(item);
			}
			return elems;
		}


		vector<string> StringUtil::getLinesForWordWrap(std::string str, int maxWidth) {
			int accumulatedWidth = 0;
			unsigned int lastLineUpToWord = 0;

			vector<string> lines;
			std::vector<std::string> words = StringUtil::split(str, " ");

			for (unsigned int i = 0; i < words.size(); i++) {
				accumulatedWidth += ARK2D::getRenderer()->getFont()->getStringWidth(words.at(i).append(" "));
				if (i == words.size()-1 && lastLineUpToWord != words.size()) {
					accumulatedWidth = maxWidth + 1;
					i += 1;
				}
				if (accumulatedWidth > maxWidth) {
					std::string line = "";
					for (unsigned int j = lastLineUpToWord; j < i; j++) {
						line += words.at(j) + " ";
					}
					lines.push_back(line);
					lastLineUpToWord = i;
					accumulatedWidth = 0;
				}
			}
			return lines;
		}


		string StringUtil::pathToFile(string file) {
			signed int slashpos = file.find_last_of("/");
			if (slashpos != (signed int) string::npos) {
				return file.substr(0, slashpos + 1);
			} else {
				return file;
			}
		}

		int StringUtil::utf8strlen(const char* s) {
			int len = 0;
			while (*s) len += (*s++ & 0xc0) != 0x80;
			return len;
		}



		/*
		   base64.cpp and base64.h

		   Copyright (C) 2004-2008 Ren� Nyffenegger

		   This source code is provided 'as-is', without any express or implied
		   warranty. In no event will the author be held liable for any damages
		   arising from the use of this software.

		   Permission is granted to anyone to use this software for any purpose,
		   including commercial applications, and to alter it and redistribute it
		   freely, subject to the following restrictions:

		   1. The origin of this source code must not be misrepresented; you must not
			  claim that you wrote the original source code. If you use this source code
			  in a product, an acknowledgment in the product documentation would be
			  appreciated but is not required.

		   2. Altered source versions must be plainly marked as such, and must not be
			  misrepresented as being the original source code.

		   3. This notice may not be removed or altered from any source distribution.

		   Ren� Nyffenegger rene.nyffenegger@adp-gmbh.ch

		*/

		const string StringUtil::base64_chars =
							 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
							 "abcdefghijklmnopqrstuvwxyz"
							 "0123456789+/";


		bool StringUtil::is_base64(unsigned char c) {
			return (base64_chars.find(c) != string::npos);
			//return (isalnum(c) || (c == '+') || (c == '/'));
		}

		string StringUtil::encodeBase64(const unsigned char* bytes_to_encode, unsigned int in_len) {
			string ret;
			int i = 0;
			int j = 0;
			unsigned char char_array_3[3];
			unsigned char char_array_4[4];

			while (in_len--) {
				char_array_3[i++] = *(bytes_to_encode++);
				if (i == 3) {
					char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
					char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
					char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
					char_array_4[3] = char_array_3[2] & 0x3f;

					for(i = 0; (i <4) ; i++)
						ret += base64_chars[char_array_4[i]];
					i = 0;
				}
			}

			if (i) {
				for(j = i; j < 3; j++)
					char_array_3[j] = '\0';

				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (j = 0; (j < i + 1); j++)
					ret += base64_chars[char_array_4[j]];

				while((i++ < 3))
					ret += '=';
			}

			return ret;
		}

		string StringUtil::decodeBase64(string const& encoded_string) {
			int in_len = encoded_string.size();
			int i = 0;
			int j = 0;
			int in_ = 0;
			unsigned char char_array_4[4], char_array_3[3];
			string ret;

			while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
				char_array_4[i++] = encoded_string[in_]; in_++;
				if (i ==4) {
					for (i = 0; i <4; i++)
						char_array_4[i] = base64_chars.find(char_array_4[i]);

					char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
					char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
					char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

					for (i = 0; (i < 3); i++)
						ret += char_array_3[i];
					i = 0;
				}
			}

			if (i) {
				for (j = i; j <4; j++)
					char_array_4[j] = 0;

				for (j = 0; j <4; j++)
					char_array_4[j] = base64_chars.find(char_array_4[j]);

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
			}

			return ret;
		}

		bool StringUtil::file_exists(const char * filename) {
			string strFileName(filename);
			//#if defined(ARK2D_ANDROID)
			//	strFileName = ARK2D::getContainer()->m_platformSpecific.m_externalDataStr + strFileName;
			//#endif

			strFileName = internalOSAppends(strFileName);

			ARK2D::getLog()->i(StringUtil::append("Does file exist: ", strFileName));
			FILE* file = NULL;
			#if defined( ARK2D_WINDOWS_PHONE_8 )
				fopen_s(&file, strFileName.c_str(), "r");
			#else
				file = fopen(strFileName.c_str(), "r");
			#endif

			//file = fopen(strFileName.c_str(), "r");
			if (file != NULL) {
				ARK2D::getLog()->v("Yup");
				fclose(file);
				return true;
			}
			ARK2D::getLog()->v("Nope");
			//fclose(file);
			return false;
		}

		string StringUtil::internalOSAppends(string strFileName) {
			#if defined(ARK2D_ANDROID)
				strFileName = ARK2D::getContainer()->m_platformSpecific.m_externalDataStr + strFileName;
			#else
				if (strFileName.substr(1,1).compare(":") == 0 || strFileName.substr(0,1).compare("/") == 0) {

				} else {
					strFileName = ARK2D::getContainer()->getResourcePath() + strFileName;
				}
			#endif
			return strFileName;
		}

		string StringUtil::file_get_contents(const char* fileName) {
			if (fileName != NULL) {
				string strFileName(fileName);

				// Prepend shit for each OS.
				strFileName = internalOSAppends(strFileName);

				ARK2D::getLog()->i(StringUtil::append("Opening file: ", strFileName));
				std::fstream f(strFileName.c_str(), std::ios::in);
				if (!f.is_open()) {
					//std::cout << "File does not exist." << std::endl;
					string str = "Could not open file ["; str += strFileName; str += "] as it does not exist.";
					ARK2D::getLog()->e(str);
					//ErrorDialog::createAndShow(str);
					//std::cout << str << std::endl;
					return string("");
				} else {
					f.close();

					char* text = NULL;

					//ARK2D::getLog()->e("pre fopen");
					FILE* file = NULL;// fopen(strFileName.c_str(), "rt");
					#if defined( ARK2D_WINDOWS_PHONE_8 )
						fopen_s(&file, strFileName.c_str(), "rt");
					#else
						file = fopen(strFileName.c_str(), "rt");
					#endif
					if (file == NULL) {
						string str = "Could not open file ["; str += strFileName; str += "] as it does not exist.";
						ARK2D::getLog()->e(str);
						return string("");
					}

					//ARK2D::getLog()->e("pre seeking");
					fseek(file, 0, SEEK_END);
					int count = ftell(file);

					//ARK2D::getLog()->e("pre rewind");
					rewind(file);

					// ***********************
					//! @todo: memory management: memory leak. yay.
					// *************************

					//ARK2D::getLog()->e("pre count/malloc");
					if (count > 0) {
						text = (char*)malloc(sizeof(char) * (count + 1));
						count = fread(text, sizeof(char), count, file);
						text[count] = '\0';
					}


					//ARK2D::getLog()->e("pre fclose");
					fclose(file);

					//ARK2D::getLog()->e("pre return str");
					string returnStr = (text == NULL)?string(""):string(text);

					//ARK2D::getLog()->e("pre free");
					free(text);

					//ARK2D::getLog()->e("pre return");
					return returnStr;

					//return text;
				}
			}
			return string("");

		}

		void StringUtil::toUpper(string& str) {
			//transform(str.begin(), str.end(), str.begin(), toupper);

			//int leng=str.length();
			//for(int i=0; i<leng; i++) {
			//	if (97<=str[i]&&str[i]<=122) {//a-z
			//		str[i]-=32;
			//	}
			//}
		}

		void StringUtil::toLower(string& str) {
			//std::transform(str.begin(), str.end(), str.begin(), tolower);

			///int leng=str.length();
			//for(int i=0; i<leng; i++) {
			//	if (65<=str[i]&&str[i]<=90) { //A-Z
			//		str[i]+=32;
			//	}
			//}

			/*for (int i=0;i<strlen(str.c_str());i++)  {
				if (str[i] >= 0x41 && str[i] <= 0x5A) {
					str[i] = str[i] + 0x20;
				}
			}*/

			#if defined(ARK2D_ANDROID)
				//ErrorDialog::createAndShow("not implemented");
			#else
				for (unsigned int q = 0; q < str.length(); q++) {
					str[q] = tolower(str[q]);
				}
			#endif

		}

		string StringUtil::urlencode(const string& c) {
			string escaped="";
		    int max = c.length();
		    for(int i=0; i<max; i++)
		    {
		        if ( (48 <= c[i] && c[i] <= 57) ||//0-9
		             (65 <= c[i] && c[i] <= 90) ||//abc...xyz
		             (97 <= c[i] && c[i] <= 122) || //ABC...XYZ
		             (c[i]=='~' || c[i]=='!' || c[i]=='*' || c[i]=='(' || c[i]==')' || c[i]=='\'')
		        )
		        {
		            escaped.append( &c[i], 1);
		        }
		        else
		        {
		            escaped.append("%");
		            escaped.append( StringUtil::char2hex(c[i]) );//converts char 255 to string "ff"
		        }
		    }
		    return escaped;
		}

		string StringUtil::char2hex( char dec )
		{
		    char dig1 = (dec&0xF0)>>4;
		    char dig2 = (dec&0x0F);
		    if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48inascii
		    if (10<= dig1 && dig1<=15) dig1+=97-10; //a,97inascii
		    if ( 0<= dig2 && dig2<= 9) dig2+=48;
		    if (10<= dig2 && dig2<=15) dig2+=97-10;

		    string r;
		    r.append( &dig1, 1);
		    r.append( &dig2, 1);
		    return r;
		}



		void StringUtil::trimLeft(string& str, const char* chars2remove) {
			if (!str.empty()) {
				string::size_type pos = str.find_first_not_of(chars2remove);

				if (pos != string::npos)
					str.erase(0,pos);
				else
					str.erase( str.begin() , str.end() ); // make empty
			}
		}

		void StringUtil::trimRight(string& str, const char* chars2remove) {
			if (!str.empty())
			{
				string::size_type pos = str.find_last_not_of(chars2remove);

				if (pos != string::npos)
					str.erase(pos+1);
				else
					str.erase( str.begin() , str.end() ); // make empty
			}
		}

		void StringUtil::trim(string& str, const char* chars2remove) {
			StringUtil::trimLeft(str, chars2remove);
			StringUtil::trimRight(str, chars2remove);
		}
		string StringUtil::trimret(string str, string charsToRemove) {
			StringUtil::trim(str, charsToRemove.c_str());
			return str;
		}
		void StringUtil::trimAll(vector<String>& strings, string chars2remove) {
			//for(unsigned int i = 0; i < strings.size(); i++) {
			//	const string& s = strings[i].get();
			//	StringUtil::trim(s, chars2remove.c_str());
			//}
		}

		string StringUtil::caesarShift(const string& s, int amount) {
			string newString;
			for(unsigned int i = 0; i < s.length(); i++) {
				const unsigned char thisChar = s.at(i);
				newString += ((unsigned char)thisChar >> amount) | (thisChar << (8-amount));
			}
			return newString;
		}

		std::wstring StringUtil::stringToWstring(const string& s) {

			std::wstring ws(s.size(), L' '); // Overestimate number of code points.
			ws.resize(mbstowcs(&ws[0], s.c_str(), s.size()));
			return ws;
		}
		std::string StringUtil::wstringToString(const wstring& s) {

			std::string ws(s.size(), ' '); // Overestimate number of code points.
			ws.resize(wcstombs(&ws[0], s.c_str(), s.size()));
			return ws;
		}

		string StringUtil::getExtension(string s) {
			unsigned int pos = s.find_last_of('.') + 1;
			return s.substr(pos);
		}

		string StringUtil::getOrdinal(int i) {
			vector<string> ends;
			ends.push_back("th");
			ends.push_back("st");
			ends.push_back("nd");
			ends.push_back("rd");
			ends.push_back("th");
			ends.push_back("th");
			ends.push_back("th");
			ends.push_back("th");
			ends.push_back("th");
			ends.push_back("th");

			if ((i % 100) >= 11 && (i % 100) <= 13) {
   				return "th";
			} else {
   				return ends.at(i % 10);
			}
			return "th";
		}

		string StringUtil::restrictToAlphanumericsSpaces(string str) {
			vector<char> allowedChars;
			allowedChars.push_back('A'); allowedChars.push_back('a');
			allowedChars.push_back('B'); allowedChars.push_back('b');
			allowedChars.push_back('C'); allowedChars.push_back('c');
			allowedChars.push_back('D'); allowedChars.push_back('d');
			allowedChars.push_back('E'); allowedChars.push_back('e');
			allowedChars.push_back('F'); allowedChars.push_back('f');
			allowedChars.push_back('G'); allowedChars.push_back('g');
			allowedChars.push_back('H'); allowedChars.push_back('h');
			allowedChars.push_back('I'); allowedChars.push_back('i');
			allowedChars.push_back('J'); allowedChars.push_back('j');
			allowedChars.push_back('K'); allowedChars.push_back('k');
			allowedChars.push_back('L'); allowedChars.push_back('l');
			allowedChars.push_back('M'); allowedChars.push_back('m');
			allowedChars.push_back('N'); allowedChars.push_back('n');
			allowedChars.push_back('O'); allowedChars.push_back('o');
			allowedChars.push_back('P'); allowedChars.push_back('p');
			allowedChars.push_back('Q'); allowedChars.push_back('q');
			allowedChars.push_back('R'); allowedChars.push_back('r');
			allowedChars.push_back('S'); allowedChars.push_back('s');
			allowedChars.push_back('T'); allowedChars.push_back('t');
			allowedChars.push_back('U'); allowedChars.push_back('u');
			allowedChars.push_back('V'); allowedChars.push_back('v');
			allowedChars.push_back('W'); allowedChars.push_back('w');
			allowedChars.push_back('X'); allowedChars.push_back('x');
			allowedChars.push_back('Y'); allowedChars.push_back('y');
			allowedChars.push_back('Z'); allowedChars.push_back('z');
			allowedChars.push_back('0');
			allowedChars.push_back('1');
			allowedChars.push_back('2');
			allowedChars.push_back('3');
			allowedChars.push_back('4');
			allowedChars.push_back('5');
			allowedChars.push_back('6');
			allowedChars.push_back('7');
			allowedChars.push_back('8');
			allowedChars.push_back('9');
			allowedChars.push_back(' ');
			return restrictToCharset(str, allowedChars);
		}
		string StringUtil::restrictToCharset(string str, vector<char> allowedChars) {
			string cleanStr("");
			for (unsigned int i = 0; i < str.length(); i++) {
				if(std::find(allowedChars.begin(), allowedChars.end(), str[i]) != allowedChars.end()) {
					cleanStr += str[i];
				}
			}
			return cleanStr;
		}


	}
}
