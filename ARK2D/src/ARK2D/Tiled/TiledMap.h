/*
 * TiledMap.h
 *
 *  Created on: 23-Apr-2010
 *      Author: Ashley
 */

#ifndef ARK_TILED_TILEDMAP_H_
#define ARK_TILED_TILEDMAP_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "../Common/Libraries/rapidxml.h"
#include "../Common/Libraries/libjson.h"
//#include "../vendor/tinyxml/tinyxml.h"
#include "../Core/ToString.h"
#include "../Core/Resource.h"
#include "../Core/Graphics/Image.h"
#include "../Core/Graphics/Color.h"

#include "TiledMapTileset.h"
#include "TiledMapProperty.h"
#include "TiledMapLayer.h"
#include "TiledMapObjectGroup.h"
#include "TiledMapObject.h"

namespace ARK {
	namespace Tiled {

		class TiledMap;
		class ARK2D_API TiledMapParser {
			public:
				TiledMap* m_map;
				TiledMapParser(TiledMap* map);
				virtual void parse() = 0;
				virtual ~TiledMapParser();
		};

		/*class ARK2D_API TiledMapParser_TinyXml : public TiledMapParser {
			public:
				TiXmlDocument m_xmldocument;
			public:
				TiledMapParser_TinyXml(TiledMap* map, string file);
				virtual void parse();
				void parseTileset(unsigned int& firstgid, string& src, TiXmlDocument* tileset_d);
		};*/
		class ARK2D_API TiledMapParser_RapidXml : public TiledMapParser {
			public:
				xml_document<> m_xmldocument;
				vector<char> m_xml_copy;
				vector<char> m_tileset_xml_temp;
			public:
				TiledMapParser_RapidXml(TiledMap* map);
				virtual void parse();
				void parseTileset(unsigned int& firstgid, string& src, xml_document<>* tileset_d);
		};
		class ARK2D_API TiledMapParser_JSON : public TiledMapParser {
			public:
				JSONNode* m_root;
			public:
				TiledMapParser_JSON(TiledMap* map);
				virtual void parse();
		};


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
		class ARK2D_API TiledMap : public ARK::Core::Resource {

			friend class TiledMapLayer;
			friend class TiledMapParser;
			friend class TiledMapParser_TinyXml;
			friend class TiledMapParser_RapidXml;
			friend class TiledMapParser_JSON;

			public:
				static TiledMap* createFromFile(string ref, bool appendPath=true);

			private:
				TiledMap(const string& file);
				TiledMap(const string& file, void* data);
				void load();

			public:
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

			protected:
				TiledMapParser* m_parser;
				string m_file;
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


		};



	}
}








#endif /* TILEDMAP_H_ */
