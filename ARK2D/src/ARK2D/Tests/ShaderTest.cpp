/*
 * ShaderTest.cpp
 *
 *  Created on: May 16, 2015
 *      Author: ashleygwinnell
 */

#include "ShaderTest.h"
#include "../../ARK.h"

namespace ARK {
	namespace Tests {

		HSVShaderTestGameState::HSVShaderTestGameState(int index, string name): GameState() {
			this->index = index;
			this->name = name;
		}
		void HSVShaderTestGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { }
		void HSVShaderTestGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }
		unsigned int HSVShaderTestGameState::id() { return this->index; }
		void HSVShaderTestGameState::init(GameContainer* container, StateBasedGame* game) {
			texture = Resource::get("ark2d/gamejolt-overlay/icon.png")->asImage();

			shader = new HSVShader();
			shader->load();

			

			m_sliderHue = new ARK::UI::Slider();
			m_sliderHue->setSize(200, 3);
			m_sliderHue->setLocation(20, 100);
			m_sliderHue->setSnapping(false);
			m_sliderHue->setRange(0.0f, 1.0f);
			m_sliderHue->setButtonPosition(0.0f);

			m_sliderSaturation = new ARK::UI::Slider();
			m_sliderSaturation->setSize(200, 3);
			m_sliderSaturation->setLocation(20, 160);
			m_sliderSaturation->setSnapping(false);
			m_sliderSaturation->setRange(0.0f, 1.0f);
			m_sliderSaturation->setButtonPosition(0.5f);

			m_sliderValue = new ARK::UI::Slider();
			m_sliderValue->setSize(200, 3);
			m_sliderValue->setLocation(20, 220);
			m_sliderValue->setSnapping(false);
			m_sliderValue->setRange(0.0f, 1.0f);
			m_sliderValue->setButtonPosition(0.5f);

			m_timer = 0.0f;
		}
		void HSVShaderTestGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
			m_timer += timer->getDelta() * 0.25f;
		}
		void HSVShaderTestGameState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
			r->drawString("HSV Shader Test", 20, 20, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
	
			m_sliderHue->updateValue();
			m_sliderSaturation->updateValue();
			m_sliderValue->updateValue();
			shader->set(m_sliderHue->getActualValue(), m_sliderSaturation->getActualValue(), m_sliderValue->getActualValue());

			r->drawString(StringUtil::appendf("Hue: ", shader->getH()), 20, 60, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
			r->drawString(StringUtil::appendf("Saturation: ", shader->getS()), 20, 120, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
			r->drawString(StringUtil::appendf("Value: ", shader->getV()), 20, 180, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);

			m_sliderHue->render();
			m_sliderSaturation->render();
			m_sliderValue->render();
			showAnyGlErrorAndExitMacro();

			shader->start();
			showAnyGlErrorAndExitMacro();


			r->setDrawColor(Color::white);
			r->setDrawColor(Color::red);
			r->fillCircle(160, 360, 51, 50);

			r->setDrawColor(Color::white);
			r->fillCircle(320, 360, 50, 5);

			r->setDrawColor(Color::green);
			r->fillCircle(480, 360, 50, 4);

			r->setDrawColor(Color::blue);
			r->fillCircle(480, 120, 50, 8);

			shader->stop();
			showAnyGlErrorAndExitMacro();

			shader->start();
			showAnyGlErrorAndExitMacro();

			r->setDrawColor(Color::white);
			texture->drawCenteredScaled(container->getWidth()/2.0f,container->getHeight()/2.0f, 8.0f, 8.0f);
			showAnyGlErrorAndExitMacro();

			shader->stop();
			showAnyGlErrorAndExitMacro();
		}
		bool HSVShaderTestGameState::keyPressed(unsigned int key) {
			if (m_sliderHue->keyPressed(key)) return true;
            if (m_sliderSaturation->keyPressed(key)) return true;
            if (m_sliderValue->keyPressed(key)) return true;
            return false;
		}
		bool HSVShaderTestGameState::keyReleased(unsigned int key) {
            if (m_sliderHue->keyReleased(key)) return true;
            if (m_sliderSaturation->keyReleased(key)) return true;
            if (m_sliderValue->keyReleased(key)) return true;
            return false;
		}
		bool HSVShaderTestGameState::mouseMoved(int x, int y, int oldx, int oldy) {
            if (m_sliderHue->mouseMoved(x, y, oldx, oldy)) return true;
            if (m_sliderSaturation->mouseMoved(x, y, oldx, oldy)) return true;
            if (m_sliderValue->mouseMoved(x, y, oldx, oldy)) return true;
            return false;
		}
		HSVShaderTestGameState::~HSVShaderTestGameState() {

		}


		ShaderTestGameState::ShaderTestGameState(int index, string name): GameState() {
			this->index = index;
			this->name = name;
		}
		void ShaderTestGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

		}
		void ShaderTestGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

		}
		unsigned int ShaderTestGameState::id() { return this->index; }
		void ShaderTestGameState::init(GameContainer* container, StateBasedGame* game) {

		}
		void ShaderTestGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

		}
		void ShaderTestGameState::render(GameContainer* container, StateBasedGame* game, Renderer* g) {
			g->setDrawColor(Color::darker_grey);
			g->fillRect(10,10,container->getWidth()-20, container->getHeight()-20);

			g->setDrawColor(Color::dark_grey);
			g->fillRect(30,30,container->getWidth()-60, container->getHeight()-60);

			g->setDrawColor(Color::grey);
			g->fillRect(50,50,container->getWidth()-100, container->getHeight()-100);

			g->setDrawColor(Color::black_50a);
			g->fillCircleSpikey(container->getWidth()/2, container->getHeight()/2, container->getHeight()/4, 120);
		}
		ShaderTestGameState::~ShaderTestGameState() {

		}

		ShaderTest::ShaderTest():
			StateBasedGame("Shaders Test"),
			shaderIndex(0),
			shaders()
		{

		}
		void ShaderTest::initStates(GameContainer* container) {

			addState( new HSVShaderTestGameState(0, "HSV Shader") );
			
			//addState(new ShaderTestGameState(0, "State One"));
			//addState(new ShaderTestGameState(1, "State Two"));
			//addState(new ShaderTestGameState(2, "State Three"));
			//addState(new ShaderTestGameState(3, "State Four"));

			enterState((unsigned int) 0);
		}
		void ShaderTest::update(GameContainer* container, GameTimer* timer) {
			StateBasedGame::update(container, timer);

			Input* i = ARK2D::getInput();
			if (i->isKeyPressed(Input::KEY_SPACE) || i->isKeyPressed(Input::KEY_ENTER)) {
				enterState(shaderIndex, NULL, NULL);
				shaderIndex++;
				if (shaderIndex >= shaders.size()) {
					shaderIndex = 0;
				}
			}
		}
		void ShaderTest::render(GameContainer* container, Renderer* g) {
			StateBasedGame::render(container, g);
			//g->drawStringCenteredAt(StringUtil::append("",shaderIndex), container->getWidth()/2, container->getHeight()/2);
		}
		void ShaderTest::resize(GameContainer* container, int width, int height) {
			StateBasedGame::resize(container, width, height);
		}
		ShaderTest::~ShaderTest() {

		}

		int ShaderTest::start() {
			ARK::Tests::ShaderTest* test = new ARK::Tests::ShaderTest();
			GameContainer container(*test, 800, 600, 32, false);
			container.start();
			return 0;
		}

	}
}



