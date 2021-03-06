/*
 * SoundStore.cpp
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#include "../Includes.h"
#include "Sound.h" 
#include "SoundStore.h"
#include "../Util/StringUtil.h"
#include "../Core/String.h"
#include "../Util/Log.h"

namespace ARK {
	namespace Audio {
		SoundStore* SoundStore::s_soundStore = NULL;
		SoundStore* SoundStore::getInstance() { 
			if (s_soundStore == NULL) {
				s_soundStore = new SoundStore();
			}
			return s_soundStore;
		}

		SoundStore::SoundStore(): 
			m_map(),
			m_currentGroupId(0)
			{
 
		}
		void SoundStore::setCurrentGroupId(unsigned int groupId) {
			m_currentGroupId = groupId;
		}
		void SoundStore::addSound(string ref, Sound* sound) {
			m_map[ref] = sound;
			sound->setGroupId(m_currentGroupId);
		}
		void SoundStore::removeSound(string s) {
			map<string, Sound*>::iterator it;
			it = m_map.find(s); 
			if (it == m_map.end()) { 
				ARK2D::getLog()->w(StringUtil::append("Sound was not in SoundStore: ", s));
				return; 
			}
			m_map.erase(it);
		}
		Sound* SoundStore::getSound(string ref) {
			return m_map[ref];
		}
		map<string, Sound*> SoundStore::getMap() { 
			return m_map;
		}
		void SoundStore::setVolumeByGroupId(unsigned int groupId, float volume) {
			map<string, Sound*>::iterator it;
			for(it = m_map.begin(); it != m_map.end(); it++) {
				if (it->second->getGroupId() == groupId) {
					it->second->setVolume(volume);
				}
			}
		}
		void SoundStore::setPanningByGroupId(unsigned int groupId, float panning) {
			map<string, Sound*>::iterator it;
			for(it = m_map.begin(); it != m_map.end(); it++) {
				if (it->second->getGroupId() == groupId) {
					it->second->setPanning(panning);
				}
			}
		}
		void SoundStore::setPitchByGroupId(unsigned int groupId, float pitch) {
			map<string, Sound*>::iterator it;
			for(it = m_map.begin(); it != m_map.end(); it++) {
				if (it->second->getGroupId() == groupId) {
					it->second->setPitch(pitch);
				}
			}
		}


		float SoundStore::getVolumeByGroupId(unsigned int groupId) {
			map<string, Sound*>::iterator it;
			for(it = m_map.begin(); it != m_map.end(); it++) {
				if (it->second->getGroupId() == groupId) {
					return it->second->getVolume();
				}
			}
			return 0.0f;
		}

		SoundStore::~SoundStore() {

		}
	}
}
