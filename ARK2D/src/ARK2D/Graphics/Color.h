/*
 * Color.h
 *
 *  Created on: 23-Apr-2010
 *      Author: Ashley
 */


#ifndef COLOR_H_
#define COLOR_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include <string>
using std::string;

namespace ARK {
	namespace Graphics {

		/*!
		 * \brief RGBA colour class.
		 *
		 * @todo Colour merging.
		 * @todo Fade from one colour to another.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Color {
			public:
				Color();
				Color(const Color& color);
				Color(const std::string hexString);
				Color(int red, int green, int blue);
				Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
				Color(float r, float g, float b, float a);
				void setRed(unsigned int r);
				void setGreen(unsigned int g);
				void setBlue(unsigned int b);
				void setAlpha(unsigned int a);
				void setRed(float r);
				void setGreen(float g);
				void setBlue(float b);
				void setAlpha(float a);
				void set(uint32_t c);
				void set(float r, float g, float b, float a);
				void set(const std::string hexString);
				unsigned int getRed() const;
				unsigned int getGreen() const;
				unsigned int getBlue() const;
				unsigned int getAlpha() const;
				const Color* constpointer() const;
				Color* pointer() const;
				inline unsigned char getRedc() const { return m_r; }
				inline unsigned char getGreenc() const { return m_g; }
				inline unsigned char getBluec() const { return m_b; }
				inline unsigned char getAlphac() const { return m_a; }
				uint32_t asInt() const;
				float getRedf() const;
				float getGreenf() const;
				float getBluef() const;
				float getAlphaf() const;
				string getHex() const;
				void bind() const;
				Color* copy();
				const Color ccopy();


				bool operator==(Color c);

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
				static const Color yellow;
				static const Color cyan;
				static const Color magenta;
				static const Color orange;
				static const Color black_50a;
				static const Color black_0a;
				static const Color white_50a;
				static const Color white_0a;
			private:
				unsigned int m_r;
				unsigned int m_g;
				unsigned int m_b;
				unsigned int m_a;

			public:
				static uint32_t pack(Color c);
				static uint32_t pack(Color* c);
                static uint32_t pack(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
				static unsigned char unpackRed(uint32_t c);
				static unsigned char unpackGreen(uint32_t c);
				static unsigned char unpackBlue(uint32_t c);
				static unsigned char unpackAlpha(uint32_t c);

		};
	}
}

#endif /* COLOR_H_ */
