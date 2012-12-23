/*
 * Texture.h
 *
 *  Created on: 11-Jan-2010
 * 	This file remained empty until: 12 Dec 2012
 *      Author: Ashley
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../Includes.h"
#include "Image.h"

namespace ARK {
	namespace Graphics {

		class Texture {
			public:
				unsigned int m_id;
				Image* m_resource;
			 
			public:
				Texture();
				Texture(unsigned int texid, Image* resource);
				inline unsigned int getId() { return m_id; }
				virtual ~Texture();
		};
	}
}

#endif /* TEXTURE_H_ */
