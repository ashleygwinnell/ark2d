/*
 * BlankState.cpp
 *
 *  Created on: 08 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "LevelEditorState.h"

#include "DefaultGame.h"

LevelEditorState::LevelEditorState():
	GameState(),
	m_level(NULL),
	m_levelIDField(NULL),
	m_playing(false),
	m_timer(0.0f),
	m_currentFile("")
	{

}

unsigned int LevelEditorState::id() {
	return 1;
}

void LevelEditorState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {
	m_sideBar->refreshDependencies();
	setTitle(m_currentFile);
}
void LevelEditorState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void LevelEditorState::init(GameContainer* container, StateBasedGame* game) {

	newLevel();

	m_rssl = new RSSL();

	RSSLFunction* gmf = new RSSLFunction();
	gmf->name = "game_mode";
	gmf->multiline = false;
	gmf->callback = (void*) LevelEditorState::script_GameMode;
	gmf->addArgument("mode", RSSL::TYPE_STRING);
	gmf->addArgument("time", RSSL::TYPE_INT);
	m_rssl->addFunction(gmf);

	RSSLFunction* sbf = new RSSLFunction();
	sbf->name = "start_boss";
	sbf->multiline = false;
	sbf->callback = (void*) LevelEditorState::script_StartBoss;
	sbf->addArgument("boss_id", RSSL::TYPE_INT);
	sbf->addArgument("time", RSSL::TYPE_INT);
	m_rssl->addFunction(sbf);

	RSSLFunction* csf = new RSSLFunction();
	csf->name = "camera_shake";
	csf->multiline = false;
	csf->callback = (void*) LevelEditorState::script_CameraShake;
	csf->addArgument("magnitude", RSSL::TYPE_INT);
	csf->addArgument("time", RSSL::TYPE_INT);
	m_rssl->addFunction(csf);

	RSSLFunction* stf = new RSSLFunction();
	stf->name = "spawn_trinket";
	stf->multiline = false;
	stf->callback = (void*) LevelEditorState::script_SpawnTrinket;
	stf->addArgument("x", RSSL::TYPE_INT);
	stf->addArgument("time", RSSL::TYPE_INT);
	m_rssl->addFunction(stf);

	/*RSSLFunction* messagef = new RSSLFunction();
	messagef->name = "modal_dialog";
	messagef->multiline = true;
	messagef->callback = (void*) LevelEditorState::testScript;
	messagef->addArgument("mode", RSSL::TYPE_STRING);
	messagef->addArgument("time", RSSL::TYPE_INT);
	messagef->addArgument("multiline", RSSL::TYPE_STRING);
	m_rssl->addFunction(gmf);*/


	m_sideBar = new LevelToolPanel();
	m_sideBar->setLocation(10, 10);
	m_sideBar->setPadding(1, 0, 0, 0);
	m_sideBar->setSize(52, 600);
	m_sideBar->m_showBorder = false;

	m_selectButton = new Button();
	m_selectButton->setText("Sel");
	m_selectButton->setSize(50, 45);
	m_selectButton->setMargin(0, 0, 0, 10);
	m_selectButton->setEvent((void*) LevelToolPanel::select);
	m_sideBar->add(m_selectButton);

	m_addWaveButton= new Button();
	m_addWaveButton->setText("Wave");
	m_addWaveButton->setSize(50, 45);
	m_addWaveButton->setMargin(0, 0, 0, 10);
	m_addWaveButton->setEvent((void*) LevelEditorState::addWave);
	m_sideBar->add(m_addWaveButton);

	m_eventsButton = new Button();
	m_eventsButton->setText("Events");
	m_eventsButton->setSize(50, 45);
	m_eventsButton->setMargin(0, 0, 0, 10);
	m_eventsButton->setEvent((void*) &LevelToolPanel::events);
	m_sideBar->add(m_eventsButton);

	m_optionsButton = new Button();
	m_optionsButton->setText("Options");
	m_optionsButton->setSize(50, 45);
	m_optionsButton->setMargin(0, 0, 0, 10);
	m_optionsButton->setEvent((void*) &LevelToolPanel::options);
	m_sideBar->add(m_optionsButton);

	m_sideBar->m_selectedTool = m_selectButton;

	Label* label1 = new Label(" ");
	label1->setSize(50, 10);
	m_sideBar->add(label1);

	m_newButton = new Button();
	m_newButton->setText("New");
	m_newButton->setSize(50, 45);
	m_newButton->setMargin(0, 0, 0, 10);
	m_newButton->setEvent((void*) &LevelEditorState::newLevel);
	m_sideBar->add(m_newButton);

	m_loadButton = new Button();
	m_loadButton->setText("Load");
	m_loadButton->setSize(50, 45);
	m_loadButton->setMargin(0, 0, 0, 10);
	m_loadButton->setEvent((void*) &LevelEditorState::loadLevel);
	m_sideBar->add(m_loadButton);

	m_saveButton = new Button();
	m_saveButton->setText("Save");
	m_saveButton->setSize(50, 45);
	m_saveButton->setMargin(0, 0, 0, 10);
	m_saveButton->setEvent((void*) &LevelEditorState::saveLevel);
	m_sideBar->add(m_saveButton);

	m_saveAsButton = new Button();
	m_saveAsButton->setText("Save As");
	m_saveAsButton->setSize(50, 45);
	m_saveAsButton->setMargin(0, 0, 0, 10);
	m_saveAsButton->setEvent((void*) &LevelEditorState::saveAsLevel);
	m_sideBar->add(m_saveAsButton);

	Label* label2 = new Label(" ");
	label2->setSize(50, 10);
	m_sideBar->add(label2);

	m_playButton = new Button();
	m_playButton->setText("Play");
	m_playButton->setSize(50, 45);
	m_playButton->setMargin(0, 0, 0, 10);
	m_playButton->setEvent((void*) &LevelEditorState::playPressed);
	m_sideBar->add(m_playButton);

	m_pauseButton = new Button();
	m_pauseButton->setText("Pause");
	m_pauseButton->setSize(50, 45);
	m_pauseButton->setMargin(0, 0, 0, 10);
	m_pauseButton->setEvent((void*) &LevelEditorState::pausePressed);
	m_sideBar->add(m_pauseButton);

	m_stopButton = new Button();
	m_stopButton->setText("Stop");
	m_stopButton->setSize(50, 45);
	m_stopButton->setMargin(0, 0, 0, 10);
	m_stopButton->setEvent((void*) &LevelEditorState::stopPressed);
	m_sideBar->add(m_stopButton);

	m_rewindButton = new Button();
	m_rewindButton->setText("<<");
	m_rewindButton->setSize(50, 45);
	m_rewindButton->setMargin(0, 0, 0, 10);
	m_sideBar->add(m_rewindButton);

	m_forwardButton = new Button();
	m_forwardButton->setText(">>");
	m_forwardButton->setSize(50, 45);
	m_forwardButton->setMargin(0, 0, 0, 10);
	m_sideBar->add(m_forwardButton);


	m_gamePanel = new LevelGamePanel();
	m_gamePanel->setWidth(690);
	m_gamePanel->setHeight(700);
	m_gamePanel->setLocation(70,10);

	m_eventsPanel = new ScrollPanel();
	m_eventsPanel->setSize(690, 700);
	m_eventsPanel->setLocation(70,10);
	m_eventsPanel->setPadding(10);
	m_eventsPanel->setTranslating(true);
	m_eventsPanel->setClipping(true);

		Label* scriptLabel = new Label();
		scriptLabel->setText("Script:");
		scriptLabel->setSize(650, 30);
		m_eventsPanel->add(scriptLabel);

		m_eventsScriptText = new TextField();
		m_eventsScriptText->setMultiline(true);
		m_eventsScriptText->setSize(800, 600);
		m_eventsScriptText->setPadding(5);
		m_eventsScriptText->setText("");
		m_eventsScriptText->setTextAlignY(TextField::ALIGN_TOP);
		m_eventsPanel->add(m_eventsScriptText);

		m_eventsScriptSave = new Button();
		m_eventsScriptSave->setSize(800, 50);
		m_eventsScriptSave->setPadding(10);
		m_eventsScriptSave->setText("Save Script");
		m_eventsScriptSave->setMargin(0, 10, 0, 0);
		m_eventsScriptSave->setEvent((void*) &LevelEditorState::saveScript);
		m_eventsPanel->add(m_eventsScriptSave);


	m_levelDetailsPanel = new ScrollPanel();
	m_levelDetailsPanel->setWidth(240);
	m_levelDetailsPanel->setHeight(500);
	m_levelDetailsPanel->setPadding(10);
	m_levelDetailsPanel->setVisible(true);
	m_levelDetailsPanel->setLocation(m_gamePanel->getX() + m_gamePanel->getWidth() + 10, 10);

		m_levelIDLabel = new Label();
		m_levelIDLabel->setText("Level ID: ");
		m_levelIDLabel->setSize(200, 30);
		m_levelDetailsPanel->add(m_levelIDLabel);

		m_levelIDField = new TextField();
		m_levelIDField->setSize(200, 30);
		m_levelIDField->setPadding(5);
		m_levelIDField->setText("");
		m_levelDetailsPanel->add(m_levelIDField);

		m_levelNameLabel = new Label();
		m_levelNameLabel->setText("Level Name: ");
		m_levelNameLabel->setSize(200, 30);
		m_levelDetailsPanel->add(m_levelNameLabel);

		m_levelNameField = new TextField();
		m_levelNameField->setSize(200, 30);
		m_levelNameField->setPadding(5);
		m_levelNameField->setText("");
		m_levelDetailsPanel->add(m_levelNameField);


	m_waveDetailsPanel = new ScrollPanel();
	m_waveDetailsPanel->setWidth(240);
	m_waveDetailsPanel->setHeight(500);
	m_waveDetailsPanel->setPadding(10);
	m_waveDetailsPanel->setVisible(false);
	m_waveDetailsPanel->setLocation(m_gamePanel->getX() + m_gamePanel->getWidth() + 10, 10);

	m_waveFileLabel = new Label();
	m_waveFileLabel->setText("Wave: ");
	m_waveFileLabel->setSize(200, 30);
	m_waveDetailsPanel->add(m_waveFileLabel);

	m_waveFileField = new TextField();
	m_waveFileField->setSize(200, 30);
	m_waveFileField->setPadding(5);
	m_waveFileField->setText("");
	m_waveDetailsPanel->add(m_waveFileField);

	m_waveOffsetXLabel = new Label();
	m_waveOffsetXLabel->setText("Offset X:");
	m_waveOffsetXLabel->setSize(200, 30);
	m_waveDetailsPanel->add(m_waveOffsetXLabel);

	m_waveOffsetXField = new TextField();
	m_waveOffsetXField->setSize(200, 30);
	m_waveOffsetXField->setPadding(5);
	m_waveOffsetXField->setText("0");
	m_waveDetailsPanel->add(m_waveOffsetXField);

	m_waveOffsetYLabel = new Label();
	m_waveOffsetYLabel->setText("Offset Y:");
	m_waveOffsetYLabel->setSize(200, 30);
	m_waveDetailsPanel->add(m_waveOffsetYLabel);

	m_waveOffsetYField = new TextField();
	m_waveOffsetYField->setSize(200, 30);
	m_waveOffsetYField->setPadding(5);
	m_waveOffsetYField->setText("0");
	m_waveDetailsPanel->add(m_waveOffsetYField);

	m_waveDelayLabel = new Label();
	m_waveDelayLabel->setText("Delay: ");
	m_waveDelayLabel->setSize(200, 30);
	m_waveDetailsPanel->add(m_waveDelayLabel);

	m_waveDelayField = new TextField();
	m_waveDelayField->setSize(200, 30);
	m_waveDelayField->setPadding(5);
	m_waveDelayField->setText(0);
	m_waveDetailsPanel->add(m_waveDelayField);

	m_waveFlipHLabel = new Label();
	m_waveFlipHLabel->setText("Flip Horizontal: ");
	m_waveFlipHLabel->setSize(200, 30);
	m_waveDetailsPanel->add(m_waveFlipHLabel);

	m_waveFlipHCheckBox = new CheckBox();
	m_waveFlipHCheckBox->setSize(30, 30);
	m_waveDetailsPanel->add(m_waveFlipHCheckBox);

	m_waveFlipVLabel = new Label();
	m_waveFlipVLabel->setText("Flip Vertical: ");
	m_waveFlipVLabel->setSize(200, 30);
	m_waveDetailsPanel->add(m_waveFlipVLabel);

	m_waveFlipVCheckBox = new CheckBox();
	m_waveFlipVCheckBox->setSize(30, 30);
	m_waveDetailsPanel->add(m_waveFlipVCheckBox);

	m_waveSetButton = new Button();
	m_waveSetButton->setText("Set");
	m_waveSetButton->setSize(200, 40);
	m_waveSetButton->setMargin(0, 10, 0, 0);
	m_waveSetButton->setEvent((void*) LevelEditorState::saveWave);
	m_waveDetailsPanel->add(m_waveSetButton);

	resize(container, container->getWidth(), container->getHeight());
}

