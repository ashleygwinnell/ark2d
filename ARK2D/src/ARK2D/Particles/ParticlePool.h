/*
 * ParticlePool.h
 *
 *  Created on: 16 Apr 2011
 *      Author: Ashley
 */

#ifndef PARTICLEPOOL_H_
#define PARTICLEPOOL_H_



#include <vector>
using namespace std;

namespace ARK {
	namespace Particles {

		class Particle;
		class ParticleSystem;

		/*!
		 * \brief Used internally by the particle engine.
		 * todo: Replace vectors with lists because items are very often removed from the middle.
		 *
		 * Adapted from Kevin Glass's Java implementation.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 * @author Kevin Glass
		 */
		class ParticlePool {
			public:
				vector<Particle*> particlesActive;
				vector<Particle*> particlesInactive;

				ParticlePool(ParticleSystem* system, int particles);
				~ParticlePool();

				void reset(ParticleSystem* system);
		};
	}
}

#endif /* PARTICLEPOOL_H_ */
