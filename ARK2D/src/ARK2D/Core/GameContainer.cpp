/*
 * GameContainer.cpp
 *
 *  Created on: 15-Oct-2009
 *      Author: Ashley Gwinnell
 */

#include "GameContainer.h"
//#include "../Geometry/GigaRectangle.h"
#include "../ARK2D.h"  

#if defined(ARK2D_FLASCC)
 	#include "Platform/GameContainerFlascc.h"
#elif defined(ARK2D_EMSCRIPTEN_JS)
 	#include "Platform/GameContainerHTML5.h"
#elif defined(ARK2D_ANDROID)
	#include "Platform/GameContainerAndroid.h"
#elif defined(ARK2D_WINDOWS)
	#include "Platform/GameContainerWindows.h"
#elif defined(ARK2D_UBUNTU_LINUX) 
 	#if defined(ARK2D_SDL2)
 		#include "Platform/GameContainerLinuxSDL2.h"
 	#else
		#include "Platform/GameContainerLinux.h"
 	#endif
#elif defined(ARK2D_MACINTOSH)
	#include "Platform/GameContainerMac.h"
#elif defined(ARK2D_WINDOWS_PHONE_8)
	#include "Platform/GameContainerWindowsPhone8.h"
#endif

#include "../Util/Callbacks.h"

namespace ARK {
	namespace Core { 

		bool GameContainer::isVerbose() {
			return m_verbose;
		}
		void GameContainer::setVerbose(bool v) {
			m_verbose = v;
		}
 
		bool GameContainer::isFullscreen() {
			return m_fullscreen; 
		}
 
		//const ARK::Geometry::Rectangle<int>& GameContainer::getWindowRectangle() const {
		//	return *m_window_rectangle;
		//}
		const Game& GameContainer::getGame() const {
			return m_game;
		}
		const Input& GameContainer::getInput() const {
			return m_input;
		}
		Renderer* GameContainer::getRenderer() const {
			return const_cast<Renderer*>(&m_graphics);
		}
		unsigned int GameContainer::getFPS() {
			return m_timer.getFPS();
		}

		unsigned int GameContainer::getWidth() const { 
			return m_originalWidth; 
		} 
		unsigned int GameContainer::getHeight() const { 
			return m_originalHeight; 
		}
		unsigned int GameContainer::getDynamicWidth() const {
			//if (m_orientationInverted) { return m_height; }
			return m_width;
		}
		unsigned int GameContainer::getDynamicHeight() const { // this changes on window resize/scaling.
			//if (m_orientationInverted) { return m_width; }
			return m_height;
		}

		/*unsigned int GameContainer::getWidth() const { // this is fixed at the static/initial/defined size
			return m_originalWidth;
		}
		unsigned int GameContainer::getHeight() const {
			return m_originalHeight;
		}*/

		unsigned int GameContainer::getScreenWidth() const {
			return m_screenWidth;
		}
		unsigned int GameContainer::getScreenHeight() const {
			return m_screenHeight;
		}

		float GameContainer::getScale() const {
			return m_scale;
		}
		float GameContainer::getScaleX() const {
			return m_scaleX;
		}
		float GameContainer::getScaleY() const {
			return m_scaleY;
		}

		float GameContainer::getTranslateX() const {
			return m_translateX;
		}
		float GameContainer::getTranslateY() const {
			return m_translateY;
		}

		void GameContainer::setTitle(const std::string title) {
			this->m_strTitle = title;
			m_platformSpecific.setTitle(title);
		}

		void GameContainer::enable2D() {
			ARK2D::getLog()->i("enable 2d");
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))// || defined(ARK2D_FLASCC))

			#else 
				Renderer* r = ARK2D::getRenderer();
				r->matrixMode(MatrixStack::TYPE_PROJECTION);
				r->pushMatrix();
				r->loadIdentity(); 
 
				//r->ortho2d(0, 0, m_width, m_height, -1.0f, 1.0f);
				r->ortho2d(0, 0, m_width, m_height, -1.0f, 1.0f);

				r->matrixMode(MatrixStack::TYPE_MODELVIEW);
				r->pushMatrix();
				r->loadIdentity();
				
				showAnyGlErrorAndExitMacro();
				
