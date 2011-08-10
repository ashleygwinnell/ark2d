/*
 * GameContainerMac.cpp
 *
 *  Created on: Aug 9, 2011
 *      Author: ashleygwinnell
 */

#include "GameContainer.h"
#include "GameContainerMac.h"

#ifdef ARK2D_MACINTOSH

	void* GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
		return new string("");
	}


	GameContainer::GameContainer(Game& g, int width, int height, int bpp, bool fullscreen):
		m_timer(),
		m_game(g),
		m_input(),
		m_graphics(),
		m_gamepads(),
		m_width(width),
		m_height(height),
		m_bpp(bpp),
		m_fullscreen(fullscreen),
		m_resizable(false),
		m_scaleToWindow(true),
		m_clearColor(Color::black),
		m_platformSpecific()
		{

	}

	void GameContainer::setFullscreen(bool fullscreen) {

	}

	void GameContainer::processGamepadInput() {

	}

	void GameContainer::setIcon(const std::string& path) {
		//m_platformSpecific.m_iconpath = path;
	}

	void GameContainer::initGamepads() {

	}

	void GameContainer::setCursorVisible(bool b) {

	}

	void GameContainer::resizeWindowToFitViewport() {

	}

	void GameContainer::start() {
		std::cout << "fill me too!" << std::endl;
	}

	void GameContainer::close() const {

	}


	void GameContainer::swapBuffers() {

	}

#endif
