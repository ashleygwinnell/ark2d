/*
 * Sound.cpp
 *  Created on: 14-Dec-2009
 *      Author: Ashley Gwinnell
 */

#include "Sound.h"
#include "SoundStore.h"

#include "../Core/Resource.h"
#include "../Core/String.h"
#include "../Core/Controls/ErrorDialog.h"
#include "../Core/Log.h"
#include "../Core/Util/StringUtil.h"

#include "../Core/Vendor/FileInterface.h"

//#include "../Includes.h"
#include "../Common/Audio.h"

#if defined(ARK2D_FLASCC)
	#include <AS3/AS3++.h>
 	//using namespace AS3::ui;


	 AS3::ui::var m_flascc_soundEventHandler(void* arg, AS3::ui::var as3Args) {
		/*flash::events::Event event = (flash::events::Event) as3Args[0];
		String type = event->type;

		// convert to std::string
		std::string typeStr = AS3::sz2stringAndFree(internal::utf8_toString(type));

		printf("Event [%s]\n", typeStr.c_str());*/

		Sound* s = (Sound*) arg;
		s->__flasccInternal_setPlaying(false);

		return AS3::ui::internal::_undefined;
	}
#endif

#if defined(ARK2D_WINDOWS_PHONE_8)
	IXAudio2* ARK::Audio::Sound::s_engine = NULL;
	IXAudio2MasteringVoice* ARK::Audio::Sound::s_master = NULL;

	Xaudio2VoiceCallback::Xaudio2VoiceCallback():
    	//hBufferEndEvent( CreateEvent( NULL, FALSE, FALSE, NULL ) ),
    	m_ark2dSoundPointer(NULL) {

    }
    Xaudio2VoiceCallback::~Xaudio2VoiceCallback() {
    	//CloseHandle( hBufferEndEvent );
    }

    //Called when the voice has just finished playing a contiguous audio stream.
    void Xaudio2VoiceCallback::OnStreamEnd() {
    	//SetEvent( hBufferEndEvent );
    	m_ark2dSoundPointer->m_xaIsPlaying = false;
    	//ARK2D::getLog()->e("Stream End");
    }

    //Unused methods are stubs
    void Xaudio2VoiceCallback::OnVoiceProcessingPassEnd() { }
    void Xaudio2VoiceCallback::OnVoiceProcessingPassStart(UINT32 SamplesRequired) {    }
    void Xaudio2VoiceCallback::OnBufferEnd(void * pBufferContext) {
    	//ARK2D::getLog()->e("Buffer End");
    }
    void Xaudio2VoiceCallback::OnBufferStart(void * pBufferContext) {
    	//ARK2D::getLog()->e("Buffer Start");
    }
    void Xaudio2VoiceCallback::OnLoopEnd(void * pBufferContext) {    }
    void Xaudio2VoiceCallback::OnVoiceError(void * pBufferContext, HRESULT Error) { }

#endif


namespace ARK {
	namespace Audio {
		float Sound::ListenerPos[3] = { 0.0, 0.0, 0.0 };
		float Sound::ListenerVel[3] = { 0.0, 0.0, 0.0 };
		float Sound::ListenerOri[6] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };

		//unsigned int Sound::DEFAULT_GROUP_ID = 0;
		//void Sound::setDefaultGroupId(unsigned int id) {
		//	DEFAULT_GROUP_ID = id;
		//}

		#if defined(ARK2D_WINDOWS_PHONE_8)
			void Sound::initialiseXAudio() {
				if (s_engine == NULL) {
					ARK2D::getLog()->i("Initialising COM...");
					CoInitializeEx(NULL, COINIT_MULTITHREADED);

					ARK2D::getLog()->i("Initialising XAudio2...");

					//UINT32 flags = 0;
					//#if defined(ARK2D_DEBUG)
					//	flags |= XAUDIO2_DEBUG_ENGINE;
					//#endif

					HRESULT hr = XAudio2Create(&s_engine, 0, XAUDIO2_DEFAULT_PROCESSOR);
					if (FAILED(hr)) {
						ARK2D::getLog()->i("Could not initialise XAudio2.");
						return;
					}

					#if defined(ARK2D_DEBUG)
						XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
						debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
						s_engine->SetDebugConfiguration(&debug);
					#endif


					HRESULT hr2 = s_engine->CreateMasteringVoice(&s_master);
					if (FAILED(hr2)) {
						ARK2D::getLog()->e("Could not initialise XAudio2 -- creating Mastering Voice.");
						return;
					}



				}
			}

			string Sound::getXAudio2Error(HRESULT hr) {
				if (hr == XAUDIO2_E_INVALID_CALL) {
					return "XAUDIO2_E_INVALID_CALL";
				}
				else if (hr == XAUDIO2_E_XMA_DECODER_ERROR) {
					return "XAUDIO2_E_XMA_DECODER_ERROR";
				}
				else if (hr == XAUDIO2_E_XAPO_CREATION_FAILED) {
					return "XAUDIO2_E_XAPO_CREATION_FAILED";
				}
				else if (hr == XAUDIO2_E_DEVICE_INVALIDATED) {
					return "XAUDIO2_E_DEVICE_INVALIDATED";
				}
				return "No error, maybe...";
			}

			void Sound::xa_startEngine() {
				HRESULT hr = s_engine->StartEngine();
				if (FAILED(hr)) {
					ARK2D::getLog()->e("Could not start XAudio2 engine.");
					return;
				}
			}
			void Sound::xa_stopEngine() {
				s_engine->StopEngine();
			}

		#endif

		Sound::Sound(const std::string& filename):
			ARK::Core::Resource(),
			m_FileName(filename),
			m_groupId(0),
			m_preloadedData(NULL),
			m_preloadedDataLength(0),
			Buffer(0),
			Source(0)

