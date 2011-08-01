/*
 * DefaultGame.cpp
 *
 *  Created on: 17 Jan 2011
 *      Author: Ashley
 */

#include "DefaultGame.h"

UIComponent* UIComponent::s_currentFocus = NULL;

DefaultGame::DefaultGame(const char* title):
	Game(title),
	pathGroup(NULL),
	m_currentFile(""),
	m_buttonsPanel(NULL),
	gamePanel(NULL),
	detailsPanel(NULL)
{

}
void DefaultGame::setTitle(string s) {
	string news = "ARK2D Path Editor";
	if (s.length() > 0) {
		news = StringUtil::append(news, ": ");
	}
	ARK2D::getContainer()->setTitle(StringUtil::append(news, s));
}
void DefaultGame::init(GameContainer* container) {

	setTitle("");

	//string defaultPath = "data/example.path";

	//pathGroup = PathIO::createFromFile(defaultPath);
	//if (pathGroup == NULL) {
	//	ErrorDialog::createAndShow(StringUtil::append("Could not load path file: ", defaultPath));
	//	exit(0);
	//}

	m_unsavedChanges = false;

	newPressed();

	/*Path* path = new Path();
	path->setDuration(1.0f);
	path->setEasing(Easing::QUADRATIC_IN_OUT);
	path->addPoint(50, 50);
	path->addPoint(200, 300);
	path->addPoint(550, 50);
	path->addPoint(550, 300);
	path->addPoint(400, 400);

	Path* path2 = new Path();
	path2->setDuration(1.0f);
	path2->addPoint(400, 400);
	path2->addPoint(100, 100);
	path2->addPoint(300, 100);

	pathGroup = new PathGroup();
	pathGroup->addPath(path);
	pathGroup->addPath(path2);*/

	m_buttonsPanel = new ScrollPanel();
	m_buttonsPanel->setLocation(10, 10);
	m_buttonsPanel->m_showBorder = false;
	m_buttonsPanel->setMargin(0);
	m_buttonsPanel->setSize(52, 700);
	m_buttonsPanel->setPadding(1, 0, 0, 0);

	selectButton = new Button();
	selectButton->setSize(50, 50);
	selectButton->setMargin(0, 0, 0, 10);
	selectButton->setText("Select");
	selectButton->setEvent((void*) DefaultGame::toolSelectPressed);
	m_buttonsPanel->add(selectButton);

	majorPointButton = new Button();
	majorPointButton->setSize(50, 50);
	majorPointButton->setMargin(0, 0, 0, 10);
	majorPointButton->setText("Major");
	majorPointButton->setEvent((void*) DefaultGame::toolMajorPointPressed);
	m_buttonsPanel->add(majorPointButton);

	minorPointButton = new Button();
	minorPointButton->setSize(50, 50);
	minorPointButton->setMargin(0, 0, 0, 10);
	minorPointButton->setText("Minor");
	minorPointButton->setEvent((void*) DefaultGame::toolMinorPointPressed);
	m_buttonsPanel->add(minorPointButton);

	Label* spacer1 = new Label(" ");
	spacer1->setSize(50, 10);
	m_buttonsPanel->add(spacer1);

	playButton = new Button();
	playButton->setText("Play");
	playButton->setSize(50, 50);
	playButton->setMargin(0, 0, 0, 10);
	playButton->setEvent((void*) DefaultGame::playPressed);
	m_buttonsPanel->add(playButton);

	pauseButton = new Button();
	pauseButton->setText("Pause");
	pauseButton->setSize(50, 50);
	pauseButton->setMargin(0, 0, 0, 10);
	pauseButton->setEvent((void*) DefaultGame::pausePressed);
	m_buttonsPanel->add(pauseButton);

	stopButton = new Button();
	stopButton->setText("Stop");
	stopButton->setSize(50, 50);
	stopButton->setMargin(0, 0, 0, 10);
	stopButton->setEvent((void*) DefaultGame::stopPressed);
	m_buttonsPanel->add(stopButton);

	forwardButton = new Button();
	forwardButton->setText(">>");
	forwardButton->setSize(50, 50);
	forwardButton->setMargin(0, 0, 0, 10);
	m_buttonsPanel->add(forwardButton);

	rewindButton = new Button();
	rewindButton->setText("<<");
	rewindButton->setSize(50, 50);
	rewindButton->setMargin(0, 0, 0, 10);
	m_buttonsPanel->add(rewindButton);

	Label* spacer2 = new Label(" ");
	spacer2->setSize(50, 10);
	m_buttonsPanel->add(spacer2);

	newButton = new Button();
	newButton->setText("New");
	newButton->setSize(50, 50);
	newButton->setMargin(0, 0, 0, 10);
	newButton->setEvent((void*) DefaultGame::newPressed);
	m_buttonsPanel->add(newButton);

	loadPathButton = new Button();
	loadPathButton->setText("Load");
	loadPathButton->setSize(50, 50);
	loadPathButton->setMargin(0, 0, 0, 10);
	loadPathButton->setEvent((void*) DefaultGame::loadPressed);
	m_buttonsPanel->add(loadPathButton);

	savePathButton = new Button();
	savePathButton->setText("Save");
	savePathButton->setSize(50, 50);
	savePathButton->setMargin(0, 0, 0, 10);
	savePathButton->setEvent((void*) DefaultGame::savePressed);
	m_buttonsPanel->add(savePathButton);

	saveAsPathButton = new Button();
	saveAsPathButton->setText("Sve As");
	saveAsPathButton->setSize(50, 50);
	saveAsPathButton->setMargin(0, 0, 0, 10);
	saveAsPathButton->setEvent((void*) DefaultGame::saveAsPressed);
	m_buttonsPanel->add(saveAsPathButton);

	Label* spacer3 = new Label(" ");
	spacer3->setSize(50, 10);
	m_buttonsPanel->add(spacer3);

	flipHorizontallyButton = new Button();
	flipHorizontallyButton->setText("Flip H");
	flipHorizontallyButton->setSize(50, 50);
	flipHorizontallyButton->setEvent((void*) DefaultGame::flipHPressed);

	flipVerticallyButton = new Button();
	flipVerticallyButton->setText("Flip V");
	flipVerticallyButton->setSize(50, 50);
	flipVerticallyButton->setEvent((void*) DefaultGame::flipVPressed);

	rotate90Button = new Button();
	rotate90Button->setText("Rotate");
	rotate90Button->setSize(50, 50);
	rotate90Button->setEvent((void*) DefaultGame::rotate90Pressed);


	gamePanelMoving = false;
	gamePanelPanX = 50;
	gamePanelPanY = 50;
	gamePanel = new Panel();
	gamePanel->setLocation(70, 10);
	gamePanel->setSize(730, 748);
	gamePanel->setClipping(true);
	gamePanel->setTranslating(true);

	detailsPanel = new Panel();
	detailsPanel->setLocation(70 + 730 + 10, 10);
	detailsPanel->setSize(200, 748);
	detailsPanel->setClipping(true);

	durationTextField = new TextField();
	durationTextField->setLocation(10, 50);
	durationTextField->setSize(150, 40);
	durationTextField->setPadding(5);
	durationTextField->setFocussed(false);

	pointXTextField = new TextField();
	pointXTextField->setLocation(10, 140);
	pointXTextField->setSize(150, 40);
	pointXTextField->setPadding(5);
	pointXTextField->setFocussed(false);

	pointYTextField = new TextField();
	pointYTextField->setLocation(10, 230);
	pointYTextField->setSize(150, 40);
	pointYTextField->setPadding(5);
	pointYTextField->setFocussed(false);

	easingComboBox = new ComboBox();
	easingComboBox->setItemChangedEvent((void*) DefaultGame::easingChangedEvent);
	easingComboBox->setLocation(10, 320);
	easingComboBox->setSize(150, 40);
	easingComboBox->setFocussed(false);

		int itemh = 27;

		ComboBoxItem* ea_linear = new ComboBoxItem();
		ea_linear->setSize(150, itemh);
		ea_linear->setText("Linear");
		ea_linear->setValue("LINEAR");
		easingComboBox->addItem(ea_linear);


		ComboBoxItem* ea_quad_in = new ComboBoxItem();
		ea_quad_in->setSize(150, itemh);
		ea_quad_in->setText("Quadratic In");
		ea_quad_in->setValue("QUADRATIC_IN");
		easingComboBox->addItem(ea_quad_in);

		ComboBoxItem* ea_quad_out = new ComboBoxItem();
		ea_quad_out->setSize(150, itemh);
		ea_quad_out->setText("Quadratic Out");
		ea_quad_out->setValue("QUADRATIC_OUT");
		easingComboBox->addItem(ea_quad_out);

		ComboBoxItem* ea_quad_in_out = new ComboBoxItem();
		ea_quad_in_out->setSize(150, itemh);
		ea_quad_in_out->setText("Quadratic In-Out");
		ea_quad_in_out->setValue("QUADRATIC_IN_OUT");
		easingComboBox->addItem(ea_quad_in_out);


		/*ComboBoxItem* ea_cubic_in = new ComboBoxItem();
		ea_cubic_in->setSize(150, itemh);
		ea_cubic_in->setText("Cubic In");
		ea_cubic_in->setValue("CUBIC_IN");
		easingComboBox->addItem(ea_cubic_in);

		ComboBoxItem* ea_cubic_out = new ComboBoxItem();
		ea_cubic_out->setSize(150, itemh);
		ea_cubic_out->setText("Cubic Out");
		ea_cubic_out->setValue("CUBIC_OUT");
		easingComboBox->addItem(ea_cubic_out);

		ComboBoxItem* ea_cubic_in_out = new ComboBoxItem();
		ea_cubic_in_out->setSize(150, itemh);
		ea_cubic_in_out->setText("Cubic In-Out");
		ea_cubic_in_out->setValue("CUBIC_IN_OUT");
		easingComboBox->addItem(ea_cubic_in_out);*/


		ComboBoxItem* ea_quartic_in = new ComboBoxItem();
		ea_quartic_in->setSize(150, itemh);
		ea_quartic_in->setText("Quartic In");
		ea_quartic_in->setValue("QUARTIC_IN");
		easingComboBox->addItem(ea_quartic_in);

		ComboBoxItem* ea_quartic_out = new ComboBoxItem();
		ea_quartic_out->setSize(150, itemh);
		ea_quartic_out->setText("Quartic Out");
		ea_quartic_out->setValue("QUARTIC_OUT");
		easingComboBox->addItem(ea_quartic_out);

		ComboBoxItem* ea_quartic_in_out = new ComboBoxItem();
		ea_quartic_in_out->setSize(150, itemh);
		ea_quartic_in_out->setText("Quartic In-Out");
		ea_quartic_in_out->setValue("QUARTIC_IN_OUT");
		easingComboBox->addItem(ea_quartic_in_out);


		ComboBoxItem* ea_sine_in = new ComboBoxItem();
		ea_sine_in->setSize(150, itemh);
		ea_sine_in->setText("Sine In");
		ea_sine_in->setValue("SINE_IN");
		easingComboBox->addItem(ea_sine_in);

		ComboBoxItem* ea_sine_out = new ComboBoxItem();
		ea_sine_out->setSize(150, itemh);
		ea_sine_out->setText("Sine Out");
		ea_sine_out->setValue("SINE_OUT");
		easingComboBox->addItem(ea_sine_out);

		ComboBoxItem* ea_sine_in_out = new ComboBoxItem();
		ea_sine_in_out->setSize(150, itemh);
		ea_sine_in_out->setText("Sine In-Out");
		ea_sine_in_out->setValue("SINE_IN_OUT");
		easingComboBox->addItem(ea_sine_in_out);


		ComboBoxItem* ea_expo_in = new ComboBoxItem();
		ea_expo_in->setSize(150, itemh);
		ea_expo_in->setText("Exponential In");
		ea_expo_in->setValue("EXPONENTIAL_IN");
		easingComboBox->addItem(ea_expo_in);

		ComboBoxItem* ea_expo_out = new ComboBoxItem();
		ea_expo_out->setSize(150, itemh);
		ea_expo_out->setText("Exponential Out");
		ea_expo_out->setValue("EXPONENTIAL_OUT");
		easingComboBox->addItem(ea_expo_out);

		ComboBoxItem* ea_expo_in_out = new ComboBoxItem();
		ea_expo_in_out->setSize(150, itemh);
		ea_expo_in_out->setText("Exponential In-Out");
		ea_expo_in_out->setValue("EXPONENTIAL_IN_OUT");
		easingComboBox->addItem(ea_expo_in_out);

/*
		ComboBoxItem* ea_circle_in = new ComboBoxItem();
		ea_circle_in->setSize(150, itemh);
		ea_circle_in->setText("Circle In");
		ea_circle_in->setValue("CIRCLE_IN");
		easingComboBox->addItem(ea_circle_in);

		ComboBoxItem* ea_circle_out = new ComboBoxItem();
		ea_circle_out->setSize(150, itemh);
		ea_circle_out->setText("Circle Out");
		ea_circle_out->setValue("CIRCLE_OUT");
		easingComboBox->addItem(ea_circle_out);

		ComboBoxItem* ea_circle_in_out = new ComboBoxItem();
		ea_circle_in_out->setSize(150, itemh);
		ea_circle_in_out->setText("Circle In-Out");
		ea_circle_in_out->setValue("CIRCLE_IN_OUT");
		easingComboBox->addItem(ea_circle_in_out);
*/

		ComboBoxItem* ea_none = new ComboBoxItem();
		ea_none->setSize(150, itemh);
		ea_none->setText("None");
		ea_none->setValue("NONE");
		easingComboBox->addItem(ea_none);


	snapField = new TextField();
	snapField->setText(10);
	snapField->setLocation(10, 500);
	snapField->setSize(150, 40);
	snapField->setPadding(5);
	snapField->setFocussed(false);

	gameAreaWidthField = new TextField();
	gameAreaWidthField->setText(480);
	gameAreaWidthField->setLocation(10, 600);
	gameAreaWidthField->setSize(150, 40);
	gameAreaWidthField->setPadding(5);
	gameAreaWidthField->setFocussed(false);

	gameAreaHeightField = new TextField();
	gameAreaHeightField->setText(640);
	gameAreaHeightField->setLocation(10, 700);
	gameAreaHeightField->setSize(150, 40);
	gameAreaHeightField->setPadding(5);
	gameAreaHeightField->setFocussed(false);

	m_playing = false;

	m_toolSelected = TOOL_SELECT;

	pointSelected = NULL;
	pointSelectedJoin = NULL;
	pointSelectedMoving = false;
	pointSelectedIndexInGroup = -1;
	pointSelectedIndexInPath = -1;

	UIComponent::s_currentFocus = NULL;

	// make sure the left buttons get layed out properly.
	// todo; write layout manager and crap.
	resize(container, container->getWidth(), container->getHeight());

	ARK2D::getContainer()->getTimer()->flush();
}

