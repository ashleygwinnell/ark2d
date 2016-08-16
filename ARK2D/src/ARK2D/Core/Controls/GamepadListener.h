/*
 * Gamepad.h
 *
 *  Created on: 28 Jul 2016
 *      Author: Ashley
 */

#ifndef ARK_CONTROLS_GAMEPADLISTENER_H_
#define ARK_CONTROLS_GAMEPADLISTENER_H_

namespace ARK {
	namespace Core {
		namespace Controls {
            class Gamepad;

			class GamepadListener {
				public:
					virtual void gamepadConnected(Gamepad* gamepad) = 0;
					virtual void gamepadDisconnected(Gamepad* gamepad) = 0;
					virtual void buttonPressed(Gamepad* gamepad, unsigned int button) = 0;
					virtual void buttonReleased(Gamepad* gamepad, unsigned int button) = 0;
					virtual void axisMoved(Gamepad* gamepad, unsigned int axis, float value) = 0;
			};
		}
	}
}

#endif
