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

	ARK::Core::GameContainer::GameContainer(Game& g, int width, int height, int bpp, bool fullscreen):
		m_timer(),
		m_game(g),
		m_input(),
		m_graphics(),
		m_gamepads(),
		m_originalWidth(width),
		m_originalHeight(height),
		m_width(width),
		m_height(height),
		m_screenWidth(0),
		m_screenHeight(0),
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
		m_clearColor(Color::black),
		m_resizeBehaviour(RESIZE_BEHAVIOUR_SCALE),
		m_showingFPS(false),
		m_platformSpecific()
		{
			
			m_platformSpecific.m_container = this;
		
			m_input.setGameContainer(this);

			ARK2D::s_container = this;
			ARK2D::s_game = &m_game;
			ARK2D::s_graphics = &m_graphics;
			ARK2D::s_input = &m_input;
			ARK2D::s_log = new ARK::Util::Log();
		
			
			// Get location of current app bundle and make sure there's a resources path.
			m_platformSpecific.m_resourcePath = [[[NSBundle mainBundle] resourcePath] fileSystemRepresentation];
			m_platformSpecific.m_resourcePath += "/data/";
			std::cout << "Resource path: " << m_platformSpecific.m_resourcePath << std::endl;
			
			
			

	} 

	void* ARK::Core::GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
		return NULL;
	}
	
	void ARK::Core::GameContainer::setSize(int width, int height) {
		
		String rsss("resizing: ");
		rsss += width;
		rsss += ", ";
		rsss += height;
		ARK2D::getLog()->i(rsss.get());
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

	   		
			
	   		if (getOrientation() == ORIENTATION_LANDSCAPE) {
   				m_scaleX = (float) height / (float) m_originalWidth;
				m_scaleY = (float) width / (float) m_originalHeight;
   			} else {
   				m_scaleX = (float) width / (float) m_originalWidth;
				m_scaleY = (float) height / (float) m_originalHeight; 
   			}
	   		

			if (m_scaleX > m_scaleY) {
				m_scale = m_scaleY;
				m_scaleX = m_scaleY;
				m_scaleY = 1.0f;
			} else { // y > x
				m_scale = m_scaleX;
				m_scaleY = m_scaleX;
				m_scaleX = 1.0f;
			}

			//m_width = m_originalWidth * m_scaleX;
			//m_height = m_originalHeight * m_scaleY;
			m_width = width;
			m_height = height;

		} else if (m_resizeBehaviour == RESIZE_BEHAVIOUR_NOSCALE) {
			m_width = width;
			m_height = height;
		}

        //unsigned int orient = getOrientation();
       // if (orient == GameContainer::ORIENTATION_LANDSCAPE) { // switch them around if it's landscape.
            ARK2D::s_game->resize(this, height, width);
        //} else {
            ARK2D::s_game->resize(this, width, height);
        //}

	}

	void ARK::Core::GameContainer::setFullscreen(bool fullscreen) {
		
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
		MathUtil::seedRandom();
		
		// Set window title
		//setTitle(m_game.getTitle());
		
		// populate the gamepads.
		ARK2D::getLog()->i("Initialising Gamepads... ");
		initGamepads();
		ARK2D::getLog()->i("done.");

		// initialise 
		enableOpenAL();

		// Start App Delegate
		NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
		//UIApplicationMain(0, NULL, nil, @"GameContainerIPhoneAppDelegate");
        UIApplicationMain(0, NULL, nil, NSStringFromClass([GameContainerIPhoneAppDelegate class]));
		[pool release];
 
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
		// initialise game.
		ARK2D::getLog()->i("Initialising "); ARK2D::getLog()->i(m_container->m_game.getTitle()); ARK2D::getLog()->i("...");
		m_container->m_game.init(m_container);
		ARK2D::getLog()->i("Initialised ");  ARK2D::getLog()->i(m_container->m_game.getTitle()); ARK2D::getLog()->i("...");

        CGRect screenRect = [[UIScreen mainScreen] bounds];
        CGFloat screenWidth = screenRect.size.width;
        CGFloat screenHeight = screenRect.size.height;
        m_container->setSize((int) screenWidth, (int) screenHeight);
		
	}

	void ARK::Core::GameContainerPlatform::updateAndRender() {
		
   		//ARK2D::getLog()->i("Doing A Frame, yay!");
        
		GameTimer* m_timer = &m_container->m_timer;
		Game* m_game = &m_container->m_game;
		Input* m_input = &m_container->m_input;
		Renderer* m_graphics = ARK2D::getRenderer();

		m_timer->tick();
		//doEvents();
		
		ARK2D::getLog()->update();

		
	   
		int delta = (int) (m_timer->getDelta() * 1000);
		m_game->preUpdate(m_container, m_timer);
		m_game->update(m_container, m_timer);
		m_game->postUpdate(m_container, m_timer);
		m_input->clearKeyPressedRecord();
		for (unsigned int i = 0; i < m_container->m_gamepads.size(); i++) {
			m_container->m_gamepads.at(i)->clearButtonPressedRecord();
		}
		
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_game->preRender(m_container, m_graphics);
		m_game->render(m_container, m_graphics);
		m_game->postRender(m_container, m_graphics);
		if (m_container->m_showingFPS) { m_container->renderFPS(); }
		ARK2D::getLog()->render();
		
    	//glColor4f(1.0, 1.0, 1.0, 1.0);
   	 	//m_graphics->drawRect(0,0,100,100);
    
    	[_context presentRenderbuffer:GL_RENDERBUFFER];
		 
		//usleep(delta * 500); // 0.017/2.
		m_timer->sleep(1);
	}

	void ARK::Core::GameContainer::close() const {
		exit(0);
	}

	void ARK::Core::GameContainerPlatform::end() { 
		[_context release];
    	_context = nil;
	}
	
	void ARK::Core::GameContainerPlatform::initOpenGL2D(int width, int height)
	{
		// Setup 2D
    	glMatrixMode(GL_PROJECTION) ;
	    glPushMatrix();
	    glLoadIdentity();

	  
        
        // Setup Viewport
    	CGRect rect = m_glView.bounds;
    	//int width = rect.size.width;
    	//int height = rect.size.height;
	    
        glOrthof(0, width, height, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
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
	    // globalise view reference
        m_glView = view;
        
        // Setup Layer
    	_eaglLayer = (CAEAGLLayer*) view.layer;
    	_eaglLayer.opaque = YES;

		// Setup Context
	    _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
	    if (!_context) {
	        NSLog(@"Failed to initialize OpenGLES 1.0 context");
	        exit(1);
	    }
	    
	    if (![EAGLContext setCurrentContext:_context]) {
	        NSLog(@"Failed to set current OpenGL context");
	        exit(1);
	    }  

	    // Setup Render Buffer
	    glGenRenderbuffers(1, &_viewRenderBuffer);
	    glBindRenderbuffer(GL_RENDERBUFFER, _viewRenderBuffer);        
	    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];    

		// Setup Frame buffer
    	glGenFramebuffers(1, &_viewFrameBuffer);
    	glBindFramebuffer(GL_FRAMEBUFFER, _viewFrameBuffer);
    	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _viewRenderBuffer);

    	// Setup Viewport 
    	CGRect rect = view.bounds;
	    glViewport(0, 0, rect.size.width, rect.size.height);
    	glClearColor(0.5, 0.5, 0.5, 1.0);
    	glClear( GL_COLOR_BUFFER_BIT );
     
    	glShadeModel(GL_SMOOTH);
    	glEnable(GL_BLEND);
    	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    	

    	glDisable( GL_DEPTH_TEST );
	    glDisable( GL_LIGHTING );
	    glDisable( GL_DITHER );

	    // Load default font.
	    ARK::Font::BMFont* fnt = ARK::Core::Resource::get("ark2d/fonts/default.fnt")->asFont()->asBMFont(); // BMFont("ark2d/fonts/default.fnt", "ark2d/fonts/default.png");
		m_container->m_graphics.m_DefaultFont = fnt;
		m_container->m_graphics.m_Font = fnt;

	}
	
	
	
	bool ARK::Core::GameContainerPlatform::initOpenAL() {
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