void DefaultGame::flip(bool hf, bool vf) {

	int gw = gameAreaWidthField->getText().getAsInt();
	int gcx = gw/2;

	int gh = gameAreaHeightField->getText().getAsInt();
	int gcy = gh/2;

	if (pathGroup != NULL) {
		pathGroup->flip(hf, vf, gcx, gcy);
	}

}



void DefaultGame::rotate(int degrees) {
	int gw = gameAreaWidthField->getText().getAsInt();
	int gcx = gw/2;

	int gh = gameAreaHeightField->getText().getAsInt();
	int gcy = gh/2;

	if (pathGroup != NULL) {
		pathGroup->rotate(degrees, gcx, gcy);
	}
}

void DefaultGame::update(GameContainer* container, GameTimer* timer) {
	/*Input* i = ARK2D::getInput();

	if (i->isKeyDown(Input::KEY_RIGHT)) {
		pathGroup->incrementTimer(0.01f);
		float d = pathGroup->getDuration();
		if (pathGroup->getTimer() >= d) {
			pathGroup->setTimer(d);
		}
	}
	else if (i->isKeyDown(Input::KEY_LEFT)) {
		pathGroup->incrementTimer(-0.01f);
		if (pathGroup->getTimer() <= 0.0f) {
			pathGroup->setTimer(0.0f);
		}
	}*/
	if (pathGroup != NULL) {

		if (forwardButton->getState() == Button::STATE_DOWN) {
			m_playing = false;
			pathGroup->incrementTimer(timer->getDelta());
			float d = pathGroup->getDuration();
			if (pathGroup->getTimer() >= d) {
				pathGroup->setTimer(d);
			}
		}
		if (rewindButton->getState() == Button::STATE_DOWN) {
			m_playing = false;
			pathGroup->incrementTimer(timer->getDelta() * -1);
			if (pathGroup->getTimer() <= 0.0f) {
				pathGroup->setTimer(0.0f);
			}
		}

		/*Input* i = ARK2D::getInput();
		if (i->isKeyPressed(Input::KEY_NUMPAD_4)) {
			gamePanelPanX -= 50;
		} else if (i->isKeyPressed(Input::KEY_NUMPAD_6)) {
			gamePanelPanX += 50;
		}

		if (i->isKeyPressed(Input::KEY_NUMPAD_2)) {
			gamePanelPanY += 50;
		} else if (i->isKeyPressed(Input::KEY_NUMPAD_8)) {
			gamePanelPanY -= 50;
		}*/



		if (m_playing) {
			pathGroup->update(timer);
		} else {
			pathGroup->updateCurrent();
		}
	}

}

