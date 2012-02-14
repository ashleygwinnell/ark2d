/*
 * GameContainerAndroid.cpp
 *
 *  Created on: Feb 13, 2012
 *      Author: ashleygwinnell
 */

#include "GameContainerAndroid.h"
#include "GameContainer.h"
#include "ARK2D.h"


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
	m_resizeBehaviour(RESIZE_BEHAVIOUR_SCALE),
	m_platformSpecific()
{
	m_platformSpecific.m_container = this;

	m_input.setGameContainer(this);

	ARK2D::s_container = this;
	ARK2D::s_game = &m_game;
	ARK2D::s_graphics = &m_graphics;
	ARK2D::s_input = &m_input;
	ARK2D::s_log = new ARKLog();

}

void GameContainer::setSize(int width, int height) {

}

void GameContainer::setFullscreen(bool fullscreen) {

}

int GameContainer::getGlobalMouseX() const {
	return 0;
}
int GameContainer::getGlobalMouseY() const {
	return 0;
}

void GameContainer::start() {

}

void GameContainer::close() const {

}

GameContainerPlatform::GameContainerPlatform():
	m_container(NULL)
	{

}

void GameContainerPlatform::setTitle(string t) {

}

bool GameContainerPlatform::initOpenAL() {
	return false;
}

bool GameContainerPlatform::deinitOpenAL() {
	return true;
}

string GameContainerPlatform::getResourcePath() const {
	return "";
}

void* GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
	return NULL;
}

GameContainerPlatform::~GameContainerPlatform() {

}
