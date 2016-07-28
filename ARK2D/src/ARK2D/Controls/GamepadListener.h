/*
 * Gamepad.h
 *
 *  Created on: 28 Jul 2016
 *      Author: Ashley
 */

#ifndef ARK_CONTROLS_GAMEPADLISTENER_H_
#define ARK_CONTROLS_GAMEPADLISTENER_H_

namespace ARK {
	namespace Controls {

		class GamepadListener {
			public:
				virtual void gamepadConnected(ARK::Controls::Gamepad* gamepad) = 0;
				virtual void gamepadDisconnected(ARK::Controls::Gamepad* gamepad) = 0;
				virtual void buttonPressed(ARK::Controls::Gamepad* gamepad, unsigned int button) = 0;
				virtual void buttonReleased(ARK::Controls::Gamepad* gamepad, unsigned int button) = 0;
				virtual void axisMoved(ARK::Controls::Gamepad* gamepad, unsigned int axis, float value) = 0;
		};
	}
}

#endif
