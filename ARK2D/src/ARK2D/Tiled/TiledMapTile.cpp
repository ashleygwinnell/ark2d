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

		vector<TiledMapProperty*> TiledMapTile::getProperties() const {
			return m_properties;
		}

		TiledMapTile::~TiledMapTile() {

		}
	}
}
