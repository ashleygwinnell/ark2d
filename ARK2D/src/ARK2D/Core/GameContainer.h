/*
 * GameContainer.h
 *
 *  Created on: 15-Oct-2009
 *      Author: Ashley
 */

#ifndef GAMECONTAINER_H_
#define GAMECONTAINER_H_

#include "../Namespaces.h"
#include "../ARK2D.h"
#include "../Includes.h"

#include "Game.h"
#include "../Controls/Gamepad.h"
#include "../Controls/Input.h"
#include "../Core/GameTimer.h"
#include "../Graphics/Renderer.h"
#include "../Font/BMFont.h"
#include "../Graphics/Image.h"
#include "../UI/ErrorDialog.h"
#include "../Util/Log.h"

#include "../Audio/Sound.h"

#include <vector>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>

//template <class T>
//class GigaRectangle;
#if defined(ARK2D_FLASCC)
 	#include "Platform/GameContainerFlascc.h"
#elif defined(ARK2D_ANDROID)
	#include "Platform/GameContainerAndroid.h"
#elif defined(ARK2D_IPHONE)
	#include "Platform/GameContainerIPhone.h"
#elif defined(ARK2D_WINDOWS)
	#include "Platform/GameContainerWindows.h"
#elif defined(ARK2D_UBUNTU_LINUX)
	#include "Platform/GameContainerLinux.h"
#elif defined(ARK2D_MACINTOSH)
	#include "Platform/GameContainerMac.h"
#endif

using namespace std;

namespace ARK {
	namespace Core {

		/*!
		 * \brief Create this class in your main method. It is the main entry point for the library.
		 *
		 * The main container for a game that holds references to context-aware input,
		 * the game itself and controls the game loop with frame limiting, etc.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class GameContainer {
			friend class ARK::Core::GameContainerPlatform;
			friend class ARK::Graphics::Renderer;
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

				void setSize(int width, int height);

				bool isResizable();
				void setResizable(bool fs);

				void setScaleToWindow(bool b);
				unsigned int getFPS();
				void setIcon(const std::string& path);
				void start();

				void setCursorVisible(bool b);

				void setClearColor(const Color& color);
				const Color& getClearColor();

				GameTimer* getTimer();

				const ARK::Geometry::Rectangle<int>& getWindowRectangle() const;
				const Game& getGame() const;
				const Input& getInput() const;
				Renderer* getRenderer() const;
				inline unsigned int getWidth() const { return m_originalWidth; } // dimensions defined in main or config
				inline unsigned int getHeight() const { return m_originalHeight; }
				unsigned int getDynamicWidth() const; // actual dimensions of the container. e.g. 800*480
				unsigned int getDynamicHeight() const;
				unsigned int getScreenWidth() const; // on pc, screen resolution. on mobile, same as dynamic widths.
				unsigned int getScreenHeight() const;
				float getScale() const; // actual scale.
				float getScaleX() const; // display scale
				float getScaleY() const; // display scale
				float getTranslateX() const;
				float getTranslateY() const;
				vector<Gamepad*> getGamepads() const;

				// always ends in the trailing slash.
				string getResourcePath() const;

				int getResizeBehaviour();
				void setResizeBehaviour(int b);

				static const unsigned int ORIENTATION_PORTRAIT = 0;
				static const unsigned int ORIENTATION_LANDSCAPE = 1;
				unsigned int getOrientation();

				void saveScreenshot(string filename);

				bool isShowingFPS();
				void setShowingFPS(bool b);
		#if defined(ARK2D_ANDROID) || defined(ARK2D_FLASCC)
				void renderFPS();
		#else
			private:
				void renderFPS();
			public:
		#endif

		#if (defined(ARK2D_MACINTOSH) || defined(ARK2D_WINDOWS) || defined(ARK2D_FLASCC))
				void resizeBehaviour(int width, int height);
		#endif

				void close() const;
				virtual ~GameContainer();

				void enable2D();
				void disable2D();

			#ifdef ARK2D_FLASCC
				public:
			#else 
				private:
			#endif


				void resize(int w, int h);
				
				void enableOpenAL();
				void disableOpenAL();
				void swapBuffers();



				float time();

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
				vector<Gamepad*> m_gamepads;

				string m_strTitle;
				unsigned int m_originalWidth;
				unsigned int m_originalHeight;
				unsigned int m_width;
				unsigned int m_height;
				unsigned int m_screenWidth;
				unsigned int m_screenHeight;
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


				Color m_clearColor;

			public:
				bool m_bRunning;
			private:
				int m_resizeBehaviour;
				bool m_showingFPS;



			// Platform Specific
			public:
				GameContainerPlatform m_platformSpecific;
				static const int RESIZE_BEHAVIOUR_SCALE = 0;
				static const int RESIZE_BEHAVIOUR_NOSCALE = 1;



		};
	}
}





#endif /* GAMECONTAINER_H_ */
