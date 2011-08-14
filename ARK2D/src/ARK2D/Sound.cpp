/*
 * Sound.cpp
 *  Created on: 14-Dec-2009
 *      Author: Ashley Gwinnell
 */

#include "Sound.h"
#include "OutputWrapper.h"

ALfloat Sound::ListenerPos[3] = {0.0, 0.0, 0.0};
ALfloat Sound::ListenerVel[3] = { 0.0, 0.0, 0.0 };
ALfloat Sound::ListenerOri[6] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

Sound::Sound(const std::string& fname):
	m_FileName(fname)
{
	std::cout << "Loading Sound: " << fname.c_str() << std::endl;

	this->setSourcePosition(0.0, 0.0, 0.0);
	this->setSourceVelocity(0.0, 0.0, 0.0);
	//this->setListenerPosition(0.0, 0.0, 0.0);
	//this->setListenerVelocity(0.0, 0.0, 0.0);
	//this->setListenerOrientation(0.0, 0.0, -1.0,  0.0, 1.0, 0.0);

	bool suc = this->load(false); // AL_TRUE on success - false (no looping)
	if (suc == true) {
		SoundStore& ss = SoundStore::getInstance();
		ss.addSound(fname, this);
	} else {

	}
}

void Sound::setSourcePosition(float x, float y, float z) {
	SourcePos[0] = x; SourcePos[1] = y; SourcePos[2] = z;
}
void Sound::setSourceVelocity(float x, float y, float z) {
	SourceVel[0] = x; SourceVel[1] = y; SourceVel[2] = z;
}

