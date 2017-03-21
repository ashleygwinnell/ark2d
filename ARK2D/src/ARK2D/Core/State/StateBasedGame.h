/*
 * StateBasedGame.h
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#ifndef ARK_CORE_STATE_STATEBASEDGAME_H_
#define ARK_CORE_STATE_STATEBASEDGAME_H_

#include "../../Namespaces.h"
#include "../Game.h"
#include "../GameContainer.h"
#include "../GameTimer.h"
#include "../Graphics/Renderer.h"

#include <vector>
#include <string>
using namespace std;

namespace ARK {
    namespace Core {
        namespace State {

            /*!
             * \brief Games with multiple game states should inherit from this class for easy management and transition effects between states.
             *
             * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
             * @todo continue updating during transitions.
             */
            class ARK2D_API StateBasedGame : public ARK::Core::Game {
                public:
                    StateBasedGame(string title);
                    string getTitle();

                    void addState(GameState* state);
                    void enterState(unsigned int id);
                    void enterState(GameState* state);

                    void setLoadingState(LoadingState* state);
                    LoadingState* getLoadingState();

                    void setFromState(GameState* state);
                    void setCurrentState(GameState* state);
                    void setCurrentState(unsigned int id);
                    GameState* getCurrentState();
                    unsigned int getCurrentStateID();

                    vector<GameState*> getStates();
                    GameState* getState(unsigned int id);

                    void enterState(unsigned int id, ARK::Core::State::Transition::Transition* leave, ARK::Core::State::Transition::Transition* enter);
                    void enterState(GameState* state, ARK::Core::State::Transition::Transition* leave, ARK::Core::State::Transition::Transition* enter);

                    void init(GameContainer* container);

                    virtual void preUpdate(GameContainer* container, GameTimer* timer);
                    virtual void update(GameContainer* container, GameTimer* timer);
                    virtual void postUpdate(GameContainer* container, GameTimer* timer);

                    virtual void preRender(GameContainer* container, Renderer* g);
                    virtual void preRenderTransitions();
                    virtual void render(GameContainer* container, Renderer* g);
                    virtual void postRenderTransitions();
                    virtual void postRender(GameContainer* container, Renderer* g);

                    // SceneNode overrides
                    virtual void preRender();
                    virtual void postRender();
                    virtual void render();


                    GameContainer* getContainer();

                    bool isTransitioning();
                    bool isInitialised();

                    virtual void initStates(GameContainer* container) = 0;
                    virtual void resize(GameContainer* container, int width, int height);

                    virtual void pause(); // android events
                    virtual void resume();

                    virtual bool keyPressed(unsigned int key);
                    virtual bool keyReleased(unsigned int key);
                    virtual bool mouseMoved(int x, int y, int oldx, int oldy);

                    // Gamepad listener
                    virtual void gamepadConnected(ARK::Core::Controls::Gamepad* gamepad);
                    virtual void gamepadDisconnected(ARK::Core::Controls::Gamepad* gamepad);
                    virtual void buttonPressed(ARK::Core::Controls::Gamepad* gamepad, unsigned int button);
                    virtual void buttonReleased(ARK::Core::Controls::Gamepad* gamepad, unsigned int button);
                    virtual void axisMoved(ARK::Core::Controls::Gamepad* gamepad, unsigned int axis, float value);

                    virtual void orientationChanged(int orientation);

                    string toString();

                    virtual ~StateBasedGame();

                protected:
                    vector<GameState*> m_states;
                    GameState* m_from_state;
                    GameState* m_current_state;
                    GameState* m_next_state;
                    GameContainer* m_container;

                    LoadingState* m_loading_state;

                    ARK::Core::State::Transition::Transition* m_enterTransition;
                    ARK::Core::State::Transition::Transition* m_leaveTransition;

                    bool m_initialised;
                    bool m_autoDeleteTransitions;

                protected:
                    bool isDuringTransition();

            };
        }
	}
}

#endif
