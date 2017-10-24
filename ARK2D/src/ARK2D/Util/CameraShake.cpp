/*
 * Camera.cpp
 *
 *  Created on: 18 Aug 2010
 *      Author: Ashley
 */

#include "CameraShake.h"
#include "../Core/Game.h"
#include "../Core/GameTimer.h"
#include "../Core/Tween/Timeline.h"
#include "../Core/Graphics/Renderer.h"

namespace ARK {
	namespace Util {
		CameraShakeLayer::CameraShakeLayer():
			m_offset_x(0),  
			m_offset_y(0)
		{
			m_xState = STATE_OFF;
			m_yState = STATE_OFF; 
			m_speedMultiplier = 1.0f;
		}
		void CameraShakeLayer::reset() {
			m_offset_x = 0;
			m_offset_y = 0; 
			
			m_xState = STATE_OFF;
			m_xMagnitude = 0.0f;

			m_yState = STATE_OFF;
			m_yMagnitude = 0.0f;

			m_speedMultiplier = 1.0f;
		}
		void CameraShakeLayer::setSpeedMultiplier(float f) {
			m_speedMultiplier = f;	
		}
		void CameraShakeLayer::start() {
			start(50.0f);
		}
		void CameraShakeLayer::start(float magnitude) {
			//if (m_timeline->getKeyedIndependentEvent("CAMERA_SHAKE_X") != NULL) {
			//	m_timeline->removeKeyedIndependentEvent("CAMERA_SHAKE_X");
			//}
			//magnitude = 20;
 
			
			

			/*int mt = MathUtil::randBetween(30, 50);
			CameraShakeMagnitude2* one   = new CameraShakeMagnitude2(this, 0.0f, magnitude, mt); 		one->m_easing = Easing::QUADRATIC_OUT; // CUBIC_OUT
			CameraShakeMagnitude2* two   = new CameraShakeMagnitude2(this, magnitude, 0.0f, mt);		two->m_easing = Easing::QUADRATIC_OUT; // CUBIC_IN;
			CameraShakeMagnitude2* three = new CameraShakeMagnitude2(this, 0.0f, magnitude * -1, mt);	three->m_easing = Easing::QUADRATIC_OUT; // CUBIC_OUT;
			CameraShakeMagnitude2* four  = new CameraShakeMagnitude2(this, magnitude * -1, 0.0f, mt);	four->m_easing = Easing::QUADRATIC_OUT; //CUBIC_IN;

			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_X_1", one);
			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_X_2", two, mt);
			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_X_3", three, mt*2);
			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_X_4", four, mt*3);

			int oldmt = mt;
			mt = MathUtil::randBetween(30, 50);
			if (oldmt == mt) {
				mt += 1;
			}
			CameraShakeMagnitude2Y* yone   = new CameraShakeMagnitude2Y(this, 0.0f, magnitude, mt); 		yone->m_easing = Easing::QUADRATIC_OUT; // CUBIC_OUT;
			CameraShakeMagnitude2Y* ytwo   = new CameraShakeMagnitude2Y(this, magnitude, 0.0f, mt);			ytwo->m_easing = Easing::QUADRATIC_OUT; //CUBIC_IN;
			CameraShakeMagnitude2Y* ythree = new CameraShakeMagnitude2Y(this, 0.0f, magnitude * -1, mt);	ythree->m_easing = Easing::QUADRATIC_OUT; //CUBIC_OUT;
			CameraShakeMagnitude2Y* yfour  = new CameraShakeMagnitude2Y(this, magnitude * -1, 0.0f, mt);	yfour->m_easing = Easing::QUADRATIC_OUT; //CUBIC_IN;

			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_Y_1", yone, 1);
			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_Y_2", ytwo, mt+1);
			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_Y_3", ythree, (mt*2)+1);
			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_Y_4", yfour, (mt*3)+1);*/

			float millis = MathUtil::randBetweenf(0.025f, 0.045f); 
			float millis2 = MathUtil::randBetweenf(0.025f, 0.045f);

			if (millis2 == millis) { millis2 += 0.001f; }

			

			//m_xState = STATE_1;
			m_xMagnitude = magnitude;
			m_x1Duration = millis; 
			m_x2Duration = millis;
			m_x3Duration = millis;
			m_x4Duration = millis;

			//m_yState = STATE_1;
			m_yMagnitude = magnitude; 
			m_y1Duration = millis2;
			m_y2Duration = millis2; 
			m_y3Duration = millis2;
			m_y4Duration = millis2;

			/*if (isShaking()) {
				m_xMagnitude *= -1.0f;
				m_yMagnitude *= -1.0f;
			}*/

			if (!isShaking()) { 
				m_xState = STATE_1;
				m_yState = STATE_1; 

				m_x1Timer = 0.0f;
				m_x2Timer = 0.0f;
				m_x3Timer = 0.0f;
				m_x4Timer = 0.0f;

				m_y1Timer = 0.0f;
				m_y2Timer = 0.0f; 
				m_y3Timer = 0.0f; 
				m_y4Timer = 0.0f; 

				float roll = MathUtil::randBetweenf(0.0f, 1.0f);
				if (roll > 0.5f) {
					m_xMagnitude *= -1.0f;
				} 
				
				roll = MathUtil::randBetweenf(0.0f, 1.0f);
				if (roll > 0.5f) {
					m_yMagnitude *= -1.0f;
				}
			} 
		}
		bool CameraShakeLayer::isShaking() {
			/*if (m_timeline->getKeyedIndependentEvent("CAMERA_SHAKE_X_1") == NULL
					&& m_timeline->getKeyedIndependentEvent("CAMERA_SHAKE_X_2") == NULL
					&& m_timeline->getKeyedIndependentEvent("CAMERA_SHAKE_X_3") == NULL
					&& m_timeline->getKeyedIndependentEvent("CAMERA_SHAKE_X_4") == NULL
					&& m_timeline->getKeyedIndependentEvent("CAMERA_SHAKE_Y_1") == NULL
					&& m_timeline->getKeyedIndependentEvent("CAMERA_SHAKE_Y_2") == NULL
					&& m_timeline->getKeyedIndependentEvent("CAMERA_SHAKE_Y_3") == NULL
					&& m_timeline->getKeyedIndependentEvent("CAMERA_SHAKE_Y_4") == NULL
					) {
				return false;
			}  
			return true;*/
			if (m_xState == STATE_OFF && m_yState == STATE_OFF) { return false; }
			return true;
		}
		void CameraShakeLayer::update(GameContainer* container, GameTimer* timer) {
			update(container, timer, timer->getDeltaModifier());
		}
		void CameraShakeLayer::update(GameContainer* container, GameTimer* timer, float deltaModifier) {
			// m_timeline->update(timer);

			// X SHAKE
			float delta = timer->getDeltaNoModifiers() * deltaModifier;
			{
				bool xComplete = false;
				if (m_xState == STATE_1) {
					m_x1Timer += delta * m_speedMultiplier;
					if (m_x1Timer >= m_x1Duration) {
						m_xState = STATE_2;
						m_x2Timer = (m_x1Timer - m_x1Duration);
						m_x1Timer = m_x1Duration;
					}

					m_offset_x = Easing::ease(Easing::QUADRATIC_OUT, m_x1Timer, 0.0f, m_xMagnitude, m_x1Duration);

				} else if (m_xState == STATE_2) {
					m_x2Timer += delta * m_speedMultiplier; 
					if (m_x2Timer >= m_x2Duration) {
						m_xState = STATE_3;
						m_x3Timer = (m_x2Timer - m_x2Duration);
						m_x2Timer = m_x2Duration;
					}

					m_offset_x = Easing::ease(Easing::QUADRATIC_IN, m_x2Timer, m_xMagnitude, m_xMagnitude * -1.0f, m_x2Duration);

				} else if (m_xState == STATE_3) {
					m_x3Timer += delta * m_speedMultiplier;
					if (m_x3Timer >= m_x3Duration) {
						m_xState = STATE_4;
						m_x4Timer = (m_x3Timer - m_x3Duration);
						m_x3Timer = m_x3Duration;
					}

					m_offset_x = Easing::ease(Easing::QUADRATIC_OUT, m_x3Timer, 0.0f, m_xMagnitude * -1.0f, m_x3Duration);

				} else if (m_xState == STATE_4) {
					m_x4Timer += delta * m_speedMultiplier;
					if (m_x4Timer >= m_x4Duration) {
						m_xState = STATE_OFF;
						m_x4Timer = m_x4Duration;
						xComplete = true;
					}

					m_offset_x = Easing::ease(Easing::QUADRATIC_IN, m_x4Timer, m_xMagnitude * -1.0f, m_xMagnitude, m_x4Duration);
				}
	 
				if (xComplete) {
					m_x1Timer = 0.0f;
					m_x2Timer = 0.0f;
					m_x3Timer = 0.0f;
					m_x4Timer = 0.0f;
				}

			}


			// Y SHAKE
			{
				bool yComplete = false;
				if (m_yState == STATE_1) {
					m_y1Timer += delta * m_speedMultiplier;
					if (m_y1Timer >= m_y1Duration) {
						m_yState = STATE_2;
						m_y2Timer = (m_y1Timer - m_y1Duration);
						m_y1Timer = m_y1Duration;
					}

					m_offset_y = Easing::ease(Easing::QUADRATIC_OUT, m_y1Timer, 0.0f, m_yMagnitude, m_y1Duration);

				} else if (m_yState == STATE_2) {
					m_y2Timer += delta * m_speedMultiplier; 
					if (m_y2Timer >= m_y2Duration) {
						m_yState = STATE_3;
						m_y3Timer = (m_y2Timer - m_y2Duration);
						m_y2Timer = m_y2Duration;
					}

					m_offset_y = Easing::ease(Easing::QUADRATIC_IN, m_y2Timer, m_yMagnitude, m_yMagnitude * -1.0f, m_y2Duration);

				} else if (m_yState == STATE_3) {
					m_y3Timer += delta * m_speedMultiplier;
					if (m_y3Timer >= m_y3Duration) {
						m_yState = STATE_4;
						m_y4Timer = (m_y3Timer - m_y3Duration);
						m_y3Timer = m_y3Duration;
					}

					m_offset_y = Easing::ease(Easing::QUADRATIC_OUT, m_y3Timer, 0.0f, m_yMagnitude * -1.0f, m_y3Duration);

				} else if (m_yState == STATE_4) {
					m_y4Timer += delta * m_speedMultiplier;
					if (m_y4Timer >= m_y4Duration) {
						m_yState = STATE_OFF;
						m_y4Timer = m_y4Duration;
						yComplete = true;
					}

					m_offset_y = Easing::ease(Easing::QUADRATIC_IN, m_y4Timer, m_yMagnitude * -1.0f, m_yMagnitude, m_y4Duration);
				}
	 
				if (yComplete) {
					m_y1Timer = 0.0f;
					m_y2Timer = 0.0f;
					m_y3Timer = 0.0f;
					m_y4Timer = 0.0f;
				}
			}

			if (!isShaking()) {
				m_offset_x = 0;
				m_offset_y = 0;
			}
		}

