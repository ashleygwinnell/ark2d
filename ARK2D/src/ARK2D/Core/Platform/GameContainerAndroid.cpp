/*
 * GameContainerAndroid.cpp
 *
 *  Created on: Feb 13, 2012
 *      Author: ashleygwinnell
 */

#include "GameContainerAndroid.h"
#include "../GameContainer.h"
#include "../../ARK2D.h"

namespace ARK {
	namespace Core {

		GameContainer::GameContainer(Game& g, int width, int height, int bpp, bool fullscreen):
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

		}

		void GameContainer::setSize(int width, int height) {
			m_screenWidth = width;
			m_screenHeight = height;

			if (m_resizeBehaviour == RESIZE_BEHAVIOUR_SCALE) {
				m_scaleX = (float) width / (float) m_originalWidth;
				m_scaleY = (float) height / (float) m_originalHeight;
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
				ARK2D::s_game->resize(this, width, height);
			} else if (m_resizeBehaviour == RESIZE_BEHAVIOUR_NOSCALE) {
				m_width = width;
				m_height = height;
				ARK2D::s_game->resize(this, width, height);
			}
		}

		void GameContainer::setFullscreen(bool fullscreen) {

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
			exit(0);
		}

		GameContainerPlatform::GameContainerPlatform():
			m_container(NULL),
			m_pluggable(NULL)
			{
			m_pluggable = new ARK::Core::AndroidPluggable();
		}

		void GameContainerPlatform::setTitle(string t) {

		}
		void GameContainerPlatform::initGL(string clearColorStr, int w, int h) {
			ARK2D::getLog()->i("init opengl");

			Color* clearColor;
			if (clearColorStr.size() > 0) {
				Color cc(clearColorStr);
				m_container->m_clearColor = cc;
				clearColor = &cc;
			} else {
				clearColor = &m_container->m_clearColor;
			} 



			glViewport(0, 0, w, h);
			glClearColor(clearColor->getRedf(),clearColor->getGreenf(), clearColor->getBluef(), clearColor->getAlphaf());
			glClear( GL_COLOR_BUFFER_BIT );

			glShadeModel(GL_SMOOTH);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		}
		void GameContainerPlatform::initGL2D(int w, int h) {
			//enable 2d?
			glMatrixMode(GL_PROJECTION) ;
			glPushMatrix();
			glLoadIdentity();

			glOrthof(0, w, h, 0, -1, 1);

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glDisable( GL_DEPTH_TEST );
			glDisable( GL_LIGHTING );
			glDisable( GL_DITHER );
		}

		bool GameContainerPlatform::initOpenAL() {
			if (alcGetCurrentContext() != NULL) {
				ErrorDialog::createAndShow("OpenAL is already initialised. Exiting program.");
				exit(0);
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
		string AndroidPluggable::urlRequest(string url) {
			return "";
		}
		void AndroidPluggable::openSoftwareKeyboard(){

		}
		void AndroidPluggable::closeSoftwareKeyboard() {

		}

	}
}
