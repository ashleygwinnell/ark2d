
#include "TiledMapObjectGroup.h"


// object group
TiledMapObjectGroup::TiledMapObjectGroup(): m_name(), m_objects(), m_visible(true) {

}
void TiledMapObjectGroup::addObject(TiledMapObject* object) {
	m_objects.push_back(object);
}
vector<TiledMapObject*> TiledMapObjectGroup::getObjects() {
	return m_objects;
}
void TiledMapObjectGroup::setName(const string& name) {
	m_name = name;
}
const string& TiledMapObjectGroup::getName() const {
	return m_name;
}
void TiledMapObjectGroup::setWidth(unsigned int width) {
	m_width = width;
}
unsigned int TiledMapObjectGroup::getWidth() const {
	return m_width;
}
void TiledMapObjectGroup::setHeight(unsigned int height) {
	m_height = height;
}
unsigned int TiledMapObjectGroup::getHeight() const {
	return m_height;
}

void TiledMapObjectGroup::setVisible(bool b) {
	m_visible = b;
}
bool TiledMapObjectGroup::isVisible() {
	return m_visible;
}

TiledMapObjectGroup::~TiledMapObjectGroup() {

}

