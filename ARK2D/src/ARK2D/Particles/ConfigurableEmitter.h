/*
 * ConfigurableEmitter.h
 *
 *  Created on: 16 Apr 2011
 *      Author: Ashley
 */

#ifndef CONFIGURABLEEMITTER_H_
#define CONFIGURABLEEMITTER_H_


#include "../Graphics/Color.h"
#include "../Util/Range.h"
#include "ParticleEmitter.h"
#include "../Graphics/Image/Image.h"


#include <string>
#include <list>
#include <vector>
using namespace std;

namespace ARK {
	namespace Particles {

		class Value {
			public:
				virtual float getValue(float time) = 0;
		};
		class SimpleValue : public ARK::Particles::Value {
			public:
				float m_value;
				float m_next;
				SimpleValue(float value) {
					m_value = value;
				}
				float getValue(float time) {
					return m_value;
				}
				void setValue(float value) {
					m_value = value;
				}
				virtual ~SimpleValue() {

				}
		};
		class RandomValue : public ARK::Particles::Value {
			public:
				float m_value;
				RandomValue(float value) {
					m_value = value;
				}
				float getValue(float time) {
					return float(float(rand())/RAND_MAX) * m_value;
				}
				float getValue() {
					return m_value;
				}
				void setValue(float value) {
					m_value = value;
				}
				virtual ~RandomValue() {

				}
		};
		class LinearInterpolator : public ARK::Particles::Value {
			public:
				vector<Vector2<float>* > m_curve;
				bool m_active;
				int m_min;
				int m_max;
				LinearInterpolator(vector<Vector2<float>*> curve, int min, int max):
					m_curve(curve), m_min(min), m_max(max) {

				}
				void setCurve(vector<Vector2<float>*> curve) {
					m_curve = curve;
				}
				vector<Vector2<float>* > getCurve() {
					return m_curve;
				}
				float getValue(float time) {
					Vector2<float>* p0 = m_curve.at(0);
					for (unsigned int i = 1; i < m_curve.size(); i++) {
						Vector2<float>* p1 = m_curve.at(i);
						if (time >= p0->getX() && time <= p1->getX()) {
							// found the segment?
							float st = (time - p0->getX()) / (p1->getX() - p0->getX());
							float r = p0->getY() + st * (p1->getY() - p0->getY());
							return r;
						}
						p0 = p1;
					}
					return 0;
				}
				bool isActive() {
					return m_active;
				}
				void setActive(bool b) {
					m_active = b;
				}
				int getMax() {
					return m_max;
				}
				void setMax(int m) {
					m_max = m;
				}
				int getMin() {
					return m_min;
				}
				void setMin(int m) {
					m_min = m;
				}
				virtual ~LinearInterpolator() {
					vector<Vector2<float>* >::iterator it = m_curve.begin();
					while (it != m_curve.end()) {
						delete (*it);
						it = m_curve.erase(it);
					}
				}

		};


		class ColorRecord {
			public:
				float pos;
				Color* col;
				ColorRecord(float p, Color* c):
					pos(p), col(c) {

				}
				~ColorRecord() {
					delete col;
				}
		};

		/*!
		 * \brief The emitter used by Pedigree and programmatic particle systems.
		 *
		 * Adapted from Kevin Glass's Java implementation.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 * @author Kevin Glass
		 */
		class ConfigurableEmitter : public ParticleEmitter {
			friend class ParticleIO;
			friend class ParticleSystem;

			private:

				Range<int> m_spawnInterval;
				Range<int> m_spawnCount;
				Range<int> m_initialLife;
				Range<int> m_initialSize;
				Range<int> m_xOffset;
				Range<int> m_yOffset;

				RandomValue m_spread;

				SimpleValue m_angularOffset;
				Range<int> m_initialDistance;
				Range<int> m_speed;
				SimpleValue m_growthFactor;
				SimpleValue m_gravityFactor;
				SimpleValue m_windFactor;
				Range<int> m_length;

				vector<ColorRecord*> m_colors;
				SimpleValue m_startAlpha;
				SimpleValue m_endAlpha;
				LinearInterpolator* m_alpha;
				LinearInterpolator* m_size;
				LinearInterpolator* m_velocity;
				LinearInterpolator* m_scaleY;

				Range<int> m_emitCount;

				int m_usePoints;
				bool m_useOriented;
				bool m_useAdditive;

				string m_name;

				Image* m_image;
				bool m_updateImage;

				bool m_enabled;

				float m_x;
				float m_y;

				int m_nextSpawn;
				int m_timeout;
				ParticleSystem* m_engine;
				int m_particleCount;
				int m_leftToEmit;

				bool m_wrapUp;
				bool m_completed;

				bool m_adjust;
				float m_adjustx;
				float m_adjusty;

			public:
				ConfigurableEmitter(string name);

				void update(ParticleSystem* system, float delta);
				void updateParticle(Particle* particle, float delta);

				bool isComplete();
				void wrapUp();
				bool isEnabled();
				void setEnabled(bool b);
				Image* getImage();
				bool useAdditive();
				bool isOriented();
				bool usePoints();
				void resetState();

				void replay();
				void reset();
				void addColorPoint(float pos, Color* col);

				float getX();
				float getY();

				void setName(string name);

				void setUsePoints(int p);
				void setUseOriented(bool b);
				void setUseAdditive(bool b);

				void setPosition(float x, float y);
				void setPosition(float x, float y, bool moveParticles);

				Range<int>* getSpawnInterval();
				Range<int>* getSpawnCount();
				Range<int>* getInitialLife();
				Range<int>* getInitialSize();
				Range<int>* getInitialDistance();
				Range<int>* getOffsetX();
				Range<int>* getOffsetY();
				Range<int>* getSpeed();
				Range<int>* getLength();
				Range<int>* getEmitCount();
				void setSpread(float value);
				void setAngularOffset(float value);
				void setGrowthFactor(float value);
				void setGravityFactor(float value);
				void setWindFactor(float value);
				void setStartAlpha(float value);
				void setEndAlpha(float value);
				void setAlpha(float value);


				virtual ~ConfigurableEmitter();
		};
	}
}

#endif /* CONFIGURABLEEMITTER_H_ */
