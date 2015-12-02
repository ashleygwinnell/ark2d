/*
 * ShaderStore.cpp
 *
 *  Created on: 1 Dec 2015
 *      Author: Ashley
 */

#include "ShaderStore.h"
#include "Shader.h"
#include "../Util/Log.h"

#include <string>
using namespace std;

namespace ARK {
	namespace Graphics {
		
		ShaderStore* ShaderStore::s_instance = NULL;
		ShaderStore* ShaderStore::getInstance() { 
			if (s_instance == NULL) {
				s_instance = new ShaderStore();
			}
			return s_instance;
		}

		ShaderStore::ShaderStore(): 
			m_map()
			{
 
		}
		void ShaderStore::addShader(unsigned int ref, Shader* shader) {
			m_map[ref] = shader; 
		}
		void ShaderStore::removeShader(unsigned int s) {
			map<unsigned int, Shader*>::iterator it;
			it = m_map.find(s);
			if (it != m_map.end()) { 
				m_map.erase(it);
			}			
		}
		Shader* ShaderStore::getShader(unsigned int ref) {
			return m_map[ref];
		}
		bool ShaderStore::hasShader(unsigned int ref) {
			return (m_map.find(ref) != m_map.end());
		}
	
		void ShaderStore::print() {
			ARK2D::getLog()->v("Printing Shader Store");

			map<unsigned int, Shader*>::iterator it;
			for (it = m_map.begin(); it != m_map.end(); it++) 
			{
				string s = it->second->getName();
				s += string(" is shader id: ");
				s += Cast::toString<unsigned int>(it->second->getId());
				ARK2D::getLog()->v(s);
			}
		}
		
		ShaderStore::~ShaderStore() {

		}

	}
}
