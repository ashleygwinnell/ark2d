/*
 * StringUtil.cpp
 *
 *  Created on: 13-Dec-2009
 *      Author: Ashley
 */

#include "StringUtil.h"
#include "../Core/ToString.h"
#include "../Core/String.h"
#include "../Core/GameContainer.h"
#include "../Util/Log.h"

namespace ARK {
	namespace Util {

		std::string StringUtil::append(string str, int i) {
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


		string StringUtil::pathToFile(string file) {
			signed int slashpos = file.find_last_of("/");
			if (slashpos != (signed int) string::npos) {
				return file.substr(0, slashpos + 1);
			} else {
				return file;
			}
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
			#if defined(ARK2D_ANDROID)
				strFileName = ARK2D::getContainer()->m_platformSpecific.m_externalDataStr + strFileName;
			#endif
			ARK2D::getLog()->i(StringUtil::append("Does file exist: ", strFileName));
			FILE* file = NULL;
			file = fopen(strFileName.c_str(), "r");
			if (file != NULL) {
				fclose(file);
				return true;
			}
			//fclose(file);
			return false;
		}

		string StringUtil::file_get_contents(const char* fileName) {
			if (fileName != NULL) {
				string strFileName(fileName);
				#if defined(ARK2D_ANDROID)
					strFileName = ARK2D::getContainer()->m_platformSpecific.m_externalDataStr + strFileName;
				#else
					strFileName = ARK2D::getContainer()->getResourcePath() + strFileName;
				#endif

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

					FILE* file = fopen(strFileName.c_str(), "rt");
					if (file == NULL) {
						string str = "Could not open file ["; str += strFileName; str += "] as it does not exist.";
						ARK2D::getLog()->e(str);
						return string(""); 
					}

					fseek(file, 0, SEEK_END);
					int count = ftell(file);
					rewind(file);

					// ***********************
					//! @todo: memory management: memory leak. yay.
					// *************************

					if (count > 0) {
						text = (char*)malloc(sizeof(char) * (count + 1));
						count = fread(text, sizeof(char), count, file);
						text[count] = '\0';
					}
					fclose(file);

					string returnStr = (text == NULL)?string(""):string(text);
					free(text);
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

		string StringUtil::getExtension(string s) {
			unsigned int pos = s.find_last_of('.') + 1;
			return s.substr(pos);
		}


	}
}
