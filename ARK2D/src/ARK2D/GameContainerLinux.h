/*
 * GameContainerLinux.h
 *
 *  Created on: 27 Feb 2011
 *      Author: ashley
 */



#ifndef GAMECONTAINERLINUX_H_
#define GAMECONTAINERLINUX_H_

#include "ARK2D_GL.h"

#ifdef ARK2D_UBUNTU_LINUX

	#include <X11/X.h>
	#include <X11/Xlib.h>

	// BIG HELP HERE, OKAY.
	// http://www.opengl.org/wiki/Programming_OpenGL_in_Linux:_GLX_and_Xlib

	class GameContainerPlatform {
	public:
		Display*				dpy;
		Window                  root;
		static GLint 			att[5];
		XVisualInfo*			vi;
		Colormap                cmap;
		XSetWindowAttributes    swa;
		Window                  win;
		GLXContext              glc;
		XWindowAttributes       gwa;
		XEvent                  xev;

		void enableOpenGL();//HWND hWnd, HDC* hDC, HGLRC* hRC);
		void disableOpenGL();//HWND hWnd, HDC hDC, HGLRC hRC);

		bool initOpenAL();
		bool deinitOpenAL();

		void setTitle(std::string title);

	};

#endif

#endif /* GAMECONTAINERLINUX_H_ */
