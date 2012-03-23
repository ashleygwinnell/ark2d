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
using namespace std;

namespace ARK {
	namespace Audio {
		class Sound;

		/*!
		 * \brief Use to control a certain group of sounds' volumes. e.g. In-game from menu sounds.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
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
				void setVolumeByGroupId(unsigned int groupId, float volume);
				void setPanningByGroupId(unsigned int groupId, float panning);
				void setPitchByGroupId(unsigned int groupId, float pitch);
				~SoundStore();

			private:
				map<string, Sound*> m_map;
		};
	}
}

#endif /* SOUNDSTORE_H_ */
