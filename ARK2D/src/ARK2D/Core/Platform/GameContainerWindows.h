/*
 * GameContainerWindows.h
 *
 *  Created on: 27 Feb 2011
 *      Author: ashley
 */


#ifndef GAMECONTAINERWINDOWS_H_
#define GAMECONTAINERWINDOWS_H_

#include "../../ARK2D.h"

	#ifdef ARK2D_WINDOWS

		#define MAPVK_VK_TO_VSC 0
		#define MAPVK_VSC_TO_VK 1
		#define MAPVK_VK_TO_CHAR 2
		#define MAPVK_VSC_TO_VK_EX 3

		

			#include "../../Windres.h"
			#include "../../Includes.h"
			#include "../../Graphics/Image.h"

			#include <windows.h>
			#include <string>
			#include <vector>

			using namespace std;

			namespace ARK {
				namespace Core {
				class GameContainer;

					struct GameContainerDisplayMode {
						unsigned int width;
						unsigned int height;
						unsigned int bpp;
					};

					class GameContainerPlatform {
							friend class GameContainer;
					public:
						GameContainer* m_container;
						HINSTANCE m_hInstance;
						WNDCLASSEX m_windowClass;
						RECT m_windowRect;
						RECT m_screenResolutionRect; // the size of the user's screen.
						HDC m_hDeviceContext;
						HWND m_hWindow;
						HGLRC m_hRenderingContext;


						float m_lastTime;

						DEVMODE m_dmScreenSettings; // device mode contains fullscreen, bitmode. something else...
						string m_iconpath;
						BOOL perf_flag;        // Timer Selection Flag
						double time_factor;    // Time Scaling Factor
						LONGLONG last_time;    // Previous timer value

						vector<GameContainerDisplayMode> m_availableDisplayModes;
						GameContainerDisplayMode* findDisplayMode(unsigned int w, unsigned int h, unsigned int bpp);

						LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);

						void getWindowFlags(DWORD* windowflags_return, DWORD* exstyle_return, bool undecorated, bool child_window);

						void setTitle(std::string title);
						string getResourcePath() const;

						void enableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC);
						void disableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);

						bool initOpenAL();
						bool deinitOpenAL();

						static void* getARK2DResource(int resourceId, int resourceType);

					};

				}
			}

	#endif

#endif /* GAMECONTAINERWINDOWS_H_ */


