/*
 * ShaderTest.h
 *
 *  Created on: May 16, 2015
 *      Author: ashleygwinnell
 */

#ifndef ARK_TESTS_SHADERTEST_H_
#define ARK_TESTS_SHADERTEST_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "../State/GameState.h"
#include "../State/StateBasedGame.h"

#include "../Util/Containers/Vector.h"

#include <string>
using namespace std;

namespace ARK {
	namespace Tests {

		class ARK2D_API HSVShaderTestGameState : public GameState {
			public:
				int index;
				string name;
				ARK::Graphics::HSVShader* shader;
				ARK::Graphics::Image* texture;
				ARK::UI::Slider* m_sliderHue;
				ARK::UI::Slider* m_sliderSaturation;
				ARK::UI::Slider* m_sliderValue;

				float m_timer;
			public:
				HSVShaderTestGameState(int index, string name);
				void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				unsigned int id();
				void init(GameContainer* container, StateBasedGame* game);
				void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				void render(GameContainer* container, StateBasedGame* game, Renderer* g);
				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);
				virtual ~HSVShaderTestGameState();
		};

		class ARK2D_API ShaderTestGameState : public GameState {
			public:
				int index;
				string name;
				ShaderTestGameState(int index, string name);
				void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				unsigned int id();
				void init(GameContainer* container, StateBasedGame* game);
				void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				void render(GameContainer* container, StateBasedGame* game, Renderer* g);
				virtual ~ShaderTestGameState();
		};

		class ARK2D_API ShaderTest : public StateBasedGame {
			public:
				unsigned int shaderIndex;
				ARK::Util::Containers::Vector<ARK::Graphics::Shader*> shaders;
				
				ShaderTest();
				virtual void initStates(GameContainer* container);
				virtual void update(GameContainer* container, GameTimer* timer);
				virtual void render(GameContainer* container, Renderer* g);
				virtual void resize(GameContainer* container, int width, int height);

				static int start();

				virtual ~ShaderTest();
		};

	}
}



#endif /* ARK_TESTS_SHADERTEST_H_ */
