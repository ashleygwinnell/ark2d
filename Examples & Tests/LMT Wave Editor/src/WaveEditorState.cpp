/*
 * WaveEditorState.cpp
 *
 *  Created on: 31 Jul 2011
 *      Author: Ashley
 */

#include "DefaultGame.h"
#include "WaveEditorState.h"
#include "GamePanel.h"
#include "EnemyButton.h"

WaveEditorState::WaveEditorState():
	GameState(),
	m_wave(NULL),
	m_toolPanel(NULL),
	m_gamePanel(NULL),
	m_enemySelectPanel(NULL),
	m_playing(false),
	m_timer(0.0f),
	m_currentFile()
{

}
void WaveEditorState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {
	m_toolPanel->refreshDependencies();
	setTitle(m_currentFile);
}
void WaveEditorState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}
unsigned int WaveEditorState::id() {
	return 0;
}

void WaveEditorState::init(GameContainer* container, StateBasedGame* game) {

	newWave();

	m_toolPanel = new ToolPanel();
	m_toolPanel->setLocation(10, 10);
	m_toolPanel->setSize(52, 300);
	m_toolPanel->setPadding(1, 0, 0, 0);
	//m_toolPanel->m_showBorder = false;

		m_toolSelect = new Button();
		m_toolSelect->setSize(50, 50);
		m_toolSelect->setMargin(0, 0, 0, 10);
		m_toolSelect->setText("Sel");
		m_toolSelect->setEvent((void*) &ToolPanel::select);
		m_toolPanel->add(m_toolSelect);
		m_toolPanel->m_selectedTool = m_toolSelect;

		m_toolAddEnemy = new Button();
		m_toolAddEnemy->setSize(50, 50);
		m_toolAddEnemy->setMargin(0, 0, 0, 10);
		m_toolAddEnemy->setText("Enmy");
		m_toolAddEnemy->setEvent((void*) &ToolPanel::addEnemy);
		m_toolPanel->add(m_toolAddEnemy);

		m_toolOptions = new Button();
		m_toolOptions->setSize(50, 50);
		m_toolOptions->setMargin(0, 0, 0, 10);
		m_toolOptions->setText("Options");
		m_toolOptions->setEvent((void*) &ToolPanel::options);
		m_toolPanel->add(m_toolOptions);

		Label* spacer = new Label(" ");
		spacer->setSize(50, 10);
		m_toolPanel->add(spacer);

		m_newButton = new Button();
		m_newButton->setSize(50, 50);
		m_newButton->setMargin(0, 0, 0, 10);
		m_newButton->setText("New");
		m_newButton->setEvent((void*) &WaveEditorState::newWave);
		m_toolPanel->add(m_newButton);

		m_loadButton = new Button();
		m_loadButton->setSize(50, 50);
		m_loadButton->setMargin(0, 0, 0, 10);
		m_loadButton->setText("Load");
		m_loadButton->setEvent((void*) &WaveEditorState::loadWave);
		m_toolPanel->add(m_loadButton);

		m_saveButton = new Button();
		m_saveButton->setSize(50, 50);
		m_saveButton->setMargin(0, 0, 0, 10);
		m_saveButton->setText("Save");
		m_saveButton->setEvent((void*) &WaveEditorState::saveWave);
		m_toolPanel->add(m_saveButton);

		m_saveAsButton = new Button();
		m_saveAsButton->setSize(50, 50);
		m_saveAsButton->setMargin(0, 0, 0, 10);
		m_saveAsButton->setText("Save As");
		m_saveAsButton->setEvent((void*) &WaveEditorState::saveAsWave);
		m_toolPanel->add(m_saveAsButton);

		m_toolPanel->add(spacer);

		m_playButton = new Button();
		m_playButton->setSize(50, 50);
		m_playButton->setMargin(0, 0, 0, 10);
		m_playButton->setText("Play");
		m_playButton->setEvent((void*) &WaveEditorState::playPressed);
		m_toolPanel->add(m_playButton);

		m_pauseButton = new Button();
		m_pauseButton->setSize(50, 50);
		m_pauseButton->setMargin(0, 0, 0, 10);
		m_pauseButton->setText("Pause");
		m_pauseButton->setEvent((void*) &WaveEditorState::pausePressed);
		m_toolPanel->add(m_pauseButton);

		m_stopButton = new Button();
		m_stopButton->setSize(50, 50);
		m_stopButton->setMargin(0, 0, 0, 10);
		m_stopButton->setText("Stop");
		m_stopButton->setEvent((void*) &WaveEditorState::stopPressed);
		m_toolPanel->add(m_stopButton);

		m_rewindButton = new Button();
		m_rewindButton->setSize(50, 50);
		m_rewindButton->setMargin(0, 0, 0, 10);
		m_rewindButton->setText("<<");
		m_rewindButton->setEvent((void*) &WaveEditorState::rewindPressed);
		m_toolPanel->add(m_rewindButton);

		m_forwardButton = new Button();
		m_forwardButton->setSize(50, 50);
		m_forwardButton->setMargin(0, 0, 0, 10);
		m_forwardButton->setText(">>");
		m_forwardButton->setEvent((void*) &WaveEditorState::forwardPressed);
		m_toolPanel->add(m_forwardButton);



	m_gamePanel = new GamePanel();
	//m_gamePanel->setTranslating(true);
	//m_gamePanel->setClipping(true);
	m_gamePanel->setWidth(690);
	m_gamePanel->setHeight(700);
	m_gamePanel->setLocation(70,10);

	DefaultGame* ga = DefaultGame::getInstance();

	m_enemySelectPanel = new EnemySelectPanel();
	m_enemySelectPanel->setPadding(10);
	m_enemySelectPanel->setSize(240, 700);
	m_enemySelectPanel->setTranslating(true);
	m_enemySelectPanel->setClipping(true);
	m_enemySelectPanel->setLocation(m_gamePanel->getX() + m_gamePanel->getWidth() + 10, 10);
	for(unsigned int i = 0; i < ga->m_enemyImages.size(); i++) {
		if (i == 0 || i == 3 || i == 6 || i == 9 || i == 12) {
			Label* lbl = new Label();
			switch(i) {
				case 0:
					lbl->setText("Small: "); break;
				case 3:
					lbl->setText("Splitter: "); break;
				case 6:
					lbl->setText("Shooter: "); break;
				case 9:
					lbl->setText("Shooter Adv: "); break;
				case 12:
					lbl->setText("Reflector: "); break;
			}
			lbl->setSize(200, 30);
			lbl->setPadding(0);
			lbl->setMargin(0);
			m_enemySelectPanel->add(lbl);
		}
		EnemyButton* b = new EnemyButton();
		b->m_id = (i+1);
		b->setSize(50, 50);
		b->setMargin(0, 0, 5, 5);
		b->setImage(ga->m_enemyImages.at(i));
		b->setEvent((void*) EnemyButton::pressedStatic);
		b->setEventObj(b);
		m_enemySelectPanel->add(b);
	}



	m_selectPanel = new ScrollPanel();
	m_selectPanel->setPadding(10);
	m_selectPanel->setSize(240, 700);
	m_selectPanel->setLocation(m_gamePanel->getX() + m_gamePanel->getWidth() + 10, 10);
	m_selectPanel->setTranslating(true);
	m_selectPanel->setClipping(true);
	m_selectPanel->setVisible(false);


			m_selectPathLabel = new Label("Path: ");
			m_selectPathLabel->setSize(200, 30);
			m_selectPanel->add(m_selectPathLabel);

			m_selectPathField = new TextField();
			m_selectPathField->setSize(200, 30);
			m_selectPathField->setPadding(5);
			m_selectPathField->setText("");
			m_selectPanel->add(m_selectPathField);

			m_selectPathButton = new Button();
			m_selectPathButton->setText("Open");
			m_selectPathButton->setSize(60, 30);
			m_selectPathButton->setEvent((void*) WaveEditorState::selectEnemyPath);
			m_selectPanel->add(m_selectPathButton);

			m_selectPathDelayLabel = new Label("Delay: ");
			m_selectPathDelayLabel->setSize(200, 30);
			m_selectPanel->add(m_selectPathDelayLabel);

			m_selectPathDelayField = new TextField();
			m_selectPathDelayField->setSize(200, 30);
			m_selectPathDelayField->setPadding(5);
			m_selectPathDelayField->setText(0.0f);
			m_selectPanel->add(m_selectPathDelayField);


			m_speedModifierLabel = new Label("Speed Modifier: ");
			m_speedModifierLabel->setSize(200, 30);
			m_selectPanel->add(m_speedModifierLabel);

			m_speedModifierField = new TextField();
			m_speedModifierField->setSize(200, 30);
			m_speedModifierField->setPadding(5);
			m_speedModifierField->setText("1.0");
			m_selectPanel->add(m_speedModifierField);


			m_flipHLabel = new Label("Flip Horizontal: ");
			m_flipHLabel->setSize(200, 30);
			m_selectPanel->add(m_flipHLabel);

			m_flipHCheckBox = new CheckBox();
			m_flipHCheckBox->setMargin(0, 0, 180, 0);
			m_selectPanel->add(m_flipHCheckBox);


			m_flipVLabel = new Label("Flip Vertical: ");
			m_flipVLabel->setSize(200, 30);
			m_selectPanel->add(m_flipVLabel);

			m_flipVCheckBox = new CheckBox();
			m_flipVCheckBox->setMargin(0, 0, 180, 0);
			m_selectPanel->add(m_flipVCheckBox);


			m_rotationLabel = new Label("Rotation (degrees): ");
			m_rotationLabel->setSize(200, 30);
			m_selectPanel->add(m_rotationLabel);

			m_rotationField = new TextField();
			m_rotationField->setSize(200, 30);
			m_rotationField->setPadding(5);
			m_rotationField->setText("0");
			m_selectPanel->add(m_rotationField);


			m_selectPathSet = new Button();
			m_selectPathSet->setText("Set");
			m_selectPathSet->setSize(200, 50);
			m_selectPathSet->setMargin(0, 10, 0, 0);
			m_selectPathSet->setEvent((void*) &WaveEditorState::saveEnemy);
			m_selectPanel->add(m_selectPathSet);

	//std::cout << "fff2" << std::endl;

	resize(container, container->getWidth(), container->getHeight());

	//std::cout << "fff3" << std::endl;
}

