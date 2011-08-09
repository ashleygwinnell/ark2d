/*
 * Sound.cpp
 *  Created on: 14-Dec-2009
 *      Author: Ashley Gwinnell
 */

#include "Sound.h"
#include "OutputWrapper.h"

Sound::Sound(const std::string& fname):
	m_FileName(fname)
{
	OutputWrapper::print("Loading Sound: ");
	OutputWrapper::println(fname.c_str());

	this->setSourcePosition(0.0, 0.0, 0.0);
	this->setSourceVelocity(0.0, 0.0, 0.0);
	this->setListenerPosition(0.0, 0.0, 0.0);
	this->setListenerVelocity(0.0, 0.0, 0.0);
	this->setListenerOrientation(0.0, 0.0, -1.0,  0.0, 1.0, 0.0);

	alListenerfv(AL_POSITION,    this->ListenerPos);
	alListenerfv(AL_VELOCITY,    this->ListenerVel);
	alListenerfv(AL_ORIENTATION, this->ListenerOri);

	bool suc = this->load(false); // AL_TRUE on success - false (no looping)
	if (suc == true) {
		SoundStore& ss = SoundStore::getInstance();
		ss.addSound(fname, this);
	}
}

void Sound::setSourcePosition(float x, float y, float z) {
	this->SourcePos[0] = x; this->SourcePos[1] = y; this->SourcePos[2] = z;
}
void Sound::setSourceVelocity(float x, float y, float z) {
	this->SourceVel[0] = x; this->SourceVel[1] = y; this->SourceVel[2] = z;
}
void Sound::setListenerPosition(float x, float y, float z) {
	this->ListenerPos[0] = x; this->ListenerPos[1] = y; this->ListenerPos[2] = z;
}
void Sound::setListenerVelocity(float x, float y, float z) {
	this->ListenerVel[0] = x; this->ListenerVel[1] = y; this->ListenerVel[2] = z;
}
void Sound::setListenerOrientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z) {
	this->ListenerOri[0] = at_x; this->ListenerOri[1] = at_y; this->ListenerOri[2] = at_z;
	this->ListenerOri[3] = up_x; this->ListenerOri[4] = up_y; this->ListenerOri[5] = up_z;
}

