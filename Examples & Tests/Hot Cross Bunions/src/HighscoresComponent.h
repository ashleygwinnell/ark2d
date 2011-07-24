/*
 * HighscoresComponent.h
 *
 *  Created on: 11 Jun 2011
 *      Author: Ashley
 */

#ifndef HIGHSCORESCOMPONENT_H_
#define HIGHSCORESCOMPONENT_H_

class HighscoresComponent {
	public:
		int y;
		HighscoresComponent();
		void update(float t);
		void render();
		virtual ~HighscoresComponent();
};

#endif /* HIGHSCORESCOMPONENT_H_ */
