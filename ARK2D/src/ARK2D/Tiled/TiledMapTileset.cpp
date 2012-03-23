
#include "TiledMapTile.h"
#include "TiledMapTileset.h"

namespace ARK {
	namespace Tiled {

		// Tilesets
		TiledMapTileset::TiledMapTileset():
			m_name(),
			m_image(NULL),
			m_mask(Color::magenta),
			m_firstgid(),
			m_lastgid(),
			m_tilewidth(),
			m_tileheight(),
			m_spacing(0),
			m_margin(0),
			m_total_tiles_across(1),
			m_total_tiles_down(1),
			m_tiles()
		{

		}

		void TiledMapTileset::ready() {
			//std::cout << "1" << std::endl;
			unsigned int current_x = m_margin;
			unsigned int current_y = m_margin;
			//std::cout << "2" << std::endl;

			m_tiles.reserve(m_total_tiles_across * m_total_tiles_down);

			for (unsigned int y = 0; y < m_total_tiles_down; y++)
			{
				current_x = m_margin;
				for (unsigned int x = 0; x < m_total_tiles_across; x++)
				{
					TiledMapTile* tile = new TiledMapTile();
					tile->m_image = m_image->getSubImage(current_x, current_y, m_tilewidth, m_tileheight);
					m_tiles.push_back(tile);

					current_x += (m_tilewidth + m_spacing);
				}
				current_y += (m_tileheight + m_spacing);
			}


			/*for (unsigned int x = 0; x < m_total_tiles_across; x++)
			{
				std::vector<TiledMapTile> tiles;
				tiles.reserve(m_total_tiles_down);

				current_y = m_margin;

				//std::cout << "3 start " << y << std::endl;

				for (unsigned int y = 0; y < m_total_tiles_down; y++)
				{
					//std::cout << "Adding image to tileset: " << current_x << ", " << current_y << std::endl;

					TiledMapTile tile;
					tile.m_image = m_image->getSubImage(current_x, current_y, m_tilewidth, m_tileheight);
					tiles.push_back(tile);

					current_y += (m_tileheight + m_spacing);
				}

				if (tiles.size() != m_total_tiles_down) {
					std::cout << "image size mismatch... was " << tiles.size() << " was meant to be " << m_total_tiles_down << " . " << std::endl;
				}

				m_tiles.push_back(tiles);


				current_x += (m_tilewidth + m_spacing);

				//std::cout << "3 end " << y << std::endl;
			}*/
			//std::cout << "4" << std::endl;

		}
		void TiledMapTileset::setName(const std::string& name) {
			m_name = name;
		}
		std::string TiledMapTileset::getName() {
			return m_name;
		}
		void TiledMapTileset::setFirstGID(unsigned int id) {
			m_firstgid = id;
		}
		unsigned int TiledMapTileset::getFirstGID() {
			return m_firstgid;
		}
		void TiledMapTileset::setLastGID(unsigned int id) {
			m_lastgid = id;
		}
		unsigned int TiledMapTileset::getLastGID() {
			return m_lastgid;
		}
		void TiledMapTileset::setTileWidth(unsigned int w) {
			m_tilewidth = w;
		}
		unsigned int TiledMapTileset::getTileWidth() {
			return m_tilewidth;
		}
		void TiledMapTileset::setTileHeight(unsigned int h) {
			m_tileheight = h;
		}
		unsigned int TiledMapTileset::getTileHeight() {
			return m_tileheight;
		}
		void TiledMapTileset::setMargin(unsigned int m) {
			m_margin = m;
		}
		unsigned int TiledMapTileset::getMargin() {
			return m_margin;
		}
		void TiledMapTileset::setSpacing(unsigned int s) {
			m_spacing = s;
		}
		unsigned int TiledMapTileset::getSpacing() {
			return m_spacing;
		}

		void TiledMapTileset::setNumberOfTilesAcross(unsigned int w) {
			m_total_tiles_across = w;
		}
		unsigned int TiledMapTileset::getNumberOfTilesAcross() {
			return m_total_tiles_across;
		}

		void TiledMapTileset::setNumberOfTilesDown(unsigned int h) {
			m_total_tiles_down = h;
		}
		unsigned int TiledMapTileset::getNumberOfTilesDown() {
			return m_total_tiles_down;
		}

		void TiledMapTileset::setImage(Image* image) {
			m_image = image;
		}

		Image* TiledMapTileset::getImage() const {
			return m_image;
		}

		TiledMapTile* TiledMapTileset::getTile(unsigned int x, unsigned int y) const {

			unsigned int gid = (y * m_total_tiles_across) + x;
			return m_tiles.at(gid);

			//return m_tiles.at(x).at(y);
		}
		TiledMapTile* TiledMapTileset::getTileByGID(unsigned int gid) const {
			if (gid == 0) { return NULL; }

			if (m_firstgid != 1) {
				gid -= m_firstgid;
			} else {
				gid -= 1;
			}

			return m_tiles.at(gid);

			/*if (gid == 0) { return NULL; }

			//int gidOffset = m_firstgid;

			if (m_firstgid != 1) {
				gid -= m_firstgid - 1;
			}

			signed int x = 0;
			//x = m_total_tiles_across - ((m_lastgid - gid) % m_total_tiles_across);// - 1;
			x = (gid % m_total_tiles_across) - 1;
			if (x < 0) { x = m_total_tiles_across - 1; }

			signed int y = 0;
			//y = ((gid - m_firstgid) % m_total_tiles_across) - 1;
			y = gid / m_total_tiles_across;

			try {
				return m_tiles.at(x).at(y);
			} catch (std::out_of_range outOfRange) {

				std::cout << "Exception: " << outOfRange.what() << std::endl;
				std::cout << "\t x: " << x << " y: " << y << std::endl;

			}*/
		}
		TiledMapTileset::~TiledMapTileset() {

		}
	}
}
