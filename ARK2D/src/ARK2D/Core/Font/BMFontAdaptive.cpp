/*
 * BMFontAdaptive.cpp
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

		BMFontAdaptive::BMFontAdaptive():
			fonts() {

		}

		void BMFontAdaptive::addFontSize( int multiplier, BMFont* font ) {
			fonts[multiplier] = font;
		}

		void BMFontAdaptive::drawStringSz( const std::string str, float size, float x, float y, signed int alignX, signed int alignY, float scale) {
			BMFont* useFont = NULL;
			float useScale = 1.0f;
			std::map<int, BMFont*>::iterator it = fonts.begin();
			do {
				if ( size <= it->second->getLineHeight() ) {
					useFont = it->second;
					useScale = size / float( it->second->getLineHeight() );
					break;
				}
				it++;
			}
			while ( it != fonts.end() );

			Renderer* r = ARK2D::getRenderer();
			r->setFont(useFont);
			r->drawString(str, x, y, alignX, alignY, rotation, useScale * scale);
		}

		BMFontAdaptive::~BMFontAdaptive() {

		}

	}
}

#endif
