/*
 * Strings.h
 *
 *  Created on: Jun 09, 2015
 *      Author: ashleygwinnell
 */

#ifndef ARK_CORE_STRINGS_H_
#define ARK_CORE_STRINGS_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include <string>
#include <map>
using namespace std;

namespace ARK {
	namespace Core {

		class ARK2D_API Strings {
			public:
				static const unsigned int LANGUAGE_ENGLISH_UK = 0;
				static const unsigned int LANGUAGE_ENGLISH_US = 1;
				static const unsigned int LANGUAGE_FRENCH_FR = 2;
				static const unsigned int LANGUAGE_FRENCH_CA = 3;
				static const unsigned int LANGUAGE_SPANISH = 4;
				static const unsigned int LANGUAGE_GERMAN = 5;
				static const unsigned int LANGUAGE_ITALIAN = 6;
				static const unsigned int LANGUAGE_PORTUGUESE = 7;
				static const unsigned int LANGUAGE_RUSSIAN = 8;
				static const unsigned int LANGUAGE_GREEK = 9;
				static const unsigned int LANGUAGE_TURKISH = 10;
				static const unsigned int LANGUAGE_DANISH = 11;
				static const unsigned int LANGUAGE_NORWEGIAN = 12;
				static const unsigned int LANGUAGE_SWEDISH = 13;
				static const unsigned int LANGUAGE_DUTCH = 14;
				static const unsigned int LANGUAGE_ARABIC = 15;
				static const unsigned int LANGUAGE_POLISH = 16;
				static const unsigned int LANGUAGE_JAPANESE = 17;
				static const unsigned int LANGUAGE_SIMPLIFIED_CHINESE = 18;
				static const unsigned int LANGUAGE_FINNISH = 19;
				static const unsigned int LANGUAGE_HEBREW = 20;
				static const unsigned int LANGUAGES_BEGIN = LANGUAGE_ENGLISH_UK;
				static const unsigned int LANGUAGES_END = LANGUAGE_HEBREW;

				static const unsigned int FORMAT_XML = 0;
				static const unsigned int FORMAT_PO = 1;

			public:
				map<unsigned int, map<string, string> > m_data;
				unsigned int m_language;
				unsigned int m_languageFallback;
				bool m_languageFallbackEnabled;


				void setLanguage(unsigned int lang);
				void setLanguageFallbackEnabled(bool on, unsigned int fallback);
				void add(string file);
				void add(string file, unsigned int format);
				void add(string key, string value, signed int overrideLanguage = -1);
				string get(string key, signed int overrideLanguage = -1);
				string print();
				virtual ~Strings();

			public:
                static Strings* s_instance;
				static Strings* getInstance();
				static bool isSupportedLanguage(unsigned int lang);
				static map<string, unsigned int>* s_textLanguageToConstLanguages;
				static unsigned int textLanguageToConstLanguage(string lang);


			private:
				Strings();

		};

	}
}

#endif /* ARK_CORE_STRINGS_H_ */