void WaveEditorState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	m_wave->m_timer = m_timer;

	if (m_playing) {
		m_timer += timer->getDelta();
		float d = m_wave->getDuration();
		if (m_timer >= d) {
			m_timer = d;
		}
		return;
	}

	if (m_forwardButton->getState() == Button::STATE_DOWN) {
		m_timer += timer->getDelta();
		float d = m_wave->getDuration();
		if (m_timer >= d) {
			m_timer = d;
		}
	}
	if (m_rewindButton->getState() == Button::STATE_DOWN) {
		m_timer -= timer->getDelta();
		if (m_timer <= 0) {
			m_timer = 0;
		}
	}
}

void WaveEditorState::resize(GameContainer* container, int width, int height) {
	if (m_gamePanel != NULL)
	{
		m_gamePanel->setWidth(container->getWidth() - 334);
		m_enemySelectPanel->setX(m_gamePanel->getX() + m_gamePanel->getWidth() + 10);
		m_selectPanel->setX(m_gamePanel->getX() + m_gamePanel->getWidth() + 10);

		m_toolPanel->setHeight(height - 20);
		m_selectPanel->setHeight(height - 20);
		m_enemySelectPanel->setHeight(height - 20);
		m_gamePanel->setHeight(height - 20);
	}
}

void WaveEditorState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {

	DefaultGame* ga = DefaultGame::getInstance();

	g->setDrawColor(Color::white);
	m_toolPanel->render();

	if (m_gamePanel->isVisible()) {
		g->setDrawColor(Color::white);
		m_gamePanel->preRender();

			g->pushMatrix();
			g->translate(m_gamePanel->m_offsetX, m_gamePanel->m_offsetY);

			g->drawRect(0, 0, ga->m_gameWidth, ga->m_gameHeight);

			m_wave->render(container, g);

			int sel = m_enemySelectPanel->m_enemySelected;
			if (sel != 0) {
				Input* in = ARK2D::getInput();
				int dx = in->getMouseX() - m_gamePanel->getOnScreenX() - m_gamePanel->m_offsetX;
				int dy = in->getMouseY() - m_gamePanel->getOnScreenY() - m_gamePanel->m_offsetY;

				int snap = ga->m_snapField->getText().getAsInt();
				MathUtil::snap(snap, dx, dy);

				ga->m_enemyImages.at(sel-1)->setAlpha(0.5f);
				ga->m_enemyImages.at(sel-1)->drawCentered(dx, dy);
				ga->m_enemyImages.at(sel-1)->setAlpha(1.0f);
			}



			// option checkbox "render all paths".
			if (ga->m_renderAllPathsCheckBox->isChecked()) {
				for(unsigned int i = 0; i < m_wave->m_enemies.size(); i++){
					renderEnemyPath(i);
				}
			}

			if (m_gamePanel->m_selectedIndex != -1) {
				renderEnemyPath(m_gamePanel->m_selectedIndex);

				EditorEnemy* e = m_wave->m_enemies.get(m_gamePanel->m_selectedIndex);
				g->setDrawColor(Color::white);
				g->drawCircle(e->m_cx, e->m_cy, 20, 30);
			}

			g->popMatrix();

		m_gamePanel->postRender();
	}



	m_selectPanel->render();
	m_enemySelectPanel->render();


	if (m_gamePanel->isVisible()) {
		string tim = "Timer: " + Cast::toString<float>(m_timer) + " / " + Cast::toString<float>(m_wave->getDuration());
		int tx = m_gamePanel->getX() + 10;
		int ty = m_gamePanel->getY() + 10;
		g->drawString(tim, tx, ty);
	}

	//for(unsigned int i = 0; i < m_enemyImages.size(); i++) {
	//	m_enemyImages.at(i)->draw(50 + (i*50), 50);
	//}

	//string s = Cast::toString<int>(m_enemySelectPanel->m_enemySelected);
	//g->drawString(s, 100, 100);

}

