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

#include "../Graphics/Image.h"
using namespace std;

namespace ARK {
	namespace Tiled {

		class TiledMapProperty;

		/*!
		 * \brief A tile with an image reference on a Tiled Map.
		 *
		 * @warning You should never need to create this manually.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class TiledMapTile {
			public: 
				TiledMapTile();
				Image* getImage() const;
				TiledMapProperty* getPropertyByName(string name) const;
				vector<TiledMapProperty*> getProperties() const;

				virtual ~TiledMapTile();

			public:
				Image* m_image;
				vector<TiledMapProperty*> m_properties;
		};

	}
}

#endif /* TILEDMAPTILE_H_ */
