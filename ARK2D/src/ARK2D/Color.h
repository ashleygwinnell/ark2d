/*
 * Color.h
 *
 *  Created on: 23-Apr-2010
 *      Author: Ashley
 */


#ifndef COLOR_H_
#define COLOR_H_

#include <string>
#include <cstring>
#include <stdlib.h>
#include "MathUtil.h"
#include "ARK2D_GL.h"

class Color {
	public:
		Color();
		Color(const Color& color);
		Color(const std::string hexString);
		Color(int red, int green, int blue);
		Color(int r, int g, int b, int a);
		void setRed(unsigned int r);
		void setGreen(unsigned int g);
		void setBlue(unsigned int b);
		void setAlpha(unsigned int a);
		unsigned int getRed() const;
		unsigned int getGreen() const;
		unsigned int getBlue() const;
		unsigned int getAlpha() const;

		void bind() const;

		~Color();
	public:
		static Color* getRandom();
		static const Color black;
		static const Color darker_grey;
		static const Color dark_grey;
		static const Color grey;
		static const Color light_grey;
		static const Color white;
		static const Color red;
		static const Color green;
		static const Color blue;
		static const Color magenta;
		static const Color black_50a;
		static const Color white_50a;
	private:
		unsigned int m_r;
		unsigned int m_g;
		unsigned int m_b;
		unsigned int m_a;
};

#endif /* COLOR_H_ */
