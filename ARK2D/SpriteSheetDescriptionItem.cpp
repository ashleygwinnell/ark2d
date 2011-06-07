/*
 * SpriteSheetDescription.cpp
 *
 *  Created on: 31 Jul 2010
 *      Author: Ashley
 */

#include <string.h>

#include "SpriteSheetDescriptionItem.h"

SpriteSheetDescriptionItem::SpriteSheetDescriptionItem():
	m_x(0),
	m_y(0),
	m_width(0),
	m_height(0)
{

}
const char* SpriteSheetDescriptionItem::getName() const {
	return m_name;
}
unsigned int SpriteSheetDescriptionItem::getX() const {
	return m_x;
}
unsigned int SpriteSheetDescriptionItem::getY() const {
	return m_y;
}
unsigned int SpriteSheetDescriptionItem::getWidth() const {
	return m_width;
}
unsigned int SpriteSheetDescriptionItem::getHeight() const {
	return m_height;
}

void SpriteSheetDescriptionItem::setName(const char* name) {
	m_name = name;
}
void SpriteSheetDescriptionItem::setX(unsigned int x) {
	m_x = x;
}
void SpriteSheetDescriptionItem::setY(unsigned int y) {
	m_y = y;
}
void SpriteSheetDescriptionItem::setWidth(unsigned int width) {
	m_width = width;
}
void SpriteSheetDescriptionItem::setHeight(unsigned int height) {
	m_height = height;
}

SpriteSheetDescriptionItem::~SpriteSheetDescriptionItem() {

}
