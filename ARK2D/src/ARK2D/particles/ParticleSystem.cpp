/*
 * ParticleSystem.cpp
 *
 *  Created on: 16 Apr 2011
 *      Author: Ashley
 */

#include "../ARK2D.h"
#include "../ARK2D_GL.h"
#include "../GameTimer.h"
#include "../Graphics.h"
#include "../Image.h"
#include "../Color.h"
#include "../OutputWrapper.h"

#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "ParticlePool.h"
#include "Particle.h"

ParticleSystem::ParticleSystem(Image* defaultSprite):
	removeMe(),
	particlesByEmitter(),
	maxParticlesPerEmitter(DEFAULT_PARTICLES),
	emitters(),
	dummy(NULL),
	blendingMode(ParticleSystem::BLEND_COMBINE),
	pCount(0),
	usePoints(false),
	m_x(0),
	m_y(0),
	removeCompletedEmitters(true),
	sprite(NULL),
	m_visible(true),
	mask()
	{
	load(defaultSprite, DEFAULT_PARTICLES);
}

void ParticleSystem::reset() {
	map<ParticleEmitter*, ParticlePool*>::iterator it = particlesByEmitter.begin();
	while (it != particlesByEmitter.end()) {
		ParticlePool* pool = it->second;
		pool->reset(this);
		it++;
	}

	for(unsigned int i = 0; i < emitters.size(); i++) {
		ParticleEmitter* emitter = emitters.at(i);
		emitter->resetState();
	}

}

bool ParticleSystem::isVisible() {
	return m_visible;
}
void ParticleSystem::setVisible(bool b) {
	m_visible = b;
}

void ParticleSystem::setRemoveCompletedEmitters(bool b) {
	removeCompletedEmitters = b;
}

void ParticleSystem::setUsePoints(bool b) {
	usePoints = b;
}
bool ParticleSystem::isUsePoints() {
	return usePoints;
}

unsigned int ParticleSystem::getBlendingMode() {
	return blendingMode;
}
void ParticleSystem::setBlendingMode(unsigned int i) {
	blendingMode = i;
}

unsigned int ParticleSystem::getEmitterCount() {
	return emitters.size();
}

ParticleEmitter* ParticleSystem::getEmitter(unsigned int index) {
	return emitters.at(index);
}
void ParticleSystem::addEmitter(ParticleEmitter* e) {
	emitters.push_back(e);
	ParticlePool* pool = new ParticlePool(this, maxParticlesPerEmitter);
	particlesByEmitter[e] = pool;
}
void ParticleSystem::removeEmitter(ParticleEmitter* e) {
	vector<ParticleEmitter*>::iterator it = emitters.begin();
	while(it != emitters.end()) {
		ParticleEmitter* e2 = (*it);
		if (e == e2) {
			it = emitters.erase(it);
		}
		it++;
	}
	particlesByEmitter.erase(e);
}

void ParticleSystem::removeAllEmitters() {
	for(unsigned int i = 0; i < emitters.size(); i++) {
		removeEmitter(emitters.at(i));
		i--;
	}
}
float ParticleSystem::getX() {
	return m_x;
}
float ParticleSystem::getY() {
	return m_y;
}
void ParticleSystem::setPosition(float x, float y) {
	m_x = x;
	m_y = y;
}