		void CameraShakeLayer::setXOffset(float x) {
			m_offset_x = x;
		}
		void CameraShakeLayer::setYOffset(float y) {
			m_offset_y = y;
		}
		float CameraShakeLayer::getXOffset() {
			return m_offset_x;
		}
		float CameraShakeLayer::getYOffset() {
			return m_offset_y;
		}
		CameraShakeLayer::~CameraShakeLayer() {

		}

		CameraShake::CameraShake(Game* game):
			m_game(game),
			m_layers() 
			{
			m_layers.push_back(new CameraShakeLayer());
		}
		CameraShake::CameraShake(Game* game, int layers):
			m_game(game),
			m_layers()
			{
			
			for(unsigned int i = 0; i < layers; ++i) {
				m_layers.push_back(new CameraShakeLayer());
			}
		}
		void CameraShake::reset() {
			for(unsigned int i = 0; i < m_layers.size(); ++i) {
				m_layers[i]->reset();
			}
		}
		void CameraShake::start() {
			start(50.0f);
		}
		void CameraShake::start(float magnitude) {
			m_layers[0]->start(magnitude);
		}
		void CameraShake::start(int layer, float magnitude) {
			m_layers[layer]->start(magnitude);
		}
		void CameraShake::setSpeedMultiplier(int layer, float f){
			m_layers[layer]->setSpeedMultiplier(f);
		}

