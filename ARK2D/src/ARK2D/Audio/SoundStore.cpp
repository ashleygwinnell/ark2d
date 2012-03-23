/*
 * SoundStore.cpp
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#include <iostream>
#include <map>
#include "Sound.h"
#include "SoundStore.h"

namespace ARK {
	namespace Audio {
		SoundStore SoundStore::s_soundStore;
		SoundStore& SoundStore::getInstance() {
			return s_soundStore;
		}

		SoundStore::SoundStore(): m_map() {

		}
		void SoundStore::addSound(string ref, Sound* sound) {
			m_map[ref] = sound;
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
		SoundStore::~SoundStore() {

		}
	}
}
