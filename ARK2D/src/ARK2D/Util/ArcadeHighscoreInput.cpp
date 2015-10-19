/*
 * ArcadeHighscoreInput.cpp
 *
 *  Created on: Mar 16, 2012
 *      Author: ashleygwinnell
 */

#include "ArcadeHighscoreInput.h"

#include "../Core/GameContainer.h"

namespace ARK {
	namespace Util { 

		ArcadeHighscoreInput* ArcadeHighscoreInput::s_current = NULL;
		void ArcadeHighscoreInput::setCurrent(ArcadeHighscoreInput* hsi) {
			s_current = hsi;
		}

		ArcadeHighscoreInput::ArcadeHighscoreInput():
			m_arcadeInputIndex(0), name(), characterIndexes(), characters(),
			m_buttonsUp(),
			m_buttonsDown(),
			m_buttonLetterEnd(NULL),
			letterChangedFunction(NULL),
			additionalEndFunction(NULL),
			m_backgroundColor(NULL)
			{
			GameContainer* container = ARK2D::getContainer();
			int centerX = container->getWidth()/2;
			int centerY = container->getHeight()/2;

			if (container->isTouchMode()) {
				centerX += 50;
				centerY += 50;
			}

			int amount = 3;
			vector<int> offsetsX;
			offsetsX.push_back(-207);
			offsetsX.push_back(-102);
			offsetsX.push_back(3);
			for(int i = 0; i < amount; i++) {
				ARK::UI::Button* thisUp = new ARK::UI::Button();
				thisUp->setText("/\\");
				thisUp->setSize(100,100);
				thisUp->setLocation(centerX + offsetsX.at(i), centerY - 160);
				thisUp->setEvent((void*) &btnClickUp);
				m_buttonsUp.push_back(thisUp);

				ARK::UI::Button* thisDown = new ARK::UI::Button();
				thisDown->setText("\\/");
				thisDown->setSize(100,100);
				thisDown->setLocation(centerX + offsetsX.at(i), centerY + 60);
				thisDown->setEvent((void*) &btnClickDown);
				m_buttonsDown.push_back(thisDown);
			}

			m_buttonLetterEnd = new ARK::UI::Button();
			m_buttonLetterEnd->setText("END");
			m_buttonLetterEnd->setSize(150,120);
			//m_buttonLetterEnd->setLocation(centerX+108, centerY-50);
			m_buttonLetterEnd->setLocation(container->getWidth()- 160, container->getHeight() - 130);
			m_buttonLetterEnd->setEvent((void*) &doEnd);

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

			if (ARK2D::getContainer()->isTouchMode()) {
				characterIndexes.push_back(1);
				characterIndexes.push_back(1);
				characterIndexes.push_back(1);
			} else {
				characterIndexes.push_back(1);
				characterIndexes.push_back(0);
				characterIndexes.push_back(0);
			}

			m_backgroundColor = new Color(0.0f, 0.0f, 0.0f, 0.5f);

			m_score = 0;

		}

		void ArcadeHighscoreInput::reset() {
			m_arcadeInputIndex = 0;
			name = "";
			if (ARK2D::getContainer()->isTouchMode()) {
				characterIndexes[0] = 1;
				characterIndexes[1] = 1;
				characterIndexes[2] = 1;
			} else{
				characterIndexes[0] = 1;
				characterIndexes[1] = 0;
				characterIndexes[2] = 0;
			}
		}



