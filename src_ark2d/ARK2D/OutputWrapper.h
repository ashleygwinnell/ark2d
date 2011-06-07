/*
 * OutputWrapper.h
 *
 *  Created on: 8 Sep 2010
 *      Author: Ashley
 */

#ifndef OUTPUTWRAPPER_H_
#define OUTPUTWRAPPER_H_

#include <stdlib.h>
#include <iostream>

class OutputWrapper {

	public:

		static bool m_enabled;

		static void setEnabled(bool b) {
			m_enabled = b;
		}

		static void print(char* c) {
			if (m_enabled == false) { return; }
			std::cout << c;
		}
		static void print(const char* c) {
			if (m_enabled == false) { return; }
			std::cout << c;
		}
		static void print(std::string s) {
			print(s.c_str());
		}

		static void println(char* c) {
			if (m_enabled == false) { return; }
			std::cout << c << std::endl;
		}
		static void println(const char* c) {
			if (m_enabled == false) { return; }
			std::cout << c << std::endl;
		}
		static void println(std::string s) {
			println(s.c_str());
		}

		static void err(char* c) {
			if (m_enabled == false) { return; }
			std::cerr << c << std::endl;
		}
		static void err(const char* c) {
			if (m_enabled == false) { return; }
			std::cerr << c << std::endl;
		}
		static void err(std::string s) {
			err(s.c_str());
		}
};



#endif /* OUTPUTWRAPPER_H_ */
