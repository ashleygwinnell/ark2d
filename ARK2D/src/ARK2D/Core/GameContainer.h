/*
 * GameContainer.h
 *
 *  Created on: 15-Oct-2009
 *      Author: Ashley
 */

#ifndef ARK_CORE_GAMECONTAINER_H_
#define ARK_CORE_GAMECONTAINER_H_


#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "ARK2D.h"
#include "../Common/OpenGL.h"

#include "Game.h"
#include "Controls/Gamepad.h"
#include "Controls/Input.h"
#include "GameTimer.h"

//#include "../Font/BMFont.h"
//#include "../Graphics/Image.h"
#include "Graphics/Color.h"
#include "Graphics/Renderer.h"
#include "Controls/ErrorDialog.h"
#include "Log.h"

//#include "../Audio/Sound.h"

#include <vector>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

//template <class T>
//class GigaRectangle;
#if defined(ARK2D_FLASCC)
 	#include "Platform/GameContainerFlascc.h"
#elif defined(ARK2D_EMSCRIPTEN_JS)
 	#include "Platform/GameContainerHTML5.h"
#elif defined(ARK2D_ANDROID)
	#include "Platform/GameContainerAndroid.h"
#elif defined(ARK2D_IPHONE)
	#include "Platform/GameContainerIPhone.h"
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
#elif defined(ARK2D_XBOXONE) || defined(ARK2D_WINDOWS_STORE)
	#include "Platform/GameContainerXboxOne.h"
#endif



namespace ARK {
	namespace Core {

		//class ARK2D_API OrientationListener {
		//	public:
		//		virtual void onOrientationChanged(int newOrientation) = 0;
		//};

