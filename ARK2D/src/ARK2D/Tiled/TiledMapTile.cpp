/*
 * TiledMapTile.cpp
 *
 *  Created on: 27 Jan 2011
 *      Author: Ashley
 */

#include "TiledMapProperty.h"
#include "TiledMapTile.h"

namespace ARK {
	namespace Tiled {

		TiledMapTile::TiledMapTile():
			m_image(),
			m_properties()
		{

		}

		Image* TiledMapTile::getImage() const{
			return m_image;
		}

		TiledMapProperty* TiledMapTile::getPropertyByName(string s) const {
			for(unsigned int p = 0; p < m_properties.size(); p++) 
			{
				TiledMapProperty* property = m_properties.at(p);
				if (property->getName() == s) {
					return property;
				}
			}
			return NULL;
		}
		vector<TiledMapProperty*> TiledMapTile::getProperties() const {
			return m_properties;
		}

		TiledMapTile::~TiledMapTile() {

		}
	}
}