void LevelEditorState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	if (m_playing) {
		m_timer += timer->getDelta();
	}
	if (m_forwardButton->getState() == Button::STATE_DOWN) {
		m_playing = false;
		m_timer += timer->getDelta();
	}
	if (m_rewindButton->getState() == Button::STATE_DOWN) {
		m_playing = false;
		m_timer -= timer->getDelta();
	}

	float d = m_level->getDuration();
	if (m_timer >= d) {
		m_timer = d;
	} else if (m_timer <= 0) {
		m_timer = 0;
	}
}

void LevelEditorState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	DefaultGame* ga = DefaultGame::getInstance();

	m_sideBar->render();

	if (m_gamePanel->isVisible()) {
		g->setDrawColor(Color::white);
		m_gamePanel->preRender();

			g->pushMatrix();
			g->translate(m_gamePanel->m_offsetX, m_gamePanel->m_offsetY);

			g->drawRect(0, 0, ga->m_gameWidth, ga->m_gameHeight);

			m_level->m_timer = m_timer;
			m_level->render(container, g);

			if (ga->m_renderAllPathsCheckBox->isChecked()) {
				for(unsigned int i = 0; i < m_level->m_waves.size(); i++) {
					m_level->m_waves.get(i)->renderPaths();
				}
			}

			if (m_gamePanel->m_selectedIndex != -1) {
				EditorWave* selectedWave = m_level->m_waves.get(m_gamePanel->m_selectedIndex);
				g->setDrawColor(Color::red);
				g->drawRect(
					selectedWave->getMinX() - 4,
					selectedWave->getMinY() - 5,
					selectedWave->getMaxX() - selectedWave->getMinX() + 9,
					selectedWave->getMaxY() - selectedWave->getMinY() + 10
				);
				if (!ga->m_renderAllPathsCheckBox->isChecked()) {
					selectedWave->renderPaths();
				}
			}

			g->popMatrix();

		m_gamePanel->postRender();
	}

	m_levelDetailsPanel->render();
	m_waveDetailsPanel->render();

	if (m_gamePanel->isVisible()) {
		string timer = string("Timer: ") + Cast::toString<float>(m_timer) + " / " + Cast::toString<float>(m_level->getDuration());
		g->drawString(timer, m_gamePanel->getX() + 10, m_gamePanel->getY() + 10);
	}

	m_eventsPanel->render();

}
void LevelEditorState::resize(GameContainer* container, int width, int height) {
	if (m_gamePanel != NULL)
	{
		m_gamePanel->setWidth(container->getWidth() - 334);
		m_eventsPanel->setWidth(width - 80);
		m_levelDetailsPanel->setX(m_gamePanel->getX() + m_gamePanel->getWidth() + 10);
		m_waveDetailsPanel->setX(m_gamePanel->getX() + m_gamePanel->getWidth() + 10);

		m_sideBar->setHeight(height - 20);
		m_gamePanel->setHeight(height - 20);
		m_eventsPanel->setHeight(height - 20);
	}
}

