
#include "TiledMapProperty.h"

// Properties
TiledMapProperty::TiledMapProperty():
	m_name(),
	m_value()
{

}
TiledMapProperty::TiledMapProperty(const string& name, const string& value):
	m_name(name),
	m_value(value)
{
 
}

const string TiledMapProperty::getName() const {
	return m_name;
}
const string TiledMapProperty::getValue() const {
	return m_value;
}

void TiledMapProperty::setName(const string& s) {
	m_name = s;
}
void TiledMapProperty::setValue(const string& s) {
	m_value = s;
}

string TiledMapProperty::toString() {
	return m_name + ":" + m_value;
}
TiledMapProperty::~TiledMapProperty() {

}
