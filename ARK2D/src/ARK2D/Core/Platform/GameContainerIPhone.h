/*
 * GameContainerIPhone.h
 *
 *  Created on: 16 May 2012
 *      Author: ashleygwinnell
 */

#ifndef GAMECONTAINERIPHONE_H_
#define GAMECONTAINERIPHONE_H_

#include "../../Includes.h"
#include "../../Namespaces.h"
#include "../../ARK2D.h"

	#ifdef ARK2D_IPHONE

		#include <string>
		using namespace std;

		#import "GameContainerIPhoneAppDelegate.h"
		#import "GameContainerIPhoneGLView.h"
		
		namespace ARK {
			namespace Core {

				class GameContainerPlatform {
					friend class GameContainer;
					public:
						GameContainer* m_container;
                        GameContainerIPhoneGLView* m_glView;
						string m_resourcePath;
						
						CAEAGLLayer* _eaglLayer;
    					EAGLContext* _context;
    					CADisplayLink* _displayLink;

    					GLuint _viewRenderBuffer;// OpenGL names for the renderbuffer and framebuffers used to render to this view
						GLuint _viewFrameBuffer;

						void setTitle(string s);
						string getResourcePath() const;
						static void* getARK2DResource(int resourceId, int resourceType);

						void initOpenGL(GameContainerIPhoneGLView* view);
						void initOpenGL2D(int width, int height);
						void deinitOpenGL();

						bool initOpenAL();
						bool deinitOpenAL();

						void initGame();
						void deinitGame();

						unsigned int getOrientation();

						static GameContainerPlatform* getInstance();
						void start();
						void updateAndRender();
						void end();

				};

			}
		}

	#endif

#endif /* GAMECONTAINERIPHONE_H_ */
