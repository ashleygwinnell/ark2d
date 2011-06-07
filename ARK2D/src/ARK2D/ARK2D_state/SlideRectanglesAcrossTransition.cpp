/*
 * SlideRectanglesAcrossTransition.cpp
 *
 *  Created on: 13 Nov 2010
 *      Author: Ashley
 */

#include "SlideRectanglesAcrossTransition.h"

SlideRectanglesAcrossTransition::SlideRectanglesAcrossTransition(Color* color, unsigned int numrects, unsigned int dir, float time):
	Transition(), m_color(color),
	m_numrects(numrects),
	m_sliderects(),
	m_time(time), m_current_time(0), m_dir(dir),
	m_doneHalfWayLogic(false) {

}

void SlideRectanglesAcrossTransition::init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) {

	Transition::init(container, game, from, to);
	m_easing = Easing::LINEAR;

	//if (m_dir == Constants::DIRECTION_RIGHT) {

		int minspeed = (int) (float(container->getWidth()) / (m_time / 2)) ;
		for (unsigned int i = 0; i < m_numrects; i++) {

			SlideRect r;
			r.xoffset = 0;
			r.xoffsetSpeed = minspeed + (rand() % int(1.5*minspeed));
			m_sliderects.push_back(r);

		}

	//}

}

void SlideRectanglesAcrossTransition::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

	m_current_time += timer->getDelta();

	//if (m_dir == Constants::DIRECTION_RIGHT) {
	if (m_current_time < m_time/2) {
		for (unsigned int i = 0; i < m_numrects; i++) {
			m_sliderects[i].xoffset += (int) (m_sliderects[i].xoffsetSpeed * timer->getDelta());
		}
	} else {

		if (!m_doneHalfWayLogic) {
			halfwayLogic(container, game);
			m_doneHalfWayLogic = true;
		}

		for (unsigned int i = 0; i < m_numrects; i++) {
			m_sliderects[i].xoffset -= (int) (m_sliderects[i].xoffsetSpeed * timer->getDelta());
		}
	}

	//} else if (m_dir == Constants::DIRECTION_LEFT) {

	//}

}


void SlideRectanglesAcrossTransition::halfwayLogic(GameContainer* container, StateBasedGame* game) {
	std::random_shuffle(m_sliderects.begin(), m_sliderects.end());
}

void SlideRectanglesAcrossTransition::preRender(GameContainer* container, StateBasedGame* game, Graphics* g) {

}

void SlideRectanglesAcrossTransition::postRender(GameContainer* container, StateBasedGame* game, Graphics* g) {

	//m_from->render(container, game, g);

	unsigned int current_y = 0;
	unsigned int single_height = (int) ceil( float(container->getHeight()) / float(m_numrects) );

	if (m_dir == Constants::DIRECTION_RIGHT) {

		// play first half
		if (m_current_time < m_time/2) {

			m_from->render(container, game, g);

			g->setDrawColor((*m_color));
			for (unsigned int i = 0; i < m_numrects; i++) {
				g->fillRect(0, current_y, m_sliderects.at(i).xoffset, single_height);
				current_y += single_height;
			}

		}

		// play second half
		else {

			m_to->render(container, game, g);

			g->setDrawColor((*m_color));
			for (unsigned int i = 0; i < m_numrects; i++) {
				g->fillRect(container->getWidth(), current_y, m_sliderects.at(i).xoffset * -1, single_height);
				current_y += single_height;
			}

		}



	} else if (m_dir == Constants::DIRECTION_LEFT) {


		// play first half
		if (m_current_time < m_time/2) {

			m_from->render(container, game, g);

			g->setDrawColor((*m_color));
			for (unsigned int i = 0; i < m_numrects; i++) {
				g->fillRect(container->getWidth(), current_y, m_sliderects.at(i).xoffset * -1, single_height);
				current_y += single_height;
			}

		}

		// play 2nd half
		else {


			m_to->render(container, game, g);

			g->setDrawColor((*m_color));
			for (unsigned int i = 0; i < m_numrects; i++) {
				g->fillRect(0, current_y, m_sliderects.at(i).xoffset, single_height);
				current_y += single_height;
			}

		}

	}

}

bool SlideRectanglesAcrossTransition::isComplete() {
	if (m_current_time >= m_time) {
		return true;
	}
	return false;
}

SlideRectanglesAcrossTransition::~SlideRectanglesAcrossTransition() {

}