				/*glMatrixMode(GL_PROJECTION);
				glPushMatrix(); 
				glLoadIdentity();

				glOrtho(0, m_width, m_height, 0, -1, 1);

				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glLoadIdentity();*/
			#endif
			ARK2D::getLog()->i("enable 2d complete");
		}
		/*void GameContainer::enable2DOriginal() {
			ARK2D::getLog()->i("enable 2d original");
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))// || defined(ARK2D_FLASCC))

			#else
				Renderer* r = ARK2D::getRenderer();
				r->matrixMode(MatrixStack::TYPE_PROJECTION);
				r->pushMatrix();
				r->loadIdentity(); 
 
				r->ortho2d(0, 0, m_originalHeight, m_originalHeight, -1.0f, 1.0f);

				r->matrixMode(MatrixStack::TYPE_MODELVIEW);
				r->pushMatrix();
				r->loadIdentity();
				
				showAnyGlErrorAndExitMacro();
	
			#endif
			ARK2D::getLog()->i("enable 2d original complete");
		}*/

		void GameContainer::disable2D() {
			//ARK2D::getLog()->i("disable 2d");
			
			Renderer* r = ARK2D::getRenderer();
			r->matrixMode(MatrixStack::TYPE_PROJECTION);
			r->popMatrix();

			r->matrixMode(MatrixStack::TYPE_MODELVIEW);
			r->popMatrix();			

			/*Image::showAnyGlErrorAndExit();
			glMatrixMode(GL_PROJECTION); 
			glPopMatrix();

			Image::showAnyGlErrorAndExit();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();*/

			showAnyGlErrorAndExitMacro();
			//ARK2D::getLog()->i("disable 2d complete");
		}

		void GameContainer::enableOpenAL() {
			m_platformSpecific.initOpenAL();
		}
		void GameContainer::disableOpenAL() {
			m_platformSpecific.deinitOpenAL();
		}

		void GameContainer::resize(int w, int h) {
			this->m_width = w;
			this->m_height = h;
		}

		bool GameContainer::isShowingFPS() {
			return m_showingFPS;
		}
		void GameContainer::setShowingFPS(bool b) {
			m_showingFPS = b;
		}
		void GameContainer::renderFPS() {
			m_graphics.setDrawColorf(1.0f, 1.0f, 1.0f, 1.0f);  

			string fps = StringUtil::append("FPS: ", m_timer.getFPS());
			m_graphics.drawString(fps, getDynamicWidth() - 10 - m_graphics.getFont()->getStringWidth(fps), 10);
		}
 

		void GameContainer::deinitGamepads() {

		}
		vector<ARK::Controls::Gamepad*>* GameContainer::getGamepads() {
			return &m_gamepads; 
		}

		void GameContainer::setClearColor(const Color& color) {
			m_clearColor = color;
			m_graphics.setScissorBoxColors(color, color, color, color);
		}
		const Color& GameContainer::getClearColor() {
			return m_clearColor;
		}

		GameTimer* GameContainer::getTimer() {
			return &m_timer;
		}

		void GameContainer::setResizable(bool b){
			m_resizable = b;
		}
		bool GameContainer::isResizable() {
			return m_resizable;
		}
		void GameContainer::setScaleToWindow(bool b) {
			m_scaleToWindow = b;
		}
		void GameContainer::setScaleLock(bool b) {
			m_scaleLock = b;
		}

		string GameContainer::getResourcePath() const {
			return m_platformSpecific.getResourcePath();
		}

		int GameContainer::getResizeBehaviour() {
			return m_resizeBehaviour;
		}
		void GameContainer::setResizeBehaviour(int b) {
			m_resizeBehaviour = b;
		}

		void GameContainer::setWillLoadDefaultFont(bool b) {
			m_willLoadDefaultFont = b;
		}

		void ARK::Core::GameContainer::resizeBehaviourNoCallback(int width, int height) {
			resizeBehaviour(width, height, true);
		}
		void ARK::Core::GameContainer::resizeBehaviour(int width, int height, bool docallback) {

			if (m_resizeBehaviour == RESIZE_BEHAVIOUR_SCALE) 
		    {
		    	if (m_orientationInverted) {
		    		ARK2D::getLog()->i("--Resize behaviour--");
		    		ARK2D::getLog()->i(StringUtil::append("Width: ", width));
					ARK2D::getLog()->i(StringUtil::append("Height: ", height));

					ARK2D::getLog()->i(StringUtil::append("Original Width: ", m_originalWidth));
					ARK2D::getLog()->i(StringUtil::append("Original Height: ", m_originalHeight));

		    		m_scaleX = (float) height / (float) m_originalWidth;
					m_scaleY = (float) width / (float) m_originalHeight;

					if (m_scaleLock) {
						m_scaleX = width / m_originalWidth;	
						m_scaleY = height / m_originalHeight;	
					}

					ARK2D::getLog()->i(StringUtil::appendf("Scale X: ", m_scaleX));
					ARK2D::getLog()->i(StringUtil::appendf("Scale Y: ", m_scaleY));

					if (m_scaleX > m_scaleY) {
						m_scale = m_scaleY; 
						m_scaleX = m_scaleY;
					} else { // y > x 
						m_scale = m_scaleX;
						m_scaleY = m_scaleX;
					}
					
					ARK2D::getLog()->i(StringUtil::appendf("Scale: ", m_scale));

		    	} else { 
			    	m_scaleX = (float) width / (float) m_originalWidth;
					m_scaleY = (float) height / (float) m_originalHeight;

					if (m_scaleLock) {
						m_scaleX = width / m_originalWidth;	
						m_scaleY = height / m_originalHeight;	
					}

					if (m_scaleX > m_scaleY) {
						m_scale = m_scaleY;
						m_scaleX = m_scaleY;
					} else { // y > x 
						m_scale = m_scaleX;
						m_scaleY = m_scaleX;
					}
				}

				m_width = width;
				m_height = height;

				//#ifdef ARK2D_WINDOWS_PHONE_8
					//ARK2D::getLog()->i(StringUtil::append("", m_scaleX))
				//#endif

		   		
		    } else if (m_resizeBehaviour == RESIZE_BEHAVIOUR_NOSCALE) {
		    	m_width = width; 
		    	m_height = height;
		    	//ARK2D::s_game->resize(this, width, height);
		    	//ARK2D::getRenderer()->setScissorTestEnabled(false);
		    	//ARK2D::getRenderer()->scissor(0,0,width,height);
		    }
		    if (docallback) { 
		    	ARK2D::s_game->resize(this, width, height);
		    }
		}


		unsigned int GameContainer::getOrientation() {
			#if defined(ARK2D_WINDOWS) || defined(ARK2D_MACINTOSH) || defined(ARK2D_UBUNTU_LINUX) || defined(ARK2D_ANDROID) || defined(ARK2D_WINDOWS_PHONE_8)
				if (m_originalWidth > m_originalHeight) { 
					return ORIENTATION_LANDSCAPE;
				} else {
					return ORIENTATION_PORTRAIT;
				}
			#elif defined(ARK2D_IPHONE)
				return m_platformSpecific.getOrientation();
			#endif
			return ORIENTATION_PORTRAIT;
		}
		unsigned int GameContainer::getScreenOrientation() {
			#if defined(ARK2D_WINDOWS) || defined(ARK2D_MACINTOSH) || defined(ARK2D_UBUNTU_LINUX) || defined(ARK2D_ANDROID) || defined(ARK2D_WINDOWS_PHONE_8)
				return (m_screenWidth >= m_screenHeight) 
					? ORIENTATION_LANDSCAPE
					: ORIENTATION_PORTRAIT;
			#else
				ARK2D::getLog()->w("getScreenOrientation not support on this platform.");
			#endif
			return ORIENTATION_LANDSCAPE;		
		}
		unsigned int GameContainer::getPreviousScreenOrientation() {
			return m_screenOrientationPrevious;
		}

		void GameContainer::setOrientationInverted(bool b) {
			if (b != m_orientationInverted) {
				// orientation changed! 
				m_orientationInverted = b;
				resizeBehaviour(m_width, m_height);
			}
			
		}
		bool GameContainer::isOrientationInverted() {
			return m_orientationInverted;
		}
		bool GameContainer::is2in1Enabled() {
			return m_2in1enabled;
		}

		bool GameContainer::isTouchMode() {
			return m_touchMode; 
		}
		void GameContainer::setTouchMode(bool tm) {
			bool previous = m_touchMode;
			m_touchMode = tm;
			if (previous != tm) {
				Callbacks::invoke(Callbacks::CALLBACK_WINDOWS_TOUCHMODE_CHANGED);
			}
		}

		void GameContainer::saveScreenshot(string filename) {
			const unsigned int datalen = m_width * m_height;
			void* data = (void*) malloc(datalen); 
			ARK2D::getRenderer()->readPixels(data, 0, 0, m_width, m_height);
			 
			PNGImage::saveFile(filename, (char*) data, m_width, m_height);

			free(data);
		}

		GameContainer::~GameContainer() {

		}

	}
}
