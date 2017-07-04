/*
 * GameContainerHTML5.h
 *
 *  Created on: Sep 18, 2014
 *      Author: ashleygwinnell
 */

#ifndef GAMECONTAINER_HTML5_H_
#define GAMECONTAINER_HTML5_H_

#include "../../Includes.h"
//#include "../../Namespaces.h"
//#include "../../ARK2D.h"

	#ifdef ARK2D_EMSCRIPTEN_JS

	#define ESUTIL_API
	#define ESCALLBACK

	#ifndef FALSE
	#define FALSE 0
	#endif
	#ifndef TRUE
	#define TRUE 1
	#endif

	typedef struct
	{
	   // Handle to a program object
	   GLuint programObject;
	} UserData;

	typedef struct
	{
	    GLfloat   m[4][4];
	} ESMatrix;

	typedef struct _escontext
	{
	   /// Put your user data here...
	   void*       userData;

	   /// Window width
	   GLint       width;

	   /// Window height
	   GLint       height;

	   /// Window handle
	   EGLNativeWindowType  hWnd;

	   /// EGL display
	   EGLDisplay  eglDisplay;
	      
	   /// EGL context
	   EGLContext  eglContext;

	   /// EGL surface
	   EGLSurface  eglSurface;

	   /// Callbacks
	   void (ESCALLBACK *drawFunc) ( struct _escontext * );
	   void (ESCALLBACK *keyFunc) ( struct _escontext *, unsigned char, int, int );
	   void (ESCALLBACK *updateFunc) ( struct _escontext *, float deltaTime );
	} ESContext;

		#include <emscripten.h>
		extern "C" void EMSCRIPTEN_KEEPALIVE emscripten_containerSetSize(int width, int height, int doCallback);

		namespace ARK {
			namespace Core {

				class GameContainerPlatform {
					friend class GameContainer;
					public:
						GameContainer* m_container;
						static void* getARK2DResource(int resourceId, int resourceType);

					public:


						ESContext* esContext;
						bool WinCreate(ESContext* esContext, const char *title);
						EGLBoolean CreateEGLContext ( EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
                              EGLContext* eglContext, EGLSurface* eglSurface,
                              EGLint attribList[]);
						void setFullscreenInternal(bool b);
						//GLboolean userInterrupt(ESContext* esc);
				
						static void staticTick();
						void doTick();
						void doEvents();
						
						bool initOpenAL();
						bool deinitOpenAL();

						void setTitle(std::string title);
						string getResourcePath() const;

						inline bool isRetina() { return false; }
				};

			}
		}

	#endif

#endif /* GAMECONTAINER_HTML5_H_ */
