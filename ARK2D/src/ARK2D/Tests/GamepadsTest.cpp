/*
 * GamepadsTest.cpp
 *
 *  Created on: May 27 2014
 *      Author: ashleygwinnell
 */

#include "GamepadsTest.h"
#include "../../ARK.h"

namespace ARK {
	namespace Tests {


		GamepadsTestGameState::GamepadsTestGameState(): GameState() {

		}

		void GamepadsTestGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { }
		void GamepadsTestGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }

		unsigned int GamepadsTestGameState::id() {
			return 0;
		}

		void GamepadsTestGameState::init(GameContainer* container, StateBasedGame* game) {
			m_gamepadIndex = 0;
		}
		void GamepadsTestGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

			Input* i = ARK2D::getInput();
 
			if (i->getGamepads()->size() > 0) {
				Gamepad* p1 = i->getGamepads()->at(0);
				if (p1->isButtonPressed(Gamepad::DPAD_UP) || p1->isButtonPressed(Gamepad::BUTTON_ACTIVATE)) {
					//m_sound->play();
				} 

				if (p1->isButtonPressed(Gamepad::BUTTON_A)) {

					ARK2D::getLog()->v("Printing Axes");
					vector<GamepadAxis* >* axes = &p1->axes;
					for(unsigned int j = 0; j < axes->size(); j++) {
						GamepadAxis* axis = axes->at(j);
						
						string str = "Index: ";
						str += Cast::toString<unsigned int>(j);
						str += ". Axis Id: ";
						str += Cast::toString<unsigned int>(axis->id);
						str += ". Value: "; 
						str += Cast::toString<float>(axis->value);
						str += ".";

						ARK2D::getLog()->i(str);
					}
				}

				if (i->isKeyPressed(Input::KEY_RIGHT)) {
					m_gamepadIndex++;
					if (m_gamepadIndex >= (signed int) i->getGamepads()->size()) {
						m_gamepadIndex = 0;
					}
				}
				if (i->isKeyPressed(Input::KEY_LEFT)) {
					m_gamepadIndex--;
					if (m_gamepadIndex < 0) {
						m_gamepadIndex = i->getGamepads()->size() - 1;
					}
				}

			}
			
		}
		void GamepadsTestGameState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
			r->setDrawColor(Color::white);
			r->setFont(r->getDefaultFont());

			Input* i = ARK2D::getInput(); 
			if (i->getGamepads()->size() > 0) {  
				Gamepad* p1 = i->getGamepads()->at(m_gamepadIndex);

				r->setDrawColor(Color::white);  
				r->setFont(r->getDefaultFont());
				r->drawString(StringUtil::append("gamepads connected: ", i->getGamepads()->size()), 230, 20);
				r->drawString(StringUtil::append("gamepad: ", m_gamepadIndex), 230, 40);
				
				r->drawString(StringUtil::append("name: ", p1->getName()), 230, 70);
				r->drawString(StringUtil::append("vendor id: ", p1->vendorId), 230, 90);
				r->drawString(StringUtil::append("product id: ", p1->productId), 230, 110);

				r->drawString(StringUtil::append("num buttons: ", p1->numButtons), 230, 140);
				r->drawString(StringUtil::append("num axes: ", p1->numAxes), 230, 160);
				r->drawString(StringUtil::append("num axes 2: ", p1->axes.size()), 230, 180);
				//r->drawString(StringUtil::append("axis 1: ", p1->axes.at(0)->value), 30, 180);
				//r->drawString(StringUtil::append("axis 2: ", p1->axes.at(1)->value), 30, 210);
				
		 		float rootY = 250.0f;
				// left stick
				float x = p1->getAxisValue(Gamepad::ANALOG_STICK_1_X);
				float y = p1->getAxisValue(Gamepad::ANALOG_STICK_1_Y);// axes.at(1)->value;
				float cx = 200.0f;
				float cy = rootY + 150; 
				float rd = 60.0f;
				r->setDrawColor(Color::white);
				r->drawCircle(cx, cy, (int) rd, (int) rd);
				if (p1->isButtonDown(Gamepad::BUTTON_L3)) { r->setDrawColor(Color::red); }
				r->fillRect(cx + (rd*x) - 10, cy + (rd*y) - 10, 20, 20);

				// right stick
				x = p1->getAxisValue(Gamepad::ANALOG_STICK_2_X); //p1->axes.at(2)->value;
				y = p1->getAxisValue(Gamepad::ANALOG_STICK_2_Y); //p1->axes.at(3)->value;
				cx = 400.0f;
				cy = rootY + 150; 
				r->setDrawColor(Color::white);
				r->drawCircle(cx, cy, (int) rd, (int)rd);
				if (p1->isButtonDown(Gamepad::BUTTON_R3)) { r->setDrawColor(Color::red); }
				r->fillRect(cx + (rd*x) - 10, cy + (rd*y) - 10, 20, 20);
				
				// reset color 
				r->setDrawColor(Color::white);
		 
				// left trigger  
				float trigger1 = p1->getAxisValue(Gamepad::TRIGGER_1); //(p1->axes.at(4)->value + 1.0f)/2.0f;
				r->drawRect(100, rootY, 100, 20); 
				r->fillRect(100, rootY, int(100 * trigger1), 20);  

				// right trigger 
				float trigger2 = p1->getAxisValue(Gamepad::TRIGGER_2);//(p1->axes.at(5)->value + 1.0f)/2.0f;
				r->drawRect(400, rootY, 100, 20);
				r->fillRect(400, rootY, int(100 * trigger2), 20);


				// left bumper
				r->setDrawColor(Color::white);
				if (p1->isButtonDown(Gamepad::BUTTON_LBUMPER)) { r->setDrawColor(Color::red); }
				r->fillRect(100, rootY+30, 50, 20);

				// right bumper
				r->setDrawColor(Color::white);
				if (p1->isButtonDown(Gamepad::BUTTON_RBUMPER)) { r->setDrawColor(Color::red); }
				r->fillRect(450, rootY+30, 50, 20); 


				//r->drawString(StringUtil::append("logical min: ", p1->axes.at(0)->logicalMin), 30, 390);
				//r->drawString(StringUtil::append("logical max: ", p1->axes.at(0)->logicalMax), 30, 420);
				//r->drawString(StringUtil::append("null state?: ", p1->axes.at(0)->isHatSwitch), 30, 450);
				//
				//r->drawString(StringUtil::append("logical min: ", p1->axes.at(4)->logicalMin), 200, 30);
				//r->drawString(StringUtil::append("logical max: ", p1->axes.at(4)->logicalMax), 200, 60);
				//r->drawString(StringUtil::append("null state?: ", p1->axes.at(4)->isHatSwitch), 30, 90);
				//r->drawString(StringUtil::append("val: ", p1->axes.at(4)->value), 200, 90);

				
				
				
				// dpad
				r->setDrawColor(Color::white);
				if (p1->isButtonDown(Gamepad::DPAD_UP)) { r->setDrawColor(Color::red); }
				r->fillRect(100 - 10, rootY+200, 20, 20); // up

				r->setDrawColor(Color::white);
				if (p1->isButtonDown(Gamepad::DPAD_LEFT)) { r->setDrawColor(Color::red); }
				r->fillRect(75 - 10, rootY+225, 20, 20); // left

			 	r->setDrawColor(Color::white);
				if (p1->isButtonDown(Gamepad::DPAD_RIGHT)) { r->setDrawColor(Color::red); }
				r->fillRect(125 - 10, rootY+225, 20, 20); // right
			
				r->setDrawColor(Color::white);
				if (p1->isButtonDown(Gamepad::DPAD_DOWN)) { r->setDrawColor(Color::red); }
				r->fillRect(100 - 10, rootY+250, 20, 20); // down

				// middle buttons
				r->setDrawColor(Color::white);
				if (p1->isButtonDown(Gamepad::BUTTON_BACK)) { r->setDrawColor(Color::red); }
				r->fillRect(250 - 10, rootY+70, 20, 20);

				r->setDrawColor(Color::white);
				if (p1->isButtonDown(Gamepad::BUTTON_ACTIVATE)) { r->setDrawColor(Color::green); }
				r->fillRect(300 - 10, rootY+70, 20, 20);

				r->setDrawColor(Color::white);
				if (p1->isButtonDown(Gamepad::BUTTON_START)) { r->setDrawColor(Color::red); }
				r->fillRect(350 - 10, rootY+70, 20, 20);


				// A button
				r->setDrawColor(Color::white);
				r->drawCircle(500.0f, rootY+250.0f, 20, 20); 
				if (p1->isButtonDown(Gamepad::BUTTON_A)) { r->setDrawColor(Color::green); }
				r->drawCircle(500.0f, rootY+250.0f, 20, 20); 
				
				// B button
				r->setDrawColor(Color::white);
				r->drawCircle(525.0f, rootY+225.0f, 20, 20); 
				if (p1->isButtonDown(Gamepad::BUTTON_B)) { r->setDrawColor(Color::red); }
				r->drawCircle(525.0f, rootY+225.0f, 20, 20); 

				// X button
				r->setDrawColor(Color::white);
				r->drawCircle(475.0f, rootY+225.0f, 20, 20); 
				if (p1->isButtonDown(Gamepad::BUTTON_X)) { r->setDrawColor(Color::blue); }
				r->drawCircle(475.0f, rootY+225.0f, 20, 20); 

				// Y button
				r->setDrawColor(Color::white);
				r->drawCircle(500.0f, rootY+200.0f, 20, 20); 
				if (p1->isButtonDown(Gamepad::BUTTON_Y)) { r->setDrawColor(Color::yellow); }
				r->drawCircle(500.0f, rootY+200.0f, 20, 20); 

		 
			} else {
				r->setDrawColor(Color::white);
				r->drawString("nothing gamepads connected...", 30, 120);	
				//r->fillRect(100,100,100,100);

			}
		}

		GamepadsTestGameState::~GamepadsTestGameState() {

		}











		GamepadsTest::GamepadsTest(): StateBasedGame("Gamepads Test") {

		}
		void GamepadsTest::initStates(GameContainer* container) {
			ARK2D::getLog()->setFilter(Log::TYPE_INFORMATION);
			addState(new GamepadsTestGameState());
			enterState((unsigned int) 0);
		} 
		void GamepadsTest::update(GameContainer* container, GameTimer* timer) {
			StateBasedGame::update(container, timer);

			Input* in = ARK2D::getInput();
			if (in->isKeyPressed(Input::KEY_F)) {
				//if (container->getDynamicWidth() == 800) {
				//	container->setSize(1000, 800);
				//} else {
				//	container->setSize(800, 600);
				//}
				container->setFullscreen(!container->isFullscreen());
			}
		}
		void GamepadsTest::render(GameContainer* container, Renderer* r) {
			StateBasedGame::render(container, r);
		}
		void GamepadsTest::resize(GameContainer* container, int width, int height) {
			StateBasedGame::resize(container, width, height);
		}
		GamepadsTest::~GamepadsTest() {

		}

		void GamepadsTest::buttonPressed(unsigned int button) {
			ErrorDialog::createAndShow(StringUtil::append("button pressed: ", button));
		}
		void GamepadsTest::buttonReleased(unsigned int button) { 

		}
		void GamepadsTest::axisMoved(unsigned int axis, float value) { }

		int GamepadsTest::start() {
			ARK::Tests::GamepadsTest* test = new ARK::Tests::GamepadsTest();
			GameContainer* container = new GameContainer(*test, 800, 600, 32, false);
			container->start();
			return 0;
		}



	}
}


