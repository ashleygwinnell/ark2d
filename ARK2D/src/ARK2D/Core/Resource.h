/*
 * Resource.h
 *
 *  Created on: Feb 12, 2012
 *      Author: ashleygwinnell
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <string>
using namespace std;

#include "../Windres.h"
#include "../Namespaces.h"

#if defined(ARK2D_ANDROID)
	#include <zip.h>
#endif


namespace ARK {
	namespace Core {

		class RawDataReturns {
			friend class TiledMap;
			public:
				void* data;
				int size;
				RawDataReturns(): data(NULL), size(0) { }
				~RawDataReturns() {
					free(data);
				}
		};

		/*!
		 * \brief Load assets in to game. Images, Sounds, TiledMaps, LocalHighscores, Text files, Fonts, etc.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class Resource {

			public:
				static Resource* get(string ref);
				static string latestName();
			protected:
				static Resource* get(string ref, bool appendPath);
				static RawDataReturns* getRawData(string ref); // remember to free() this data.
				static unsigned int getResourceTypeByExtension(string extension);
				static string s_latestName;

			public:

				#if defined(ARK2D_ANDROID)
					static zip* apkZip;
					static string apkZipName;
				#endif


				Resource();
				Image* asImage();
				Sound* asSound();
				ARK::Font::Font* asFont();
				LocalHighscores* asLocalHighscores();
				TiledMap* asTiledMap();
				String* asString();
				PathGroup* asPathGroup();
				SpriteSheetDescription* asSpriteSheetDescription();
				virtual ~Resource();


		};
	}
}

#endif
