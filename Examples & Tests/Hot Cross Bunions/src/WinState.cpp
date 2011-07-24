/*
 * TYPGameState.cpp
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#include "ARK.h"
#include "WinState.h"

#include "DefaultGame.h"

WinState::WinState():
	GameState(), m_game(NULL), m_win(false), m_arcadeInputIndex(0), bg(NULL), name(), characters(),
	m_timer(0.0f) {

}

unsigned int WinState::id() {
	return 2;
}

void WinState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}
void WinState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void WinState::init(GameContainer* container, StateBasedGame* game) {
	m_game = DefaultGame::getGame();

	characters.push_back(" ");
	characters.push_back("A");
	characters.push_back("B");
	characters.push_back("C");
	characters.push_back("D");
	characters.push_back("E");
	characters.push_back("F");
	characters.push_back("G");
	characters.push_back("H");
	characters.push_back("I");
	characters.push_back("J");
	characters.push_back("K");
	characters.push_back("L");
	characters.push_back("M");
	characters.push_back("N");
	characters.push_back("O");
	characters.push_back("P");
	characters.push_back("Q");
	characters.push_back("R");
	characters.push_back("S");
	characters.push_back("T");
	characters.push_back("U");
	characters.push_back("V");
	characters.push_back("W");
	characters.push_back("X");
	characters.push_back("Y");
	characters.push_back("Z");
	characters.push_back("0");
	characters.push_back("1");
	characters.push_back("2");
	characters.push_back("3");
	characters.push_back("4");
	characters.push_back("5");
	characters.push_back("6");
	characters.push_back("7");
	characters.push_back("8");
	characters.push_back("9");

	characterIndexes.push_back(1);
	characterIndexes.push_back(0);
	characterIndexes.push_back(0);

	bg = m_game->sheet->getSubImage(m_game->desc->getItemByName("win.png"))->getScaledCopy(8, 8);
}

void WinState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	m_timer += timer->getDelta();
	if (m_timer > 1.0f) {
		m_timer -= 1.0f;
	}

	if (DefaultGame::ARCADE_VERSION) {
		Input* i = ARK2D::getInput();

		if (i->isKeyPressed(DefaultGame::KEY_UP)) {
			characterIndexes[m_arcadeInputIndex]--;
			if (characterIndexes[m_arcadeInputIndex] < 1) {
				characterIndexes[m_arcadeInputIndex] = 1;
			}
		} else if (i->isKeyPressed(DefaultGame::KEY_DOWN)) {
			characterIndexes[m_arcadeInputIndex]++;
			if ((unsigned int) characterIndexes[m_arcadeInputIndex] > characters.size()-1) {
				characterIndexes[m_arcadeInputIndex] = characters.size()-1;
			}
		}

		if (DefaultGame::isAnyKeyPressed() || i->isKeyPressed(DefaultGame::KEY_RIGHT)
			//i->isKeyPressed(DefaultGame::KEY_ENTER)
			//|| i->isKeyPressed(DefaultGame::KEY_RIGHT)) {
				){
			m_arcadeInputIndex++;

			if (m_arcadeInputIndex == 4 && i->isKeyPressed(DefaultGame::KEY_RIGHT)) {
				m_arcadeInputIndex--;
				return;
			}

			if (characterIndexes[m_arcadeInputIndex] == 0) {
				characterIndexes[m_arcadeInputIndex] = 1;
			}

			if (m_arcadeInputIndex == 4 )
			{
				m_game->snd_enter->play();

				name = characters[characterIndexes[0]] + characters[characterIndexes[1]] + characters[characterIndexes[2]];

				m_game->local_highscores->addItem(name, m_game->score);
				m_game->local_highscores->sort();
				m_game->local_highscores->save();

				m_game->state_viewhighscores->m_fromEndGame = true;

				SlideRectanglesAcrossTransition* t = new SlideRectanglesAcrossTransition(
					const_cast<Color*>(&Color::darker_grey), 50, Constants::DIRECTION_RIGHT, 1.0f
				);

				m_game->enterState(m_game->state_viewhighscores, t, NULL);
			}
		}

		//if (i->isKeyPressed(Input::KEY_BACKSPACE)// ) {
		if (i->isKeyPressed(DefaultGame::KEY_LEFT)
				//i->isKeyPressed(DefaultGame::KEY_BACK)
		//		|| i->isKeyPressed(DefaultGame::KEY_LEFT)
				) {

			m_arcadeInputIndex--;
			if (m_arcadeInputIndex < 0) {
				m_arcadeInputIndex = 0;
			}
			characterIndexes[m_arcadeInputIndex+1] = 0;
		}

	}
}

void WinState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	bg->draw(0, 0);

	g->setFont(m_game->font);

	if (m_win) {
		m_game->font->drawString("YOU WIN! ", 10, 12);
	} else {
		m_game->font->drawString("YOU FAIL! ", 10, 12);
	}

	string s = "FINAL SCORE: ";
	s.append(Cast::toString<int>(m_game->score));



	if (!DefaultGame::ARCADE_VERSION)
	{
		g->drawStringCenteredAt("ENTER YOUR NAME:", int(ORIGINAL_WIDTH/2),  int(ORIGINAL_HEIGHT/2) - 100);
		g->drawStringCenteredAt("THEN HIT THE ENTER KEY!", int(ORIGINAL_WIDTH/2),  int(ORIGINAL_HEIGHT/2) - 70);
		g->drawStringCenteredAt(s, int(ORIGINAL_WIDTH/2), int(ORIGINAL_HEIGHT/2));
		g->drawStringCenteredAt(name, int(ORIGINAL_WIDTH/2), int(ORIGINAL_HEIGHT/2) + 100);
		if (m_timer <= 0.5f) {
			//g->drawStringCenteredAt("_", int(container->getWidth()/2) + (m_game->font->getStringWidth(name)/2), int(container->getHeight()/2) + 100);
			g->drawString("_", int(ORIGINAL_WIDTH/2) + (m_game->font->getStringWidth(name)/2), int(ORIGINAL_HEIGHT/2) + 95);
		}
	}
	else
	{
		g->drawStringCenteredAt(s, int(ORIGINAL_WIDTH/2), int(ORIGINAL_HEIGHT/2) - 110);
		g->drawStringCenteredAt("ENTER YOUR NAME:", int(ORIGINAL_WIDTH/2),  int(ORIGINAL_HEIGHT/2) - 50);


		int centerX = ORIGINAL_WIDTH/2;
		int centerY = (ORIGINAL_HEIGHT/2) + 50;

		int cx1 = centerX - 207 + 50;
		int cx2 = centerX - 102 + 50;
		int cx3 = centerX + 3 + 50;
		//int cx4 = centerX + 108 + 50;

		g->setDrawColor(Color::black_50a);
		g->fillRoundedRect(centerX-207, centerY-50, 100, 100, 10, 20);
		g->fillRoundedRect(centerX-102, centerY-50, 100, 100, 10, 20);
		g->fillRoundedRect(centerX+3, centerY-50, 100, 100, 10, 20);
		g->fillRoundedRect(centerX+108, centerY-50, 100, 100, 10, 20);

		if (m_arcadeInputIndex == 0) {
			if (characterIndexes[0] != 1) {
				m_game->arrow_up->drawCentered(cx1, centerY - 65);
			}
			if (characterIndexes[0] != (signed int) characters.size()-1) {
				m_game->arrow_down->drawCentered(cx1, centerY + 65);
			}
		} else if (m_arcadeInputIndex == 1) {
			if (characterIndexes[1] != 1) {
				m_game->arrow_up->drawCentered(cx2, centerY - 65);
			}
			if (characterIndexes[1] != (signed int) characters.size()-1) {
				m_game->arrow_down->drawCentered(cx2, centerY + 65);
			}
		} else if (m_arcadeInputIndex == 2) {
			if (characterIndexes[2] != 1) {
				m_game->arrow_up->drawCentered(cx3, centerY - 65);
			}
			if (characterIndexes[2] != (signed int) characters.size()-1) {
				m_game->arrow_down->drawCentered(cx3, centerY + 65);
			}
		}

		g->setDrawColor(Color::white);
		g->pushMatrix();
		g->translate(centerX - 207 + 50, centerY);
		g->scale(2.0f, 2.0f);
		g->drawStringCenteredAt(characters[characterIndexes[0]], 0, 0);
		g->scale(0.5f, 0.5f);
		g->popMatrix();

		g->setDrawColor(Color::white);
		g->pushMatrix();
		g->translate(centerX - 102 + 50, centerY);
		g->scale(2.0f, 2.0f);
		g->drawStringCenteredAt(characters[characterIndexes[1]], 0, 0);
		g->scale(0.5f, 0.5f);
		g->popMatrix();

		g->setDrawColor(Color::white);
		g->pushMatrix();
		g->translate(centerX + 3 + 50, centerY);
		g->scale(2.0f, 2.0f);
		g->drawStringCenteredAt(characters[characterIndexes[2]], 0, 0);
		g->scale(0.5f, 0.5f);
		g->popMatrix();

		if (m_arcadeInputIndex == 3) {
			g->setDrawColor(Color::white);
			g->pushMatrix();
			g->translate(centerX + 108 + 50, centerY);
			g->drawStringCenteredAt("END", 0, 0);
			g->popMatrix();
		}
	}

}

void WinState::keyPressed(unsigned int key) {

	Input* i = ARK2D::getInput();
	//std::cout << key << " : " << i->getKeyChar(key) << " : " << i->getKeyName(key) << std::endl;

	if (!DefaultGame::ARCADE_VERSION) {


		if ((key >= 48 && key <= 90) || key == 32) {
			m_game->snd_keypress->play();
			name.append(i->getKeyChar(key));
		}
		if (key == 8) {
			m_game->snd_keypress->play();
			name = name.substr(0, name.size()-1);
		}
		if (key == 13) {
			m_game->snd_enter->play();

			m_game->local_highscores->addItem(name, m_game->score);
			m_game->local_highscores->sort();
			m_game->local_highscores->save();

			m_game->state_viewhighscores->m_fromEndGame = true;

			SlideRectanglesAcrossTransition* t = new SlideRectanglesAcrossTransition(
				const_cast<Color*>(&Color::darker_grey), 50, Constants::DIRECTION_RIGHT, 1.0f
			);

			m_game->enterState(m_game->state_viewhighscores, t, NULL);
		}
	}

	//std::cout << "key pressed: " << i->getKeyName(key) << std::endl;
}
void WinState::keyReleased(unsigned int key) {

}

WinState::~WinState() {

}

