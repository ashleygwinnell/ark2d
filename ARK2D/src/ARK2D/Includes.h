/*
 * Includes.h
 *
 *  Created on: Apr 11, 2012
 *      Author: ashleygwinnell
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

	#if defined(ARK2D_ANDROID)
		#include <string>
		#include <vector>
		#include <list>
		#include <map>
		#include <sstream>
		#include <iostream>
		#include <fstream>
		#include <jni.h>
		#include <sys/time.h>
		#include <time.h>
		#include <android/log.h>
		#include <stdint.h>
		#include <stdio.h>
		#include <stdlib.h>
		#include <zip.h>
		#include <sys/stat.h>
		#include <sys/types.h>
		#include <math.h> 
		#define STL_AVAILABLE

		#include <GLES/gl.h>
		#include <GLES/glext.h>
		#include <GLES/glplatform.h>

		#include <AL/AL.h>
		#include <AL/ALc.h>
		#include <AL/ALext.h>
	#elif defined(ARK2D_IPHONE)

 		#define STL_AVAILABLE
		#define EXCEPTIONS_AVAILABLE

 		#include <OpenAL/al.h>
		#include <OpenAL/alc.h>

		#include <OpenGLES/EAGL.h>
		#include <QuartzCore/QuartzCore.h>

		#include <OpenGLES/ES1/gl.h>
		#include <OpenGLES/ES1/glext.h>
		#include <OpenGLES/ES2/gl.h>
		#include <OpenGLES/ES2/glext.h>

 		#include <string>
 		#include <vector>
		#include <list>
		#include <map>

 		#include <stdlib.h>
		#include <sys/types.h>
		#include <math.h>

	#elif defined(_WIN32)

		#define ARK2D_WINDOWS
		#define STL_AVAILABLE
		#define EXCEPTIONS_AVAILABLE
		#include <windows.h>
		#include <mmsystem.h>
		#include <winbase.h>
 		#include <wininet.h>

 		//#include <winsock.h>
		//#include <winsock2.h>
		
		#include <vector>
		#include <list>
		#include <map>

		#include <stdlib.h>
		#include <string>
		#include <time.h>
		#include <sys/time.h>

		#include <GL/GLew.h>
		#include <GL/GL.h>

		#include <AL/AL.h>
		#include <AL/ALc.h>
		#include <AL/ALext.h>
		#include <AL/ALut.h>

 		#include "vendor/libcurl-windows/curl.h"

	#elif defined(__linux__)
		#define ARK2D_UBUNTU_LINUX
		#define STL_AVAILABLE
		#define EXCEPTIONS_AVAILABLE
		#include <X11/X.h>
		#include <X11/Xlib.h>

		#include <GL/glew.h>
		#include <GL/gl.h>
		#include <GL/glx.h>
		#include <GL/glu.h>

		#include <AL/al.h>
		#include <AL/alc.h>
		#include <AL/alext.h>
		#include <AL/alut.h>

 		#include "vendor/libcurl/curl.h"

	#elif (defined(__MACH__) || defined(__APPLE__))

		#define ARK2D_MACINTOSH
		#define STL_AVAILABLE
		#define EXCEPTIONS_AVAILABLE
		#include <Cocoa/Cocoa.h>
		#include <CoreFoundation/CFBundle.h>
		#include <ApplicationServices/ApplicationServices.h>

		#include <stdlib.h>
		#include <string>
		#include <stdio.h>
		#include <vector>
		#include <list>
		#include <map>
		#include <fstream>
		#include <iostream>
		#include <cstdio>
		#include <sstream>
		#include <cstring>
		#include <math.h>
		#include <time.h>
		#include <ctime>
		#include <sys/time.h>

		#include <OpenGL/OpenGL.h>
		#include <OpenGL/gl.h>
		#include <OpenGL/gltypes.h>
		#include <OpenGL/glu.h>
		#include <OpenGL/glext.h>

		#include <OpenAL/al.h>
		#include <OpenAL/alc.h>

 		#include "vendor/libcurl/curl.h"

	#endif

	#include <stdint.h>

	typedef unsigned int GLuint;
	typedef unsigned int GLenum;
	typedef void GLvoid;

	#include "vendor/ogg130/ogg.h"
	#include "vendor/vorbis132/vorbisfile.h"

	#include "vendor/lpng151/png.h"

	

	#include "vendor/libJSON/JSON_Defs.h"
	#include "vendor/libJSON/JSONNode.h"
	#include "vendor/libJSON/JSONChildren.h"
	#include "vendor/libJSON/JSON_Worker.h"
	#include "vendor/libJSON/JSON_Strings.h"
	#include "vendor/libJSON/JSON_StringConv.h"
	#include "vendor/libJSON/jsonmain.h"
	#include "vendor/libJSON/Debug.h"

#endif /* INCLUDES_H_ */
