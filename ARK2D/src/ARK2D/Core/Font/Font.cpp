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
    namespace Core {
        namespace Font {
            Font::Font():
                ARK::Core::Resource()
                {

            }


            void Font::drawStringCentered(const string& Str, int left_x, int right_x, int y) {
                int c_x = (right_x / 2) + (left_x/2);
                drawString(Str, c_x - (getStringWidth(Str)/2), y);
            }

            void Font::drawStringCenteredAt(const string& Str, float x, float y) {
                drawString(Str, x - (getStringWidth(Str)/2), y - (getStringHeight(Str)/2));
            }

            ARK::Core::Font::BMFont* Font::asBMFont() {
                return dynamic_cast<ARK::Core::Font::BMFont*>(this);
            }
            ARK::Core::Font::FTFont* Font::asFTFont() {
                return dynamic_cast<ARK::Core::Font::FTFont*>(this);
            }

            uint32_t Font::getLetterColor( int position ) {
                return 0;
            }

            Font::~Font() {

            }
        }
	}
}
