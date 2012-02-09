/*
 * GameContainerMac.h
 *
 *  Created on: Aug 9, 2011
 *      Author: ashleygwinnell
 */

#ifndef GAMECONTAINERMAC_H_
#define GAMECONTAINERMAC_H_

#include "ARK2D.h"

	#ifdef ARK2D_MACINTOSH

		#include <string>
		using namespace std;

		#import <Cocoa/Cocoa.h>
		#import "GameContainerMacAppDelegate.h"
		#import "GameContainerMacWindowListener.h"
		#include <string>
		#include <stdlib.h>

		class GameContainerPlatform {
			friend class GameContainer;
			public:
				GameContainer* m_container;
				NSWindow* m_window;
				string m_resourcePath;
				NSOpenGLContext* m_context;

				static void* getARK2DResource(int resourceId, int resourceType);

			private:
				NSOpenGLContext* createGLContext();
				void makeContextCurrent(NSWindow* window, NSOpenGLContext* context);
				void deleteGLContext(NSOpenGLContext* context);

				void doEvents();
				void handleKeyEvent(NSEvent* event);

				bool initOpenAL();
				bool deinitOpenAL();

				void setTitle(std::string title);
				string getResourcePath() const;
		};

	#endif

#endif /* GAMECONTAINERMAC_H_ */
