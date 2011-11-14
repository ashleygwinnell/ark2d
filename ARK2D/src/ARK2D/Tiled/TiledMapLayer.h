#ifndef TILED_MAP_LAYER_H_
#define TILED_MAP_LAYER_H_

#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>

#include "TiledMapProperty.h"

using namespace std;

class TiledMap;

class TiledMapLayer {
	public:
		TiledMapLayer();
		void setMap(TiledMap* map);
		void setName(const string& name);
		const string& getName() const;
		void setWidth(unsigned int width);
		unsigned int getWidth() const;
		void setHeight(unsigned int height);
		unsigned int getHeight() const;
		void setVisible(bool v);
		bool isVisible() const;
		void prepare();
		unsigned int getTileGID(unsigned int x, unsigned int y) const;
		void setTileGID(unsigned int x, unsigned int y, unsigned int gid);

		void setOpacity(float f);
		float getOpacity();

		void draw(int xOffset, int yOffset) const;

		~TiledMapLayer();
	public:

		TiledMap* m_map;

		string m_name;
		unsigned int m_width;
		unsigned int m_height;
		bool m_visible;
		float m_opacity;
		vector<TiledMapProperty> m_properties;
		vector< std::vector<unsigned int > > m_data;
};


#endif
