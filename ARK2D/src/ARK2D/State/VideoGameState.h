/*
 * VIDEOGameState.h
 *
 *  Created on: Oct 8, 2013
 *      Author: ashleygwinnell
 */

#ifndef VIDEOGAMESTATE_H_
#define VIDEOGAMESTATE_H_

#include <map>
#include <string>
#include "GameState.h"

#include "../Includes.h"
#include "../Namespaces.h"
#include "../Util/Containers/Vector.h"

using namespace std;


#ifdef ARK2D_MACINTOSH

@interface QTMovieView (TFOverrideDrag)

- (void)mouseDown:(NSEvent *)theEvent;
- (void)mouseDragged:(NSEvent *)theEvent;
- (void)rightMouseDown:(NSEvent *)theEvent;
- (void)keyDown:(NSEvent *)theEvent;
- (void)keyUp:(NSEvent *)theEvent;

@end

#endif


namespace ARK {
	namespace State {

		/*!
		 * \brief This game state plays video... 
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API VideoGameState : public GameState {
			private:

				string m_source;
				bool m_loaded;
				
				#ifdef ARK2D_MACINTOSH

					QTMovie* m_movie;
					QTMovieView* m_movieView;
					

				#endif

			public:
				VideoGameState();

				virtual void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				virtual void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				virtual unsigned int id() = 0;
				virtual void init(GameContainer* container, StateBasedGame* game);
				virtual void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				virtual void render(GameContainer* container, StateBasedGame* game, Renderer* g);

				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);

				virtual void setSource(string s);
				virtual string getSource();

				virtual void load();
				virtual void unload();

				virtual void play();
				//virtual void pause();
				virtual void stop();
				
				virtual float getCurrentTime();
				virtual void setCurrentTime(float pos);

				virtual float getVolume();
				virtual void setVolume(float f);

				virtual bool isCompleted();

				virtual ~VideoGameState();
		};
	}
}

#endif /* VIDEOGAMESTATE_H_ */


