/*
 * Validation.cpp
 *
 *  Created on: 13 Feb 2013
 *      Author: Ashley
 */


#include "Validation.h"

#include "../Core/ARK2D.h"
#include "../Core/Log.h"

#include <algorithm>

namespace ARK {
	namespace Util {

		bool Validation::isAlphanumeric(string s)
		{
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
			//allowedChars.push_back(' ');
			return isCharset(allowedChars, s);
		}

		bool Validation::isAlphanumericSpaces(string s)
		{
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
			return isCharset(allowedChars, s);
		}

		bool Validation::isCharset(const vector<char>& chars, string s)
		{
			for (unsigned int i = 0; i < s.length(); i++) {
				if (std::find(chars.begin(), chars.end(), s[i]) == chars.end()) {
					return false;
				}
			}
			return true;
		}

	}
}

