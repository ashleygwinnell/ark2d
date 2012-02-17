/*
 * Resource.cpp
 *
 *  Created on: Feb 12, 2012
 *      Author: ashleygwinnell
 */

#include "ARK2D.h"
#include "Resource.h"

#include "Image/Image.h"
#include "Audio/Sound.h"
#include "Font/FTFont.h"
#include "Font/BMFont.h"
#include "Font/Font.h"
#include "GameContainer.h"
#include "Util/StringUtil.h"
#include "ARKString.h"

namespace ARK {

	#if defined(ARK2D_ANDROID)
		zip* Resource::apkZip = NULL;
		string Resource::apkZipName = "";
	#endif

	Resource* Resource::get(string ref) {
		return get(ref, true);
	}

	Resource* Resource::get(string ref, bool appendPath) {
		GameContainer* container = ARK2D::getContainer();
		if (appendPath) {
			ref = container->getResourcePath() + ref;
		}

		ARK2D::getLog()->i(StringUtil::append("Loading Resource: ", ref));

		Resource* resource = NULL;
		string extension = StringUtil::getExtension(ref);
		if (extension == "fnt") {
			string pngref = ref.substr(0, ref.find_last_of(".")) + ".png";
			#if defined(ARK2D_ANDROID)
				void* fntData = getRawData(ref);
				Image* imgData = get(pngref, false)->asImage();
				resource = new BMFont(fntData, imgData);

				free(fntData);
			#else
				resource = new BMFont(ref, pngref);
			#endif
		}
		else if (extension == "png" || extension == "bmp" || extension == "tga") { // Image
			#if defined(ARK2D_ANDROID)
				ARK2D::getLog()->i("Creating raw resource data... ");
				void* fileData = getRawData(ref);

				ARK2D::getLog()->i("Creating resource type from data... ");
				resource = new Image(fileData, getResourceTypeByExtension(extension));

				ARK2D::getLog()->i("Freeing raw resource data... ");
				free(fileData);
			#else
				resource = new Image(ref);
			#endif
		} else if (extension == "wav" || extension == "ogg") { // Iound
			resource = new Sound(ref);
		} else { // Assume plain text.

		}

		ARK2D::getLog()->i(StringUtil::append("Loaded Resource: ", ref));

		return resource;
	}
	void* Resource::getRawData(string ref) {
		#if defined(ARK2D_ANDROID)
			if (apkZip == NULL) {
				apkZip = zip_open(apkZipName.c_str(), 0, NULL);
				if (apkZip == NULL) {
					ARK2D::getLog()->i(StringUtil::append("Error Loading APK: ", apkZipName));
					return NULL;
				}
				//Just for debug, print APK contents
				int numFiles = zip_get_num_files(apkZip);
				for (int i=0; i<numFiles; i++)
				{
					const char* name = zip_get_name(apkZip, i, 0);
					if (name == NULL)
					{
						ARKString thisErr("Error reading zip file name at index ");
						thisErr += i;
						thisErr += " : ";
						thisErr += zip_strerror(apkZip);
						ARK2D::getLog()->i(thisErr.get());
						return NULL;
					}
					ARKString thisErr("File ");
					thisErr += i;
					thisErr += " : ";
					thisErr += name;
					ARK2D::getLog()->i(thisErr.get());
				}
			}
			zip_file* file;
			file = zip_fopen(apkZip, ref.c_str(), 0); // ref = filename
			if (!file) {
				ARK2D::getLog()->i(StringUtil::append("Could not open file in zip: ", ref));
				return NULL;
			}
			struct zip_stat fileStats;
			zip_stat(apkZip, ref.c_str(), 0, &fileStats); // ZIP_FL_NOCASE | ZIP_FL_NODIR

			ARKString fileStatsStr("Resource: ");
			fileStatsStr += ref;
			fileStatsStr += ". uncompressed size: ";
			fileStatsStr += (int) fileStats.size;
			fileStatsStr += ". compressed size: ";
			fileStatsStr += (int) fileStats.comp_size;
			ARK2D::getLog()->i(fileStatsStr.get());

			char* fileUncompressed = (char*) malloc((int) fileStats.size);
			int fileUncompressedPointer = 0;
			unsigned int fileUncompressedBufferSize = 4096;
			if (fileUncompressedBufferSize > (int) fileStats.size) {
				fileUncompressedBufferSize = (int) fileStats.size;
			}
			for(signed int i = 0; i < (int) fileStats.size; i += fileUncompressedBufferSize) {
				char fileUncompressedBuffer[fileUncompressedBufferSize];
				signed int done = zip_fread(file, &fileUncompressedBuffer, fileUncompressedBufferSize);
				if (done == -1) { break; }
				memcpy(fileUncompressed + i, fileUncompressedBuffer, done);
			}
			zip_fclose(file);

			return (void*) fileUncompressed;
		#else
			return NULL;
		#endif
	}
	unsigned int Resource::getResourceTypeByExtension(string extension) {
		if (extension == "tga") {
			return ARK2D_RESOURCE_TYPE_TGA;
		} else if (extension == "bmp") {
			return ARK2D_RESOURCE_TYPE_BMP;
		} else if (extension == "png") {
			return ARK2D_RESOURCE_TYPE_PNG;
		} else if (extension == "fnt") {
			return ARK2D_RESOURCE_TYPE_FNT;
		} else if (extension == "txt") {
			return ARK2D_RESOURCE_TYPE_TXT;
		}
		return 0;
	}

	Resource::Resource() {

	}

	Image* Resource::asImage() {
		return dynamic_cast<Image*>(this);
	}
	Sound* Resource::asSound() {
		return dynamic_cast<Sound*>(this);
	}
	ARK::Font* Resource::asFont() {
		return dynamic_cast<ARK::Font*>(this);
	}

	Resource::~Resource() {

	}

}
