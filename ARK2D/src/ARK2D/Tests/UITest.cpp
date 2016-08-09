/*
 * UITest.cpp
 *
 *  Created on: Mar 23, 2012
 *      Author: ashleygwinnell
 */

#include "UITest.h"
//#include "../../ARK.h"
#include "../Core/GameContainer.h"
#include "../UI/TextField.h"
#include "../UI/ComboBox.h"
#include "../UI/ComboBoxItem.h"
#include "../UI/CheckBox.h"


namespace ARK {
	namespace Tests {


		UITestGameState::UITestGameState(): GameState() {

		}

		void UITestGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { }
		void UITestGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }

		unsigned int UITestGameState::id() {
			return 0;
		}

		void itemChangedEvent();
		void itemChangedEvent() {
			ErrorDialog::createAndShow("Item changed");
		}

		void buttonClickEvent();
		void buttonClickEvent() {
			ErrorDialog::createAndShow("Button clicked");
		}
		void stateChangedEvent();
		void stateChangedEvent() {
			ErrorDialog::createAndShow("Checkbox changed");
		}

		void UITestGameState::init(GameContainer* container, StateBasedGame* game) {
//			Panel* root = new Panel();
//			root->setSize(container->getWidth(), container->getHeight());
//			root->setShowBorder(false);

            SceneNode* root = new SceneNode();
            
			rightPanel = new ScrollPanel();
			rightPanel->setLocation(50, 50);
			rightPanel->setWidth(container->getWidth()/2);
			rightPanel->setHeight(container->getHeight()-100);
			rightPanel->m_layout = ScrollPanel::LAYOUT_FLOW;

			Label* label2 = new Label("A Text Label #2");
			label2->setMargin(10);
			rightPanel->addChild(label2); 

			TextField* textField2 = new TextField();
			textField2->setMargin(10);
			textField2->setSize(200, 30);
			textField2->setText("A Text Field #2");
			rightPanel->addChild(textField2);

			Label* label3 = new Label("A Text Label #2");
			label3->setMargin(10);
			rightPanel->addChild(label3);

			ComboBox* comboBox = new ComboBox();
			comboBox->setMargin(10);
			comboBox->setSize(200, 30);
			comboBox->setItemChangedEvent((void*) &itemChangedEvent);
				ComboBoxItem* comboItemOne = new ComboBoxItem();
				comboItemOne->setText("Item One");
				comboBox->addItem(comboItemOne);
				ComboBoxItem* comboItemTwo = new ComboBoxItem();
				comboItemTwo->setText("Item Two");
				comboBox->addItem(comboItemTwo);
			rightPanel->addChild(comboBox);

			Label* label4 = new Label("A Text Label #3");
			label4->setMargin(10);
			rightPanel->addChild(label4);

			CheckBox* checkBox = new CheckBox();
			checkBox->setMargin(10);
			checkBox->setChecked(false);
			checkBox->setStateChangedEvent((void*) &stateChangedEvent);
			rightPanel->addChild(checkBox);

			CheckBox* checkBox2 = new CheckBox();
			checkBox2->setMargin(10);
			checkBox2->setChecked(true);
			rightPanel->addChild(checkBox2);

			Label* label5 = new Label("A Text Label four is big");
			label5->setMargin(10);
			rightPanel->addChild(label5);

			ARK::UI::Button* button = new ARK::UI::Button();
			button->setText("Click me!");
			button->setSize(50, 50);
			button->setEvent((void*) &buttonClickEvent);
			button->setMargin(10);
			rightPanel->addChild(button); 


			root->addChild(rightPanel);

            addChild(root);
		}

		void UITestGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
			//GameState::update(container, game, timer);
			//Input* i = ARK2D::getInput();
            SceneNode::update();

		}
		void UITestGameState::render(GameContainer* container, StateBasedGame* game, Renderer* g) {
			//GameState::update(container, game. timer);
            SceneNode::render();



		}

		/*void UITestGameState::keyPressed(unsigned int key) {
			GameState::keyPressed(key);
		}
		void UITestGameState::keyReleased(unsigned int key) {
			GameState::keyReleased(key);
		}
*/

		UITestGameState::~UITestGameState() {

		}











		UITest::UITest(): StateBasedGame("UI Test") {

		}
		void UITest::initStates(GameContainer* container) {
			addState(new UITestGameState());
			enterState((unsigned int) 0);
		}
		void UITest::update(GameContainer* container, GameTimer* timer) {
			StateBasedGame::update(container, timer);
		}
		void UITest::render(GameContainer* container, Renderer* g) {
			StateBasedGame::render(container, g);
			g->setDrawColor(Color::white);
			g->drawString("UI Tests", 10, 10);
		}
		void UITest::resize(GameContainer* container, int width, int height) {
			StateBasedGame::resize(container, width, height);
		}
		UITest::~UITest() {

		}
		int UITest::start() {
			ARK::Tests::UITest* test = new ARK::Tests::UITest();
			GameContainer container(*test, 800, 600, 32, false);
			container.start();
			return 0;
		}



	}
}