			#if defined(ARK2D_FLASCC)
				,
				m_flascc_sound(AS3::ui::internal::_null),
				m_flascc_channel(AS3::ui::internal::_null),
				m_flascc_transform(AS3::ui::internal::_null),
				m_flascc_isSoundPlaying(false),
				m_flascc_pausePosition(0)
			#elif defined(ARK2D_WINDOWS_PHONE_8)
				,
				m_xaSource(NULL),
				m_xaWF(),
				m_xaBuffer(),
				m_xaIsPlaying(false),
				m_xaCallbacks(NULL),
				m_xaPlayhead(0)
			#endif
			//Buffer(AL_NONE),
			//Source(AL_NONE)
		{


			ARK2D::getLog()->v(StringUtil::append("Loading Sound: ", filename));

			this->setSourcePosition(0.0, 0.0, 0.0);
			this->setSourceVelocity(0.0, 0.0, 0.0);

			bool suc = this->load(false); // AL_TRUE on success - false (no looping)
			if (suc == true) {
				SoundStore* ss = SoundStore::getInstance();
				ss->addSound(filename, this);
				ARK2D::getLog()->v(StringUtil::append("Loaded Sound: ", filename));
			} else {
				ARK2D::getLog()->e(StringUtil::append("Did not load sound: ", filename));
			}


		}
		Sound::Sound(const std::string& filename, void* data, int dataLength):
			ARK::Core::Resource(),
			m_FileName(filename),
			m_groupId(0),
			m_preloadedData(data),
			m_preloadedDataLength(dataLength),
			Buffer(0),
			Source(0)

			#if defined(ARK2D_FLASCC)
				,
				m_flascc_sound(AS3::ui::internal::_null),
				m_flascc_channel(AS3::ui::internal::_null),
				m_flascc_transform(AS3::ui::internal::_null),
				m_flascc_isSoundPlaying(false),
				m_flascc_pausePosition(0)
			#elif defined(ARK2D_WINDOWS_PHONE_8)
				,
				m_xaSource(NULL),
				m_xaWF(),
				m_xaBuffer(),
				m_xaIsPlaying(false)
			#endif

