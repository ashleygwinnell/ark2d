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

#include "../Resource.h"

namespace ARK {
	class Font : public ARK::Resource {
		public:
			Font();
			virtual void drawString(const string& str, int x, int y) const = 0;
			virtual void drawStringCentered(const string& Str, int left_x, int right_x, int y) const;
			virtual void drawStringCenteredAt(const string& Str, int x, int y) const;
			virtual unsigned int getStringWidth(const string& Str) const = 0;
			virtual unsigned int getStringHeight(const string& Str) const = 0;
			virtual unsigned int getLineHeight() const = 0;
			BMFont* asBMFont();
			FTFont* asFTFont();
			virtual ~Font();
	};
}
#endif /* FONT_H_ */
