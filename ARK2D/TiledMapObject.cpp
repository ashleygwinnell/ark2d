
#include "TiledMapObject.h"


// tiled map object
TiledMapObject::TiledMapObject() {

}

void TiledMapObject::setName(string s) {
	m_name = s;
}
string& TiledMapObject::getName() {
	return m_name;
}

void TiledMapObject::setType(string s) {
	m_type = s;
}
string& TiledMapObject::getType() {
	return m_type;
}

void TiledMapObject::setX(signed int x) {
	m_x = x;
}
signed int TiledMapObject::getX() {
	return m_x;
}

void TiledMapObject::setY(signed int y) {
	m_y = y;
}
signed int TiledMapObject::getY() {
	return m_y;
}

void TiledMapObject::setWidth(unsigned int w) {
	m_width = w;
}
unsigned int TiledMapObject::getWidth() {
	return m_width;
}

void TiledMapObject::setHeight(unsigned int h) {
	m_height = h;
}
unsigned int TiledMapObject::getHeight() {
	return m_height;
}

TiledMapObject::~TiledMapObject() {

}
