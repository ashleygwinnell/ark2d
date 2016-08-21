/*
 * Resource.cpp
 *
 *  Created on: Feb 12, 2012
 *      Author: ashleygwinnell
 */

#include "../Namespaces.h"

#include "ARK2D.h"
#include "Resource.h"

#include "Graphics/Image.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureStore.h"
#include "Graphics/SpriteSheetDescription.h"

#include "Font/FTFont.h"
#include "Font/BMFont.h"
#include "Font/Font.h"
#include "GameContainer.h"
#include "String.h"

#include "../Audio/Sound.h"
#include "../Tiled/TiledMap.h"
#include "../Path/PathGroup.h"
#include "../Path/PathIO.h"
#include "../Util/StringUtil.h"
#include "../Util/LocalHighscores.h"
#include "../Util/KeyPairFile.h"
#include "../Util/FileUtil.h"

//#include "../vendor/spine/includes/spine/spine.h"
//#include "../vendor/spine/SpineSkeleton.h"

namespace ARK {
	namespace Core {


		RawDataReturns::RawDataReturns(): data(NULL), size(0)
		{

		}
		RawDataReturns::~RawDataReturns()
		{
			ARK2D::getLog()->v("Deleting RawDataReturns");
			if (data != NULL) {
				free(data);
			}
			ARK2D::getLog()->v("Deleted RawDataReturns");
		}

		string Resource::s_latestName;

		#if defined(ARK2D_ANDROID)
			zip* Resource::apkZip = NULL;
			string Resource::apkZipName = "";
		#endif

		string Resource::latestName() {
			return s_latestName;
		}

		void Resource::setLatestName(string s) {
			s_latestName = s;
		}

		Resource* Resource::get(string ref) {
			return get(ref, true);
		}

