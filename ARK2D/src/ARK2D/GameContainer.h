/*
 * GameContainer.h
 *
 *  Created on: 15-Oct-2009
 *      Author: user
 */

#ifndef GAMECONTAINER_H_
#define GAMECONTAINER_H_

#include "ARK2D.h"
#include "ARK2D_AL.h"
#include "ARK2D_GL.h"

#include "Gamepad.h"
#include "Game.h"
#include "Input.h"
#include "GameTimer.h"
//#include "GigaRectangle.h"
#include "Graphics.h"
#include "Font/BMFont.h"
#include "Image.h"
#include "ErrorDialog.h"
#include "OutputWrapper.h"

#include "Sound.h"

#include <vector>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>

template <class T>
class GigaRectangle;

#ifdef ARK2D_WINDOWS
	#include "GameContainerWindows.h"
#endif

#ifdef ARK2D_UBUNTU_LINUX
	#include "GameContainerLinux.h"
#endif

#ifdef ARK2D_MACINTOSH
	#include "GameContainerMac.h"
#endif

using namespace std;

class GameContainer {
	friend class GameContainerPlatform;
	friend class Graphics;
	friend class Input;

	public:

		GameContainer(Game& game, int width, int height, int bpp, bool fullscreen);
		void setTitle(const std::string title);

		bool isVerbose();
		void setVerbose(bool v);

		bool isFullscreen();
		void setFullscreen(bool fs);

		bool isResizable();
		void setResizable(bool fs);
		void setScaleToWindow(bool b);
		unsigned int getFPS();
		void setIcon(const std::string& path);
		void start();

		void setCursorVisible(bool b);

		void setClearColor(const Color& color);

		GameTimer* getTimer();

		const GigaRectangle<int>& getWindowRectangle() const;
		const Game& getGame() const;
		const Input& getInput() const;
		const Graphics* getGraphics() const;
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		vector<Gamepad*> getGamepads() const;

		// always ends in the trailing slash.
		string getResourcePath() const;

		void close() const;
		~GameContainer();

	private:

		void resize(int w, int h);
		void enable2D();
		void disable2D();
		void enableOpenAL();
		void disableOpenAL();
		void swapBuffers();
		float time();

		void initGamepads();
		void processGamepadInput();
		void deinitGamepads();

		void resizeWindowToFitViewport();

		int getGlobalMouseX() const;
		int getGlobalMouseY() const;

		GigaRectangle<int>* m_window_rectangle;


	// Generic items.
		GameTimer m_timer;
		Game& m_game;
		Input m_input;
		Graphics m_graphics;
		vector<Gamepad*> m_gamepads;

		string m_strTitle;
		unsigned int m_width;
		unsigned int m_height;
		int m_bpp;
		bool m_fullscreen;
		bool m_resizable;
		bool m_scaleToWindow;
		bool m_verbose;
		int m_fps;

		Color m_clearColor;

		bool m_bRunning;

	// Platform Specific
	public:
		GameContainerPlatform m_platformSpecific;




};





#endif /* GAMECONTAINER_H_ */
