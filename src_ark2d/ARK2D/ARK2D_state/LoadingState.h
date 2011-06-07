/*
 * LoadingState.h
 *
 *  Created on: 1 Apr 2011
 *      Author: Ashley
 */

#ifndef LOADINGSTATE_H_
#define LOADINGSTATE_H_

#include "GameState.h"
class ARKThread;
class GameContainer;
class StateBasedGame;
class GameState;

class LoadingState : public GameState {
	public:
		LoadingState();

		static void load();

		virtual bool isLoading();
		virtual void setLoading(bool b);

		ARKThread* getThread();

		virtual void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		virtual void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		virtual unsigned int id() = 0;
		virtual void init(GameContainer* container, StateBasedGame* game);
		virtual void update(GameContainer* container, StateBasedGame* game, GameTimer* timer) = 0;
		virtual void render(GameContainer* container, StateBasedGame* game, Graphics* g) = 0;

		virtual ~LoadingState();

	protected:
		ARKThread* m_thread;
		bool m_loading;

};

#endif /* LOADINGSTATE_H_ */
