/*
 * SoundStore.h
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#ifndef SOUNDSTORE_H_
#define SOUNDSTORE_H_

#include <map>
#include <string>

class Sound;

using namespace std;

class SoundStore {
	private:
		static SoundStore s_soundStore;

	public:
		static SoundStore& getInstance();

	public:
		SoundStore();
		void addSound(string s, Sound* sound);
		Sound* getSound(string s);
		map<string, Sound*> getMap();
		~SoundStore();

	private:
		map<string, Sound*> m_map;
};

#endif /* SOUNDSTORE_H_ */
