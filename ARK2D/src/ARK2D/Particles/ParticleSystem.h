/*
 * ParticleSystem.h
 *
 *  Created on: 16 Apr 2011
 *      Author: Ashley
 */

#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <vector>
#include <list>
#include <map>
using namespace std;

#include "../ARK2D_namespaces.h"

class GameTimer;

namespace ARK {
	namespace Particles {


		class ParticleEmitter;
		class ParticlePool;
		class Particle;

		/*!
		 * \brief Main Particle System class that contains the Emitters, Pools and Particles.
		 * \warning todo: Occasionally these break when starting.
		 *
		 * Adapted from Kevin Glass's Java implementation.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 * @author Kevin Glass
		 */
		class ParticleSystem {
			public:
				static const unsigned int BLEND_ADDITIVE = 1;
				static const unsigned int BLEND_COMBINE = 2;

				static const unsigned int DEFAULT_PARTICLES = 100;

				vector<ParticleEmitter*> removeMe;

				map<ParticleEmitter*, ParticlePool*> particlesByEmitter;

				// The maximum number of particles allows per emitter
				int maxParticlesPerEmitter;

				// The list of emitters producing and controlling particles
				vector<ParticleEmitter*> emitters;

				// The dummy particle to return should no more particles be available
				Particle* dummy;

				// The blending mode
				unsigned int blendingMode;

				// The number of particles in use
				int pCount;

				// True if we're going to use points to render the particles
				bool usePoints;

				// The x coordinate at which this system should be rendered
				float m_x;

				// The y coordinate at which this system should be rendered
				float m_y;

				// True if we should remove completed emitters
				bool removeCompletedEmitters;

				// The default image for the particles
				Image* sprite;

				// True if the particle system is visible
				bool m_visible;

				// The mask used to make the particle image background transparent if any
				Color* mask;

				ParticleSystem(Image* defaultSprite);

				void reset();

				bool isVisible();
				void setVisible(bool b);

				void setRemoveCompletedEmitters(bool b);

				void setUsePoints(bool b);
				bool isUsePoints();

				unsigned int getBlendingMode();
				void setBlendingMode(unsigned int i);

				unsigned int getEmitterCount();

				ParticleEmitter* getEmitter(unsigned int index);
				void addEmitter(ParticleEmitter* e);
				void removeEmitter(ParticleEmitter* e);
				void removeAllEmitters();
				float getX();
				float getY();
				void setPosition(float x, float y);

				void render();
				void render(float x, float y);

				void update(GameTimer* timer);
				void update(float delta);

				unsigned int getParticleCount();

				Particle* getNewParticle(ParticleEmitter* emitter, float life);

				void release(Particle* p);
				void releaseAll(ParticleEmitter* e);

				void moveAll(ParticleEmitter* emitter, float x, float y);

				void load(Image* defaultSprite, int numberOfParticles);

				virtual ~ParticleSystem();

				static Particle* createParticle(ParticleSystem* system);
		};
	}
}

#endif /* PARTICLESYSTEM_H_ */
