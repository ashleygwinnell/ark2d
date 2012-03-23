/*
 * TiledMap.h
 *
 *  Created on: 23-Apr-2010
 *      Author: Ashley
 */

#ifndef TILEDMAP_H_
#define TILEDMAP_H_

#include <string>
#include <vector>
#include "../vendor/tinyxml/tinyxml.h"
#include "../ToString.h"
#include "../Graphics/Image/Image.h"
#include "../Graphics/Color.h"

#include "TiledMapTileset.h"
#include "TiledMapProperty.h"
#include "TiledMapLayer.h"
#include "TiledMapObjectGroup.h" 
#include "TiledMapObject.h"

#include "../Resource.h"
using namespace std;

namespace ARK {
	namespace Tiled {

		/*!
		 * \brief Class for managing tiled maps.
		 *
		 * @warning Don't use the constructor of this class to create a tiled map.
		 *
		 * @see Resource class
		 * @see Resource::asTiledMap()
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class TiledMap : public ARK::Resource {

			friend class TiledMapLayer;

			public:
				TiledMap(const string& file);
				TiledMap(const string& file, void* data);
				void addProperty(const TiledMapProperty& property);
				void addTileset(const TiledMapTileset& tileset);
				void addLayer(const TiledMapLayer& layer);
				void addObjectGroup(const TiledMapObjectGroup& objectgroup);

				TiledMapLayer* getLayerByName(const char* name);
				TiledMapLayer* getLayer(unsigned int index);

				TiledMapObjectGroup* getObjectGroupByName(const char* name);
				TiledMapObjectGroup* getObjectGroup(unsigned int index);

				TiledMapTileset* getTileset(unsigned int index);
				TiledMapTileset* getTilesetByGID(unsigned int gid);

				TiledMapProperty* getProperty(unsigned int index);
				TiledMapProperty* getPropertyByName(const char* name);

				unsigned int getTileWidth();
				unsigned int getTileHeight();

				unsigned int getWidth();
				unsigned int getHeight();
				unsigned int getWidthInTiles();
				unsigned int getHeightInTiles();

				void printProperties();
				void draw();
				void draw(signed int x, signed int y);
				virtual ~TiledMap();
			private:
				string m_file;
				TiXmlDocument m_xmldocument;
				void* m_data;
				vector<TiledMapProperty> m_properties;
				vector<TiledMapTileset> m_tilesets;
				vector<TiledMapLayer> m_layers;
				vector<TiledMapObjectGroup> m_objectgroups;
				int m_tileWidth;
				int m_tileHeight;

				unsigned int m_widthInTiles;
				unsigned int m_heightInTiles;
				unsigned int m_widthInPixels;
				unsigned int m_heightInPixels;

				void parse();
				void parseTileset(unsigned int& firstgid, string& src, TiXmlDocument* tileset_d);

		};

	}
}








#endif /* TILEDMAP_H_ */
