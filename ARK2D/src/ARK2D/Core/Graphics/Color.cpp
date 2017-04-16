/*
 * Color.cpp
 *
 *  Created on: 23-Apr-2010
 *      Author: Ashley
 */

#include "../Math/MathUtil.h"
#include "../Controls/ErrorDialog.h"
#include "../Cast.h"
#include "../../Common/OpenGL.h"
#include "Color.h"
#include "../Geometry/Vector4.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace ARK {
    namespace Core {
        namespace Graphics {

            const Color Color::black = Color(0, 0, 0);
            const Color Color::white = Color(255, 255, 255);

            const Color Color::red = Color(255, 0, 0);
            const Color Color::green = Color(0, 255, 0);
            const Color Color::cyan = Color(0, 255, 255);
            const Color Color::blue = Color(0, 0, 255);
            const Color Color::magenta = Color(255, 0, 255);
            const Color Color::orange = Color(255, 127, 0);
            const Color Color::yellow = Color(255, 255, 0);

            const Color Color::black_50a = Color(0.0f, 0.0f, 0.0f, 0.5f);
            const Color Color::black_0a = Color(0.0f, 0.0f, 0.0f, 0.0f);

            const Color Color::white_50a = Color(1.0f, 1.0f, 1.0f, 0.5f);
            const Color Color::white_0a = Color(1.0f, 1.0f, 1.0f, 0.0f);

            const Color Color::darker_grey = Color(32, 32, 32);
            const Color Color::dark_grey = Color(64, 64, 64);
            const Color Color::grey = Color(127, 127, 127);
            const Color Color::light_grey = Color(191, 191, 191);


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
                set(hexString);
            }

            Color::Color(int red, int green, int blue):
                m_r(red),
                m_g(green),
                m_b(blue),
                m_a(255)
            {

            }

            Color::Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a):
                m_r(r),
                m_g(g),
                m_b(b),
                m_a(a)
            {

            }

            Color::Color(float r, float g, float b, float a):
                m_r(0),
                m_g(0),
                m_b(0),
                m_a(0) {
                m_r = ((unsigned int) (r * 255.0f));
                m_g = ((unsigned int) (g * 255.0f));
                m_b = ((unsigned int) (b * 255.0f));
                m_a = ((unsigned int) (a * 255.0f));
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


            void Color::set(float r, float g, float b, float a) {
                m_r = (unsigned int) (r * 255.0f);
                m_g = (unsigned int) (g * 255.0f);
                m_b = (unsigned int) (b * 255.0f);
                m_a = (unsigned int) (a * 255.0f);

                if (m_r > 255) m_r = 255;
                if (m_g > 255) m_g = 255;
                if (m_b > 255) m_b = 255;
                if (m_a > 255) m_a = 255;
            }
            void Color::setRed(float r) {
                m_r = (unsigned int) (r * 255.0f);
                if (m_r > 255) m_r = 255;
            }
            void Color::setGreen(float g) {
                m_g = (unsigned int) (g * 255.0f);
                if (m_g > 255) m_g = 255;
            }
            void Color::setBlue(float b) {
                m_b = (unsigned int) (b * 255.0f);
                if (m_b > 255) m_b = 255;
            }
            void Color::setAlpha(float a) {
                m_a = (unsigned int) (a * 255.0f);
                if (m_a > 255) m_a = 255;
            }

            void Color::set(const std::string hexString) {
                char hex[2];
                strncpy(hex, &hexString[0], 1);
                hex[1] = 0;

                //std::cout << hex << " : " << hexString << std::endl;
                if (strcmp(hex, "#") != 0) {
                    ErrorDialog::createAndShow("Color::Color(string) constructor's hex color must begin with a hash symbol.");
                    exit(0);
                }

                char red[3];
                strncpy(red, &hexString[1], 2);
                red[2] = 0;

                char green[3];
                strncpy(green, &hexString[3], 2);
                green[2] = 0;

                char blue[3];
                strncpy(blue, &hexString[5], 2);
                blue[2] = 0;

                //std::cout << "Loaded Color: #" << red << green << blue << std::endl;
                m_r = Cast::hextoint(&red[0]);
                m_g = Cast::hextoint(&green[0]);
                m_b = Cast::hextoint(&blue[0]);
                m_a = 255;
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

            const Color* Color::constpointer() const {
                return this;
            }
            Color* Color::pointer() const {
                return const_cast<Color*>( this );
            }

            float Color::getRedf() const {
                return float(m_r)/255.0f;
            }
            float Color::getGreenf() const {
                return float(m_g)/255.0f;
            }
            float Color::getBluef() const {
                return float(m_b)/255.0f;
            }
            float Color::getAlphaf() const {
                return float(m_a)/255.0f;
            }
            string Color::getHex() const {
                string returnStr = string("#");;
                returnStr += Cast::inttohexcolor(m_r);
                returnStr += Cast::inttohexcolor(m_g);
                returnStr += Cast::inttohexcolor(m_b);
                return returnStr;
            }
            uint32_t Color::asInt() const {
                return pack(*this);
            }

            void Color::bind() const {
                #ifndef NO_FIXED_FUNCTION_PIPELINE
                    glColor4f(getRedf(), getGreenf(), getBluef(), getAlphaf());
                #endif
            }
            Color* Color::copy() {
                return new Color(m_r,m_g,m_b,m_a);
            }
            const Color Color::ccopy() {
                return Color(m_r, m_g, m_b, m_a);
            }
            bool Color::operator==(Color c) {
                if (m_r == c.getRed() && m_g == c.getGreen() && m_b == c.getBlue() && m_a == c.getAlpha()) {
                    return true;
                }
                return false;
            }

            Color::~Color() {

            }


            uint32_t Color::pack(Color c) {
                return pack( &c );
            }
            uint32_t Color::pack(Color* c) {
                return pack(
                    (unsigned char) (c->m_r),
                    (unsigned char) (c->m_g),
                    (unsigned char) (c->m_b),
                    (unsigned char) (c->m_a)
                );
            }
            uint32_t Color::pack(Vector4* vec) {
                return pack( (unsigned char) vec->x, (unsigned char) vec->y, (unsigned char) vec->z, (unsigned char) vec->w );
            }
            uint32_t Color::pack(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
                uint32_t u = (unsigned char) r;
                u <<= 8; 					// now u would be 11001010 00000000
                u |= (unsigned char) g;		// now u would be 11001010 11111110
                u <<= 8;					// now u would be 11001010 11111110 00000000
                u |= (unsigned char) b;		// now u would be 11001010 11111110 10111010
                u <<= 8; 					// now u would be 11001010 11111110 10111010 00000000
                u |= (unsigned char) a;		// now u would be 11001010 11111110 10111010 10111110
                                            // This is how        r        g        b        a
                                            // are packed into one integer u.
                return u;
            }
            unsigned char Color::unpackRed(uint32_t c) {
                return (c >> 24) & 0xff;
            }
            unsigned char Color::unpackGreen(uint32_t c) {
                return (c >> 16) & 0xff;
            }
            unsigned char Color::unpackBlue(uint32_t c) {
                return (c >> 8) & 0xff;
            }
            unsigned char Color::unpackAlpha(uint32_t c) {
                return c & 0xff;
            }
        }

	}
}