		void ArcadeHighscoreInput::update() {
			GameContainer* container = ARK2D::getContainer();
			if (container->isTouchMode()) {

			} else {
				Input* i = ARK2D::getInput();
				if (i->isKeyPressed(Input::KEY_UP)) {
					goUp(m_arcadeInputIndex);
					if (letterChangedFunction != NULL) {
						void (*pt)() = (void(*)()) letterChangedFunction;
						pt();
					}

				} else if (i->isKeyPressed(Input::KEY_DOWN)) {
					goDown(m_arcadeInputIndex);

					if (letterChangedFunction != NULL) {
						void (*pt)() = (void(*)()) letterChangedFunction;
						pt();
					}
				}

				if (i->isKeyPressed(Input::KEY_RIGHT) || i->isKeyPressed(Input::KEY_ENTER)
						){
					m_arcadeInputIndex++;

					if (m_arcadeInputIndex == 4 && i->isKeyPressed(Input::KEY_RIGHT)) {
						m_arcadeInputIndex--;
						return;
					}

					if (characterIndexes[m_arcadeInputIndex] == 0) {
						characterIndexes[m_arcadeInputIndex] = 1;
					}

					if (m_arcadeInputIndex == 4)
					{
						m_buttonLetterEnd->doEvent();
					}
				}

				if (i->isKeyPressed(Input::KEY_LEFT)) {

					m_arcadeInputIndex--;
					if (m_arcadeInputIndex < 0) {
						m_arcadeInputIndex = 0;
					}
					characterIndexes[m_arcadeInputIndex+1] = 0;
				}
			}
		}
		void ArcadeHighscoreInput::render() {
			GameContainer* container = ARK2D::getContainer();
			Renderer* g = ARK2D::getRenderer();
			int centerX = container->getWidth()/2;
			int centerY = (container->getHeight()/2);

			if (container->isTouchMode()) {
				centerX += 50;
				centerY += 50;
			}

			int cx1 = centerX - 207 + 50;
			int cx2 = centerX - 102 + 50;
			int cx3 = centerX + 3 + 50;
			//int cx4 = centerX + 108 + 50;

			g->setDrawColor(m_backgroundColor->getRed(), m_backgroundColor->getGreen(), m_backgroundColor->getBlue(), m_backgroundColor->getAlpha());
			g->fillRoundedRect(centerX-207, centerY-50, 100, 100, 10, 20);
			g->fillRoundedRect(centerX-102, centerY-50, 100, 100, 10, 20);
			g->fillRoundedRect(centerX+3, centerY-50, 100, 100, 10, 20);
			if (!container->isTouchMode()) g->fillRoundedRect(centerX+108, centerY-50, 100, 100, 10, 20);

			if (!container->isTouchMode()) {
				if (m_arcadeInputIndex == 0) {
					if (characterIndexes[0] != 1) {
						//m_game->arrow_up->drawCentered(cx1, centerY - 65);
						g->fillCircle(cx1,centerY - 65, 10, 15);
					}
					if (characterIndexes[0] != (signed int) characters.size()-1) {
						//m_game->arrow_down->drawCentered(cx1, centerY + 65);
						g->fillCircle(cx1,centerY + 65, 10, 15);
					}
				} else if (m_arcadeInputIndex == 1) {
					if (characterIndexes[1] != 1) {
						g->fillCircle(cx2,centerY - 65, 10, 15);
						//m_game->arrow_up->drawCentered(cx2, centerY - 65);
					}
					if (characterIndexes[1] != (signed int) characters.size()-1) {
						//m_game->arrow_down->drawCentered(cx2, centerY + 65);
						g->fillCircle(cx2,centerY + 65, 10, 15);
					}
				} else if (m_arcadeInputIndex == 2) {
					if (characterIndexes[2] != 1) {
						//m_game->arrow_up->drawCentered(cx3, centerY - 65);
						g->fillCircle(cx3,centerY - 65, 10, 15);
					}
					if (characterIndexes[2] != (signed int) characters.size()-1) {
						//m_game->arrow_down->drawCentered(cx3, centerY + 65);
						g->fillCircle(cx3,centerY + 65, 10, 15);
					}
				}
			}

			g->setDrawColor(Color::white);
			g->pushMatrix();
			g->translate(centerX - 207 + 52, centerY-4);
			g->scale(2.0f, 2.0f);
			g->drawStringCenteredAt(characters[characterIndexes[0]], 0, 0);
			g->scale(0.5f, 0.5f);
			g->popMatrix();

			g->setDrawColor(Color::white);
			g->pushMatrix();
			g->translate(centerX - 102 + 52, centerY-4);
			g->scale(2.0f, 2.0f);
			g->drawStringCenteredAt(characters[characterIndexes[1]], 0, 0);
			g->scale(0.5f, 0.5f);
			g->popMatrix();

			g->setDrawColor(Color::white);
			g->pushMatrix();
			g->translate(centerX + 3 + 52, centerY-4);
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

			if (container->isTouchMode()) {
				for(unsigned int i = 0; i < m_buttonsUp.size(); i++) { m_buttonsUp.at(i)->render(); }
				for(unsigned int i = 0; i < m_buttonsDown.size(); i++) { m_buttonsDown.at(i)->render(); }
				m_buttonLetterEnd->render();
			}
		}
		void ArcadeHighscoreInput::init(LocalHighscores* highscores, void* letterChangedF=NULL, void* additionalEndF=NULL) {
			m_highscores = highscores;
			letterChangedFunction = letterChangedF;
			additionalEndFunction = additionalEndF;
		}
		void ArcadeHighscoreInput::setScore(int score) {
			m_score = score;
		}