void LevelEditorState::keyPressed(unsigned int key) {
	m_sideBar->keyPressed(key);
	m_gamePanel->keyPressed(key);
	m_eventsPanel->keyPressed(key);
	m_levelDetailsPanel->keyPressed(key);
	m_waveDetailsPanel->keyPressed(key);
}
void LevelEditorState::keyReleased(unsigned int key) {
	m_sideBar->keyReleased(key);
	m_gamePanel->keyReleased(key);
	m_eventsPanel->keyReleased(key);
	m_levelDetailsPanel->keyReleased(key);
	m_waveDetailsPanel->keyReleased(key);
}
void LevelEditorState::mouseMoved(int x, int y, int oldx, int oldy) {
	m_sideBar->mouseMoved(x, y, oldx, oldy);
	m_gamePanel->mouseMoved(x, y, oldx, oldy);
	m_eventsPanel->mouseMoved(x, y, oldx, oldy);
	m_levelDetailsPanel->mouseMoved(x, y, oldx, oldy);
	m_waveDetailsPanel->mouseMoved(x, y, oldx, oldy);

	if (m_gamePanel->m_selectedIndex != -1 && m_gamePanel->m_movingWave) {
		moveWave();
	}
}
void LevelEditorState::setTitle(string title) {
	GameContainer* container = ARK2D::getContainer();
	if (title.size() == 0) {
		container->setTitle("LOVE ME, TOO! Level Editor");
	} else {
		string s = "LOVE ME, TOO! Level Editor: " + title;
		container->setTitle(s);
	}
}
void LevelEditorState::selectWave(int index) {
	m_gamePanel->m_selectedIndex = index;
	if (index == -1) {
		m_levelDetailsPanel->setVisible(true);
		m_waveDetailsPanel->setVisible(false);
	} else {
		m_levelDetailsPanel->setVisible(false);
		m_waveDetailsPanel->setVisible(true);

		EditorWave* wave = m_level->m_waves.get(index);

		m_waveFileField->setText(wave->fname);

		m_waveOffsetXField->setText(Cast::toString<int>(int(wave->m_offsetx)));
		m_waveOffsetYField->setText(Cast::toString<int>(int(wave->m_offsety)));

		m_waveDelayField->setText(Cast::toString<float>(wave->m_delay));

		m_waveFlipHCheckBox->setChecked(wave->m_flippedH);
		m_waveFlipVCheckBox->setChecked(wave->m_flippedV);

		/*
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
		}*/
	}
}
/*
	game_mode(normal, 3000)

	game_mode(surrounded, 6000)

	start_boss(0, 15000) // skip blank lines.

	camera_shake(30, 20000)

	spawn_trinket(240, 30000)

	modal_message(1, 25000)
	Hello World //a comment


 */
