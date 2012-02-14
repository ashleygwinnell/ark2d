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

namespace ARK {

	class Resource {
		public:
			static Resource* get(string ref);

			Resource();
			Image* asImage();
			Sound* asSound();
			virtual ~Resource();
	};
}

#endif