		Resource* Resource::get(string ref, bool appendPath) {
			setLatestName(ref);

			GameContainer* container = ARK2D::getContainer();
			string oldref = ref;

			if (ref.substr(1,1).compare(":") == 0 || ref.substr(0,1).compare("/") == 0) { appendPath = false; }
			if (appendPath) {
				ref = container->getResourcePath() + ref;
			}

			ARK2D::getLog()->i(StringUtil::append("Loading Resource: ", ref));

			Resource* resource = NULL;
			string extension = StringUtil::getExtension(ref);
			if (extension == "fnt")
			{
				//string pngref = ref.substr(0, ref.find_last_of(".")) + ".png";
				string pngref = oldref.substr(0, oldref.find_last_of(".")) + ".png";
				#if defined(ARK2D_ANDROID)
					RawDataReturns* rt = getRawData(ref);

					Image* imgData = get(pngref)->asImage();
					resource = new ARK::Core::Font::BMFont(rt->data, imgData);

					delete rt;
				#else
                    resource = new ARK::Core::Font::BMFont(ref, pngref);
				#endif
			}
			/*else if (extension == "ttf")
			{
				#if defined(ARK2D_ANDROID)
					RawDataReturns* rt = getRawData(ref);

					resource = new ARK::Font::FTFont(rt->data);

					delete rt;
				#else
					resource = new ARK::Font::FTFont(ref);
				#endif
			}*/
			/*else if (extension == "spine")
			{
				ARK::Spine::Skeleton* skeleton = NULL;
				#if defined(ARK2D_ANDROID)
					string genericName = ref.substr(0, ref.find_last_of("."));
					string atlasFile = genericName + string(".atlas");
					string skeletonFile = genericName + string(".json");

					RawDataReturns* rt = getRawData(atlasFile);
					RawDataReturns* rt2 = getRawData(skeletonFile);

					//skeleton = new ARK::Spine::Skeleton(newtextbuffer, rt->size, newtextbuffer2, rt2->size);
					ARK2D::getLog()->i("New Skeleton object");
					skeleton = new ARK::Spine::Skeleton(rt->data, rt->size, rt2->data, rt2->size);
					skeleton->m_fname = ref;
					skeleton->load();

					//ARK2D::getLog()->e("Spine is currently not supported on Android...");
					//exit(0);

				#else
					skeleton = new ARK::Spine::Skeleton(ref);
					skeleton->load();
				#endif
				resource = skeleton;
			}*/
			else if (extension == "tmx")
			{
				ARK::Tiled::TiledMap* map = NULL;
				#if defined(ARK2D_ANDROID)
					RawDataReturns* rt = getRawData(ref);

					//char* newtextbuffer = (char*) realloc(rt->data, rt->size+1);
					//newtextbuffer[rt->size] = '\0';

					map = new ARK::Tiled::TiledMap(ref, rt->data);
					map->load();
				#else
					map = new ARK::Tiled::TiledMap(ref);
					map->load();
				#endif
				resource = map;
			}
			else if (extension == "kpf")
			{
				KeyPairFile* keypairfile = NULL;
				#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
					RawDataReturns* rt = getRawData(ref);

					char* newtextbuffer = (char*) realloc(rt->data, rt->size+1);
					newtextbuffer[rt->size] = '\0';

					keypairfile = new KeyPairFile(ref, newtextbuffer);

					ARK2D::getLog()->v("Freeing raw resource data... ");

					rt->data = newtextbuffer;
					delete rt;
					//free(newtextbuffer);
				#else
					keypairfile = new KeyPairFile(oldref);
				#endif
				resource = keypairfile;

			}
			else if (extension == "localhighscores")
			{
				LocalHighscores* scores = NULL;
				#if defined(ARK2D_ANDROID)
					RawDataReturns* rt = getRawData(ref);

					//char* newtextbuffer = (char*) malloc(rt->size+1);
					//memcpy(newtextbuffer, rt->data, rt->size);
					//newtextbuffer[rt->size] = '\0';

					char* newtextbuffer = (char*) realloc(rt->data, rt->size+1);
					newtextbuffer[rt->size] = '\0';

					//scores = new LocalHighscores(ref, rt->data);
					scores = new LocalHighscores(ref, newtextbuffer);

					ARK2D::getLog()->v("Freeing raw resource data... ");

					rt->data = newtextbuffer;
					delete rt;
					//free(newtextbuffer);
				#else
					scores = new LocalHighscores(oldref);
				#endif
				resource = scores;
			}
			else if (extension == "spritesheetdescription")
			{
				SpriteSheetDescription* desc = NULL;
				#if defined(ARK2D_ANDROID)
					RawDataReturns* rt = getRawData(ref);

					//char* newtextbuffer = (char*) malloc(rt->size+1);
					//memcpy(newtextbuffer, rt->data, rt->size);
					//newtextbuffer[rt->size] = '\0';

					char* newtextbuffer = (char*) realloc(rt->data, rt->size+1);
					newtextbuffer[rt->size] = '\0';

					desc = new SpriteSheetDescription(ref, newtextbuffer);

					ARK2D::getLog()->v("Freeing raw resource data... ");
					rt->data = newtextbuffer;
					delete rt;
					//free(newtextbuffer);
				#else
					desc = new SpriteSheetDescription(oldref);
				#endif
				resource = desc;
			}
			else if (extension == "png" || extension == "bmp" || extension == "tga" || extension == "pkm_png")
			{ // Image
				#if defined(ARK2D_ANDROID)
					ARK2D::getLog()->v("Creating raw resource data... ");
					RawDataReturns* rt = getRawData(ref);
					void* fileData = rt->data;

					ARK2D::getLog()->v("Creating resource type from data... ");
					resource = new Image(fileData, rt->size, getResourceTypeByExtension(extension), ref);
					//resource->asImage()->filename = ref;

					ARK2D::getLog()->v("Freeing raw resource data... ");
					delete rt;
				#else
					resource = new Image(ref);
				#endif
			}
			else if (extension == "wav" || extension == "ogg" || extension == "mp3")
			{ // Sound
				#if (defined(ARK2D_ANDROID) || defined(ARK2D_FLASCC))
					RawDataReturns* rt = getRawData(ref);
					resource = new Sound(ref, rt->data, rt->size);
					delete rt;
				#else
					resource = new Sound(ref);
				#endif
			}
			else if (extension == "path")
			{
				#if defined(ARK2D_ANDROID)
					ErrorDialog::createAndShow("Path implementation not on Android.");
				#elif defined(ARK2D_WINDOWS)
					resource = PathIO::createFromFile(oldref);
				#else
					resource = PathIO::createFromFile(ref);
				#endif
			}
			else
			{ // Assume plain text.
				String* arkstr = new String();
				#if defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE)
					RawDataReturns* rt = getRawData(ref);

					//ARK2D::getLog()->v("pre contents: ");
					//ARK2D::getLog()->v((char*) rt->data);

					/*ARK2D::getLog()->v(StringUtil::append("raw string return size: ", rt->size));
					char* newtextbuffer = (char*) malloc(rt->size + 1);
					memcpy(newtextbuffer, (char*) rt->data, rt->size);

					char* newtextbufferEnd = newtextbuffer + rt->size;
					newtextbufferEnd = '\0';*/
/*
					if (rt->size >= 1024) {
						unsigned int count = 0;
						while(count < rt->size) {
							memcpy(newtextbuffer, rt->data + count, 1024);
							count += 1024;
						}
					} else {
						memcpy(newtextbuffer, rt->data, rt->size);
					}


					newtextbuffer[rt->size] = '\0';*/

					/*char* newtextbuffer = (char*) malloc(rt->size+1);
					if (newtextbuffer == NULL) {
						printf("Out of memory\n");
   						exit(-1);
					}*/



   					/*char* newtextbuffer = (char*) realloc(rt->data, rt->size+1);
					if (newtextbuffer == NULL) {
						printf("Out of memory\n");
   						exit(-1);
					}*/

					/*if (rt->size + 1 > 1024) {
						signed int c = 0;
						signed int remaining = rt->size;
						do {
							signed int count = (remaining >= 1024)?1024:remaining;
							memcpy(newtextbuffer + c, ((char*)rt->data) + c, count);
							remaining -= count;
							c += count;
						} while (remaining > 0);
					} else {*/
					//	memcpy(newtextbuffer, rt->data, rt->size);
					//}

					//char* newtextbufferEnd = newtextbuffer + rt->size;
					//(*newtextbufferEnd) = '\0';

					//(*newtextbufferEnd) = '\0';
					//newtextbuffer[rt->size] = '\0';
					//strcpy(newtextbufferEnd, "\0");

					char* newtextbuffer = (char*) realloc(rt->data, rt->size+1);
					newtextbuffer[rt->size] = '\0';


					//ARK2D::getLog()->v("contents: ");
					//ARK2D::getLog()->v(newtextbuffer);

					arkstr->append(newtextbuffer);
					//arkstr->append("\0");

					//ARK2D::getLog()->v(StringUtil::append("len: ", arkstr->length()));

					/*string s((const char*) rt->data);
					arkstr->append(s);*/
					//ARK2D::getLog()->v(arkstr->get());

					rt->data = newtextbuffer;
					delete rt;
					//free(newtextbuffer);

				#elif defined(ARK2D_FLASCC)

					// return /local/ file if it exists, otherwise return default from inside swf.
					ARK2D::getLog()->v("Checking for Local Storage file first.");
					string flasccLocalFile = string("/local/") + oldref;
					if (exists(flasccLocalFile)) {
						string s = StringUtil::file_get_contents(flasccLocalFile.c_str());
						arkstr->append(s);
					} else {
						string s = StringUtil::file_get_contents(oldref.c_str());
						arkstr->append(s);
					}

				#else
					string s = StringUtil::file_get_contents(oldref.c_str());
					arkstr->append(s);
				#endif
				resource = arkstr;
			}

			ARK2D::getLog()->i(StringUtil::append("Loaded Resource: ", ref));

			#if defined(ARK2D_FLASCC)
				ARK2D::getLog()->i("Attempting GC");
				inline_as3(
					"import flash.system.System;\n"
					"System.pauseForGCIfCollectionImminent(0.25);\n"
					: :
				);
			#endif

			return resource;
		}

