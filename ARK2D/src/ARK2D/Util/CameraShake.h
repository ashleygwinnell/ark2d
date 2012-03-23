/*
 * CameraShake.h
 *
 *  Created on: 18 Aug 2010
 *      Author: Ashley
 */

#ifndef CAMERASHAKE_H_
#define CAMERASHAKE_H_

#include "../ARK2D_namespaces.h"
#include "../Tween/Timeline.h"

class GameContainer;
class Game;
class Event;
class GameTimer;
class Graphics;

namespace ARK {
	namespace Util {

		/*!
		 * \brief Used to shake the GL viewport.
		 *
		 * It needs improvement because it's very jagged and not smooth enough and it only shakes in X axis (IIRC).
		 * @todo Improve the shaky jaggedness.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class CameraShake {
			public:
				CameraShake(Game* game);

				void start();
				void start(float magnitude);

				void setXOffset(float x);
				void setYOffset(float y);

				float getXOffset();
				float getYOffset();

				bool isShaking();
				void update(GameContainer* container, GameTimer* timer);

				void preRender(GameContainer* container, Graphics* g);
				void render(GameContainer* container, Graphics* g);
				void postRender(GameContainer* container, Graphics* g);

				virtual ~CameraShake();

			private:
				Game* m_game;
				float m_offset_x;
				float m_offset_y;
				Timeline* m_timeline;
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
				}
				void invoke(Timeline* t, float current_val) {
					m_cameraShake->setXOffset(current_val);
				}
				virtual ~CameraShakeMagnitude2() {

				}
		};

	}
}


#endif /* CAMERASHAKE_H_ */