void WaveEditorState::renderEnemyPath(int index) {
	Graphics* g = ARK2D::getGraphics();
	EditorEnemy* e = m_wave->m_enemies.get(index);

	try {
		if (e->m_pathGroup != NULL) {
			PathGroup* p = e->m_pathGroup;
			p->setRelative(true);
			if (p->isRelative()) {
				g->pushMatrix();
				g->translate(e->m_cx, e->m_cy);
				p->renderCurve();
				g->popMatrix();
			} else {
				p->renderCurve();
			}

		}
	} catch (...) {

	}
}


void WaveEditorState::keyPressed(unsigned int key) {
	m_toolPanel->keyPressed(key);
	m_gamePanel->keyPressed(key);

	m_selectPanel->keyPressed(key);
	m_enemySelectPanel->keyPressed(key);
}
void WaveEditorState::keyReleased(unsigned int key) {
	m_toolPanel->keyReleased(key);
	m_gamePanel->keyReleased(key);

	m_selectPanel->keyReleased(key);
	m_enemySelectPanel->keyReleased(key);
}

void WaveEditorState::mouseMoved(int x, int y, int oldx, int oldy) {
	m_toolPanel->mouseMoved(x, y, oldx, oldy);
	m_gamePanel->mouseMoved(x, y, oldx, oldy);

	m_selectPanel->mouseMoved(x, y, oldx, oldy);
	m_enemySelectPanel->mouseMoved(x, y, oldx, oldy);
}

