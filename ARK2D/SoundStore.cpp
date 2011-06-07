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
SoundStore::~SoundStore() {

}
