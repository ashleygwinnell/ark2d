/*
 * Sound.cpp
 *  Created on: 14-Dec-2009
 *      Author: Ashley Gwinnell
 */

#include "Sound.h"
#include "../Core/Resource.h"
#include "../Util/StringUtil.h"
#include "../Util/Log.h" 
#include "../vendor/FileInterface.h"

namespace ARK {
	namespace Audio {
		ALfloat Sound::ListenerPos[3] = { 0.0, 0.0, 0.0 };
		ALfloat Sound::ListenerVel[3] = { 0.0, 0.0, 0.0 };
		ALfloat Sound::ListenerOri[6] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

		//unsigned int Sound::DEFAULT_GROUP_ID = 0;
		//void Sound::setDefaultGroupId(unsigned int id) {
		//	DEFAULT_GROUP_ID = id;
		//}

		Sound::Sound(const std::string& filename):
			ARK::Core::Resource(),
			m_FileName(filename),
			m_groupId(0),
			m_preloadedData(NULL),
			m_preloadedDataLength(0)
			//Buffer(AL_NONE),
			//Source(AL_NONE)
		{
			ARK2D::getLog()->i(StringUtil::append("Loading Sound: ", filename));

			this->setSourcePosition(0.0, 0.0, 0.0);
			this->setSourceVelocity(0.0, 0.0, 0.0);

			bool suc = this->load(false); // AL_TRUE on success - false (no looping)
			if (suc == true) {
				SoundStore* ss = SoundStore::getInstance();
				ss->addSound(filename, this);
				ARK2D::getLog()->i(StringUtil::append("Loaded Sound: ", filename));
			} else {
				ARK2D::getLog()->e(StringUtil::append("Did not load sound: ", filename));
			}


		}
		Sound::Sound(const std::string& filename, void* data, int dataLength):
			ARK::Core::Resource(),
			m_FileName(filename),
			m_groupId(0),
			m_preloadedData(data),
			m_preloadedDataLength(dataLength)
			{
			ARK2D::getLog()->i(StringUtil::append("Loading Sound from memory: ", filename));
			this->setSourcePosition(0.0, 0.0, 0.0);
			this->setSourceVelocity(0.0, 0.0, 0.0);

			bool suc = this->load(false);
			if (suc == true) {
				SoundStore* ss = SoundStore::getInstance();
				ss->addSound(filename, this);
				ARK2D::getLog()->i(StringUtil::append("Loaded Sound: ", filename));
			} else {
				ARK2D::getLog()->e(StringUtil::append("Did not load sound: ", filename));
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

		//	#if defined(ARK2D_ANDROID)
		//		return false;
		//	#else

				// Discard previous errors:
				alGetError();

				// Generate an open buffer.
				alGenBuffers(1, &Buffer);
				ALenum bufferGenError = alGetError();
				if (bufferGenError != AL_NO_ERROR || Buffer == AL_NONE) {
					ErrorDialog::createAndShow("Error creating OpenAL Buffers.");
					deinit();
					return false; //alutGetErrorString(bufferGenError);
				}

				// Load Sound contents!
				bool b = false;
				string ext = StringUtil::getExtension(m_FileName);
				if (ext == "wav") {
					b = loadWAV(loop);
				} else if (ext == "ogg") {
					b = loadOGG(loop);
				} else {
					string errStr = "Can only load WAV and OGG (1): ";
					errStr += m_FileName;
					ErrorDialog::createAndShow(errStr);
					deinit();
					return false;
				}

				if (b == false) {
					deinit();
					//std::cout << "Did not load sound " << m_FileName << std::endl;
					return false;
				}

				// By now, the file is loaded and copied into the Buffer.
				// So, bind the Buffer with a Source.
				// (clear error first)
				/*ALenum derpError = alGetError();
				if (derpError != AL_NO_ERROR) {
					string str = "pre gen sources error in load() OpenAL: ";
					str += getALErrorString(derpError);
					ErrorDialog::createAndShow(str);
					deinit();
					return false;
				}*/

				alGetError();
				alGenSources(1, &Source);

			/*	ALenum sourceGenError = alGetError();
				if (sourceGenError != AL_NO_ERROR || Source == AL_NONE) {
					string errStr = "Error creating OpenAL Sources for file:\r\n ";
					errStr += m_FileName + "\r\n";
					errStr += getALErrorString(sourceGenError);
					ErrorDialog::createAndShow(errStr);
					deinit();
					return false; //alutGetErrorString(sourceGenError);
				}*/


				// Source Location details
				alSourcef (Source, AL_PITCH,    1.0     ); //miscerror("pitch");
				alSourcef (Source, AL_GAIN,     1.0     ); //miscerror("gain");
				alSourcefv(Source, AL_POSITION, SourcePos); //miscerror("pos");
				alSourcefv(Source, AL_VELOCITY, SourceVel); //miscerror("vel");
				alSourcei (Source, AL_BUFFER,   Buffer   ); //miscerror("buf");
				alSourcei (Source, AL_LOOPING,  AL_FALSE ); //miscerror("loop");

				miscerror("source");

				// Do another error check and return.
				/*ALenum s = alGetError();
				if (s != AL_NO_ERROR) {
					string str = "Miscellaneous error in load() OpenAL: ";
					str += getALErrorString(s);
					ErrorDialog::createAndShow(str);
					deinit();
					return false; //alutGetErrorString(s);
				}*/
				return true;
		//	#endif
		}

		void Sound::miscerror(string ss) {
		//	#if defined(ARK2D_ANDROID)
		//	#else
				ALenum s = alGetError();
				if (s != AL_NO_ERROR) {
					string str = "Miscellaneous error in load() OpenAL: ";
					str += ss;
					str += getALErrorString(s);
					ErrorDialog::createAndShow(str);
					deinit();
					//exit(0);
				}
		//	#endif
		}


		/*size_t ark_ov_read (void* ptr, size_t size, size_t nmemb, void* datasource) {
			return (size_t) fi_fread(ptr, (unsigned long int) size, (unsigned long int) nmemb, (FILE_INTERFACE*) datasource);
		}
		int ark_ov_seek (void *datasource, ogg_int64_t offset, int whence) {
			return fi_fseek((FILE_INTERFACE*) datasource, offset, whence);
		}
		long ark_ov_tell (void *datasource) {
			return fi_ftell((FILE_INTERFACE*) datasource);
		}
		int ark_ov_close (void *datasource) {
			fi_fclose((FILE_INTERFACE*) datasource);
			return 0;
		}*/

#ifndef DOXYGEN_SHOULD_SKIP_THIS
		size_t ark_ov_read (void* ptr, size_t size, size_t nmemb, void* datasource);
		size_t ark_ov_read (void* ptr, size_t size, size_t nmemb, void* datasource) {
			return (size_t) fi_fread(ptr, (unsigned long int) size, (unsigned long int) nmemb, (FILE_INTERFACE*) datasource);
		}
		int ark_ov_seek (void *datasource, ogg_int64_t offset, int whence);
		int ark_ov_seek (void *datasource, ogg_int64_t offset, int whence) {
			return fi_fseek((FILE_INTERFACE*) datasource, offset, whence);
		}
		long ark_ov_tell (void *datasource);
		long ark_ov_tell (void *datasource) {
			return fi_ftell((FILE_INTERFACE*) datasource);
		}
		int ark_ov_close (void *datasource);
		int ark_ov_close (void *datasource) {
			fi_fclose((FILE_INTERFACE*) datasource);
			return 0;
		}
#endif

		bool Sound::loadOGG(bool loop) {

			//#if defined(ARK2D_ANDROID)
			//	return false;
			//#else

				// references
				// http://www.ogre3d.org/tikiwiki/OpenAl+Soundmanager



				// Some vars!
				const unsigned int BUFFER_SIZE = 32768; // 32kb buffer
				ALenum format;
				ALsizei frequency;
				int bitStream;
				long bytes;
				//long totalSize;
				int endian = 0; // 0 for Little-Endian, 1 for Big-Endian
				if (ARK2D::isBigEndian()) {
					endian = 1;
				}

				//std::cout << "1" << std::endl;

				char array[BUFFER_SIZE]; // Local fixed size array
				FILE* f = NULL;
				vorbis_info* oggInfo = NULL;
				OggVorbis_File oggFile;
				vector<char> bufferData;

			//	std::cout << "2" << std::endl;

				// Open for binary reading
				#if defined (ARK2D_ANDROID)

					if (m_preloadedData != NULL) {
						FILE_INTERFACE* fi = fi_fopen(m_FileName.c_str(), "r", m_preloadedData, m_preloadedDataLength);
						ov_callbacks lalala;
						lalala.read_func = &ark_ov_read;
						lalala.seek_func = &ark_ov_seek;
						lalala.tell_func = &ark_ov_tell;
						lalala.close_func = &ark_ov_close;
						//lalala.read_func = &fi_fread;
						//lalala.seek_func = &fi_fseek;
						//lalala.tell_func = &fi_ftell;
						//lalala.close_func = &fi_fclose;
						int e = ov_open_callbacks((void*) fi, &oggFile, NULL, 0, lalala);
						if (e < 0) {
							string errStr = "Sound::loadOGG() - could not open ogg file (ov_open_callbacks) : ";
							errStr += m_FileName;
							errStr += getOggErrorString(e);
							ErrorDialog::createAndShow(errStr);
							return false;
						}
					} else {
						string errStr = "Sound::loadOGG() - Android must use Resource::get() for sounds: ";
						errStr += m_FileName;
						ErrorDialog::createAndShow(errStr);
						return false;
					}



				#else
					f = fopen(m_FileName.c_str(), "rb");

					if (f == NULL) {
						string errStr = "Sound::loadOGG() - could not open file (fopen):";
						errStr += m_FileName;
						ErrorDialog::createAndShow(errStr);
						return false;
					}



					// open using the SDK, no need to call fclose() now.
					int e = ov_open(f, &oggFile, NULL, 0);
					if (e < 0) {
						string errStr = "Sound::loadOGG() - could not open ogg file (ov_open): ";
						errStr += m_FileName;
						errStr += getOggErrorString(e);
						ErrorDialog::createAndShow(errStr);
						return false;
					}

				#endif


				

				//bool b = ov_fopen(const_cast<char*>(m_FileName.c_str()), &oggFile);
				//if (b == false) {
				//	string errStr = "Sound::loadOGG() - could not open file:";
				//	errStr += m_FileName;
				//	ErrorDialog::createAndShow(errStr);
				//	return false;
				//}

			//	std::cout << "4" << std::endl;
				// Get some info about the OGG and store it in oggInfo.
				ARK2D::getLog()->i("Getting OGG Info");
				oggInfo = ov_info(&oggFile, -1);
				if (oggInfo == NULL) {
					string errStr = "Sound::loadOGG() - could not get ogg info: ";
					errStr += m_FileName;
					ErrorDialog::createAndShow(errStr);

					ov_clear(&oggFile);

					return false;
				}

				//int samples = ov_pcm_total(&oggFile, -1);
				//totalSize = 2 * oggInfo->channels * samples;



				//std::cout << "5" << std::endl;

				if (oggInfo->channels == 1) {
					format = AL_FORMAT_MONO16;
				} else {
					format = AL_FORMAT_STEREO16;
				}

				// Check the number of channels... always use 16-bit samples
				//switch(oggInfo->channels){
				//	case 1:
				//		format = AL_FORMAT_MONO16;
				//		break;
				//	case 2:
				//		format = AL_FORMAT_STEREO16;
				//		break;
				//	case 4:
				//		format = alGetEnumValue("AL_FORMAT_QUAD16");
				//		break;
				//	case 6:
				//		format = alGetEnumValue("AL_FORMAT_51CHN16");
				//		break;
				//	case 7:
				//		format = alGetEnumValue("AL_FORMAT_61CHN16");
				//		break;
				//	case 8:
				//		format = alGetEnumValue("AL_FORMAT_71CHN16");
				//		break;
				//	default:
				//		string errStr = "Sound::loadOGG() - could not load sound. Invalid `format`. :";
				//		errStr += m_FileName;
				//		ErrorDialog::createAndShow(errStr);
				//		return false;
				//		break;
				//}

			///	std::cout << "6" << std::endl;

				//oggFile. = loop;
				// The frequency of the sampling rate
				frequency = (ALsizei) oggInfo->rate;


				// Now we are ready to decode the OGG file and put the raw audio data into the buffer.
				// We use a fixed size buffer and keep on reading until there is no more data left, like this:
				int r = 0;
				ARK2D::getLog()->i("Decoding OGG Data");
				do {
					// Read up to a buffer's worth of decoded sound data
					bytes = ov_read(&oggFile, &array[0], BUFFER_SIZE, endian, 2, 1, &bitStream);

					if (bytes < 0) {
						ARK2D::getLog()->i(StringUtil::append("ov_read error: ", getOggErrorString(r)));
					}

					// Append to end of buffer
					bufferData.insert(bufferData.end(), array, array + bytes);
				}
				while (bytes > 0);

			//	std::cout << "7" << std::endl;



				//vector<int16_t> samples;
				//char tmpbuf[4096];
				//bool firstrun = true;
				//while(1)
				//{
				//	int result = ov_read(&oggFile, &array[0], BUFFER_SIZE, endian, 2, 1, &bitStream);
				//	if(result > 0)
				//	{
				//		firstrun = false;
				//		//samples.insert(samples.end(), tmpbuf, tmpbuf + (result));
				//		bufferData.insert(bufferData.end(), array, array + (result));
				//	}
				//	else
				//	{
				//		if(result < 0)
				//		{
				//			printf("Sound::loadOGG() : Loading ogg sound data failed!");
				//			ov_clear(&oggFile);
				//			return false;
				//		}
				//		else
				//		{
				//			if(firstrun)
				//				return false;
				//			break;
				//		}
				//	}
				//}

				//if (bufferData.size() > 65536) {
				//	bufferData.resize(65536);
				//}

				//std::cout << "format: " << format << std::endl;
				//std::cout << "frequency: " << frequency << std::endl;
				//std::cout << "buffersize: " << bufferData.size() << std::endl;

				//std::cout << "samplessize: " << samples.size() << std::endl;
				//std::cout << "pcmtotal: " << ov_pcm_total(&oggFile, -1) << std::endl;

				// Load the wav into the buffer
				alGetError();
				//alBufferData(Buffer, format, &bufferData[0],  static_cast <ALsizei>( bufferData.size() ), frequency);
				alBufferData(Buffer, format, &bufferData[0],  bufferData.size(), frequency);
				ALenum bufferwaverr = alGetError();
				if (bufferwaverr != AL_NO_ERROR) {
					ErrorDialog::createAndShow("Error copying OGG file into buffer.");
					return false;
				}

				// Now all the audio data has been decoded and stuffed into the buffer.
				// We can release the file resources (resource leaks are bad!).
				bufferData.clear();
				ov_clear(&oggFile);

				//if (f != NULL) {
				//	fclose(f);
				//}

				//std::cout << "8" << std::endl;

				//return true;
				return true;
			//#endif
		}
		bool Sound::loadWAV(bool loop) {
			#if ( defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE) )
				ARK2D::getLog()->e("Wav not supported on Android");
				return false;
			#elif ( defined(ARK2D_WINDOWS) || defined(ARK2D_UBUNTU_LINUX) )
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
				//if (subChunk1Size >= 16) {
				if (subChunk1Size < 16) {
					string errStr = "Could not read wav file. This is not a wav file ('fmt ' chunk too small, truncated file?): "; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr); return false;
				}

				// check PCM audio format.
				if (fread(buffer16, 2, 1, f) != 1) {
					string errStr = "Could not load wav file: "; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr); return false;
				}
				unsigned short audioFormat = wav_readByte16(buffer16);
				if (audioFormat != 1) {
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

				std::cout << "format: " << format << std::endl;
				std::cout << "datasize: " << data.size() << std::endl;
				std::cout << "frequency: " << frequency << std::endl;


				// Load the wav into the buffer
				ALenum derpError = alGetError();
				if (derpError != AL_NO_ERROR) {
					ErrorDialog::createAndShow("pre buffer data error.");
					return false;
				}

				//if (data.size() > 65536) {
				//	data.resize(65536);
				//}

				alBufferData(Buffer, format, &data[0],  data.size(), frequency);
				ALenum bufferwaverr = alGetError();
				if (bufferwaverr != AL_NO_ERROR) {
					ErrorDialog::createAndShow("Error copying WAV file into buffer.");
					return false;
				}



				return true;

			#endif
		}

