#ifndef TILED_MAP_OBJECTGROUP_H_
#define TILED_MAP_OBJECTGROUP_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

//#include "TiledMapObject.h"

#include <vector>
#include <string>
using namespace std;

namespace ARK {
	namespace Tiled {

		/*!
		 * \brief An Object Group (a type of layer) in a Tiled Map.
		 *
		 * @warning You should never need to create this manually.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API TiledMapObjectGroup {
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

				void setOpacity(float f);
				float getOpacity();

				~TiledMapObjectGroup();
			private:
				string m_name;
				unsigned int m_width;
				unsigned int m_height;
				bool m_visible;
				float m_opacity;
				vector<TiledMapObject*> m_objects;



		};
	}
}
#endif