void ParticleSystem::render() {
	render(m_x, m_y);
}
void ParticleSystem::render(float x, float y) {
	if (sprite == NULL) {
		std::cout << "sprite was null" << std::endl; return;
	}
	if (!m_visible) {
		std::cout << "invisible" << std::endl;
		return;
	}

	Graphics* g = ARK2D::getGraphics();

	g->translate(x, y);
	if (blendingMode == BLEND_ADDITIVE) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	if (isUsePoints()) {
		glEnable(GL_POINT_SMOOTH);
	}
	for(unsigned int i = 0; i < emitters.size(); i++) {
		ParticleEmitter* emitter = emitters.at(i);
		if (!emitter->isEnabled()) {
			continue;
		}
		//std::cout << "render" << std::endl;
		// check for additive override and enable when set
		if (emitter->useAdditive()) {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		}

		// now get the particle pool for this emitter and render all particles that are in use
		ParticlePool* pool = particlesByEmitter.find(emitter)->second;
		Image* image = emitter->getImage();
		if (image == NULL) {
			//image = sprite;
			//emitter->u
			sprite->bind();
		}

		//if (!emitter->isOriented() && !emitter->usePoints(this)) {
		//	image->startUse();
		//}

		for (unsigned int i = 0; i < pool->particlesActive.size(); i++) {
			if (pool->particlesActive.at(i)->inUse()) {
				pool->particlesActive.at(i)->render();
			}
		}

		//if (!emitter->isOriented() && !emitter->usePoints(this)) {
		//	image->endUse();
		//}

		// reset additive blend mode
		if (emitter->useAdditive()) {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		//std::cout << "render2" << std::endl;
	}
	if (isUsePoints()) {
		glDisable(GL_POINT_SMOOTH);
	}

	if (blendingMode == BLEND_ADDITIVE) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	g->setDrawColor(Color::white);
	g->translate(x * -1, y * -1);
}


void ParticleSystem::update(GameTimer* timer) {
	update(timer->getDelta());
}
void ParticleSystem::update(float delta) {
	if (sprite == NULL) {
		std::cout << "sprite was null" << std::endl; return;
		return;
	}
	removeMe.clear();

	//std::cout << "1" << std::endl;


	for(unsigned int i = 0; i < emitters.size(); i++) {
		ParticleEmitter* emitter = emitters.at(i);
		if (emitter->isEnabled()) {
			//std::cout << "en" << std::endl;
			emitter->update(this, delta);
			//std::cout << "en2" << std::endl;
			if (removeCompletedEmitters) {
				if (emitter->isComplete()) {
					removeMe.push_back(emitter);
					particlesByEmitter.erase(emitter);
				}
			}
		}
	}

	//std::cout << "2" << std::endl;

	// remove items in removeMe from emitters.
	vector<ParticleEmitter*>::iterator ito = emitters.begin();
	while (ito != emitters.end()) {
		ParticleEmitter* e = (*ito);

		vector<ParticleEmitter*>::iterator ito2 = removeMe.begin();
		while (ito2 != removeMe.end()) {
			ParticleEmitter* e2 = (*ito2);

			if (e == e2) {
				ito = emitters.erase(ito);
				continue;
			}

			ito2++;
		}

		ito++;
	}

	//std::cout << "3" << std::endl;

	pCount = 0;

	if (!particlesByEmitter.empty()) {
		//std::cout << "4: " << particlesByEmitter.size() << std::endl;
		map<ParticleEmitter*, ParticlePool*>::iterator it = particlesByEmitter.begin();
		while (it != particlesByEmitter.end()) {
			ParticleEmitter* emitter = it->first;
			if (emitter->isEnabled()) {
				ParticlePool* pool = it->second;
				//std::cout << "5" << std::endl;
				for (unsigned int i = 0; i < pool->particlesActive.size(); i++) {
					if (pool->particlesActive.at(i)->getLife() > 0) {
						pool->particlesActive.at(i)->update(delta);
						pCount++;
					}
				}
			}
			it++;
			//std::cout << "6" << std::endl;
		}
	}
}

unsigned int ParticleSystem::getParticleCount() {
	return pCount;
}

Particle* ParticleSystem::getNewParticle(ParticleEmitter* emitter, float life) {
	ParticlePool* pool = particlesByEmitter.find(emitter)->second;
	if (pool->particlesInactive.size() > 0) {
		Particle* p = pool->particlesInactive.back();
		pool->particlesInactive.pop_back();
		p->init(emitter, life);
		p->setImage(sprite);
		return p;
	}

	OutputWrapper::println("Ran out of particles (increase the limit)!");
	if (dummy->m_emitter == NULL) {
		dummy->init(emitter, life);
		dummy->setImage(sprite);
	}
	return dummy;
}

void ParticleSystem::release(Particle* p) {
	if (p != dummy) {
		ParticlePool* pool = particlesByEmitter.find(p->getEmitter())->second;
		pool->particlesInactive.push_back(p);
	}
}
void ParticleSystem::releaseAll(ParticleEmitter* e) {
	if (!particlesByEmitter.empty()) {

		map<ParticleEmitter*, ParticlePool*>::const_iterator it = particlesByEmitter.begin();
		while (it != particlesByEmitter.end()) {
			ParticlePool* pool = it->second;
			for(unsigned int i = 0; i < pool->particlesActive.size(); i++) {
				if (pool->particlesActive.at(i)->inUse()) {
					if (pool->particlesActive.at(i)->getEmitter() == e) {
						pool->particlesActive.at(i)->setLife(-1);
						release(pool->particlesActive.at(i));
						//i--;
					}
				}
			}
			it++;
		}
	}
}

void ParticleSystem::moveAll(ParticleEmitter* emitter, float x, float y) {
	ParticlePool* pool = particlesByEmitter.find(emitter)->second;
	for (unsigned int i = 0; i < pool->particlesActive.size(); i++) {
		if (pool->particlesActive.at(i)->inUse()) {
			pool->particlesActive.at(i)->move(x, y);
		}
	}
}

void ParticleSystem::load(Image* defaultSprite, int numberOfParticles) {
	sprite = defaultSprite;
	maxParticlesPerEmitter = numberOfParticles;
	dummy = createParticle(this);
}

ParticleSystem::~ParticleSystem() {
	vector<ParticleEmitter*>::iterator rme = emitters.begin();
	while (rme != emitters.end()) {
		removeMe.push_back(*rme);
	}

	vector<ParticleEmitter*>::iterator rm = removeMe.begin();
	while (rm != removeMe.end()) {
		delete (*rm);
		rm = removeMe.erase(rm);
	}

	delete dummy;
	delete sprite;
	delete mask;
}

Particle* ParticleSystem::createParticle(ParticleSystem* system) {
	return new Particle(system);
}
