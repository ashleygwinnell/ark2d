/*
 * Strings.cpp
 *
 *  Created on: 9 June 2015
 *      Author: Ashley
 */

#include "Strings.h"
//#include "../Includes.h"
#include "Resource.h"
#include "String.h"
#include "GameContainer.h"
#include "../Util/StringUtil.h"
//#include "Log.h"
#include "../Common/Libraries/rapidxml.h"
#include "../vendor/rapidxml/ark_rapidxml_util.hpp"
#include "Log.h"

namespace ARK {
	namespace Core {

		Strings* Strings::s_instance = NULL;
		Strings* Strings::getInstance() {
			if (s_instance == NULL) {
				s_instance = new Strings();
			}
			return s_instance;
		}

		map<string, unsigned int>* Strings::s_textLanguageToConstLanguages = NULL;

		Strings::Strings():
			m_data(),
			m_language(LANGUAGE_ENGLISH_UK),
			m_languageFallback(LANGUAGE_ENGLISH_UK),
			m_languageFallbackEnabled(false)
		{

		}

		void Strings::setLanguage(unsigned int lang) {
			m_language = lang;
		}
		void Strings::setLanguageFallbackEnabled(bool on, unsigned int fallback) {
			m_languageFallbackEnabled = on;
			m_languageFallback = fallback;
		}
		unsigned int Strings::getLanguage() {
			return m_language;
		}
		unsigned int Strings::getSystemLanguage() {
			string clanguage = "";
			#if defined( ARK2D_MACINTOSH ) || defined( ARK2D_IPHONE )
				NSString* language = [[NSLocale preferredLanguages] objectAtIndex:0];
				clanguage = [language cStringUsingEncoding:[NSString defaultCStringEncoding]];
			#elif defined( ARK2D_ANDROID )
				clanguage = ARK2D::getContainer()->getPlatformSpecific()->getPluggable()->getISO6391Language();
			#else
				clanguage = "en-GB";
			#endif

			ARK2D::getLog()->e(clanguage);
            map<string, unsigned int> langs;
            langs.insert(std::pair<string, unsigned int>("fr", (unsigned int) LANGUAGE_FRENCH_FR ));
            langs.insert(std::pair<string, unsigned int>("de", (unsigned int) LANGUAGE_GERMAN ));
            langs.insert(std::pair<string, unsigned int>("it", (unsigned int) LANGUAGE_ITALIAN ));
            langs.insert(std::pair<string, unsigned int>("es", (unsigned int) LANGUAGE_SPANISH ));
            langs.insert(std::pair<string, unsigned int>("pt-PT", (unsigned int) LANGUAGE_PORTUGUESE ));
            langs.insert(std::pair<string, unsigned int>("pt-BR", (unsigned int) LANGUAGE_PORTUGUESE ));
            langs.insert(std::pair<string, unsigned int>("en-US", (unsigned int) LANGUAGE_ENGLISH_US ));


            map<string, unsigned int>::iterator it = langs.find(clanguage);
            if (it != langs.end()) {
                return it->second;
            }

            return LANGUAGE_ENGLISH_UK;
		}
		void Strings::add(string file) {
			add(file, FORMAT_XML);
		}
		void Strings::add(string file, unsigned int format) {
			if (format == FORMAT_XML) {
				String* s = Resource::get(file)->asString();
				string result = s->getc();

				//wstring result = StringUtil::stringToWstring(ss);
				// ARK2D::getLog()->e(result);

				vector<char> xml_copy = vector<char>(result.begin(), result.end());
				xml_copy.push_back('\0');

				xml_document<char> xmldocument;
				xmldocument.parse<0>((char*) &xml_copy[0]);

				xml_node<char>* root = xmldocument.first_node("strings");
				xml_attribute<char>* grouped_attr = root->first_attribute("grouped");

                //ARK2D::getLog()->e("grouped!");
                bool grouped = Cast::boolFromString(grouped_attr->value());

                if (grouped) {

                	unsigned int i = 0;
					xml_node<char>* groupNode = 0;
					for (groupNode = root->first_node("group");
						groupNode != NULL;
						groupNode = groupNode->next_sibling("group"))
					{
						xml_attribute<char>* lang = groupNode->first_attribute("lang");
						unsigned int languageInteger = textLanguageToConstLanguage(lang->value());

						if (m_data.find(languageInteger) == m_data.end()) {
							m_data[languageInteger] = map<string, string>();
						}

						xml_node<char>* stringNode = 0;
						for (stringNode = groupNode->first_node("string");
							stringNode != NULL;
							stringNode = stringNode->next_sibling("string"))
						{
                            string name = stringNode->first_attribute("name")->value();
							string value = rapidxml_myutil<string, char>::rapidXmlUtil_value(stringNode);
							//ARK2D::getLog()->e(name);
							m_data[languageInteger].insert(std::pair<string, string>( name, value ));

						}
					}

                }

				// rapidXmlUtil_value
                // ...

				/*strings
					string name lang

				strings
					group lang
					string name*/
                delete s;
			}
		}
		void Strings::add(string key, string value, signed int overrideLanguage) {
			unsigned int useLanguage = (overrideLanguage >= 0) ? overrideLanguage : m_language;
			m_data[useLanguage][key] = value;
		}
		string Strings::get(string key, signed int overrideLanguage) {
			// Try to use requested language.
			unsigned int useLanguage = (overrideLanguage >= 0) ? overrideLanguage : m_language;
			map<string, string>::iterator it = m_data[useLanguage].find(key);
			if (it != m_data[useLanguage].end()) {
				return it->second;
			}

			if (m_languageFallbackEnabled) {
				stringstream err;
				err << "Could not find string with key '" << key << "' in language '" << useLanguage <<  "'. Reverting to default of '" << m_languageFallback << "'.";
				//ARK2D::getLog()->w( err.str() );

				// Fallback language
				map<string, string>::iterator it2 = m_data[m_languageFallback].find(key);
				if (it2 != m_data[m_languageFallback].end()) {
					return it2->second;
				}
				return "";
			}

			return "";
		}
		string Strings::print() {
			stringstream out;

			map<unsigned int, map<string, string> >::iterator it = m_data.begin();
			while (it != m_data.end()) {
				map<string, string>::iterator it2 = it->second.begin();
				while (it2 != it->second.end()) {
					out << it2->first << " " << it2->second << "\n";
					it2++;
				}
				it++;
			}
			string str = out.str();
			//ARK2D::getLog()->i(str);
			return str;
		}
		void Strings::setExclusiveLanguages(const vector<string>& useLangs) {

			map<unsigned int, map<string, string> >::iterator it = m_data.begin(); // for each language
			while (it != m_data.end()) {
				bool found = false;

				for(unsigned int i = 0; i < useLangs.size(); ++i) { // see if we can find it in the exclusive list
					unsigned int lang = textLanguageToConstLanguage( useLangs[i] );
					if ( lang == it->first ) {
						found = true;
						break;
					}
				}

				if (!found) {
					it->second = map<string, string>(); // if it's not there, reset to an empty map.
				}
			}
		}

