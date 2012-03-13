/*
 * Resource.h
 *
 *  Created on: Feb 12, 2012
 *      Author: ashleygwinnell
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

class Image;
class Sound;
class Animation;
class LocalHighscores;
class TiledMap;
class ARKString;

#include <string>
using namespace std;

#include "ARK2D_windres.h"

#if defined(ARK2D_ANDROID)
	#include <zip.h>
#endif

namespace ARK {

class Font;

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
			ARK::Font* asFont();
			LocalHighscores* asLocalHighscores();
			TiledMap* asTiledMap();
			ARKString* asString();
			virtual ~Resource();


	};
}

#endif
