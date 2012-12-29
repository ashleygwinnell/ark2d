/*
 * GameContainerFlascc.cpp
 *
 *  Created on: Dec 19, 2012 
 *      Author: ashleygwinnell 
 */
 
#include "../GameContainer.h"
#include "GameContainerMac.h"  
#include "../../Util/Log.h"   
 
#ifdef ARK2D_FLASCC  
  
			
			void* ARK::Core::GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
				return new string("");
			}
			
		
			ARK::Core::GameContainer::GameContainer(Game& g, int width, int height, int bpp, bool fullscreen):
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
				m_touchMode(false),
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
				
					
					//m_platformSpecific.m_window = window;
					
					
					m_bRunning = true;
		
			} 
			
			

			void ARK::Core::GameContainer::setSize(int width, int height) {

			}
		
			void ARK::Core::GameContainer::setFullscreen(bool fullscreen) {
				

				if (fullscreen && !m_fullscreen) {
					// go to fullscreen mode 

					inline_as3(
						"import com.adobe.flascc.Console;\n"\
						"Console.setFullscreen(true);\n"
					);

					int fs_width = m_originalWidth;
					int fs_height = m_originalHeight;
 
					inline_as3(
						"import com.adobe.flascc.Console;\n"\
						"%0 = Console.s_console.stage.fullScreenWidth;\n"\
						"%1 = Console.s_console.stage.fullScreenHeight;\n" 
						: "=r"(fs_width), "=r"(fs_height) : 
					);

					String s;
					s.append("changing to fullscreen mode. w: ");
					s.append(fs_width);
					s.append(" h: ");
					s.append(fs_height);
					ARK2D::getLog()->i(s.get()); 

					resizeBehaviour(fs_width, fs_height);
					//ARK2D::s_game->resize(this, fs_width, fs_height);

				} else if (!fullscreen && m_fullscreen) {
					// go back to normal flash window mode.

					inline_as3(
						"import com.adobe.flascc.Console;\n"\
						"Console.setFullscreen(false);\n"
					);

					resizeBehaviour(m_originalWidth, m_originalHeight);
					//ARK2D::s_game->resize(this, m_originalWidth, m_originalHeight);
				}

				m_fullscreen = fullscreen;
			}
		
			void ARK::Core::GameContainer::processGamepadInput() {
		
			}
		
			void ARK::Core::GameContainer::setIcon(const std::string& path) {
				//m_platformSpecific.m_iconpath = path;
			}
		
			void ARK::Core::GameContainer::initGamepads() {
		
			}
		
			void ARK::Core::GameContainer::setCursorVisible(bool b) {
		
			}
		
			void ARK::Core::GameContainer::resizeWindowToFitViewport() {
		
			}
		
			void ARK::Core::GameContainer::start() {
			
				// seed the random
				MathUtil::seedRandom();
				
				// Set window title
				//setTitle(m_game.getTitle());
				
				// populate the gamepads.
				ARK2D::getLog()->i("Initialising Gamepads... ");
				initGamepads();
				ARK2D::getLog()->i("done.");
				
				

				//enable gl
				ARK2D::getLog()->i("Enabling OpenGL.");
				//glLoadIdentity();
				//glViewport(0, 0, m_originalWidth, m_originalHeight);
				
				glClear( GL_COLOR_BUFFER_BIT );
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

				glDisable( GL_DEPTH_TEST );
	    		glDisable( GL_LIGHTING );
	    		glDisable( GL_DITHER );
 
		
				// enable 2d!
				ARK2D::getLog()->i("Enabling 2D");
				enable2D();
				glClearColor(m_clearColor.getRed()/255.0f, m_clearColor.getGreen()/255.0f, m_clearColor.getBlue()/255.0f, m_clearColor.getAlpha()/255.0f);
				
				// load default font. 
				ARK::Font::Font* fnt = ARK::Core::Resource::get("ark2d/fonts/default.fnt")->asFont(); // BMFont("ark2d/fonts/default.fnt", "ark2d/fonts/default.png");
				m_graphics.m_DefaultFont = fnt;
				m_graphics.m_Font = fnt; 
				//ARK2D::s_graphics->setDefaultFont(fnt);
				//ARK2D::s_graphics->setFont(fnt);

				std::cout << "font pointer: " << (int) (fnt) <<  " and null is: " << (int) (NULL) << "." << std::endl;
				
				// initialise 
				enableOpenAL();
				
				// initialise game.
				ARK2D::getLog()->i("Initialising ");
				ARK2D::getLog()->i(m_game.getTitle());
				ARK2D::getLog()->i("...");
				m_game.init(this);
		
				ARK2D::getLog()->i("Initialised "); 
				ARK2D::getLog()->i(m_game.getTitle());
				ARK2D::getLog()->i("...");

				if (m_fullscreen) {
					setFullscreen(true);
				}
				 
				AS3_GoAsync();
				//disableOpenAL();
				//close();
			}
 
			void GameContainerPlatform::doTick() {
				ARK2D::getLog()->i("Flascc Main Loop Tick");
				GameContainer* container = ARK2D::getContainer(); 

				GameTimer* m_timer = container->getTimer();
				Game* m_game = ARK2D::getGame();
				Input* m_input = ARK2D::getInput();
				Renderer* m_graphics = ARK2D::getRenderer();
				//vector<Gamepad*> m_gamepads = container->getGamepads();

				ARK2D::getLog()->i("Timer Tick");
				m_timer->tick();
				container->m_platformSpecific.doEvents();
				
				//container->processGamepadInput(); 
			   
				//ARK2D::getLog()->update();
			   
			   	ARK2D::getLog()->i("Game Pre-update");
			   	//ARK2D::getLog()->i("timerdelta:");
			   	//string deltastr = Cast::toString<int>(int(m_timer->getDelta()*1000));
			   	//ARK2D::getLog()->i(deltastr);
				//int delta = (int) (m_timer->getDelta() * 1000); 
				//ARK2D::getLog()->i("Game Pre-update 2");
				m_game->preUpdate(container, m_timer);			//ARK2D::getLog()->i("Game Pre-update 3");
				m_game->update(container, m_timer);				//ARK2D::getLog()->i("Game Pre-update 4");
				m_game->postUpdate(container, m_timer);

				ARK2D::getLog()->i("Clear Input");
				m_input->clearKeyPressedRecord();
				//for (unsigned int i = 0; i < m_gamepads.size(); i++) {
				//	m_gamepads.at(i)->clearButtonPressedRecord();
				//}
				
				ARK2D::getLog()->i("Game Pre-render");
				RendererStats::reset();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
				m_game->preRender(container, m_graphics);
				m_game->render(container, m_graphics);
				m_game->postRender(container, m_graphics);
				//if (container->isShowingFPS()) { container->renderFPS(); }
				ARK2D::getLog()->render();
				
				container->swapBuffers();
				ARK2D::getLog()->i("Game Post-render");
				 
				//usleep(delta * 500); // 0.017/2.
			}
		
			void ARK::Core::GameContainer::close() const {
				ARK2D::getLog()->i("GameContainer::close"); 
				//ARK2D::getLog()->i("Deleting Game object "); 
				//delete &m_game;

				//ARK2D::getLog()->i("Closing Window at last "); 
				//[m_platformSpecific.m_window close];
				//exit(0);
			}
			
			 
			void ARK::Core::GameContainer::swapBuffers() {
				/*NSOpenGLContext* context = [NSOpenGLContext currentContext];
				if (context != nil) {
					[context flushBuffer];
				}*/
			}
			
			
			
			
			
			/*void ARK::Core::GameContainerPlatform::handleKeyEvent(NSEvent* event) {
				NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
				
				unsigned short scancode = [event keyCode];
				unsigned int key;
				
				if (scancode < 128) {
					key = 0; //darwin_scancode_table[scancode];
				}
				else {
					//Debug::Log("Unknown scancode");
				}
				
				switch ([event type]) {
					case NSKeyDown:
						// if (![event isARepeat]) {
						//     // See if we need to rebuild the keyboard layout 
						//     UpdateKeymap(data);
						// }
						
						//CocoaPlatform::platform->SetLocalKey(key, true);
						break;
					case NSKeyUp:
					   // CocoaPlatform::platform->SetLocalKey(key, false);
						break;
						// case NSFlagsChanged:
						//     HandleModifiers(_this, scancode, [event modifierFlags]);
						//     break;
					default:  //just to avoid compiler warnings
						break;
				}*/
				
				//[pool release];
			//}
			
			void ARK::Core::GameContainerPlatform::doEvents() {
				
			}
			
			
			void ARK::Core::GameContainerPlatform::setTitle(std::string title) {
				//[m_window setTitle:[NSString stringWithCString:title.c_str() encoding:NSUTF8StringEncoding]];
			}
			
			string ARK::Core::GameContainerPlatform::getResourcePath() const {
				return "/";
			}
			
			bool ARK::Core::GameContainerPlatform::initOpenAL() {
				/*
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
			
				alListenerfv(AL_POSITION,    ARK::Audio::Sound::ListenerPos);
				alListenerfv(AL_VELOCITY,    ARK::Audio::Sound::ListenerVel);
				alListenerfv(AL_ORIENTATION, ARK::Audio::Sound::ListenerOri);
			
				if (alGetError() != AL_NO_ERROR) {
					ErrorDialog::createAndShow("Could not set OpenAL Listener");
					exit(0);
				}
			
				std::cout << "Initialised OpenAL" << std::endl;
				return true;*/
				return false;
			}
			
			bool ARK::Core::GameContainerPlatform::deinitOpenAL() {
				// discard context and device
				return true;
			}
			 
			int ARK::Core::GameContainer::getGlobalMouseX() const {
				//NSPoint mouseLoc = [NSEvent mouseLocation];
				//return mouseLoc.x;
				return 0;
			}
			int ARK::Core::GameContainer::getGlobalMouseY() const {
				//NSPoint mouseLoc = [NSEvent mouseLocation];
				//return mouseLoc.y;
				return 0;
			}
			
	

#endif
