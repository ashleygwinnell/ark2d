/*
 * ConfigurableEmitter.cpp
 *
 *  Created on: 16 Apr 2011
 *      Author: Ashley
 */

#include "ConfigurableEmitter.h"
#include "ParticleSystem.h"
#include "Particle.h"
#include "Range.h"

ConfigurableEmitter::ConfigurableEmitter(string name):
	ParticleEmitter(),
	m_spawnInterval(100, 100),
	m_spawnCount(5, 5),
	m_initialLife(1000, 1000),
	m_initialSize(10, 10),
	m_xOffset(0, 0),
	m_yOffset(0, 0),
	m_spread(360),
	m_angularOffset(0),
	m_initialDistance(0, 0),
	m_speed(50, 50),
	m_growthFactor(0),
	m_gravityFactor(0),
	m_windFactor(0),
	m_length(1000, 1000),
	m_colors(),
	m_startAlpha(255),
	m_endAlpha(0),
	m_alpha(NULL),
	m_size(NULL),
	m_velocity(NULL),
	m_scaleY(NULL),
	m_emitCount(1000, 1000),
	m_usePoints(Particle::INHERIT_POINTS),
	m_useOriented(false),
	m_useAdditive(false),
	m_name(name),
	m_image(NULL),
	m_updateImage(false),
	m_enabled(true),
	m_x(0),
	m_y(0),
	m_nextSpawn(0),
	m_timeout(0),
	m_engine(NULL),
	m_particleCount(0),
	m_leftToEmit(0),
	m_wrapUp(false),
	m_completed(false),
	m_adjust(false),
	m_adjustx(0),
	m_adjusty(0)
	{
	m_leftToEmit = m_emitCount.random();
	m_timeout = m_length.random();
	m_colors.push_back(new ColorRecord(0, new Color(Color::white))); // const_cast<Color*>(&Color::white)));
	m_colors.push_back(new ColorRecord(1, new Color(Color::red))); //const_cast<Color*>(&Color::red)));

	vector<Vector2<float>*> alpha;
	alpha.push_back(new Vector2<float>(0.0f, 0.0f));
	alpha.push_back(new Vector2<float>(1.0f, 255.0f));
	m_alpha = new LinearInterpolator(alpha, 0, 255);

	vector<Vector2<float>*> size;
	size.push_back(new Vector2<float>(0.0f, 0.0f));
	size.push_back(new Vector2<float>(1.0f, 255.0f));
	m_size = new LinearInterpolator(size, 0, 255);

	vector<Vector2<float>*> velocity;
	velocity.push_back(new Vector2<float>(0.0f, 0.0f));
	velocity.push_back(new Vector2<float>(1.0f, 1.0f));
	m_velocity = new LinearInterpolator(velocity, 0, 1);

	vector<Vector2<float>*> scaleY;
	scaleY.push_back(new Vector2<float>(0.0f, 0.0f));
	scaleY.push_back(new Vector2<float>(1.0f, 1.0f));
	m_scaleY = new LinearInterpolator(scaleY, 0, 1);

}

void ConfigurableEmitter::setPosition(float x, float y) {
	setPosition(x, y, true);
}
void ConfigurableEmitter::setPosition(float x, float y, bool moveParticles) {
	if (moveParticles) {
		m_adjust = true;
		m_adjustx -= m_x - x;
		m_adjusty -= m_y - y;
	}
	m_x = x;
	m_y = y;
}

float ConfigurableEmitter::getX() {
	return m_x;
}
float ConfigurableEmitter::getY() {
	return m_y;
}

void ConfigurableEmitter::setName(string name) {
	m_name = name;
}

void ConfigurableEmitter::setUsePoints(int p) {
	m_usePoints = p;
}

void ConfigurableEmitter::setUseOriented(bool b) {
	m_useOriented = b;
}

void ConfigurableEmitter::setUseAdditive(bool b) {
	m_useAdditive = b;
}

