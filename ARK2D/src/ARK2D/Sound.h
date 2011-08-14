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

#include "ErrorDialog.h"
#include "SoundStore.h"

#include "ARK2D.h"
#include "ARK2D_AL.h"

class Sound {
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
		void setPanning(float pan);
		~Sound();
	protected:
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

		// File Name
		std::string m_FileName;

		void setSourcePosition(float x, float y, float z);
		void setSourceVelocity(float x, float y, float z);
		static void setListenerPosition(float x, float y, float z);
		static void setListenerVelocity(float x, float y, float z);
		static void setListenerOrientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z);
		bool load(bool loop);
		bool loadWAV(bool loop);
		bool loadOGG(bool loop);

	protected:
		string getALErrorString(ALenum err);

		static unsigned short wav_readByte16(const unsigned char buffer[2]);
		static unsigned short wav_readByte32(const unsigned char buffer[4]);


};

#endif /* SOUND_H_ */