		/*!
		 * \brief Create this class in your main method. It is the main entry point for the library.
		 *
		 * The main container for a game that holds references to context-aware input,
		 * the game itself and controls the game loop with frame limiting, etc.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API GameContainer {
			friend class ARK::Core::GameContainerPlatform;
			friend class ARK::Core::Graphics::Renderer;
			friend class Log;
			friend class ARK::Core::Game;

			public:

				GameContainer(Game& game, int width, int height, int bpp, bool fullscreen);
				void setTitle(const std::string title);

				bool isVerbose();
				void setVerbose(bool v);

				bool isFullscreen();
				void setFullscreen(bool fs);

				bool isTouchMode();
				void setTouchMode(bool tm);

				void setSize(int width, int height, bool docallback = true);
				#if defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE) || defined(ARK2D_DESKTOP)
					// does the setSize without calling game::resize.
					// so the game can adjust without infinitely recursively looping.
					void setSizeNoCallback(int width, int height);
				#endif

				bool isResizable();
				void setResizable(bool fs);

				void setScaleToWindow(bool b);
				void setScaleLock(bool b);
				unsigned int getFPS();
				void setIcon(const std::string& path);
				void start();

				void setCursorVisible(bool b);

				void setCursorLocked(bool b);
				bool isCursorLocked();

				void setClearColor(const Color& color);
				const Color& getClearColor();

				GameTimer* getTimer();

				const ARK::Core::Geometry::RectangleTemplate<int>& getWindowRectangle() const;
				const Game& getGame() const;
				const Input& getInput() const;
				Renderer* getRenderer() const;
				//inline unsigned int getWidth() const { return m_originalWidth; } // dimensions defined in main or config
				//inline unsigned int getHeight() const { return m_originalHeight; }
				unsigned int getWidth() const;
				unsigned int getHeight() const;
				unsigned int getDynamicWidth() const; // actual dimensions of the container. e.g. 800*480
				unsigned int getDynamicHeight() const;
				unsigned int getScreenWidth() const; // on pc, screen resolution. on mobile, same as dynamic widths.
				unsigned int getScreenHeight() const;
				float getScale() const; // actual scale.
				float getScaleX() const; // display scale
				float getScaleY() const; // display scale
				void setScaleX(float f) { m_scaleX = f; }
				void setScaleY(float f) { m_scaleY = f; }
				float getTranslateX() const;
				float getTranslateY() const;
				void setTranslateX(float f) { m_translateX = f; }
				void setTranslateY(float f) { m_translateY = f; }
                vector<ARK::Core::Controls::Gamepad*>* getGamepads();

				// always ends in the trailing slash.
				string getResourcePath() const;

				int getResizeBehaviour();
				void setResizeBehaviour(int b);

				void setWillLoadDefaultFont(bool b);

				static const unsigned int ORIENTATION_PORTRAIT = 0;
				static const unsigned int ORIENTATION_LANDSCAPE = 1;
				static const unsigned int ORIENTATION_DUMMY = 2;
				unsigned int getOrientation();
				unsigned int getScreenOrientation();
				unsigned int getPreviousScreenOrientation();
				void setOrientationInverted(bool b);
				bool isOrientationInverted();
				bool is2in1Enabled();

				void saveScreenshot(string filename);

				bool isShowingFPS();
				void setShowingFPS(bool b);


				void renderFPS();
				virtual void preRender();
				virtual void postRender();
			public:


	//	#if (defined(ARK2D_MACINTOSH) || defined(ARK2D_WINDOWS) || defined(ARK2D_FLASCC) || defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
				void resizeBehaviourNoCallback(int width, int height);
				void resizeBehaviour(int width, int height, bool docallback=true);
				void resizeGame();
	//	#endif

				void setHint(unsigned int hinttype, unsigned int val);

				void close() const;
				virtual ~GameContainer();

				void enable2D();
				//void enable2DOriginal();
				void disable2D();

			#ifdef ARK2D_FLASCC
				public:
			#else
				private:
			#endif


				void resize(int w, int h);

				void enableOpenAL();
				void disableOpenAL();
			public:
				void swapBuffers();
			private:


				float time();
			public:
				void initLocalisation();

				void initGamepads();
				void processGamepadInput();
				void deinitGamepads();

				void resizeWindowToFitViewport();
			public:
				int getGlobalMouseX() const;
				int getGlobalMouseY() const;
			private:
				//ARK::Geometry::Rectangle<int>* m_window_rectangle;

			// Generic items.
				GameTimer m_timer;
				Game& m_game;
				Input m_input;
				Renderer m_graphics;
				vector<ARK::Core::Controls::Gamepad*> m_gamepads;
				map<unsigned int, unsigned int> hints;

			public:
				Scene* scene;
			private:

				string m_strTitle;

			public:
				unsigned int m_originalWidth;
				unsigned int m_originalHeight;

				unsigned int m_width;
				unsigned int m_height;

			private:
				unsigned int m_screenWidth;
				unsigned int m_screenHeight;
				bool m_scaleLock;
				float m_scale;
				float m_scaleX;
				float m_scaleY;
				int m_translateX; // used in scale mode on android to keep the game centered.
				int m_translateY;

				int m_bpp;
				bool m_fullscreen;
				bool m_resizable;
				bool m_scaleToWindow;
				bool m_verbose;
				bool m_touchMode;
				unsigned int m_screenOrientationPrevious;
				bool m_orientationInverted; // portrait game rotated landscape for example. useful in tablet/touch-based games that go fullscreen.
				bool m_2in1enabled;

				bool m_mouseLock = false;

				Color m_clearColor;

			public:
				bool m_bRunning;
			private:
				int m_resizeBehaviour;
				bool m_showingFPS;
				bool m_willLoadDefaultFont;



			// Platform Specific
			public:
				GameContainerPlatform m_platformSpecific;
				GameContainerPlatform* getPlatformSpecific() { return &m_platformSpecific; }
				static const int RESIZE_BEHAVIOUR_SCALE = 0;
				static const int RESIZE_BEHAVIOUR_NOSCALE = 1;

				static const unsigned int HINT_MULTISAMPLING = 0;
				static const unsigned int HINT_MULTISAMPLING_NONE = 1;
				static const unsigned int HINT_MULTISAMPLING_4X = 2;



		};
	}
}





#endif /* ARK_CORE_GAMECONTAINER_H_ */
