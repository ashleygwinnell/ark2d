/*
 * SpriteSheetDescription.h
 *
 *  Created on: 31 Jul 2010
 *      Author: Ashley
 */

#ifndef SPRITESHEETDESCRIPTION_H_
#define SPRITESHEETDESCRIPTION_H_

/*#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <map>*/
//#include "../Includes.h"

#include "../Core/Resource.h"
#include "SpriteSheetDescriptionItem.h"


#include <string>
#include <map>
using namespace std;

namespace ARK {
	namespace Graphics {

		/*!
		 * \brief Read a spritesheet description file produce by a Sprite Sheet tool.
		 * It can read JSON or CSV.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API SpriteSheetDescription : public ARK::Core::Resource {
			public:
				SpriteSheetDescription(string path);
				SpriteSheetDescription(string path, void* rawData);
				const SpriteSheetDescriptionItem& getItemByName(const char* name) const;
				bool hasItemByName(const char* name) const;
				~SpriteSheetDescription();

			private:
				string m_path;
				string m_data;
				map<string, SpriteSheetDescriptionItem> m_items;
				void load();
		};
	}
}

#endif /* SPRITESHEETDESCRIPTION_H_ */
