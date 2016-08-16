/*
 * GamepadsTest.cpp
 *
 *  Created on: May 27 2014
 *      Author: ashleygwinnell
 */

#include "GamepadsTest.h"

#include "../Core/Controls/Gamepad.h"
#include "../Util/Log.h"
#include "../UI/CheckBox.h"
#include "../UI/Button.h"

namespace ARK {
	namespace Tests {

		ARK::Tests::GamepadsTest* gamepadsTest = NULL;

		GamepadConfigureGameState::GamepadConfigureGameState(unsigned int stateId): 
			GameState(),
			m_stateId(stateId),
			m_mappingString(""),
			m_returnToState(NULL) {

		}
		void GamepadConfigureGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { 

			ARK::Core::Controls::Gamepad* gamepad = ARK2D::getInput()->getGamepads()->at(m_gamepadIndex);

			for(signed int i = 0; i < ARK::Core::Controls::Gamepad::s_gamepadMapping->size(); ++i) {
				GamepadMapping* map = &ARK::Core::Controls::Gamepad::s_gamepadMapping->at(i);
				if (map->vendorId == gamepad->vendorId && 
					map->productId == gamepad->productId && 
					map->name == gamepad->name 
					) {
					ARK2D::getLog()->e("Erasing configuration of controller before reconfiguring.");
					ARK::Core::Controls::Gamepad::s_gamepadMapping->erase(ARK::Core::Controls::Gamepad::s_gamepadMapping->begin() + i);
					break;
				}
			}

			m_mapping = GamepadMapping();
			m_mapping.name = gamepad->getName();
			m_mapping.vendorId = gamepad->vendorId;
			m_mapping.productId = gamepad->productId;

			m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_ACTIVATE] = -1;
			m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_LTRIGGER] = -1;
			m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_RTRIGGER] = -1;
		}
		void GamepadConfigureGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }

		unsigned int GamepadConfigureGameState::id() { return m_stateId; }
		void GamepadConfigureGameState::init(GameContainer* container, StateBasedGame* game) {
			visible = false;
			m_gamepadIndex = -1;
			m_state = STATE_A; 

			m_axisChangedCooldown = 0.0f;
		}
		void GamepadConfigureGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
			if (m_axisChangedCooldown > 0.0f) {
				m_axisChangedCooldown += timer->getDelta();
				if (m_axisChangedCooldown > 0.5f) {
					m_axisChangedCooldown = 0.0f;
				}
			}

			Input* in = ARK2D::getInput();
			if (in->isKeyPressed(Input::KEY_ESCAPE) || in->isKeyPressed(Input::KEY_BACKSPACE)) {
				//gamepadsTest->enterState((unsigned int) 0); 
				returnToStateStatic(this);
				return;
			}
			if (in->isKeyPressed(Input::KEY_ENTER)) {
				switch(m_state) {
					case STATE_A: { m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_A] = -1; break; }
					case STATE_B: {	m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_B] = -1; break; }
					case STATE_X: { m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_X] = -1; break; }
					case STATE_Y: { m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_Y] = -1; break; }
					case STATE_DPAD_UP: { m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::DPAD_UP] = -1; break; }
					case STATE_DPAD_DOWN: { m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::DPAD_DOWN] = -1; break; }
					case STATE_DPAD_LEFT: { m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::DPAD_LEFT] = -1; break; }
					case STATE_DPAD_RIGHT: { m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::DPAD_RIGHT] = -1; break; }
					case STATE_LBUMPER: { m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_LBUMPER] = -1; break; }
					case STATE_RBUMPER: { m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_RBUMPER] = -1; break; }
					case STATE_L3: { m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_L3] = -1; break; }
					case STATE_R3: { m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_R3] = -1; break; }
					case STATE_LSTICK_X: { m_mapping.axes[(unsigned int) ARK::Core::Controls:: Gamepad::ANALOG_STICK_1_X] = -1; break; }
					case STATE_LSTICK_Y: { m_mapping.axes[(unsigned int) ARK::Core::Controls::Gamepad::ANALOG_STICK_1_Y] = -1; break; }
					case STATE_RSTICK_X: { m_mapping.axes[(unsigned int) ARK::Core::Controls::Gamepad::ANALOG_STICK_2_X] = -1; break; }
					case STATE_RSTICK_Y: { m_mapping.axes[(unsigned int) ARK::Core::Controls::Gamepad::ANALOG_STICK_2_Y] = -1; break; }
					case STATE_LTRIGGER: { m_mapping.axes[(unsigned int) ARK::Core::Controls::Gamepad::TRIGGER_1] = -1; break; }
					case STATE_RTRIGGER: { m_mapping.axes[(unsigned int) ARK::Core::Controls::Gamepad::TRIGGER_2] = -1; break; }
					case STATE_BACK: { m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_BACK] = -1; break; }
					case STATE_START: { m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_START] = -1; break; }
				}
				m_state++;
				stateChanged();
			}
		}
		void GamepadConfigureGameState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
			r->setDrawColor(Color::white);
			r->setFont(r->getDefaultFont());

			r->drawString("Configuring controller...", 20, 20);

			

			float commandX = container->getWidth() * 0.5f;
			float commandY = container->getHeight() * 0.3f;
			float commandScale = 4.0f;

			if (m_axisChangedCooldown > 0.0f) {
				r->drawString("processing...", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, 1.0f);
				return;
			}
			ARK::Core::Controls::Gamepad* p1 = ARK2D::getInput()->getGamepadByIndex(m_gamepadIndex);
			GamepadsTestGameState::renderGamepad(p1, container->getWidth()*0.5f, container->getHeight()*0.7f);

			r->drawString("Hit ENTER to skip this button/axis!", container->getWidth()-30, container->getHeight()-30, Renderer::ALIGN_RIGHT, Renderer::ALIGN_BOTTOM, 0.0, 1.0f);

			switch(m_state) {
				case STATE_A: {
					r->drawString("Press A", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_B: {
					r->drawString("Press B", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_X: {
					r->drawString("Press X", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_Y: {
					r->drawString("Press Y", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_DPAD_UP: {
					r->drawString("Press DPAD-UP", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_DPAD_DOWN: {
					r->drawString("Press DPAD-DOWN", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_DPAD_LEFT: {
					r->drawString("Press DPAD-LEFT", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_DPAD_RIGHT: {
					r->drawString("Press DPAD-RIGHT", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_LBUMPER: {
					r->drawString("Press LEFT BUMPER", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_RBUMPER: {
					r->drawString("Press RIGHT BUMPER", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_LTRIGGER: {
					r->drawString("Move LEFT TRIGGER", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_RTRIGGER: {
					r->drawString("Move RIGHT TRIGGER", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_LSTICK_X: {
					r->drawString("Move LEFT STICK: left and right", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_LSTICK_Y: {
					r->drawString("Move LEFT STICK: up and down", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_RSTICK_X: {
					r->drawString("Move RIGHT STICK: left and right", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_RSTICK_Y: {
					r->drawString("Move RIGHT STICK: up and down", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_L3: {
					r->drawString("Press LEFT STICK in", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_R3: {
					r->drawString("Press RIGHT STICK in", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_BACK: {
					r->drawString("Press BACK / SELECT", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
				case STATE_START: {
					r->drawString("Press START", commandX, commandY, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER, 0.0, commandScale);
					break;
				}
			}

			if (m_returnToState != NULL) {
				r->drawString("BACKSPACE TO GO BACK", 30, container->getHeight() - 30, Renderer::ALIGN_LEFT, Renderer::ALIGN_BOTTOM, 0.0f, 2.0f);	
			}
		}
		void GamepadConfigureGameState::alertMappingString(GamepadConfigureGameState* gs) {
			ErrorDialog::createAndShow(gs->m_mappingString);
		}
		void GamepadConfigureGameState::returnToStateStatic(GamepadConfigureGameState* gs) {
			// go back to controller state to test everything.
			if (gs->m_returnToState != NULL) {
				ARK2D::getLog()->i("entering state...");
                StateBasedGame* game = dynamic_cast<StateBasedGame*>(ARK2D::getGame());
                if (game != NULL) {
                    game->enterState(gs->m_returnToState);
                }
			} else {
				ARK2D::getLog()->w("entering state - NULL... ");
			}
		}
		void GamepadConfigureGameState::stateChanged() {
			ARK2D::getLog()->w(StringUtil::append("state: ", m_state));
			if (m_state == STATE_END) {
				// add config to ark2d.
				m_mapping.toInverse();
				string str = m_mapping.toString();
				m_mappingString = str;
				ARK2D::getLog()->i(StringUtil::append("mapping: ", m_mappingString));

				
				//Dialog::openInputDialog(0, "config:", m_mapping.toString());
				ARK2D::getLog()->i("pushing...");
				ARK::Core::Controls::Gamepad::s_gamepadMapping->push_back(m_mapping);

				// Alert this on the main thread. 
				//ARK2D::getGame()->getTimeline()->staticEvent( (void*) this, (void*) &GamepadConfigureGameState::alertMappingString, 0.0f);
				
				//ARK2D::getGame()->getTimeline()->staticEvent( (void*) this, (void*) &GamepadConfigureGameState::returnToStateStatic, 0.0f);

				alertMappingString(this);
				returnToStateStatic(this);
				// todo: remove existing mapping or something?
			
				
			}
		}
		void GamepadConfigureGameState::gamepadConnected(ARK::Core::Controls::Gamepad* gamepad) {
			ARK2D::getLog()->i(StringUtil::append("gamepad connected: ", gamepad->getId()));
		}
		void GamepadConfigureGameState::gamepadDisconnected(ARK::Core::Controls::Gamepad* gamepad) {
			ARK2D::getLog()->i(StringUtil::append("gamepad disconnected: ", gamepad->getId()));
		}
		void GamepadConfigureGameState::buttonPressed(ARK::Core::Controls::Gamepad* gamepad, unsigned int button) {
			
		}
		void GamepadConfigureGameState::buttonReleased(ARK::Core::Controls::Gamepad* gamepad, unsigned int button) {
			bool done = false;

			if (m_axisChangedCooldown > 0.0) { return; }
			if (gamepad != ARK2D::getInput()->getGamepadByIndex(m_gamepadIndex)) { 
				ARK2D::getLog()->w("Got input from wrong gamepad.");
				return; 
			}

			switch(m_state) {
				case STATE_A: { 
					m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_A] = button;
					done = true;
					break;
				}
				case STATE_B: {
					m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_B] = button;
					done = true;
					break;
				}
				case STATE_X: {
					m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_X] = button;
					done = true;
					break;
				}
				case STATE_Y: {
					m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_Y] = button;
					done = true;
					break;
				}
				case STATE_DPAD_UP: {
					m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::DPAD_UP] = button;
					done = true;
					break;
				}
				case STATE_DPAD_DOWN: {
					m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::DPAD_DOWN] = button;
					done = true;
					break;
				}
				case STATE_DPAD_LEFT: {
					m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::DPAD_LEFT] = button;
					done = true;
					break;
				}
				case STATE_DPAD_RIGHT: {
					m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::DPAD_RIGHT] = button;
					done = true;
					break;
				}
				case STATE_LBUMPER: {
					m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_LBUMPER] = button;
					done = true;
					break;
				}
				case STATE_RBUMPER: {
					m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_RBUMPER] = button;
					done = true;
					break;
				}
				case STATE_L3: {
					m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_L3] = button;
					done = true;
					break;
				}
				case STATE_R3: {
					m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_R3] = button;
					done = true;
					break;
				}
				case STATE_BACK: {
					m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_BACK] = button;
					done = true;
					break;
				}
				case STATE_START: {
					m_mapping.buttons[(unsigned int) ARK::Core::Controls::Gamepad::BUTTON_START] = button;
					done = true;
					break;
				}

			}

			if (done) {
				m_state++;
				stateChanged();
			}

		}
		void GamepadConfigureGameState::axisMoved(ARK::Core::Controls::Gamepad* gamepad, unsigned int axis, float value) {
			if (m_axisChangedCooldown > 0.0) { return; }

			if (gamepad != ARK2D::getInput()->getGamepadByIndex(m_gamepadIndex)) { 
				ARK2D::getLog()->w("Got input from wrong gamepad.");
				return; 
			}
			if (value > -0.2f && value < 0.2f) { return; }

			bool done = false;
			switch(m_state) {
				case STATE_LSTICK_X: { 
					m_mapping.axes[(unsigned int) ARK::Core::Controls::Gamepad::ANALOG_STICK_1_X] = axis;
					done = true;
					break;
				}
				case STATE_LSTICK_Y: { 
					m_mapping.axes[(unsigned int) ARK::Core::Controls::Gamepad::ANALOG_STICK_1_Y] = axis;
					done = true;
					break;
				}
				case STATE_RSTICK_X: { 
					m_mapping.axes[(unsigned int) ARK::Core::Controls::Gamepad::ANALOG_STICK_2_X] = axis;
					done = true;
					break;
				}
				case STATE_RSTICK_Y: { 
					m_mapping.axes[(unsigned int) ARK::Core::Controls::Gamepad::ANALOG_STICK_2_Y] = axis;
					done = true;
					break;
				}
				case STATE_LTRIGGER: { 
					m_mapping.axes[(unsigned int) ARK::Core::Controls::Gamepad::TRIGGER_1] = axis;
					done = true;
					break;
				}
				case STATE_RTRIGGER: { 
					m_mapping.axes[(unsigned int) ARK::Core::Controls::Gamepad::TRIGGER_2] = axis;
					done = true;
					break;
				}
			}

			if (done) {
				m_axisChangedCooldown = 0.01f;
				m_state++;
				stateChanged();
			}
		}

        bool GamepadConfigureGameState::keyPressed(unsigned int key) { return false;}
        bool GamepadConfigureGameState::keyReleased(unsigned int key) { return false; }
        bool GamepadConfigureGameState::mouseMoved(int x, int y, int oldx, int oldy) { return false; }
		GamepadConfigureGameState::~GamepadConfigureGameState() {

		}








		GamepadsTestGameState::GamepadsTestGameState(): 
			GameState(),
			m_returnToState(NULL) {

		}
		void GamepadsTestGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { }
		void GamepadsTestGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }

		unsigned int GamepadsTestGameState::id() {
			return 0;
		}

		void alertButtonsCheckboxEvent() {

		}
		void alertAxesCheckboxEvent() {

		}
		void GamepadsTestGameState::returnToStateStatic(GamepadsTestGameState* gs) {
			// go back to controller state to test everything.
			if (gs->m_returnToState != NULL) {
				ARK2D::getLog()->i("entering state...");
                StateBasedGame* game = dynamic_cast<StateBasedGame*>(ARK2D::getGame());
                if (game != NULL) {
                    game->enterState(gs->m_returnToState);
                }
			} else {
				ARK2D::getLog()->w("entering state - NULL... ");
			}
		}

		void configureButtonEvent() {
			GamepadsTestGameState* test = dynamic_cast<GamepadsTestGameState*>( gamepadsTest->getState(0) );
			GamepadConfigureGameState* configure = dynamic_cast<GamepadConfigureGameState*>( gamepadsTest->getState(1) );
			configure->m_gamepadIndex = test->m_gamepadIndex;
			configure->m_state = GamepadConfigureGameState::STATE_A;
			gamepadsTest->enterState(configure);
		}
		void removeConfigurationButtonEvent() {
			GamepadsTestGameState* test = dynamic_cast<GamepadsTestGameState*>( gamepadsTest->getState(0) );
			Gamepad* thispad = ARK2D::getInput()->getGamepadByIndex(test->m_gamepadIndex);

			bool removed = false;
			vector<GamepadMapping>* map = ARK::Core::Controls::Gamepad::s_gamepadMapping;
			for(unsigned int i = 0; i < map->size(); i++) {
				if (map->at(i).vendorId == thispad->vendorId && 
					map->at(i).productId == thispad->productId) {
					map->erase(map->begin() + i);
					removed = true;
					break;
				}
			}

			if (removed) {
				ARK2D::getLog()->e("removed mapping");
			}
		}


		void GamepadsTestGameState::init(GameContainer* container, StateBasedGame* game) {
			visible = false;
			m_gamepadIndex = 0;
			
			m_alertButtons = new CheckBox();
			m_alertButtons->setMargin(10);
			m_alertButtons->setChecked(false);
			m_alertButtons->setStateChangedEvent((void*) &alertButtonsCheckboxEvent);
			m_alertButtons->transform.position.set(container->getWidth() - 50, 50);
			m_alertButtons->pivot.set(1.0f, 0.0f);

			addChild(m_alertButtons);

			m_alertAxes = new CheckBox();
			m_alertAxes->setMargin(10);
			m_alertAxes->setChecked(false);
			m_alertAxes->setStateChangedEvent((void*) &alertAxesCheckboxEvent);
			m_alertAxes->transform.position.set(container->getWidth() - 50, 100);
			m_alertAxes->pivot.set(1.0f, 0.0f);
			addChild(m_alertAxes);

			

			

            m_autoConfig = new ARK::UI::Button();
			m_autoConfig->setText("Configure");
			m_autoConfig->setSize(100, 30);
			m_autoConfig->setEvent((void*) &configureButtonEvent);
			m_autoConfig->setMargin(10); 
			m_autoConfig->transform.position.set(container->getWidth() - 85, 150);
			m_autoConfig->pivot.set(0.5, 0.5f);
			addChild(m_autoConfig);

			m_removeConfig = new ARK::UI::Button();
			m_removeConfig->setText("Remove Configuration");
			m_removeConfig->setSize(100, 30);
			m_removeConfig->setEvent((void*) &removeConfigurationButtonEvent);
			m_removeConfig->setMargin(10); 
			m_removeConfig->transform.position.set(container->getWidth() - 85, 200);
			m_removeConfig->pivot.set(0.5, 0.5f);
			addChild(m_removeConfig);
 
			// scene->addChild(m_alertButtons);
			// scene->addChild(m_alertAxes);
			// scene->addChild(m_autoConfig);
		}
		void GamepadsTestGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

			Input* i = ARK2D::getInput();

			if (i->isKeyPressed(Input::KEY_BACKSPACE)) {
				returnToStateStatic(this);
			}
 
			if (i->getGamepads()->size() > 0) {
				ARK::Core::Controls::Gamepad* p1 = i->getGamepads()->at(0);
				if (p1->isButtonPressed(ARK::Core::Controls::Gamepad::DPAD_UP) || p1->isButtonPressed(ARK::Core::Controls::Gamepad::BUTTON_ACTIVATE)) {
					//m_sound->play();
				} 

				if (p1->isButtonPressed(ARK::Core::Controls::Gamepad::BUTTON_START)) {
					ARK2D::getLog()->i("START PRESSED");
				}

				if (p1->isButtonPressed(ARK::Core::Controls::Gamepad::BUTTON_A)) {

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

		void GamepadsTestGameState::renderGamepad(ARK::Core::Controls::Gamepad* p1, float rootX, float rootY)
		{
			if (p1 == NULL) { 
			 return; }
			// rootX/Y is the center C/Y.
			// make rootX the leftest X;
			rootX -= 400;
			rootY -= 150;
			Renderer* r = ARK2D::getRenderer();
			// left stick
			float x = p1->getAxisValue(ARK::Core::Controls::Gamepad::ANALOG_STICK_1_X);
			float y = p1->getAxisValue(ARK::Core::Controls::Gamepad::ANALOG_STICK_1_Y);// axes.at(1)->value;
			float cx = rootX + 300.0f;
			float cy = rootY + 150; 
			float rd = 60.0f;
			r->setDrawColor(Color::white);
			r->drawCircle(cx, cy, (int) rd, (int) rd);
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::BUTTON_L3)) { r->setDrawColor(Color::red); }
			r->fillRect(cx + (rd*x) - 10, cy + (rd*y) - 10, 20, 20);

			// right stick
			x = p1->getAxisValue(ARK::Core::Controls::Gamepad::ANALOG_STICK_2_X); //p1->axes.at(2)->value;
			y = p1->getAxisValue(ARK::Core::Controls::Gamepad::ANALOG_STICK_2_Y); //p1->axes.at(3)->value;
			cx = rootX + 500.0f;
			cy = rootY + 150; 
			r->setDrawColor(Color::white);
			r->drawCircle(cx, cy, (int) rd, (int)rd);
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::BUTTON_R3)) { r->setDrawColor(Color::red); }
			r->fillRect(cx + (rd*x) - 10, cy + (rd*y) - 10, 20, 20);
			
			// reset color 
			r->setDrawColor(Color::white);
	 
			// left trigger  
			float trigger1 = p1->getAxisValue(ARK::Core::Controls::Gamepad::TRIGGER_1); //(p1->axes.at(4)->value + 1.0f)/2.0f;
			r->drawRect(rootX + 200, rootY, 100, 20); 
			r->fillRect(rootX + 200, rootY, int(100 * trigger1), 20);  

			// right trigger 
			float trigger2 = p1->getAxisValue(ARK::Core::Controls::Gamepad::TRIGGER_2);//(p1->axes.at(5)->value + 1.0f)/2.0f;
			r->drawRect(rootX + 500, rootY, 100, 20);
			r->fillRect(rootX + 500, rootY, int(100 * trigger2), 20);


			// left bumper
			r->setDrawColor(Color::white);
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::BUTTON_LBUMPER)) { r->setDrawColor(Color::red); }
			r->fillRect(rootX + 200, rootY+30, 50, 20);

			// right bumper
			r->setDrawColor(Color::white);
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::BUTTON_RBUMPER)) { r->setDrawColor(Color::red); }
			r->fillRect(rootX + 550, rootY+30, 50, 20); 


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
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::DPAD_UP)) { r->setDrawColor(Color::red); }
			r->fillRect(rootX + 200 - 10, rootY+200, 20, 20); // up

			r->setDrawColor(Color::white);
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::DPAD_LEFT)) { r->setDrawColor(Color::red); }
			r->fillRect(rootX + 175 - 10, rootY+225, 20, 20); // left

		 	r->setDrawColor(Color::white);
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::DPAD_RIGHT)) { r->setDrawColor(Color::red); }
			r->fillRect(rootX + 225 - 10, rootY+225, 20, 20); // right
		
			r->setDrawColor(Color::white);
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::DPAD_DOWN)) { r->setDrawColor(Color::red); }
			r->fillRect(rootX + 200 - 10, rootY+250, 20, 20); // down

			// middle buttons
			r->setDrawColor(Color::white);
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::BUTTON_BACK)) { r->setDrawColor(Color::red); }
			r->fillRect(rootX + 350 - 10, rootY+70, 20, 20);

			r->setDrawColor(Color::white);
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::BUTTON_ACTIVATE)) { r->setDrawColor(Color::green); }
			r->fillRect(rootX + 400 - 10, rootY+70, 20, 20);

			r->setDrawColor(Color::white);
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::BUTTON_START)) { r->setDrawColor(Color::red); }
			r->fillRect(rootX + 450 - 10, rootY+70, 20, 20);


			// A button
			r->setDrawColor(Color::white);
			r->drawCircle(rootX + 600.0f, rootY+250.0f, 20, 20); 
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::BUTTON_A)) { r->setDrawColor(Color::green); }
			r->drawCircle(rootX + 600.0f, rootY+250.0f, 20, 20); 
			
			// B button
			r->setDrawColor(Color::white);
			r->drawCircle(rootX + 625.0f, rootY+225.0f, 20, 20); 
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::BUTTON_B)) { r->setDrawColor(Color::red); }
			r->drawCircle(rootX + 625.0f, rootY+225.0f, 20, 20); 

			// X button
			r->setDrawColor(Color::white);
			r->drawCircle(rootX + 575.0f, rootY+225.0f, 20, 20); 
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::BUTTON_X)) { r->setDrawColor(Color::blue); }
			r->drawCircle(rootX + 575.0f, rootY+225.0f, 20, 20); 

			// Y button
			r->setDrawColor(Color::white);
			r->drawCircle(rootX + 600.0f, rootY+200.0f, 20, 20); 
			if (p1->isButtonDown(ARK::Core::Controls::Gamepad::BUTTON_Y)) { r->setDrawColor(Color::yellow); }
			r->drawCircle(rootX + 600.0f, rootY+200.0f, 20, 20); 
		}
		void GamepadsTestGameState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
			r->setDrawColor(Color::white);
			r->setFont(r->getDefaultFont());

			Input* i = ARK2D::getInput(); 
			if (i->getGamepads()->size() > 0) {  
				ARK::Core::Controls::Gamepad* p1 = i->getGamepads()->at(m_gamepadIndex);

				r->setDrawColor(Color::white);  
				r->setFont(r->getDefaultFont());
				r->drawString(StringUtil::append("gamepads connected: ", i->getGamepads()->size()), 30, 20);
				r->drawString(StringUtil::append("gamepad: ", m_gamepadIndex), 30, 40);
				
				string name = p1->getName();
				if (p1->isPS4Controller()) { name += " -- PS4 Controller!"; }
				r->drawString(StringUtil::append("name: ", name), 30, 70);
				r->drawString(StringUtil::append("vendor id: ", p1->vendorId), 30, 90);
				r->drawString(StringUtil::append("product id: ", p1->productId), 30, 110);

				r->drawString(StringUtil::append("num buttons: ", p1->numButtons), 30, 140);
				r->drawString(StringUtil::append("num axes: ", p1->numAxes), 30, 160);
				r->drawString(StringUtil::append("num axes 2: ", p1->axes.size()), 30, 180);
				r->drawString(StringUtil::append("has configuration: ", (p1->isConfigured()?"true":"false")), 30, 210, -1, -1);
				r->drawString(StringUtil::append("shared trigger axis: ", (p1->m_sharedTriggerAxis?"true":"false")), 30, 220, -1, -1);
				//r->drawString(StringUtil::append("axis 1: ", p1->axes.at(0)->value), 30, 180);
				//r->drawString(StringUtil::append("axis 2: ", p1->axes.at(1)->value), 30, 210);
				renderGamepad(p1, container->getWidth()*0.5f, container->getHeight()*0.7f);

				//m_alertButtons->render();
				//m_alertAxes->render();
				//m_autoConfig->render();

				r->drawString("Alert Buttons: ", container->getWidth() - 80, 50,  Renderer::ALIGN_RIGHT, Renderer::ALIGN_CENTER);	
				r->drawString("Alert Axes: ",    container->getWidth() - 80, 100, Renderer::ALIGN_RIGHT, Renderer::ALIGN_CENTER);	
		 
			} else {
				r->setDrawColor(Color::white);
				r->drawString("nothing gamepads connected...", 30, 120);	
				//r->fillRect(100,100,100,100);

			}

			if (m_returnToState != NULL) {
				r->drawString("BACKSPACE TO GO BACK", 30, container->getHeight() - 30, Renderer::ALIGN_LEFT, Renderer::ALIGN_BOTTOM, 0.0f, 2.0f);	
			}

            renderChildren();
		}

		bool GamepadsTestGameState::keyPressed(unsigned int key) {
            GameState::keyPressed(key);
            return false;
		}
		bool GamepadsTestGameState::keyReleased(unsigned int key) {
			GameState::keyReleased(key);
            return false;
		}
		bool GamepadsTestGameState::mouseMoved(int x, int y, int oldx, int oldy) {
			GameState::mouseMoved(x,y,oldx,oldy);
            return false;
		}

		void GamepadsTestGameState::gamepadConnected(ARK::Core::Controls::Gamepad* gamepad) {
			ARK2D::getLog()->i(StringUtil::append("gamepad connected: ", gamepad->getId()));
		}
		void GamepadsTestGameState::gamepadDisconnected(ARK::Core::Controls::Gamepad* gamepad) {
			ARK2D::getLog()->i(StringUtil::append("gamepad disconnected: ", gamepad->getId()));
		}
		void GamepadsTestGameState::buttonPressed(ARK::Core::Controls::Gamepad* gamepad, unsigned int button) {
			if (m_alertButtons->isChecked()) {
				ErrorDialog::createAndShow(StringUtil::append("button pressed: ", button));
			}
		}
		void GamepadsTestGameState::buttonReleased(ARK::Core::Controls::Gamepad* gamepad, unsigned int button) {

		}
		void GamepadsTestGameState::axisMoved(ARK::Core::Controls::Gamepad* gamepad, unsigned int axis, float value) {
			if (m_alertAxes->isChecked()) { 
				if (value > 0.75f || value < -0.75f) {
					ErrorDialog::createAndShow(StringUtil::append("axis moved: ", axis));
				}
			}
		}

		GamepadsTestGameState::~GamepadsTestGameState() {

		}











		GamepadsTest::GamepadsTest(): StateBasedGame("Gamepads Test") {

		}
		void GamepadsTest::initStates(GameContainer* container) {
			ARK2D::getLog()->setFilter(Log::TYPE_INFORMATION);

			GamepadsTestGameState* testState = new GamepadsTestGameState();
			GamepadConfigureGameState* configureState = new GamepadConfigureGameState(1);

			testState->m_returnToState = testState;
			configureState->m_returnToState = testState;

			addState(testState);
			addState(configureState);
			enterState(testState);
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
		void GamepadsTest::render() {
			StateBasedGame::render();
		}
		void GamepadsTest::render(GameContainer* container, Renderer* r) {
			//StateBasedGame::render(container, r);
		}
		void GamepadsTest::resize(GameContainer* container, int width, int height) {
			StateBasedGame::resize(container, width, height);
		}
		GamepadsTest::~GamepadsTest() {

		}

		

		int GamepadsTest::start() {
			gamepadsTest = new ARK::Tests::GamepadsTest();
			GameContainer* container = new GameContainer(*gamepadsTest, 800, 600, 32, false);
			container->setClearColor(Color::black);
			container->start();
			return 0;
		}



	}
}


