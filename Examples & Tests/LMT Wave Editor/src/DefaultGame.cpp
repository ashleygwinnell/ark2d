/*
 * DefaultGame.cpp
 *
 *  Created on: 17 Jan 2011
 *      Author: Ashley
 */

#include "DefaultGame.h"
#include "WaveEditorState.h"
#include "LevelEditorState.h"
#include "EditorEnemy.h"
#include "EditorWave.h"

#include "GamePanel.h"


DefaultGame::DefaultGame(string title):
	StateBasedGame(title),
	m_enemyImages(),
	m_options(NULL)
{

}

void DefaultGame::initStates(GameContainer* container) {

	Image* enemiesImage = new Image(container->getResourcePath() + "data/enemies.png");
	SpriteSheetDescription* enemiesDesc = new SpriteSheetDescription(container->getResourcePath() + "data/enemies.json");

	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy1.png"))->getScaledCopy(2, 2));
	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy2.png"))->getScaledCopy(2, 2));
	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy3.png"))->getScaledCopy(2, 2));
	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy4.png"))->getScaledCopy(2, 2));
	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy5.png"))->getScaledCopy(2, 2));
	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy6.png"))->getScaledCopy(2, 2));
	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy7.png"))->getScaledCopy(2, 2));
	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy8.png"))->getScaledCopy(2, 2));
	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy9.png"))->getScaledCopy(2, 2));
	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy10.png"))->getScaledCopy(2, 2));
	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy11.png"))->getScaledCopy(2, 2));
	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy12.png"))->getScaledCopy(2, 2));
	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy13.png"))->getScaledCopy(2, 2));
	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy14.png"))->getScaledCopy(2, 2));
	m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy15.png"))->getScaledCopy(2, 2));
	//m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy16.png"))->getScaledCopy(2, 2));
	//m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy17.png"))->getScaledCopy(2, 2));
	//m_enemyImages.push_back(enemiesImage->getSubImage(enemiesDesc->getItemByName("enemy18.png"))->getScaledCopy(2, 2));

	m_levelState = new LevelEditorState();
	addState(m_levelState);

	m_waveState = new WaveEditorState();
	addState(m_waveState);


	//enterState(m_waveState);

	m_options = new ScrollPanel();
	m_options->setPadding(10);
	m_options->setSize(800, 700);
	m_options->setTranslating(true);
	m_options->setClipping(true);
	m_options->setVisible(false);
	m_options->setLocation(70, 10);

		m_snapLabel = new Label("Snap pixels:");
		m_snapLabel->setSize(750, 30);
		m_options->add(m_snapLabel);

		m_snapField = new TextField();
		m_snapField->setSize(750, 30);
		m_snapField->setText(20);
		m_snapField->setPadding(5);
		m_options->add(m_snapField);

		m_pathsLocationLabel = new Label("Paths Location: ");
		m_pathsLocationLabel->setSize(750, 30);
		m_options->add(m_pathsLocationLabel);

		m_pathsLocationField = new TextField();
		m_pathsLocationField->setSize(750, 30);
		m_pathsLocationField->setPadding(5);
		//string cwd = FileUtil::getCurrentDirectory() + "\\data\\paths\\";
		//string cwd = container->getResourcePath() + "../../../data" + FileUtil::getSeparator() + "paths" + FileUtil::getSeparator();

		string cwd = container->getResourcePath();
		string newcwd = cwd.substr(0,cwd.find_last_of("/"));
		newcwd = newcwd.substr(0,newcwd.find_last_of("/"));
		newcwd = newcwd.substr(0,newcwd.find_last_of("/"));
		newcwd = newcwd.substr(0,newcwd.find_last_of("/"));
		newcwd += FileUtil::getSeparator() + "data" + FileUtil::getSeparator() + "paths" + FileUtil::getSeparator();

		m_pathsLocationField->setText(newcwd); //m_pathsLocationField->setText("data/paths/");
		m_options->add(m_pathsLocationField);

		m_wavesLocationLabel = new Label("Waves Location: ");
		m_wavesLocationLabel->setSize(750, 30);
		m_options->add(m_wavesLocationLabel);

		m_wavesLocationField = new TextField();
		m_wavesLocationField->setSize(750, 30);
		m_wavesLocationField->setPadding(5);
		//string cwd2 = FileUtil::getCurrentDirectory() + "\\data\\waves\\";
		//string cwd2 = container->getResourcePath() + "../../../data" + FileUtil::getSeparator() + "waves" + FileUtil::getSeparator();

		string cwd2 = container->getResourcePath();
		string newcwd2 = cwd2.substr(0,cwd2.find_last_of("/"));
		newcwd2 = newcwd2.substr(0,newcwd2.find_last_of("/"));
		newcwd2 = newcwd2.substr(0,newcwd2.find_last_of("/"));
		newcwd2 = newcwd2.substr(0,newcwd2.find_last_of("/"));
		newcwd2 += FileUtil::getSeparator() + "data" + FileUtil::getSeparator() + "waves" + FileUtil::getSeparator();

		m_wavesLocationField->setText(newcwd2); //m_pathsLocationField->setText("data/paths/");
		m_options->add(m_wavesLocationField);

		m_renderAllPathsLabel = new Label("Render All Paths: ");
		m_renderAllPathsLabel->setSize(750, 30);
		m_options->add(m_renderAllPathsLabel);

		m_renderAllPathsCheckBox = new CheckBox();
		m_options->add(m_renderAllPathsCheckBox);


	ARK2D::getContainer()->getTimer()->flush();
	//std::cout << "ff" << std::endl;
}


void DefaultGame::update(GameContainer* container, GameTimer* timer) {
	//std::cout << "update" << std::endl;
	StateBasedGame::update(container, timer);

	Input* i = ARK2D::getInput();
	if (i->isKeyPressed(Input::KEY_F1)) {
		unsigned int newStateId = getCurrentStateID() + 1;
		if (newStateId == getStates().size()) {
			newStateId = 0;
		}
		enterState(newStateId);
	}
}

void DefaultGame::render(GameContainer* container, Graphics* g) {
	//std::cout << "render" << std::endl;
	StateBasedGame::render(container, g);
	m_options->render();
}

void DefaultGame::resize(GameContainer* container, int width, int height) {
	//if (getCurrentState() == m_waveState) {
	if (isInitialised()) {
		m_options->setWidth(width - 80);
		for(unsigned int i = 0; i < m_options->m_children.size(); i++) {
			if (m_options->m_children.at(i) != m_renderAllPathsCheckBox) {
				m_options->m_children.at(i)->setWidth(width - 110);
			}
		}
		m_options->setHeight(height - 20);


		m_waveState->resize(container, width, height);
		m_levelState->resize(container, width, height);
	}
}

void DefaultGame::keyPressed(unsigned int key) {
	StateBasedGame::keyPressed(key);
	m_options->keyPressed(key);
}
void DefaultGame::keyReleased(unsigned int key) {
	StateBasedGame::keyReleased(key);
	m_options->keyReleased(key);
}
void DefaultGame::mouseMoved(int x, int y, int oldx, int oldy) {
	StateBasedGame::mouseMoved(x, y, oldx, oldy);
	m_options->mouseMoved(x, y, oldx, oldy);
}

DefaultGame* DefaultGame::getInstance() {
	return dynamic_cast<DefaultGame*>(ARK2D::getGame());
}

DefaultGame::~DefaultGame() {

}