		void ArcadeHighscoreInput::goUp(int index) {
			characterIndexes[index]--;
			if (characterIndexes[index] < 1) {
				characterIndexes[index] = 1;
			}
		}
		void ArcadeHighscoreInput::goDown(int index) {
			characterIndexes[index]++;
			if ((unsigned int) characterIndexes[index] > characters.size()-1) {
				characterIndexes[index] = characters.size()-1;
			}
		}
		void ArcadeHighscoreInput::doEnd() {
			s_current->name = s_current->characters[s_current->characterIndexes[0]] + s_current->characters[s_current->characterIndexes[1]] + s_current->characters[s_current->characterIndexes[2]];

			s_current->m_highscores->addItem(s_current->name, s_current->m_score);
			s_current->m_highscores->sort();
			s_current->m_highscores->save();

			if (s_current->additionalEndFunction != NULL) {
				void (*pt)() = (void(*)()) s_current->additionalEndFunction;
				pt();
			}
		}
		void ArcadeHighscoreInput::btnClickUp() {
			for(unsigned int i = 0; i < s_current->m_buttonsUp.size(); i++) {
				if (UIComponent::s_currentFocus == s_current->m_buttonsUp.at(i)) { s_current->goUp(i); break; }
			}
		}
		void ArcadeHighscoreInput::btnClickDown() {
			for(unsigned int i = 0; i < s_current->m_buttonsDown.size(); i++) {
				if (UIComponent::s_currentFocus == s_current->m_buttonsDown.at(i)) { s_current->goDown(i); break; }
			}
		}

		void ArcadeHighscoreInput::keyPressed(unsigned int key) {
			if (ARK2D::getContainer()->isTouchMode()) {
				for(unsigned int i = 0; i < m_buttonsUp.size(); i++) { m_buttonsUp.at(i)->keyPressed(key); }
				for(unsigned int i = 0; i < m_buttonsDown.size(); i++) { m_buttonsDown.at(i)->keyPressed(key); }
				m_buttonLetterEnd->keyPressed(key);
			}
		}
		void ArcadeHighscoreInput::keyReleased(unsigned int key) {
			if (ARK2D::getContainer()->isTouchMode()) {
				for(unsigned int i = 0; i < m_buttonsUp.size(); i++) { m_buttonsUp.at(i)->keyReleased(key); }
				for(unsigned int i = 0; i < m_buttonsDown.size(); i++) { m_buttonsDown.at(i)->keyReleased(key); }
				m_buttonLetterEnd->keyReleased(key);
			}
		}
		void ArcadeHighscoreInput::mouseMoved(int x, int y, int oldx, int oldy) {
			if (ARK2D::getContainer()->isTouchMode()) {
				for(unsigned int i = 0; i < m_buttonsUp.size(); i++) { m_buttonsUp.at(i)->mouseMoved(x,y,oldx,oldy); }
				for(unsigned int i = 0; i < m_buttonsDown.size(); i++) { m_buttonsDown.at(i)->mouseMoved(x,y,oldx,oldy); }
				m_buttonLetterEnd->mouseMoved(x,y,oldx,oldy);
			}
		}

		ArcadeHighscoreInput::~ArcadeHighscoreInput() {
			delete m_backgroundColor;
		}
	}
}
