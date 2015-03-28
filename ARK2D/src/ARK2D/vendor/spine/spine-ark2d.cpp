
// ARK2D edits:
// - renamed Animation.c to AnimationSp.c
// - renamed Event.c to EventSp.c

// https://github.com/EsotericSoftware/spine-runtimes/blob/master/spine-c/README.md
#include <spine/spine.h>
#include <spine/extension.h>
 
#include "../../Includes.h" 
#include "../../Core/Resource.h"
#include "../../Graphics/Image.h"
#include "../../Graphics/Texture.h"
#include "../../Graphics/TextureStore.h"

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path) {

	string oldfname = StringUtil::append("", path);
	string fname = StringUtil::append("", path);
	#if defined(ARK2D_WINDOWS) 
		bool windowsWeirdPath = fname.substr(7).compare("./data/") != 0;
		if (windowsWeirdPath) {
			fname = fname.substr(7); 
		}
	#endif



	ARK2D::getLog()->v(StringUtil::append("Original fname: ", path));
	ARK2D::getLog()->v(StringUtil::append("New fname: ", fname));

	Image* img = NULL;
	TextureStore* ts = TextureStore::getInstance();
	if (ts->hasTexture(oldfname)) { 
		Texture* tx = ts->getTexture(oldfname);
		img = tx->m_resource;
	} else {
		ts->print();

		#if defined(ARK2D_UBUNTU_LINUX)
		 	bool linuxWeirdPath = fname.substr(5).compare("data/") != 0;
			if (linuxWeirdPath) {
				fname = fname.substr(5); 
			}
		#endif

		img = ARK::Core::Resource::get(fname)->asImage();
	}

	self->rendererObject = img; 
	self->width = img->getWidth();
	self->height = img->getHeight();
}
void _spAtlasPage_disposeTexture (spAtlasPage* self) {
	delete (Image*) self->rendererObject;
}
char* _spUtil_readFile (const char* path, int* length) {
	return _readFile(path, length);
}


#if defined(ARK2D_WINDOWS_PHONE_8)

	#ifdef MALLOC_STR
		#undef MALLOC_STR
		#define MALLOC_STR(TO,FROM) strcpy_s(CONST_CAST(char*, TO) = (char*)malloc(strlen(FROM) + 1), strlen(FROM), FROM)

	#endif

#endif