void DefaultGame::render(GameContainer* container, Graphics* g) {

	g->setDrawColor(Color::white);
	m_buttonsPanel->render();



	g->setDrawColor(Color::white);
	gamePanel->preRender();

		g->pushMatrix();
		g->translate(gamePanelPanX, gamePanelPanY);

		// draw origin
		g->setLineWidth(2);
		g->drawLine(-10, 0, 10, 0);
		g->drawLine(0, -10, 0, 10);
		g->setLineWidth(1);

		// draw guide.
		g->drawRect(0, 0,
			gameAreaWidthField->getText().getAsInt(),
			gameAreaHeightField->getText().getAsInt()
		);


		if (pathGroup != NULL)
		{
			// render lines
			g->setDrawColor(Color::white);
			pathGroup->render();

			// render point
			Vector2<float>* lastPoint = pathGroup->getLocation();
			g->setDrawColor(Color::red);
			g->fillCircle(int(lastPoint->getX()), int(lastPoint->getY()), 7, 10);

			// render curve.
			//pathGroup->renderCurve();


		}

		g->popMatrix();

	gamePanel->postRender();

	//string pointStr = "point: ";
	//pointStr = StringUtil::append(pointStr, Cast::toString<int>(int(lastPoint->getX())));
	//pointStr = StringUtil::append(pointStr, ":");
	//pointStr = StringUtil::append(pointStr, Cast::toString<int>(int(lastPoint->getY())));
	//g->drawString(pointStr, 640 - g->getFont()->getStringWidth(pointStr), 5);

	//string timeStr = "time: ";
	//timeStr = StringUtil::append(timeStr, Cast::toString<float>(pathGroup->getTimer()));
	//g->drawString(timeStr, 75, 5);

	//string curStr = "cur: ";
	//curStr = StringUtil::append(curStr, Cast::toString<float>(pathGroup->getIndex()));
	//g->drawString(curStr, 75, 450);

	if (pointSelected != NULL) {
		g->setDrawColor(Color::white);
		g->drawCircle(
			int(pointSelected->getX()) + gpmx(),
			int(pointSelected->getY()) + gpmy(), 20, 20);
	}


	detailsPanel->render();
	if (pointSelectedIndexInGroup != -1) {

		// render items
		g->drawString("Duration: ", detailsPanel->getX() + 10, 15);
		durationTextField->setX(detailsPanel->getX() + 10);
		durationTextField->render();

		g->drawString("X: ", detailsPanel->getX() + 10, pointXTextField->getY()-35);
		pointXTextField->setX(detailsPanel->getX() + 10);
		pointXTextField->render();

		g->drawString("Y: ", detailsPanel->getX() + 10, pointYTextField->getY()-35);
		pointYTextField->setX(detailsPanel->getX() + 10);
		pointYTextField->render();

		g->drawString("Easing: ", detailsPanel->getX() + 10, easingComboBox->getY()-35);
		easingComboBox->setX(detailsPanel->getX() + 10);
		easingComboBox->render();

		// update data from items
		string s = durationTextField->getText().get();
		if (s.length() > 0) {
			float sf = Cast::fromString<float>(s);
			pathGroup->getPath(pointSelectedIndexInGroup)->setDuration(sf);
		}

		if (pointSelected != NULL && !pointSelectedMoving) {
			string px = pointXTextField->getText().get();
			int pxi = Cast::fromString<int>(px);

			string py = pointYTextField->getText().get();
			int pyi = Cast::fromString<int>(py);

			// snapping
			int snap = snapField->getText().getAsInt();
			MathUtil::snap(snap, pxi, pyi);

			pointSelected->set(pxi, pyi);
			if (pointSelectedJoin != NULL) {
				pointSelectedJoin->set(pxi, pyi);
			}
		}

	} else {


		gameAreaHeightField->setY(detailsPanel->getY() + detailsPanel->getHeight() - 10 - 100);

		gameAreaWidthField->setY(gameAreaHeightField->getY() - 80);
		g->drawString("Guide Width: ", detailsPanel->getX() + 10, gameAreaWidthField->getY() - 35);
		gameAreaWidthField->setX(detailsPanel->getX() + 10);
		gameAreaWidthField->render();

		snapField->setY(gameAreaWidthField->getY() - 80);
		g->drawString("Snap To: ", detailsPanel->getX() + 10, snapField->getY() - 35);
		snapField->setX(detailsPanel->getX() + 10);
		snapField->render();

		g->drawString("Guide Height: ", detailsPanel->getX() + 10, gameAreaHeightField->getY() - 35);
		gameAreaHeightField->setX(detailsPanel->getX() + 10);
		gameAreaHeightField->render();

		// flip scene
			flipHorizontallyButton->setX(detailsPanel->getX() + 10);
			flipHorizontallyButton->setY(detailsPanel->getY() + detailsPanel->getHeight() - 10 - 50);
			flipHorizontallyButton->render();

			flipVerticallyButton->setX(detailsPanel->getX() + 70);
			flipVerticallyButton->setY(detailsPanel->getY() + detailsPanel->getHeight() - 10 - 50);
			flipVerticallyButton->render();

			rotate90Button->setX(detailsPanel->getX() + 130);
			rotate90Button->setY(detailsPanel->getY() + detailsPanel->getHeight() - 10 - 50);
			rotate90Button->render();
	}

	if (gamePanel->isVisible()) {
		string tim = "Timer: " + Cast::toString<float>(pathGroup->getTimer()) + " / " + Cast::toString<float>(pathGroup->getDuration());
		int tx = gamePanel->getX() + 10;
		int ty = gamePanel->getY() + 10;
		g->drawString(tim, tx, ty);
	}




}

