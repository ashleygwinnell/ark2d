/*
 * Font.cpp
 *
 *  Created on: Feb 4, 2012
 *      Author: ashleygwinnell
 */

#include "Font.h"
#include "BMFont.h"
#include "FTFont.h"

namespace ARK {
	namespace Font {
		Font::Font() {

		}


		void Font::drawStringCentered(const string& Str, int left_x, int right_x, int y) {
			int c_x = (right_x / 2) + (left_x/2);
			drawString(Str, c_x - (getStringWidth(Str)/2), y);
		}

		void Font::drawStringCenteredAt(const string& Str, int x, int y) {
			drawString(Str, x - (getStringWidth(Str)/2), y - (getStringHeight(Str)/2));
		}

		ARK::Font::BMFont* Font::asBMFont() {
			return dynamic_cast<ARK::Font::BMFont*>(this);
		}
		ARK::Font::FTFont* Font::asFTFont() {
			return dynamic_cast<ARK::Font::FTFont*>(this);
		}

		Font::~Font() {

		}

	}
}
