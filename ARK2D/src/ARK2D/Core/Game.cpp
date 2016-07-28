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
#include "../Controls/Gamepad.h"

#ifdef ARK2D_ANDROID
#include "Platform/GameContainerAndroid.h"
#endif

namespace ARK {
	namespace Core {

		Game::Game(string title):
			SceneNode("game"),
			m_container(NULL),
			m_title(title),
			m_timeline(NULL),
			scene(NULL),
			bounds(0,0,0)
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

		void Game::init(GameContainer* container) {
			scene = container->scene;
			bounds.setSize(container->m_originalWidth, container->m_originalHeight, 0);
		}

		void Game::preUpdate(GameContainer* container, GameTimer* timer) { }
		void Game::postUpdate(GameContainer* container, GameTimer* timer) { }
		void Game::update(GameContainer* container, GameTimer* timer) {
			m_timeline->update(timer->getDelta());
		}

		void Game::preRender(GameContainer* container, Renderer* r) {
			preRender();
		}

		void Game::postRender(GameContainer* container, Renderer* r) {
			postRender();
		}

		void Game::onResize() {
			GameContainer* c = ARK2D::getContainer();
			resize(c, c->getDynamicWidth(), c->getDynamicHeight());
		}

		void Game::update() {
			update(ARK2D::getContainer(), ARK2D::getTimer());
		}
		void Game::render() {
			if (!visible) { return; }
			render(ARK2D::getContainer(), ARK2D::getRenderer());
			renderChildren();
			if (ARK2D::getLog()->isVisible()) {
				renderBounds();
			}
		}
		void Game::preRender() {
			if (!visible) { return; }
			SceneNode::preRender();
		}
		void Game::postRender() {
			if (!visible) { return; }
			SceneNode::postRender();
		}

		void Game::resize(GameContainer* container, int width, int height) {
			// renderBounds();
			//ARK2D::getLog()->resize
		}

		void Game::pause()
		{

		}

		void Game::resume()
		{

		}


		// key listener
		bool Game::keyPressed(unsigned int key) {
			return false;
		}
		bool Game::keyReleased(unsigned int key) {
			return false;
		}
		// mouse listener
		bool Game::mouseMoved(int x, int y, int oldx, int oldy) {
			return false;
		}

		// gamepad listener
		void Game::gamepadConnected(ARK::Controls::Gamepad* gamepad) { }
		void Game::gamepadDisconnected(ARK::Controls::Gamepad* gamepad) { }
		void Game::buttonPressed(ARK::Controls::Gamepad* gamepad, unsigned int button) { }
		void Game::buttonReleased(ARK::Controls::Gamepad* gamepad, unsigned int button) { }
		void Game::axisMoved(ARK::Controls::Gamepad* gamepad, unsigned int axis, float value) { }

		void Game::orientationChanged(int orientation) {

		}

		ARK::Geometry::Cube* Game::getBounds() {
			return &bounds;
		}

	}
}
