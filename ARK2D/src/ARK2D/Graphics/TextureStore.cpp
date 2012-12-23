/*
 * TextureStore.cpp
 *
 *  Created on: 12 Dec 2012
 *      Author: Ashley
 */

#include "../Includes.h"
#include "Texture.h" 
#include "TextureStore.h"
#include "../ARK2D.h"
#include "../Util/Log.h"
#include "Renderer.h"

namespace ARK {
	namespace Graphics {
		TextureStore* TextureStore::s_textureStore = NULL;
		TextureStore* TextureStore::getInstance() { 
			if (s_textureStore == NULL) {
				s_textureStore = new TextureStore();
			}
			return s_textureStore;
		}

		TextureStore::TextureStore(): 
			m_map() 
			{
 
		}
		void TextureStore::addTexture(string ref, Texture* texture) {
			m_map[ref] = texture;
		}
		void TextureStore::removeTexture(string s) {
			map<string, Texture*>::iterator it;
			it = m_map.find(s);
			m_map.erase(it);
		}
		Texture* TextureStore::getTexture(string ref) {
			return m_map[ref];
		}
		bool TextureStore::hasTexture(string ref) {
			return (m_map.find(ref) != m_map.end());
		}

		void TextureStore::reloadTextures() 
		{
			ARK2D::getLog()->i("Reloading Textures in the Texture Store.");

			// reload EVERY texture for now...
			map<string, Texture*>::iterator it;
			for (it = m_map.begin(); it != m_map.end(); it++) 
			{
				ARK2D::getLog()->i(StringUtil::append("Reloading: ", it->second->m_resource->filename));
				//unsigned int textureId = it->second->m_textureId;

				/*int w = 1024;
				int h = 1024;
				char* data = NULL;

				RendererState::internalBindTexture(textureId);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Renderer::getInterpolationGL());
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Renderer::getInterpolationGL());
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				RendererState::internalBindTexture(0);*/

				// TODO: unload old texture resource.
				// glDeleteTextures( 1, &texture );
				
				if (it->second->m_resource->m_data != NULL) { 
					//unsigned int oldTextureId = it->second->m_resource->getTexture()->getId();
					RawDataReturns* rdr = Resource::getRawData(it->second->m_resource->filename);
					it->second->m_resource->m_data = rdr->data;
					it->second->m_id = it->second->m_resource->load();
					//it->second->m_id = it->second->m_resource->texture_temp;
					delete rdr; 
				} else {
					it->second->m_id = it->second->m_resource->load();
				}
				ARK2D::getLog()->i(StringUtil::append("New Texture ID: ", it->second->m_id));
			}

			/*map<string, Texture*>::iterator it;
			for (it = m_map.begin(); it != m_map.end(); it++) 
			{
				unsigned int texid = it->second->m_textureId;
				if (!glIsTexture(texid)) 
				{
					texid = glGenTextures(1, &texid);
				}
			}*/


			/*for tex in textures:
			    if glIsTexture(tex.opengl_name) == false:
			        glGenTextures(1, &tex.opengl_name)

			    glBindTexture(tex.texture_target);
			    glTexImage(..., texture.image);*/
		}

		void TextureStore::print() {
			ARK2D::getLog()->i("Printing Texture Store");

			map<string, Texture*>::iterator it;
			for (it = m_map.begin(); it != m_map.end(); it++) 
			{
				string s = it->second->m_resource->filename;
				s += string(" is texture id: ");
				s += Cast::toString<unsigned int>(it->second->m_id);
				ARK2D::getLog()->i(s);
			}
		}
		
		TextureStore::~TextureStore() {

		}
	}
}
