/*
 * Color.cpp
 *
 *  Created on: 23-Apr-2010
 *      Author: Ashley
 */

#include "Util/MathUtil.h"
#include "UI/ErrorDialog.h"
#include "Util/Cast.h"
#include "Color.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

const Color Color::black = Color(0, 0, 0, 0);
const Color Color::white = Color(255, 255, 255, 255);

const Color Color::red = Color(255, 0, 0);
const Color Color::green = Color(0, 255, 0);
const Color Color::cyan = Color(0, 255, 255);
const Color Color::blue = Color(0, 0, 255);
const Color Color::magenta = Color(255, 0, 255);

const Color Color::black_50a = Color(0, 0, 0, 127);
const Color Color::white_50a = Color(255, 255, 255, 127);

const Color Color::darker_grey = Color(32, 32, 32, 255);
const Color Color::dark_grey = Color(63, 63, 63, 255);
const Color Color::grey = Color(127, 127, 127, 255);
const Color Color::light_grey = Color(191, 191, 191, 255);


Color::Color():
	m_r(255),
	m_g(255),
	m_b(255),
	m_a(255)
{

}

Color::Color(const Color& color) {
	m_r = color.getRed();
	m_g = color.getGreen();
	m_b = color.getBlue();
	m_a = color.getAlpha();
}

Color::Color(const std::string hexString) {
	char hex[2]; strncpy(hex, &hexString[0], 1); hex[1] = 0;

	//std::cout << hex << " : " << hexString << std::endl;
	if (strcmp(hex, "#") != 0) {
		ErrorDialog::createAndShow("Color::Color(string) constructor's hex color must begin with a hash symbol.");
		exit(0);
	}
	char red[3]; strncpy(red, &hexString[1], 2); red[2] = 0;
	char green[3]; strncpy(green, &hexString[3], 2); green[2] = 0;
	char blue[3]; strncpy(blue, &hexString[5], 2); blue[2] = 0;
	std::cout << "Loaded Color: #" << red << green << blue << std::endl;
	m_r = Cast::hextoint(&red[0]);
	m_g = Cast::hextoint(&green[0]);
	m_b = Cast::hextoint(&blue[0]);
	m_a = 255;
}

Color::Color(int red, int green, int blue):
	m_r(red),
	m_g(green),
	m_b(blue),
	m_a(255)
{

}

Color::Color(int r, int g, int b, int a):
	m_r(r),
	m_g(g),
	m_b(b),
	m_a(a)
{

}

Color* Color::getRandom() {
	unsigned int r = MathUtil::randBetween(0, 255);
	unsigned int g = MathUtil::randBetween(0, 255);
	unsigned int b = MathUtil::randBetween(0, 255);
	unsigned int a = MathUtil::randBetween(0, 255);
	Color* c = new Color(r, g, b, a);
	return c;
}

void Color::setRed(unsigned int r) {
	m_r = r;
}
void Color::setGreen(unsigned int g) {
	m_g = g;
}
void Color::setBlue(unsigned int b) {
	m_b = b;
}
void Color::setAlpha(unsigned int a) {
	m_a = a;
}

unsigned int Color::getRed() const {
	return m_r;
}
unsigned int Color::getGreen() const {
	return m_g;
}
unsigned int Color::getBlue() const {
	return m_b;
}
unsigned int Color::getAlpha() const {
	return m_a;
}

void Color::bind() const {
	glColor4f(m_r/255.f, m_g/255.f, m_b/255.f, m_a/255.f);
}
bool Color::operator==(Color c) {
	if (m_r == c.getRed() && m_g == c.getGreen() && m_b == c.getBlue() && m_a == c.getAlpha()) {
		return true;
	}
	return false;
}

Color::~Color() {

}
