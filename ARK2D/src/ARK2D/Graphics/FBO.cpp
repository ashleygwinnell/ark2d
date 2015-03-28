
#include "../Includes.h"
#include "../Namespaces.h"
#include "FBO.h"
#include "../ARK2D.h"
#include "../Core/GameContainer.h"
#include "Renderer.h" 
#include "TextureStore.h"
 
namespace ARK { 
	namespace Graphics {
		unsigned int FBO::_internalIdCount = 1; 
		bool FBO::m_supported = false;  
		bool FBO::m_supportedNPO2 = false;
		bool FBO::m_supportedVAOs = false;
		bool FBO::m_supportedChecked = false;
 
		FBO::FBO():
			m_dummy(false),
			m_clearScreenOnBind(true)
		{
			_internalId = _internalIdCount;
			_internalIdCount++;

			window_width = ARK2D::getContainer()->getWidth();
			window_height = ARK2D::getContainer()->getHeight();
			init();
 
			addFBOToStore();
		}
		FBO::FBO(bool dummy):
			m_dummy(dummy),
			m_clearScreenOnBind(true)
		{
			_internalId = _internalIdCount;
			_internalIdCount++;

			window_width = ARK2D::getContainer()->getWidth();
			window_height = ARK2D::getContainer()->getHeight();
			original_width = window_width;
			original_height = window_height;
		}

		FBO::FBO(unsigned int w, unsigned int h):
			m_dummy(false),
			m_clearScreenOnBind(true)
		{
			_internalId = _internalIdCount;
			_internalIdCount++;

			window_width = w;
			window_height = h;
			init();

			addFBOToStore();
		}

		void FBO::addFBOToStore() {

			// add to the FBO store. for android reloading purposes, yo. 
			FBOStore* store = FBOStore::getInstance();
			if (!store->hasFBO(_internalId)) { 
				ARK2D::getLog()->v("New FBO Object");
				store->addFBO(_internalId, this);
			} else {
				ARK2D::getLog()->v("Recyling FBO Object");
				ARK2D::getLog()->v("...somehow");
				//this = store->getFBO(_internalId);
			}
		}

		unsigned int FBO::getId() {
			return fbo;
		}

		void FBO::init() 
		{
			if (!isSupported()) { return; }

			#ifdef ARK2D_RENDERER_OPENGL

				#ifdef FBO_SUPPORT

					// enable things
					ARK2D::getLog()->v("FBO: Enable State");
					#ifndef NO_FIXED_FUNCTION_PIPELINE
						glEnable(GL_TEXTURE_2D); // Enable texturing so we can bind our frame buffer texture  
						RendererStats::s_glCalls++;
					#endif
					//glEnable(GL_DEPTH_TEST); // Enable depth testing 
					//Image::showAnyGlErrorAndExit(); 
					showAnyGlErrorAndExitMacro();

					// FBO 
					ARK2D::getLog()->v("FBO: Generate FBO");
					glGenFramebuffersARK(1, &fbo); // Generate one frame buffer and store the ID in fbo  
					glBindFramebufferARK(GL_FRAMEBUFFER_ARK, fbo); // Bind our frame buffer 
					//Image::showAnyGlErrorAndExit(); 
					showAnyGlErrorAndExitMacro();
					RendererStats::s_glCalls += 2;

					// depth buffer
					/*ARK2D::getLog()->i("FBO: Generate Depth Buffer");
					glGenRenderbuffersEXT(1, &fbo_depth);
					glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_depth);

					glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, window_width, window_height);
					glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth); // Set the render buffer of this buffer to the depth buffer  

					glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

					Image::showAnyGlErrorAndExit(); */

					original_width = window_width;
					original_height = window_height;

					if (!m_supportedNPO2) {
						window_width = MathUtil::nextPowerOfTwo(window_width);
						window_height = MathUtil::nextPowerOfTwo(window_height); 
					}
					ARK2D::getLog()->v(StringUtil::append("FBO: Width is ", window_width));
					ARK2D::getLog()->v(StringUtil::append("FBO: Height is ", window_height));
					//ARK2D::getLog()->v(StringUtil::append("FBO: Max-texture-size is: ", ARK2D::getRenderer()->getMaxTextureSize()));


					// texture buffer
					ARK2D::getLog()->v("FBO: Generate Texture Buffer");
					glGenTextures(1, &fbo_texture); 
					glBindTexture(GL_TEXTURE_2D, fbo_texture); // Bind the texture fbo_texture  
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Create a standard texture with the width and height of our window  
					RendererStats::s_glCalls += 3;

					RendererStats::s_textureAllocatedBytes += (window_width * window_height * 8 * 4);

					//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
					//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Renderer::getInterpolationGL());
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Renderer::getInterpolationGL());

//					glHint(GL, GL_NICEST);
					//glGenerateMipmap(GL_TEXTURE_2D);
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					
					//#ifndef NO_FIXED_FUNCTION_PIPELINE
					glBindTexture(GL_TEXTURE_2D, 0);  
					//#endif
					RendererStats::s_glCalls += 3;


