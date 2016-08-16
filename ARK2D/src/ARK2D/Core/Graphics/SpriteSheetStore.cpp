/*
 * SpriteSheetStore.cpp
 *
 *  Created on: 6 Oct 2014
 *      Author: Ashley
 */

#include "SpriteSheetStore.h"

#include "../Font/Font.h"
#include "Image.h"
#include "SpriteSheetDescription.h"
#include "../ARK2D.h"
#include "../../Util/Log.h"

namespace ARK {
    namespace Core {
        namespace Graphics {

            bool SpriteSheetStore::s_initialised = true;
            map<SpriteSheetDescription*, Image*> SpriteSheetStore::s_sheets;

            void SpriteSheetStore::addSheet(SpriteSheetDescription* desc, Image* sheet) {
                s_sheets[desc] = sheet;
            } 

            void SpriteSheetStore::removeSheet(SpriteSheetDescription* desc, Image* sheet) {
                map<SpriteSheetDescription*, Image*>::iterator iterator;
                for (iterator = s_sheets.begin(); iterator != s_sheets.end(); iterator++) {
                    if (iterator->first == desc && iterator->second == sheet) {
                        s_sheets.erase(iterator);
                        break;
                    }
                }
            }

            Image* SpriteSheetStore::getImage(string name) 
            {
                map<SpriteSheetDescription*, Image*>::iterator iterator;
                for (iterator = s_sheets.begin(); iterator != s_sheets.end(); iterator++ ) 
                {
                    SpriteSheetDescription* desc = iterator->first;
                    Image* sheet = iterator->second;
                    if (desc->hasItemByName(name.c_str())) {
                        return sheet->getSubImage(desc->getItemByName(name.c_str()));
                    }
                }
                ARK2D::getLog()->w(StringUtil::append("Could not get image from SpriteSheetStore with name: ", name));
                ARK2D::getLog()->w("Returning a dummy...");
                return Image::getDummy();
            }

            
        }
	}
}
