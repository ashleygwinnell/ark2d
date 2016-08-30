/*
 * Font.h
 *
 *  Created on: Feb 4, 2012
 *      Author: ashleygwinnell
 */

#ifndef FONT_H_
#define FONT_H_

#include <stdint.h>
#include <string>
using namespace std;

#include "../../Namespaces.h"
/*namespace ARK {
	namespace Font {
		class BMFont;
		class FTFont;
	}
}*/

#include "../../Common/DLL.h"
#include "../Resource.h"

//namespace ARK {
	//namespace Font {

		/*!
		 * \brief Abstract class for all Font implementations.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
        class ARK2D_API ARK::Core::Font::Font : public ARK::Core::Resource {
			public:
				Font();
				virtual void drawString(const string& str, float x, float y) = 0;
				virtual void drawString(const string& str, float x, float y, float drawz) = 0;
				virtual void drawStringCentered(const string& Str, int left_x, int right_x, int y);
				virtual void drawStringCenteredAt(const string& Str, float x, float y);
				virtual unsigned int getStringWidth(const string& Str) const = 0;
				virtual unsigned int getStringHeight(const string& Str) const = 0;
				virtual unsigned int getLineHeight() const = 0;

				virtual void scale(float f) = 0;

                ARK::Core::Font::BMFont* asBMFont();
				ARK::Core::Font::FTFont* asFTFont();
				virtual ~Font();

			protected:
				virtual uint32_t getLetterColor( int position );
		};
//	}
//}
#endif /* FONT_H_ */
