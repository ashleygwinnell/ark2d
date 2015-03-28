/*
 * GameInterface.cpp
 *
 *  Created on: 15-Oct-2009
 *      Author: Ashley
 */
#include "Game.h"
#include "GameContainer.h" 
#include "../ARK2D.h"
#include "../Tween/Timeline.h"

#ifdef ARK2D_ANDROID
#include "Platform/GameContainerAndroid.h" 
#endif

namespace ARK {
	namespace Core {

		Game::Game(string title):
			m_container(NULL),
			m_title(title), 
			m_timeline(NULL) 
		{
			m_timeline = new Timeline();
		}

		string Game::getTitle() {
			return m_title;
		}
		Timeline* Game::getTimeline() {
			return m_timeline;
		}

		Game::~Game() {
			// delete &m_strTitle; std::strings clean themselves up!
			delete m_timeline;
		}

		//void Game::init(GameContainer& container) = 0;
		void Game::preUpdate(GameContainer* container, GameTimer* timer) { }
		void Game::postUpdate(GameContainer* container, GameTimer* timer) { }
		void Game::update(GameContainer* container, GameTimer* timer) {
			m_timeline->update(timer->getDelta());
		}

		void Game::preRender(GameContainer* container, Renderer* r) { 
			#if defined(ARK2D_ANDROID) || defined(ARK2D_WINDOWS_PHONE_8)

				r->pushMatrix();
				r->translate(container->getTranslateX(), container->getTranslateY());
				r->pushMatrix();
				r->scale(container->getScale(), container->getScale());
            #elif defined(ARK2D_IPHONE)
				if (container->getOrientation() == GameContainer::ORIENTATION_LANDSCAPE) {
					//g->pushMatrix();
					//g->translate(container->m_platformSpecific.m_glView.bounds.size.width, 0.0f );
					//g->rotate(-90.0f, 0.0f, 0.0f, 1.0f);
				}  
				
                r->pushMatrix();
                r->translate(container->getTranslateX(), container->getTranslateY());
                r->pushMatrix();
                r->scale(container->getScale(), container->getScale());
            #elif (defined(ARK2D_MACINTOSH) || defined(ARK2D_WINDOWS) || defined(ARK2D_FLASCC) || defined(ARK2D_UBUNTU_LINUX))

              	if (container->isOrientationInverted()) {

              		r->pushMatrix();
					r->rotate(-90);	
					
					r->pushMatrix();
					r->translate(container->getDynamicHeight()*-1.0f, 0.0f);

					r->pushMatrix();
					r->translate(container->getTranslateY(), container->getTranslateX()); // flipped because x is y and y is x...

					r->pushMatrix();
					r->scale(container->getScale(), container->getScale());

					//r->pushMatrix();
					//r->translate(0.0f, container->getTranslateY());

					r->pushMatrix();
				} else { 
					r->pushMatrix();
                	r->translate(container->getTranslateX(), container->getTranslateY());
                	r->pushMatrix();
                	r->scale(container->getScale(), container->getScale());
                }

            #else
                r->pushMatrix(); 
			#endif
		}
		
		void Game::postRender(GameContainer* container, Renderer* r) { 
			#if defined(ARK2D_IPHONE)
				r->popMatrix(); // pop scale
				r->popMatrix(); // pop translate
				if (container->getOrientation() == GameContainer::ORIENTATION_LANDSCAPE) {
				//	g->popMatrix(); // pop rotate
				}
				r->drawScissorBoxes(); 

			#elif (defined(ARK2D_MACINTOSH) || defined(ARK2D_WINDOWS) || defined(ARK2D_FLASCC) || defined(ARK2D_UBUNTU_LINUX))
				
				if (container->isOrientationInverted()) {
					r->popMatrix();
					r->popMatrix();
					r->popMatrix();
					r->popMatrix();
					r->popMatrix();
				} else {
					r->popMatrix(); // pop scale
					r->popMatrix(); // pop translate
				}
				r->drawScissorBoxes();
				
			#elif defined( ARK2D_ANDROID ) || defined(ARK2D_WINDOWS_PHONE_8)
				r->popMatrix(); // pop scale
				r->popMatrix(); // pop translate

				r->drawScissorBoxes(); 
			#else
				r->popMatrix();
			#endif
		}
		
