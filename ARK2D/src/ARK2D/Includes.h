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
 		#include <Flash++.h>

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

 		//#include <inttypes.h>

 		#include <GL/gl.h>
 		#include <GL/glext.h>
 		#include <GL/glu.h>
 		//#include <GL/glxext.h>

 		/*#include <GLES/gl.h>
		#include <GLES/glext.h>
		#include <GLES/glplatform.h>

 		#include <GLES2/gl2.h>
 		#include <GLES2/gl2ext.h>*/

 		using namespace std;

 		typedef unsigned int ALenum;
 		typedef unsigned int ALuint;
 		typedef float ALfloat;

 		#include "vendor/libcurl/curl.h"


	#elif defined(ARK2D_ANDROID)
 		#define STL_AVAILABLE

		#include <string>
		#include <vector>
		#include <list>
		#include <map>
 		#include <set>
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
		#include <algorithm>

		#include <GLES/gl.h>
		#include <GLES/glext.h>
		#include <GLES/glplatform.h>

 		#include <GLES2/gl2.h>
 		#include <GLES2/gl2ext.h>

		#include <AL/AL.h>
		#include <AL/ALc.h>
		#include <AL/ALext.h>

 		

	 	using namespace std;

	#elif defined(ARK2D_IPHONE)

 		#define STL_AVAILABLE
		#define EXCEPTIONS_AVAILABLE

 		#include <stdio.h>
		#include <stdlib.h>
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

 		#include <OpenAL/al.h>
		#include <OpenAL/alc.h>

		#include <OpenGLES/EAGL.h>
		#include <QuartzCore/QuartzCore.h>

		#include <OpenGLES/ES1/gl.h>
		#include <OpenGLES/ES1/glext.h>
		#include <OpenGLES/ES2/gl.h>
		#include <OpenGLES/ES2/glext.h>
		#include <OpenGLES/EAGLDrawable.h>

        //typedef unsigned int ALuint;
        //typedef unsigned int ALenum;
        //typedef float ALfloat;



	#elif defined(_WIN32)

		#ifndef ARK2D_WINDOWS
			#define ARK2D_WINDOWS
		#endif
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
		#include <set>

		#include <stdlib.h>
		#include <string>
		#include <time.h>
		#include <sys/time.h>

 		#include <cstdlib>
		#include <cstring>
		#include <fstream>
		#include <iostream>

		#include <GL/GLew.h>
		#include <GL/GL.h>

		#include <AL/AL.h>
		#include <AL/ALc.h>
		#include <AL/ALext.h>
		#include <AL/ALut.h>

 		#include "vendor/libcurl-windows/curl.h"

 		using namespace std;




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

	#elif (defined(ARK2D_MACINTOSH)) // defined(__MACH__) || defined(__APPLE__)

		//#define ARK2D_MACINTOSH
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

	#ifndef FBO_SUPPORT
		#if (defined(ARK2D_WINDOWS)  || defined(ARK2D_ANDROID) || defined(ARK2D_FLASCC))
			//#define FBO_SUPPORT false
		#else
			#define FBO_SUPPORT true
		#endif
	#endif

	#ifndef SHADER_SUPPORT
		#if (defined(ARK2D_WINDOWS)  || defined(ARK2D_ANDROID) || defined(ARK2D_FLASCC))
			//#define SHADER_SUPPORT false
		#else
			#define SHADER_SUPPORT true
		#endif
	#endif

    

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

	//#ifndef STDSTRDEF
	//	typedef std::string stdstring; 
	//	#define STDSTRDEF
	//#endif

	/*


	using std::vector;
	using std::map;
	using std::set;

	*/


#endif /* INCLUDES_H_ */
