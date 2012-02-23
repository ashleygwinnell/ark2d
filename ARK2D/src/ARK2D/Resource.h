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


#include <string>
using namespace std;

#include "ARK2D_windres.h"

#if defined(ARK2D_ANDROID)
	#include <zip.h>
#endif

namespace ARK {

class Font;

	class RawDataReturns {
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
		private:
			static Resource* get(string ref, bool appendPath);
			static RawDataReturns* getRawData(string ref); // remember to free() this data.
			static unsigned int getResourceTypeByExtension(string extension);

		public:

			#if defined(ARK2D_ANDROID)
				static zip* apkZip;
				static string apkZipName;
			#endif

			Resource();
			Image* asImage();
			Sound* asSound();
			ARK::Font* asFont();
			virtual ~Resource();


	};
}

#endif
