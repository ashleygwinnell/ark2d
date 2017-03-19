/*
 * Sound.h
 *
 *  Created on: 14-Dec-2009
 *      Author: Ashley
 */

#ifndef SOUND_H_
#define SOUND_H_

//
// http://www.devmaster.net/articles/openal-tutorials/lesson4.php
//

#include "../Namespaces.h"
#include "../Common/DLL.h"
#include "../Common/Audio.h"
#include "../Core/Resource.h"

#include <string>
using std::string;

#if defined(ARK2D_FLASCC)
	#include <AS3/AS3++.h>
 	//using namespace AS3::ui;
#endif

#if (defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_XBOXONE) || defined(ARK2D_WINDOWS_STORE))

	class Xaudio2VoiceCallback : public IXAudio2VoiceCallback
	{
		public:
		  //  HANDLE hBufferEndEvent;
			Sound* m_ark2dSoundPointer;

			Xaudio2VoiceCallback();
			~Xaudio2VoiceCallback();

			//Called when the voice has just finished playing a contiguous audio stream.
			//void OnStreamEnd();
			STDMETHOD_(void, OnStreamEnd) (THIS);

			// Called just before this voice's processing pass begins.
			STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired);;

			// Called just after this voice's processing pass ends.
			STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS);

			//Unused methods are stubs
		   // void OnVoiceProcessingPassEnd();
			//void OnVoiceProcessingPassStart(UINT32 SamplesRequired);
			//void OnBufferEnd(void * pBufferContext);
			//void OnBufferStart(void * pBufferContext);
			//void OnLoopEnd(void * pBufferContext);
			//void OnVoiceError(void * pBufferContext, HRESULT Error);

			// Called when this voice is about to start processing a new buffer.
			STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext);

			// Called when this voice has just finished processing a buffer.
			// The buffer can now be reused or destroyed.
			STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext);

			// Called when this voice has just reached the end position of a loop.
			STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext);

			STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error);;
	};

#endif

namespace ARK {
	namespace Audio {

		/*!
		 * \brief Use to play sound effects.
		 *
		 * @todo For larger files, introduce a Music class which can stream files and not take up lots of memory and big load time on mobile devices.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Sound : public ARK::Core::Resource {
			friend class ARK::Core::GameContainer;
			friend class ARK::Core::GameContainerPlatform;

			public:
				static unsigned int DEFAULT_GROUP_ID;
				static void setDefaultGroupId(unsigned int id);

			public:
				Sound(const std::string& filename);
				Sound(const std::string& filename, void* data, int dataLength);
				void play();
				void setOffset(float seconds);
				float getOffset();
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
				string getFilename();
				~Sound();
			protected:
				// File Name
				std::string m_FileName;

				// sound group - set volume by sound group.
				unsigned int m_groupId;

				// preloaded data, e.g. on Android using Resource::get().
				void* m_preloadedData;
				int m_preloadedDataLength;

				// Buffers hold sound data.
				unsigned int Buffer;

				// Sources are points emitting sound.
				unsigned int Source;

				// FLASCC
				#if defined(ARK2D_FLASCC)
					AS3::ui::flash::media::Sound m_flascc_sound;
					AS3::ui::flash::media::SoundChannel m_flascc_channel;
					AS3::ui::flash::media::SoundTransform m_flascc_transform;
					bool m_flascc_isSoundPlaying;
					signed int m_flascc_pausePosition;
					public:
						void __flasccInternal_setPlaying(bool b);
					protected:

					//AS3::ui::var m_flascc_soundEventHandler(void* arg, AS3::ui::var as3Args);
				#endif

				// Position of the source sound.
				float SourcePos[3]; // = { 0.0, 0.0, 0.0 };

				// Velocity of the source sound.
				float SourceVel[3]; // = { 0.0, 0.0, 0.0 };

				// Position of the Listener.
			private:
				static float ListenerPos[3]; // = { 0.0, 0.0, 0.0 };

				// Velocity of the Listener.
				static float ListenerVel[3]; // = { 0.0, 0.0, 0.0 };

				// Orientation of the Listener. (first 3 elements are "at", second 3 are "up"):; Also note that these should be units of '1'.
				static float ListenerOri[6]; // = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

			protected:
				// Volume, no need to store internally to be honest, but keep it simples, yarp!
				float m_volume;
				float m_pitch;



				void setSourcePosition(float x, float y, float z);
				void setSourceVelocity(float x, float y, float z);
				static void setListenerPosition(float x, float y, float z);
				static void setListenerVelocity(float x, float y, float z);
				static void setListenerOrientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z);
				bool load(bool loop);
				bool loadWAV(bool loop);
				bool loadOGG(bool loop);

				void clearErrors();

			protected:
				void miscerror(string ss);
				string getALErrorString(unsigned int err);
				string getOggErrorString(int code);

				void deinit();

				static string getALErrorStringStatic(unsigned int err);

				static unsigned short wav_readByte16(const unsigned char buffer[2]);
				static unsigned int wav_readByte32(const unsigned char buffer[4]);

			public:
				// WP8
				#if (defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_XBOXONE) || defined(ARK2D_WINDOWS_STORE))
					static IXAudio2* s_engine;
					static IXAudio2MasteringVoice* s_master;
					static void initialiseXAudio();
					static string getXAudio2Error(HRESULT hr);

					static void xa_startEngine();
					static void xa_stopEngine();

					IXAudio2SourceVoice* m_xaSource;
					WAVEFORMATEX m_xaWF;
					XAUDIO2_BUFFER m_xaBuffer;
					Xaudio2VoiceCallback* m_xaCallbacks;
					bool m_xaIsPlaying;
					unsigned int m_xaPlayhead;
				#endif
		};
	}
}

#endif /* SOUND_H_ */

