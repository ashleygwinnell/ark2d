#ifndef TILED_MAP_OBJECT_H_
#define TILED_MAP_OBJECT_H_

#include <vector>
#include <string>

#include "TiledMapProperty.h"

using namespace std;

namespace ARK {
	namespace Tiled {

		/*!
		 * \brief An Object in an Object Group in a Tiled Map.
		 *
		 * @warning You should never need to create this manually.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class TiledMapObject {

			public:
				TiledMapObject();

				void setName(string s);
				string& getName();

				void setType(string s);
				string& getType();

				void setX(signed int x);
				signed int getX();

				void setY(signed int y);
				signed int getY();

				void setWidth(unsigned int w);
				unsigned int getWidth();

				void setHeight(unsigned int h);
				unsigned int getHeight();

				void setGID(unsigned int h);
				unsigned int getGID();

				TiledMapProperty* getPropertyByName(string name) const;
				void addProperty(string key, string value);

				~TiledMapObject();
			private:
				string m_name;
				string m_type;
				signed int m_x;
				signed int m_y;
				unsigned int m_width;
				unsigned int m_height;
				unsigned int m_gid;
				vector<TiledMapProperty*> m_properties;
		};
	}
}
#endif
