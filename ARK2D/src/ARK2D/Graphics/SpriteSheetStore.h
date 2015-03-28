/*
 * SpriteSheetStore.h
 *
 *  Created on: 6 Oct 2014
 *      Author: Ashley
 */

#ifndef ARK_GRAPHICS_SPRITESHEETSTORE_H_
#define ARK_GRAPHICS_SPRITESHEETSTORE_H_

#include <string>
#include <map>
using namespace std;

#include "../Includes.h"

namespace ARK {
	namespace Graphics {

		class SpriteSheetDescription;
		class Image;

		class ARK2D_API SpriteSheetStore {
			public:
				static bool s_initialised;
				static map<SpriteSheetDescription*, Image*> s_sheets;
			public:
				static void init();
				static void addSheet(SpriteSheetDescription* desc, Image* sheet);
				static void removeSheet(SpriteSheetDescription* desc, Image* sheet);
				static Image* getImage(string name);
				
		};
	}
}

#endif // ARK_GRAPHICS_SPRITESHEETSTORE_H_