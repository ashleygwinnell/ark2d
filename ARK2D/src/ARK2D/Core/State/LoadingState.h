/*
 * LoadingState.h
 *
 *  Created on: 1 Apr 2011
 *      Author: Ashley
 */

#ifndef ARK_CORE_STATE_LOADINGSTATE_H_
#define ARK_CORE_STATE_LOADINGSTATE_H_

#include "../../Namespaces.h"
#include "../../Common/DLL.h"
#include "GameState.h"

namespace ARK {
    namespace Core {
        namespace State {

            /*!
             * \brief This state contains thread information and state for you to multithread loading in your game.
             *
             * @warning Note that you CANNOT load Images in this separate thread because OpenGL fails.
             * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
             */
            class ARK2D_API LoadingState : public GameState {
                public:
                    LoadingState();

                    static void load();

                    virtual bool isLoading();
                    virtual void setLoading(bool b);

                    ARK::Core::Threading::Thread* getThread();

                    virtual void enter(GameContainer* container, StateBasedGame* game, GameState* from);
                    virtual void leave(GameContainer* container, StateBasedGame* game, GameState* to);

                    virtual unsigned int id() = 0;
                    virtual void init(GameContainer* container, StateBasedGame* game);
                    virtual void update(GameContainer* container, StateBasedGame* game, GameTimer* timer) = 0;
                    virtual void render(GameContainer* container, StateBasedGame* game, Renderer* g) = 0;

                    virtual ~LoadingState();

                protected:
                    ARK::Core::Threading::Thread* m_thread;
                    bool m_loading;

            };
        }
	}
}

#endif /* LOADINGSTATE_H_ */
