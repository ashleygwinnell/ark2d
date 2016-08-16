/*
 * CameraShake.h
 *
 *  Created on: 18 Aug 2010
 *      Author: Ashley
 */

#ifndef ARK_UTIL_CAMERASHAKE_H_
#define ARK_UTIL_CAMERASHAKE_H_

//#include "../Includes.h"
#include "../Namespaces.h"
#include "../Common/DLL.h"
#include "../Core/Tween/Timeline.h"
#include "../Util/Log.h"

namespace ARK {
	namespace Util {

		/*!
		 * \brief Used to shake the GL viewport.
		 *
		 * It needs improvement because it's very jagged and not smooth enough and it only shakes in X axis (IIRC).
		 * @todo Improve the shaky jaggedness.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class CameraShakeLayer {
			public:
				float m_offset_x;
				float m_offset_y;

				static const unsigned int STATE_OFF = 0;
				static const unsigned int STATE_1 = 1;
				static const unsigned int STATE_2 = 2;
				static const unsigned int STATE_3 = 3;
				static const unsigned int STATE_4 = 4;

				unsigned int m_xState;
				float m_xMagnitude;
				float m_x1Timer;
				float m_x1Duration;
				float m_x2Timer;
				float m_x2Duration;
				float m_x3Timer;
				float m_x3Duration;
				float m_x4Timer;
				float m_x4Duration;

				unsigned int m_yState;
				float m_yMagnitude;
				float m_y1Timer;
				float m_y1Duration;
				float m_y2Timer;
				float m_y2Duration;
				float m_y3Timer;
				float m_y3Duration;
				float m_y4Timer;
				float m_y4Duration;

				float m_speedMultiplier;

				CameraShakeLayer();
				void reset();
				void start();
				void start(float magnitude);
				void setSpeedMultiplier(float f);
				bool isShaking();
				void setXOffset(float x); 
				void setYOffset(float y);
				float getXOffset();
				float getYOffset();
				void update(GameContainer* container, GameTimer* timer);
				virtual ~CameraShakeLayer();
		};
		class ARK2D_API CameraShake {
			public:
				CameraShake(Game* game);
				CameraShake(Game* game, int layers);

				void reset();
				void start(); 
				void start(float magnitude);

				void start(int layer, float magnitude);
				void setSpeedMultiplier(int layer, float f);

				void setXOffset(float x); 
				void setYOffset(float y);

				float getXOffset();
				float getYOffset();

				bool isShaking();
				void update(GameContainer* container, GameTimer* timer);

				void preRender(GameContainer* container, Renderer* g);
				void render(GameContainer* container, Renderer* g);
				void postRender(GameContainer* container, Renderer* g);

				virtual ~CameraShake();

			private:
				Game* m_game;
				vector<CameraShakeLayer*> m_layers;
				
				//Timeline* m_timeline;

				

		};

		class CameraShakeMagnitude1 : public TweenedEvent {
			public:
				CameraShake* m_cameraShake;
				CameraShakeMagnitude1(CameraShake* shake, float start_val, float end_val, int millis):
										TweenedEvent(start_val, end_val, millis),
										m_cameraShake(shake) {
					m_easing = Easing::CUBIC_IN_OUT;
				}
				void invoke(Timeline* t, float current_val) {
					if (m_cameraShake->getXOffset() >= 0) {
						m_cameraShake->setXOffset(current_val * -1);
					} else {
						m_cameraShake->setXOffset(current_val);
					}


				}
				virtual ~CameraShakeMagnitude1() {

				}
		};

		class CameraShakeMagnitude2 : public TweenedEvent {
			public:
				CameraShake * m_cameraShake;
				CameraShakeMagnitude2(CameraShake* shake, float start_val, float end_val, int millis):
										TweenedEvent(start_val, end_val, millis),
										m_cameraShake(shake) { 
					m_easing = Easing::CUBIC_IN_OUT;
					ARK2D::getLog()->w("New CameraShakeMagnitude2");
				}
				void invoke(Timeline* t, float current_val) {
					m_cameraShake->setXOffset(current_val);
				}
				virtual ~CameraShakeMagnitude2() {
					ARK2D::getLog()->w("Delete CameraShakeMagnitude2");
				}
		};
		class CameraShakeMagnitude2Y : public TweenedEvent { 
			public:
				CameraShake * m_cameraShake;
				CameraShakeMagnitude2Y(CameraShake* shake, float start_val, float end_val, int millis):
										TweenedEvent(start_val, end_val, millis),
										m_cameraShake(shake) {
					m_easing = Easing::CUBIC_IN_OUT;
					ARK2D::getLog()->w("New CameraShakeMagnitude2Y");
				}
				void invoke(Timeline* t, float current_val) {
					m_cameraShake->setYOffset(current_val);
				} 
				virtual ~CameraShakeMagnitude2Y() {
					ARK2D::getLog()->w("Delete CameraShakeMagnitude2Y");

				}
		};

	}
}


#endif /* CAMERASHAKE_H_ */