		bool Strings::isSupportedLanguage(unsigned int lang) {
			return (lang <= LANGUAGES_END);
		}
		unsigned int Strings::textLanguageToConstLanguage(string lang) {
			if (s_textLanguageToConstLanguages == NULL) {
				s_textLanguageToConstLanguages = new map<string, unsigned int>();
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("english_uk", (unsigned int) LANGUAGE_ENGLISH_UK));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("english_us", (unsigned int) LANGUAGE_ENGLISH_UK));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("french_fr", (unsigned int) LANGUAGE_FRENCH_FR));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("french_ca", (unsigned int) LANGUAGE_FRENCH_CA));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("spanish", (unsigned int) LANGUAGE_SPANISH));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("german", (unsigned int) LANGUAGE_GERMAN));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("italian", (unsigned int) LANGUAGE_ITALIAN));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("portuguese", (unsigned int) LANGUAGE_PORTUGUESE));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("russian", (unsigned int) LANGUAGE_RUSSIAN));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("greek", (unsigned int) LANGUAGE_GREEK));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("turkish", (unsigned int) LANGUAGE_TURKISH));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("danish", (unsigned int) LANGUAGE_DANISH));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("norwegian", (unsigned int) LANGUAGE_NORWEGIAN));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("swedish", (unsigned int) LANGUAGE_SWEDISH));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("dutch", (unsigned int) LANGUAGE_DUTCH));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("arabic", (unsigned int) LANGUAGE_ARABIC));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("polish", (unsigned int) LANGUAGE_POLISH));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("japanese", (unsigned int) LANGUAGE_JAPANESE));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("simplified_chinese", (unsigned int) LANGUAGE_SIMPLIFIED_CHINESE));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("finnish", (unsigned int) LANGUAGE_FINNISH));
				s_textLanguageToConstLanguages->insert(std::pair<string, unsigned int>("hebrew", (unsigned int) LANGUAGE_HEBREW));
			}

			map<string, unsigned int>::iterator it = s_textLanguageToConstLanguages->find(lang);
			if (it != s_textLanguageToConstLanguages->end()) {
				return it->second;
			}

			string err = "Could not find language: ";
			err += lang;
			//ARK2D::getLog()->e(err);

			return LANGUAGE_ENGLISH_UK;
		}

		Strings::~Strings() {

		}
	}
}