			{
			ARK2D::getLog()->v(StringUtil::append("Loading Sound from memory: ", filename));
			this->setSourcePosition(0.0, 0.0, 0.0);
			this->setSourceVelocity(0.0, 0.0, 0.0);

			bool suc = this->load(false);
			if (suc == true) {
				SoundStore* ss = SoundStore::getInstance();
				ss->addSound(filename, this);
				ARK2D::getLog()->v(StringUtil::append("Loaded Sound: ", filename));
			} else {
				ARK2D::getLog()->e(StringUtil::append("Did not load sound: ", filename));
			}

			m_preloadedData = NULL;
			m_preloadedDataLength = 0;
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


		/*AS3::ui::var soundEventHandler(void *arg, AS3::ui::var as3Args){
		    AS3::ui::flash::events::Event event = (AS3::ui::flash::events::Event) as3Args[0];
		    AS3::ui::String type = event->type;

		    // convert to std::string
		    std::string typeStr = AS3::sz2stringAndFree(AS3::ui::internal::utf8_toString(type));

		    printf("Event [%s]\n", typeStr.c_str());

		    return AS3::ui::internal::_undefined;
		}*/

		// returns true on success.
		bool Sound::load(bool loop) {

			#if defined(ARK2D_FLASCC)


				//string ext = StringUtil::getExtension(m_FileName);
				//if (ext != "mp3") {
				//	ARK2D::getLog()->e("Can only load mp3s in flascc export.");
				//	return false;
				//}


				//ARK2D::getLog()->v("Trying things 1");
			 	//AS3::ui::String filename = AS3::ui::internal::new_String(m_FileName.c_str());

				try {

				 	//ARK2D::getLog()->v("Trying things 2");
					m_flascc_sound = AS3::ui::flash::media::Sound::_new(AS3::ui::internal::_null, AS3::ui::internal::_null);

					//ARK2D::getLog()->v("Trying things 3");
					m_flascc_channel = AS3::ui::flash::media::SoundChannel::_new();

					//ARK2D::getLog()->v("Trying things 4");
					//string d = StringUtil::file_get_contents(m_FileName.c_str());
					//ARK2D::getLog()->v(StringUtil::append("data len: ", m_preloadedDataLength));




					//ARK2D::getLog()->v("another new str");
					//AS3::ui::String newdata = AS3::ui::internal::new_String((const char*) m_preloadedData);

					//ARK2D::getLog()->v(StringUtil::append("data len 2: ", newdata.length));

					// Throws RangeError — If the length is larger than 65535.
					//flash::utils::ByteArray ram = internal::get_ram();
					//arr->writeBytes(, 0, m_preloadedDataLength, (const char*) m_preloadedData);

					// flash::utils::ByteArray arr = flash::utils::ByteArray::_new();
					// inline_as3(
					// 	"CModule.readBytes(%0, %1, %2);\n"
     //    				: : "r"(m_preloadedData), "r"(m_preloadedDataLength), "r"(&arr)
     //    			);

					//ARK2D::getLog()->v("getting ram");
					AS3::ui::flash::utils::ByteArray ram = AS3::ui::internal::get_ram();

					//ARK2D::getLog()->v("new bytearr");
					AS3::ui::flash::utils::ByteArray arr = AS3::ui::flash::utils::ByteArray::_new();

					//ARK2D::getLog()->v("filling bytearr");
					//arr->writeBytes(ram, 0, m_preloadedDataLength, m_preloadedData);
					arr->writeBytes(ram, (unsigned int) m_preloadedData, m_preloadedDataLength);
//					ram->readBytes(arr, (unsigned int) m_preloadedData, m_preloadedDataLength);

					//arr->writeUTFBytes((const char*) m_preloadedData);
					//arr->writeUTF((const char*) m_preloadedData);
					//arr->writeByte((int) m_preloadedData)

					arr->position = 0;
					int byteArrayLength = arr->length;
					//ARK2D::getLog()->v(StringUtil::append("data len 3: ", byteArrayLength));

					//ARK2D::getLog()->v("Trying things 5");
					m_flascc_sound->loadCompressedDataFromByteArray( arr, byteArrayLength );

					//ARK2D::getLog()->v("Trying things 6");
					m_flascc_transform = AS3::ui::flash::media::SoundTransform::_new();
	      			m_flascc_transform->volume = 1.0f;

	      			//ARK2D::getLog()->v("Trying things 7");
	      			//delete arr;
	      			//free(arr);
	      			//AS3::ui::internal::_delete(arr); //AS3::local::internal::new_String("firstName"));

	      		} catch(AS3::ui::var ex) {
	      			//ex->toString();
	      			std::string err = AS3::sz2stringAndFree(AS3::ui::internal::utf8_toString(ex));
					ARK2D::getLog()->e(StringUtil::append("Could not create sound. AS3Error.", err));
					return false;
				} catch(std::exception ex) {
					ARK2D::getLog()->e(StringUtil::append("Could not create sound: ", ex.what()));
					return false;
				} catch(...) {
					ARK2D::getLog()->e("Exception but no idea which one. could not create Sound. :(");
					return false;
				}
					// Do it in AS3.
					// Problems with Workers not being able to populate Main Thread data.
					/*
						ARK2D::getLog()->v(StringUtil::append("Calling AS3 Console.loadSound(): ", m_FileName));
						const char* str = m_FileName.c_str();
						inline_as3(
							"import com.adobe.flascc.Console;\n"\
							"Console.s_console.loadSound(CModule.readString(%0, %1));\n"
							: : "r"(str), "r"(m_FileName.length())
						);
						ARK2D::getLog()->v("Done");
					*/

					/*AS3::ui::String url(m_FileName.c_str());
					AS3::ui::flash::net::URLRequest request = AS3::ui::flash::net::URLRequest::_new(url);
	    			AS3::ui::flash::media::Sound soundFactory = AS3::ui::flash::media::Sound::_new();
	    			AS3::ui::flash::media::SoundChannel song = AS3::ui::flash::media::SoundChannel::_new();

	    			soundFactory->addEventListener(AS3::ui::flash::events::Event::COMPLETE,
	                                   			   AS3::ui::Function::_new(soundEventHandler, NULL));
				    soundFactory->addEventListener(AS3::ui::flash::events::Event::ID3,
				                                   AS3::ui::Function::_new(soundEventHandler, NULL));
				    soundFactory->addEventListener(AS3::ui::flash::events::IOErrorEvent::IO_ERROR,
				                                   AS3::ui::Function::_new(soundEventHandler, NULL));
				    soundFactory->addEventListener(AS3::ui::flash::events::ProgressEvent::PROGRESS,
				                                   AS3::ui::Function::_new(soundEventHandler, NULL));

				    soundFactory->load(request);

				    song = soundFactory->play();*/

				    ARK2D::getLog()->v("success! :D");
					return true;

			#elif defined(ARK2D_XAUDIO2)
 				//ARK2D::getLog()->w("Sound::load Not implemented");

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

				// http://msdn.microsoft.com/en-gb/library/windows/desktop/dd390970(v=vs.85).aspx

				return b;
			#else

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
					miscerror("gen al source");

					ARK2D::getLog()->v(StringUtil::append("AL Source: ", Source));

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

					alSourcei (Source, AL_BUFFER,   Buffer   ); //miscerror("buf");
					miscerror("al source buffer ");

					alSourcef (Source, AL_PITCH,    1.0     ); //miscerror("pitch");
					miscerror("al source pitch ");

					alSourcef (Source, AL_GAIN,     1.0     ); //miscerror("gain");
					miscerror("al source gain ");

					alSourcefv(Source, AL_POSITION, SourcePos); //miscerror("pos");
					miscerror("al source position ");

					alSourcefv(Source, AL_VELOCITY, SourceVel); //miscerror("vel");
					miscerror("al source velocity ");

					alSourcei (Source, AL_LOOPING,  AL_FALSE ); //miscerror("loop");
					miscerror("al source looping ");

					// Do another error check and return.
					/*ALenum s = alGetError();
					if (s != AL_NO_ERROR) {
						string str = "Miscellaneous error in load() OpenAL: ";
						str += getALErrorString(s);
						ErrorDialog::createAndShow(str);
						deinit();
						return false; //alutGetErrorString(s);
					}*/
					return b;
			//	#endif
			#endif
		}

		void Sound::miscerror(string ss) {
			#if defined(ARK2D_FLASCC)

			#elif defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_XBOXONE)

			#else
				ALenum s = alGetError();
				if (s != AL_NO_ERROR) {
					string str = "Miscellaneous error in load() OpenAL: ";
					str += ss;
					str += getALErrorString(s);
					//ErrorDialog::createAndShow(str);
					ARK2D::getLog()->e(str);
					deinit();
					//exit(0);
				}
			#endif
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

			#if defined(ARK2D_FLASCC) // || defined(ARK2D_WINDOWS_PHONE_8)
				return false;
			#else

				// references
				// http://www.ogre3d.org/tikiwiki/OpenAl+Soundmanager



				// Some vars!
				const unsigned int BUFFER_SIZE = 32768; // 32kb buffer
				unsigned int format;
				//ALsizei frequency;
				unsigned long frequency;
				int bitStream;
				long bytes;
				//long totalSize;
				int endian = 0; // 0 for Little-Endian, 1 for Big-Endian
				if (ARK2D::isBigEndian()) {
					endian = 1;
				}

				//std::cout << "1" << std::endl;

				char arr[BUFFER_SIZE]; // Local fixed size array
				FILE* f = NULL;
				vorbis_info* oggInfo = NULL;
				OggVorbis_File oggFile;
				vector<char> bufferData;

			//	std::cout << "2" << std::endl;

				// Open for binary reading
				#if defined (ARK2D_ANDROID)

					if (m_preloadedData != NULL) {
						FILE_INTERFACE* fi = fi_fopen(m_FileName.c_str(), "rb", m_preloadedData, m_preloadedDataLength);
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

					#if defined(ARK2D_WINDOWS_PHONE_8)
						int wp8retval = fopen_s(&f, m_FileName.c_str(), "rb");
						if (wp8retval != 0) {
							ErrorDialog::createAndShow(StringUtil::append("Could not fopen_s file: ", m_FileName));
							return false;
						}
					#else
						f = fopen(m_FileName.c_str(), "rb");
					#endif

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
				ARK2D::getLog()->v("Getting OGG Info");
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

				#if defined(ARK2D_XAUDIO2)
					format = 0;
				#else
					if (oggInfo->channels == 1) {
						format = AL_FORMAT_MONO16;
					} else {
						format = AL_FORMAT_STEREO16;
					}
				#endif

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

				//frequency = (ALsizei) oggInfo->rate;
				frequency = oggInfo->rate;


				// Now we are ready to decode the OGG file and put the raw audio data into the buffer.
				// We use a fixed size buffer and keep on reading until there is no more data left, like this:
				int r = 0;
				ARK2D::getLog()->v("Decoding OGG Data");
				do {
					// Read up to a buffer's worth of decoded sound data
					bytes = ov_read(&oggFile, &arr[0], BUFFER_SIZE, endian, 2, 1, &bitStream);

					if (bytes < 0) {
						//ARK2D::getLog()->i(StringUtil::append("ov_read error: ", getOggErrorString(r)));
					}

					// Append to end of buffer
					bufferData.insert(bufferData.end(), arr, arr + bytes);
				}
				while (bytes > 0);

			//	std::cout << "7" << std::endl;



				//vector<int16_t> samples;
				//char tmpbuf[4096];
				//bool firstrun = true;
				//while(1)
				//{
				//	int result = ov_read(&oggFile, &arr[0], BUFFER_SIZE, endian, 2, 1, &bitStream);
				//	if(result > 0)
				//	{
				//		firstrun = false;
				//		//samples.insert(samples.end(), tmpbuf, tmpbuf + (result));
				//		bufferData.insert(bufferData.end(), arr, arr + (result));
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


				#if defined(ARK2D_XAUDIO2)

					unsigned int ubps = 16;// ogg is always 16 bit. (unsigned int)oggInfo->rate; //bps;
					unsigned int ubyps = (unsigned int) ubps / 8;
					unsigned int uchannels = (unsigned int) oggInfo->channels;
					unsigned int ufreq = (unsigned int) frequency;



					ARK2D::getLog()->i("WP8 bits...");
					ARK2D::getLog()->i(StringUtil::append("bps: ", ubps));
					ARK2D::getLog()->i(StringUtil::append("frequency: ", ufreq));
					ARK2D::getLog()->i(StringUtil::append("channels: ", uchannels));
					ARK2D::getLog()->i(StringUtil::append("datasize: ", bufferData.size()));

					// http://msdn.microsoft.com/en-gb/library/windows/desktop/dd390970(v=vs.85).aspx
					m_xaWF.wFormatTag = WAVE_FORMAT_PCM;
					m_xaWF.nChannels =  uchannels;
					m_xaWF.nSamplesPerSec = ufreq;
					m_xaWF.nBlockAlign = ubyps * uchannels;
					m_xaWF.nAvgBytesPerSec = ufreq * ubyps * uchannels;
					m_xaWF.wBitsPerSample = ubps;
					m_xaWF.cbSize = sizeof(m_xaWF); // 0;// data.size();

					ARK2D::getLog()->v("Creating source voice callback...");
					m_xaCallbacks = new Xaudio2VoiceCallback();
					m_xaCallbacks->m_ark2dSoundPointer = this;

					ARK2D::getLog()->v("Creating Xaudio2 voice...");
					HRESULT hr = s_engine->CreateSourceVoice(&m_xaSource, &m_xaWF, 0, XAUDIO2_DEFAULT_FREQ_RATIO, m_xaCallbacks, NULL, NULL);
					if (FAILED(hr)) {
						ARK2D::getLog()->e(StringUtil::append("Could not create XAudio2 voice. ", getXAudio2Error(hr)));
						return false;
					}

					//m_xaSource->Start();

					// Buffer struct.
					unsigned int sz = bufferData.size();
					m_xaBuffer.Flags = XAUDIO2_END_OF_STREAM;
					m_xaBuffer.AudioBytes = sz;// bufferData.size();
					m_xaBuffer.pAudioData = new BYTE[sz];// m_xaWF.cbSize];
					memcpy((void*)m_xaBuffer.pAudioData, &bufferData[0], sz);//m_xaWF.cbSize);
					m_xaBuffer.PlayBegin = m_xaPlayhead;
					m_xaBuffer.PlayLength = 0;
					m_xaBuffer.LoopBegin = 0;
					m_xaBuffer.LoopLength = 0;
					m_xaBuffer.LoopCount = 0;
					m_xaBuffer.pContext = NULL;
					//HRESULT hr2 = m_xaSource->SubmitSourceBuffer(&m_xaBuffer);
					//if (FAILED(hr2)) {
					//	ARK2D::getLog()->e(StringUtil::append("Could not submit XAudio2 source buffer.", getXAudio2Error(hr2)));
					//	return false;
					//}
				#else

					// Load the wav into the buffer
					alGetError();
					//alBufferData(Buffer, format, &bufferData[0],  static_cast <ALsizei>( bufferData.size() ), frequency);
					alBufferData(Buffer, format, &bufferData[0],  bufferData.size(), frequency);
					ALenum bufferwaverr = alGetError();
					if (bufferwaverr != AL_NO_ERROR) {
						ErrorDialog::createAndShow("Error copying OGG file into buffer.");
						return false;
					}
				#endif

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
			#endif
		}
		void Sound::clearErrors() {
			#if defined(ARK2D_XAUDIO2)
				// things
			#else
				alGetError();
			#endif
		}

		bool Sound::loadWAV(bool loop) {
			#if ( defined(ARK2D_FLASCC) )
				ARK2D::getLog()->e("Wav not supported on FlasCC");
				return false;
			//#elif defined(ARK2D_WINDOWS_PHONE_8)
			//	ARK2D::getLog()->e("Wav not supported on WP8");
			//	return false;
			#elif ( defined(ARK2D_EMSCRIPTEN_JS) )
				ARK2D::getLog()->e("Wav not supported on HTML5/JS?");
				return false;
			//#elif defined(ARK2D_WINDOWS_PHONE_8)
			//	ARK2D::getLog()->e("Wav not supported on WP8");
			//	return false;
			#elif ( defined(ARK2D_WINDOWS) ) // || defined(ARK2D_UBUNTU_LINUX) )
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

			#elif (defined(ARK2D_ANDROID) || defined(ARK2D_MACINTOSH) || defined(ARK2D_IPHONE) || defined(ARK2D_XAUDIO2) || defined(ARK2D_UBUNTU_LINUX))

				// References
				// -  http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
				// -  http://www.borg.com/~jglatt/tech/wave.htm
				// -  Alut source code: static BufferData *loadWavFile (InputStream *stream)
				//    http://www.openal.org/repos/openal/tags/freealut_1_1_0/alut/alut/src/alutLoader.c
				// -  http://crownandcutlass.svn.sourceforge.net/viewvc/crownandcutlass/trunk/Protocce/src/soundutil.cpp?revision=914&view=markup

				const unsigned int BUFFER_SIZE = 32768;     // 32 KB buffers
				long bytes;
				vector <char> data;
				unsigned int format;
				//ALsizei freq;
				unsigned int freq;

				FILE_INTERFACE* fi = NULL;
				//FILE* f = NULL;
				char* array = NULL;

				clearErrors();


				#if defined (ARK2D_ANDROID)

					if (m_preloadedData != NULL) {
						fi = fi_fopen(m_FileName.c_str(), "rb", m_preloadedData, m_preloadedDataLength);
					} else {
						string errStr = "Sound::loadWAV() - Android must use Resource::get() for sounds: ";
						errStr += m_FileName;
						ErrorDialog::createAndShow(errStr);
						return false;
					}
				#else
					/*char* thisData = NULL;
					unsigned int thisDataLen = 0;

					FILE* f = fopen(m_FileName.c_str(), "rb");
					if (f == NULL) {
						string errStr = "Could not load wav file. It does not exist. 1. "; errStr += m_FileName;
						//ErrorDialog::createAndShow(errStr);
						ARK2D::getLog()->e(errStr);
						return false;
					}

					fseek(f, 0, SEEK_END);
					int count = ftell(f);
					thisDataLen = count;
					rewind(f);

					if (count > 0) {
						thisData = (char*) malloc(sizeof(char) * (count));
						count = fread(thisData, sizeof(char), count, f);
					}
					fclose(f);*/

//					fi = fi_fopen(m_FileName.c_str(), "rb", thisData, thisDataLen);
					ARK2D::getLog()->v(m_FileName.c_str());
					fi = fi_fopen(m_FileName.c_str(), "rb");
				#endif

				//f = fopen(m_FileName.c_str(), "rb");
				//if (f == NULL) {
				if (fi == NULL) {
					string errStr = "Could not load wav file. It does not exist. 2. "; errStr += m_FileName;
					//ErrorDialog::createAndShow(errStr);
					ARK2D::getLog()->e(errStr);
					return false;
				}

				// buffers
				char magic[5];
				magic[4] = '\0';
				unsigned char buffer32[4];
				unsigned char buffer16[2];

				// check magic
				//if (fread(magic, 4,1,f) != 1) {
				if (fi_fread(magic, 4,1,fi) != 1) {
					string errStr = "Could not read wav file: "; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr);
					return false;
				}
				ARK2D::getLog()->v(StringUtil::append("compare RIFF: ", string(magic)));
				if (string(magic) != "RIFF") {
					string errStr = "Could not read wav file. This is not a wav file (no RIFF magic): "; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr);
					return false;
				}

				// skip 4 bytes (file size)
				//fseek(f, 4, SEEK_CUR);
				//fi_fseek(fi, 4, SEEK_CUR);
				if (fi_fread(buffer32, 4, 1, fi) != 1) {
					string errStr = "Could not load wav file, filesize: "; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr);
					return false;
				}
				unsigned int wavefilesize = wav_readByte32(buffer32);
				//ARK2D::getLog()->i(StringUtil::append("wavefilesize: ", wavefilesize));

