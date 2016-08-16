/*
 * Game.h
 *  Created on: 15-Oct-2009
 *      Author: Ashley Gwinnell
 */

#ifndef ARK_CORE_GAME_H_
#define ARK_CORE_GAME_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "Controls/GamepadListener.h"
#include "Geometry/Cube.h"
#include "SceneGraph/Scene.h"

namespace ARK {
	namespace Core {

		/*!
		 * \brief A game with one game state should extend this class. e.g. test cases.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Game : public SceneNode, public GamepadListener/*, public MouseListener*/ {
			public:

				Game(string title);
				string getTitle();
				Timeline* getTimeline();
				virtual void init(GameContainer* container);

				virtual void preUpdate(GameContainer* container, GameTimer* timer);
				virtual void update(GameContainer* container, GameTimer* timer) = 0;
				virtual void postUpdate(GameContainer* container, GameTimer* timer);

				virtual void preRender(GameContainer* container, Renderer* g);
				virtual void render(GameContainer* container, Renderer* g) = 0;
				virtual void postRender(GameContainer* container, Renderer* g);

				// SceneNode overrides
				virtual void update();
				virtual void render();
				virtual void preRender();
				virtual void postRender();
				virtual void onResize();

				virtual void resize(GameContainer* container, int width, int height);
				virtual ARK::Core::Geometry::Cube* getBounds();

				virtual void pause(); // android events
				virtual void resume();

				// Key Listener
				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);

				// Mouse Listener
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);

				// Gamepad Listener
				virtual void gamepadConnected(Controls::Gamepad* gamepad);
				virtual void gamepadDisconnected(Controls::Gamepad* gamepad);
				virtual void buttonPressed(Controls::Gamepad* gamepad, unsigned int button);
				virtual void buttonReleased(Controls::Gamepad* gamepad, unsigned int button);
				virtual void axisMoved(Controls::Gamepad* gamepad, unsigned int axis, float value);

				// Orientation Listener
				virtual void orientationChanged(int orientation);

				virtual ~Game();

			protected:
				GameContainer* m_container;
				string m_title;
				Timeline* m_timeline;
				Scene* scene;
                ARK::Core::Geometry::Cube bounds;

		};
	}
}

#endif /* ARK_CORE_GAME_H_ */
