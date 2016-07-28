/*
 * SoundStore.h
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#ifndef SOUNDSTORE_H_
#define SOUNDSTORE_H_

#include "../Common/DLL.h"
#include "../Namespaces.h"

#include <string>
#include <map>
using namespace std;

namespace ARK {
	namespace Audio {
		class Sound;

		/*!
		 * \brief Use to control a certain group of sounds' volumes. e.g. In-game from menu sounds.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API SoundStore {
			private:
				static SoundStore* s_soundStore;

			public:
				static SoundStore* getInstance();

			public:
				SoundStore();
				void setCurrentGroupId(unsigned int groupId);
				void addSound(string s, Sound* sound);
				void removeSound(string s);
				Sound* getSound(string s);
				map<string, Sound*> getMap();
				void setVolumeByGroupId(unsigned int groupId, float volume);
				void setPanningByGroupId(unsigned int groupId, float panning);
				void setPitchByGroupId(unsigned int groupId, float pitch);

				float getVolumeByGroupId(unsigned int groupId);

				virtual ~SoundStore();

			private:
				map<string, Sound*> m_map;
				unsigned int m_currentGroupId;
		};
	}
}

#endif /* SOUNDSTORE_H_ */