					// add to the texture store. for android reloading purposes, yo. 
					/*ARK2D::getLog()->i("add fbo texture to texture store?");
					TextureStore* ts = TextureStore::getInstance();
					if (!ts->hasTexture(filename)) { 
						ARK2D::getLog()->i("New Texture Object");
						Texture* t = new Texture(texture_temp, this);
						ts->addTexture(filename, t);
						m_texture = t;
					} else {
						ARK2D::getLog()->i("Recyling Texture Object");
						m_texture = ts->getTexture(filename);
					}*/

					//Image::showAnyGlErrorAndExit();  
					showAnyGlErrorAndExitMacro();
		 
					// Bind depth/texture buffers to FBO.
					ARK2D::getLog()->v("FBO: Bind depth/texture buffers to FBO");
					glFramebufferTexture2DARK(GL_FRAMEBUFFER_ARK, GL_COLOR_ATTACHMENT0_ARK, GL_TEXTURE_2D, fbo_texture, 0); // Attach the texture fbo_texture to the color buffer in our frame buffer  
					//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth); // Attach the depth buffer fbo_depth to our frame buffer  
					//Image::showAnyGlErrorAndExit(); 
					showAnyGlErrorAndExitMacro();
					RendererStats::s_glCalls++;

					// do add stencil/depth
					#if defined(ARK2D_OPENGL_3_2)
						ARK2D::getLog()->v("FBO: Adding stencil buffer to FBO");
						GLuint rboDepthStencil;
	    				glGenRenderbuffers(1, &rboDepthStencil);
	    				ARK2D::getLog()->v("FBO: 1");
	    				glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	    				ARK2D::getLog()->v("FBO: 2");
	    				glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, window_width, window_height);
	    				ARK2D::getLog()->v("FBO: 3");
	    				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);
	    			#endif

					// FBO error check
					ARK2D::getLog()->v("FBO: Error Checking");
					GLenum status = glCheckFramebufferStatusARK(GL_FRAMEBUFFER_ARK); // Check that status of our generated frame buffer  
					ARK2D::getLog()->v("FBO: Error Checking 2");
					if (status != GL_FRAMEBUFFER_COMPLETE_ARK) // If the frame buffer does not report back as complete  
					{  
						showAnyGlErrorAndExitMacro();
						ErrorDialog::createAndShow("Couldn't create frame buffer");
						exit(0);
						//std::cout << "Couldn't create frame buffer" << std::endl; // Make sure you include <iostream>  
						//exit(0); // Exit the application  
					}  

					ARK2D::getLog()->v("FBO: Bind framebuffer");
					glBindFramebufferARK(GL_FRAMEBUFFER_ARK, 0); // Unbind our frame buffer  
					RendererStats::s_glCalls += 2;

					//Image::showAnyGlErrorAndExit(); 
					showAnyGlErrorAndExitMacro();
		 
					//RendererState::start(RendererState::GEOMETRY);
				#endif

			#endif
		}
		
		void FBO::deinit() {
			// delete framebuffer
			// delete textures
		}

		void FBO::resize(int width, int height) {
			if (m_dummy) { return; }
			deinit();

			window_width = width;
			window_height = height;
			init();
		}

		void FBO::bind() { 
			bind(window_width, window_height);
		}

		void FBO::bind(unsigned int vw, unsigned int vh) { 
			if (!isSupported()) { return; }
			 
			#ifdef ARK2D_RENDERER_OPENGL
				#ifdef FBO_SUPPORT

					Renderer* r = ARK2D::getRenderer();

					#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE) || defined(NO_FIXED_FUNCTION_PIPELINE))
						ARK2D::getGame()->postRender(ARK2D::getContainer(), ARK2D::getRenderer());
						glBindFramebufferARK(GL_FRAMEBUFFER_ARK, fbo); // Bind our frame buffer for rendering  
						RendererStats::s_glCalls++;
					#else 
						glBindFramebufferARK(GL_FRAMEBUFFER_ARK, fbo); // Bind our frame buffer for rendering  
						glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT); // Push our glEnable and glViewport states  
						RendererStats::s_glCalls += 2;
					#endif

					


					//int ww = ARK2D::getContainer()->getDynamicWidth();
					//int wh = ARK2D::getContainer()->getDynamicHeight();

					int ww = vw;//window_width
					int wh = vh;//window_height;
	 
					glViewport(0, 0, ww, wh);
					RendererStats::s_glCalls++;   
					//glViewport(0, 0, ww, wh);
					 
					//Color c = ARK2D::getRenderer()->getClearColor();
					Color c = ARK2D::getContainer()->getClearColor();

					glClearColor (c.getRedf(), c.getGreenf(), c.getBluef(), c.getAlphaf()); // Set the clear colour  
					if (m_clearScreenOnBind) { 
						glClear (GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT); // Clear the depth and colour buffers  
					}
					RendererStats::s_glCalls += 2;
		  
					r->pushMatrix();
					//ARK2D::getContainer()->enable2D();
					r->loadIdentity();  // Reset the modelview matrix 
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
			
			#endif
		}

		void FBO::bind_2d() {
			bind_2d(window_width, window_height);
		}

		void FBO::bind_2d(unsigned int w, unsigned int h) {
			if (!isSupported()) { return; }

			#ifdef ARK2D_RENDERER_OPENGL
				#ifdef FBO_SUPPORT

					Renderer* r = ARK2D::getRenderer();

					r->matrixMode(MatrixStack::TYPE_PROJECTION);
					r->pushMatrix();  
					r->loadIdentity();  
	 
					r->ortho2d(0, 0, w, h, -1.0f, 1.0f);

					r->matrixMode(MatrixStack::TYPE_MODELVIEW);
					r->pushMatrix();
					r->loadIdentity(); 

				#endif
			#endif
		}

		void FBO::unbind_2d() {
			if (!isSupported()) { return; }

			#ifdef ARK2D_RENDERER_OPENGL 
				#ifdef FBO_SUPPORT
					ARK2D::getContainer()->disable2D();
				#endif
			#endif
		}

		void FBO::unbind() {
			if (!isSupported()) { return; }

			#ifdef ARK2D_RENDERER_OPENGL

				#ifdef FBO_SUPPORT
		 
					#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE) || defined(NO_FIXED_FUNCTION_PIPELINE))
						Renderer* r = ARK2D::getRenderer();
						r->popMatrix();

						glViewport(0, 0, ARK2D::getContainer()->getDynamicWidth(),  ARK2D::getContainer()->getDynamicHeight());
						
						#ifdef ARK2D_IPHONE
							glBindFramebufferARK(GL_FRAMEBUFFER_ARK, ARK2D::getContainer()->m_platformSpecific._viewFrameBuffer); // Unbind our texture  
						#else
							glBindFramebufferARK(GL_FRAMEBUFFER_ARK, 0); // Unbind our texture  
						#endif

						ARK2D::getGame()->preRender(ARK2D::getContainer(), ARK2D::getRenderer());
					#else 
						glPopAttrib(); // Restore our glEnable and glViewport states  
						glBindFramebufferARK(GL_FRAMEBUFFER_ARK, 0); // Unbind our texture  
					#endif
					RendererStats::s_glCalls += 2;

					

					//ARK2D::getContainer()->enable2D();

				#endif
			
			#endif
		}



		unsigned int FBO::getTextureId() {
			return fbo_texture;
		}

		FBO::~FBO() {

		}


		bool FBO::isSupported() {
			if (m_supportedChecked) { return m_supported; }

			#ifdef ARK2D_RENDERER_OPENGL

				#ifndef FBO_SUPPORT
					// TODO: 
					// const unsigned char* extensions = glGetString(GL_EXTENSIONS);
					m_supportedChecked = true;
					m_supported = false;
					return false;
				#else
 
					showAnyGlErrorAndExitMacro();

					const GLubyte * strVersion;
					const GLubyte * strGLSLVersion;
					const GLubyte * strExt;
					float myGLVersion; 
	 
					strVersion = glGetString(GL_VERSION);
					sscanf((char *)strVersion, "%f", &myGLVersion); 
					RendererStats::s_glCalls++;

					#ifndef ARK2D_OPENGL_3_2
						strExt = glGetString (GL_EXTENSIONS); 
						RendererStats::s_glCalls++;
					#else 				
						int extCount;
						glGetIntegerv(GL_NUM_EXTENSIONS, &extCount);
						string extStr;
						for(unsigned int ii = 0; ii < extCount; ii++) {
							extStr += (const char*) glGetStringi (GL_EXTENSIONS, ii);
							extStr += ", ";
						}
						strExt = (const unsigned char*) extStr.c_str();

						RendererStats::s_glCalls += 1 + extCount;
					#endif
	  
					#if defined(ARK2D_MACINTOSH) 
						// FBO part of the 3.2 core.
						bool supported = true; //gluCheckExtensionARK ((const GLubyte*)"GL_EXT_framebuffer_object",strExt); 
						m_supportedVAOs = true;
					#elif defined(ARK2D_WINDOWS) 
						bool supported = true;
						m_supportedVAOs = ARK2D::getRenderer()->gluCheckExtensionARK ((const GLubyte*)"GL_ARB_vertex_array_object",strExt); 
	 				#elif ( defined(ARK2D_ANDROID) )
						bool supported = true;
						m_supportedVAOs = ARK2D::getRenderer()->gluCheckExtensionARK ((const GLubyte*)"GL_OES_vertex_array_object",strExt); 
					#elif defined(ARK2D_IPHONE) || defined(ARK2D_EMSCRIPTEN_JS)
						bool supported = true; 
						m_supportedVAOs = false; //ARK2D::getRenderer()->gluCheckExtensionARK ((const GLubyte*)"GL_OES_vertex_array_object",strExt); //false; // GL_OES_vertex_array_object
					#elif ( defined(ARK2D_UBUNTU_LINUX) )
						bool supported = true;  
						m_supportedVAOs = false;
					#else 
						m_supportedVAOs = false;
						bool supported = ARK2D::getRenderer()->gluCheckExtensionARK ((const GLubyte*)"GL_EXT_framebuffer_object",strExt); 
					#endif

					strGLSLVersion = glGetString(GL_SHADING_LANGUAGE_VERSION); // ver = "1.50 NVIDIA via Cg compiler"
					RendererStats::s_glCalls++; 

					m_supportedNPO2 = ARK2D::getRenderer()->gluCheckExtensionARK ((const GLubyte*)"GL_ARB_texture_non_power_of_two",strExt); 
					
					m_supportedChecked = true;
					m_supported = (supported); // && supportedNPO2);

					ARK2D::getLog()->i(StringUtil::append("OpenGL Version: ", (char*) strVersion));
					ARK2D::getLog()->i(StringUtil::append("OpenGL GLSL Version: ", (char*) strGLSLVersion));
					

					

					//#if !defined(ARK2D_MACINTOSH)
					
					//	string _extensions = string("OpenGL Extensions: ");
					//	_extensions += string((char*)strExt);
					//	_extensions += "\0";
					//	ARK2D::getLog()->i(_extensions);
					//#endif 

				
	 				//Image::showAnyGlErrorAndExit();
					showAnyGlErrorAndExitMacro();
					if (!m_supported) {
						ARK2D::getLog()->w("FBO not supported!");
					}
					//Image::showAnyGlErrorAndExit();
					showAnyGlErrorAndExitMacro();

				#endif

			#endif

			return m_supported;
		}



	}
}


