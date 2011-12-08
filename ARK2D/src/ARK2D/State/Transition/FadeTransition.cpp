/*
 * FadeTransition.cpp
 *
 *  Created on: 22nd Nov 2010
 *      Author: Ashley
 */

#include "FadeTransition.h"
#include "../../ARK2D_GL.h"

FadeTransition::FadeTransition(unsigned int timeMillis):
	Transition(),
	m_time(timeMillis),
	m_time_current(0),
	m_alpha(1.0f)
	{

}
void FadeTransition::init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) {
	Transition::init(container, game, from, to);

	int width = container->getWidth();
	int height = container->getHeight();

	// Generate one texture (we're creating only one).
	unsigned Object(0);
	glGenTextures(1, &Object);

	void* data[width * height];
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &data);

	// bind?
	glBindTexture(GL_TEXTURE_2D, Object);

	// You can use these values to specify mipmaps if you want to, such as 'GL_LINEAR_MIPMAP_LINEAR'.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Create the actual texture object.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data);

	m_textureId = Object;

}
void FadeTransition::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	m_alpha = (float) Easing::ease(m_easing, double(m_time_current), 1.0, -1.0, double(m_time));
	m_time_current += int(timer->getDelta() * 1000.0f);
}
void FadeTransition::preRender(GameContainer* container, StateBasedGame* game, Graphics* g) {

}
void FadeTransition::postRender(GameContainer* container, StateBasedGame* game, Graphics* g) {

	glColor4f(1.0f, 1.0f, 1.0f, m_alpha);

	int m_Width = container->getWidth();
	int m_Height = container->getHeight();

	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glPushMatrix();
	glBegin( GL_QUADS );
		glTexCoord2f( 0, 1.0f);
		glVertex2f(0, 0);

		glTexCoord2f( 0, 0 );
		glVertex2f(0, m_Height);

		glTexCoord2f( 1.0f, 0.0f );
		glVertex2f(m_Width, m_Height);

		glTexCoord2f(1.0f, 1.0f );
		glVertex2f(m_Width, 0);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);

}
bool FadeTransition::isComplete() {
	if (m_time_current >= m_time) {
		return true;
	}
	return false;
}

FadeTransition::~FadeTransition() {

}
