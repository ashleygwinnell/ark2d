/*
 * Texture.cpp
 *
 *  Created on: 11-Jan-2010
 * 	This file remained empty until: 12 Dec 2012
 *      Author: Ashley
 */
#include "Texture.h"

namespace ARK {
	namespace Graphics {

		Texture::Texture():
			m_id(0),
			m_resource(NULL)
		{ 

		}   

		Texture::Texture(unsigned int texid, Image* resource):
			m_id(texid),
			m_resource(resource)
		{ 

		}

		Texture::~Texture()
		{

		}

	}
}
