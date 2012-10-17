/*
 * Camera.cpp
 *
 *  Created on: 18 Aug 2010
 *      Author: Ashley
 */

#include "CameraShake.h"
#include "../Core/Game.h"
#include "../Tween/Timeline.h"
#include "../Graphics/Renderer.h"

namespace ARK {
	namespace Util {

		CameraShake::CameraShake(Game* game):
			m_game(game),
			m_offset_x(0),
			m_offset_y(0)
			{
			m_timeline = game->getTimeline();
		}
		void CameraShake::start() {
			start(50.0f);
		}
		void CameraShake::start(float magnitude) {
			//if (m_timeline->getKeyedIndependentEvent("CAMERA_SHAKE_X") != NULL) {
			//	m_timeline->removeKeyedIndependentEvent("CAMERA_SHAKE_X");
			//}
			//magnitude = 20;
			int mt = 20;
			CameraShakeMagnitude2* one   = new CameraShakeMagnitude2(this, 0.0f, magnitude, mt); 		one->m_easing = Easing::CUBIC_OUT;
			CameraShakeMagnitude2* two   = new CameraShakeMagnitude2(this, magnitude, 0.0f, mt);		two->m_easing = Easing::CUBIC_IN;
			CameraShakeMagnitude2* three = new CameraShakeMagnitude2(this, 0.0f, magnitude * -1, mt);	three->m_easing = Easing::CUBIC_OUT;
			CameraShakeMagnitude2* four  = new CameraShakeMagnitude2(this, magnitude * -1, 0.0f, mt);	four->m_easing = Easing::CUBIC_IN;

			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_X_1", one);
			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_X_2", two, mt);
			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_X_3", three, mt*2);
			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_X_4", four, mt*3);

			mt = 20;
			CameraShakeMagnitude2Y* yone   = new CameraShakeMagnitude2Y(this, 0.0f, magnitude, mt); 		yone->m_easing = Easing::CUBIC_OUT;
			CameraShakeMagnitude2Y* ytwo   = new CameraShakeMagnitude2Y(this, magnitude, 0.0f, mt);			ytwo->m_easing = Easing::CUBIC_IN;
			CameraShakeMagnitude2Y* ythree = new CameraShakeMagnitude2Y(this, 0.0f, magnitude * -1, mt);	ythree->m_easing = Easing::CUBIC_OUT;
			CameraShakeMagnitude2Y* yfour  = new CameraShakeMagnitude2Y(this, magnitude * -1, 0.0f, mt);	yfour->m_easing = Easing::CUBIC_IN;

			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_Y_1", yone, 1);
			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_Y_2", ytwo, mt+1);
			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_Y_3", ythree, (mt*2)+1);
			m_timeline->addKeyedIndependentEvent("CAMERA_SHAKE_Y_4", yfour, (mt*3)+1);
		}

		void CameraShake::setXOffset(float x) {
			m_offset_x = x;
		}
		float CameraShake::getXOffset() {
			return m_offset_x;
		}

		void CameraShake::setYOffset(float y) {
			m_offset_y = y;
		}
		float CameraShake::getYOffset() {
			return m_offset_y;
		}


		bool CameraShake::isShaking() {
			if (m_timeline->getKeyedIndependentEvent("CAMERA_SHAKE_X_1") == NULL
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
			return true;
		}
		void CameraShake::update(GameContainer* container, GameTimer* timer) {
			m_timeline->update(timer);

			if (!isShaking()) {
				m_offset_x = 0;
				m_offset_y = 0;
			}
		}


		void CameraShake::preRender(GameContainer* container, Renderer* g) {
			if (!isShaking()) { return; }
			g->pushMatrix();
			g->translate(m_offset_x, m_offset_y);
		}
		void CameraShake::render(GameContainer* container, Renderer* g) {

		}
		void CameraShake::postRender(GameContainer* container, Renderer* g) {
			if (!isShaking()) { return; }
			g->translate(m_offset_x * -1, m_offset_y * -1);
			g->popMatrix();
		}
		CameraShake::~CameraShake() {

		}

	}
}
