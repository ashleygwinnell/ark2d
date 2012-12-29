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

namespace ARK {
	namespace Core {

		Game::Game(string title) {
			this->m_title = title;
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
		void Game::preRender(GameContainer* container, Renderer* timer) { }
		void Game::postRender(GameContainer* container, Renderer* timer) { }
		//void Game::render(GameContainer& container, const Graphics& g) = 0;
		//void Game::resize(GameContainer& container, int width, int height) = 0;
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
                    } else {
                        float tx = float(container->getDynamicHeight() - (float(container->getWidth())*container->getScale()))/2;
                        float ty = float(container->getDynamicWidth() - (float(container->getHeight())*container->getScale()))/2;
                        container->m_translateX = tx;
                        container->m_translateY = ty;
                    }
                //    container->m_platformSpecific.initOpenGL2D(width, height);
				}
			#elif (defined(ARK2D_MACINTOSH) || defined(ARK2D_WINDOWS) || defined(ARK2D_FLASCC))
				float tx = float(container->getDynamicWidth() - (float(container->getWidth())*container->getScale()))/2;
                float ty = float(container->getDynamicHeight() - (float(container->getHeight())*container->getScale()))/2;
                container->m_translateX = (int) tx;
                container->m_translateY = (int) ty;

                glLoadIdentity();
				glViewport(0, 0, width, height);

				container->enable2D();
			#else
				if (container->getResizeBehaviour() == GameContainer::RESIZE_BEHAVIOUR_SCALE)
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
				}
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

	}
}
