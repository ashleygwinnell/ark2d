/*
 * TiledMap.cpp
 *
 *  Created on: 23-Apr-2010
 *      Author: Ashley
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

#include "TiledMap.h"
#include "TiledMapTile.h"

#include "../Core/Resource.h"
#include "../Core/ARK2D.h"
#include "../Core/Controls/ErrorDialog.h"
#include "../Core/Cast.h"
#include "../Core/Graphics/Image.h"
#include "../Core/GameContainer.h"
#include "../Core/Graphics/Renderer.h"

#include "../Util/Log.h"
#include "../Util/StringUtil.h"

#include "../vendor/zlib123/zlib.h"

//#include "zlib123/zutil.h"
//#include "zlib123/deflate.h"

//using std::stringstream;

namespace ARK {
	namespace Tiled {

		TiledMap::TiledMap(const string& file, void* data):
			ARK::Core::Resource(),
			m_parser(NULL),
			m_file(file),
			m_data(data),
			m_properties(),
			m_tilesets(),
			m_layers(),
			m_objectgroups(),
			m_tileWidth(0), 
			m_tileHeight(0),
			m_widthInTiles(0),
			m_heightInTiles(0),
			m_widthInPixels(0),
			m_heightInPixels(0)
		{
			ARK2D::getLog()->i("New TiledMap");

			char firstChar = *((char*) m_data);
			if (StringUtil::str_contains(file, "json") || firstChar == '{') {
				m_parser = new TiledMapParser_JSON(this);
			} else {
				if (ARK2D::getPlatform() == ARK2D::PLATFORM_BROWSER_FLASCC) {
					m_parser = new TiledMapParser_RapidXml(this);
				} else {
					m_parser = new TiledMapParser_TinyXml(this, file);
				}
			}
		}

		TiledMap::TiledMap(const string& file):
			ARK::Core::Resource(),
			m_parser(NULL),
			m_file(file),
			m_data(NULL),
			m_properties(),
			m_tilesets(),
			m_layers(),
			m_objectgroups(),
			m_tileWidth(0), 
			m_tileHeight(0),
			m_widthInTiles(0),
			m_heightInTiles(0),
			m_widthInPixels(0),
			m_heightInPixels(0)
		{
			ARK2D::getLog()->i("New TiledMap");

			if (StringUtil::str_contains(file, "json")) {
				m_parser = new TiledMapParser_JSON(this);
			} else {
				if (ARK2D::getPlatform() == ARK2D::PLATFORM_OSX || ARK2D::getPlatform() == ARK2D::PLATFORM_BROWSER_FLASCC) {
				//if (ARK2D::getPlatform() == ARK2D::PLATFORM_BROWSER_FLASCC) { 
					m_parser = new TiledMapParser_RapidXml(this);
				} else {
					m_parser = new TiledMapParser_TinyXml(this, file);
				}
			}
			
		}

		void TiledMap::load() {
			ARK2D::getLog()->i("Load TiledMap");
			if (m_parser != NULL) {
				m_parser->parse();
			}
		}

		

	

		void TiledMap::addProperty(const TiledMapProperty& property) {
			m_properties.push_back(property);
		}
		void TiledMap::addTileset(const TiledMapTileset& tileset) {
			m_tilesets.push_back(tileset);
		}
		void TiledMap::addLayer(const TiledMapLayer& layer) {
			m_layers.push_back(layer);
		}

		void TiledMap::printProperties() {
			string output;
			output += "[Vector: ";
			for (unsigned int i = 0; i < m_properties.size(); i++) {
				TiledMapProperty t = m_properties.at(i);
				output += t.toString();
				if (i != m_properties.size()-1) {
					output += ", ";
				}
			}
			output += "]";
			ARK2D::getLog()->i(output); //cout << output << endl;
		}

		void TiledMap::draw() {
			draw(0, 0);
		}
		void TiledMap::draw(signed int x, signed int y) {

			//Renderer* g = const_cast<Renderer*>(ARK2D::getContainer()->getRenderer());

			//g->pushMatrix();
			//g->translate(x, y);
			//g->scale(2.0f, 2.0f);

			int xOffset = x;
			int yOffset = y;

			//std::cout << "Before l()" << std::endl;
			//unsigned int count = 0;
			for (signed int l = 0; l < (signed int) m_layers.size(); l++) { // for each layer.
				//std::cout << "Before x()" << std::endl;
				const TiledMapLayer& layer = m_layers.at(l);

				//std::cout << m_layers.at(l).m_data.at(0).size() << std::endl;

				layer.draw(xOffset, yOffset);
				//	std::cout << StringUtil::append("done row ", y) << std::endl;
				//	std::cout << count << std::endl;

			}

			//g->popMatrix();
		}

		TiledMapLayer* TiledMap::getLayerByName(const char* name) {
			vector<TiledMapLayer>::iterator it;
			for (it = m_layers.begin(); it != m_layers.end(); it++) {
				const TiledMapLayer& layer = (*it);
				if (layer.m_name.compare(name) == 0) {
					return const_cast<TiledMapLayer*>(&layer);
				}
			}
			//ErrorDialog::createAndShow(StringUtil::append("There is no Layer by  name: ", name));
			//exit(0);
			return NULL;
		}

		TiledMapLayer* TiledMap::getLayer(unsigned int index) {
			if (index >= m_layers.size()) {
				ErrorDialog::createAndShow(StringUtil::append("There is no Layer at index: ", index));
				return NULL;
			}
			return &m_layers.at(index);
		}

		TiledMapObjectGroup* TiledMap::getObjectGroupByName(const char* name) {
			vector<TiledMapObjectGroup>::iterator it;
			for (it = m_objectgroups.begin(); it != m_objectgroups.end(); it++) {
				const TiledMapObjectGroup& group = (*it);
				if (group.getName().compare(name) == 0) {
					return const_cast<TiledMapObjectGroup*>(&group);
				}
			}
			return NULL;
		}
		TiledMapObjectGroup* TiledMap::getObjectGroup(unsigned int index) {
			if (index >= m_objectgroups.size()) {
				ErrorDialog::createAndShow(StringUtil::append("There is no Object group at index: ", index));
				return NULL;
			}
			return &m_objectgroups.at(index);
		}

		TiledMapTileset* TiledMap::getTileset(unsigned int index) {
			if (index >= m_tilesets.size()) {
				ErrorDialog::createAndShow(StringUtil::append("There is no Tileset at index: ", index));
				return NULL;
			}
			return &m_tilesets.at(index);
		}

		TiledMapProperty* TiledMap::getProperty(unsigned int index) {
			if (index >= m_properties.size()) {
				ErrorDialog::createAndShow(StringUtil::append("There is no TiledMap Property at index: ", index));
				return NULL;
			}
			return &m_properties.at(index);
		}
		TiledMapProperty* TiledMap::getPropertyByName(const char* name) {
			vector<TiledMapProperty>::iterator it;
			for (it = m_properties.begin(); it != m_properties.end(); it++) {
				const TiledMapProperty& p = (*it);
				if (p.getName().compare(name) == 0) {
					return const_cast<TiledMapProperty*>(&p);
				}
			}
			return NULL;
		}


		TiledMapTileset* TiledMap::getTilesetByGID(unsigned int gid) {
			// find which tileset it's in.
			for (unsigned int ts = 0; ts < m_tilesets.size(); ts++)
			{
				TiledMapTileset* tileset = &m_tilesets.at(ts);
				if (tileset != 0)
				{
					// it's in this tileset!!
					if (gid >= tileset->getFirstGID() && gid <= tileset->getLastGID())
					{
						return tileset;
					}
				}
			}
			return NULL;
		}

		unsigned int TiledMap::getTileWidth() {
			return m_tileWidth;
		}
		unsigned int TiledMap::getTileHeight() {
			return m_tileHeight;
		}

		unsigned int TiledMap::getWidth() {
			return m_widthInPixels;
		}
		unsigned int TiledMap::getHeight() {
			return m_heightInPixels;
		}
		unsigned int TiledMap::getWidthInTiles() {
			return m_widthInTiles;
		}
		unsigned int TiledMap::getHeightInTiles() {
			return m_heightInTiles;
		}

		TiledMap::~TiledMap() {

		}


		// --------------------------------------------------------------------------------------------------------
		// parsers
		// --------------------------------------------------------------------------------------------------------
		TiledMapParser::TiledMapParser(TiledMap* map): 
			m_map(map) {

		}
		TiledMapParser::~TiledMapParser() {
			
		}





		// --------------------------------------------------------------------------------------------------------
		// json parser
		// --------------------------------------------------------------------------------------------------------
		TiledMapParser_JSON::TiledMapParser_JSON(TiledMap* map):
			TiledMapParser(map),
			m_root(NULL) {
				ARK2D::getLog()->i("New TiledMapParser_JSON");
		}
		void TiledMapParser_JSON::parse() {
			ARK2D::getLog()->i("TiledMapParser_JSON::parse()");

			if (m_map->m_data != NULL) {
				ARK2D::getLog()->i("Parsing TiledMap from passed-in data.");
				m_root = libJSON::Parse((char*) m_map->m_data);
			} else { 
				ARK2D::getLog()->i(StringUtil::append("Parsing TiledMap from new data from file: ", m_map->m_file));
				string docstr = StringUtil::file_get_contents(m_map->m_file.c_str());
				m_root = libJSON::Parse(docstr.c_str());
			}
			ARK2D::getLog()->i("Root obtained! ");



			ARK2D::getLog()->i(StringUtil::append("Loading TiledMap file: ", m_map->m_file));

			// check version
			if (m_root->GetNode("version")->NodeAsInt() != 1) {
				ErrorDialog::createAndShow(StringUtil::append("Ark 2D only supports Version 1 TilED/JSON Maps: ", m_map->m_file));
				exit(0);
			}
			
			// check orientation
			if (m_root->GetNode("orientation")->NodeAsString() != "orthogonal") {
				ErrorDialog::createAndShow(StringUtil::append("Ark 2D only supports Orthogonal TilED Maps: ", m_map->m_file));
				exit(0);
			}

			// general "stuff"
			m_map->m_tileWidth = m_root->GetNode("tilewidth")->NodeAsInt();
			m_map->m_tileHeight = m_root->GetNode("tileheight")->NodeAsInt();
			m_map->m_widthInTiles = m_root->GetNode("width")->NodeAsInt();
			m_map->m_heightInTiles = m_root->GetNode("height")->NodeAsInt();
			m_map->m_widthInPixels = m_map->m_tileWidth * m_map->m_widthInTiles;
			m_map->m_heightInPixels = m_map->m_tileHeight * m_map->m_heightInTiles;

			// map properties
			JSONNode* propertiesNode = m_root->GetNode("properties");
			for(unsigned int i = 0; i < propertiesNode->NodeSize(); ++i) {
				JSONNode* propertyNode = propertiesNode->NodeAt(i);

				TiledMapProperty property(propertyNode->NodeName(), propertyNode->NodeAsString());
				m_map->addProperty(property);
			}
			//m_map->printProperties();

			// tilesets
			JSONNode* tilesetsNode = m_root->GetNode("tilesets");
			for(unsigned int i = 0; i < tilesetsNode->NodeSize(); ++i) {
				JSONNode* tilesetNode = tilesetsNode->NodeAt(i);

				unsigned int firstgid = tilesetNode->GetNode("firstgid")->NodeAsInt();
				
				TiledMapTileset tileset;
				tileset.setName( tilesetNode->GetNode("name")->NodeAsString() );
				tileset.setFirstGID( firstgid );
				tileset.setTileWidth( tilesetNode->GetNode("tilewidth")->NodeAsInt() );
				tileset.setTileHeight( tilesetNode->GetNode("tileheight")->NodeAsInt() );

				// Margin & Spacing
				tileset.setMargin(0);
				tileset.setSpacing(0);
				if (tilesetNode->GetNode("margin") != NULL ) {
					tileset.setMargin( tilesetNode->GetNode("margin")->NodeAsInt() );
				} 
				if (tilesetNode->GetNode("spacing") != NULL) { 
					tileset.setSpacing( tilesetNode->GetNode("spacing")->NodeAsInt() );
				} 

				// Image
				string image_src = tilesetNode->GetNode("image")->NodeAsString();

				// make sure the tileset is in the same folder as the map.
				string image_path = StringUtil::pathToFile(m_map->m_file);
				if (image_path != image_src) {
					image_src.insert(0, image_path);
				}

				//! @TODO: image mask in resource::get(); -- load image file.
				ARK2D::getLog()->i(StringUtil::append("Loading TiledMap Tileset Image file: ", image_src));
				Image* img_obj = ARK::Core::Resource::get(image_src, true)->asImage();;
				tileset.setImage(img_obj);

				// Calculate number of tiles going down.
				unsigned int current_height = tileset.getImage()->getHeight() - (2*tileset.getMargin());
				current_height -= current_height % (tileset.getTileHeight() + tileset.getSpacing());
				
				unsigned int tiles_down = current_height / (tileset.getTileHeight() + tileset.getSpacing());
				tileset.setNumberOfTilesDown(tiles_down);

				// Calculate number of tiles going across.
				unsigned int current_width = tileset.getImage()->getWidth() - tileset.getMargin();
				unsigned int tiles_across = current_width / (tileset.getTileWidth() + tileset.getSpacing());
				tileset.setNumberOfTilesAcross(tiles_across);

				// set last GID.
				unsigned int last_gid = firstgid + (tiles_across * tiles_down) - 1;
				tileset.setLastGID(last_gid);

				tileset.ready();

				// tile properties.
				JSONNode* tilePropertiesNode = tilesetNode->GetNode("tileproperties");
				if (tilePropertiesNode != NULL) { 
					for(unsigned int j = 0; j < tilePropertiesNode->NodeSize(); ++j) {
						JSONNode* tilePropertyNode = tilePropertiesNode->NodeAt(j);

						unsigned int tid = Cast::fromString<unsigned int>( tilePropertyNode->NodeName() );

						// each property...
						for(unsigned int k = 0; k < tilePropertyNode->NodeSize(); k++) {
							JSONNode* tilePropertySingleNode = tilePropertyNode->NodeAt(k);

							TiledMapProperty* propertyObj = new TiledMapProperty();
							propertyObj->setName(tilePropertySingleNode->NodeName());
							propertyObj->setValue(tilePropertySingleNode->NodeAsString());
							tileset.m_tiles.at(tid)->m_properties.push_back(propertyObj);
						}
					}
				}

				m_map->addTileset(tileset);

				ARK2D::getLog()->i(StringUtil::append("Loaded TiledMap Tileset file: ", "nofile"));

			}

			

			// layers
			JSONNode* layersNode = m_root->GetNode("layers");
			for (unsigned int i = 0; i < layersNode->NodeSize(); ++i) {
				JSONNode* layerNode = layersNode->NodeAt(i);

				string layerType = layerNode->GetNode("type")->NodeAsString();
				if (layerType == "tilelayer") {
					TiledMapLayer layer;
					layer.setMap(m_map);
					layer.setName( layerNode->GetNode("name")->NodeAsString() );
					layer.setWidth( layerNode->GetNode("width")->NodeAsInt() );
					layer.setHeight( layerNode->GetNode("height")->NodeAsInt() );

					layer.setOpacity( layerNode->GetNode("opacity")->NodeAsFloat() );
					layer.setVisible( layerNode->GetNode("visible")->NodeAsBool() );

					layer.prepare();

					unsigned int layer_current_x = 0;
					unsigned int layer_current_y = 0;
				
					JSONNode* layerData = layerNode->GetNode("data");
					for(unsigned int j = 0; j < layerData->NodeSize(); j++) {
						unsigned int gid = layerData->NodeAt(j)->NodeAsInt();
						if (gid != 0) {
							/*String s;
							s += StringUtil::append("x ", layer_current_x);
							s += StringUtil::append("y ", layer_current_y);
							s += StringUtil::append("gid ", gid);
							ARK2D::getLog()->i(s.get());*/
							layer.setTileGID(layer_current_x, layer_current_y, gid);
						}

						layer_current_x++;
						if (layer_current_x >= layer.getWidth()) {
							layer_current_x = 0;
							layer_current_y++;
						}
					}
					ARK2D::getLog()->i("done.");

					if (layer.getWidth() != layer.m_data.size() || layer.getHeight() != layer.m_data.at(0).size()) {
						String s;
						s += StringUtil::append("layer width: ", layer.getWidth());
						s += StringUtil::append(", layer data width: ", layer.m_data.at(0).size());
						s += StringUtil::append(", layer height: ", layer.getHeight());
						s += StringUtil::append(", layer data height: ", layer.m_data.at(0).size());
						string s2 = StringUtil::append("Error Loading tiled map.\nThe width/height was incorrect after populating data.", s.get());
						ErrorDialog::createAndShow(s2);
						exit(0);
					}

					m_map->addLayer(layer);

				} else if (layerType == "objectgroup") {
					TiledMapObjectGroup group;
					group.setName( layerNode->GetNode("name")->NodeAsString() );
					group.setWidth( layerNode->GetNode("width")->NodeAsInt() );
					group.setHeight( layerNode->GetNode("height")->NodeAsInt() );

					group.setOpacity( layerNode->GetNode("opacity")->NodeAsFloat() );
					group.setVisible( layerNode->GetNode("visible")->NodeAsBool() );
				
					JSONNode* objectsNode = layerNode->GetNode("objects");
					for(unsigned int j = 0; j < objectsNode->NodeSize(); j++) {
						JSONNode* objectNode = objectsNode->NodeAt(j);

						TiledMapObject* obj = new TiledMapObject();
						obj->setName( objectNode->GetNode("name")->NodeAsString() );
						obj->setType( objectNode->GetNode("type")->NodeAsString() );

						obj->setX( objectNode->GetNode("x")->NodeAsInt() );
						obj->setY( objectNode->GetNode("y")->NodeAsInt() );

						obj->setGID( objectNode->GetNode("gid")->NodeAsInt() );

						if (objectNode->GetNode("width") != NULL) {
							obj->setWidth( objectNode->GetNode("width")->NodeAsInt() );
						}
 
						if (objectNode->GetNode("height") != NULL) {
							obj->setHeight( objectNode->GetNode("height")->NodeAsInt() );
						}

						JSONNode* objectPropertiesNode = objectNode->GetNode("properties");
						for(unsigned int k = 0; k < objectPropertiesNode->NodeSize(); k++) {
							JSONNode* objectPropertyNode = objectPropertiesNode->NodeAt(k);
							obj->addProperty(objectPropertyNode->NodeName(), objectPropertyNode->NodeAsString());
						}

						// support for polygon lines in new tiled.
						// <polyline points="127,-96 223,-160 319,-96 415,-160"/>
						JSONNode* polylinePointsNode = objectNode->GetNode("polyline");
						if (polylinePointsNode != NULL) { 
							ARK2D::getLog()->v(StringUtil::append("We've got ourselves a polyline with # points: ", polylinePointsNode->NodeSize()));
							for(unsigned int k = 0; k < polylinePointsNode->NodeSize(); k++) {
								JSONNode* pointNode = polylinePointsNode->NodeAt(k);
								obj->addPolylinePoint(pointNode->GetNode("x")->NodeAsInt(), pointNode->GetNode("y")->NodeAsInt());
							}
						} 
 
						for(unsigned int k = 0; k < objectPropertiesNode->NodeSize(); k++) {
							JSONNode* objectPropertyNode = objectPropertiesNode->NodeAt(k);
							obj->addProperty(objectPropertyNode->NodeName(), objectPropertyNode->NodeAsString());
						}

						group.addObject(obj);
					}
					m_map->m_objectgroups.push_back(group);

				}
			}

			ARK2D::getLog()->i("Loading TiledMap... done.");

		}
	







		// --------------------------------------------------------------------------------------------------------
		// tinyxml parser
		// --------------------------------------------------------------------------------------------------------
		TiledMapParser_TinyXml::TiledMapParser_TinyXml(TiledMap* map, string file): 
			TiledMapParser(map), 
			m_xmldocument(file) {
				ARK2D::getLog()->i("New TiledMapParser_TinyXml");
		}
		void TiledMapParser_TinyXml::parseTileset(unsigned int& firstgid, string& src, TiXmlDocument* tileset_d) {
			TiXmlElement* external_tileset_e = tileset_d->FirstChildElement("tileset");
			TiledMapTileset tileset;
			tileset.setName(external_tileset_e->Attribute("name"));
			tileset.setFirstGID(firstgid);
			tileset.setTileWidth(Cast::fromString<unsigned int>(external_tileset_e->Attribute("tilewidth")));
			tileset.setTileHeight(Cast::fromString<unsigned int>(external_tileset_e->Attribute("tileheight")));

			// Margin
			const char* tileset_margin = external_tileset_e->Attribute("margin");
			if (tileset_margin != 0) {
				tileset.setMargin(Cast::fromString<unsigned int>(tileset_margin));
			} else {
				tileset.setMargin(0);
			}

			// Spacing
			const char* tileset_spacing = external_tileset_e->Attribute("spacing");
			if (tileset_spacing != 0) {
				tileset.setSpacing(Cast::fromString<unsigned int>(tileset_spacing));
			} else {
				tileset.setSpacing(0);
			}

			// Image
			TiXmlElement* external_tileset_image_e = external_tileset_e->FirstChildElement("image");
			string image_src = external_tileset_image_e->Attribute("source");

			bool hasImageMask = false;
			string image_mask;
			if (external_tileset_image_e->Attribute("trans") != 0) {
				image_mask = external_tileset_image_e->Attribute("trans");
				hasImageMask = true;
			}

			// this piece makes sure the tileset is in the same folder as the map.
			string image_path = StringUtil::pathToFile(src);
			if (image_path != image_src) {
				image_src.insert(0, image_path);
			}

			ARK2D::getLog()->i(StringUtil::append("Loading TiledMap Tileset Image file: ", image_src));
			Image* img_obj = NULL;
			if (!hasImageMask) {
				//img_obj = new Image(image_src);
				img_obj = ARK::Core::Resource::get(image_src, false)->asImage();
			} else {
				//img_obj = new Image(image_src, Color(StringUtil::append("#", image_mask)));
				img_obj = ARK::Core::Resource::get(image_src, false)->asImage();
				//! @TODO: image mask in resource::get();
			}
			tileset.setImage(img_obj);



			// 86 x 69
			// margin 1
			// 69-2
			// 67
			// 51
			// Calculate number of tiles going down.
			unsigned int current_height = tileset.getImage()->getHeight() - (2*tileset.getMargin());
			//std::cout << "image height: " << tileset.getImage()->getHeight() << std::endl;
			//std::cout << "tileset margin: " << tileset.getMargin() << std::endl;
			//std::cout << "current height: " << current_height << std::endl;
			
			current_height -= current_height % (tileset.getTileHeight() + tileset.getSpacing());
			//std::cout << "current height: " << current_height << std::endl;


			unsigned int tiles_down = current_height / (tileset.getTileHeight() + tileset.getSpacing());
			tileset.setNumberOfTilesDown(tiles_down);

			//std::cout << "tile height: " << tileset.getTileHeight() << std::endl;
			//std::cout << "tile spacing: " << tileset.getSpacing() << std::endl;
			//std::cout << "Tileset height: " << tiles_down << std::endl;


			// Calculate number of tiles going across.
			unsigned int current_width = tileset.getImage()->getWidth() - tileset.getMargin();
			unsigned int tiles_across = current_width / (tileset.getTileWidth() + tileset.getSpacing());
			tileset.setNumberOfTilesAcross(tiles_across);

			// set last GID.
			unsigned int last_gid = firstgid + (tiles_across * tiles_down) - 1;
			tileset.setLastGID(last_gid);

			//#ifdef ARK2D_VERBOSE
			//std::cout << "Before ready()" << std::endl;
			//#endif

			// ready for stuff, woo.
			tileset.ready();

			//std::cout << "After ready()" << std::endl;

			// add tile properties O_O
			TiXmlElement* tileset_property_e = 0;
			for (tileset_property_e = external_tileset_e->FirstChildElement("tile");
					tileset_property_e;
					tileset_property_e = tileset_property_e->NextSiblingElement("tile"))
			{
				unsigned int tid = Cast::fromString<unsigned int>( tileset_property_e->Attribute("id") );
				//std::cout << "tid: " << tid << std::endl;

				TiXmlElement* tileset_property_set_e = tileset_property_e->FirstChildElement("properties");

				TiXmlElement* tileset_property_single_e = 0;
				for (tileset_property_single_e = tileset_property_set_e->FirstChildElement("property");
					tileset_property_single_e;
					tileset_property_single_e = tileset_property_single_e->NextSiblingElement("property"))
				{
					//std::cout << "got here" << std::endl;
					TiledMapProperty* propertyObj = new TiledMapProperty();
					propertyObj->setName(tileset_property_single_e->Attribute("name"));
					propertyObj->setValue(tileset_property_single_e->Attribute("value"));
					//std::cout << "got here 2" << std::endl;

					tileset.m_tiles.at(tid)->m_properties.push_back(propertyObj);
					//std::cout << "got here 3" << std::endl;
				}

			}

			// add it, finally.
			m_map->addTileset(tileset);

			ARK2D::getLog()->i(StringUtil::append("Loaded TiledMap Tileset file: ", src));

		}
		void TiledMapParser_TinyXml::parse() {
			if (m_map->m_data != NULL) { 
				m_xmldocument.Parse((const char*)m_map->m_data, 0, TIXML_ENCODING_UTF8);
			} else { 
				bool loadOkay = m_xmldocument.LoadFile();
				if (!loadOkay) {
					ErrorDialog::createAndShow(StringUtil::append("Could not load Tiled Map: ", m_map->m_file));
					exit(0);
				}
			}

			ARK2D::getLog()->i(StringUtil::append("Loading TiledMap file: ", m_map->m_file));

			const char* version;
			const char* orientation;
			TiXmlNode* mapnode = m_xmldocument.FirstChild("map");
			TiXmlElement* mapelement = mapnode->ToElement();
			version = mapelement->Attribute("version");
			orientation = mapelement->Attribute("orientation");
			if (strcmp(orientation, "orthogonal") != 0) {
				ErrorDialog::createAndShow(StringUtil::append("Ark 2D only supports Orthogonal TilED Maps: ", m_map->m_file));
				exit(0);
			}
			m_map->m_tileWidth = Cast::fromString<unsigned int>(mapelement->Attribute("tilewidth"));
			m_map->m_tileHeight = Cast::fromString<unsigned int>(mapelement->Attribute("tileheight"));
			m_map->m_widthInTiles = Cast::fromString<unsigned int>(mapelement->Attribute("width"));
			m_map->m_heightInTiles = Cast::fromString<unsigned int>(mapelement->Attribute("height"));
			m_map->m_widthInPixels = m_map->m_tileWidth * m_map->m_widthInTiles;
			m_map->m_heightInPixels = m_map->m_tileHeight * m_map->m_heightInTiles;

			// Properties
			TiXmlNode* propertiesnode = mapelement->FirstChild("properties");
			if (propertiesnode != NULL) {
				TiXmlElement* propertieselement = propertiesnode->ToElement();
				TiXmlElement* property_e = 0;
				for (property_e = propertieselement->FirstChild("property")->ToElement(); property_e;
									property_e = property_e->NextSiblingElement()) {
					TiledMapProperty property(property_e->Attribute("name"), property_e->Attribute("value"));
					m_map->addProperty(property);
				}
				//this->printProperties();
			}


			// TileSets
			TiXmlElement* tileset_e = 0;
			for (tileset_e = mapelement->FirstChildElement("tileset"); tileset_e;
							tileset_e = tileset_e->NextSiblingElement("tileset")) {
				unsigned int firstgid = Cast::fromString<unsigned int>(tileset_e->Attribute("firstgid"));
				const char* src_cstr = tileset_e->Attribute("source");

				if (src_cstr == 0) {
					ErrorDialog::createAndShow(StringUtil::append("Ark 2D does not support inline Tileset data: ", m_map->m_file));
					exit(0);
				}

				string src(src_cstr);

				string path = StringUtil::pathToFile(m_map->m_file); // this piece makes sure the tileset is in the same folder as the map.
				if (path != m_map->m_file) {
					src.insert(0, path);
				}

				ARK2D::getLog()->i(StringUtil::append("Loading TiledMap Tileset file: ", src));

				if (m_map->m_data == NULL) {
					//std::cout << src << std::endl;
					TiXmlDocument tileset_d(src);
					bool ts_loadOkay = tileset_d.LoadFile();
					if (!ts_loadOkay) {
						ErrorDialog::createAndShow(StringUtil::append("Could not load Tiled Tileset: ", src));
						exit(0);
					} else {
						parseTileset(firstgid, src, &tileset_d);
					}
				} else {
					#if !defined(ARK2D_ANDROID)
						//! @TODO: Read tiledmap from data buffer from non-android.
						ErrorDialog::createAndShow("Could not get tiled map because reading from data buffer and not on android. ");
						exit(0);
					#endif

					TiXmlDocument tileset_d;
					ARK::Core::RawDataReturns* ret = ARK::Core::Resource::getRawData(src);
					tileset_d.Parse((const char*)ret->data, 0, TIXML_ENCODING_UTF8);
					parseTileset(firstgid, src, &tileset_d);
				}


			}


			//unsigned int count = 0;

			// layers please!
			TiXmlElement* layer_element = 0;
			for (layer_element = mapelement->FirstChildElement("layer");
							layer_element;
							layer_element = layer_element->NextSiblingElement("layer")) {
				TiledMapLayer layer;
				layer.setMap(m_map);
				layer.setName(layer_element->Attribute("name"));
				layer.setWidth(Cast::fromString<unsigned int>(layer_element->Attribute("width")));
				layer.setHeight(Cast::fromString<unsigned int>(layer_element->Attribute("height")));

				const char* opacity_cstr = layer_element->Attribute("opacity");
				if (opacity_cstr != NULL) {
					float opac = Cast::fromString<float>(layer_element->Attribute("opacity"));
					layer.setOpacity(opac);
				}


				const char* visible_cstr = layer_element->Attribute("visible");
				if (visible_cstr != NULL) {
					unsigned int visible = Cast::fromString<unsigned int>(visible_cstr);
					if (visible == 0) {
						layer.setVisible(false);
					}
				}


				//std::cout << "Before Prepare()" << std::endl;



				layer.prepare();

		//		std::cout << "After Prepare()" << std::endl;

				// do all of that data stuff
				TiXmlElement* data_element = layer_element->FirstChildElement("data");


				const char* encodingStr = data_element->Attribute("encoding");
				//std::cout << "derp: " << encodingStr << std::endl;
				if (encodingStr != NULL && string(encodingStr).compare("csv") == 0) {

					ARK2D::getLog()->v("Tiled CSV Map");

					const char* data_c_str = data_element->GetText();
 
					vector<string> lines = StringUtil::split(string(data_c_str), " ");
					if (lines.size() > layer.getHeight()) {
						ErrorDialog::createAndShow("Tiled CSV. Layer data height larger than defined height.");
						exit(0);
					}

					for (unsigned int thisy = 0; thisy < lines.size(); ++thisy) 
					{
						ARK2D::getLog()->v(StringUtil::append("Line: ", lines.at(thisy)));
						vector<string> lineItems = StringUtil::split(lines.at(thisy), ",");
						for (unsigned int thisx = 0; thisx < layer.getWidth(); ++thisx) 
						{
							unsigned int tile_gid_int = Cast::fromString<unsigned int>(lineItems.at(thisx));
							if (tile_gid_int != 0) {
								layer.setTileGID(thisx, thisy, tile_gid_int);
							}
						}
					}
  
					ARK2D::getLog()->i("done.");

					if (layer.getWidth() != layer.m_data.size()
							|| layer.getHeight() != layer.m_data.at(0).size()) {
						String s;
						s += StringUtil::append("layer width: ", layer.getWidth());
						s += StringUtil::append(", layer data width: ", layer.m_data.at(0).size());
						s += StringUtil::append(", layer height: ", layer.getHeight());
						s += StringUtil::append(", layer data height: ", layer.m_data.at(0).size());
						string s2 = StringUtil::append("Error Loading tiled map.\nThe width/height was incorrect after populating data.", s.get());
						ErrorDialog::createAndShow(s2);
						exit(0);
					}

				} else if (encodingStr == NULL) { 


					const char* data_c_str = data_element->GetText();
					//std::cout << data_c_str << std::endl;
					if (data_c_str == 0) {
						if (!data_element->NoChildren()) { // does have children, it's pure xml.

							//std::cout << "Befpre setting tile gids()" << std::endl;

							ARK2D::getLog()->i("Loading TiledMap data... ");

							// for each tile...
							unsigned int layer_current_x = 0;
							unsigned int layer_current_y = 0;
							TiXmlElement* tile_element = 0;
							for (tile_element = data_element->FirstChildElement("tile");
									tile_element;
									tile_element = tile_element->NextSiblingElement("tile")) {

								const char* tile_gid = tile_element->Attribute("gid");
								unsigned int tile_gid_int = Cast::fromString<unsigned int>(tile_gid);

								if (tile_gid_int != 0) {
									layer.setTileGID(layer_current_x, layer_current_y, tile_gid_int);
								}
								//std::cout << tile_gid_int << std::endl;

								layer_current_x++;
								if (layer_current_x >= layer.getWidth()) {
									layer_current_x = 0;
									layer_current_y++;
								}
								//count++;
								//std::cout << count << std::endl;
							}

							ARK2D::getLog()->i("done.");


							//std::cout << "layer width: " << layer.getWidth() << std::endl;
							//std::cout << "layer height: " << layer.getHeight() << std::endl;

							//std::cout << "layer width: " << layer.m_data.size() << std::endl;
							//std::cout << "layer height: " << layer.m_data.at(0).size() << std::endl;

							if (layer.getWidth() != layer.m_data.size()
									|| layer.getHeight() != layer.m_data.at(0).size()) {
								String s;
								s += StringUtil::append("layer width: ", layer.getWidth());
								s += StringUtil::append(", layer data width: ", layer.m_data.at(0).size());
								s += StringUtil::append(", layer height: ", layer.getHeight());
								s += StringUtil::append(", layer data height: ", layer.m_data.at(0).size());
								string s2 = StringUtil::append("Error Loading tiled map.\nThe width/height was incorrect after populating data.", s.get());
								ErrorDialog::createAndShow(s2);
								exit(0);
							}

							//std::cout << "After setting tile gids()" << std::endl;

						} else {
							ErrorDialog::createAndShow(StringUtil::append("There is no tile data in one of your TilED layers: ", m_map->m_file));
							exit(0);
						}
					}
					else {

						string data(data_c_str);
						//std::cout << data << std::endl;

						const char* layer_encoding = data_element->Attribute("encoding");
						if (layer_encoding == 0 ) { layer_encoding = ""; }
						if (strcmp(layer_encoding, "base64") == 0) {
							data = StringUtil::decodeBase64(data);
						}

						const char* layer_compression = data_element->Attribute("compression");
						if (layer_compression == 0) { layer_compression = ""; }
						if (strcmp(layer_compression, "gzip") == 0) {
							ErrorDialog::createAndShow(StringUtil::append("Ark 2D only supports TilED Maps without gzip compression on layers. ", m_map->m_file));
							exit(0);
						}
					}

					//layer.prepare(); // the array resizes itself after assign()ing stuff, so re set the size! D:
				}

				m_map->addLayer(layer);
			}

			// read in object groups.
			TiXmlElement* objectgroup_element = 0;
			for (objectgroup_element = mapelement->FirstChildElement("objectgroup");
					objectgroup_element;
					objectgroup_element = objectgroup_element->NextSiblingElement("objectgroup")) {
				TiledMapObjectGroup group;
				group.setName(objectgroup_element->Attribute("name"));
				group.setWidth(Cast::fromString<unsigned int>(objectgroup_element->Attribute("width")));
				group.setHeight(Cast::fromString<unsigned int>(objectgroup_element->Attribute("height")));

				const char* visible_cstr = objectgroup_element->Attribute("visible");
				if (visible_cstr != NULL) {
					unsigned int visible = Cast::fromString<unsigned int>(visible_cstr);
					if (visible == 0) {
						group.setVisible(false);
					}
				}

				TiXmlElement* object_element = 0;
				for (object_element = objectgroup_element->FirstChildElement("object");
						object_element;
						object_element = object_element->NextSiblingElement("object")) {

					TiledMapObject* obj = new TiledMapObject();

					const char* object_name = object_element->Attribute("name");
					if (object_name != NULL) {
						obj->setName(object_name);
					}

					const char* object_type = object_element->Attribute("type");
					if (object_type != NULL) {
						obj->setType(object_type);
					}

					signed int object_x = Cast::fromString<signed int>(object_element->Attribute("x"));
					signed int object_y = Cast::fromString<signed int>(object_element->Attribute("y"));
					obj->setX(object_x);
					obj->setY(object_y);

					if (object_element->Attribute("width") != NULL) {
						obj->setWidth( Cast::fromString<unsigned int>(object_element->Attribute("width")) );
					}
					if (object_element->Attribute("height") != NULL) {
						obj->setHeight( Cast::fromString<unsigned int>(object_element->Attribute("height")) );
					}

					const char* gid_cstr = object_element->Attribute("gid");
					if (gid_cstr != NULL) {
						int gid = Cast::fromString<signed int>(gid_cstr);
						obj->setGID(gid);
					}

					// add object properties
					TiXmlElement* object_properties_root = NULL;
					object_properties_root = object_element->FirstChildElement("properties");

					if (object_properties_root != NULL) { 

						TiXmlElement* object_property = NULL;
						for (object_property = object_properties_root->FirstChildElement("property");
								object_property;
								object_property = object_property->NextSiblingElement("property"))
						{
							obj->addProperty(object_property->Attribute("name"), object_property->Attribute("value"));
						}

					}

					// support for polygon lines in new tiled.
					// <polyline points="127,-96 223,-160 319,-96 415,-160"/>
					TiXmlElement* polyline_root = NULL;
					polyline_root = object_element->FirstChildElement("polyline");
					if (polyline_root != NULL)
					{
						string polylinePoints = string(polyline_root->Attribute("points"));
						vector<string> pointsvec = StringUtil::split(polylinePoints, " ");

						ARK2D::getLog()->v(StringUtil::append("We've got ourselves a polyline with # points: ", pointsvec.size()));
						for(unsigned int k = 0; k < pointsvec.size(); k++) {
							vector<string> pointsActual = StringUtil::split(pointsvec.at(k), ",");
							int pointx = Cast::fromString<int>( pointsActual.at(0) );
							int pointy = Cast::fromString<int>( pointsActual.at(1) );
							obj->addPolylinePoint(pointx, pointy);
						}
					}

					group.addObject(obj);
				}

				m_map->m_objectgroups.push_back(group);

			}
			ARK2D::getLog()->i("Loading TiledMap... done.");

		}



















		// --------------------------------------------------------------------------------------------------------
		// rapidxml
		// --------------------------------------------------------------------------------------------------------
		TiledMapParser_RapidXml::TiledMapParser_RapidXml(TiledMap* map):
			TiledMapParser(map),
			m_xmldocument(),
			m_xml_copy(),
			m_tileset_xml_temp() {
				ARK2D::getLog()->i("New TiledMapParser_RapidXml");

		}
		void TiledMapParser_RapidXml::parseTileset(unsigned int& firstgid, string& src, xml_document<>* tileset_d) 
		{
			//xml_node<>* mapnode = m_xmldocument.first_node("map");
			//xml_attribute<>* attr_version = mapnode->first_attribute("version");

			xml_node<>* external_tileset_e = tileset_d->first_node("tileset");
			
			TiledMapTileset tileset;
			tileset.setName(external_tileset_e->first_attribute("name")->value());
			tileset.setFirstGID(firstgid);
			tileset.setTileWidth(Cast::fromString<unsigned int>(external_tileset_e->first_attribute("tilewidth")->value()));
			tileset.setTileHeight(Cast::fromString<unsigned int>(external_tileset_e->first_attribute("tileheight")->value()));

			// Margin
			xml_attribute<>* tileset_margin = external_tileset_e->first_attribute("margin");
			if (tileset_margin != 0) {
				tileset.setMargin(Cast::fromString<unsigned int>(tileset_margin->value()));
			} else {
				tileset.setMargin(0);
			}

			// Spacing
			xml_attribute<>* tileset_spacing = external_tileset_e->first_attribute("spacing");
			if (tileset_spacing != 0) {
				tileset.setSpacing(Cast::fromString<unsigned int>(tileset_spacing->value()));
			} else {
				tileset.setSpacing(0);
			}

			// Image
			xml_node<>* external_tileset_image_e = external_tileset_e->first_node("image");
			string image_src = external_tileset_image_e->first_attribute("source")->value();

			bool hasImageMask = false;
			string image_mask;
			if (external_tileset_image_e->first_attribute("trans") != 0) {
				image_mask = external_tileset_image_e->first_attribute("trans")->value();
				hasImageMask = true;
			}

			// this piece makes sure the tileset is in the same folder as the map.
			string image_path = StringUtil::pathToFile(src);
			if (image_path != image_src) {
				image_src.insert(0, image_path);
			}

			ARK2D::getLog()->i(StringUtil::append("Loading TiledMap Tileset Image file: ", image_src));
			Image* img_obj = NULL;
			if (!hasImageMask) {
				//img_obj = new Image(image_src);
				img_obj = ARK::Core::Resource::get(image_src, false)->asImage();
			} else {
				//img_obj = new Image(image_src, Color(StringUtil::append("#", image_mask)));
				img_obj = ARK::Core::Resource::get(image_src, false)->asImage();
				//! @TODO: image mask in resource::get();
			}
			tileset.setImage(img_obj);



			// Calculate number of tiles going down.
			unsigned int current_height = tileset.getImage()->getHeight() - (2*tileset.getMargin());
			current_height -= current_height % (tileset.getTileHeight() + tileset.getSpacing());
			

			unsigned int tiles_down = current_height / (tileset.getTileHeight() + tileset.getSpacing());
			tileset.setNumberOfTilesDown(tiles_down);
			

			// Calculate number of tiles going across.
			unsigned int current_width = tileset.getImage()->getWidth() - tileset.getMargin();
			unsigned int tiles_across = current_width / (tileset.getTileWidth() + tileset.getSpacing());
			tileset.setNumberOfTilesAcross(tiles_across);

			// set last GID.
			unsigned int last_gid = firstgid + (tiles_across * tiles_down) - 1;
			tileset.setLastGID(last_gid);

			//#ifdef ARK2D_VERBOSE
			ARK2D::getLog()->i("Before ready()");
			//#endif

			// ready for stuff, woo.
			tileset.ready();

			ARK2D::getLog()->i("After ready()");

			// add tile properties O_O
			xml_node<>* tileset_property_e = 0;
			for (tileset_property_e = external_tileset_e->first_node("tile");
					tileset_property_e;
					tileset_property_e = tileset_property_e->next_sibling("tile"))
			{
				unsigned int tid = Cast::fromString<unsigned int>( tileset_property_e->first_attribute("id")->value() );
				//std::cout << "tid: " << tid << std::endl;

				xml_node<>* tileset_property_set_e = tileset_property_e->first_node("properties");

				xml_node<>* tileset_property_single_e = 0;
				for (tileset_property_single_e = tileset_property_set_e->first_node("property");
					tileset_property_single_e;
					tileset_property_single_e = tileset_property_single_e->next_sibling("property"))
				{
					//std::cout << "got here" << std::endl;
					TiledMapProperty* propertyObj = new TiledMapProperty();
					propertyObj->setName(tileset_property_single_e->first_attribute("name")->value());
					propertyObj->setValue(tileset_property_single_e->first_attribute("value")->value());
					//std::cout << "got here 2" << std::endl;

					tileset.m_tiles.at(tid)->m_properties.push_back(propertyObj);
					//std::cout << "got here 3" << std::endl;
				}

			} 

			//std::cout << "After ready()" << std::endl;

			// add it, finally.
			m_map->addTileset(tileset);

			ARK2D::getLog()->i(StringUtil::append("Loaded TiledMap Tileset file: ", src));
		}
		#if defined(ARK2D_ANDROID)
			void TiledMapParser_RapidXml::parse() {
				ARK2D::getLog()->e("Rapid xml not implemented android.");
			}
		#else 
			void TiledMapParser_RapidXml::parse() {

				if (m_map->m_data != NULL) {
					m_xmldocument.parse<0>((char*)m_map->m_data); 
				} else { 

					string docstr = StringUtil::file_get_contents(m_map->m_file.c_str());
					ARK2D::getLog()->i(StringUtil::append("data: ", docstr));

				    // make a safe-to-modify copy of input_xml
				    // (you should never modify the contents of an std::string directly)
				    m_xml_copy = vector<char>(docstr.begin(), docstr.end());
				    m_xml_copy.push_back('\0');

				    ARK2D::getLog()->i("document.parse");
					m_xmldocument.parse<0>((char*) &m_xml_copy[0]);
					bool loadOkay = true; 

					if (!loadOkay) {
						ErrorDialog::createAndShow(StringUtil::append("Could not load Tiled Map: ", m_map->m_file));
						exit(0);
					}
				}

				ARK2D::getLog()->i(StringUtil::append("Loading TiledMap file: ", m_map->m_file));

				xml_node<>* mapnode = m_xmldocument.first_node("map");
				//xml_attribute<>* attr_version = mapnode->first_attribute("version");

				//const char* version = mapnode->first_attribute("version")->value();
				const char* orientation = mapnode->first_attribute("orientation")->value();

				if (strcmp(orientation, "orthogonal") != 0) {
					ErrorDialog::createAndShow(StringUtil::append("Ark 2D only supports Orthogonal TilED Maps: ", m_map->m_file));
					exit(0);
				} 

				m_map->m_tileWidth = Cast::fromString<unsigned int>(mapnode->first_attribute("tilewidth")->value());
				m_map->m_tileHeight = Cast::fromString<unsigned int>(mapnode->first_attribute("tileheight")->value());
				m_map->m_widthInTiles = Cast::fromString<unsigned int>(mapnode->first_attribute("width")->value());
				m_map->m_heightInTiles = Cast::fromString<unsigned int>(mapnode->first_attribute("height")->value());
				 
				m_map->m_widthInPixels = m_map->m_tileWidth * m_map->m_widthInTiles;
				m_map->m_heightInPixels = m_map->m_tileHeight * m_map->m_heightInTiles;

				// Properties
				xml_node<>* propertiesnode = mapnode->first_node("properties");
				if (propertiesnode != NULL) 
				{
					xml_node<>* property_e = 0;
					for (property_e = propertiesnode->first_node("property"); 
							property_e;
							property_e = property_e->next_sibling("property")) 
					{
						TiledMapProperty property(property_e->first_attribute("name")->value(), property_e->first_attribute("value")->value());
						m_map->addProperty(property);
					}
				}
				m_map->printProperties();

				// TileSets
				xml_node<>* tileset_e = 0;
				for (tileset_e = mapnode->first_node("tileset"); 
					tileset_e;
					tileset_e = tileset_e->next_sibling("tileset")) 
				{
					unsigned int firstgid = Cast::fromString<unsigned int>(tileset_e->first_attribute("firstgid")->value());
					const char* src_cstr = tileset_e->first_attribute("source")->value();

					if (src_cstr == 0) {
						ErrorDialog::createAndShow(StringUtil::append("Ark 2D does not support inline Tileset data: ", m_map->m_file));
						exit(0);
					}

					string src(src_cstr);

					string path = StringUtil::pathToFile(m_map->m_file); // this piece makes sure the tileset is in the same folder as the map.
					if (path != m_map->m_file) {
						src.insert(0, path);
					}

					ARK2D::getLog()->i(StringUtil::append("Loading TiledMap Tileset file: ", src));

					if (m_map->m_data == NULL) {

						string tilesetStr = StringUtil::file_get_contents(src.c_str());
						m_tileset_xml_temp = vector<char>(tilesetStr.begin(), tilesetStr.end());
			   			m_tileset_xml_temp.push_back('\0');

						xml_document<> tileset_d;
						tileset_d.parse<0>(&m_tileset_xml_temp[0]);

						parseTileset(firstgid, src, &tileset_d);
						
					} else {
						#if !defined(ARK2D_ANDROID)
							//! @TODO: Read tiledmap from data buffer from non-android.
							ErrorDialog::createAndShow("Could not get tiled map because reading from data buffer and not on android. ");
							exit(0);
						#endif

						ARK::Core::RawDataReturns* ret = ARK::Core::Resource::getRawData(src);
						string tilesetStr = string((const char*)ret->data);
						m_tileset_xml_temp = vector<char>(tilesetStr.begin(), tilesetStr.end());
			   			m_tileset_xml_temp.push_back('\0');

						xml_document<> tileset_d;
						tileset_d.parse<0>(&m_tileset_xml_temp[0]);
						
						parseTileset(firstgid, src, &tileset_d);
					}


				}


				// layers please!
				xml_node<>* layer_element = 0;
				for (layer_element = mapnode->first_node("layer");
					layer_element;
					layer_element = layer_element->next_sibling("layer")) 
				{
					TiledMapLayer layer;
					layer.setMap(m_map);
					layer.setName(layer_element->first_attribute("name")->value());
					layer.setWidth(Cast::fromString<unsigned int>(layer_element->first_attribute("width")->value()));
					layer.setHeight(Cast::fromString<unsigned int>(layer_element->first_attribute("height")->value()));

					xml_attribute<>* opacity_attr = layer_element->first_attribute("opacity");
					if (opacity_attr != NULL) {
						float opac = Cast::fromString<float>(opacity_attr->value());
						layer.setOpacity(opac);
					}


					xml_attribute<>* visible_attr = layer_element->first_attribute("visible");
					if (visible_attr != NULL) {
						unsigned int visible = Cast::fromString<unsigned int>(visible_attr->value());
						if (visible == 0) {
							layer.setVisible(false);
						}
					}

					std::cout << "Before Prepare()" << std::endl;
					layer.prepare();
					std::cout << "After Prepare()" << std::endl;

					// do all of that data stuff
					xml_node<>* data_element = layer_element->first_node("data");

					xml_attribute<>* encodingStr = data_element->first_attribute("encoding");
					//std::cout << "derp: " << encodingStr << std::endl;
					
					if (encodingStr != NULL && string(encodingStr->value()).compare("csv") == 0) 
					{
						ARK2D::getLog()->v("Tiled CSV Map");

						const char* data_c_str = data_element->value();
	 					string thisdatastr = string(data_c_str);

						vector<string> lines = StringUtil::split(thisdatastr, "\n");
						for(unsigned int doline = 0; doline < lines.size(); doline++) {
							if (lines.at(doline).length() == 0) {
								lines.erase(lines.begin() + doline);
							}
						}
						if (lines.size() > layer.getHeight()) {
							ErrorDialog::createAndShow("Tiled CSV. Layer data height larger than defined height.");
							exit(0);
						}

						for (unsigned int thisy = 0; thisy < lines.size() && thisy < layer.getHeight(); ++thisy) 
						{
							ARK2D::getLog()->v(StringUtil::append("Line: ", lines.at(thisy)));
							vector<string> lineItems = StringUtil::split(lines.at(thisy), ",");
							for (unsigned int thisx = 0; thisx < layer.getWidth(); ++thisx) 
							{
								unsigned int tile_gid_int = Cast::fromString<unsigned int>(lineItems.at(thisx));
								if (tile_gid_int != 0) {
									layer.setTileGID(thisx, thisy, tile_gid_int);
								}
							}
						}
	  
						ARK2D::getLog()->i("done.");

						if (layer.getWidth() != layer.m_data.size()
								|| layer.getHeight() != layer.m_data.at(0).size()) {
							String s;
							s += StringUtil::append("layer width: ", layer.getWidth());
							s += StringUtil::append(", layer data width: ", layer.m_data.at(0).size());
							s += StringUtil::append(", layer height: ", layer.getHeight());
							s += StringUtil::append(", layer data height: ", layer.m_data.at(0).size());
							string s2 = StringUtil::append("Error Loading tiled map.\nThe width/height was incorrect after populating data.", s.get());
							ErrorDialog::createAndShow(s2);
							exit(0);
						}

					} 
					else if (encodingStr != NULL && string(encodingStr->value()).compare("base64") == 0) 
					{ 
						//std::cout << data_c_str << std::endl;
							
						const char* data_c_str = data_element->value();
						string data(data_c_str);
						data = StringUtil::decodeBase64(data);
						
						xml_attribute<>* layer_compression = data_element->first_attribute("compression");
						if (layer_compression != NULL) { 
							if (strcmp(layer_compression->value(), "gzip") == 0) 
							{
								ErrorDialog::createAndShow(StringUtil::append("Ark 2D only supports TilED Maps without gzip compression on layers. ", m_map->m_file));
								exit(0);

								// std::string compressed = data;
								// uLongf outputSize = data.length() * 10;
								// std::vector<Bytef> output(outputSize);
								// const Bytef *input = reinterpret_cast<const Bytef*>(compressed.c_str());

								// int result;
								// while((result = uncompress(&output.front(), &outputSize, input, compressed.length())) == Z_BUF_ERROR)
								// {
								//    outputSize *= 2;
								//    output.resize(outputSize);
								// }
								// std::cout << ((int) output.at(1)) << std::endl;
				
								// std::cout << decoded << std::endl;
								// int err;
								// unsigned long RawDataLength = decoded.length();

								// void *RawDataBuff = malloc(decoded.length());
								// memcpy(RawDataBuff, decoded.c_str(), decoded.length());

								// void *UnCompDataBuff = NULL;
								// uLongf UnCompSize = 482000;
								// UnCompDataBuff = malloc(UnCompSize);

								// err = uncompress((Bytef*)UnCompDataBuff, &UnCompSize, (const Bytef*) RawDataBuff, RawDataLength);
								// if (err != Z_OK) {
								// 	ErrorDialog::createAndShow(StringUtil::append("Error: ", err));
								// }
								//std::cout << uncomp << std::endl;
							}
						}
					
					} 
					else if (encodingStr == NULL) 
					{ 


						const char* data_c_str = data_element->value();
						//std::cout << data_c_str << std::endl;
						if (data_c_str != 0) {
							if (data_element->first_node("tile") != NULL) { // does have children, it's pure xml.

								//std::cout << "Befpre setting tile gids()" << std::endl;

								ARK2D::getLog()->i("Loading TiledMap data... ");

								// for each tile...
								unsigned int layer_current_x = 0;
								unsigned int layer_current_y = 0;
								xml_node<>* tile_element = 0;
								for (tile_element = data_element->first_node("tile");
										tile_element;
										tile_element = tile_element->next_sibling("tile")) {

									const char* tile_gid = tile_element->first_attribute("gid")->value();
									unsigned int tile_gid_int = Cast::fromString<unsigned int>(tile_gid);

									if (tile_gid_int != 0) {
										layer.setTileGID(layer_current_x, layer_current_y, tile_gid_int);
									}
									//std::cout << tile_gid_int << std::endl;

									layer_current_x++;
									if (layer_current_x >= layer.getWidth()) {
										layer_current_x = 0;
										layer_current_y++;
									}
									//count++;
									//std::cout << count << std::endl;
								}

								ARK2D::getLog()->i("done.");


								//std::cout << "layer width: " << layer.getWidth() << std::endl;
								//std::cout << "layer height: " << layer.getHeight() << std::endl;

								//std::cout << "layer width: " << layer.m_data.size() << std::endl;
								//std::cout << "layer height: " << layer.m_data.at(0).size() << std::endl;

								if (layer.getWidth() != layer.m_data.size()
										|| layer.getHeight() != layer.m_data.at(0).size()) {
									String s;
									s += StringUtil::append("layer width: ", layer.getWidth());
									s += StringUtil::append(", layer data width: ", layer.m_data.at(0).size());
									s += StringUtil::append(", layer height: ", layer.getHeight());
									s += StringUtil::append(", layer data height: ", layer.m_data.at(0).size());
									string s2 = StringUtil::append("Error Loading tiled map.\nThe width/height was incorrect after populating data.", s.get());
									ErrorDialog::createAndShow(s2);
									exit(0);
								}

								//std::cout << "After setting tile gids()" << std::endl;

							} else {
								ErrorDialog::createAndShow(StringUtil::append("There is no tile data in one of your TilED layers: ", m_map->m_file));
								exit(0);
							}
						}
						

						//layer.prepare(); // the array resizes itself after assign()ing stuff, so re set the size! D:
					}

					m_map->addLayer(layer);
				}

				// read in object groups.
				xml_node<>* objectgroup_element = 0;
				for (objectgroup_element = mapnode->first_node("objectgroup");
						objectgroup_element;
						objectgroup_element = objectgroup_element->next_sibling("objectgroup")) 
				{
					TiledMapObjectGroup group;
					group.setName(objectgroup_element->first_attribute("name")->value());
					group.setWidth(Cast::fromString<unsigned int>(objectgroup_element->first_attribute("width")->value()));
					group.setHeight(Cast::fromString<unsigned int>(objectgroup_element->first_attribute("height")->value()));

					xml_attribute<>* visible_attr = objectgroup_element->first_attribute("visible");
					if (visible_attr != NULL) {
						unsigned int visible = Cast::fromString<unsigned int>(visible_attr->value());
						if (visible == 0) {
							group.setVisible(false);
						}
					}

					xml_node<>* object_element = 0;
					for (object_element = objectgroup_element->first_node("object");
							object_element;
							object_element = object_element->next_sibling("object")) {

						TiledMapObject* obj = new TiledMapObject();

						xml_attribute<>* object_name = object_element->first_attribute("name");
						if (object_name != NULL) {
							obj->setName(object_name->value());
						}

						xml_attribute<>* object_type = object_element->first_attribute("type");
						if (object_type != NULL) {
							obj->setType(object_type->value());
						}

						xml_attribute<>* object_width = object_element->first_attribute("width");
						if (object_width != NULL) {
							unsigned int obj_width_int = Cast::fromString<unsigned int>(object_width->value());
							obj->setWidth(obj_width_int);
						}

						xml_attribute<>* object_height = object_element->first_attribute("height");
						if (object_height != NULL) {
							unsigned int obj_height_int = Cast::fromString<unsigned int>(object_height->value());
							obj->setHeight(obj_height_int);
						}

						signed int object_x = Cast::fromString<signed int>(object_element->first_attribute("x")->value());
						signed int object_y = Cast::fromString<signed int>(object_element->first_attribute("y")->value());
						obj->setX(object_x);
						obj->setY(object_y);

						xml_attribute<>* gid_cstr = object_element->first_attribute("gid");
						if (gid_cstr != NULL) {
							int gid = Cast::fromString<signed int>(gid_cstr->value());
							obj->setGID(gid);
						}

						
						//	 <object name="spawn" type="spawn" gid="2" x="448" y="544">
						//	   <properties>
						//	    <property name="id" value="1"/>
						//	   </properties>
						//	  </object>
						  

						// add object properties
						xml_node<>* object_properties_root = NULL;
						object_properties_root = object_element->first_node("properties");

						if (object_properties_root != NULL) { 

							xml_node<>* object_property = NULL;
							for (object_property = object_properties_root->first_node("property");
									object_property;
									object_property = object_property->next_sibling("property"))
							{
								obj->addProperty(object_property->first_attribute("name")->value(), object_property->first_attribute("value")->value());
							}

						}

						// support for polygon lines in new tiled.
						// <polyline points="127,-96 223,-160 319,-96 415,-160"/>
						xml_node<>* polyline_root = NULL;
						polyline_root = object_element->first_node("polyline");
						if (polyline_root != NULL)
						{
							string polylinePoints = string(polyline_root->first_attribute("points")->value());
							vector<string> pointsvec = StringUtil::split(polylinePoints, " ");

							ARK2D::getLog()->v(StringUtil::append("We've got ourselves a polyline with # points: ", pointsvec.size()));
							for(unsigned int k = 0; k < pointsvec.size(); k++) {
								vector<string> pointsActual = StringUtil::split(pointsvec.at(k), ",");
								int pointx = Cast::fromString<int>( pointsActual.at(0) );
								int pointy = Cast::fromString<int>( pointsActual.at(1) );
								obj->addPolylinePoint(pointx, pointy);
							}
						}


						group.addObject(obj);
					}

					m_map->m_objectgroups.push_back(group);

				}
				ARK2D::getLog()->i("Loading TiledMap... done.");
			}
		#endif


	}
}