void DefaultGame::resize(GameContainer* container, int width, int height) {
	//Graphics* g = ARK2D::getGraphics();
	//g->ortho2d(0, 0, width, height);

	if (gamePanel != NULL) {
		m_buttonsPanel->setHeight(height - 20);

		gamePanel->setSize(width - 40 - 50 - detailsPanel->getWidth(), height - 20);
		detailsPanel->setLocation(width - 10 - detailsPanel->getWidth(), 10);
		detailsPanel->setSize(detailsPanel->getWidth(), height - 20);

	}



}

void DefaultGame::easingChangedEvent() {
	DefaultGame* game = DefaultGame::getInstance();
	if (game->pathGroup != NULL && game->pointSelected != NULL) {
		Path* p = game->pathGroup->getPath(game->pointSelectedIndexInGroup);
		p->setEasing(Easing::getByString(game->easingComboBox->getSelected()->getValue()));
	}
}

void DefaultGame::keyPressed(unsigned int key) {
	//Input* i = ARK2D::getInput();
	//std::cout << key << " : " << i->getKeyChar(key) << " : " << i->getKeyName(key) << std::endl;
	durationTextField->keyPressed(key);
	pointXTextField->keyPressed(key);
	pointYTextField->keyPressed(key);
	easingComboBox->keyPressed(key);

	if (pointSelectedIndexInGroup == -1) {

		flipHorizontallyButton->keyPressed(key);
		flipVerticallyButton->keyPressed(key);
		rotate90Button->keyPressed(key);

		snapField->keyPressed(key);
		gameAreaWidthField->keyPressed(key);
		gameAreaHeightField->keyPressed(key);
	}

	m_buttonsPanel->keyPressed(key);

	if (pathGroup != NULL){

		// Checking selecting points.
		Input* in = ARK2D::getInput();

		if (key == (unsigned int) Input::MOUSE_BUTTON_RIGHT) {
			gamePanelMoving = true;
		}

		if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {
			int mx = in->getMouseX();
			int my = in->getMouseY();

			if(GigaRectangle<int>::s_collides(gamePanel->getX(), gamePanel->getY(), gamePanel->getWidth(), gamePanel->getHeight(), mx, my, 1, 1))
			{
				bool selected = false;
				for(unsigned int i = 0; i < pathGroup->getPaths().size(); i++) {
					for(unsigned int j = 0; j < pathGroup->getPath(i)->getPoints().size(); j++) {
						Vector2<float>* p = pathGroup->getPath(i)->getPoints().at(j);
						bool s = Circle::s_collides(
							p->getX(),
							p->getY(),
							5,
							//float(mx - gamePanel->getX()),
							//float(my - gamePanel->getY()),
							float(mx - gpmx()),
							float(my - gpmy()),
							5
						);
						if(s) {
							pointSelected = p;
							pointSelectedJoin = NULL;
							pointSelectedMoving = true;
							selected = true;

							pointSelectedIndexInGroup = i;
							pointSelectedIndexInPath = j;

							// set property of duration text field.
							float f = pathGroup->getPath(pointSelectedIndexInGroup)->getDuration();
							durationTextField->setText(Cast::toString<float>(f));

							pointXTextField->setText(Cast::toString<int>(int(p->getX())));
							pointYTextField->setText(Cast::toString<int>(int(p->getY())));

							durationTextField->setFocussed(false);
							pointXTextField->setFocussed(false);
							pointYTextField->setFocussed(false);

							unsigned int thisEasing = pathGroup->getPath(pointSelectedIndexInGroup)->getEasing();
							easingComboBox->setSelectedByValue(Easing::getByInt(thisEasing));

							UIComponent::s_currentFocus = NULL;

							if (i == 0) { // start path
								if (pathGroup->getPath(i)->getPoints().size() > 1) {
									if (j == pathGroup->getPath(i)->getPoints().size() - 1) {
										if (pathGroup->getNumPaths() > 1) {
											pointSelectedJoin = pathGroup->getPath(i+1)->getPoints().at(0);
										}
									}
								}
							} else if (i == pathGroup->getPaths().size() - 1) { // end path
								if (pathGroup->getPath(i)->getPoints().size() > 1) {
									if (j == 0) {
										pointSelectedJoin = pathGroup->getPath(i-1)->getPoints().at(pathGroup->getPath(i-1)->getPoints().size()-1);
									}
								}
							} else { // middle path.
								if (pathGroup->getPath(i)->getPoints().size() > 1) {
									if (j == 0) {
										pointSelectedJoin = pathGroup->getPath(i-1)->getPoints().at(pathGroup->getPath(i-1)->getPoints().size()-1);
									} else if (j == pathGroup->getPath(i)->getPoints().size() - 1) {
										pointSelectedJoin = pathGroup->getPath(i+1)->getPoints().at(0);
									}
								}
							}

							/*if (i != pathGroup->getPaths().size() - 1
								&& j == pathGroup->getPath(i)->getPoints().size() - 1)
							{
								pointSelectedJoin = pathGroup->getPath(i+1)->getPoints().at(0);
							}
							else if (i == pathGroup->getPaths().size() - 1
									 && j == 0)
							{
								pointSelectedJoin = pathGroup->getPath(i-1)->getPoints().at(pathGroup->getPath(i-1)->getPoints().size()-1);
							}
							else if (i != 0
									&& i != pathGroup->getPaths().size() - 1
									&& j == pathGroup->getPath(i)->getPoints().size() - 1)
							{
								pointSelectedJoin = pathGroup->getPath(i+1)->getPoints().at(0);
							}
							else if (j == 0
									&& i != pathGroup->getPaths().size() - 1
									&& i != 0)
							{
								pointSelectedJoin = pathGroup->getPath(i-1)->getPoints().at(pathGroup->getPath(i-1)->getPoints().size()-1);
							}*/
						}
					}
				}
				if (selected == false) {
					//pointSelected = NULL;
					//pointSelectedJoin = NULL;
					//pointSelectedIndexInGroup = -1;
					//pointSelectedIndexInPath = -1;
					unselectPoint();
				}
			}

		}

	}
}
void DefaultGame::keyReleased(unsigned int key) {

	m_buttonsPanel->keyReleased(key);

	if (pointSelectedIndexInGroup == -1) {

		flipHorizontallyButton->keyReleased(key);
		flipVerticallyButton->keyReleased(key);
		rotate90Button->keyReleased(key);

	}

	//easingComboBox->keyReleased(key);

	if (pathGroup != NULL) {

		Input* in = ARK2D::getInput();

		if (key == (unsigned int) Input::MOUSE_BUTTON_RIGHT) {
			gamePanelMoving = false;
		}

		if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT && pointSelected != NULL) {
			pointSelectedMoving = false;

			int mx = in->getMouseX();
			int my = in->getMouseY();

			if(GigaRectangle<int>::s_collides(gamePanel->getX(), gamePanel->getY(), gamePanel->getWidth(), gamePanel->getHeight(), mx, my, 1, 1))
			{

				bool s = Circle::s_collides(
					pointSelected->getX(),
					pointSelected->getY(),
					5,
					//float(mx - gamePanel->getX()),
					//float(my - gamePanel->getY()),
					float(mx - gpmx()),
					float(my - gpmy()),
					5
				);
				if (!s) {
					//pointSelected = NULL;
					//pointSelectedJoin = NULL;
					unselectPoint();
				}

			}
		}

		if (key == (unsigned int) Input::KEY_DELETE || key == (unsigned int) Input::KEY_BACKSPACE) {
			//std::cout << "focus:" << durationTextField->isFocussed() << std::endl;
			if (!durationTextField->isFocussed()
				&& !pointXTextField->isFocussed()
				&& !pointYTextField->isFocussed()
				&& !snapField->isFocussed()
				&& !gameAreaWidthField->isFocussed()
				&& !gameAreaHeightField->isFocussed()
			) {
				if (pointSelected != NULL) {
					// delete minor point.
					Path* pp = pathGroup->getPath(pointSelectedIndexInGroup);
					if (pointSelectedIndexInPath != 0 && pointSelectedIndexInPath != (signed int) pp->getPoints().size()-1) {
						std::cout << "remove point: " << pointSelectedIndexInGroup << ":" << pointSelectedIndexInPath << " . " << std::endl;
						pp->removePoint(pointSelectedIndexInPath);

						unselectPoint();
					} else {

						if ((pointSelectedIndexInGroup == 0 && pointSelectedIndexInPath == 0)
							|| (pointSelectedIndexInGroup == (int) pathGroup->getNumPaths() - 1 && pointSelectedIndexInPath == (int) pathGroup->getPath(pointSelectedIndexInGroup)->getSize() - 1)) {
							std::cout << "first and last point cannot be deleted" << std::endl;
							return;
						}

						Path* removeLastFrom = pathGroup->getPath(pointSelectedIndexInGroup-1);
						Path* removeFirstFrom = pathGroup->getPath(pointSelectedIndexInGroup);

						removeLastFrom->removePoint(removeLastFrom->getSize()-1);
						removeFirstFrom->removePoint(0);

						Path* newPath = new Path();
						for(unsigned int i = 0; i < removeLastFrom->getSize(); i++) {
							Vector2<float>* v = removeLastFrom->getPoint(i);
							newPath->addPoint(v->getX(), v->getY());
						}
						for(unsigned int i = 0; i < removeFirstFrom->getSize(); i++) {
							Vector2<float>* v = removeFirstFrom->getPoint(i);
							newPath->addPoint(v->getX(), v->getY());
						}

						//std::cout << pointSelectedIndexInGroup << std::endl;
						//for(unsigned int i = 0; i < newPath->getSize(); i++) {
							//Vector2<float>* v = newPath->getPoint(i);
						//	std::cout << v->getX() << " : " << v->getY() << std::endl;
						//}

						if (pathGroup->getNumPaths() == 2) {
							pathGroup->clear();
						} else {
							pathGroup->removePath(pointSelectedIndexInGroup);
							pathGroup->removePath(pointSelectedIndexInGroup-1);
						}
						pathGroup->addPath(newPath, pointSelectedIndexInGroup-1);

						//std::cout << "major point del" << std::endl;

						unselectPoint();
					}

					//
				}
			}
		}
	}
}
void DefaultGame::unselectPoint() {
	pointSelected = NULL;
	pointSelectedJoin = NULL;
	pointSelectedMoving = false;
	pointSelectedIndexInGroup = -1;
	pointSelectedIndexInPath = -1;
}
void DefaultGame::mouseMoved(int x, int y, int oldx, int oldy) {
	m_buttonsPanel->mouseMoved(x, y, oldx, oldy);


	if (pointSelectedIndexInGroup == -1) {
		flipHorizontallyButton->mouseMoved(x, y, oldx, oldy);
		flipVerticallyButton->mouseMoved(x, y, oldx, oldy);
		rotate90Button->mouseMoved(x, y, oldx, oldy);
	}


	if (gamePanelMoving == true) {
		gamePanelPanX += (x - oldx);
		gamePanelPanY += (y - oldy);
	}

	if (pointSelectedMoving && pointSelected != NULL) {
		//pointSelected->set(x - gamePanel->getX(), y - gamePanel->getY());
		m_unsavedChanges = true;

		int nx = x - gpmx();
		int ny = y - gpmy();

		int snap = snapField->getText().getAsInt();
		MathUtil::snap(snap, nx, ny);

		pointXTextField->setText(Cast::toString<int>(nx));
		pointYTextField->setText(Cast::toString<int>(ny));

		pointSelected->set(nx, ny);
		if (pointSelectedJoin != NULL) {
			//pointSelectedJoin->set(x - gamePanel->getX(), y - gamePanel->getY());
			pointSelectedJoin->set(nx, ny);
		}
	}

}

