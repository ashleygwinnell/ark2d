/*
 * IntelligentGameState.h
 *
 *  Created on: Dec 9, 2011
 *      Author: ashleygwinnell
 */

#ifndef ARK_CORE_STATE_INTELLIGENTGAMESTATE_H_
#define ARK_CORE_STATE_INTELLIGENTGAMESTATE_H_

#include <map>
#include <string>
#include "GameState.h"

#include "../../Namespaces.h"
#include "../../Common/DLL.h"
#include "../Vector.h"

using namespace std;

namespace ARK {
    namespace Core {
        namespace State {

            /*!
             * \brief This game state holds, updates and renders entities intelligently using ARKGameObject properties.
             *
             * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
             */
            class ARK2D_API IntelligentGameState : public GameState {
                private:

                    map<string, Vector<GameObject*> > m_entities;

                public:
                    IntelligentGameState();

                    void add(string group, GameObject* entity);
                    void pruneEntities();

                    virtual void enter(GameContainer* container, StateBasedGame* game, GameState* from);
                    virtual void leave(GameContainer* container, StateBasedGame* game, GameState* to);

                    virtual unsigned int id() = 0;
                    virtual void init(GameContainer* container, StateBasedGame* game);
                    virtual void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
                    virtual void render(GameContainer* container, StateBasedGame* game, Renderer* g);

                    virtual bool keyPressed(unsigned int key);
                    virtual bool keyReleased(unsigned int key);
                    virtual bool mouseMoved(int x, int y, int oldx, int oldy);

                    virtual ~IntelligentGameState();
            };
        }
	}
}

#endif /* ARK_CORE_STATE_INTELLIGENTGAMESTATE_H_ */
