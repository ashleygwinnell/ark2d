/*
 * SlideRectanglesAcrossTransition.h
 *
 *  Created on: 13 Nov 2010
 *      Author: Ashley
 */

#ifndef ARK_SLIDERECTANGLESACROSSTRANSITION_H_
#define ARK_SLIDERECTANGLESACROSSTRANSITION_H_

#include "Transition.h"

#include <vector>
using std::vector;

namespace ARK {
    namespace Core {
        namespace State {
            namespace Transition {

                class SlideRect { public:
                    int xoffset;
                    float xoffsetSpeed;
                };

                /*!
                 * \brief Slide rectangles across the screen like in indie-hit VVVVVV.
                 *
                 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
                 * @todo Introduce a Colour set instead of a single colour?
                 * @todo Implement sliding rectangles DOWN the screen.
                 */
                class ARK2D_API SlideRectanglesAcrossTransition: public Transition {
                    public:
                        Color* m_color; // the colour for the overlay

                        unsigned int m_numrects;

                        vector<SlideRect> m_sliderects;

                        float m_time;
                        float m_current_time;
                        unsigned int m_dir;

                        bool m_doneHalfWayLogic;

                        static const unsigned int DIRECTION_UP = 0;
                        static const unsigned int DIRECTION_DOWN = 1;
                        static const unsigned int DIRECTION_LEFT = 2;
                        static const unsigned int DIRECTION_RIGHT = 3;

                        SlideRectanglesAcrossTransition(Color* color, unsigned int numrects, unsigned int dir, float time);
                        virtual void init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to);
                        virtual void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
                        virtual void halfwayLogic(GameContainer* container, StateBasedGame* game);
                        virtual void preRender(GameContainer* container, StateBasedGame* game, Renderer* g);
                        virtual void postRender(GameContainer* container, StateBasedGame* game, Renderer* g);
                        virtual bool isComplete();
                        virtual ~SlideRectanglesAcrossTransition();
                };
            }
		}
	}
}

#endif /* SLIDERECTANGLESACROSSTRANSITION_H_ */
