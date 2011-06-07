/*
 * ARKString.h
 *
 *  Created on: 23 Sep 2010
 *      Author: Ashley
 */

#ifndef ARKSTRING_H_
#define ARKSTRING_H_

#include <string>

using namespace std;

class ARKString {
	public:
		ARKString();
		ARKString(char* c);
		ARKString(const char* c);
		ARKString(string s);
		ARKString(int i);
		ARKString(double d);
		void append(int i);
		void append(float f);
		void append(double d);
		void append(const char* c);

		int length();

		virtual ~ARKString();

	private:
		std::string m_string;
};

#endif /* ARKSTRING_H_ */