DefaultGame* DefaultGame::getInstance() {
	return dynamic_cast<DefaultGame*>(ARK2D::getGame());
}

void DefaultGame::toolSelectPressed() {
	DefaultGame* game = DefaultGame::getInstance();
	game->m_toolSelected = DefaultGame::TOOL_SELECT;
}
void DefaultGame::toolMajorPointPressed() {
	DefaultGame* game = DefaultGame::getInstance();
	if (game->pathGroup != NULL)
	{
		if (game->pointSelected == NULL) {
			if (game->pathGroup->getPaths().size() == 0) {

			} else {
				ErrorDialog::createAndShow("You must have a point selected to use this tool.");
			}
		} else {
			int i = game->pointSelectedIndexInGroup;
			int j = game->pointSelectedIndexInPath;

			Path* p = game->pathGroup->getPath(i);

			// ...
			game->m_unsavedChanges = true;

			// If we have the last point in the path selected. make it the one before.
			bool lastPointSelected = false;
			if (i == (signed int) game->pathGroup->getPaths().size() - 1
					&& j == (signed int) game->pathGroup->getPath(i)->getSize() - 1) {
				j--;
				lastPointSelected = true;
			}

			// new point location
			int newpx = 0;
			int newpy = 0;
			Vector2<float>* selectedPoint = game->pathGroup->getPath(i)->getPoint(j);
			Vector2<float>* nextPoint = game->pathGroup->getPath(i)->getPoint(j+1);
			newpx = int(selectedPoint->getX() + nextPoint->getX()) / 2;
			newpy = int(selectedPoint->getY() + nextPoint->getY()) / 2;

			// make a new path to put shit in to - it always starts with the new point.
			Path* newPath = new Path();
			newPath->addPoint(newpx, newpy);

			// Add all points above currently selected index to the new path.
			int countAdded = 0;
			for(int k = j+1; k < (signed int) p->getSize(); k++) {
				Vector2<float>* pk = p->getPoint(k);
				newPath->addPoint(pk->getX(), pk->getY());
				countAdded++;
			}

			// Remove all points above currently selected index from path.
			for(unsigned int k = 0; k < (unsigned int) countAdded; k++) {
				p->removePoint(p->getSize()-1);
			}

			// Old path always ends in the new point.
			p->addPoint(newpx, newpy);

			// Add the new path to the pathgroup.
			game->pathGroup->addPath(newPath, i+1); // it is _always_ at +1.

			//std::cout << PathIO::getAsJSON(game->pathGroup) << std::endl;

			if (lastPointSelected) {
				// fix this properly.
				game->unselectPoint();
			}

/*
			int addAt;
			Vector2<float>* after;
			Vector2<float>* before;
			if (j == (signed int) p->getPoints().size()-1) {
				after = p->getPoints().at(j-1);
				before = p->getPoints().at(j);
				addAt = j-1;
			} else {
				after = p->getPoints().at(j);
				before = p->getPoints().at(j+1);
				addAt = j;
			}

			int newpx = int(after->getX() + before->getX()) / 2;
			int newpy = int(after->getY() + before->getY()) / 2;

			int addAtGroup = game->pointSelectedIndexInGroup;
			if (i == 0) {
				addAtGroup++;
			} else if ( i == (signed int) game->pathGroup->getPaths().size()-1) {
				addAtGroup--;
			}

			// if we add major point between 1 and 2, this pathgroup
			// 	P1: 0, 1, 2, 3
			// 	P2: 3, 4
			// becomes this
			// 	P1: 0, 1, n
			// 	P2: n, 2, 3
			// 	P3: 3, 4

			// Make new path and add n.
			Path* newp = new Path();
			newp->addPoint(newpx, newpy);

			// add 2 and 3 to P2 and remove them from P1.
			for(unsigned int k = p->getSize() - 1; k > (unsigned int) j; k--) {
				Vector2<float>* pn = p->getPoint(k);
				newp->addPoint(pn->getX(), pn->getY());
				p->removePoint(k);
			}

			// Add n to P1.
			p->addPoint(newpx, newpy); // it's fine to add it to the end as items have been removed.

			//game->pathGroup->addPath(newp, game->pointSelectedIndexInGroup);
			game->pathGroup->addPath(newp, addAtGroup);

			//before->set(newpx, newpy);
 	 	 	 */
		}
	}
}
void DefaultGame::toolMinorPointPressed() {
	DefaultGame* game = DefaultGame::getInstance();
	if (game->pathGroup != NULL)
	{
		if (game->pointSelected == NULL) {
			ErrorDialog::createAndShow("You must have a point selected to use this tool.");
		} else {
			int i = game->pointSelectedIndexInGroup;
			int j = game->pointSelectedIndexInPath;
			Path* p = game->pathGroup->getPath(i);

			// ...
			game->m_unsavedChanges = true;

			int addAt;
			Vector2<float>* after;
			Vector2<float>* before;
			if (j == (signed int) p->getPoints().size()-1) {
				after = p->getPoints().at(j);
				before = p->getPoints().at(j-1);
				addAt = j;
			} else {
				after = p->getPoints().at(j);
				before = p->getPoints().at(j+1);
				addAt = j+1;
			}

			int newpx = int(after->getX() + before->getX()) / 2;
			int newpy = int(after->getY() + before->getY()) / 2;

			p->addPoint(newpx, newpy, addAt);
		}
	}
}
void DefaultGame::playPressed() {
	//std::cout << "play pressed!!" << std::endl;
	DefaultGame* game = getInstance();
	if (game->pathGroup != NULL)
	{
		game->m_playing = true;

		if (game->pathGroup->getTimer() >= game->pathGroup->getDuration()) {
			game->pathGroup->setTimer(0.0f);
		}
	}

}
void DefaultGame::pausePressed() {
	//std::cout << "pause pressed!!" << std::endl;
	DefaultGame* game = getInstance();
	if (game->pathGroup != NULL) {
		game->m_playing = false;
	}
}
void DefaultGame::stopPressed() {
	//std::cout << "stop pressed!!" << std::endl;
	DefaultGame* game = getInstance();
	if (game->pathGroup != NULL) {
		game->m_playing = false;
		game->pathGroup->setTimer(0.0f);
	}
}

