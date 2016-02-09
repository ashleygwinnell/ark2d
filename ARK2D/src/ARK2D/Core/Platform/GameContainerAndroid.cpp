/*
 * GameContainerAndroid.cpp
 *
 *  Created on: Feb 13, 2012
 *      Author: ashleygwinnell
 */

#include "GameContainerAndroid.h"
#include "../GameContainer.h"
#include "../../ARK2D.h"

#include "../../Graphics/Image.h" 
#include "../../Audio/Sound.h" 
#include "../Camera.h" 

namespace ARK {
	namespace Core {
 
		bool GameContainerPlatform::s_nativeResizing = false;
		bool GameContainerPlatform::s_gamePaused = false;

		GameContainer::GameContainer(Game& g, int width, int height, int bpp, bool fullscreen):
			m_timer(),
			m_game(g),
			m_input(),
			m_graphics(),
			m_gamepads(), 
			hints(),
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
			m_platformSpecific.m_container = this;

			m_input.setGameContainer(this);

			ARK2D::s_container = this;
			ARK2D::s_game = &m_game;
			ARK2D::s_graphics = &m_graphics;
			ARK2D::s_input = &m_input;
			ARK2D::s_camera = new ARK::Core::Camera();
			ARK2D::s_camera->setViewport(0, 0, m_width, m_height);
			//ARK2D::s_camera->setClipping(-1, 1);
			ARK2D::s_log = ARK::Util::Log::getInstance();
			scene = new Scene();
			scene->addChild(ARK2D::s_camera);
			scene->addChild(ARK2D::s_game);
			scene->addChild(ARK2D::s_log);
			scene->addChild(new LetterboxNode());


			ARK2D::getRenderer()->preinit();
		}

		
		void GameContainer::setSizeNoCallback(int width, int height) {
			setSize(width, height, false);
		}

		void GameContainer::setSize(int width, int height, bool docallback) {
			m_screenWidth = width;
			m_screenHeight = height; 
			/*
			if (m_resizeBehaviour == RESIZE_BEHAVIOUR_SCALE) {
				m_scaleX = float(width) / float(m_originalWidth);
				m_scaleY = float(height) / float(m_originalHeight);
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
				m_width = width;
				m_height = height;
				ARK2D::s_game->resize(this, width, height);
			} else if (m_resizeBehaviour == RESIZE_BEHAVIOUR_NOSCALE) {
				m_width = width;
				m_height = height;
				ARK2D::s_game->resize(this, width, height);
			}*/
			resizeBehaviour(width, height, docallback);
		}

		void GameContainer::setFullscreen(bool fullscreen) {
			m_fullscreen = fullscreen;
		}

		int GameContainer::getGlobalMouseX() const {
			return 0;
		}
		int GameContainer::getGlobalMouseY() const {
			return 0;
		}
		void GameContainer::setIcon(const std::string& path){

		}
		void GameContainer::setCursorVisible(bool b) {

		} 

		void GameContainer::start() {

		}  

		void GameContainer::close() const {
			//exit(0);
			m_platformSpecific.m_pluggable->container_close();
		} 

		GameContainerPlatform::GameContainerPlatform():
			m_container(NULL),
			m_pluggable(NULL),
			m_gamepadId(0),
			m_jvm(NULL)
			{
			m_pluggable = new ARK::Core::AndroidPluggable();
		}

		void GameContainerPlatform::setTitle(string t) {

		}
		void GameContainerPlatform::initGL(string clearColorStr, int w, int h) {
			ARK2D::getLog()->i("init opengl");

			Color clearColor;  
			if (clearColorStr.length() > 0) { 
				Color cc(clearColorStr);
				m_container->m_clearColor = cc; 
		 		clearColor = cc;
			} else {
				clearColor = m_container->m_clearColor;
			} 
			m_container->setClearColor(clearColor);

			Image::showAnyGlErrorAndExit();

			//glEnable(GL_TEXTURE_2D);

			ARK2D::getLog()->v("viewport");
			glViewport(0, 0, w, h); 

			ARK2D::getLog()->v("clear color");
			glClearColor(clearColor.getRedf(), clearColor.getGreenf(), clearColor.getBluef(), clearColor.getAlphaf());

			ARK2D::getLog()->v("clear");
			glClear( GL_COLOR_BUFFER_BIT );

			Image::showAnyGlErrorAndExit();

			//ARK2D::getLog()->v("shade model");
			//glShadeModel(GL_SMOOTH);

			//glDisable(GL_DEPTH_TEST);

			Image::showAnyGlErrorAndExit();
 
			ARK2D::getLog()->v("enable blend");
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			Image::showAnyGlErrorAndExit();
		}
		void GameContainerPlatform::initGL2D(int w, int h) {
			
			//enable 2d?
			/*ARK2D::getLog()->v("projection");
			glPushMatrix();
			glLoadIdentity();

			Image::showAnyGlErrorAndExit();

			ARK2D::getLog()->v("ortho");
			glOrthof(0, w, h, 0, -1, 1);

			Image::showAnyGlErrorAndExit();

			ARK2D::getLog()->v("modelview");
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity(); */

			ARK2D::getLog()->v("enable 2d"); 
			Image::showAnyGlErrorAndExit();

			/*Renderer* r = ARK2D::getRenderer();
			r->matrixMode(MatrixStack::TYPE_PROJECTION);
			r->pushMatrix();
			r->loadIdentity(); 

			r->ortho2d(0, 0, w, h, -1, 1);

			r->matrixMode(MatrixStack::TYPE_VIEW);
			r->pushMatrix();
			r->loadIdentity();
			 
			Image::showAnyGlErrorAndExit(); 

			ARK2D::getLog()->v("disables");
			ARK2D::getLog()->v("disables depth");
			glDisable( GL_DEPTH_TEST );
			Image::showAnyGlErrorAndExit();*/

			//ARK2D::getLog()->v("disables lighting");
			//glDisable( GL_LIGHTING ); 
			//Image::showAnyGlErrorAndExit();
			
			//ARK2D::getLog()->v("disables dither");
			//glDisable( GL_DITHER );
			//
		}

