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

BMFont* ARK::Font::asBMFont() {
	return dynamic_cast<BMFont*>(this);
}
FTFont* ARK::Font::asFTFont() {
	return dynamic_cast<FTFont*>(this);
}

ARK::Font::~Font() {

}
