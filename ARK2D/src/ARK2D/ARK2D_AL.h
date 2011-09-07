/*
 * ARK2D_AL.h
 *
 *  Created on: 27 Feb 2011
 *      Author: ashley
 */

#ifdef ARK2D_WINDOWS
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
