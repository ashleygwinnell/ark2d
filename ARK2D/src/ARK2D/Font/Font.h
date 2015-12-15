/*
 * Font.h
 *
 *  Created on: Feb 4, 2012
 *      Author: ashleygwinnell
 */

#ifndef FONT_H_
#define FONT_H_
 
#include <string>
using namespace std;

class BMFont;
class FTFont;

#include "../Includes.h"
#include "../Core/Resource.h"

//namespace ARK {
	//namespace Font {

		/*!
		 * \brief Abstract class for all Font implementations.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API ARK::Font::Font : public ARK::Core::Resource {
			public:
				Font();
				virtual void drawString(const string& str, int x, int y) = 0;
				virtual void drawString(const string& str, int x, int y, int drawz) = 0;
				virtual void drawStringCentered(const string& Str, int left_x, int right_x, int y);
				virtual void drawStringCenteredAt(const string& Str, int x, int y);
				virtual unsigned int getStringWidth(const string& Str) const = 0;
				virtual unsigned int getStringHeight(const string& Str) const = 0;
				virtual unsigned int getLineHeight() const = 0;

				virtual void scale(float f) = 0;

				ARK::Font::BMFont* asBMFont();
				ARK::Font::FTFont* asFTFont();
				virtual ~Font();
		};
//	}
//}
#endif /* FONT_H_ */