		bool GameContainerPlatform::initOpenAL() {
			 
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
			return true;
		}

		bool GameContainerPlatform::deinitOpenAL() {
			return true;
		}

		string GameContainerPlatform::getResourcePath() const {
			return "assets/";
		}
		string GameContainerPlatform::getExternalResourcePath() const {
			return m_externalDataStr;
		}

		void* GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
			return NULL;
		}

		GameContainerPlatform::~GameContainerPlatform() {

		}

		// Android pluggable.
		string AndroidPluggable::urlRequest(JNIEnv* env, string url) {
			return "";
		}
		string AndroidPluggable::urlRequestThreaded(string url) { 
			return "";
		}
		bool AndroidPluggable::isNetworkAvailable() {
			
		}
		void AndroidPluggable::openBrowserToUrl(string url) {
			
		}
		void AndroidPluggable::openGalleryToImageUrl(string url) {
			
		}
		void AndroidPluggable::openGooglePlayStore(string packageName) {
			
		}
		void AndroidPluggable::openErrorDialog(string message) {

		}
		void AndroidPluggable::openSoftwareKeyboard(){

		}
		void AndroidPluggable::closeSoftwareKeyboard() {

		}
		void AndroidPluggable::openInputDialog(unsigned int jniCallbackId, string title, string defaultStr) {

		}
		string AndroidPluggable::getInputDialogText() {
			return "";
		}


		bool AndroidPluggable::vibrator_hasVibrator() {
			
		}
		void AndroidPluggable::vibrator_vibrate(int millis) {

		}
    	void AndroidPluggable::vibrator_cancel() {

    	}

		void AndroidPluggable::ga_sendSocial(string network, string action, string targeturl) {

		}
		void AndroidPluggable::ga_sendEvent(string category, string action, string label, long opt_value) {

		}
		void AndroidPluggable::ga_sendTiming(long loadTime, string category, string name, string label) {

		}

		void AndroidPluggable::share_googleplus(string text) {

		}
		void AndroidPluggable::thread_start(unsigned int thread_id) {
			
		}

		void AndroidPluggable::googleplaygameservices_signIn() {
			
		}
		void AndroidPluggable::googleplaygameservices_signOut() {
			
		}		
		bool AndroidPluggable::googleplaygameservices_isSignedIn() {
			return false;
		}
		bool AndroidPluggable::googleplaygameservices_isSigningIn() {
			return false;
		}
		void AndroidPluggable::googleplaygameservices_viewAchievements() {
			
		}
		void AndroidPluggable::googleplaygameservices_unlockAchievement(string id) {
			
		}
		void AndroidPluggable::googleplaygameservices_viewScores(string id) {

		}
		void AndroidPluggable::googleplaygameservices_submitScore(string id, int score) {
			
		}

		void AndroidPluggable::container_close() {
			
		}

		bool AndroidPluggable::ouya_isOuya() {
			return false;
		}
		void AndroidPluggable::ouya_requestPurchase(string name) {

		}
		string AndroidPluggable::ouya_getUsername() {
			return "";
		}

		bool AndroidPluggable::firetv_isAmazonFireTV() {
			return false;
		}
		string AndroidPluggable::firetv_getUsername() {
			return "";
		}
		void AndroidPluggable::firetv_viewAchievements() { 

		}
		void AndroidPluggable::firetv_unlockAchievement(string id) { 

		}
		void AndroidPluggable::firetv_viewScores() { 

		}
		void AndroidPluggable::firetv_viewScores(string id) { 

		}
		void AndroidPluggable::firetv_submitScore(string id, int score) { 

		}

		// bool AndroidPluggable::googleplaygameservices_isConnected() {
		// 	return false;
		// }	
		// bool AndroidPluggable::googleplaygameservices_isConnecting() {
		// 	return false;
		// }


	}
}
