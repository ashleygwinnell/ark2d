/*
 * Resource.h
 *
 *  Created on: Feb 12, 2012
 *      Author: ashleygwinnell
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <string>
using std::string;

#include "../Windres.h"
#include "../Namespaces.h"
#include "../Common/DLL.h"



#if defined(ARK2D_ANDROID)
	#include <zip.h>
#endif

namespace ARK {
	namespace Core {



		class ARK2D_API RawDataReturns {
			friend class ARK::Tiled::TiledMap;
			//friend class ARK::Tiled::TiledMapParser_TinyXml;
			friend class ARK::Tiled::TiledMapParser_RapidXml;
			friend class ARK::Tiled::TiledMapParser_JSON;
			public:
				void* data;
				int size;
				RawDataReturns();
				~RawDataReturns();
		};

		/*!
		 * \brief Load assets in to game. Images, Sounds, TiledMaps, LocalHighscores, Text files, Fonts, etc.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Resource {
            friend class ARK::Core::Graphics::TextureStore;
			//friend class ARK::Tiled::TiledMapParser_TinyXml;
			friend class ARK::Tiled::TiledMapParser_RapidXml;
			friend class ARK::Tiled::TiledMapParser_JSON;

			public:
				static Resource* get(string ref);
				static bool exists(string ref);
				static string latestName();
				static void setLatestName(string s);
				static void copy(string ref, string ref2);
#if defined(ARK2D_ANDROID) || defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_WINDOWS)
				public:
			#else
				protected:
			#endif

				static Resource* get(string ref, bool appendPath);
				static bool exists(string ref, bool appendPath);
				static RawDataReturns* getRawData(string ref); // remember to free() this data.
				static unsigned int getResourceTypeByExtension(string extension);
				static string s_latestName;

			protected:
				static void init();
				static string fixpath(string ref, bool appendPath);

			public:

				#if defined(ARK2D_ANDROID)
					static zip* apkZip;
					static string apkZipName;
				#endif


				Resource();
				Image* asImage();
				Sound* asSound();
				ARK::Core::Font::Font* asFont();
				LocalHighscores* asLocalHighscores();
				//TiledMap* asTiledMap();
				ARK::Core::String* asString();
				//PathGroup* asPathGroup();
				SpriteSheetDescription* asSpriteSheetDescription();
				KeyPairFile* asKeyPairFile();
				//ARK::Spine::Skeleton* asSkeleton();
				Resource& operator=(const Resource& r);
				virtual ~Resource();




		};
	}
}

#endif
