/*
 * ARK2D_GL.h
 *
 *  Created on: 27 Feb 2011
 *      Author: ashley
 */

#include "ARK2D.h"

#if defined(ARK2D_WINDOWS) || defined(_WIN32)
	#include <GL/GLew.h>
	#include <GL/GL.h>
#endif

#if defined(ARK2D_UBUNTU_LINUX) || defined(__linux__)
	#include <GL/glew.h>
	#include <GL/gl.h>

	//#include <GL/glxew.h>


	#include <GL/glx.h>

	//#include <GL/glext.h>
	//#include <GL/glxext.h>
	#include <GL/glu.h>
	//#include <GL/glut.h>

	//#include <GL/freeglut.h>
	//#include <GL/freeglut_std.h>
	//#include <GL/freeglut_ext.h>
#endif

#if defined(ARK2D_MACINTOSH)
	#include <stdlib.h>
	#include <OpenGL/OpenGL.h>
	#include <OpenGL/gl.h>
	//#include <OpenGL/gl3.h>
	//#include <OpenGL/gl3ext.h>
	//#include <OpenGL/gluMacro.h>
	#include <OpenGL/gltypes.h>
	#include <OpenGL/glu.h>
	//#include <OpenGL/CGLContext.h>
	// #include <GLUT/glut.h>
	#include <OpenGL/glext.h>

#endif

#ifndef ARK2D_GL_H_
#define ARK2D_GL_H_

typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef void GLvoid;

#endif /* ARK2D_GL_H_ */
