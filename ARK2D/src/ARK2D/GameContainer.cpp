/*
 * GameContainer.cpp
 *
 *  Created on: 15-Oct-2009
 *      Author: Ashley Gwinnell
 */

#include "GigaRectangle.h"
#include "GameContainer.h"


#if defined(_WIN32) || defined(ARK2D_WINDOWS)
	#include "GameContainerWindows.h"
#endif

#if defined(__linux__) || defined(ARK2D_UBUNTU_LINUX)
	#include "GameContainerLinux.h"
#endif

#if defined(ARK2D_MACINTOSH)
	#include "GameContainerMac.h"
#endif

bool GameContainer::isVerbose() {
	return m_verbose;
}
void GameContainer::setVerbose(bool v) {
	m_verbose = v;
}

bool GameContainer::isFullscreen() {
	return m_fullscreen;
}

const GigaRectangle<int>& GameContainer::getWindowRectangle() const {
	return *m_window_rectangle;
}
const Game& GameContainer::getGame() const {
	return m_game;
}
const Input& GameContainer::getInput() const {
	return m_input;
}
const Graphics* GameContainer::getGraphics() const {
	return &m_graphics;
}
unsigned int GameContainer::getFPS() {
	return m_timer.getFPS();
}
unsigned int GameContainer::getWidth() const {
	return m_width;
}
unsigned int GameContainer::getHeight() const {
	return m_height;
}


void GameContainer::enable2D() {
	glMatrixMode(GL_PROJECTION) ;
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, m_width, m_height, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void GameContainer::disable2D() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GameContainer::enableOpenAL() {
	#ifdef ARK2D_WINDOWS
		alutInit(0, NULL);
		alGetError();
	#endif
}
void GameContainer::disableOpenAL() {

}

void GameContainer::resize(int w, int h) {
	this->m_width = w;
	this->m_height = h;
}




void GameContainer::deinitGamepads() {

}
vector<Gamepad*> GameContainer::getGamepads() const {
	return m_gamepads;
}

void GameContainer::setClearColor(const Color& color) {
	m_clearColor = color;
}

GameTimer* GameContainer::getTimer() {
	return &m_timer;
}

GameContainer::~GameContainer() {

}
