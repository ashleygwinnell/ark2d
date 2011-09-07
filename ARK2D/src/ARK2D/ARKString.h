/*
 * ARKString.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef ARKSTRING_H_
#define ARKSTRING_H_

#include "StringUtil.h"
#include "Cast.h"

#include <string>
using namespace std;

class ARKString {
	private:
		string m_string;
	public:
		ARKString(): m_string("") {

		}
		ARKString(string s): m_string(s) {

		}


		void operator+=(const ARKString& r) {
			m_string.append(r.get());
		}
		void operator+=(const char* c) {
			m_string.append(c);
		}
		void operator+=(unsigned int i) {
			m_string = StringUtil::append(m_string, i);
		}
		void operator+=(signed int i) {
			m_string = StringUtil::append(m_string, i);
		}
		void operator+=(float f) {
			m_string = Cast::toString<float>(f);
		}


		void operator=(const ARKString& r) {
			m_string = string(r.get());
		}
		void operator=(const char* c) {
			m_string = string(c);
		}
		void operator=(unsigned int i) {
			m_string = StringUtil::append(m_string, i);
		}
		void operator=(signed int i) {
			m_string = StringUtil::append(m_string, i);
		}
		void operator=(float f) {
			m_string = Cast::toString<float>(f);
		}


		void insert(const ARKString& str, int at) {
			insert(str.get(), at);
		}
		void insert(const string& str, int at) {
			if (at > (int) m_string.length()) { return; }
		 	m_string.insert(at, str);
		}
		void insert(const char* str, int at) {
			if (at > (int) m_string.length()) { return; }
			m_string.insert(at, str);
		}
		void remove(int from, int length) {
			if (from < 0) { return; }
			m_string.erase(from, length);
		}
		void clear() {
			m_string.erase(0, m_string.length());
		}
		void operator--() {
			m_string = m_string.substr(0, m_string.size()-1);
		}
		ARKString substring(int from, int length) {
			return ARKString(m_string.substr(from, length));
		}
		const string& get() const {
			return m_string;
		}
		int getAsInt() const {
			return Cast::fromString<int>(m_string);
		}
		float getAsFloat() const {
			return Cast::fromString<float>(m_string);
		}
		int length() const {
			return m_string.length();
		}
		bool contains(const char* c) {
			return (m_string.find(c) != string::npos);
		}
		bool contains(string c) {
			return (m_string.find(c) != string::npos);
		}
		vector<ARKString> split(string delim) {
			string thisString = m_string;

			vector<ARKString> pieces;
			size_t cutIndex;

			while ((cutIndex = thisString.find_first_of(delim)) != string::npos)
			{
				string piece = thisString.substr(0, cutIndex);
				pieces.push_back(ARKString(piece));

				thisString = thisString.substr(cutIndex+delim.length());
			}

			pieces.push_back(ARKString(thisString));

			return pieces;
		}
		unsigned int countOccurrences(string s) {
			return split(s).size() - 1; // TODO: do this better. it's -1 because split includes empty strings AFTER the delimter.
		}
		bool equals(string s) {
			return (strcmp(m_string.c_str(), s.c_str()) == 0);
		}
		string lastChar() {
			return m_string.substr(m_string.size()-1);
		}
};

#endif /* MYSTRING_H_ */
