/*
 * Resource.cpp
 *
 *  Created on: Feb 12, 2012
 *      Author: ashleygwinnell
 */

#include "Resource.h"

namespace ARK {

	Resource* Resource::get(string ref) {
		GameContainer* container = ARK2D::getContainer();
		container->getResourcePath();
		return NULL;
	}

	Resource::Resource() {

	}

	Image* Resource::asImage() {
		return dynamic_cast<Image*>(this);
	}
	Sound* Resource::asSound() {
		return dynamic_cast<Sound*>(this);
	}

	Resource::~Resource() {

	}

}
