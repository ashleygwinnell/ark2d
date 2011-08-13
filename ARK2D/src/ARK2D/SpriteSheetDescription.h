/*
 * SpriteSheetDescription.h
 *
 *  Created on: 31 Jul 2010
 *      Author: Ashley
 */

#ifndef SPRITESHEETDESCRIPTION_H_
#define SPRITESHEETDESCRIPTION_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <map>

#include "SpriteSheetDescriptionItem.h"

using namespace std;

class SpriteSheetDescription {
	public:
		SpriteSheetDescription(const char* path);
		SpriteSheetDescription(string path);
		const SpriteSheetDescriptionItem& getItemByName(const char* name);
		~SpriteSheetDescription();

	private:
		string m_path;
		map<string, SpriteSheetDescriptionItem> m_items;
		void load();
};

#endif /* SPRITESHEETDESCRIPTION_H_ */