void DefaultGame::newPressed() {
	DefaultGame* game = getInstance();
	if (game->pathGroup != NULL) {
		//delete game->pathGroup;
	}

	if (game->m_unsavedChanges == true) {
		// todo: confirm dialog
	}

	game->pathGroup = new PathGroup();
	Path* defaultPath = new Path();
	defaultPath->addPoint(100, 100);
	defaultPath->addPoint(300, 100);
	game->pathGroup->addPath(defaultPath);

	game->setTitle("");
	game->m_currentFile = "";
	//game->m_playing = false;
}
void DefaultGame::loadPressed() {
	//std::cout << "next path pressed!!" << std::endl;
	DefaultGame* game = getInstance();

	if (game->m_unsavedChanges == true) {
		// todo: confirm dialog
	}

	string f = FileDialog::openFileDialog();
	if (f.length() > 0) {

		if (StringUtil::getExtension(f) != "path") {
			ErrorDialog::createAndShow("Can only load .path files.");
			return;
		}
		//std::cout << "loading file: " << f << std::endl;
		if (game->pathGroup != NULL) {
			delete game->pathGroup;
		}
		game->pathGroup = PathIO::createFromFile(f);
		game->setTitle(f);
		game->m_currentFile = f;
	}

}
void DefaultGame::savePressed() {
	std::cout << "save pressed!!" << std::endl;

	DefaultGame* game = DefaultGame::getInstance();
	if (game->pathGroup == NULL) {
		ErrorDialog::createAndShow("There is no path data to save.");
		return;
	}

	if (game->m_currentFile.length() > 0) {
		string s = PathIO::getAsJSON(game->pathGroup);
		FileUtil::file_put_contents(game->m_currentFile, s);
		ErrorDialog::createAndShow("Saved!");
		game->m_unsavedChanges = false;
	} else {
		saveAsPressed();
	}

}
void DefaultGame::saveAsPressed() {
	std::cout << "save-as pressed!!" << std::endl;

	DefaultGame* game = DefaultGame::getInstance();
	string f = FileDialog::saveFileDialog();
	if (f.length() > 0) {
		if (StringUtil::getExtension(f) != "path") {
			ErrorDialog::createAndShow("Can only save .path files.");
			return;
		}
		string s = PathIO::getAsJSON(game->pathGroup);
		std::cout << s << std::endl;
		FileUtil::file_put_contents(f, s);
		ErrorDialog::createAndShow("Saved!");
		game->setTitle(f);
		game->m_currentFile = f;
		game->m_unsavedChanges = false;
	}
}

void DefaultGame::flipHPressed() {
	std::cout << "h flip pressed" << std::endl;

	DefaultGame* game = DefaultGame::getInstance();
	if (game->pointSelected == NULL) {
		game->flip(true, false);
	} //else {
		//game->flipSingular(true, false);
	//}
}
void DefaultGame::flipVPressed() {
	std::cout << "v flip pressed" << std::endl;

	DefaultGame* game = DefaultGame::getInstance();
	if (game->pointSelected == NULL) {
		game->flip(false, true);
	} //else {
	//	game->flipSingular(false, true);
	//}
}

void DefaultGame::rotate90Pressed() {
	std::cout << "rotate 90 pressed" << std::endl;

	DefaultGame* game = DefaultGame::getInstance();
	if (game->pointSelected == NULL) {
		game->rotate(90);
	}
}

DefaultGame::~DefaultGame() {

}

