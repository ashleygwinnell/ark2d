#ifndef TILED_MAP_TILESET_H_
#define TILED_MAP_TILESET_H_

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "../Graphics/Image/Image.h"

#include "TiledMapTile.h"

using namespace std;

namespace ARK {
	namespace Tiled {

		/*!
		 * \brief A tile set as part of a Tiled Map.
		 *
		 * @warning You should never need to create this manually.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class TiledMapTileset {
			public:
				TiledMapTileset();

				void ready(); // this should be after when the image, width, height, spacing, margin have been set. It create's Image pointers and stores them.

				void setName(const std::string& name);
				string getName();

				void setFirstGID(unsigned int id);
				unsigned int getFirstGID();

				void setLastGID(unsigned int id);
				unsigned int getLastGID();

				void setTileWidth(unsigned int w);
				unsigned int getTileWidth();

				void setTileHeight(unsigned int h);
				unsigned int getTileHeight();

				void setImage(Image* image);

				void setSpacing(unsigned int s);
				unsigned int getSpacing();

				void setMargin(unsigned int m);
				unsigned int getMargin();

				void setNumberOfTilesAcross(unsigned int w);
				unsigned int getNumberOfTilesAcross();

				void setNumberOfTilesDown(unsigned int h);
				unsigned int getNumberOfTilesDown();

				TiledMapTile* getTileByGID(unsigned int gid) const;
				TiledMapTile* getTile(unsigned int x, unsigned int y) const;
				Image* getImage() const;

				~TiledMapTileset();
			public:
				string m_name;
				Image* m_image;
				Color m_mask;
				unsigned int m_firstgid;
				unsigned int m_lastgid;
				unsigned int m_tilewidth;
				unsigned int m_tileheight;
				unsigned int m_spacing;
				unsigned int m_margin;

				unsigned int m_total_tiles_across;
				unsigned int m_total_tiles_down;

				//vector< vector<TiledMapTile> > m_tiles;
				vector<TiledMapTile*> m_tiles;
		};
	}
}

#endif