		void Resource::copy(string ref, string ref2) {
			bool appendPath = true;

			string oldref = ref;
			if (ref.substr(1,1).compare(":") == 0 || ref.substr(0,1).compare("/") == 0) { appendPath = false; }

			if (appendPath) {
				#if defined(ARK2D_ANDROID)
					GameContainer* container = ARK2D::getContainer();
					ref = container->getResourcePath() + ref;
				#else
					ref = StringUtil::internalOSAppends(ref);
				#endif
			}

			//#if defined(ARK2D_ANDROID)
				RawDataReturns* data = getRawData(ref);
				FileUtil::file_put_contents(ref2, (const char*) data->data, data->size);
				delete data;
			//#endif
		}

		bool Resource::exists(string ref) {
			return exists(ref, true);
		}
		bool Resource::exists(string ref, bool appendPath) {

			string oldref = string(ref);
			if (ref.substr(1,1).compare(":") == 0 || ref.substr(0,1).compare("/") == 0) { appendPath = false; }
			if (appendPath) {

				#if defined(ARK2D_ANDROID)
					GameContainer* container = ARK2D::getContainer();
					ref = container->getResourcePath() + ref;
				#else
					ref = StringUtil::internalOSAppends(ref);
				#endif
			}

			ARK2D::getLog()->v(StringUtil::append("Does Resource Exist: ", ref));

			#if defined(ARK2D_IPHONE)

				// check Documents folder first.
				NSArray* searchPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
				NSString* documentsPath = [searchPaths objectAtIndex:0];
				string respath( [ documentsPath UTF8String ] );

				string localRef = respath + string("/") + oldref;
				ARK2D::getLog()->v(StringUtil::append("Does (Local) Resource Exist: ", localRef));

				bool localExists = StringUtil::file_exists(localRef.c_str());
				if (localExists) {
					return true;
				}

				return StringUtil::file_exists(ref.c_str());

			#elif defined(ARK2D_FLASCC)

				// check if it exists in /local/ before anywhere else.
				string flasccLocalFile = string("/local/") + oldref;
				bool localExists = StringUtil::file_exists(flasccLocalFile.c_str());
				if (localExists) {
					return true;
				}

				return StringUtil::file_exists(oldref.c_str());

			#elif defined(ARK2D_ANDROID)
				if (apkZip == NULL) {
					init();
				}

				string anotherref = string(oldref); //StringUtil::internalOSAppends(oldref);
				bool rawFileExists = StringUtil::file_exists(anotherref.c_str());
				if (rawFileExists) {
					return true;
				}

				//Just for debug, print APK contents
				int numFiles = zip_get_num_files(apkZip);
				for (int i=0; i<numFiles; i++)
				{
					const char* name = zip_get_name(apkZip, i, 0);
					if (name == NULL)
					{
						String thisErr("Error reading zip file name at index ");
						thisErr += i;
						thisErr += " : ";
						thisErr += zip_strerror(apkZip);
						ARK2D::getLog()->e(thisErr.get());
						return false;
					}
					if (string(name) == ref) {
						return true;
					}
				}

				return false;


			#else
				return StringUtil::file_exists(oldref.c_str());
			#endif
		}