				// check file format
				//if (fread(magic, 4,1,f) != 1) {
				if (fi_fread(magic, 4, 1, fi) != 1) {
					string errStr = "Could not load wav file: "; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr);
					return false;
				}
				ARK2D::getLog()->v(StringUtil::append("compare WAVE: ", string(magic)));
				if (string(magic) != "WAVE") {
					string errStr = "Could not read wav file. This is not a wav file (no WAVE format - 4byte chunk was '";
					errStr += string(magic);
					errStr += "' ): "; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr);
					return false;
				}

				// check 'fmt ' sub chunk (1)
				//if (fread(magic, 4,1,f) != 1) {
				if (fi_fread(magic, 4, 1, fi) != 1) {
					string errStr = "Could not load wav file: "; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr); return false;
				}
				ARK2D::getLog()->v(StringUtil::append("compare fmt : ", string(magic)));
				if (string(magic) != "fmt ") {
					string errStr = "Could not read wav file. This is not a wav file (no 'fmt ' subchunk): "; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr); return false;
				}

				// read (1)s size
				//if (fread(buffer32, 4, 1, f) != 1) {
				if (fi_fread(buffer32, 4, 1, fi) != 1) {
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
				//if (fread(buffer16, 2, 1, f) != 1) {
				if (fi_fread(buffer16, 2, 1, fi) != 1) {
					string errStr = "Could not load wav file: "; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr); return false;
				}
				unsigned short audioFormat = wav_readByte16(buffer16);
				if (audioFormat != 1) {
					string errStr = "Could not read wav file. This is not a wav file (audio format is not PCM): "; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr); return false;
				}

				// read number of channels
				//if (fread(buffer16, 2, 1, f) != 1) {
				if (fi_fread(buffer16, 2, 1, fi) != 1) {
					string errStr = "Could not load wav file. Could not read number of channels.\r\n "; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr); return false;
				}
				unsigned short channels = wav_readByte16(buffer16);

				// read frequency (sample rate)
				//if (fread(buffer32, 4, 1, f) != 1) {
				if (fi_fread(buffer32, 4, 1, fi) != 1) {
					string errStr = "Could not load wav file. Could not read frequency/sample rate. \r\n"; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr); return false;
				}
				unsigned long frequency = wav_readByte32(buffer32);

				// skip 6 bytes (Byte rate (4), Block align (2))
				//fseek(f,6,SEEK_CUR);
				//fi_fseek(fi,6,SEEK_CUR);
				char skipdata[6];
				fi_fread(skipdata, 6, 1, fi);

				// read bits per sample.
				//if (fread(buffer16, 2, 1, f) != 1) {
				if (fi_fread(buffer16, 2, 1, fi) != 1) {
					string errStr = "Could not load wav file. Could not read bits per sample. \r\n"; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr); return false;
				}
				unsigned short bps = wav_readByte16(buffer16);

				#if defined(ARK2D_XAUDIO2)
					format = 0;
				#else
					if (channels == 1) {
						format = (bps == 8)? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
					} else {
						format = (bps == 8)? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
					}
				#endif




				// check 'data' sub chunk (2)
				//if (fread(magic, 4, 1, f) != 1) {
				if (fi_fread(magic, 4, 1, fi) != 1) {
					string errStr = "Could not load wav file. Could not read 'data' subchunk 2. \r\n"; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr); return false;
				}

				// check for BEXT markers
				ARK2D::getLog()->v(StringUtil::append("compare bext: ", string(magic)));
				if (string(magic) == "bext") {
					string errStr = "Could not read wav file. There are bext markers which WE CAN'T PARSE. :( "; errStr += m_FileName;
					ARK2D::getLog()->e(errStr);
					//ErrorDialog::createAndShow(errStr);

					fi_fread(buffer32, 4, 1, fi);
					unsigned long bextsz = wav_readByte32(buffer32);
					ARK2D::getLog()->e(StringUtil::append("bext sz: ", bextsz));
					//ErrorDialog::createAndShow( StringUtil::append("bext sz: ", bextsz)) ;


					char* bextdata = (char*) alloca(bextsz);
					fi_fread(bextdata, bextsz, 1, fi);
					fi_fread(magic, 4, 1, fi);


					//return false;
				}

				// JUNK
				// http://tech.ebu.ch/docs/tech/tech3306-2009.pdf

				ARK2D::getLog()->v(StringUtil::append("compare junk: ", string(magic)));
				if (string(magic) == "junk") {
					//fi_fread(magic, 4, 1, fi);
					fi_fread(buffer32, 4, 1, fi);
					unsigned long junklen = wav_readByte32(buffer32);
					//char junkdata[junklen];
					char* junkdata = (char*) alloca(junklen);
					fi_fread(junkdata, junklen, 1, fi);
					fi_fread(magic, 4, 1, fi);

					ARK2D::getLog()->e("There was JUNK in the TRUNK. (junk in the wav file...)");
					//ErrorDialog::createAndShow(errStr); return false;
				}

				ARK2D::getLog()->v(StringUtil::append("compare data: ", string(magic)));
				if (string(magic) != "data") {
					ARK2D::getLog()->e(string(magic));
					string errStr = "Could not read wav file. This is not a wav file (no data subchunk): "; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr); return false;
				}

				// read subchunk 2 (data) size.
				//if (fread(buffer32, 4, 1, f) != 1) {
				if (fi_fread(buffer32, 4, 1, fi) != 1) {
					string errStr = "Could not load wav file. Could not read subchunk 2 size. \r\n"; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr); return false;
				}
				unsigned long subChunk2Size = wav_readByte32(buffer32);

				// the frequency of the sampling rate.
				freq = frequency;
				ARK2D::getLog()->v(StringUtil::append("compare frequency: ", string(magic)));
				//if (sizeof(freq) != sizeof(frequency)) {
				if (freq != frequency) {
					string errStr = "Could not load wav file. 'frequency' and 'freq' are different sizes. \r\n"; errStr += m_FileName;
					ErrorDialog::createAndShow(errStr); return false;
				}

				// read raw data.
				ARK2D::getLog()->v("read raw data...");
				array = new char[BUFFER_SIZE];
				while (data.size() != subChunk2Size) {
					//read a buffer's worth of data.
					//bytes = fread(array, 1, BUFFER_SIZE, f);
					bytes = fi_fread(array, 1, BUFFER_SIZE, fi);

					if (bytes <= 0) { break; }

					if (data.size() +bytes >subChunk2Size) {
						bytes = subChunk2Size - data.size();
					}

					// append to end of buffer.
					data.insert(data.end(), array, array + bytes);
				}

				ARK2D::getLog()->v("free memory...");
				delete[] array;
				array = NULL;

				/*#if defined(ARK2D_MACINTOSH)
					if (thisData != NULL) {
						free(thisData);
					}
				#endif*/

				//fclose(f);
				fi_fclose(fi);
				//f = NULL;
				fi = NULL;

				#if defined(ARK2D_XAUDIO2)
					unsigned int ubps = (unsigned int) bps;
					unsigned int ubyps = (unsigned int)bps / 8;
					unsigned int uchannels = (unsigned int) channels;
					unsigned int ufreq = (unsigned int) frequency;



					ARK2D::getLog()->v("WP8 bits...");
					ARK2D::getLog()->v(StringUtil::append("bps: ", ubps));
					ARK2D::getLog()->v(StringUtil::append("frequency: ", ufreq));
					ARK2D::getLog()->v(StringUtil::append("channels: ", uchannels));
					ARK2D::getLog()->v(StringUtil::append("datasize: ", data.size()));


					// http://msdn.microsoft.com/en-gb/library/windows/desktop/dd390970(v=vs.85).aspx
					m_xaWF.wFormatTag = WAVE_FORMAT_PCM;
					m_xaWF.nChannels =  uchannels;
					m_xaWF.nSamplesPerSec = ufreq;
					m_xaWF.nBlockAlign = ubyps * uchannels;
					m_xaWF.nAvgBytesPerSec = ufreq * ubyps * uchannels;
					m_xaWF.wBitsPerSample = bps;
					m_xaWF.cbSize = 0;// data.size();

					ARK2D::getLog()->v(StringUtil::append("datasize: ", data.size()));

					ARK2D::getLog()->v("Creating source voice callback...");
					m_xaCallbacks = new Xaudio2VoiceCallback();
					m_xaCallbacks->m_ark2dSoundPointer = this;

					ARK2D::getLog()->v("Creating source voice...");
					HRESULT hr = s_engine->CreateSourceVoice(&m_xaSource, &m_xaWF, 0, XAUDIO2_DEFAULT_FREQ_RATIO, m_xaCallbacks, NULL, NULL);
					if (FAILED(hr)) {
						ARK2D::getLog()->v(StringUtil::append("Could not create XAudio2 voice. ", getXAudio2Error(hr)));
						return false;
					}
					//m_xaSource->Start();

					ARK2D::getLog()->v("XAUDIO2_BUFFER struct");
					unsigned int sz = data.size();
					m_xaBuffer.Flags = XAUDIO2_END_OF_STREAM;
					m_xaBuffer.AudioBytes = sz;
					m_xaBuffer.pAudioData = new BYTE[sz];
					memcpy((void*)m_xaBuffer.pAudioData, (void*)&data[0], sz);
					m_xaBuffer.PlayBegin = m_xaPlayhead;
					m_xaBuffer.PlayLength  = 0;
					m_xaBuffer.LoopBegin = 0;
					m_xaBuffer.LoopLength = 0;
					m_xaBuffer.LoopCount = 0;
					m_xaBuffer.pContext = NULL;

					//ARK2D::getLog()->v("Submitting source buffer...");
					//HRESULT hr2 = m_xaSource->SubmitSourceBuffer( &m_xaBuffer );
					//if (FAILED(hr2)) {
					//	ARK2D::getLog()->v(StringUtil::append("Could not submit XAudio2 source buffer.", getXAudio2Error(hr2)));
					//	return false;
					//}
				#else

					//std::cout << "format: " << format << ". 16bitmono is " << AL_FORMAT_MONO16 << std::endl;
					//std::cout << "datasize: " << data.size() << std::endl;
					//std::cout << "frequency: " << frequency << std::endl;



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
				#endif

				// do we free the buffer manually now or does std::vector do it?
				// http://stackoverflow.com/questions/2035975/how-do-you-completely-remove-and-release-memory-of-an-openal-sound-file
				// free(data);
				data.clear();

				return true;

			#endif
			return false;
		}

		//! @todo: AL Doppler effects
		// alDopplerFactor(1.0f); // 1.2 is 20% pitch shift up.
		// alDopplerVelocity( 343.0f ); // m/s this may need to be scaled at some point

		#if defined(ARK2D_FLASCC)
			void Sound::__flasccInternal_setPlaying(bool b) {
				m_flascc_isSoundPlaying = b;
			}
		#endif


		void Sound::setOffset(float seconds) {
			#if defined(ARK2D_FLASCC)

			#elif defined(ARK2D_XAUDIO2)

			#else
				alSourcef(Source, AL_SEC_OFFSET, seconds);
			#endif
		}
		float Sound::getOffset() {
			#if defined(ARK2D_FLASCC)

			#elif defined(ARK2D_XAUDIO2)
				return 0.0f;
			#else
				float seconds;
				alGetSourcef(Source, AL_SEC_OFFSET, &seconds);
				return seconds;
			#endif
		}

		void Sound::play() {
			//ARK2D::getLog()->v(StringUtil::append("sound played: ", m_FileName));
			#if defined(ARK2D_FLASCC)
				/*const char* str = m_FileName.c_str();
				inline_as3(
					"import com.adobe.flascc.Console;\n"\
					"Console.s_console.playSound(CModule.readString(%0, %1));\n"
					: : "r"(str), "r"(strlen(str))
				);*/
				//m_flascc_channel = m_flascc_sound->play(0.0);
				if(!m_flascc_isSoundPlaying)
				{
					m_flascc_channel = m_flascc_sound->play(m_flascc_pausePosition);
					//m_flascc_channel->addEventListener(AS3::ui::flash::events::Event::SOUND_COMPLETE, AS3::ui::Function::_new(m_flascc_soundEventHandler, (void*) this));
					m_flascc_channel->addEventListener(AS3::ui::flash::events::Event::SOUND_COMPLETE, AS3::ui::Function::_new(m_flascc_soundEventHandler, (void*) this));
					m_flascc_channel->soundTransform = m_flascc_transform;

					m_flascc_isSoundPlaying = true;
				}
				else
				{
					stop();
					m_flascc_pausePosition = 0;
					play();
				}
			#elif defined(ARK2D_XAUDIO2)
				if (m_xaSource == NULL) { return; }
				if (isPlaying()) {
					stop();
				}

				m_xaBuffer.PlayBegin = m_xaPlayhead;
				HRESULT hr2 = m_xaSource->SubmitSourceBuffer(&m_xaBuffer);
				if (FAILED(hr2)) {
					ARK2D::getLog()->e(StringUtil::append("Could not submit XAudio2 source buffer.", getXAudio2Error(hr2)));
					return;
				}

				m_xaSource->Start(0);
				m_xaIsPlaying = true;

			#else
				alSourcePlay(Source);
			#endif
		}
		bool Sound::isPlaying() {
			#if defined(ARK2D_FLASCC)


				// Use AS3 to negate a 32bit int!
			   /* int someint = 123;
			    int intresult = 0;
			    inline_as3(
			        "%0 = -%1;\n"
			        : "=r"(intresult) : "r"(someint)
			    );*/

			    /*int playing = 0; // 0 for false, 1 for true.
			    const char* str = m_FileName.c_str();
				inline_as3(
					"import com.adobe.flascc.Console;\n"\
					"%0 = Console.s_console.isPlayingSound(CModule.readString(%1, %2));\n"
					: "=r"(playing) : "r"(str), "r"(strlen(str))
				);
				return (playing == 1);*/
				return m_flascc_isSoundPlaying;
			#elif defined(ARK2D_XAUDIO2)
				if (m_xaSource == NULL) { return false; }

				//XAUDIO2_VOICE_STATE state;
				//m_xaSource->GetState(&state);
				//m_xaIsPlaying = (state.BuffersQueued > 0) != 0;
				//return m_xaIsPlaying;
				return m_xaIsPlaying;
			#else
				ALint state;
				alGetSourcei(Source, AL_SOURCE_STATE, &state);
				return (state == AL_PLAYING);
			#endif
		}
		void Sound::stop() {
			#if defined(ARK2D_FLASCC)
				/*const char* str = m_FileName.c_str();
				inline_as3(
					"import com.adobe.flascc.Console;\n"\
					"Console.stopSound(CModule.readString(%0, %1));\n"
					: : "r"(str), "r"(strlen(str))
				);*/
				if (m_flascc_isSoundPlaying)
				{
					m_flascc_isSoundPlaying = false;
					m_flascc_channel->stop();
				}
			#elif defined(ARK2D_XAUDIO2)
				if (m_xaSource == NULL) { return; }
				m_xaSource->Stop(0); // XAUDIO2_PLAY_TAILS
				m_xaSource->FlushSourceBuffers();
				m_xaIsPlaying = false;
				m_xaPlayhead = 0;
			#else
				alSourceStop(Source);
			#endif
		}
		void Sound::pause() {
			#if defined(ARK2D_FLASCC)
				/*const char* str = m_FileName.c_str();
				inline_as3(
					"import com.adobe.flascc.Console;\n"\
					"Console.pauseSound(CModule.readString(%0, %1));\n"
					: : "r"(str), "r"(strlen(str))
				);*/
				if (m_flascc_isSoundPlaying)
				{
					m_flascc_isSoundPlaying = false;
					m_flascc_pausePosition = m_flascc_channel->position;
      				m_flascc_channel->stop();
      			}
      		#elif defined(ARK2D_XAUDIO2)
      			//ARK2D::getLog()->w("Sound::pause Not implemented");
      			if (m_xaSource == NULL) { return; }

				XAUDIO2_VOICE_STATE state;
				m_xaSource->GetState(&state);

				m_xaPlayhead = state.SamplesPlayed;
      			//stop();
      			//m_xaBuffer.AudioData.Position = 0;
				//m_xaSource->SubmitSourceBuffer(&m_xaBuffer);

				m_xaSource->Stop(0); // XAUDIO2_PLAY_TAILS
				m_xaSource->FlushSourceBuffers();
				m_xaIsPlaying = false;


			#else
				alSourcePause(Source);
			#endif
		}
		float Sound::getVolume() {
			return m_volume;
		}
		void Sound::setVolume(float volume) {
			m_volume = volume;
			#if defined(ARK2D_FLASCC)
				/*const char* str = m_FileName.c_str();
				inline_as3(
					"import com.adobe.flascc.Console;\n"\
					"Console.setSoundVolume(CModule.readString(%0, %1), %2);\n"
					: : "r"(str), "r"(strlen(str)), "r"(volume)
				);*/
				//if (m_flascc_channel != AS3::ui::internal::_null) { // set in ::load
					//if (m_flascc_channel->soundTransform != AS3::ui::internal::_null) {
          				m_flascc_transform->volume = volume;
          				m_flascc_channel->soundTransform = m_flascc_transform;
        			//}
      			//}
          	#elif defined(ARK2D_XAUDIO2)
				if (m_xaSource == NULL) { return; }
				m_xaSource->SetVolume(m_volume);
			#else
				alSourcef(Source, AL_GAIN, volume);
			#endif
		}

		float Sound::getPitch() {
			return m_pitch;
		}
		void Sound::setPitch(float pitch) {
			m_pitch = pitch;

			#if defined(ARK2D_FLASCC)

			#elif defined(ARK2D_XAUDIO2)
				ARK2D::getLog()->w("Sound::setPitch Not implemented");
			#else
				alSourcef(Source, AL_PITCH, pitch);
			#endif
		}

		void Sound::setPanning(float pan) {
			SourcePos[0] = pan;
			#if defined(ARK2D_FLASCC)

			#elif defined(ARK2D_XAUDIO2)
				ARK2D::getLog()->w("Sound::setPanning Not implemented");
			#else
				alSourcefv(Source, AL_POSITION, SourcePos);
			#endif
			//std::cerr << "OpenAL is broken -- seek alternative." << std::endl;
		}

		void Sound::setGroupId(unsigned int id) {
			m_groupId = id;
		}
		unsigned int Sound::getGroupId() {
			return m_groupId;
		}

		string Sound::getALErrorString(unsigned int err) {
			return getALErrorStringStatic(err);
		}
		string Sound::getALErrorStringStatic(unsigned int err) {
			#if defined(ARK2D_FLASCC)
				return string("UNKNOWN_ERROR");
			#elif defined(ARK2D_XAUDIO2)
				return string("UNKNOWN_ERROR");
			#else
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
				return string("AL_UNKNOWN_ERROR");
			#endif
		}
		string Sound::getOggErrorString(int code) {
			#if defined(ARK2D_FLASCC)
				return "";
			#elif defined(ARK2D_XAUDIO2)
				ARK2D::getLog()->w("Sound::getOggErrorString Not implemented");
				return "";
			#else
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
			#endif
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
			#if defined(ARK2D_FLASCC)

			#elif defined(ARK2D_XAUDIO2)
				ARK2D::getLog()->w("Sound::deinit not implemented");
			#else
				ARK2D::getLog()->v("Deinitialising Sound: OpenAL bits. ");

				if (Source == 0 || Buffer == 0) {
					ARK2D::getLog()->v("Sound OpenAL Source/Buffer was not valid.");
					return;
				}

				// make sure source and buffer ids are valid before trying to delete.
				if (alIsSource(Source)) {
					alSourceStop(Source);
					alDeleteSources(1, &Source);
				}
				if (alIsBuffer(Buffer)) {
					alDeleteBuffers(1, &Buffer);
				}
			#endif
		}

		Sound::~Sound() {
			ARK2D::getLog()->v(StringUtil::append("Deleting Sound: OpenAL bits. ", m_FileName));

			deinit();

			// remove from sound store.
			SoundStore* ss = SoundStore::getInstance();
			ss->removeSound(m_FileName);
		}
	}
}
