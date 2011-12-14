/*
 * Particle.h
 *
 *  Created on: 16 Apr 2011
 *      Author: Ashley
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "../ARK2D_GL.h"

class Color;
class Image;

class ParticleSystem;
class ParticleEmitter;
class ParticlePool;

class Particle
{
	friend class ParticleSystem;

	public:
		static const unsigned int INHERIT_POINTS = 1;
		static const unsigned int USE_POINTS = 2;
		static const unsigned int USE_QUADS = 3;

		float m_x;
		float m_y;

		float m_velx;
		float m_vely;

		float m_size;

		Color* m_color;

		float life;
		float originalLife;

		ParticleSystem* m_engine;
		ParticleEmitter* m_emitter;
		Image* image;
		unsigned int type;

		unsigned int usePoints;
		bool oriented;

		float scaleY;

	public:

		Particle(ParticleSystem* engine);

		ParticleEmitter* getEmitter();

		void init(ParticleEmitter* emitter, float life);
		void update(float delta);
		void render();

		float getX();
		float getY();
		void move(float x, float y);

		float getOriginalLife();
		float getLife();
		bool inUse();

		void setImage(Image* i);

		void setType(unsigned int t);
		unsigned int getType();

		void setUsePoint(unsigned int i);

		float getSize();
		void setSize(float size);
		void adjustSize(float pixels);

		void setLife(float l);
		void adjustLife(float delta);

		void kill();

		Color* getColor();
		void setColor(float r, float g, float b, float a);
		void setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
		void adjustColor(float r, float g, float b, float a);
		void adjustColor(signed int r, signed int g, signed int b, signed int a);

		void setPosition(float x, float y);
		void adjustPosition(float x, float y);

		void setVelocity(float dirx, float diry);
		void setVelocity(float dirx, float diry, float speed);
		void adjustVelocity(float dx, float dy);
		void setSpeed(float speed);

		void setOriented(bool o);
		bool isOriented();

		float getScaleY();
		void setScaleY(float y);

};

#endif /* PARTICLE_H_ */