		void Resource::init() {
			#if defined(ARK2D_ANDROID)
				if (apkZip == NULL) {
					apkZip = zip_open(apkZipName.c_str(), 0, NULL);
					if (apkZip == NULL) {
						string errorMessage = StringUtil::append("Error Loading APK: ", apkZipName);
						ARK2D::getLog()->e(errorMessage);
						//return NULL;
						ErrorDialog::createAndShow(errorMessage);
						exit(0);
					}
					//Just for debug, print APK contents
					int numFiles = zip_get_num_files(apkZip);
					for (int i=0; i<numFiles; i++)
					{
						const char* name = zip_get_name(apkZip, i, 0);
						if (name == NULL)
						{
							String thisErr("Error reading zip file name at index ");
							thisErr += i;
							thisErr += " : ";
							thisErr += zip_strerror(apkZip);
							ARK2D::getLog()->e(thisErr.get());
							ErrorDialog::createAndShow(thisErr.get());
							//return NULL;
							exit(0);
						}
						String thisErr("File ");
						thisErr += i;
						thisErr += " : ";
						thisErr += name;
						ARK2D::getLog()->v(thisErr.get());
					}
				}
			#endif
		}

		RawDataReturns* Resource::getRawData(string ref) {
			#if defined(ARK2D_ANDROID)
				if (apkZip == NULL) {
					init();
				}

				bool useoldref = (ref.substr(0,7).compare("assets/") == 0);
				string oldref = ref.substr(7, string::npos);
				bool rawFileExists = StringUtil::file_exists(oldref.c_str());
				if (rawFileExists) {

					string contents = StringUtil::file_get_contents(oldref.c_str());

					char* newData = (char*) malloc((int) contents.size()+1);
					strcpy(newData, contents.c_str());

					//newData[(int) contents.size()] = '\0';

					RawDataReturns* rt = new RawDataReturns();
					rt->data = (void*) newData;
					rt->size = (int) contents.length();
					return rt;


					/*file_get_contents_binary_result r = FileUtil::file_get_contents_binary(oldref);

					ARK2D::getLog()->v("getting raw data: android platform");
					ARK2D::getLog()->v(StringUtil::append("len: ", r.len));

					RawDataReturns* rt = new RawDataReturns();
					rt->data = (void*) r.data;
					rt->size = (int) r.len;
					return rt; */



				}

				zip_file* file;
				file = zip_fopen(apkZip, ref.c_str(), 0); // ref = filename
				if (!file) {
					ARK2D::getLog()->e(StringUtil::append("Could not open file in zip (return empty file): ", ref));
					//return NULL;

					RawDataReturns* rt = new RawDataReturns();
					rt->data = NULL;
					rt->size = (int) 0;
					return rt;

				}
				struct zip_stat fileStats;
				zip_stat(apkZip, ref.c_str(), 0, &fileStats); // ZIP_FL_NOCASE | ZIP_FL_NODIR

				String fileStatsStr("Resource: ");
				fileStatsStr += ref;
				fileStatsStr += ". uncompressed size: ";
				fileStatsStr += (int) fileStats.size;
				fileStatsStr += ". compressed size: ";
				fileStatsStr += (int) fileStats.comp_size;
				ARK2D::getLog()->v(fileStatsStr.get());

				char* fileUncompressed = (char*) malloc((int) fileStats.size);
				int fileUncompressedPointer = 0;
				unsigned int fileUncompressedBufferSize = 8192;
				if (fileUncompressedBufferSize > (int) fileStats.size) {
					fileUncompressedBufferSize = (int) fileStats.size;
				}
				for(signed int i = 0; i < (int) fileStats.size; i += fileUncompressedBufferSize) {
					char fileUncompressedBuffer[fileUncompressedBufferSize];
					signed int done = zip_fread(file, &fileUncompressedBuffer, fileUncompressedBufferSize);

					if (done == -1) { break; }
					memcpy(fileUncompressed + i, &fileUncompressedBuffer[0], done);
				}
				zip_fclose(file);

				RawDataReturns* rt = new RawDataReturns();
				rt->data = (void*) fileUncompressed;
				rt->size = (int) fileStats.size;
				return rt;

			#elif defined(ARK2D_IPHONE)

				// check Documents first.
				int findit = ref.find(".app/data/");
				bool useoldref = (findit != string::npos);
				if (useoldref) {
					string oldref = ref.substr(findit+10, string::npos);

					NSArray* searchPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
					NSString* documentsPath = [searchPaths objectAtIndex:0];
					string respath( [ documentsPath UTF8String ] );
					string localRef = respath + string("/") + oldref;
					ARK2D::getLog()->v(StringUtil::append("Does (Local) Resource Exist: ", localRef));

					bool localExists = StringUtil::file_exists(localRef.c_str());
					if (localExists) {
						file_get_contents_binary_result r = FileUtil::file_get_contents_binary(localRef);

						RawDataReturns* rt = new RawDataReturns();
						rt->data = (void*) r.data;
						rt->size = (int) r.len;
						return rt;
					}
				}

				file_get_contents_binary_result r = FileUtil::file_get_contents_binary(ref);

				RawDataReturns* rt = new RawDataReturns();
				rt->data = (void*) r.data;
				rt->size = (int) r.len;
				return rt;

			#else
				file_get_contents_binary_result r = FileUtil::file_get_contents_binary(ref);

				//ARK2D::getLog()->v("getting raw data: misc platform");
				//ARK2D::getLog()->v(StringUtil::append("len: ", r.len));

				RawDataReturns* rt = new RawDataReturns();
				rt->data = (void*) r.data;
				rt->size = (int) r.len;
				return rt;

			#endif
			return NULL;
		}
		unsigned int Resource::getResourceTypeByExtension(string extension) {
			if (extension == "tga") {
				return ARK2D_RESOURCE_TYPE_TGA;
			} else if (extension == "bmp") {
				return ARK2D_RESOURCE_TYPE_BMP;
			} else if (extension == "png") {
				return ARK2D_RESOURCE_TYPE_PNG;
			} else if (extension == "pkm_png") {
				return ARK2D_RESOURCE_TYPE_PKM;
			} else if (extension == "fnt") {
				return ARK2D_RESOURCE_TYPE_FNT;
			} else if (extension == "localhighscores") {
				return ARK2D_RESOURCE_TYPE_LOCAL_HIGHSCORES;
			} else if (extension == "tmx") {
				return ARK2D_RESOURCE_TYPE_TILED_MAP;
			} else if (extension == "spritesheetdescription") {
				return ARK2D_RESOURCE_TYPE_SPRITESHEET_DESCRIPTION;
			} else if (extension == "txt") {
				return ARK2D_RESOURCE_TYPE_TXT;
			} else if (extension == "kpf") {
				return ARK2D_RESOURCE_TYPE_KEYPAIRFILE;
			}
			return 0;
		}

