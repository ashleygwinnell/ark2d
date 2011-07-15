/*
 * MyString.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef MYSTRING_H_
#define MYSTRING_H_

#include "ARK.h"
#include <string>
using namespace std;

class MyString {
	private:
		string m_string;
	public:
		MyString(): m_string("") {}
		MyString(string s): m_string(s) {}
		void operator+=(const MyString& r) {
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
		void operator+=(float i) {

		}
		void insert(const MyString& str, int at) {
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
		MyString substring(int from, int length) {
			return MyString(m_string.substr(from, length));
		}
		const string& get() const {
			return m_string;
		}
		int getAsInt() const {
			return Cast::fromString<int>(m_string);
		}
		int length() const {
			return m_string.length();
		}
};

#endif /* MYSTRING_H_ */