void LevelEditorState::parseScript(string s) {
	try {
		DefaultGame* game = DefaultGame::getInstance();
		game->m_levelState->m_rssl->parse(s); // this calls any functions from the script.
		game->m_levelState->m_level->m_script = s;
	} catch(RSSLException* e) {
		string s = e->getMessage();
		delete e;

		string dialogResp = StringUtil::append(string("script parse response: \r\n"), s);
		ErrorDialog::createAndShow(dialogResp);
	}
}
void LevelEditorState::saveScript() {
	DefaultGame* game = DefaultGame::getInstance();
	ARKString str = game->m_levelState->m_eventsScriptText->getText();

	LevelEditorState::parseScript(str.get());
}

void LevelEditorState::addWave() {
	DefaultGame* game = DefaultGame::getInstance();

	string file = FileDialog::openFileDialog("Add Wave");
	if (StringUtil::getExtension(file) != "wave") {
		ErrorDialog::createAndShow("Wave files must end in .wave!");
		return;
	}
	StringUtil::toLower(file);

	bool f = StringUtil::file_exists(file.c_str());
	if (!f) {
		ErrorDialog::createAndShow(StringUtil::append("Wave does not exist: ", file));
		return;
	}

	string derp = game->m_wavesLocationField->getText().get();
	StringUtil::toLower(derp);
	unsigned int found = file.find(derp);
	if (found == file.npos) {
		ErrorDialog::createAndShow("Waves must be within the wave directory.");
		return;
	}

	//string waveName = file.substr(file.find_last_of("\\")+1);
	string waveName = file.substr(file.find_last_of(FileUtil::getSeparator())+1);

	string contents = StringUtil::file_get_contents(file.c_str());

	EditorWave* wave = EditorWave::createFromString(contents);
	wave->fname = waveName;
	game->m_levelState->m_level->m_waves.add(wave);

}
void LevelEditorState::moveWave() {
	DefaultGame* game = DefaultGame::getInstance();

	int index = game->m_levelState->m_gamePanel->m_selectedIndex;
	EditorWave* wave = game->m_levelState->m_level->m_waves.get(index);

	game->m_levelState->m_waveOffsetXField->setText(wave->m_offsetx);
	game->m_levelState->m_waveOffsetYField->setText(wave->m_offsety);
}