void Sound::setListenerPosition(float x, float y, float z) {
	ListenerPos[0] = x; ListenerPos[1] = y; ListenerPos[2] = z;
}
void Sound::setListenerVelocity(float x, float y, float z) {
	ListenerVel[0] = x; ListenerVel[1] = y; ListenerVel[2] = z;
}
void Sound::setListenerOrientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z) {
	ListenerOri[0] = at_x; ListenerOri[1] = at_y; ListenerOri[2] = at_z;
	ListenerOri[3] = up_x; ListenerOri[4] = up_y; ListenerOri[5] = up_z;
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
	bool b = false;
	unsigned int pos = m_FileName.find_last_of('.');
	if (m_FileName.substr(pos) == ".wav") {
		b = loadWAV(loop);
	} else if (m_FileName.substr(pos) == ".ogg") {
		b = loadOGG(loop);
	} else {
		string errStr = "Can only load WAV and OGG (1): ";
		errStr += m_FileName;
		ErrorDialog::createAndShow(errStr);
		return false;
	}

	if (b == false) {
		return false;
	}

	// By now, the file is loaded and copied into the Buffer.
	// So, bind the Buffer with a Source.
	// (clear error first)
	alGetError();
	alGenSources(1, &Source);

	ALenum sourceGenError = alGetError();
	if (sourceGenError != AL_NO_ERROR) {
		string errStr = "Error creating OpenAL Sources for file:\r\n ";
		errStr += m_FileName + "\r\n";
		errStr += getALErrorString(sourceGenError);
		ErrorDialog::createAndShow(errStr);
		return false; //alutGetErrorString(sourceGenError);
	}

	// Source Location details
	alSourcef (Source, AL_PITCH,    1.0     );
	alSourcef (Source, AL_GAIN,     1.0     );
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
	const unsigned int BUFFER_SIZE = 32768; // 32kb buffer
	ALenum format;
	ALsizei frequency;
	int bitStream;
	long bytes;
	int endian = 0; // 0 for Little-Endian, 1 for Big-Endian
	#ifdef BIG_ENDIAN
		endian = 1;
	#endif

	char array[BUFFER_SIZE]; // Local fixed size array
	FILE* f;
	vorbis_info* oggInfo;
	OggVorbis_File oggFile;
	std::vector<char> bufferData;

	// Open for binary reading
	f = fopen(m_FileName.c_str(), "rb");
	if (f == NULL) {
		string errStr = "Sound::loadOGG() - could not open file:";
		errStr += m_FileName;
		ErrorDialog::createAndShow(errStr);
		return false;
	}

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
		bytes = ov_read(&oggFile, &array[0], BUFFER_SIZE, endian, 2, 1, &bitStream);

		// Append to end of buffer
		bufferData.insert(bufferData.end(), array, array + bytes);
	}
	while (bytes > 0);

	// Load the wav into the buffer
	alGetError();
	alBufferData(Buffer, format, &bufferData[0],  static_cast <ALsizei>( bufferData.size() ), frequency);
	//alBufferData(Buffer, format, &bufferData[0],  (ALsizei) bufferData.size(), frequency);
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
	#elif defined(ARK2D_MACINTOSH)

		// References
		// -  http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
		// -  http://www.borg.com/~jglatt/tech/wave.htm
		// -  Alut source code: static BufferData *loadWavFile (InputStream *stream)
		//    http://www.openal.org/repos/openal/tags/freealut_1_1_0/alut/alut/src/alutLoader.c
		// -  http://crownandcutlass.svn.sourceforge.net/viewvc/crownandcutlass/trunk/Protocce/src/soundutil.cpp?revision=914&view=markup

		const unsigned int BUFFER_SIZE = 32768;     // 32 KB buffers
		long bytes;
		vector <char> data;
		ALenum format;
		ALsizei freq;

		FILE* f = NULL;
		char* array = NULL;

		alGetError(); // clear error.

		f = fopen(m_FileName.c_str(), "rb");
		if (f == NULL) {
			string errStr = "Could not load wav file. It does not exist. "; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}

		// buffers
		char magic[5];
		magic[4] = '\0';
		unsigned char buffer32[4];
		unsigned char buffer16[2];

		// check magic
		if (fread(magic, 4,1,f) != 1) {
			string errStr = "Could not read wav file: "; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}
		if (string(magic) != "RIFF") {
			string errStr = "Could not read wav file. This is not a wav file (no RIFF magic): "; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}

		// skip 4 bytes (file size)
		fseek(f, 4, SEEK_CUR);

		// check file format
		if (fread(magic, 4,1,f) != 1) {
			string errStr = "Could not load wav file: "; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}
		if (string(magic) != "WAVE") {
			string errStr = "Could not read wav file. This is not a wav file (no WAVE format): "; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}

		// check 'fmt ' sub chunk (1)
		if (fread(magic, 4,1,f) != 1) {
			string errStr = "Could not load wav file: "; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}
		if (string(magic) != "fmt ") {
			string errStr = "Could not read wav file. This is not a wav file (no 'fmt ' subchunk): "; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}

		// read (1)s size
		if (fread(buffer32, 4, 1, f) != 1) {
			string errStr = "Could not load wav file: "; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}
		unsigned long subChunk1Size = wav_readByte32(buffer32);
		if (subChunk1Size >= 16) {
			string errStr = "Could not read wav file. This is not a wav file ('fmt ' chunk too small, truncated file?): "; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}

		// check PCM audio format.
		if (fread(buffer16, 2, 1, f) != 1) {
			string errStr = "Could not load wav file: "; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}
		unsigned short audioFormat = wav_readByte16(buffer16);
		if (audioFormat == 1) {
			string errStr = "Could not read wav file. This is not a wav file (audio format is not PCM): "; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}

		// read number of channels
		if (fread(buffer16, 2, 1, f) != 1) {
			string errStr = "Could not load wav file. Could not read number of channels.\r\n "; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}
		unsigned short channels = wav_readByte16(buffer16);

		 // read frequency (sample rate)
		if (fread(buffer32, 4, 1, f) != 1) {
			string errStr = "Could not load wav file. Could not read frequency/sample rate. \r\n"; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}
		unsigned long frequency = wav_readByte32(buffer32);

		// skip 6 bytes (Byte rate (4), Block align (2))
		fseek(f,6,SEEK_CUR);

		// read bits per sample.
		if (fread(buffer16, 2, 1, f) != 1) {
			string errStr = "Could not load wav file. Could not read bits per sample. \r\n"; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}
		unsigned short bps = wav_readByte16(buffer16);

		if (channels == 1) {
			format = (bps == 8)? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
		} else {
			format = (bps == 8)? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
		}

		// check 'data' sub chunk (2)
		if (fread(magic, 4, 1, f) != 1) {
			string errStr = "Could not load wav file. Could not read 'data' subchunk 2. \r\n"; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}
		if (string(magic) != "data") {
			string errStr = "Could not read wav file. This is not a wav file (no data subchunk): "; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}

		// read subchunk 2 (data) size.
		if (fread(buffer32, 4, 1, f) != 1) {
			string errStr = "Could not load wav file. Could not read subchunk 2 size. \r\n"; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}
		unsigned long subChunk2Size = wav_readByte32(buffer32);

		// the frequency of the sampling rate.
		freq = frequency;
		if (sizeof(freq) != sizeof(frequency)) {
			string errStr = "Could not load wav file. 'frequency' and 'freq' are different sizes. \r\n"; errStr += m_FileName;
			ErrorDialog::createAndShow(errStr); return false;
		}

		// read raw data.
		array = new char[BUFFER_SIZE];
		while (data.size() != subChunk2Size) {
			//read a buffer's worth of data.
			bytes = fread(array, 1, BUFFER_SIZE, f);

			if (bytes <= 0) { break; }

			if (data.size() +bytes >subChunk2Size) {
				bytes = subChunk2Size - data.size();
			}

			// append to end of buffer.
			data.insert(data.end(), array, array + bytes);
		}

		delete[] array;
		array = NULL;

		fclose(f);
		f = NULL;

		// Load the wav into the buffer
		alGetError();
		alBufferData(Buffer, format, &data[0],  data.size(), frequency);
		ALenum bufferwaverr = alGetError();
		if (bufferwaverr != AL_NO_ERROR) {
			ErrorDialog::createAndShow("Error copying WAV file into buffer.");
			return false;
		}

		return false;

	#endif
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

string Sound::getALErrorString(ALenum err) {
	switch(err)
	{
		case AL_NO_ERROR:
			return string("AL_NO_ERROR");
		break;

		case AL_INVALID_NAME:
			return string("AL_INVALID_NAME");
		break;

		case AL_INVALID_ENUM:
			return string("AL_INVALID_ENUM");
		break;

		case AL_INVALID_VALUE:
			return string("AL_INVALID_VALUE");
		break;

		case AL_INVALID_OPERATION:
			return string("AL_INVALID_OPERATION");
		break;

		case AL_OUT_OF_MEMORY:
			return string("AL_OUT_OF_MEMORY");
		break;
	};
	return "AL_UNKNOWN_ERROR";
}
unsigned short Sound::wav_readByte16(const unsigned char buffer[2]) {
	#ifdef BIG_ENDIAN
		return (buffer[0] << 8) + buffer[1];
	#else
		return (buffer[1] << 8) + buffer[0];
	#endif
}
unsigned short Sound::wav_readByte32(const unsigned char buffer[4]) {
	#ifdef BIG_ENDIAN
		return (buffer[0] << 24) + (buffer[1] << 16) + (buffer[2] << 8) + buffer[3];
	#else
		return (buffer[3] << 24) + (buffer[2] << 16) + (buffer[1] << 8) + buffer[0];
	#endif
}

Sound::~Sound() {
	alDeleteBuffers(1, &Buffer);
	alDeleteSources(1, &Source);
}
