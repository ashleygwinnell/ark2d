/*
 * EmptyTransition.h
 *
 *  Created on: 1 Aug 2010
 *      Author: Ashley
 */

#ifndef EMPTYTRANSITION_H_
#define EMPTYTRANSITION_H_

#include "../../ARK2D.h"
#include "Transition.h"
#include "../../Tween/Timeline.h"

namespace ARK {
    namespace Core {
        namespace State {
            namespace Transition {

                /*!
                 * \brief An empty transition that implements the Transition interface. This is used internally when a NULL transition is specified.
                 *
                 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
                 */
                class ARK2D_API EmptyTransition : public Transition {
                    public:
                        EmptyTransition();
                        void init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to);
                        void update(GameContainer* container, StateBasedGame* game, GameTimer* delta);
                        void preRender(GameContainer* container, StateBasedGame* game, Renderer* g);
                        void postRender(GameContainer* container, StateBasedGame* game, Renderer* g);
                        bool isComplete();
                        virtual ~EmptyTransition();
                };
            }
        }
    }
}

#endif /* EMPTYTRANSITION_H_ */
