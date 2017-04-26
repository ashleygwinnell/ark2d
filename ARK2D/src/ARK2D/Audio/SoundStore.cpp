/*
 * SoundStore.cpp
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

//#include "../Includes.h"
#include "Sound.h"
#include "SoundStore.h"
#include "../Core/Util/StringUtil.h"
#include "../Core/String.h"
#include "../Core/Log.h"

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
			m_volumeMap(),
			m_panningMap(),
			m_pitchMap(),
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
			if (m_volumeMap.find(groupId) == m_volumeMap.end()) {
				m_volumeMap.insert( std::pair<unsigned int, float>(groupId, volume) );
			}


			//string s = StringUtil::append("**** Setting soundstore group ", groupId);
			//s += StringUtil::appendf(" to ", volume);
			//ARK2D::getLog()->e(s);

			map<string, Sound*>::iterator it;
			for(it = m_map.begin(); it != m_map.end(); it++) {
				if (it->second->getGroupId() == groupId) {
					it->second->setVolume(volume);
				}
			}
		}
		void SoundStore::setPanningByGroupId(unsigned int groupId, float panning) {
			m_panningMap[groupId] = panning;

			map<string, Sound*>::iterator it;
			for(it = m_map.begin(); it != m_map.end(); it++) {
				if (it->second->getGroupId() == groupId) {
					it->second->setPanning(panning);
				}
			}
		}
		void SoundStore::setPitchByGroupId(unsigned int groupId, float pitch) {
			m_pitchMap[groupId] = pitch;

			map<string, Sound*>::iterator it;
			for(it = m_map.begin(); it != m_map.end(); it++) {
				if (it->second->getGroupId() == groupId) {
					it->second->setPitch(pitch);
				}
			}
		}


		float SoundStore::getVolumeByGroupId(unsigned int groupId) {
			map<unsigned int, float>::iterator itv = m_volumeMap.find(groupId);

			float volume = 0.0f;
			if (itv != m_volumeMap.end()) {
				volume = itv->second;
			} else {
				map<string, Sound*>::iterator it;
				for(it = m_map.begin(); it != m_map.end(); it++) {
					if (it->second->getGroupId() == groupId) {
						volume = it->second->getVolume();
						break;
					}
				}
				ARK2D::getLog()->e(StringUtil::append("Could not get volumy by group id as no sounds exist in the group. ", groupId));
			}

			//string s = StringUtil::append("**** Getting soundstore group ", groupId);
			//s += StringUtil::appendf(" as ", volume);
			//ARK2D::getLog()->e(s);
			return volume;
		}

		SoundStore::~SoundStore() {

		}
	}
}
