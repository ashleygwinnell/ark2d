/*
 * BMFontAdaptive.h
 *
 *  Created on: 4-Aug-2016
 *      Author: Ashley
 */

#ifndef ARK_FONT_BMFONTADAPTIVE_H_
#define ARK_FONT_BMFONTADAPTIVE_H_

#include "../ARK2D.h"
#include "../Namespaces.h"

#include <map>

namespace ARK {
	namespace Font {

		class ARK2D_API BMFontAdaptive {
			private:
				std::map<int, BMFont*> fonts; // int-scale to bmfont.

			public:
				BMFontAdaptive();

				void addFontSize( int multiplier, BMFont* font );
				void drawStringSz( const std::string str, float size, float x, float y, signed int alignX, signed int alignY, float scale = 1.0f );

				virtual ~BMFontAdaptive();
		};

	}
}

#endif
