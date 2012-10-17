/*
 * ParticleEmitter.h
 *
 *  Created on: 16 Apr 2011
 *      Author: Ashley
 */

#ifndef PARTICLEEMITTER_H_
#define PARTICLEEMITTER_H_

#include "../Namespaces.h"

namespace ARK {
	namespace Particles {
		class ParticleSystem;
		class Particle;

		/*!
		 * \brief An abstract particle emitter.
		 *
		 * Adapted from Kevin Glass's Java implementation.
		 *
		 * @see ConfigurableEmitter
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 * @author Kevin Glass
		 */
		class ParticleEmitter {
			friend class ParticleSystem;
			public:
			virtual void update(ParticleSystem* system, float delta) = 0;
			virtual void updateParticle(Particle* particle, float delta) = 0;

			virtual bool isComplete() = 0;
			virtual void wrapUp() = 0;
			virtual bool isEnabled() = 0;
			virtual void setEnabled(bool b) = 0;
			virtual Image* getImage() = 0;
			virtual bool useAdditive() = 0;
			virtual bool isOriented() = 0;
			virtual bool usePoints() = 0;
			virtual void resetState() = 0;
            
            virtual ~ParticleEmitter() {};
		};
	}
}

#endif /* PARTICLEEMITTER_H_ */
