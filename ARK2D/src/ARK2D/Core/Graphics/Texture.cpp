/*
 * Texture.cpp
 *
 *  Created on: 11-Jan-2010
 * 	This file remained empty until: 12 Dec 2012
 *      Author: Ashley
 */
#include "Texture.h"

namespace ARK {
    namespace Core {
        namespace Graphics {

            Texture::Texture():
                m_id(0),
                m_resource(NULL),
                m_interpolation(0)
                #ifdef ARK2D_RENDERER_DIRECTX
                    , m_dxTexture(NULL)
                    , m_dxSampler(NULL)
                    , m_dxResourceView(NULL)
                #endif
            { 

            }   

            Texture::Texture(unsigned int texid, Image* resource):
                m_id(texid),
                m_resource(resource),
                m_interpolation(0)
                #ifdef ARK2D_RENDERER_DIRECTX
                    , m_dxTexture(NULL)
                    , m_dxSampler(NULL)
                    , m_dxResourceView(NULL)
                #endif
            { 

            }

            Texture::~Texture()
            {

            }
        }
	}
}
