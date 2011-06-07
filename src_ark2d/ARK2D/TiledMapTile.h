/*
 * TiledMapTile.h
 *
 *  Created on: 27 Jan 2011
 *      Author: Ashley
 */

#ifndef TILEDMAPTILE_H_
#define TILEDMAPTILE_H_

#include <string>
#include <vector>

#include "Image.h"

class TiledMapProperty;

using namespace std;

class TiledMapTile {
	public:
		TiledMapTile();
		Image* getImage() const;
		vector<TiledMapProperty*> getProperties() const;

		virtual ~TiledMapTile();

	public:
		Image* m_image;
		vector<TiledMapProperty*> m_properties;
};

#endif /* TILEDMAPTILE_H_ */
