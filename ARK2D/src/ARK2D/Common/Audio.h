// Common/Audio.h
// Date: 6 Oct 2014
#ifndef COMMON_AUDIO_INCLUDES_H_
#define COMMON_AUDIO_INCLUDES_H_

	#if defined(__AVM2__) || defined(ARK2D_FLASCC)
		typedef unsigned int ALenum;
 		typedef unsigned int ALuint;
 		typedef float ALfloat;
	#elif defined(ARK2D_EMSCRIPTEN_JS)
		#include <AL/al.h>
		#include <AL/alc.h>
	#elif defined(ARK2D_ANDROID)
		#include <AL/AL.h>
		#include <AL/ALc.h> 
		#include <AL/ALext.h>
	#elif defined(ARK2D_IPHONE)
		#include <OpenAL/al.h>
		#include <OpenAL/alc.h>
	#elif defined(ARK2D_WINDOWS_PHONE_8)
 		// Had to add !defined(_M_ARM) to vorbis/os.h
		#include <mfmediaengine.h>
		#include <xaudio2.h>
	#elif defined(_WIN32) || defined(ARK2D_WINDOWS)
		#include <AL/AL.h>
		#include <AL/ALc.h>
		#include <AL/ALext.h>
		#include <AL/ALut.h>
	#elif defined(ARK2D_MACINTOSH)
		#include <OpenAL/al.h>
		#include <OpenAL/alc.h>
	#elif defined(__linux__) || defined(ARK2D_UBUNTU_LINUX)
		#include <AL/al.h>
		#include <AL/alc.h>
		#include <AL/alext.h>
		#include <AL/alut.h>
	#endif

	#include "../vendor/ogg130/ogg.h"
	#include "../vendor/vorbis132/vorbisfile.h"	

#endif // COMMON_AUDIO_INCLUDES_H_