void WaveEditorState::saveEnemy() {
	DefaultGame* game = DefaultGame::getInstance();

	EditorEnemy* e = game->m_waveState->m_wave->m_enemies.get(game->m_waveState->m_gamePanel->m_selectedIndex);
	e->m_delay = game->m_waveState->m_selectPathDelayField->getText().getAsFloat();
	e->m_speedModifier = game->m_waveState->m_speedModifierField->getText().getAsFloat();

	string s = game->m_waveState->pathDir(game->m_waveState->m_selectPathField->getText().get());
	//std::cout << "path: " << s << std::endl;
	bool f = StringUtil::file_exists(s.c_str());
	if (!f) {
		ErrorDialog::createAndShow(StringUtil::append("Path does not exist: ", s));
		return;
	}
	PathGroup* p = PathIO::createFromFile(s);
	if (p == NULL) {
		return;
	}
	p->setFlip(game->m_waveState->m_flipHCheckBox->isChecked(), game->m_waveState->m_flipVCheckBox->isChecked());
	p->setRotation(game->m_waveState->m_rotationField->getText().getAsInt());
	p->setRelative(true);
	e->m_path = game->m_waveState->m_selectPathField->getText().get();
	e->m_pathGroup = p;
	//game->m_pathGroups[game->m_selectPathField->getText().get()] = p;
}
EditorEnemy* WaveEditorState::placeEnemy(int id, int cx, int cy) {
	EditorEnemy* e = new EditorEnemy();
	e->m_id = id;
	e->m_cx = cx;
	e->m_cy = cy;
	e->m_delay = 0.0f;
	e->m_path = "";
	e->m_mouseOver = false;
	e->m_pathGroup = NULL;
	e->m_speedModifier = 1.0f;
	m_wave->m_enemies.add(e);
	return e;
}

