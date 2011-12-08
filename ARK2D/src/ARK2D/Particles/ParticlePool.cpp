/*
 * ParticlePool.cpp
 *
 *  Created on: 16 Apr 2011
 *      Author: Ashley
 */

#include "ParticlePool.h"
#include "Particle.h"
#include "ParticleSystem.h"

ParticlePool::ParticlePool(ParticleSystem* system, int particles):
	particlesActive(),
	particlesInactive()
	{
	for (int i = 0; i < particles; i++) {
		particlesActive.push_back( ParticleSystem::createParticle(system) );
	}
	reset(system);
}

void ParticlePool::reset(ParticleSystem* system) {
	//particlesInactive.clear();
	for (unsigned int i = 0; i < particlesActive.size(); i++) {
		particlesInactive.push_back(particlesActive[i]);
	}
}

ParticlePool::~ParticlePool() {

}
