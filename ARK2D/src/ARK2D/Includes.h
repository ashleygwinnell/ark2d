/*
 * Includes.h
 *
 *  Created on: Apr 11, 2012
 *      Author: ashleygwinnell
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

 	#if defined(__AVM2__)

 		#ifndef ARK2D_FLASCC
			#define ARK2D_FLASCC
		#endif

 		#define STL_AVAILABLE
		#define EXCEPTIONS_AVAILABLE
 		#include <AS3/AS3.h>
 		//#include <AS3/AS3++.h>
 		#include <Flash++.h>
 		//using namespace AS3;



 		#include <string>
		#include <vector>
		#include <list>
		#include <map>
 		#include <set>
		#include <sstream>
		#include <iostream>
		#include <fstream>
		#include <sys/time.h>
		#include <time.h>
		#include <stdint.h>
		#include <stdio.h>
		#include <stdlib.h>
		#include <sys/stat.h>
		#include <sys/types.h>
		#include <math.h>
		#include <algorithm>
 		#include <unistd.h>

 		#include "Common/OpenGL.h"

 		using namespace std;

 		typedef unsigned int ALenum;
 		typedef unsigned int ALuint;
 		typedef float ALfloat;

 		#include "vendor/libcurl/curl.h"

 	#elif defined(ARK2D_EMSCRIPTEN_JS)

 		#define STL_AVAILABLE
		//#define EXCEPTIONS_AVAILABLE
		#define JSON_SAFE
		//#define ALLOW_MEMORY_GROWTH

 		#include "Common/Audio.h"
 		#include "Common/OpenGL.h"

		#include <vector>
		#include <list>
		#include <map>
		#include <set>
		#include <limits>

		#include <stdlib.h>
		#include <stdint.h>
		#include <inttypes.h>
		#include <string>
		#include <iostream>
		#include <fstream>
		#include <sys/time.h>
		#include <sys/ioctl.h>
		#include <sys/stat.h>
		#include <pthread.h>
		#include <unistd.h>

		//typedef signed long int64_t;
		#define typeof __typeof__

		#include "vendor/libcurl/curl.h"

 		using namespace std;

	#elif defined(ARK2D_ANDROID)
 		#define STL_AVAILABLE
 		//#define EXCEPTIONS_AVAILABLE
 		#define JSON_SAFE

		#include <string>
		#include <vector>
		#include <list>
		#include <map>
 		#include <set>
		#include <sstream>
		#include <iostream>
		#include <fstream>
		#include <sys/time.h>
		#include <time.h>

		#include <stdint.h>
		#include <stdio.h>
		#include <stdlib.h>
		#include <zip.h>
		#include <sys/stat.h>
		#include <sys/types.h>
		#include <math.h>
		#include <algorithm>
		#include <unistd.h>

 		#include "Common/Audio.h"
 		#include "Common/OpenGL.h"
 		#include "Common/DLL.h"
 		#include "Common/Platform.h"


	 	using namespace std;

	#elif defined(ARK2D_IPHONE)

	 	#define STL_AVAILABLE
		#define EXCEPTIONS_AVAILABLE
		#define JSON_SAFE

 		#include <stdio.h>
		#include <stdlib.h>
		#include <unistd.h>
		#include <sys/types.h>
 		#include <sys/time.h>

		#include <iostream>
 		#include <sstream>
 		#include <fstream>
 		#include <string>

 		#include <vector>
		#include <list>
		#include <map>
 		#include <set>

 		#include <math.h>

 		using namespace std;



		#include <AVFoundation/AVFoundation.h>

		#include <OpenGLES/EAGL.h>
		#include <QuartzCore/QuartzCore.h>

 		#include "Common/Audio.h"
		#include "Common/OpenGL.h"

 		#include <GameKit/GameKit.h>
		#include "TargetConditionals.h"

		#include "vendor/iphone/ga/GAI.h"
		#include "vendor/iphone/ga/GAIDictionaryBuilder.h"
		#include "vendor/iphone/ga/GAIFields.h"

 		#include "Common/DLL.h"



		#if (TARGET_IPHONE_SIMULATOR)
 			#define ARK2D_IPHONE_SIMULATOR
		#endif

//		#include <EGL/egl.h>


        //typedef unsigned int ALuint;
        //typedef unsigned int ALenum;
        //typedef float ALfloat;

    #elif defined(_XBOX_ONE)

 		#define STL_AVAILABLE
		#define EXCEPTIONS_AVAILABLE
		#define JSON_SAFE

		#define ARK2D_MISSING_C99_FUNCTIONS
		#define ARK2D_MISSING_C99_FUNCTIONDEFINITIONS

 		#include "Common/DLL.h"
		#include <xdk.h>
		#include <winsock2.h>


		#include <windows.h>
		#include <wrl.h>

		#include <vector>
		#include <list>
		#include <map>
		#include <set>

		#include <stdlib.h>
		#include <string>
		#include <time.h>


		#include <cstdlib>
		#include <fstream>

		#include <math.h>
		#include <cmath>
		#include <iostream>

		#include "Common/OpenGL.h"
		#include "Common/Audio.h"

		#ifdef min
			#undef min
		#endif

		#ifdef max
			#undef max
		#endif

		#ifndef Assert
			#if defined( ARK2D_DEBUG )
				#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
			#else
				#define Assert(b)
			#endif //DEBUG || _DEBUG
		#endif





 		typedef unsigned char GLubyte;
		typedef unsigned int GLsizei;
		typedef int socklen_t;

		#include <memory>
		#include <ppl.h>
		#include <ppltasks.h>

		#ifdef __cplusplus_winrt
			#include <agile.h>
			#include <collection.h>
		#endif

 	#elif defined(ARK2D_WINDOWS_PHONE_8)

 		#define STL_AVAILABLE
		#define EXCEPTIONS_AVAILABLE
		#define ARK2D_WINDOWS_VS



		//#define ARK2D_MISSING_C99_FUNCTIONS true

		#include <windows.h>
		#include <wrl.h>

		#include <vector>
		#include <list>
		#include <map>
		#include <set>

		#include <stdlib.h>
		#include <string>
		#include <time.h>


		#include <cstdlib>
		#include <fstream>

		#include <math.h>
		#include <cmath>
		#include <iostream>

		#ifdef min
			#undef min
		#endif

		#ifdef max
			#undef max
		#endif

		#define snprintf _snprintf
		#define sprintf sprintf_s


		typedef unsigned int ALuint;
		typedef unsigned int ALenum;
		typedef float ALfloat;



		#include <wrl/client.h>

		using namespace std;

		//#include <d2d1.h>
		//#include <d2d1effects_1.h>
		//#include <dwrite.h>

		#include "Common/OpenGL.h"


		#include <memory>
		#include <ppl.h>
		#include <ppltasks.h>



 		// audio
		#include "Common/Audio.h"

		// netcode
		//#include "Net/Includes.h"

		#ifndef Assert
			#if defined( ARK2D_DEBUG )
				#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
			#else
				#define Assert(b)
			#endif //DEBUG || _DEBUG
		#endif


		// vendor
		#include "vendor/wp8/gl2dx/gl2dx.h"
		//#include "vendor/libcurl-windows/curl.h"

		// PNG LIBs
		// https://github.com/cocos2d-x/cocos2dx-win8/tree/wp8/cocos2dx/platform/third_party/win8_metro/libraries

		//using namespace Windows::ApplicationModel;
		//using namespace Windows::ApplicationModel::Core;
		//using namespace Windows::ApplicationModel::Activation;
		//using namespace Windows::UI::Core;
		//using namespace Windows::System;
		//using namespace Windows::Foundation;
		//using namespace Windows::Graphics::Display;
		//using namespace concurrency;

		//#define strncpy strncpy_s
		#define fopen fopen_s
		// #define OF(args)  ()

		//#using <Windows.winmd>

		#ifdef __cplusplus_winrt
			#include <agile.h>
			#include <collection.h>
		#endif
		//#using <Platform.winmd>


		using namespace std;

		#include "vendor/spine/includes/spine/spine.h"


	#elif defined(_WIN32)

		#ifndef ARK2D_WINDOWS
			#define ARK2D_WINDOWS
		#endif
		#define STL_AVAILABLE
		#define EXCEPTIONS_AVAILABLE

		//#define _WINSOCKAPI_
		#include <winsock2.h>
		#include <windows.h>
		#include <mmsystem.h>
		#include <winbase.h>
 		//#include <wininet.h>
 		#include <Winhttp.h>

 		//#include <winsock.h>
		//#include <winsock2.h>

		#include <vector>
		#include <list>
		#include <map>
		#include <set>

		#include <stdlib.h>
		#include <string>
		#include <time.h>

		// multimedia system (midi)
		#include <mmsystem.h>



		#ifdef __GNUC__
			#include <sys/time.h>
 		#elif (defined(_MSC_FULL_VER) || defined(_MSC_VER))

			#ifndef ARK2D_WINDOWS_VS
				#define ARK2D_WINDOWS_VS
			#endif
 			#include <time.h>


 			#ifdef min
 				#undef min
 			#endif

			#ifdef max
 				#undef max
 			#endif

 			#define snprintf _snprintf


 			#if defined(ARK2D_WINDOWS_VS)
				#ifndef Assert
					#if defined( ARK2D_DEBUG )
						#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
					#else
						#define Assert(b)
					#endif //DEBUG || _DEBUG
				#endif
			#endif

	 		//#define JSON_DLL

		#endif

 		#include <cstdlib>
		#include <cstring>
		#include <fstream>

		#include <math.h>
		#include <cmath>
		#include <iostream>




		#pragma comment(lib, "advapi32.lib")
		#pragma comment(lib, "user32.lib")
		#pragma comment(lib, "gdi32.lib")
		#pragma comment(lib, "shell32.lib")
		#pragma comment(lib, "kernel32.lib")
		#pragma comment(lib, "Winhttp.lib")

		//#pragma comment(lib, "msvcr120.lib")

		#include "Common/OpenGL.h"
		#include "Common/Audio.h"

		#ifdef ARK2D_WINDOWS
 			#include "vendor/libcurl-windows/curl.h"
		#endif

 		using namespace std;






	#elif (defined(ARK2D_MACINTOSH)) // defined(__MACH__) || defined(__APPLE__)

 		#define STL_AVAILABLE
		#define EXCEPTIONS_AVAILABLE

		//#include "Common/OpenGL.h"

		//#include <Cocoa/Cocoa.h>
		//#include <CoreFoundation/CFBundle.h>
		//#include <ApplicationServices/ApplicationServices.h>

		#include <stdlib.h>
		#include <string>
		#include <stdio.h>
		#include <cstdio>
		#include <vector>
		#include <unistd.h>
		#include <list>
		#include <map>
 		#include <set>
		#include <fstream>
		#include <iostream>
		#include <cstdio>
		#include <sstream>
		#include <cstring>
		#include <math.h>
		#include <time.h>
		#include <ctime>
		#include <sys/time.h>

		#include <stdarg.h>

		#include <execinfo.h>

 		#include "vendor/spine/includes/spine/spine.h"

		//#include "vendor/spine/SpineSkeleton.h"

 		using namespace std;

 	#elif defined(__linux__)
		//#define ARK2D_UBUNTU_LINUX
		#define STL_AVAILABLE
		#define EXCEPTIONS_AVAILABLE

		#include "Common/Audio.h"
		#include "Common/OpenGL.h"

		#if defined(ARK2D_SDL2)
 			#include <SDL2/SDL.h>
 			#include <SDL2/SDL_opengl.h>
		#endif

		#include <vector>
		#include <list>
		#include <map>
		#include <set>
		#include <limits>

		#include <stdlib.h>
		#include <stdint.h>
		#include <inttypes.h>
		#include <string>
		#include <iostream>
		#include <fstream>
		#include <sys/time.h>
		#include <sys/ioctl.h>
		#include <sys/stat.h>
		#include <pthread.h>
		#include <unistd.h>

		#include <dirent.h>
		#include <errno.h>
		#include <fcntl.h>


 		using namespace std;

	#endif

	#include <stdint.h>



 	// TODO: Only include these where they are necessary.
	//#include "Common/Libraries/libpng.h"
	// #include "Common/Libraries/libjson.h"
	// #include "Common/Libraries/rapidxml.h"


#endif /* INCLUDES_H_ */