void WaveEditorState::selectEnemy(int index) {
	m_gamePanel->m_selectedIndex = index;
	if (index == -1) {
		m_selectPanel->setVisible(false);
	} else {
		m_selectPanel->setVisible(true);

		m_selectPathField->setText(m_wave->m_enemies.get(index)->m_path);

		string s = Cast::toString<float>(m_wave->m_enemies.get(index)->m_delay);
		m_selectPathDelayField->setText(s); //std::cout << s << std::endl;

		string s2 = Cast::toString<float>(m_wave->m_enemies.get(index)->m_speedModifier);
		m_speedModifierField->setText(s2);

		if (m_wave->m_enemies.get(index)->m_pathGroup != NULL) {
			m_flipHCheckBox->setChecked(m_wave->m_enemies.get(index)->m_pathGroup->isFlippedH());
			m_flipVCheckBox->setChecked(m_wave->m_enemies.get(index)->m_pathGroup->isFlippedV());

			string s5 = Cast::toString<float>(m_wave->m_enemies.get(index)->m_pathGroup->getRotation());
			m_rotationField->setText(s5);
		}
	}
}

string WaveEditorState::pathDir(string s) {
	//StringUtil::str_replace("\\", "\\\\", s);
	return DefaultGame::getInstance()->m_pathsLocationField->getText().get() + s;
}

void WaveEditorState::setTitle(string title) {
	GameContainer* container = ARK2D::getContainer();
	if (title.size() == 0) {
		container->setTitle("LOVE ME, TOO! Wave Editor");
	} else {
		string s = "LOVE ME, TOO! Wave Editor: " + title;
		container->setTitle(s);
	}
}

