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
#include "../UI/ErrorDialog.h"
#include "../Util/Cast.h"
#include "../Util/StringUtil.h"
#include "../Image/Image.h"
#include "../vendor/zlib123/zlib.h"

#include "../GameContainer.h"
#include "../Graphics.h"

#include "../Util/ARKLog.h"

//#include "zlib123/zutil.h"
//#include "zlib123/deflate.h"

//using std::stringstream;

TiledMap::TiledMap(const string& file):
	m_file(file),
	m_xmldocument(file),
	m_tileWidth(0),
	m_tileHeight(0)
{
	bool loadOkay = m_xmldocument.LoadFile();
	if (!loadOkay) {
		ErrorDialog::createAndShow(StringUtil::append("Could not load Tiled Map: ", m_file));
		exit(0);
	} else {
		ARK2D::getLog()->i(StringUtil::append("Loading TiledMap file: ", m_file));
		const char* version;
		const char* orientation;
		TiXmlNode* mapnode = m_xmldocument.FirstChild("map");
		TiXmlElement* mapelement = mapnode->ToElement();
		version = mapelement->Attribute("version");
		orientation = mapelement->Attribute("orientation");
		if (strcmp(orientation, "orthogonal") != 0) {
			ErrorDialog::createAndShow(StringUtil::append("Ark 2D only supports Orthogonal TilED Maps: ", m_file));
			exit(0);
		}
		m_tileWidth = Cast::fromString<unsigned int>(mapelement->Attribute("tilewidth"));
		m_tileHeight = Cast::fromString<unsigned int>(mapelement->Attribute("tileheight"));
		m_widthInTiles = Cast::fromString<unsigned int>(mapelement->Attribute("width"));
		m_heightInTiles = Cast::fromString<unsigned int>(mapelement->Attribute("height"));
		m_widthInPixels = m_tileWidth * m_widthInTiles;
		m_heightInPixels = m_tileHeight * m_heightInTiles;


		// Properties
		TiXmlNode* propertiesnode = mapelement->FirstChild("properties");
		if (propertiesnode != NULL) {
			TiXmlElement* propertieselement = propertiesnode->ToElement();
			TiXmlElement* property_e = 0;
			for (property_e = propertieselement->FirstChild("property")->ToElement(); property_e;
								property_e = property_e->NextSiblingElement()) {
				TiledMapProperty property(property_e->Attribute("name"), property_e->Attribute("value"));
				addProperty(property);
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
				ErrorDialog::createAndShow(StringUtil::append("Ark 2D does not support inline Tileset data: ", m_file));
				exit(0);
			}

			string src(src_cstr);

			string path = StringUtil::pathToFile(file); // this piece makes sure the tileset is in the same folder as the map.
			if (path != file) {
				src.insert(0, path);
			}

			ARK2D::getLog()->i(StringUtil::append("Loading TiledMap Tileset file: ", src));

			//std::cout << src << std::endl;
			TiXmlDocument tileset_d(src);
			bool ts_loadOkay = tileset_d.LoadFile();
			if (!ts_loadOkay) {
				ErrorDialog::createAndShow(StringUtil::append("Could not load Tiled Tileset: ", src));
				exit(0);
			} else {
				TiXmlElement* external_tileset_e = tileset_d.FirstChildElement("tileset");
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
				if (!hasImageMask) {
					tileset.setImage(new Image(image_src));
				} else {
					tileset.setImage(new Image(image_src, Color(StringUtil::append("#", image_mask))));
				}



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
				/*while (current_height > 0) {
					tiles_down += 1;
					current_height -= (signed int) (tileset.getTileHeight() + tileset.getSpacing());
				}
				tileset.setNumberOfTilesDown(tiles_down);*/
				current_height -= current_height % (tileset.getTileHeight() + tileset.getSpacing());
				//std::cout << "current height: " << current_height << std::endl;


				unsigned int tiles_down = current_height / (tileset.getTileHeight() + tileset.getSpacing());
				tileset.setNumberOfTilesDown(tiles_down);

				//std::cout << "tile height: " << tileset.getTileHeight() << std::endl;
				//std::cout << "tile spacing: " << tileset.getSpacing() << std::endl;
				//std::cout << "Tileset height: " << tiles_down << std::endl;


				// Calculate number of tiles going across.
				//unsigned int current_width = tileset.getImage()->getWidth() - (2*tileset.getMargin());
				//current_width -= current_width % (tileset.getTileWidth() + tileset.getSpacing());

				//unsigned int tiles_across = current_width / (tileset.getTileWidth() + tileset.getSpacing());
				//tileset.setNumberOfTilesAcross(tiles_across);


				// Calculate number of tiles going across.
				unsigned int current_width = tileset.getImage()->getWidth() - tileset.getMargin();
				unsigned int tiles_across = current_width / (tileset.getTileWidth() + tileset.getSpacing());
				tileset.setNumberOfTilesAcross(tiles_across);
				
				// set last GID.
				unsigned int last_gid = firstgid + (tiles_across * tiles_down) - 1;
				tileset.setLastGID(last_gid);








				//#ifdef ARK2D_VERBOSE
				std::cout << "Before ready()" << std::endl;
				//#endif

				// ready for stuff, woo.
				tileset.ready();

				std::cout << "After ready()" << std::endl;

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

				//std::cout << "After ready()" << std::endl;

				// add it, finally.
				addTileset(tileset);

				ARK2D::getLog()->i(StringUtil::append("Loaded TiledMap Tileset file: ", src));

			}
		}


		//unsigned int count = 0;

		// layers please!
		TiXmlElement* layer_element = 0;
		for (layer_element = mapelement->FirstChildElement("layer");
						layer_element;
						layer_element = layer_element->NextSiblingElement("layer")) {
			TiledMapLayer layer;
			layer.setMap(this);
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
						ErrorDialog::createAndShow("Error Loading tiled map.\nThe width/height was incorrect after populating data.");
						exit(0);
					}

					//std::cout << "After setting tile gids()" << std::endl;

				} else {
					ErrorDialog::createAndShow(StringUtil::append("There is no tile data in one of your TilED layers: ", m_file));
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
					ErrorDialog::createAndShow(StringUtil::append("Ark 2D only supports TilED Maps without gzip compression on layers. ", m_file));
					exit(0);

					/* std::string compressed = data;
					uLongf outputSize = data.length() * 10;
					std::vector<Bytef> output(outputSize);
					const Bytef *input = reinterpret_cast<const Bytef*>(compressed.c_str());

					int result;
					while((result = uncompress(&output.front(), &outputSize, input, compressed.length())) == Z_BUF_ERROR)
					{
					   outputSize *= 2;
					   output.resize(outputSize);
					}
					std::cout << ((int) output.at(1)) << std::endl;
	*/
					/*std::cout << decoded << std::endl;
					int err;
					unsigned long RawDataLength = decoded.length();

					void *RawDataBuff = malloc(decoded.length());
					memcpy(RawDataBuff, decoded.c_str(), decoded.length());

					void *UnCompDataBuff = NULL;
					uLongf UnCompSize = 482000;
					UnCompDataBuff = malloc(UnCompSize);

					err = uncompress((Bytef*)UnCompDataBuff, &UnCompSize, (const Bytef*) RawDataBuff, RawDataLength);
					if (err != Z_OK) {
						ErrorDialog::createAndShow(StringUtil::append("Error: ", err));
					}*/
					//std::cout << uncomp << std::endl;
				}
			}

			//layer.prepare(); // the array resizes itself after assign()ing stuff, so re set the size! D:


			addLayer(layer);
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

				const char* gid_cstr = object_element->Attribute("gid");
				if (gid_cstr != NULL) {
					int gid = Cast::fromString<signed int>(gid_cstr);
					obj->setGID(gid);
				}

				group.addObject(obj);
			}

			m_objectgroups.push_back(group);

		}



		//std::cout << (*mapnode);
	}
	ARK2D::getLog()->i("Loading TiledMap... done.");
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
	cout << output << endl;
}

void TiledMap::draw() {
	draw(0, 0);
}
void TiledMap::draw(signed int x, signed int y) {

	//Graphics* g = const_cast<Graphics*>(ARK2D::getContainer()->getGraphics());

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
	try {
		return &m_layers.at(index);
	}  catch (...) {
		ErrorDialog::createAndShow(StringUtil::append("There is no Layer at index: ", index)); exit(0);
	}
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
	try {
		return &m_objectgroups.at(index);
	}  catch (...) {
		ErrorDialog::createAndShow(StringUtil::append("There is no Object group at index: ", index)); exit(0);
	}
}

TiledMapTileset* TiledMap::getTileset(unsigned int index) {
	try {
		return &m_tilesets.at(index);
	} catch (...) {
		ErrorDialog::createAndShow(StringUtil::append("There is no Tileset at index: ", index)); exit(0);
	}
}

TiledMapProperty* TiledMap::getProperty(unsigned int index) {
	try {
		return &m_properties.at(index);
	} catch (...) {
		ErrorDialog::createAndShow(StringUtil::append("There is no TiledMap Property at index: ", index)); exit(0);
	}
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











