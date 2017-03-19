/*
 * main.cpp
 *
 *  Created on: 7 Jun 2011
 *      Author: Ashley
 */

#if defined(__AVM2__)
 #include <AS3/AS3.h>
#endif

#if defined(ARK2D_XBOXONE) || defined(ARK2D_WINDOWS_STORE)
	#include <pch.h>
#endif

#if defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_XBOXONE) || defined(ARK2D_WINDOWS_STORE)
	int main(Platform::Array<Platform::String^>^ args) {
		return 0;
	}
#elif defined(ARK2D_EMSCRIPTEN_JS)

#else
	int main(int argc, char *argv[]) {
		return 0;
	}
#endif
