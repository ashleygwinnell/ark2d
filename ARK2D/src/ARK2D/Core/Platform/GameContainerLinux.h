 /*
 * GameContainerLinux.h
 *
 *  Created on: 27 Feb 2011
 *      Author: ashley
 */



#ifndef GAMECONTAINERLINUX_H_
#define GAMECONTAINERLINUX_H_

#include "../../Includes.h"
#include "../../Namespaces.h"

	#if defined(ARK2D_UBUNTU_LINUX) && !defined(ARK2D_SDL2)

 		#include <X11/X.h>
		#include <X11/Xlib.h>
 		
		namespace ARK {
			namespace Core { 
				class GameContainer;

				// BIG HELP HERE, OKAY.
				// http://www.opengl.org/wiki/Programming_OpenGL_in_Linux:_GLX_and_Xlib

				enum Gamepad_eventType {
					GAMEPAD_EVENT_DEVICE_ATTACHED,
					GAMEPAD_EVENT_DEVICE_REMOVED,
					GAMEPAD_EVENT_BUTTON_DOWN,
					GAMEPAD_EVENT_BUTTON_UP,
					GAMEPAD_EVENT_AXIS_MOVED
				};
 
				struct Gamepad_buttonEvent {
					// Device that generated the event
					struct Gamepad* device;
					
					// Relative time of the event, in seconds
					double timestamp;
					
					// Button being pushed or released
					unsigned int buttonID;
					
					// True if button is down
					bool down;
				};

				struct Gamepad_axisEvent {
					// Device that generated the event
					struct Gamepad* device;
					
					// Relative time of the event, in seconds
					double timestamp;
					
					// Axis being moved
					unsigned int axisID;
					
					// Axis position value, in the range [-1..1]
					float value;
					
					// Previous axis position value, in the range [-1..1]
					float lastValue;
				};

				struct Gamepad_queuedEvent {
					unsigned int deviceID;
					enum Gamepad_eventType eventType;
					void * eventData;
				};

#define test_bit(bitIndex, array) \
	((array[(bitIndex) / (sizeof(int) * 8)] >> ((bitIndex) % (sizeof(int) * 8))) & 0x1)

				struct GameContainerPlatform_Screens {
					int id;
					int x; 
					int y;
					int width;
					int height;
				};

				class GameContainerPlatform {
					public:
						GameContainer*		 	m_container;	
						map<int, int>*			keycodemap;
						map<int, int>*			keycodemap_reverse;
						string 					m_iconPath;
						vector<GameContainerPlatform_Screens>* m_screens;

						Display*				dpy;
						Window                  root;
						static GLint 			att[11];
						XVisualInfo*			vi;
						Colormap                cmap;
						XSetWindowAttributes    swa;
						Window                  win;
						GLXContext              glc;
						XWindowAttributes       gwa;
						XEvent                  xev;

						

						string m_resourcePath;
						int m_globalMouseX;
						int m_globalMouseY;

						void enableOpenGL();//HWND hWnd, HDC* hDC, HGLRC* hRC);
						void disableOpenGL();//HWND hWnd, HDC hDC, HGLRC hRC);

						bool initOpenAL();
						bool deinitOpenAL();

						void setTitle(std::string title);

						string getResourcePath() const;

						inline bool isRetina() { return false; } 
						static void* getARK2DResource(int resourceId, int resourceType);

						void setWindowBordered(bool b);
						void setAlwaysOnTop(bool b);
						void setFocus(bool b);
						int getScreenForWindow();

						// Gamepad bits
						static pthread_mutex_t devicesMutex;
						static unsigned int numDevices;// = 0;
						static unsigned int nextDeviceID;// = 0;
						bool m_initialisedGamepads;

						static struct Gamepad_queuedEvent * eventQueue;
						static size_t eventQueueSize;
						static size_t eventCount;
						static pthread_mutex_t eventQueueMutex;

						void initGamepads();
						void detectGamepads();
						static void* deviceThread(void* context);
						
						void queueEvent(unsigned int deviceID, enum Gamepad_eventType eventType, void * eventData);
						void queueAxisEvent(struct Gamepad* device, double timestamp, unsigned int axisID, float value, float lastValue);
						void queueButtonEvent(struct Gamepad * device, double timestamp, unsigned int buttonID, bool down);


				};

				
			}
		}

	#endif

#endif /* GAMECONTAINERLINUX_H_ */
