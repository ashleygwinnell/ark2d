/*
 * Font.cpp
 *
 *  Created on: Feb 4, 2012
 *      Author: ashleygwinnell
 */

#include "Font.h"
#include "BMFont.h"
#include "FTFont.h"

ARK::Font::Font() {

}

void ARK::Font::drawStringCentered(const string& Str, int left_x, int right_x, int y) const {
	int c_x = (right_x / 2) + (left_x/2);
	drawString(Str, c_x - (getStringWidth(Str)/2), y);
}

void ARK::Font::drawStringCenteredAt(const string& Str, int x, int y) const {
	drawString(Str, x - (getStringWidth(Str)/2), y - (getStringHeight(Str)/2));
}

BMFont* ARK::Font::asBMFont() {
	return dynamic_cast<BMFont*>(this);
}
FTFont* ARK::Font::asFTFont() {
	return dynamic_cast<FTFont*>(this);
}

ARK::Font::~Font() {

}
