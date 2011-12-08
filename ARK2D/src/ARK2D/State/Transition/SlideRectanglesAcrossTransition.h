/*
 * SlideRectanglesAcrossTransition.h
 *
 *  Created on: 13 Nov 2010
 *      Author: Ashley
 */

#ifndef SLIDERECTANGLESACROSSTRANSITION_H_
#define SLIDERECTANGLESACROSSTRANSITION_H_


#include "Transition.h"
#include "../../Color.h"
#include "../../Timeline.h"
#include "../../OutputWrapper.h"
#include "../../Constants.h"

#include <vector>
using std::vector;

class SlideRect { public:
	int xoffset;
	float xoffsetSpeed;
};

class SlideRectanglesAcrossTransition: public Transition {
	public:
		Color* m_color; // the colour for the overlay

		unsigned int m_numrects;

		vector<SlideRect> m_sliderects;

		float m_time;
		float m_current_time;
		unsigned int m_dir;

		bool m_doneHalfWayLogic;

		SlideRectanglesAcrossTransition(Color* color, unsigned int numrects, unsigned int dir, float time);
		virtual void init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to);
		virtual void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		virtual void halfwayLogic(GameContainer* container, StateBasedGame* game);
		virtual void preRender(GameContainer* container, StateBasedGame* game, Graphics* g);
		virtual void postRender(GameContainer* container, StateBasedGame* game, Graphics* g);
		virtual bool isComplete();
		virtual ~SlideRectanglesAcrossTransition();
};

#endif /* SLIDERECTANGLESACROSSTRANSITION_H_ */
