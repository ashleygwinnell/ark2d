/*
 * GameContainerMac.cpp
 *
 *  Created on: 16 May 2012
 *      Author: ashleygwinnell
 */

#include "../GameContainer.h"
#include "GameContainerIPhone.h"
#include "../../Util/Log.h" 

#ifdef ARK2D_IPHONE

#import "GameContainerIPhoneAppDelegate.h"
#import "../../Util/ICloudUtil.h"
#import "../../Math/Random.h"
#import "../../Graphics/Image.h"
#import "../../Audio/Sound.h"

namespace ARK {
	namespace Core {
 		bool GameContainerPlatform::s_gamePaused = false;
 		
 	} 
 }

	ARK::Core::GameContainer::GameContainer(Game& g, int width, int height, int bpp, bool fullscreen):
		m_timer(),
		m_game(g),
		m_input(),
		m_graphics(),
		m_gamepads(),
		scene(NULL),
		m_originalWidth(width),
		m_originalHeight(height),
		m_width(width),
		m_height(height),
		m_screenWidth(0),
		m_screenHeight(0),
		m_scaleLock(false),
		m_scale(1.0f),
		m_scaleX(1.0f),
		m_scaleY(1.0f),
		m_translateX(0),
		m_translateY(0),
		m_bpp(bpp),
		m_fullscreen(fullscreen),
		m_resizable(false),
		m_scaleToWindow(true),
		m_touchMode(true),
		m_screenOrientationPrevious(ORIENTATION_DUMMY),
		m_orientationInverted(false),
		m_2in1enabled(false),
		m_clearColor(Color::black),
		m_resizeBehaviour(RESIZE_BEHAVIOUR_SCALE),
		m_showingFPS(false),
		m_willLoadDefaultFont(true),
		m_platformSpecific()
		{
			
			m_fullscreen = true; 

			m_platformSpecific.m_container = this;
		
			m_input.setGameContainer(this);

			ARK2D::s_container = this;
			ARK2D::s_game = &m_game;
			ARK2D::s_graphics = &m_graphics;
			ARK2D::s_input = &m_input;
			ARK2D::s_log = ARK2D::getLog();
			scene = new Scene();
			scene->addChild(ARK2D::s_game);
			scene->addChild(ARK2D::s_log);
		
			 
			// Get location of current app bundle and make sure there's a resources path.
			m_platformSpecific.m_resourcePath = [[[NSBundle mainBundle] resourcePath] fileSystemRepresentation];
			m_platformSpecific.m_resourcePath += "/data/";
			std::cout << "Resource path: " << m_platformSpecific.m_resourcePath << std::endl;
			
			
			ARK2D::getRenderer()->preinit();
            

	} 

	void* ARK::Core::GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
		return NULL;
	}


	void ARK::Core::GameContainer::setSizeNoCallback(int width, int height) {
		
	
	    /*if (m_resizeBehaviour == RESIZE_BEHAVIOUR_SCALE) {
	   		ARK2D::s_game->resize(this, width, height);
	    } else if (m_resizeBehaviour == RESIZE_BEHAVIOUR_NOSCALE) {
	    	m_width = width;
	    	m_height = height;
	    	ARK2D::s_game->resize(this, width, height);
	    	//ARK2D::getRenderer()->setScissorTestEnabled(false);
	    	//ARK2D::getRenderer()->scissor(0,0,width,height);
	    }*/
  

    	

	   if (m_resizeBehaviour == RESIZE_BEHAVIOUR_SCALE) {

	   		/*if (getOrientation() == ORIENTATION_LANDSCAPE) {
				
	   			ARK2D::getLog()->v(StringUtil::appendf("PASSED IN WIDTH: ", width));
				ARK2D::getLog()->v(StringUtil::appendf("PASSED IN HEIGHT: ", height));
				m_width = height;
				m_height = width;

				m_screenWidth = height;
				m_screenHeight = width; 
 
				m_scaleX = (float) m_width / (float) m_originalWidth;//Width
				m_scaleY = (float) m_height / (float) m_originalHeight;//Height; 

				ARK2D::getLog()->v("SET SIZE NO CALLBACK!"); 
				ARK2D::getLog()->v(StringUtil::appendf("Width: ", m_width));
				ARK2D::getLog()->v(StringUtil::appendf("Height: ", m_height));
				ARK2D::getLog()->v(StringUtil::appendf("Scale X: ", m_scaleX));
				ARK2D::getLog()->v(StringUtil::appendf("Scale Y: ", m_scaleY));
   			} else {*/
   				m_scaleX = (float) width / (float) m_originalWidth;
				m_scaleY = (float) height / (float) m_originalHeight; 

				m_width = width;
				m_height = height;

				m_screenWidth = width;
				m_screenHeight = height;
   			//}

   			if (m_scaleX > m_scaleY) {
				m_scale = m_scaleY; 
				m_scaleX = m_scaleY;
				//m_scaleY = 1.0f;
			} else { // y > x
				m_scale = m_scaleX;
				m_scaleY = m_scaleX;
				//m_scaleX = 1.0f;
			} 
	   		


			//m_width = m_originalWidth * m_scaleX;
			//m_height = m_originalHeight * m_scaleY;
			

		} else if (m_resizeBehaviour == RESIZE_BEHAVIOUR_NOSCALE) {
			m_width = width;
			m_height = height; 

			m_screenWidth = width;
			m_screenHeight = height;
		}

       /* unsigned int orient = getOrientation();
        if (orient == GameContainer::ORIENTATION_LANDSCAPE) { // switch them around if it's landscape.
            
    		String rsss("resizing: ");
			rsss += height;
			rsss += ", ";
			rsss += width;
			ARK2D::getLog()->i(rsss.get());

            ARK2D::s_game->resize(this, m_width, m_height);
        } else {

        	String rsss("resizing: ");
			rsss += width;
			rsss += ", ";
			rsss += height; 
			ARK2D::getLog()->i(rsss.get());

            ARK2D::s_game->resize(this, width, height);
        } */
 
	}
	
	void ARK::Core::GameContainer::setSize(int width, int height, bool docallback) {
		setSizeNoCallback(width, height);
        ARK2D::s_game->resize(this, m_width, m_height);
	}

	void ARK::Core::GameContainer::setFullscreen(bool fullscreen) {
		m_fullscreen = fullscreen;
	}

	void ARK::Core::GameContainer::processGamepadInput() {

	}

	void ARK::Core::GameContainer::setIcon(const std::string& path) {
		
	}

	void ARK::Core::GameContainer::initGamepads() {

	}

	void ARK::Core::GameContainer::setCursorVisible(bool b) {

	}

	void ARK::Core::GameContainer::resizeWindowToFitViewport() {

	}

	void ARK::Core::GameContainerPlatform::setTitle(string s) {
		// do nothing.
	}
	string ARK::Core::GameContainerPlatform::getResourcePath() const {
		return m_resourcePath;
	}

	void ARK::Core::GameContainer::start() {
	
		// seed the random
		Random::init();
		
		// Set window title
		//setTitle(m_game.getTitle());
		
		// populate the gamepads.
		ARK2D::getLog()->i("Initialising Gamepads... ");
		initGamepads();
		ARK2D::getLog()->i("done.");

		// initialise 
		enableOpenAL();
 
		// Start App Delegate
		//NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
		//UIApplicationMain(0, NULL, nil, @"GameContainerIPhoneAppDelegate");
		
		/*int retVal = 0;
		NSString* classString = NSStringFromClass([GameContainerIPhoneAppDelegate class]);
		@try {
            retVal = UIApplicationMain(0, NULL, nil, classString); 
        }
        @catch (NSException *exception) { 
            NSLog(@"Exception - %@",[exception description]);
            exit(EXIT_FAILURE); 
        }*/
		
        UIApplicationMain(0, NULL, nil, NSStringFromClass([GameContainerIPhoneAppDelegate class]));
		//[pool release];
 
		/*
		
		
		
		
		
		 
		while(m_bRunning) {
		
		}
		
		disableOpenAL();
		*/
	}
	GameContainerPlatform* ARK::Core::GameContainerPlatform::getInstance() {
		return &ARK2D::getContainer()->m_platformSpecific;
	}
	void ARK::Core::GameContainerPlatform::start() {
		return;
        
        // Setup Animation Frame
		//objc_object* s = (objc_object*) this;
		id s = (id) this;
        //id s = (id) &ARK2D::getContainer()->m_platformSpecific;
		CADisplayLink* aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:s selector:@selector(updateAndRender)];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        _displayLink = aDisplayLink;
        
        
        
        /*CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        _displayLink = aDisplayLink;*/
        

	}

	void ARK::Core::GameContainerPlatform::initGame() {
		ARK2D::getLog()->i("Initialising Log");
		ARK2D::getLog()->init();

		ARK2D::getLog()->i("Initialising Localisations");
		m_container->initLocalisation();
		
		// initialise game.
		ARK2D::getLog()->i("Initialising "); ARK2D::getLog()->i(m_container->m_game.getTitle()); ARK2D::getLog()->i("...");
		m_container->m_game.init(m_container);
		ARK2D::getLog()->i("Initialised ");  ARK2D::getLog()->i(m_container->m_game.getTitle()); ARK2D::getLog()->i("...");

		ARK2D::getLog()->i(StringUtil::append("Screen Scale: ", [UIScreen mainScreen].scale));
        CGRect screenRect = [[UIScreen mainScreen] bounds];
        CGFloat screenWidth = screenRect.size.width * [UIScreen mainScreen].scale; 
        CGFloat screenHeight = screenRect.size.height * [UIScreen mainScreen].scale; 
        m_container->setSize((int) screenWidth, (int) screenHeight); 
		
	}

	void ARK::Core::GameContainerPlatform::updateAndRender() {

		if (GameContainerPlatform::s_gamePaused) { 
			return;
		}
		
   		//ARK2D::getLog()->i("Doing A Frame, yay!");
        
		GameTimer* m_timer = &m_container->m_timer;
		Game* m_game = &m_container->m_game;
		Input* m_input = &m_container->m_input;
		Renderer* m_graphics = ARK2D::getRenderer();

		m_timer->tick();
		//doEvents();
		
		
		
	   
		//int delta = (int) (m_timer->getDelta() * 1000);
        m_container->scene->update();
        
		m_input->clearKeyPressedRecord();
		for (unsigned int i = 0; i < m_container->m_gamepads.size(); i++) {
			m_container->m_gamepads.at(i)->clearButtonPressedRecord();
		}
		
		RendererStats::reset();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_container->scene->render();
		//if (m_container->m_showingFPS) { m_container->renderFPS(); }
		
		 
    	//glColor4f(1.0, 1.0, 1.0, 1.0);
   	 	//m_graphics->drawRect(0,0,100,100);
    
    	m_container->swapBuffers();
    	
		 
		//usleep(delta * 500); // 0.017/2.
		m_timer->sleep(1); 
	}
 
	void ARK::Core::GameContainer::swapBuffers() {   
		glBindRenderbuffer(GL_RENDERBUFFER, m_platformSpecific._viewRenderBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_platformSpecific._viewFrameBuffer);
		[m_platformSpecific._context presentRenderbuffer:GL_RENDERBUFFER];

		showAnyGlErrorAndExitMacro();
	}

	void ARK::Core::GameContainer::close() const {
		exit(0);
	}

	void ARK::Core::GameContainerPlatform::end() { 
		[_context release];
    	_context = nil;
	}

	bool ARK::Core::GameContainerPlatform::isRetina() {
		return ([UIScreen mainScreen].scale == 2);
	}
	

	unsigned int ARK::Core::GameContainerPlatform::getOrientation() {
		UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
		if (orientation == 0 || orientation == UIInterfaceOrientationPortrait) {
			return GameContainer::ORIENTATION_PORTRAIT;
		} else if (orientation == UIInterfaceOrientationLandscapeLeft || orientation == UIInterfaceOrientationLandscapeRight) {
			return GameContainer::ORIENTATION_LANDSCAPE;
		}
		return GameContainer::ORIENTATION_PORTRAIT;
	}

	void ARK::Core::GameContainerPlatform::initOpenGL(GameContainerIPhoneGLView* view) 
	{
	    ARK2D::getLog()->v("enable opengl "); 

	    // globalise view reference
        m_glView = view;
//		m_window.rootViewController = m_glView.viewController;
        
        // Setup Layer
    	_eaglLayer = (CAEAGLLayer*) view.layer;
    	_eaglLayer.opaque = YES;
		//[_eaglLayer.drawableProperties setObject:@(YES) forKey:kEAGLDrawablePropertyRetainedBacking];


		// Setup Context 
	    _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	    if (!_context) {
	        NSLog(@"Failed to initialize OpenGLES 2.0 context");
	        exit(1);
	    }
		//_context.drawableProperties[kEAGLDrawablePropertyRetainedBacking] = YES;

		    	
	    
	    if (![EAGLContext setCurrentContext:_context]) {
	        NSLog(@"Failed to set current OpenGL context");
	        exit(1);
	    }  

	    //Image::showAnyGlErrorAndExit();
	    showAnyGlErrorAndExitMacro();

	    // Setup Render Buffer
	    ARK2D::getLog()->v("renderbuffers");
	    glGenRenderbuffers(1, &_viewRenderBuffer);
	    glBindRenderbuffer(GL_RENDERBUFFER, _viewRenderBuffer);        
	    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];    

	    //Image::showAnyGlErrorAndExit();
	    showAnyGlErrorAndExitMacro();

		// Setup Frame buffer
		ARK2D::getLog()->v("framebuffers");
    	glGenFramebuffers(1, &_viewFrameBuffer);
    	glBindFramebuffer(GL_FRAMEBUFFER, _viewFrameBuffer);
    	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _viewRenderBuffer);
 
    	//glBindRenderbuffer(GL_RENDERBUFFER, 0);        
    	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
 
    	//Image::showAnyGlErrorAndExit();
    	showAnyGlErrorAndExitMacro();

    	CGRect rect = view.bounds;
    	initOpenGL2(rect.size.width, rect.size.height);

	    ARK2D::getRenderer()->init();

	    // Load default font.
	    if (m_container->m_willLoadDefaultFont) { 
	    	ARK::Font::BMFont* fnt = ARK::Core::Resource::get("ark2d/fonts/default.fnt")->asFont()->asBMFont(); // BMFont("ark2d/fonts/default.fnt", "ark2d/fonts/default.png");
	    	fnt->scale(0.5f);
			m_container->m_graphics.m_DefaultFont = fnt;
			m_container->m_graphics.m_Font = fnt;
		} else {
			m_container->m_graphics.m_DefaultFont = NULL;
			m_container->m_graphics.m_Font = NULL;
		}

	}
 
	void GameContainerPlatform::initOpenGL2(int width, int height) {

    	// Setup Viewport 
    	ARK2D::getLog()->v("viewport");
	    glViewport(0, 0, width, height);

	    showAnyGlErrorAndExitMacro();

		ARK2D::getLog()->v("clear color");
		glClearColor(m_container->getClearColor().getRedf(), m_container->getClearColor().getGreenf(), m_container->getClearColor().getBluef(), m_container->getClearColor().getAlphaf());

		ARK2D::getLog()->v("clear");
		glClear( GL_COLOR_BUFFER_BIT ); 

		//Image::showAnyGlErrorAndExit(); 
		showAnyGlErrorAndExitMacro(); 
     
    	//glShadeModel(GL_SMOOTH);
    	ARK2D::getLog()->v("enable blend");
    	glEnable(GL_BLEND);
    	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		showAnyGlErrorAndExitMacro();
    	 
		
	    //glDisable( GL_LIGHTING ); 
	    //glDisable( GL_DITHER );  

	    showAnyGlErrorAndExitMacro();
	}

	void ARK::Core::GameContainerPlatform::initOpenGL2D(int width, int height)
	{
		ARK2D::getLog()->v("enable 2d"); 
		//Image::showAnyGlErrorAndExit();
		showAnyGlErrorAndExitMacro();

		Renderer* r = ARK2D::getRenderer();
		r->matrixMode(MatrixStack::TYPE_PROJECTION);
		r->pushMatrix();
		r->loadIdentity(); 

		r->ortho2d(0, 0, width, height, -1, 1);

		r->matrixMode(MatrixStack::TYPE_VIEW);
		r->pushMatrix();
		r->loadIdentity();


		//Image::showAnyGlErrorAndExit(); 
		showAnyGlErrorAndExitMacro();

		ARK2D::getLog()->v("disables");
		ARK2D::getLog()->v("disables depth");
		glDisable( GL_DEPTH_TEST );
		//Image::showAnyGlErrorAndExit();
		showAnyGlErrorAndExitMacro();
 
    }