		Resource::Resource() {

		}

		Image* Resource::asImage() {
			return dynamic_cast<Image*>(this);
		}
		Sound* Resource::asSound() {
			return dynamic_cast<ARK::Audio::Sound*>(this);
		}
        ARK::Core::Font::Font* Resource::asFont() {
			return dynamic_cast<ARK::Core::Font::Font*>(this);
		}
		LocalHighscores* Resource::asLocalHighscores() {
			return dynamic_cast<ARK::Util::LocalHighscores*>(this);
		}
		PathGroup* Resource::asPathGroup() {
			return dynamic_cast<ARK::Path::PathGroup*>(this);
		}
		TiledMap* Resource::asTiledMap() {
			return dynamic_cast<ARK::Tiled::TiledMap*>(this);
		}
		SpriteSheetDescription* Resource::asSpriteSheetDescription() {
			return dynamic_cast<ARK::Core::Graphics::SpriteSheetDescription*>(this);
		}
		KeyPairFile* Resource::asKeyPairFile() {
			return dynamic_cast<ARK::Util::KeyPairFile*>(this);
		}
		//ARK::Spine::Skeleton* Resource::asSkeleton() {
		//	return dynamic_cast<ARK::Spine::Skeleton*>(this);
		//}
		String* Resource::asString() {
			return dynamic_cast<String*>(this);
		}
        Resource& Resource::operator=(const Resource& r) {
			return *this;
		}

		Resource::~Resource() {

		}

	}
}
