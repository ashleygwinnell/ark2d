/*
 * Sound.h
 *
 *  Created on: 14-Dec-2009
 *      Author: Ashley
 */

#ifndef SOUND_H_
#define SOUND_H_

#ifdef _WIN32
	#include <windows.h>
#endif

//
// http://www.devmaster.net/articles/openal-tutorials/lesson4.php
//

#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "../UI/ErrorDialog.h"
#include "SoundStore.h"

#include "../ARK2D.h"
#include "../ARK2D_AL.h"
#include "../Resource.h"

class Sound : public ARK::Resource {
	friend class GameContainer;
	friend class GameContainerPlatform;

	public:
		Sound(const std::string& filename);
		void play();
		bool isPlaying();
		void stop();
		void pause();
		void setVolume(float volume);
		float getVolume();
		void setPitch(float pitch);
		float getPitch(); /** 1.0 is normal, from 0.5 to 2.0*/
		void setPanning(float pan);
		void setGroupId(unsigned int id);
		unsigned int getGroupId();
		~Sound();
	protected:
		// File Name
		std::string m_FileName;

		// Buffers hold sound data.
		ALuint Buffer;

		// Sources are points emitting sound.
		ALuint Source;

		// Position of the source sound.
		ALfloat SourcePos[3]; // = { 0.0, 0.0, 0.0 };

		// Velocity of the source sound.
		ALfloat SourceVel[3]; // = { 0.0, 0.0, 0.0 };

		// Position of the Listener.
		static ALfloat ListenerPos[3]; // = { 0.0, 0.0, 0.0 };

		// Velocity of the Listener.
		static ALfloat ListenerVel[3]; // = { 0.0, 0.0, 0.0 };

		// Orientation of the Listener. (first 3 elements are "at", second 3 are "up"):; Also note that these should be units of '1'.
		static ALfloat ListenerOri[6]; // = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

		// Volume, no need to store internally to be honest, but keep it simples, yarp!
		ALfloat m_volume;
		ALfloat m_pitch;

		// sound group - set volume by sound group.
		unsigned int m_groupId;

		void setSourcePosition(float x, float y, float z);
		void setSourceVelocity(float x, float y, float z);
		static void setListenerPosition(float x, float y, float z);
		static void setListenerVelocity(float x, float y, float z);
		static void setListenerOrientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z);
		bool load(bool loop);
		bool loadWAV(bool loop);
		bool loadOGG(bool loop);

	protected:
		void miscerror(string ss);
		string getALErrorString(ALenum err);
		string getOggErrorString(int code);

		void deinit();

		static unsigned short wav_readByte16(const unsigned char buffer[2]);
		static unsigned int wav_readByte32(const unsigned char buffer[4]);


};

#endif /* SOUND_H_ */
