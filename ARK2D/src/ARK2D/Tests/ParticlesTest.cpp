/*
 * ParticlesTest.cpp
 *
 *  Created on: Mar 23, 2012
 *      Author: ashleygwinnell
 */

#include "ParticlesTest.h"
#include "../../ARK.h"

namespace ARK {
	namespace Tests {

		ParticlesTestGameState::ParticlesTestGameState(): GameState() {

		}
		void ParticlesTestGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { }
		void ParticlesTestGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }
		unsigned int ParticlesTestGameState::id() { return 0; }
		void ParticlesTestGameState::init(GameContainer* container, StateBasedGame* game) {

			// Create default image.
			defaultParticle = Resource::get("ark2d/particle.png")->asImage();

			// Create Particle System
			system = new ParticleSystem(defaultParticle);
			system->setBlendingMode(ParticleSystem::BLEND_ADDITIVE);
			system->setUsePoints(false);
			system->setRemoveCompletedEmitters(false);

			// Add Emitters.
			ConfigurableEmitter* emitter = new ConfigurableEmitter("Undefined");
			emitter->setUsePoints(Particle::INHERIT_POINTS);
			//emitter->setUseOriented(true);
			emitter->getSpawnInterval()->set(false, 100,100);
			emitter->getSpawnCount()->set(true, 1, 1);
			emitter->getInitialLife()->set(true, 4000,4000);
			emitter->getInitialSize()->set(true, 28,28);
			emitter->getInitialDistance()->set(false, 0,0);
			emitter->getOffsetX()->set(false, 0, 0);
			emitter->getOffsetY()->set(true, 0, 0);
			emitter->getSpeed()->set(true, 50, 70);
			emitter->getLength()->set(false, 1000, 1000);
			//emitter->getEmitCount()->set(10,10);

			emitter->setSpread(30.0f);
			emitter->setAngularOffset(0.0f);
			emitter->setGrowthFactor(9.0f);
			emitter->setGravityFactor(0.0f);
			emitter->setWindFactor(0.0f);
			emitter->setStartAlpha(255);
			emitter->setEndAlpha(0);
			//emitter->setAlpha(alpha);
			//emitter->setSize(size);
			//emitter->setVelocity(velocity);
			//emitter->setScaleY(scale_y);
			emitter->addColorPoint(0.0f, new Color(Color::cyan));

			Color* thisRed = new Color(Color::red); thisRed->setAlpha(0);
			emitter->addColorPoint(1.0f, thisRed);
			emitter->replay();
			system->addEmitter(emitter);

		//	system->setPosition(container->getWidth()/2, container->getHeight()/2);



		}
		void ParticlesTestGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
			system->update(timer);
		}
		void ParticlesTestGameState::render(GameContainer* container, StateBasedGame* game, Renderer* g) {
			g->drawString("Particles Test:", 10, 10);
			g->drawString(StringUtil::append("pcount ", system->pCount), 10, 30);
			g->drawString(StringUtil::append("ecount ", system->emitters.size()), 10, 50);

			g->setDrawColor(Color::white);
			system->setUsePoints(true);
			system->render(container->getWidth()/3, 2*container->getHeight()/3);

			//system->setUsePoints(false);
			//system->render(2*container->getWidth()/3, 2*container->getHeight()/3);

			defaultParticle->draw(0,0);
		}
		ParticlesTestGameState::~ParticlesTestGameState() {

		}


		ParticlesTest::ParticlesTest(): StateBasedGame("Particles Test") {

		}
		void ParticlesTest::initStates(GameContainer* container) {
			addState(new ParticlesTestGameState());
			enterState((unsigned int) 0);
		}
		void ParticlesTest::update(GameContainer* container, GameTimer* timer) {
			StateBasedGame::update(container, timer);
		}
		void ParticlesTest::render(GameContainer* container, Renderer* g) {
			StateBasedGame::render(container, g);
		}
		void ParticlesTest::resize(GameContainer* container, int width, int height) {
			StateBasedGame::resize(container, width, height);
		}
		int ParticlesTest::start() {
			ARK::Tests::ParticlesTest* test = new ARK::Tests::ParticlesTest();
			GameContainer container(*test, 800, 600, 32, false);
			container.setClearColor(Color::darker_grey);
			container.setShowingFPS(true);
			container.start();
			return 0;
		}
		ParticlesTest::~ParticlesTest() {

		}

	}
}
