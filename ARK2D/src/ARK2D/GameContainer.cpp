/*
 * GameContainer.cpp
 *
 *  Created on: 15-Oct-2009
 *      Author: Ashley Gwinnell
 */

#include "Geometry/GigaRectangle.h"
#include "GameContainer.h"
#include "ARK2D.h"

#if defined(ARK2D_ANDROID)
	#include "GameContainerAndroid.h"
#elif defined(ARK2D_WINDOWS)
	#include "GameContainerWindows.h"
#elif defined(ARK2D_UBUNTU_LINUX)
	#include "GameContainerLinux.h"
#elif defined(ARK2D_MACINTOSH)
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
Graphics* GameContainer::getGraphics() const {
	return const_cast<Graphics*>(&m_graphics);
}
unsigned int GameContainer::getFPS() {
	return m_timer.getFPS();
}
unsigned int GameContainer::getDynamicWidth() const {
	return m_width;
}
unsigned int GameContainer::getDynamicHeight() const { // this changes on window resize/scaling.
	return m_height;
}

unsigned int GameContainer::getWidth() const { // this is fixed at the static/initial/defined size
	return m_originalWidth;
}
unsigned int GameContainer::getHeight() const {
	return m_originalHeight;
}

unsigned int GameContainer::getScreenWidth() const {
	return m_screenWidth;
}
unsigned int GameContainer::getScreenHeight() const {
	return m_screenHeight;
}

float GameContainer::getScale() const {
	return m_scale;
}
float GameContainer::getScaleX() const {
	return m_scaleX;
}
float GameContainer::getScaleY() const {
	return m_scaleY;
}

float GameContainer::getTranslateX() const {
	return m_translateX;
}
float GameContainer::getTranslateY() const {
	return m_translateY;
}

void GameContainer::setTitle(const std::string title) {
	this->m_strTitle = title;
	m_platformSpecific.setTitle(title);
}

void GameContainer::enable2D() {
	#if defined(ARK2D_ANDROID)

	#else
		glMatrixMode(GL_PROJECTION) ;
		glPushMatrix();
		glLoadIdentity();

		glOrtho(0, m_width, m_height, 0, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	#endif
}

void GameContainer::disable2D() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GameContainer::enableOpenAL() {
	m_platformSpecific.initOpenAL();
}
void GameContainer::disableOpenAL() {
	m_platformSpecific.deinitOpenAL();
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
const Color& GameContainer::getClearColor() {
	return m_clearColor;
}

GameTimer* GameContainer::getTimer() {
	return &m_timer;
}

void GameContainer::setResizable(bool b){
	m_resizable = b;
}
bool GameContainer::isResizable() {
	return m_resizable;
}
void GameContainer::setScaleToWindow(bool b) {
	m_scaleToWindow = b;
}

string GameContainer::getResourcePath() const {
	return m_platformSpecific.getResourcePath();
}

int GameContainer::getResizeBehaviour() {
	return m_resizeBehaviour;
}
void GameContainer::setResizeBehaviour(int b) {
	m_resizeBehaviour = b;
}

bool GameContainer::isTouchMode() {
	return m_touchMode;
}
void GameContainer::setTouchMode(bool tm) {
	m_touchMode = tm;
}

void GameContainer::saveScreenshot(string filename) {
	void* data[m_width * m_height];
	glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, &data);

	PNGImage::saveFile(filename, (char*) data, m_width, m_height);
}

GameContainer::~GameContainer() {

}