void LevelEditorState::saveWave() {
	DefaultGame* game = DefaultGame::getInstance();

	int index = game->m_levelState->m_gamePanel->m_selectedIndex;
	EditorWave* wave = game->m_levelState->m_level->m_waves.get(index);

	wave->setFlipped(
				game->m_levelState->m_waveFlipHCheckBox->isChecked(),
				game->m_levelState->m_waveFlipVCheckBox->isChecked());

	wave->m_delay = game->m_levelState->m_waveDelayField->getText().getAsFloat();
	wave->m_offsetx = game->m_levelState->m_waveOffsetXField->getText().getAsInt();
	wave->m_offsety = game->m_levelState->m_waveOffsetYField->getText().getAsInt();


}

void LevelEditorState::newLevel() {
	DefaultGame* game = DefaultGame::getInstance();
	if (game->m_levelState->m_level != NULL) {
		delete game->m_levelState->m_level;
		game->m_levelState->m_level = NULL;
	}

	game->m_levelState->m_level = new EditorLevel();
/*
	string contents = StringUtil::file_get_contents("data/waves/example.wave");

	EditorWave* wave = EditorWave::createFromString(contents);
	wave->fname = "example.wave";
	game->m_levelState->m_level->m_waves.add(wave);*/

	game->m_levelState->m_currentFile = "";
	game->m_levelState->setTitle(game->m_levelState->m_currentFile);

	if (game->m_levelState->m_levelIDField != NULL) {
		game->m_levelState->m_levelIDField->setText("");
		game->m_levelState->m_levelNameField->setText("");
	}
}
void LevelEditorState::loadLevel() {
	DefaultGame* game = DefaultGame::getInstance();

	// unsaved changes
	// if () {
	//
	//

	string result = FileDialog::openFileDialog("Load Level");
	if (StringUtil::getExtension(result) != "level") {
		ErrorDialog::createAndShow("Level files must end in .level!");
		return;
	}

	// make sure it's in the

	char* c = StringUtil::file_get_contents(result.c_str());
	if (c == NULL) {
		ErrorDialog::createAndShow(StringUtil::append("Could not load Level file: ", result));
		return;
	}

	if (game->m_levelState->m_level != NULL) {
		delete game->m_levelState->m_level;
		game->m_levelState->m_level = NULL;
	}

	game->m_levelState->m_playing = false;
	game->m_levelState->m_timer = 0.0f;

	game->m_levelState->m_level = EditorLevel::createFromString(string(c));

	game->m_levelState->m_currentFile = result;
	game->m_levelState->setTitle(result);

	game->m_levelState->m_levelIDField->setText((int) game->m_levelState->m_level->m_id);
	game->m_levelState->m_levelNameField->setText(game->m_levelState->m_level->m_name);
}