		void CameraShake::setXOffset(float x) {
			for(unsigned int i = 0; i < m_layers.size(); ++i) {
				m_layers[i]->setXOffset((i==0)?x:0);
			}
			
		}
		float CameraShake::getXOffset() {
			float x = 0.0f;
			for(unsigned int i = 0; i < m_layers.size(); ++i) {
				x += m_layers[i]->getXOffset();
			}
			return x; 
		}

		
		void CameraShake::setYOffset(float y) {
			for(unsigned int i = 0; i < m_layers.size(); ++i) {
				m_layers[i]->setYOffset((i==0)?y:0);
			}
		}
		float CameraShake::getYOffset() {
			float y = 0.0f;
			for(unsigned int i = 0; i < m_layers.size(); ++i) {
				y += m_layers[i]->getYOffset();
			}
			return y; 
		}


		bool CameraShake::isShaking() {
			for(unsigned int i = 0; i < m_layers.size(); ++i) {
				if (m_layers[i]->isShaking()) { 
					return true; 
				}
			}
			return false;
		}
		void CameraShake::update(GameContainer* container, GameTimer* timer) {
			update(container, timer, timer->getDeltaModifier());
		}
		void CameraShake::update(GameContainer* container, GameTimer* timer, float deltaModifier) {
			for(unsigned int i = 0; i < m_layers.size(); ++i) {
				m_layers[i]->update(container, timer, deltaModifier);
			}
		}


		void CameraShake::preRender(GameContainer* container, Renderer* g) {
			if (!isShaking()) { return; }
			g->pushMatrix();
			g->translate(getXOffset(), getYOffset());
		}
		void CameraShake::render(GameContainer* container, Renderer* g) {

		}
		void CameraShake::postRender(GameContainer* container, Renderer* g) {
			if (!isShaking()) { return; }
			g->translate(getXOffset() * -1.0f, getYOffset() * -1.0f);
			g->popMatrix();
 
			//g->drawString("");
		}
		CameraShake::~CameraShake() {

		}

	}
}