void beginInterruption() {
    //if (playing) {
    //    playing = NO;
    //    interruptedWhilePlaying = YES;
    //    [self updateUserInterface];
    //}
}

NSError *activationError = nil;
void endInterruption() {
    /*if (interruptedWhilePlaying) {
        BOOL success = [[AVAudioSession sharedInstance] setActive: YES error: &activationError];
        //if (!success) {  handle the error in activationError  }
        [player play];
        playing = YES;
        interruptedWhilePlaying = NO;
        [self updateUserInterface];
    }*/
}

void openALInterruptionListener (
								 void   *inClientData,
								 UInt32 inInterruptionState
								 ) {
	GameContainerPlatform* pl = (GameContainerPlatform*) inClientData;
    if (inInterruptionState == kAudioSessionBeginInterruption) {
        alcMakeContextCurrent (NULL);
	} else if (inInterruptionState == kAudioSessionEndInterruption) {
        alcMakeContextCurrent (pl->_ctx);
	}
    // other interruption-listener handling code
}

	bool ARK::Core::GameContainerPlatform::initOpenAL() {
	 /*
		if (alcGetCurrentContext() != NULL) {
			ErrorDialog::createAndShow("OpenAL is already initialised. "); //Exiting program.");
			//exit(0);
			return true; 
		}

		// Load OpenAL.
		ALCdevice* dev = NULL; 

		dev = alcOpenDevice(NULL);
		if(dev == NULL) {
			ErrorDialog::createAndShow("Could not open Audio Device.");
			exit(0);
		} else if (alcGetError(dev) != ALC_NO_ERROR) {
			ErrorDialog::createAndShow("Could not open Audio Device.");
			exit(0);
		}

		//! @todo: check the context attributes, maybe something is useful:
		// http://www.openal.org/openal_webstf/specs/oal11spec_html/oal11spec6.html
		// 6.2.1. Context Attributes
		// my bet is on ALC_STEREO_SOURCES
		ALCcontext* ctx = NULL;
		ctx = alcCreateContext(dev, NULL);
		if(ctx == NULL) {
			ErrorDialog::createAndShow("Could not create Audio Context.");
			return false;
		} else if (alcGetError(dev) != ALC_NO_ERROR) {
			ErrorDialog::createAndShow("Could not create Audio Context.");
			return false;
		}

		ALboolean b = alcMakeContextCurrent(ctx);
		if (b != ALC_TRUE) {
			ErrorDialog::createAndShow("Could not make Audio Context current.");
			exit(0);
		}

		if (alcGetError(dev) != ALC_NO_ERROR) {
			ErrorDialog::createAndShow("Problem with Audio Device.");
			exit(0);
		}

		//alcProcessContext(ctx);

		alListenerfv(AL_POSITION,    Sound::ListenerPos);
		alListenerfv(AL_VELOCITY,    Sound::ListenerVel);
		alListenerfv(AL_ORIENTATION, Sound::ListenerOri);

		if (alGetError() != AL_NO_ERROR) {
			ErrorDialog::createAndShow("Could not set OpenAL Listener");
			exit(0);
		}

		std::cout << "Initialised OpenAL" << std::endl;
		return true;*/

		AudioSessionInitialize (
								NULL,                            // 1
								NULL,                            // 2
								openALInterruptionListener,    // 3
								(void*) this                         // 4
								);

				ARK2D::getLog()->v("Does OpenAL context already exist?");
				if (alcGetCurrentContext() != NULL) {
					ErrorDialog::createAndShow("OpenAL is already initialised. Exiting program.");
					exit(0);
				}
			
				// Load OpenAL.
				ARK2D::getLog()->v("OpenAL: Open Device");
				ALCdevice* dev = NULL;
				dev = alcOpenDevice(NULL);
				if(dev == NULL) {
					ErrorDialog::createAndShow("Could not open Audio Device.");
					exit(0);
				} else if (alcGetError(dev) != ALC_NO_ERROR) {
					ErrorDialog::createAndShow("Could not open Audio Device.");
					exit(0);
				}
			
				//! @todo: check the context attributes, maybe something is useful:
				// http://www.openal.org/openal_webstf/specs/oal11spec_html/oal11spec6.html
				// 6.2.1. Context Attributes
				// my bet is on ALC_STEREO_SOURCES
				ARK2D::getLog()->v("OpenAL: Create Context");
				_ctx = NULL; 
				_ctx = alcCreateContext(dev, NULL);
				if(_ctx == NULL) {
					ErrorDialog::createAndShow("Could not create Audio Context.");
					return false;
				} else if (alcGetError(dev) != ALC_NO_ERROR) {
					ErrorDialog::createAndShow("Could not create Audio Context.");
					return false;
				} 
			
				ARK2D::getLog()->v("OpenAL Make context current");
				ALboolean b = alcMakeContextCurrent(_ctx);
				if (b != ALC_TRUE) {
					ErrorDialog::createAndShow("Could not make Audio Context current.");
					exit(0);
				} 
			
				if (alcGetError(dev) != ALC_NO_ERROR) {
					ErrorDialog::createAndShow("Problem with Audio Device.");
					exit(0);  
				} 
			 
				//alcProcessContext(ctx);
			
				alListenerfv(AL_POSITION,    ARK::Audio::Sound::ListenerPos);
				alListenerfv(AL_VELOCITY,    ARK::Audio::Sound::ListenerVel);
				alListenerfv(AL_ORIENTATION, ARK::Audio::Sound::ListenerOri);
			
				if (alGetError() != AL_NO_ERROR) {
					ErrorDialog::createAndShow("Could not set OpenAL Listener");
					exit(0);
				}
			 
				ARK2D::getLog()->v("Initialised OpenAL");
				return true;
	}
	
	bool ARK::Core::GameContainerPlatform::deinitOpenAL() {
		return true;
	}
	
	int ARK::Core::GameContainer::getGlobalMouseX() const {
		return 0;
	}
	int ARK::Core::GameContainer::getGlobalMouseY() const {
		return 0;
	}
	
	

#endif