void LevelEditorState::saveLevel() {
	DefaultGame* game = DefaultGame::getInstance();

	game->m_levelState->m_level->m_id = game->m_levelState->m_levelIDField->getText().getAsInt();
	game->m_levelState->m_level->m_name = game->m_levelState->m_levelNameField->getText().get();

	if (game->m_levelState->m_currentFile.size() == 0) {
		saveAsLevel();
		return;
	}

	string s = game->m_levelState->m_level->toString();
	FileUtil::file_put_contents(game->m_levelState->m_currentFile, s);
	ErrorDialog::createAndShow("Saved!");
}
void LevelEditorState::saveAsLevel() {
	DefaultGame* game = DefaultGame::getInstance();

	string result = FileDialog::saveFileDialog("Save File");
	if (StringUtil::getExtension(result) != "level") {
		ErrorDialog::createAndShow("Level files must end in .level!");
		return;
	}

	string s = game->m_levelState->m_level->toString();
	FileUtil::file_put_contents(result, s);
	game->m_levelState->setTitle(result);
	game->m_levelState->m_currentFile = result;
	ErrorDialog::createAndShow("Saved!");
}

void LevelEditorState::playPressed() {
	DefaultGame* game = DefaultGame::getInstance();
	game->m_levelState->m_playing = true;
	game->m_levelState->m_timer = 0.0f;
}
void LevelEditorState::pausePressed() {
	DefaultGame* game = DefaultGame::getInstance();
	game->m_levelState->m_playing = false;
}
void LevelEditorState::stopPressed() {
	DefaultGame* game = DefaultGame::getInstance();
	game->m_levelState->m_playing = false;
	game->m_levelState->m_timer = 0.0f;
}

