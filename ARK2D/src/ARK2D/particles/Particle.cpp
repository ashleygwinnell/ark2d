/*
 * Particle.cpp
 *
 *  Created on: 16 Apr 2011
 *      Author: Ashley
 */


#include "../ARK2D_GL.h"
#include "../Color.h"
#include "../Image/Image.h"
#include "../Graphics.h"
#include "../ARK2D.h"

#include "Particle.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "ParticlePool.h"

Particle::Particle(ParticleSystem* engine):
	m_x(0),
	m_y(0),
	m_velx(0),
	m_vely(0),
	m_size(10),
	m_color(NULL),
	life(0),
	originalLife(0),
	m_engine(NULL),
	m_emitter(NULL),
	image(NULL),
	type(0),
	usePoints(INHERIT_POINTS),
	oriented(false),
	scaleY(1.0f)
{
	m_engine = engine;

	m_color = new Color(Color::white); //
}

float Particle::getX() {
	return m_x;
}

float Particle::getY() {
	return m_y;
}

void Particle::move(float x, float y) {
	m_x += x;
	m_y += y;
}

float Particle::getSize() {
	return m_size;
}

Color* Particle::getColor() {
	return m_color;
}

void Particle::setImage(Image* i) {
	image = i;
}

float Particle::getOriginalLife() {
	return originalLife;
}
float Particle::getLife() {
	return life;
}
bool Particle::inUse() {
	return (life > 0);
}

void Particle::render() {
	Graphics* g = ARK2D::getGraphics();
	if ((m_engine->isUsePoints() && usePoints == INHERIT_POINTS) || usePoints == USE_POINTS) {
		// TODO: bind no texture (-1)
		glBegin(GL_POINT_SMOOTH);
		glPointSize(m_size / 2);
		m_color->bind();
		g->drawPoint(m_x, m_y);
	} else if (oriented || scaleY != 1.0f) {
		g->pushMatrix();

		g->translate(m_x, m_y);
		//if (oriented) {
			//float angle = (float) (Math.atan2(y, x) * 180 / Math.PI);
			//GL.glRotatef(angle, 0f, 0f, 1.0f);
		//}
		g->scale(1.0f, scaleY);

		image->setColor(m_color);
		image->draw((m_size/2) * -1, (m_size/2) * -1, (unsigned int) m_size, (unsigned int) m_size);

		g->popMatrix();
	} else {
		m_color->bind();
		image->setColor(m_color);
		image->draw(m_x - (m_size/2), m_y - (m_size/2), (unsigned int) m_size, (unsigned int) m_size);
	}
}

void Particle::update(float delta) {
	//std::cout << "7" << std::endl;
	if (m_emitter == NULL) {
		std::cout << "blah" << std::endl;
		return;
	}

	m_emitter->updateParticle(this, delta);
	//std::cout << "8" << std::endl;
	life -= (int) (delta * 1000);
	if (life > 0) {
		m_x += (delta * 1000) * m_velx;
		m_y += (delta * 1000) * m_vely;
	} else {
		m_engine->release(this);
	}
}

void Particle::init(ParticleEmitter* emitter, float life) {
	m_x = 0;
	m_y = 0;
	m_emitter = emitter;
	m_velx = 0;
	m_vely = 0;
	m_size = 10;
	type = 0;
	originalLife = life;
	this->life = life;
	oriented = false;
	scaleY = 1.0f;
}

void Particle::setType(unsigned int t) {
	type = t;
}
void Particle::setUsePoint(unsigned int i) {
	usePoints = i;
}
unsigned int Particle::getType() {
	return type;
}
void Particle::setSize(float size) {
	m_size = size;
}
void Particle::adjustSize(float pixels) {
	m_size += pixels;
	//m_size = MathUtil::max(0, m_size);
}
void Particle::setLife(float l) {
	life = l;
}
void Particle::adjustLife(float delta) {
	life += (delta * 1000);
}
void Particle::kill() {
	life = 1;
}
void Particle::setColor(float r, float g, float b, float a) {
	m_color->setRed((unsigned int) (r * 255));
	m_color->setGreen((unsigned int) (g * 255));
	m_color->setBlue((unsigned int) (b * 255));
	m_color->setAlpha((unsigned int) (a * 255));
}
void Particle::setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
	m_color->setRed(r);
	m_color->setGreen(g);
	m_color->setBlue(b);
	m_color->setAlpha(a);
}
void Particle::setPosition(float x, float y) {
	m_x = x;
	m_y = y;
}
void Particle::setVelocity(float dirx, float diry) {
	setVelocity(dirx, diry, 1.0f);
}
void Particle::setVelocity(float dirx, float diry, float speed) {
	m_velx = dirx * speed;
	m_vely = diry * speed;
}
void Particle::setSpeed(float speed) {
	float currentSpeed = (float) sqrt((m_velx*m_velx) + (m_vely*m_vely));
	m_velx *= speed;
	m_vely *= speed;
	m_velx /= currentSpeed;
	m_vely /= currentSpeed;
}
void Particle::adjustPosition(float x, float y) {
	m_x += x;
	m_y += y;
}
void Particle::adjustColor(float r, float g, float b, float a) {
	m_color->setRed(m_color->getRed() + int(r * 255.0f));
	m_color->setGreen(m_color->getGreen() + int(g * 255.0f));
	m_color->setBlue(m_color->getBlue() + int(b * 255.0f));
	m_color->setAlpha(m_color->getAlpha() + int(a * 255.0f));
}
void Particle::adjustColor(signed int r, signed int g, signed int b, signed int a) {
	m_color->setRed(m_color->getRed() + r);
	m_color->setGreen(m_color->getGreen() + g);
	m_color->setBlue(m_color->getBlue() + b);
	m_color->setAlpha(m_color->getAlpha() + a);
}
void Particle::adjustVelocity(float dx, float dy) {
	m_velx += dx;
	m_vely += dy;
}
ParticleEmitter* Particle::getEmitter() {
	return m_emitter;
}
void Particle::setOriented(bool o) {
	oriented = o;
}
bool Particle::isOriented() {
	return oriented;
}
float Particle::getScaleY() {
	return scaleY;
}
void Particle::setScaleY(float y) {
	scaleY = y;
}
