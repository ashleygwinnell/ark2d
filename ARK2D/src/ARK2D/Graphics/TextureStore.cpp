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
#include "FBO.h"

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
			m_map(),
			m_mapById()
			{
 
		}
		void TextureStore::addTexture(string ref, Texture* texture) {
			m_map[ref] = texture; 
			m_mapById[texture->m_id] = texture;
		}
		void TextureStore::removeTexture(string s) {
			map<string, Texture*>::iterator it;
			it = m_map.find(s);
			if (it != m_map.end()) { 
				unsigned int id = it->second->m_id;
				m_map.erase(it);

				map<unsigned int, Texture*>::iterator it2;
				it2 = m_mapById.find(id);
				if (it2 != m_mapById.end()) {
					m_mapById.erase(it2);
				}
			}

			
		}
		Texture* TextureStore::getTexture(string ref) {
			return m_map[ref];
		}
		Texture* TextureStore::getTexture(unsigned int ref) {
			return m_mapById[ref];
		}
		bool TextureStore::hasTexture(string ref) {
			return (m_map.find(ref) != m_map.end());
		}
		bool TextureStore::hasTexture(unsigned int ref) {
			return (m_mapById.find(ref) != m_mapById.end());
		}

		void TextureStore::reloadTextures() 
		{
			ARK2D::getLog()->i("Reloading Textures in the Texture Store.");

			// reload EVERY texture for now...
			map<string, Texture*>::iterator it;
			for (it = m_map.begin(); it != m_map.end(); it++) 
			{
				ARK2D::getLog()->v(StringUtil::append("Reloading: ", it->second->m_resource->filename));
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
					Renderer::setInterpolation(it->second->m_interpolation);
					it->second->m_resource->m_data = rdr->data;
					it->second->m_id = it->second->m_resource->load();
					//it->second->m_id = it->second->m_resource->texture_temp;
					delete rdr; 
				} else {
					Renderer::setInterpolation(it->second->m_interpolation);
					it->second->m_id = it->second->m_resource->load();
				}
				ARK2D::getLog()->v(StringUtil::append("New Texture ID: ", it->second->m_id));
			}
		}

		void TextureStore::print() {
			ARK2D::getLog()->v("Printing Texture Store");

			map<string, Texture*>::iterator it;
			for (it = m_map.begin(); it != m_map.end(); it++) 
			{
				string s = it->second->m_resource->filename;
				s += string(" is texture id: ");
				s += Cast::toString<unsigned int>(it->second->m_id);
				ARK2D::getLog()->v(s);
			}
		}
		
		TextureStore::~TextureStore() {

		}


		// FBO Store

		FBOStore* FBOStore::s_fboStore = NULL;
		FBOStore* FBOStore::getInstance() { 
			if (s_fboStore == NULL) {
				s_fboStore = new FBOStore();
			}
			return s_fboStore;
		}

		FBOStore::FBOStore(): 
			m_map() 
			{
 
		}
		void FBOStore::addFBO(unsigned int ref, FBO* texture) {
			m_map[ref] = texture;
		}
		void FBOStore::removeFBO(unsigned int ref) {
			map<unsigned int, FBO*>::iterator it;
			it = m_map.find(ref);
			m_map.erase(it);
		}
		FBO* FBOStore::getFBO(unsigned int ref) {
			return m_map[ref];
		}
		bool FBOStore::hasFBO(unsigned int ref) {
			return (m_map.find(ref) != m_map.end());
		}

		void FBOStore::reloadFBOs()
		{
			ARK2D::getLog()->i("Reloading FBOs in the FBO Store.");

			// reload EVERY FBO for now...
			map<unsigned int, FBO*>::iterator it;
			for (it = m_map.begin(); it != m_map.end(); it++) 
			{
				ARK2D::getLog()->v(StringUtil::append("Reloading FBO: ", it->second->_internalId));
				ARK2D::getLog()->v(StringUtil::append("Old FBO ID: ", it->second->fbo));
				ARK2D::getLog()->v(StringUtil::append("Old FBO Texture ID: ", it->second->fbo_texture));
				
				// TODO: unload old resources.
				// glDeleteTextures( 1, &texture );
				
				// reload resource
				it->second->init();
				ARK2D::getLog()->v(StringUtil::append("New FBO ID: ", it->second->fbo));
				ARK2D::getLog()->v(StringUtil::append("New FBO Texture ID: ", it->second->fbo_texture));
			}
		}

		void FBOStore::print() {
			ARK2D::getLog()->i("Printing FBO Store");

			map<unsigned int, FBO*>::iterator it;
			for (it = m_map.begin(); it != m_map.end(); it++) 
			{
				string s = StringUtil::append("FBO internal id ", it->second->_internalId);
				s += string(" has fbo id: ");
				s += Cast::toString<unsigned int>(it->second->fbo);
				s += string(" and texture id: ");
				s += Cast::toString<unsigned int>(it->second->fbo_texture);
				ARK2D::getLog()->i(s);
			}
		}
		
		FBOStore::~FBOStore() {

		}
	}
}
