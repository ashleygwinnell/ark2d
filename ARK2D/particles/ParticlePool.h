/*
 * ParticlePool.h
 *
 *  Created on: 16 Apr 2011
 *      Author: Ashley
 */

#ifndef PARTICLEPOOL_H_
#define PARTICLEPOOL_H_

class Particle;
class ParticleSystem;

#include <vector>
using namespace std;

class ParticlePool {
	public:
		vector<Particle*> particlesActive;
		vector<Particle*> particlesInactive;

		ParticlePool(ParticleSystem* system, int particles);
		~ParticlePool();

		void reset(ParticleSystem* system);
};

#endif /* PARTICLEPOOL_H_ */