void ConfigurableEmitter::update(ParticleSystem* system, float delta) {
	m_engine = system;
	if (!m_adjust) {
		m_adjustx = 0;
		m_adjusty = 0;
	} else {
		m_adjust = false;
	}

	//if (m_updateImage) {
		//m_updateImage = false;
		//image = new Image(relativePath + imageName);
	//}

	if ((m_wrapUp)
			|| (m_length.isEnabled() && m_timeout < 0)
			|| (m_emitCount.isEnabled() && m_leftToEmit <= 0))
		{
		if (m_particleCount == 0) {
			m_completed = true;
		}
	}
	m_particleCount = 0;

	if (m_wrapUp) {
		return;
	}

	if (m_length.isEnabled()) {
		if (m_timeout < 0) {
			return;
		}
		m_timeout -= (int) (delta * 1000);
	}
	if (m_emitCount.isEnabled()) {
		if (m_leftToEmit <= 0) {
			return;
		}
	}



	m_nextSpawn -= int(delta * 1000);
	if (m_nextSpawn < 0) {
		m_nextSpawn = m_spawnInterval.random();
		int count = m_spawnCount.random();
		for (unsigned int i = 0; i < (unsigned int) count; i++) {
			Particle* p = m_engine->getNewParticle(this, m_initialLife.random());
			p->setSize(m_initialSize.random());
			p->setPosition(m_x + m_xOffset.random(), m_y + m_yOffset.random());
			p->setVelocity(0, 0, 0);

			float dist = m_initialDistance.random();
			float power = m_speed.random();
			if (dist != 0 || power != 0) {
				float s = m_spread.getValue(0);
				float ang = (s + m_angularOffset.getValue(0) - (m_spread.getValue() / 2)) - 90;
				float xa = (float) cos(MathUtil::toRadians(double(ang))) * dist;
				float ya = (float) sin(MathUtil::toRadians(double(ang))) * dist;
				p->adjustPosition(xa, ya);

				float xv = (float) cos(MathUtil::toRadians(double(ang)));
				float yv = (float) sin(MathUtil::toRadians(double(ang)));
				p->setVelocity(xv, yv, power * 0.001f);
			}

			if (m_image != NULL) {
				p->setImage(m_image);
			}

			//std::cout << "hereh1" << std::endl;

			ColorRecord* start = m_colors.at(0);
			p->setColor(start->col->getRed(), start->col->getGreen(), start->col->getBlue(), (unsigned int) m_startAlpha.getValue(0));
			p->setUsePoint(m_usePoints);
			p->setOriented(m_useOriented);

			//std::cout << "hereh2" << std::endl;

			if (m_emitCount.isEnabled()) {
				m_leftToEmit--;
				if (m_leftToEmit <= 0) {
					break;
				}
			}
			//std::cout << "hereh3" << std::endl;
		}
	}

}
void ConfigurableEmitter::updateParticle(Particle* particle, float delta) {
	m_particleCount++;

	//std::cout << "a" << std::endl;

	particle->m_x += m_adjustx;
	particle->m_y += m_adjusty;

	particle->adjustVelocity(
		m_windFactor.getValue(0) * 0.00005f * (delta*1000),
		m_gravityFactor.getValue(0) * 0.00005f * (delta*1000));

	float offset = particle->getLife() / particle->getOriginalLife();
	float inv = 1 - offset;
	float colOffset = 0;
	float colInv = 1;

	Color* startColor = NULL;
	Color* endColor = NULL;



	for (unsigned int i = 0; i < m_colors.size() - 1; i++) {
		ColorRecord* rec1 = m_colors.at(i);
		ColorRecord* rec2 = m_colors.at(i + 1);

		if ((inv >= rec1->pos) && (inv <= rec2->pos)) {
			startColor = rec1->col;
			endColor = rec2->col;

			float step = rec2->pos - rec1->pos;
			colOffset = inv - rec1->pos;
			colOffset /= step;
			colOffset = 1 - colOffset;
			colInv = 1 - colOffset;
		}
	}

	//std::cout << "b" << std::endl;

	if (startColor != NULL) {
		float r = ((startColor->getRed()/255.0f) * colOffset) + ((endColor->getRed()/255.0f) * colInv);
		float g = ((startColor->getGreen()/255.0f) * colOffset) + ((endColor->getGreen()/255.0f) * colInv);
		float b = ((startColor->getBlue()/255.0f) * colOffset) + ((endColor->getBlue()/255.0f) * colInv);

		float a;
		if (m_alpha->isActive()) {
			a = m_alpha->getValue(inv) / 255.0f;
		} else {
			a = ((m_startAlpha.getValue(0) / 255.0f) * offset)
					+ ((m_endAlpha.getValue(0) / 255.0f) * inv);
		}
		particle->setColor(r, g, b, a);
	}

	//std::cout << "c" << std::endl;

	if (m_size->isActive()) {
		float s = m_size->getValue(inv);
		particle->setSize(s);
	} else {
		particle->adjustSize((delta*1000) * m_growthFactor.getValue(0) * 0.001f);
	}

	if (m_velocity->isActive()) {
		particle->setSpeed(m_velocity->getValue(inv));
	}

	if (m_scaleY->isActive()) {
		particle->setScaleY(m_scaleY->getValue(inv));
	}
}

bool ConfigurableEmitter::isComplete() {
	if (m_engine == NULL) {
		return false;
	}

	if (m_length.isEnabled()) {
		if (m_timeout > 0) {
			return false;
		}
		return m_completed;
	}
	if (m_emitCount.isEnabled()) {
		if (m_leftToEmit > 0) {
			return false;
		}
		return m_completed;
	}

	if (m_wrapUp) {
		return m_completed;
	}

	return false;
}

void ConfigurableEmitter::replay() {
	reset();
	m_nextSpawn = 0;
	m_leftToEmit = m_emitCount.random();
	m_timeout = m_length.random();
}

void ConfigurableEmitter::reset() {
	m_completed = false;
	if (m_engine != NULL) {
		m_engine->releaseAll(this);
	}
}

void ConfigurableEmitter::wrapUp() {
	m_wrapUp = true;
}
bool ConfigurableEmitter::isEnabled() {
	return m_enabled;
}
void ConfigurableEmitter::setEnabled(bool b) {
	m_enabled = b;
}
Image* ConfigurableEmitter::getImage() {
	return m_image;
}
bool ConfigurableEmitter::useAdditive() {
	return m_useAdditive;
}
bool ConfigurableEmitter::isOriented() {
	return m_useOriented;
}
bool ConfigurableEmitter::usePoints() {
	return (m_usePoints == (signed int) Particle::INHERIT_POINTS) && (m_engine->isUsePoints()) ||
			   (m_usePoints == (signed int)Particle::USE_POINTS);
}
void ConfigurableEmitter::resetState() {
	m_wrapUp = false;
	replay();
}

void ConfigurableEmitter::addColorPoint(float pos, Color* col) {
	m_colors.push_back(new ColorRecord(pos, col));
}

ConfigurableEmitter::~ConfigurableEmitter() {

	vector<ColorRecord*>::iterator it = m_colors.begin();
	while (it != m_colors.end()) {
		delete (*it);
		it = m_colors.erase(it);
	}

	delete m_image;

	delete m_alpha;
	delete m_size;
	delete m_velocity;
	delete m_scaleY;
}