		//! @todo: AL Doppler effects
		// alDopplerFactor(1.0f); // 1.2 is 20% pitch shift up.
		// alDopplerVelocity( 343.0f ); // m/s this may need to be scaled at some point

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

		float Sound::getPitch() {
			return m_pitch;
		}
		void Sound::setPitch(float pitch) {
			m_pitch = pitch;
			alSourcef(Source, AL_PITCH, pitch);
		}

		void Sound::setPanning(float pan) {
			SourcePos[0] = pan;
			alSourcefv(Source, AL_POSITION, SourcePos);
			//std::cerr << "OpenAL is broken -- seek alternative." << std::endl;
		}

		void Sound::setGroupId(unsigned int id) {
			m_groupId = id;
		}
		unsigned int Sound::getGroupId() {
			return m_groupId;
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
			//return "AL_UNKNOWN_ERROR";
			//const char* ch = (const char*) alGetString(err);
			//if (ch == NULL) {
			//	std::cout << "errorstr: " << err << std::endl;

			return string("AL_UNKNOWN_ERROR");
			//} else {
			//	return string(ch);
			//}
		}
		string Sound::getOggErrorString(int code) {
			switch(code) {
				case OV_EREAD:
					return string("Read from media.");
				case OV_ENOTVORBIS:
					return string("Not Vorbis data.");
				case OV_EVERSION:
					return string("Vorbis version mismatch.");
				case OV_EBADHEADER:
					return string("Invalid Vorbis header.");
				case OV_EFAULT:
					return string("Internal logic fault (bug or heap/stack corruption.");
				default:
					return string("Unknown Ogg error.");
			}
		}
		unsigned short Sound::wav_readByte16(const unsigned char buffer[2]) {
			if (ARK2D::isBigEndian()) {
				return (buffer[0] << 8) + buffer[1];
			} else {
				return (buffer[1] << 8) + buffer[0];
			}
		}
		unsigned int Sound::wav_readByte32(const unsigned char buffer[4]) {
			if (ARK2D::isBigEndian()) {
				return (buffer[0] << 24) + (buffer[1] << 16) + (buffer[2] << 8) + buffer[3];
			} else {
				return (buffer[3] << 24) + (buffer[2] << 16) + (buffer[1] << 8) + buffer[0];
			}
		}

		void Sound::deinit() {

			// make sure source and buffer ids are valid before trying to delete.
			if (alIsSource(Source)) {
				alSourceStop(Source);
				alDeleteSources(1, &Source);
			}
			if (alIsBuffer(Buffer)) {
				alDeleteBuffers(1, &Buffer);
			}
		}

		Sound::~Sound() {
			deinit();
		}
	}
}
