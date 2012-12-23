	
#include "../Includes.h"
#include "../Namespaces.h"
#include "FBO.h"
#include "../ARK2D.h"
#include "../Core/GameContainer.h"
 
namespace ARK { 
	namespace Graphics {

		bool FBO::m_supported = false;
		bool FBO::m_supportedChecked = false;

		FBO::FBO() 
		{
			window_width = ARK2D::getContainer()->getWidth();
			window_height = ARK2D::getContainer()->getHeight();
			init();
		}

		FBO::FBO(unsigned int w, unsigned int h) 
		{
			window_width = w;
			window_height = h;
			init();
		}

		void FBO::init() 
		{
			if (!isSupported()) { return; }

			#ifdef FBO_SUPPORT

				// enable things
				ARK2D::getLog()->i("FBO: Enable State");
				glEnable(GL_TEXTURE_2D); // Enable texturing so we can bind our frame buffer texture  
				//glEnable(GL_DEPTH_TEST); // Enable depth testing 
				Image::showAnyGlErrorAndExit(); 

				// FBO 
				ARK2D::getLog()->i("FBO: Generate FBO");
				glGenFramebuffersEXT(1, &fbo); // Generate one frame buffer and store the ID in fbo  
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // Bind our frame buffer 
				Image::showAnyGlErrorAndExit(); 

				// depth buffer
				/*ARK2D::getLog()->i("FBO: Generate Depth Buffer");
				glGenRenderbuffersEXT(1, &fbo_depth);
				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_depth);

				glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, window_width, window_height);
				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth); // Set the render buffer of this buffer to the depth buffer  

				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

				Image::showAnyGlErrorAndExit(); */

				// texture buffer
				ARK2D::getLog()->i("FBO: Generate Texture Buffer");
				glGenTextures(1, &fbo_texture); 
				glBindTexture(GL_TEXTURE_2D, fbo_texture); // Bind the texture fbo_texture  
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Create a standard texture with the width and height of our window  

				//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
				//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				glBindTexture(GL_TEXTURE_2D, 0);  

				Image::showAnyGlErrorAndExit();  
	 
				// Bind depth/texture buffers to FBO.
				ARK2D::getLog()->i("FBO: Bind depth/texture buffers to FBO");
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, fbo_texture, 0); // Attach the texture fbo_texture to the color buffer in our frame buffer  
				//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth); // Attach the depth buffer fbo_depth to our frame buffer  
				Image::showAnyGlErrorAndExit(); 

				// FBO error check
				ARK2D::getLog()->i("FBO: Error Checking");
				GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); // Check that status of our generated frame buffer  
				if (status != GL_FRAMEBUFFER_COMPLETE_EXT) // If the frame buffer does not report back as complete  
				{  
					ErrorDialog::createAndShow("Couldn't create frame buffer");
					exit(0);
					//std::cout << "Couldn't create frame buffer" << std::endl; // Make sure you include <iostream>  
					//exit(0); // Exit the application  
				}  

				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our frame buffer  

				Image::showAnyGlErrorAndExit(); 
	 
				//RendererState::start(RendererState::GEOMETRY);
			#endif
		}
		


		void FBO::bind() { 
			if (!isSupported()) { return; }

			#ifdef FBO_SUPPORT

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // Bind our frame buffer for rendering  
			glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT); // Push our glEnable and glViewport states  


			//int ww = ARK2D::getContainer()->getDynamicWidth();
			//int wh = ARK2D::getContainer()->getDynamicHeight();
			glViewport(0, 0, window_width, window_height);
			//glViewport(0, 0, ww, wh);
			
			Color c = ARK2D::getContainer()->getClearColor();

			glClearColor (c.getRedf(), c.getGreenf(), c.getBluef(), c.getAlphaf()); // Set the clear colour  
			glClear (GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT); // Clear the depth and colour buffers  
  
			
			//ARK2D::getContainer()->enable2D();
			glLoadIdentity();  // Reset the modelview matrix 
			//glOrtho(0, window_width, 0, window_height, -1, 1);

			//ARK2D::getContainer()->enable2D();

			// enable 2d
			/*glMatrixMode(GL_PROJECTION) ; 
			glPushMatrix();
			glLoadIdentity();
			
			glOrtho(0, window_width, 0, window_height, -1, 1);
			
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();*/
			#endif
		}

		void FBO::unbind() {
			if (!isSupported()) { return; }

			#ifdef FBO_SUPPORT

				glPopAttrib(); // Restore our glEnable and glViewport states  
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our texture  

				//ARK2D::getContainer()->enable2D();

			#endif
		}



		unsigned int FBO::getTextureId() {
			return fbo_texture;
		}

		FBO::~FBO() {

		}


		bool FBO::isSupported() {
			if (m_supportedChecked) { return m_supported; }

			#ifndef FBO_SUPPORT
				// TODO: 
				// const unsigned char* extensions = glGetString(GL_EXTENSIONS);
				m_supportedChecked = true;
				m_supported = false;
				return false;
			#else

				const GLubyte * strVersion;
				const GLubyte * strExt;
				float myGLVersion; 

				strVersion = glGetString(GL_VERSION);
				sscanf((char *)strVersion, "%f", &myGLVersion);
				strExt = glGetString (GL_EXTENSIONS); 

				bool supported = gluCheckExtension ((const GLubyte*)"GL_EXT_framebuffer_object",strExt); 
				bool supportedNPO2 = gluCheckExtension ((const GLubyte*)"GL_ARB_texture_non_power_of_two",strExt); 
				
				m_supportedChecked = true;
				m_supported = (supported && supportedNPO2);

				ARK2D::getLog()->i(StringUtil::append("OpenGL Version: ", (char*) strVersion));
				ARK2D::getLog()->i(StringUtil::append("OpenGL Extensions: ", (char*) strExt));

				if (!m_supported) {
					ARK2D::getLog()->w("FBO not supported!");
				}

			#endif

			return m_supported;
		}



	}
}


