
#include "TiledMapObject.h"

namespace ARK {
	namespace Tiled {

		// tiled map object
		TiledMapObject::TiledMapObject():
			m_name(),
			m_type(),
			m_x(0),
			m_y(0),
			m_width(1),
			m_height(1),
			m_gid(0) 
		{
 
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

		void TiledMapObject::setGID(unsigned int h) {
			m_gid = h;
		}
		unsigned int TiledMapObject::getGID() {
			return m_gid;
		}

		void TiledMapObject::addProperty(string key, string value) {
			m_properties.push_back(new TiledMapProperty(key, value));
		}

		TiledMapProperty* TiledMapObject::getPropertyByName(string s) const {
			for(unsigned int p = 0; p < m_properties.size(); p++) 
			{
				TiledMapProperty* property = m_properties.at(p);
				if (property->getName() == s) {
					return property;
				}
			}
			return NULL;
		}

		TiledMapObject::~TiledMapObject() {

		}


	}
}