void LevelEditorState::script_GameMode(RSSLFunction* f) {
	string mode = f->getArgument(0)->asString();
	int delay = f->getArgument(1)->asInteger();

	LevelEvent* e = new LevelEvent();
	e->m_type = LevelEvent::TYPE__SET_GAME_MODE;
	e->m_time = delay;
	e->m_setGameModeType = 0;

	DefaultGame* game = DefaultGame::getInstance();
	game->m_levelState->m_level->m_events.push_back(e);
}
void LevelEditorState::script_StartBoss(RSSLFunction* f) {
	int boss_id = f->getArgument(0)->asInteger();
	int delay = f->getArgument(1)->asInteger();

	LevelEvent* e = new LevelEvent();
	e->m_type = LevelEvent::TYPE__START_BOSS;
	e->m_time = delay;
	e->m_startBossType = boss_id;

	DefaultGame* game = DefaultGame::getInstance();
	game->m_levelState->m_level->m_events.push_back(e);
}
void LevelEditorState::script_CameraShake(RSSLFunction* f) {
	int magnitude = f->getArgument(0)->asInteger();
	int delay = f->getArgument(1)->asInteger();

	LevelEvent* e = new LevelEvent();
	e->m_type = LevelEvent::TYPE__CAMERA_SHAKE;
	e->m_time = delay;
	e->m_cameraShakeMagnitude = magnitude;

	DefaultGame* game = DefaultGame::getInstance();
	game->m_levelState->m_level->m_events.push_back(e);
}
void LevelEditorState::script_SpawnTrinket(RSSLFunction* f) {
	int x = f->getArgument(0)->asInteger();
	int delay = f->getArgument(1)->asInteger();

	LevelEvent* e = new LevelEvent();
	e->m_type = LevelEvent::TYPE__SPAWN_TRINKET;
	e->m_time = delay;
	e->m_spawnTrinketX = x;

	DefaultGame* game = DefaultGame::getInstance();
	game->m_levelState->m_level->m_events.push_back(e);
}

string LevelEditorState::pathDir(string s) {
	return DefaultGame::getInstance()->m_wavesLocationField->getText().get() + s;
}

LevelEditorState::~LevelEditorState() {

}

