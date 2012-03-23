/*
 * ArcadeHighscoreInput.h
 *
 *  Created on: Mar 16, 2012
 *      Author: ashleygwinnell
 */

#ifndef ArcadeHighscoreInput_H_
#define ArcadeHighscoreInput_H_

#include "../ARK2D.h"
#include "../UI/Button.h"
#include "LocalHighscores.h"

namespace ARK {
	namespace Util {
		class ArcadeHighscoreInput {
			public:

				signed int m_arcadeInputIndex;
				string name;
				vector<signed int> characterIndexes;
				vector<string> characters;

				vector<Button*> m_buttonsUp;
				vector<Button*> m_buttonsDown;
				Button* m_buttonLetterEnd;

				void* letterChangedFunction;
				void* additionalEndFunction;

				Color* m_backgroundColor;

				LocalHighscores* m_highscores;
				int m_score;

				ArcadeHighscoreInput();
				void init(LocalHighscores* highscores, void* letterChangedF, void* additionalEndF);
				void setScore(int score);
				void reset();
				void update();
				void render();
				virtual ~ArcadeHighscoreInput();

				void goUp(int index);
				void goDown(int index);
				static void doEnd();

				static void btnClickUp();
				static void btnClickDown();

				void keyPressed(unsigned int key);
				void keyReleased(unsigned int key);
				void mouseMoved(int x, int y, int oldx, int oldy);

				static ArcadeHighscoreInput* s_current;
				static void setCurrent(ArcadeHighscoreInput* hsi);
		};
	}
}

#endif /* ArcadeHighscoreInput_H_ */
