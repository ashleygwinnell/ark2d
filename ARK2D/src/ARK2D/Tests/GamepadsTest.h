/*
 * GamepadsTest.h
 *
 *  Created on: May 27 2014
 *      Author: ashleygwinnell
 */

#ifndef ARK_TESTS_GamepadsTestState_H__
#define ARK_TESTS_GamepadsTestState_H__

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "../Core/State/GameState.h"
#include "../Core/State/StateBasedGame.h"

#include <vector>
using std::vector;

namespace ARK {
	namespace Tests {

		class ARK2D_API GamepadConfigureGameState : public GameState, public GamepadListener {
			public:
				GamepadMapping m_mapping;
				string m_mappingString;
				signed int m_gamepadIndex;
				unsigned int m_stateId;
				GameState* m_returnToState;
				unsigned int m_state;
				static const unsigned int STATE_A = 0;
				static const unsigned int STATE_B = 1;
				static const unsigned int STATE_X = 2;
				static const unsigned int STATE_Y = 3;
				static const unsigned int STATE_DPAD_UP = 4;
				static const unsigned int STATE_DPAD_DOWN = 5;
				static const unsigned int STATE_DPAD_LEFT = 6;
				static const unsigned int STATE_DPAD_RIGHT = 7;
				static const unsigned int STATE_LBUMPER = 8;
				static const unsigned int STATE_RBUMPER = 9;
				static const unsigned int STATE_L3 = 10;
				static const unsigned int STATE_R3 = 11;
				static const unsigned int STATE_LSTICK_X = 12;
				static const unsigned int STATE_LSTICK_Y = 13;
				static const unsigned int STATE_RSTICK_X = 14;
				static const unsigned int STATE_RSTICK_Y = 15;
				static const unsigned int STATE_LTRIGGER = 16;
				static const unsigned int STATE_RTRIGGER = 17;
				static const unsigned int STATE_BACK = 18;
				static const unsigned int STATE_START = 19;
				static const unsigned int STATE_END = 20;

				float m_axisChangedCooldown;

			public:
				GamepadConfigureGameState(unsigned int stateId);
				void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				unsigned int id();
				void init(GameContainer* container, StateBasedGame* game);
				void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				void render(GameContainer* container, StateBasedGame* game, Renderer* r);

				static void alertMappingString(GamepadConfigureGameState* gs);
				static void returnToStateStatic(GamepadConfigureGameState* gs);
				void stateChanged();
				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);

				virtual void gamepadConnected(ARK::Core::Controls::Gamepad* gamepad);
				virtual void gamepadDisconnected(ARK::Core::Controls::Gamepad* gamepad);
				virtual void buttonPressed(ARK::Core::Controls::Gamepad* gamepad, unsigned int button);
				virtual void buttonReleased(ARK::Core::Controls::Gamepad* gamepad, unsigned int button);
				virtual void axisMoved(ARK::Core::Controls::Gamepad* gamepad, unsigned int axis, float value);

				virtual ~GamepadConfigureGameState();
		};


		class ARK2D_API GamepadsTestGameState : public GameState, public GamepadListener {
			public:
				signed int m_gamepadIndex;
				CheckBox* m_alertButtons;
				CheckBox* m_alertAxes;
                ARK::UI::Button* m_autoConfig;
                ARK::UI::Button* m_removeConfig;
                GameState* m_returnToState;
			public:
				GamepadsTestGameState();
				void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				unsigned int id();
				void init(GameContainer* container, StateBasedGame* game);
				void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				void render(GameContainer* container, StateBasedGame* game, Renderer* r);
				static void renderGamepad(ARK::Core::Controls::Gamepad* g, float rootX, float rootY);

				static void returnToStateStatic(GamepadsTestGameState* gs);

				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);

				virtual void gamepadConnected(ARK::Core::Controls::Gamepad* gamepad);
				virtual void gamepadDisconnected(ARK::Core::Controls::Gamepad* gamepad);
				virtual void buttonPressed(ARK::Core::Controls::Gamepad* gamepad, unsigned int button);
				virtual void buttonReleased(ARK::Core::Controls::Gamepad* gamepad, unsigned int button);
				virtual void axisMoved(ARK::Core::Controls::Gamepad* gamepad, unsigned int axis, float value);

				virtual ~GamepadsTestGameState();
		};

		class ARK2D_API GamepadsTest : public StateBasedGame {
			public:
				GamepadsTest();

				virtual void initStates(GameContainer* container);
				virtual void update(GameContainer* container, GameTimer* timer);
				virtual void render(GameContainer* container, Renderer* r);
				virtual void render();
				virtual void resize(GameContainer* container, int width, int height);

				static int start();

				virtual ~GamepadsTest();
		};

	}
}



#endif /* ARK_TESTS_GamepadsTestState_H__ */
