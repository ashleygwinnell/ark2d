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
	m_touchMode(true),
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
	m_screenWidth = width;
	m_screenHeight = height;

    if (m_resizeBehaviour == RESIZE_BEHAVIOUR_SCALE) {
    	m_scaleX = (float) width / (float) m_originalWidth;
    	m_scaleY = (float) height / (float) m_originalHeight;
    	if (m_scaleX > m_scaleY) {
    		m_scale = m_scaleY;
    		m_scaleX = m_scaleY;
    		m_scaleY = 1.0f;
    	} else { // y > x
    		m_scale = m_scaleX;
    		m_scaleY = m_scaleX;
    		m_scaleX = 1.0f;
    	}

    	//m_width = m_originalWidth * m_scaleX;
    	//m_height = m_originalHeight * m_scaleY;
    	m_width = width;
    	m_height = height;
   		ARK2D::s_game->resize(this, width, height);
    } else if (m_resizeBehaviour == RESIZE_BEHAVIOUR_NOSCALE) {
    	m_width = width;
    	m_height = height;
    	ARK2D::s_game->resize(this, width, height);
    }
}

void GameContainer::setFullscreen(bool fullscreen) {

}

int GameContainer::getGlobalMouseX() const {
	return 0;
}
int GameContainer::getGlobalMouseY() const {
	return 0;
}
void GameContainer::setIcon(const std::string& path){

}
void GameContainer::setCursorVisible(bool b) {

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
void GameContainerPlatform::initGL(string clearColorStr, int w, int h) {
	ARK2D::getLog()->i("init opengl");

	Color* clearColor;
	if (clearColorStr.size() > 0) {
		Color cc(clearColorStr);
		m_container->m_clearColor = cc;
		clearColor = &cc;
	} else {
		clearColor = &m_container->m_clearColor;
	}

	glViewport(0, 0, w, h);
	glClearColor(clearColor->getRedf(),clearColor->getGreenf(), clearColor->getBluef(), clearColor->getAlphaf());
	glClear( GL_COLOR_BUFFER_BIT );

	glShadeModel(GL_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

}
void GameContainerPlatform::initGL2D(int w, int h) {
	//enable 2d?
	glMatrixMode(GL_PROJECTION) ;
	glPushMatrix();
	glLoadIdentity();

	glOrthof(0, w, h, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

bool GameContainerPlatform::initOpenAL() {
	return false;
}

bool GameContainerPlatform::deinitOpenAL() {
	return true;
}

string GameContainerPlatform::getResourcePath() const {
	return "assets/";
}

void* GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
	return NULL;
}

GameContainerPlatform::~GameContainerPlatform() {

}
