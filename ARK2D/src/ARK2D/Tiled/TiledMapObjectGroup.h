#ifndef TILED_MAP_OBJECTGROUP_H_
#define TILED_MAP_OBJECTGROUP_H_

#include <vector>
#include <string>

#include "TiledMapObject.h"

using namespace std;

namespace ARK {
	namespace Tiled {

		/*!
		 * \brief An Object Group (a type of layer) in a Tiled Map.
		 *
		 * @warning You should never need to create this manually.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class TiledMapObjectGroup {
			public:
				TiledMapObjectGroup();
				void addObject(TiledMapObject* object);
				vector<TiledMapObject*> getObjects();
				void setName(const string& name);
				const string& getName() const;
				void setWidth(unsigned int width);
				unsigned int getWidth() const;
				void setHeight(unsigned int height);
				unsigned int getHeight() const;

				void setVisible(bool b);
				bool isVisible();

				~TiledMapObjectGroup();
			private:
				string m_name;
				vector<TiledMapObject*> m_objects;
				bool m_visible;
				unsigned int m_width;
				unsigned int m_height;

		};
	}
}
#endif
