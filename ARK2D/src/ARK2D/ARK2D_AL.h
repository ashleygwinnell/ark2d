/*
 * ARK2D_AL.h
 *
 *  Created on: 27 Feb 2011
 *      Author: ashley
 */

#if defined(ARK2D_ANDROID)
	/*typedef unsigned int ALuint;
	typedef float ALfloat;
	typedef int ALenum;
	typedef int ALint;

*/

	/*#ifndef SOUNDTEMP__
	#define SOUNDTEMP__

		#define AL_SOURCE_STATE 0
		#define AL_PLAYING 0
		#define AL_GAIN 0
		#define AL_PITCH 0
		#define AL_POSITION 0

		void alSourcePlay(ALuint i) { }
		void alSourcePause(ALuint i) { }
		void alSourceStop(ALuint i) {}
		void alGetSourcei(ALuint i, ALuint j, ALfloat k) {  }
		void alSourcef(ALuint i, ALuint j, ALfloat k) { }
		void alSourcefv(ALuint i, ALuint j, ALfloat* k) { }
		bool alIsSource(ALuint i) { return false; }
		bool alIsBuffer(ALuint i) { return false; }
		void alDeleteSources(ALuint d, ALuint* i) {}
		void alDeleteBuffers(ALuint d, ALuint* i) {}
	#endif*/

	#include <AL/AL.h>
	#include <AL/ALc.h>
	#include <AL/ALext.h>

	#include "vendor/ogg130/ogg.h"
	#include "vendor/vorbis132/vorbisfile.h"

#elif defined(ARK2D_WINDOWS)
	#include <AL/AL.h>
	#include <AL/ALc.h>
	#include <AL/ALext.h>
	#include <AL/ALut.h>

	#include "vendor/ogg130/ogg.h"
	#include "vendor/vorbis132/vorbisfile.h"
#elif defined( ARK2D_UBUNTU_LINUX )
	#include <AL/al.h>
	#include <AL/alc.h>
	#include <AL/alext.h>
	#include <AL/alut.h>
	#include "vendor/ogg130/ogg.h"
	#include "vendor/vorbis132/vorbisfile.h"
	//#include <AL/efx.h>
#elif defined( ARK2D_MACINTOSH )
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
	//#include <OpenAL/MacOSX_OALExtensions.h>
	//#include "vendor/stbvorbis/stb.h"
	#include "vendor/ogg130/ogg.h"
	#include "vendor/vorbis132/vorbisfile.h"

	//typedef unsigned int ALuint;
	//typedef float ALfloat;
	//typedef bool ALboolean;
	//typedef int ALenum; //unsigned?
	//typedef int ALsidei; //unsigned?

#endif

//#ifndef ARK2D_AL_H_
//#define ARK2D_AL_H_



//#endif /* ARK2D_AL_H_ */