		void Game::resize(GameContainer* container, int width, int height) {
			#if defined(ARK2D_ANDROID)
				if (container->getResizeBehaviour() == GameContainer::RESIZE_BEHAVIOUR_SCALE) {
					float tx = float(container->getDynamicWidth() - (float(container->getWidth())*container->getScale()))/2;
					float ty = float(container->getDynamicHeight() - (float(container->getHeight())*container->getScale()))/2;
					container->m_translateX = tx;
					container->m_translateY = ty;



					/*glLoadIdentity();
					glViewport( 
						0, 0,
						float(container->getWidth()) * container->getScaleX(), // original width.
						float(container->getHeight()) * container->getScaleY() // original height
					);*/
 
					// we pop to below the stack if this is the init cycle, so only do this on artificial resize.
					//if (!GameContainerPlatform::s_nativeResizing) { 
					if (!GameContainerPlatform::s_gamePaused) { 
						container->disable2D();  
					}
					//}   
  
					container->m_platformSpecific.initGL("", width, height);
					container->m_platformSpecific.initGL2D(width, height);
				}
				else
				{ 
 
				}
			#elif defined(ARK2D_IPHONE)
				if (container->getResizeBehaviour() == GameContainer::RESIZE_BEHAVIOUR_SCALE) {
					if (container->getOrientation() == GameContainer::ORIENTATION_PORTRAIT)
                 	{
                        float tx = float(container->getDynamicWidth() - (float(container->getWidth())*container->getScale()))/2;
                        float ty = float(container->getDynamicHeight() - (float(container->getHeight())*container->getScale()))/2;
                        container->m_translateX = tx;
                        container->m_translateY = ty;

                        container->m_platformSpecific.initOpenGL2(width, height);
                    	container->m_platformSpecific.initOpenGL2D(width, height);
                   } else {
                   	 	float tx = float(container->getDynamicWidth() - (float(container->getWidth())*container->getScale()))/2;
                        float ty = float(container->getDynamicHeight() - (float(container->getHeight())*container->getScale()))/2;
                        container->m_translateX = tx;
                        container->m_translateY = ty; 
   
                        container->m_platformSpecific.initOpenGL2(height, width);
                    	container->m_platformSpecific.initOpenGL2D(width, height);
  
                    	ARK2D::getLog()->v("RESIZE LANDSCAPE!");  
                    	ARK2D::getLog()->v(StringUtil::appendf("Width: ", width));
                    	ARK2D::getLog()->v(StringUtil::appendf("Height: ", height));
                    	ARK2D::getLog()->v(StringUtil::appendf("Translate X: ", tx));
                    	ARK2D::getLog()->v(StringUtil::appendf("Translate Y: ", ty)); 

                    	/*float tx = float(container->getDynamicHeight() - (float(container->getHeight())*container->getScale()))/2;
                        float ty = float(container->getDynamicWidth() - (float(container->getWidth())*container->getScale()))/2;
                        container->m_translateX = tx; 
                        container->m_translateY = ty;

                        

                    	container->m_platformSpecific.initOpenGL2(width, height); 
                    	container->m_platformSpecific.initOpenGL2D(width, height);*/
                    }
                    
                    

				}
			#elif defined(ARK2D_WINDOWS_PHONE_8)

				if (container->getResizeBehaviour() == GameContainer::RESIZE_BEHAVIOUR_SCALE) {
					float tx = float(container->getDynamicWidth() - (float(container->getWidth())*container->getScale()))/2;
					float ty = float(container->getDynamicHeight() - (float(container->getHeight())*container->getScale()))/2;
					container->m_translateX = tx;
					container->m_translateY = ty;

					ARK2D::getLog()->v(StringUtil::appendf("Normal Width: ", container->getWidth()));
					ARK2D::getLog()->v(StringUtil::appendf("Normal Height: ", container->getHeight()));

					ARK2D::getLog()->v(StringUtil::appendf("Dynamic Width: ", container->getDynamicWidth()));
					ARK2D::getLog()->v(StringUtil::appendf("Dynamic Height: ", container->getDynamicHeight()));

					ARK2D::getLog()->v(StringUtil::appendf("Screen Width: ", container->getScreenWidth()));
					ARK2D::getLog()->v(StringUtil::appendf("Screen Height: ", container->getScreenHeight()));

					ARK2D::getLog()->v(StringUtil::appendf("Translate X: ", container->m_translateX));
					ARK2D::getLog()->v(StringUtil::appendf("Translate Y: ", container->m_translateY));

					ARK2D::getLog()->v(StringUtil::appendf("Scale X: ", container->m_scaleX));
					ARK2D::getLog()->v(StringUtil::appendf("Scale Y: ", container->m_scaleY));

					//if (!GameContainerPlatform::s_gamePaused) { 
					container->disable2D();  
					//}
					container->enable2D();
  
					//container->m_platformSpecific.initGL("", width, height);
					//container->m_platformSpecific.initGL2D(width, height);
				}
				else
				{ 
 
				}


			#elif (defined(ARK2D_MACINTOSH) || defined(ARK2D_WINDOWS) || defined(ARK2D_FLASCC) || defined(ARK2D_UBUNTU_LINUX))
				
				if (container->isOrientationInverted()) {
					float scale = container->getScale();
					float translateX = (container->m_width - (container->m_originalHeight*scale)) / 2.0f;
					//translateX /= scale; // because translate happens after scale we have to divide it. 

					container->m_translateX = translateX;// float(container->m_height) * -1.0f;
					container->m_translateY = 0;

					ARK2D::getLog()->v("-------");
					ARK2D::getLog()->v(StringUtil::appendf("Normal Width: ", container->getWidth()));
					ARK2D::getLog()->v(StringUtil::appendf("Normal Height: ", container->getHeight()));

					ARK2D::getLog()->v(StringUtil::appendf("Dynamic Width: ", container->getDynamicWidth()));
					ARK2D::getLog()->v(StringUtil::appendf("Dynamic Height: ", container->getDynamicHeight()));

					ARK2D::getLog()->v(StringUtil::appendf("Screen Width: ", container->getScreenWidth()));
					ARK2D::getLog()->v(StringUtil::appendf("Screen Height: ", container->getScreenHeight()));

					ARK2D::getLog()->v(StringUtil::appendf("Translate X: ", container->m_translateX));
					ARK2D::getLog()->v(StringUtil::appendf("Translate Y: ", container->m_translateY));

					ARK2D::getLog()->v(StringUtil::appendf("Scale X: ", container->m_scaleX));
					ARK2D::getLog()->v(StringUtil::appendf("Scale Y: ", container->m_scaleY));

				} else {

					float tx = float(container->getDynamicWidth() - (float(container->getWidth())*container->getScale()))/2;
                	float ty = float(container->getDynamicHeight() - (float(container->getHeight())*container->getScale()))/2;
                	container->m_translateX = (int) tx;
                	container->m_translateY = (int) ty;
                }

                Renderer* r = ARK2D::getRenderer();
                r->loadIdentity();
                r->viewport(0, 0, width, height);
                container->enable2D();

                //glLoadIdentity();
				//glViewport(0, 0, width, height);
				//container->enable2D();

			#else
				/*if (container->getResizeBehaviour() == GameContainer::RESIZE_BEHAVIOUR_SCALE)
				{
					glLoadIdentity();
					glViewport(0, 0, width, height);

				}
				else if (container->getResizeBehaviour() == GameContainer::RESIZE_BEHAVIOUR_NOSCALE)
				{
					glLoadIdentity();
					glViewport(0, 0, width, height);

					glMatrixMode(GL_PROJECTION) ;
					glPushMatrix();
					glLoadIdentity();

					#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
						glOrthof(0, width, height, 0, -1, 1);
					#else
						glOrtho(0, width, height, 0, -1, 1);
					#endif

					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
					glLoadIdentity();
				}*/
			#endif
		}

		void Game::pause() 
		{

		}

		void Game::resume() 
		{

		}


		void Game::keyPressed(unsigned int key) {

		}
		void Game::keyReleased(unsigned int key) {

		}
		void Game::mouseMoved(int x, int y, int oldx, int oldy) {

		}

		void Game::orientationChanged(int orientation) {

		}

	}
}