void WaveEditorState::selectEnemyPath() {
	DefaultGame* game = DefaultGame::getInstance();
	//int index = game->m_waveState->m_gamePanel->m_selectedIndex;
	string file = FileDialog::openFileDialog("Set Enemy Path");
	StringUtil::toLower(file);
	std::cout << file << std::endl;

	string thisPath = game->m_pathsLocationField->getText().get();
	StringUtil::toLower(thisPath);

	unsigned int found = file.find(thisPath);
	if (found == file.npos) {
		ErrorDialog::createAndShow("Paths must be within the path directory.");
		return;
	}
	string newStr = file.substr(found + game->m_pathsLocationField->getText().length());
	StringUtil::str_replace("\\", "\\\\", newStr);
	game->m_waveState->m_selectPathField->setText(newStr);
}

void WaveEditorState::newWave() {
	DefaultGame* game = DefaultGame::getInstance();

	if (game->m_waveState->m_wave != NULL) {
		game->m_waveState->m_gamePanel->m_selectedIndex = -1;
		game->m_waveState->m_currentFile = "";
		game->m_waveState->setTitle("");
		delete game->m_waveState->m_wave;
		game->m_waveState->m_wave = NULL;
	}
	game->m_waveState->m_wave = new EditorWave();
	game->m_waveState->placeEnemy(1, 240, 320);
}

void WaveEditorState::loadWave() {
	DefaultGame* game = DefaultGame::getInstance();

	// unsaved changes
	// if () {
	//
	//

	string result = FileDialog::openFileDialog("Load Wave");
	if (StringUtil::getExtension(result) != "wave") {
		ErrorDialog::createAndShow("Wave files must end in .wave!");
		return;
	}

	char* c = StringUtil::file_get_contents(result.c_str());
	if (c == NULL) {
		ErrorDialog::createAndShow(StringUtil::append("Could not load Wave file: ", result));
		return;
	}

	if (game->m_waveState->m_wave != NULL) {
		delete game->m_waveState->m_wave;
		game->m_waveState->m_wave = NULL;
	}

	game->m_waveState->m_playing = false;
	game->m_waveState->m_timer = 0.0f;

	game->m_waveState->m_wave = EditorWave::createFromString(string(c));


	game->m_waveState->m_currentFile = result;
	game->m_waveState->setTitle(result);


}

void WaveEditorState::saveWave() {
	DefaultGame* game = DefaultGame::getInstance();

	if (game->m_waveState->m_currentFile.size() == 0) {
		saveAsWave();
		return;
	}

	string s = game->m_waveState->m_wave->toString();
	FileUtil::file_put_contents(game->m_waveState->m_currentFile, s);
	ErrorDialog::createAndShow("Saved!");
}

void WaveEditorState::saveAsWave() {
	DefaultGame* game = DefaultGame::getInstance();

	string result = FileDialog::saveFileDialog("Save File");
	if (StringUtil::getExtension(result) != "wave") {
		ErrorDialog::createAndShow("Wave files must end in .wave!");
		return;
	}
	string s = game->m_waveState->m_wave->toString();
	FileUtil::file_put_contents(result, s);
	game->m_waveState->setTitle(result);
	game->m_waveState->m_currentFile = result;
	ErrorDialog::createAndShow("Saved!");
}


void WaveEditorState::playPressed() {
	DefaultGame* game = DefaultGame::getInstance();
	game->m_waveState->m_playing = true;
	game->m_waveState->m_timer = 0.0f;
}
void WaveEditorState::pausePressed() {
	DefaultGame* game = DefaultGame::getInstance();
	game->m_waveState->m_playing = false;
}
void WaveEditorState::stopPressed() {
	DefaultGame* game = DefaultGame::getInstance();
	game->m_waveState->m_playing = false;
	game->m_waveState->m_timer = 0.0f;
}
void WaveEditorState::forwardPressed() {
	DefaultGame* game = DefaultGame::getInstance();
	game->m_waveState->m_playing = false;
}
void WaveEditorState::rewindPressed() {
	DefaultGame* game = DefaultGame::getInstance();
	game->m_waveState->m_playing = false;
}

WaveEditorState::~WaveEditorState() {

}