// returns true on success.
bool Sound::load(bool loop) {

	// Discard previous errors:
	alGetError();

	// Generate an open buffer.
	alGenBuffers(1, &Buffer);
	ALenum bufferGenError = alGetError();
	if (bufferGenError != AL_NO_ERROR) {
		ErrorDialog::createAndShow("Error creating OpenAL Buffers.");
		return false; //alutGetErrorString(bufferGenError);
	}

	// Load Sound contents!
	unsigned int pos = m_FileName.find_last_of('.');
	if (m_FileName.substr(pos) == ".wav") {
		loadWAV(loop);
	} else if (m_FileName.substr(pos) == ".ogg") {
		loadOGG(loop);
	}

	// By now, the file is loaded and copied into the Buffer.
	// So, bind the Buffer with a Source.
	alGenSources(1, &Source);

	ALenum sourceGenError = alGetError();
	if (sourceGenError != AL_NO_ERROR) {
		ErrorDialog::createAndShow("Error creating OpenAL Sources.");
		return false; //alutGetErrorString(sourceGenError);
	}

	// Source Location details
	alSourcef (Source, AL_PITCH,    1.0f     );
	alSourcef (Source, AL_GAIN,     1.0f     );
	alSourcefv(Source, AL_POSITION, SourcePos);
	alSourcefv(Source, AL_VELOCITY, SourceVel);
	alSourcei (Source, AL_BUFFER,   Buffer   );
	alSourcei (Source, AL_LOOPING,  AL_FALSE     );

	// Do another error check and return.
	ALenum s = alGetError();
	if (s != AL_NO_ERROR) {
		ErrorDialog::createAndShow("Miscellaneous error in load() OpenAL.");
		return false; //alutGetErrorString(s);
	}
	return true;
}
bool Sound::loadOGG(bool loop) {
	// Some vars!
	unsigned int BUFFER_SIZE = 32768; // 32kb buffer
	ALenum format;
	ALsizei frequency;
	int bitStream;
	long bytes;
	int endian = 0; // 0 for Little-Endian, 1 for Big-Endian
	char array[BUFFER_SIZE]; // Local fixed size array
	FILE *f;
	vorbis_info* oggInfo;
	OggVorbis_File oggFile;
	std::vector<char> bufferData;

	// Open for binary reading
	f = fopen(m_FileName.c_str(), "rb");

	// open using the SDK, no need to call fclose() now.
	ov_open(f, &oggFile, NULL, 0);

	// Get some info about the OGG and store it in oggInfo.
	oggInfo = ov_info(&oggFile, -1);

	// Check the number of channels... always use 16-bit samples
	if (oggInfo->channels == 1) {
		format = AL_FORMAT_MONO16;
	} else {
		format = AL_FORMAT_STEREO16;
	}

	//oggFile. = loop;
	// The frequency of the sampling rate
	frequency = oggInfo->rate;

	// Now we are ready to decode the OGG file and put the raw audio data into the buffer.
	// We use a fixed size buffer and keep on reading until there is no more data left, like this:
	do {
		// Read up to a buffer's worth of decoded sound data
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);

		// Append to end of buffer
		bufferData.insert(bufferData.end(), array, array + bytes);
	}
	while (bytes > 0);

	// Load the wav into the buffer
	alBufferData(Buffer, format, &bufferData[0],  static_cast <ALsizei>( bufferData.size() ), frequency);
	ALenum bufferwaverr = alGetError();
	if (bufferwaverr != AL_NO_ERROR) {
		ErrorDialog::createAndShow("Error copying OGG file into buffer.");
		return false;
	}

	// Now all the audio data has been decoded and stuffed into the buffer.
	// We can release the file resources (resource leaks are bad!).
	ov_clear(&oggFile);

	return true;
}
bool Sound::loadWAV(bool loop) {
	#if ( defined(ARK2D_WINDOWS) || defined(ARK2D_UBUNTU_LINUX) )
		// Variables to load into.
		ALenum format;
		ALvoid* data;
		ALsizei size;
		ALsizei freq;
		ALboolean al_loop = (ALboolean) loop;

		// Load the wav into memory from disk.
		#ifdef _WIN32
			ALbyte* name = const_cast<char*>(m_FileName.c_str());
			alutLoadWAVFile(name, &format, &data, &size, &freq, &al_loop);
		#endif
		#ifdef __linux__
			ALbyte* name = (ALbyte*) m_FileName.c_str();
			alutLoadWAVFile(name, &format, &data, &size, &freq, &al_loop);
		#endif
		ALenum loadwaverr = alGetError();
		if (loadwaverr != AL_NO_ERROR) {
			ErrorDialog::createAndShow("Error loading wav file from disk.");
			return false;//alutGetErrorString(loadwaverr);
		}

		// Load the wav into the buffer
		alBufferData(Buffer, format, data, size, freq);
		ALenum bufferwaverr = alGetError();
		if (bufferwaverr != AL_NO_ERROR) {
			ErrorDialog::createAndShow("Error copying wav file into buffer.");
			return false;//alutGetErrorString(bufferwaverr);
		}

		// Remove the wav from memory.
		alutUnloadWAV(format, data, size, freq);
		ALenum remove_wav_err = alGetError();
		if (remove_wav_err != AL_NO_ERROR) {
			ErrorDialog::createAndShow("Error removing wav file from memory.");
			return false;//alutGetErrorString(remove_wav_err);
		}

		return true;
	#endif
	return false;
}

void Sound::play() {
	alSourcePlay(Source);
}
bool Sound::isPlaying() {
	ALint state;
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	return (state == AL_PLAYING);
}
void Sound::stop() {
	alSourceStop(Source);
}
void Sound::pause() {
	alSourcePause(Source);
}
float Sound::getVolume() {
	return m_volume;
}
void Sound::setVolume(float volume) {
	m_volume = volume;
	alSourcef(Source, AL_GAIN, volume);
}

void Sound::setPanning(float pan) {
	//SourcePos[0] = pan;
	//alSourcefv(Source, AL_POSITION, SourcePos);
	std::cerr << "OpenAL is broken -- seek alternative." << std::endl;
}

Sound::~Sound() {
	alDeleteBuffers(1, &Buffer);
	alDeleteSources(1, &Source);
}
