/*
 * GameContainerMac.h
 *
 *  Created on: Aug 9, 2011
 *      Author: ashleygwinnell
 */

#ifndef GAMECONTAINERMAC_H_
#define GAMECONTAINERMAC_H_

#include "../../Namespaces.h"
//#include "../../ARK2D.h"

	#ifdef ARK2D_MACINTOSH

		#include <string>
		using namespace std;

		#import "GameContainerMacAppDelegate.h"
		#import "GameContainerMacWindowListener.h"
		#include <string>
		#include <stdlib.h>

		// Gamepad headers
		#include <IOKit/hid/IOHIDLib.h>
		#include <limits.h> 
		#include <mach/mach.h>
		#include <mach/mach_time.h>


		namespace ARK {
			namespace Core {

				class GameContainerPlatform {
					friend class GameContainer;
					public:
						GameContainer* m_container;
						NSWindow* m_window;
						GameContainerMacWindowListener* m_listener;
						GameContainerMacAppDelegate* m_appDelegate;
						string m_resourcePath;
						string m_resourcePathSandbox;
						NSOpenGLContext* m_context;

						// Gamepad vars
						IOHIDManagerRef m_hidManager;
						

						static void* getARK2DResource(int resourceId, int resourceType);

					public:
						NSOpenGLContext* createGLContext();
						void makeContextCurrent(NSWindow* window, NSOpenGLContext* context);
						void deleteGLContext(NSOpenGLContext* context);

						void doEvents();
						void handleKeyEvent(NSEvent* event);

						bool initOpenAL();
						bool deinitOpenAL();

						inline bool isRetina() { return false; } 

						bool isSandboxed();

					public:
						void setBorderlessWindow(bool b);

					public:

						void setTitle(std::string title);
						string getResourcePath() const;
				};

			}
		}

	#endif

#endif /* GAMECONTAINERMAC_H_ */
