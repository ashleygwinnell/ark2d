
#include "TiledMap.h"
#include "TiledMapTile.h"
#include "TiledMapLayer.h"

namespace ARK {
	namespace Tiled {

		// Layer
		TiledMapLayer::TiledMapLayer():
			m_map(),
			m_name(),
			m_width(0),
			m_height(0),
			m_visible(true),
			m_opacity(1.0f),
			m_properties(),
			m_data()
		{

		}
		void TiledMapLayer::setMap(TiledMap* map) {
			m_map = map;
		}
		void TiledMapLayer::setName(const std::string& name) {
			m_name = name;
		}
		const std::string& TiledMapLayer::getName() const {
			return m_name;
		}
		void TiledMapLayer::setWidth(unsigned int width) {
			m_width = width;
		}
		unsigned int TiledMapLayer::getWidth() const {
			return m_width;
		}
		void TiledMapLayer::setHeight(unsigned int height) {
			m_height = height;
		}
		unsigned int TiledMapLayer::getHeight() const {
			return m_height;
		}
		void TiledMapLayer::setVisible(bool v) {
			m_visible = v;
		}
		bool TiledMapLayer::isVisible() const {
			return m_visible;
		}
		void TiledMapLayer::prepare() {
			//m_data = new unsigned int[m_width][m_height];
			//m_data();
			/*m_data.resize(m_width);
			for (unsigned int i = 0; i < m_data.size(); i++) {
				m_data.at(i).resize(m_height);
			}*/


			for (unsigned int i = 0; i < m_width; i++) {
				std::vector<unsigned int> ints;
				ints.reserve(m_height);
				for (unsigned int j = 0; j < m_height; j++) {
					ints.push_back(0);
				}
				m_data.push_back(ints);
			}

			//std::cout << "Prepared";
			//std::cout << m_data.size();
			//std::cout << " : ";
			//std::cout << m_data.at(0).size() << std::endl;
		}
		void TiledMapLayer::setTileGID(unsigned int x, unsigned int y, unsigned int gid) {
			//m_data[x][y] = gid;
			//std::cout << "assigning " << gid << " at: x: " << x << ", y: " << y << std::endl;
			//m_data.at(x).assign(y, gid);
			m_data.at(x)[y] = gid;
		}
		unsigned int TiledMapLayer::getTileGID(unsigned int x, unsigned int y) const {
			//return m_data[x][y];
			//! @todo: this. returns something bad, sometimes.
			if (x >= m_data.size()) {
				return 0;
			} else if (y >= m_data.at(x).size()) {
				return 0; // return NULL;
			}

			return m_data.at(x).at(y);

			//std::cout << "Exception: " << outOfRange.what() << std::endl;
			//std::cout << "\t data.size(): " << m_data.size() << std::endl;
			//std::cout << "\t data[" << x << "].size(): " << m_data.at(x).size() << std::endl;
		}

		void TiledMapLayer::setOpacity(float f) {
			m_opacity = f;
		}
		float TiledMapLayer::getOpacity() {
			return m_opacity;
		}

		void TiledMapLayer::draw(int xOffset, int yOffset) const
		{
			if (!m_visible) { return; }
			// for each height.
			for (unsigned int y = 0; y < m_height; y++)
			{
				// for each width  -- this goes across the map.
				for (unsigned int x = 0; x < m_width; x++)
				{
					//g->setDrawColor(Color::white);
					//g->drawRect((x * m_tileWidth) + xOffset, (y * m_tileWidth) + yOffset, m_tileWidth, m_tileHeight);

					unsigned int gid = getTileGID(x, y);
					if (gid != 0) {
						TiledMapTileset* tileset = m_map->getTilesetByGID(gid);

						Image* t = tileset->getTileByGID(gid)->getImage();
						if (t != NULL) {
							t->setAlpha(m_opacity);
							t->draw((int) (x * tileset->getTileWidth()) + xOffset, (int) (y * tileset->getTileHeight()) + yOffset);
							t->setAlpha(1.0f);
						}
					}
				}
			}
		}




		TiledMapLayer::~TiledMapLayer() {

		}

	}
}
